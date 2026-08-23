package kvraft

import (
	"bytes"
	"log"
	"sync"
	"sync/atomic"
	"time"

	"6.824/labgob"
	"6.824/labrpc"
	"6.824/raft-ok"
)


// 3A 该op用于发给服务端进行数据库请求执行
type Op struct {
	Key       string
	Value     string
	Operation string //put or get
	MsgId     int64  //当前封装的来自clerk的指令消息号
	ReqId     int64  //标识当前Op的消息号
	ClientId  int64
}

// 3A 结构体，用来存储每个消息指令的返回值
type NotifyMsg struct {
	Err   Err
	Value string
}

type KVServer struct {
	mu           sync.Mutex
	me           int
	rf           *raft.Raft
	applyCh      chan raft.ApplyMsg //提交通道
	dead         int32              // set by Kill()
	maxraftstate int                // snapshot if log grows this big
	//3A 部分
	msgNotify   map[int64]chan NotifyMsg //管道map：在get/append用来接收raft集群的处理结果。
	data        map[string]string        //数据存储空间:存储键值对
	lastApplies map[int64]int64          //记录每个节点上次提交的日志，查重用以避免重复执行
	persister   *raft.Persister          //restart的测试任务
}

const WaitTimeOut = time.Millisecond * 500

// 3A
func (kv *KVServer) Get(args *GetArgs, reply *GetReply) {
	//收到get请求，则调用start交给raft集群达成共识。start包含检查自身是否leader的代码
	op := Op{MsgId: args.MsgId, ReqId: nrand(), Key: args.Key, Operation: GET, ClientId: args.ClientId}
	var res NotifyMsg
	_, isLeader := kv.rf.GetState()
	if !isLeader { //不是leader直接返回
		res.Err = ErrWrongLeader
	} else {
		kv.rf.Start(op) //交给start进行一致性共识
		ch := make(chan NotifyMsg, 1)
		kv.mu.Lock()
		kv.msgNotify[op.ReqId] = ch //将管道存放到map中，当start处理结束后，其他进程会存放结果到该管道中
		kv.mu.Unlock()
		t := time.NewTimer(WaitTimeOut) //定时器，避免超时
		defer t.Stop()
		select {
		case res = <-ch: //处理结束后，另一个进程会将结果存放到ch中。
			kv.mu.Lock()
			delete(kv.msgNotify, op.MsgId)
			kv.mu.Unlock()
		case <-t.C: //超时，删除管道。
			kv.mu.Lock()
			delete(kv.msgNotify, op.MsgId)
			kv.mu.Unlock()
			res.Err = ErrTimeOut
		}
	}
	reply.Err = res.Err
	reply.Value = res.Value
}

// 3A
func (kv *KVServer) PutAppend(args *PutAppendArgs, reply *PutAppendReply) {
	op := Op{MsgId: args.MsgId, ReqId: nrand(), Key: args.Key, Value: args.Value, Operation: args.Op, ClientId: args.ClientId}
	var res NotifyMsg
	_, isLeader := kv.rf.GetState()
	if !isLeader { //不是leader直接返回
		res.Err = ErrWrongLeader
	} else {
		kv.rf.Start(op)
		ch := make(chan NotifyMsg, 1)
		kv.mu.Lock()
		kv.msgNotify[op.ReqId] = ch //管道等待一致性结果，如果有调用成功则将结果放在ch里
		kv.mu.Unlock()
		t := time.NewTimer(WaitTimeOut)
		defer t.Stop()
		select {
		case res = <-ch:
			kv.mu.Lock()
			delete(kv.msgNotify, op.ReqId)
			kv.mu.Unlock()
		case <-t.C: //超时，同样
			kv.mu.Lock()
			delete(kv.msgNotify, op.ReqId)
			kv.mu.Unlock()
			res.Err = ErrTimeOut
		}
	}
	reply.Err = res.Err
}

// 3A 自己写，获取数据
func (kv *KVServer) dataGet(key string) (err Err, val string) {
	if v, ok := kv.data[key]; ok {
		err = OK
		val = v
		return
	} else {
		err = ErrNoKey
		return
	}
}

// 3A 检查命令是否重复
func (kv *KVServer) isRepeated(clientId int64, id int64) bool {
	if val, ok := kv.lastApplies[clientId]; ok {
		return val == id
	}
	return false
}

// 3A 自己写，等待应用
func (kv *KVServer) waitApplyCh() {
	for {
		if kv.killed() { //如果节点被删除则return，避免进程卡死
			return
		}
		select {
		case msg := <-kv.applyCh: //循环监视apply通道，一旦有已提交日志则取出。
			if !msg.CommandValid { //如果是无效指令，即说明自身
				kv.mu.Lock()
				kv.readPersist(kv.persister.ReadSnapshot())
				kv.mu.Unlock()
				continue
			}
			op := msg.Command.(Op)
			kv.mu.Lock()
			isRepeated := kv.isRepeated(op.ClientId, op.MsgId) //判断当前请求是否重复请求，若是，不执行写操作。
			switch op.Operation {
			case "Put": //put操作
				if !isRepeated {
					kv.data[op.Key] = op.Value
					kv.lastApplies[op.ClientId] = op.MsgId //更新对应client的日志情况
				}
			case "Append": //append操作，叠加值
				if !isRepeated {
					_, v := kv.dataGet(op.Key)
					kv.data[op.Key] = v + op.Value
					kv.lastApplies[op.ClientId] = op.MsgId //更新client的日志情况
				}
			case "Get": //不需要写操作，跳过
			}
			//kv.persist_kvdata()                       //写操作结束，执行持久化
			kv.saveSnapshot(msg.CommandIndex)         //使用快照存储
			if ch, ok := kv.msgNotify[op.ReqId]; ok { //处理返回值到NotifyMsg管道中，交还给get/append进程处理
				_, v := kv.dataGet(op.Key)
				ch <- NotifyMsg{
					Err:   OK,
					Value: v,
				}
			}
			kv.mu.Unlock()
		}
	}
}

// 3D
func (kv *KVServer) saveSnapshot(logIndex int) {
	if kv.maxraftstate == -1 {
		return
	}
	if kv.persister.RaftStateSize() < kv.maxraftstate {
		return
	}
	w := new(bytes.Buffer)
	e := labgob.NewEncoder(w)
	//存起来所有内容
	e.Encode(kv.data)
	e.Encode(kv.lastApplies)
	data := w.Bytes()
	kv.rf.SavePersistAndShnapshot(logIndex, data)
}

func (kv *KVServer) Kill() {
	atomic.StoreInt32(&kv.dead, 1)
	kv.rf.Kill()
}

func (kv *KVServer) killed() bool {
	z := atomic.LoadInt32(&kv.dead)
	return z == 1
}

func StartKVServer(servers []*labrpc.ClientEnd, me int, persister *raft.Persister, maxraftstate int) *KVServer {
	labgob.Register(Op{})
	kv := new(KVServer)
	kv.me = me
	kv.maxraftstate = maxraftstate //3B那日
	kv.applyCh = make(chan raft.ApplyMsg)
	kv.rf = raft.Make(servers, me, persister, kv.applyCh)
	//3A
	kv.data = make(map[string]string)
	kv.lastApplies = make(map[int64]int64)
	kv.msgNotify = make(map[int64]chan NotifyMsg)
	//持久化：restart时，重新读取data 和lastapply两个重要参数
	kv.persister = persister
	kv.readPersist(kv.persister.ReadSnapshot())
	go kv.waitApplyCh() //启动协程进行应用日志处理
	return kv
}

func (kv *KVServer) readPersist(data []byte) {
	if data == nil || len(data) < 1 {
		return
	}
	r := bytes.NewBuffer(data)
	d := labgob.NewDecoder(r)
	var kvData map[string]string
	var lastApplies map[int64]int64
	if d.Decode(&kvData) != nil ||
		d.Decode(&lastApplies) != nil {
		log.Fatal("kv read persist err")
	} else {
		kv.data = kvData
		kv.lastApplies = lastApplies
	}
}
