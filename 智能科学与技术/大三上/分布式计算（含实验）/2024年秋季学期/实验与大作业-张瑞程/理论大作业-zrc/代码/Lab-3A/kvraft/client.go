package kvraft

import (
	"crypto/rand"
	"math/big"

	"6.824/labrpc"
)

// 3A
type Clerk struct {
	servers []*labrpc.ClientEnd
	// You will have to modify this struct.
	// 3A
	LeaderId int   //记录搜索到的leader的ID，用于访问
	clientId int64 //标识自身，用来给server检查我发的最后一条请求，避免重复
}

// nrand，生成大范围的随机数，从概率上每个随机数是唯一的。
func nrand() int64 {
	max := big.NewInt(int64(1) << 62)
	bigx, _ := rand.Int(rand.Reader, max) //rand.reader是密码学安全的随机数生成器
	x := bigx.Int64()
	return x
}

func MakeClerk(servers []*labrpc.ClientEnd) *Clerk {
	ck := new(Clerk)
	ck.servers = servers
	//2A
	ck.LeaderId = 0 //初始化为0,虽然不知道此时谁是leader
	ck.clientId = nrand()
	return ck
}

func (ck *Clerk) Get(key string) string {
	//3A
	args := GetArgs{Key: key, MsgId: nrand(), ClientId: ck.clientId}
	for { //循环，轮询，直到问出为止
		reply := GetReply{}
		ok := ck.servers[ck.LeaderId].Call("KVServer.Get", &args, &reply)
		if !ok || reply.Err == ErrWrongLeader { //网络不通||不是leader，搜索下一个
			ck.LeaderId = (ck.LeaderId + 1) % len(ck.servers) //由于轮询，需要整除
			continue
		}
		switch reply.Err {
		case OK:
			return reply.Value //查询得到返回值
		case ErrNoKey:
			return ""
		case ErrTimeOut: //对方管道处理超时
			continue
		}
	}
}

func (ck *Clerk) PutAppend(key string, value string, op string) {
	//3A 与get同理
	args := PutAppendArgs{Key: key, Value: value, Op: op, MsgId: nrand(), ClientId: ck.clientId}
	for { //同理,进行轮询
		reply := PutAppendReply{}
		ok := ck.servers[ck.LeaderId].Call("KVServer.PutAppend", &args, &reply)
		if !ok || reply.Err == ErrWrongLeader { //对方非leader，询问其他设备
			ck.LeaderId = (ck.LeaderId + 1) % len(ck.servers)
			continue
		}
		switch reply.Err { //发送成功且对方为leader
		case OK: //对方处理成功，return
			return
		default: //其他，存在错误，如超时，共识失败等。执行轮询。
			ck.LeaderId = (ck.LeaderId + 1) % len(ck.servers)
			continue
		}
	}
}

func (ck *Clerk) Put(key string, value string) {
	ck.PutAppend(key, value, "Put")
}
func (ck *Clerk) Append(key string, value string) {
	ck.PutAppend(key, value, "Append")
}
