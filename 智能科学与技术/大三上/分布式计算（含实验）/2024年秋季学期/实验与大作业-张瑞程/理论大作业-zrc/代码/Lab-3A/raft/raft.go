package raft

import (
	"bytes"
	"log"
	"math/rand"
	"sync"
	"sync/atomic"
	"time"

	"6.824/labgob"
	"6.824/labrpc"
)


type Node_State int

const (
	Follower  Node_State = 0
	Candidate Node_State = 1
	Leader    Node_State = 2
)

const (
	ElectionTimeLimit  = time.Millisecond * 300 // 选举
	HeartBeatTimeLimit = time.Millisecond * 150 // leader 发送心跳
	RPCTimeout         = time.Millisecond * 150 //避免RPC等待过长
)

// 2A 用于在选举结束后重置定时器时间
func (rf *Raft) ResetElectionTimer() {
	rf.ElectionTimer.Stop()
	random_time := time.Duration(rand.Int63()) % ElectionTimeLimit
	rf.ElectionTimer.Reset(ElectionTimeLimit + random_time)
}

// 2D
type ApplyMsg struct {
	CommandValid bool
	Command      interface{}
	CommandIndex int

	// For 2D:
	SnapshotValid bool
	Snapshot      []byte
	SnapshotTerm  int
	SnapshotIndex int
}

// 2A
type votefor struct {
	Term int //上次投票的任期
	id   int //上次投票的id
}

// 2A
func (rf *Raft) GetState() (int, bool) {
	rf.mu.Lock()
	defer rf.mu.Unlock()
	term := rf.Term
	isleader := rf.State == Leader
	return term, isleader
}

// 2A 2B
type RequestVoteArgs struct {
	//2A
	Term        int //term now
	CandidateId int //my id
	//2B  用以限制选举
	LastLogIndex int
	LastLogTerm  int
}

// 2A
type RequestVoteReply struct {
	Term        int  //my term
	Voter_id    int  //whom to vote for
	VoteGranted bool //granted or not
}

// 2A 2B
// 投票函数
func (rf *Raft) RequestVote(args *RequestVoteArgs, reply *RequestVoteReply) {
	rf.mu.Lock()
	defer rf.mu.Unlock()
	reply.Term = rf.Term
	reply.VoteGranted = false //默认反对票
	//////fmt.Println(rf.me, " term ", rf.Term, "收到", args.CandidateId, " term ", args.Term, "的投票请求")
	//对方任期小 or 自己是leader，不予理会
	if args.Term < rf.Term || rf.State == Leader {
		return
	} else if args.Term == rf.Term {
		//相同任期 ①可能是对方落伍了 ②自己在当前任期投过票
		if rf.voteFor.id == args.CandidateId && rf.voteFor.Term == args.Term { //当前任期给他投过票但包丢了，重投
			reply.VoteGranted = true
			return
		}
		if rf.voteFor.Term == args.Term && rf.voteFor.id != args.CandidateId { //当前任期内给其他人投过票
			return
		}
	} else if args.Term > rf.Term { //对方任期比自己大，需要给他投票
		rf.Term = args.Term //更新自己任期
		rf.State = Follower //一旦收到有效选票就要让位，不管对方是否承认
		rf.persist()
		//2B
		lastLogTerm, lastLogIndex := rf.lastLogInfo()
		if lastLogTerm > args.LastLogTerm || (args.LastLogTerm == lastLogTerm && args.LastLogIndex < lastLogIndex) {
			// 如果他的日志比我的旧，反对票,体现在任期号,日志号
			return
		}
		rf.voteFor.id = args.CandidateId
		rf.voteFor.Term = args.Term
		reply.VoteGranted = true
		rf.persist()
		rf.ResetElectionTimer() //当前有人触发选举，重设自身定时器，避免冲突
		return
	}
}

// 2A 投票请求RPC发送
func (rf *Raft) sendRequestVote(server int, args *RequestVoteArgs, reply *RequestVoteReply) {
	t := time.NewTimer(RPCTimeout) //引入超时机制，避免网络影响
	defer t.Stop()
	for {
		ch := make(chan bool, 1) //由于select结构必须用管道，所以构造一个
		//用协程发，否则无法判断超时
		go func() {
			ok := rf.peers[server].Call("Raft.RequestVote", args, reply)
			ch <- ok
		}()
		select {
		case <-t.C:
			return
		case ok := <-ch:
			if ok {
				return
			}
		}
	}
}

// 2A
func (rf *Raft) ticker() {
	for !rf.killed() {
		select {
		case <-rf.ElectionTimer.C: //触发定时器
			rf.ResetElectionTimer() //重设选举计时器
			if rf.State != Leader { //不是leader，则开启选举
				rf.startElection() //发起投票
			}
		}
	}
}

// 2A 通过重置计时器马上触发心跳
func (rf *Raft) sendHeartBeat() {
	for index := range rf.appendEntriesTimers {
		if index == rf.me {
			continue
		}
		rf.appendEntriesTimers[index].Stop()
		rf.appendEntriesTimers[index].Reset(0) //重置心跳定时器 必须重置，不然取不出来！
	}
}

// 2B
// 返回上一个已提交日志的index
func (rf *Raft) lastLogInfo() (int, int) {
	term := rf.logEntries[len(rf.logEntries)-1].Term //最后一个日志
	index := rf.logEntries[len(rf.logEntries)-1].Idx
	return term, index
}

// 2A 2B
// 2B:选举信号加入提交日志信息，限制选举
// 当选leader后，重置自身的matchindex与nextindex
func (rf *Raft) startElection() {
	rf.mu.Lock()
	//转换到candidate状态,并给自己投一票
	rf.Term += 1 //更新任期
	rf.State = Candidate
	rf.voteFor.id = rf.me //避免一个任期投多个票数
	rf.voteFor.Term = rf.Term
	rf.persist()

	//2B:加入日志信号
	lastLogTerm, lastLogIndex := rf.lastLogInfo()
	args := RequestVoteArgs{
		Term:        rf.Term,
		CandidateId: rf.me,
		//2B
		LastLogIndex: lastLogIndex,
		LastLogTerm:  lastLogTerm,
	}

	rf.mu.Unlock()
	votesCh := make(chan bool, len(rf.peers)) //创建管道，存放每个peers的返回票,统一管理，及时销毁
	for index := range rf.peers {
		if index == rf.me {
			continue //不给自己发
		}
		//发起投票协程
		go func(ch chan bool, index int) {
			reply := RequestVoteReply{}
			rf.sendRequestVote(index, &args, &reply)
			ch <- reply.VoteGranted //放入管道内,供后面统计
			//节省时间，一旦发现对方任期比自己高，则转换到follower状态
			if reply.Term > args.Term {
				//当reply任期比自己高，则自己返回到follower
				rf.mu.Lock()
				rf.Term = reply.Term //更新term
				rf.State = Follower  //转换回follower
				rf.persist()
				rf.ResetElectionTimer() //相当于是收到了leader心跳，因此可以重置选举时钟
				rf.mu.Unlock()
			}
		}(votesCh, index)
	}
	//循环统计选票
	voteCount := 1    //统计当前处理过的票数
	grantedCount := 1 //统计赞成票
	rejectCount := 0  //反对票
	for {
		granted := <-votesCh //取票
		voteCount += 1
		if granted {
			grantedCount += 1
		} else {
			rejectCount += 1
		}
		//当赞成票 > 1/2，转换到leader
		if grantedCount > len(rf.peers)/2 {
			rf.mu.Lock()
			if rf.State == Candidate {
				rf.State = Leader
				rf.sendHeartBeat() //马上触发心跳
				//2B //重置nextIndex，match_index
				term, lastLogIndex := rf.lastLogInfo() //取出自己上一个提交的日志
				_ = term
				//fmt.Println("节点", rf.me, "成为新leader,lastlog_index:", lastLogIndex, "term:", term, "last_apply:", rf.lastApplied)
				rf.nextIndex = make([]int, len(rf.peers))
				for i := 0; i < len(rf.peers); i++ {
					rf.nextIndex[i] = lastLogIndex + 1
				}
				rf.matchIndex = make([]int, len(rf.peers))
				rf.matchIndex[rf.me] = lastLogIndex
				rf.persist()
			}
			rf.mu.Unlock()
			return
		}
		//当统计票数到头了还没拿到足够选票||反对票>1/2，则返回。定时器会在一段时间后开启重新选举的
		if voteCount == len(rf.peers) || rejectCount > len(rf.peers) {
			return
		}
	}
}

// 2A
func (rf *Raft) conductHeartbeat(index int) {
	for { //死循环，一直检查对应计时器是否到时
		if rf.killed() {
			return
		}
		select {
		case <-rf.appendEntriesTimers[index].C: //到点了，发日志
			rf.appendEntriesTimers[index].Reset(HeartBeatTimeLimit) //重置心跳定时器 必须重置，不然取不出来！
			if rf.State == Leader {
				rf.appendEntriesToPeer(index) //调用发送函数
			}
		}
	}
}

// 2A 2B
type HeartBeatArgs struct {
	LeaderId int
	Term     int
	//2B 心跳补充日志内容，index等信息，供一致性检查
	PrevLogIndex int        //
	PervLogTerm  int        // 定位上一条日志，一致性检查
	Entries      []LogEntry //当前日志
	LeaderCommit int        // leader提交的日志号？
}
type AppendEntriesReply struct {
	FollowerId int
	Term       int
	//2B
	Success   bool //是否接受本条日志
	NextIndex int  //reply的下一条提交日志号，以确认一致性
}

// 2B 获取到某一peer的心跳包信息，包含日志信息
func (rf *Raft) getAppendEntriesArgs(peerIdx int) HeartBeatArgs {
	var prevLogIndex, prevLogTerm int
	var logs []LogEntry
	nextIdx := rf.nextIndex[peerIdx]              //获取对peeridx应发送的下一个id
	lastLogTerm, lastLogIndex := rf.lastLogInfo() //获取上一个已提交日志的信息
	if nextIdx > lastLogIndex {                   // 没有需要发送的 log
		prevLogIndex = lastLogIndex
		prevLogTerm = lastLogTerm
	} else {
		//否则应该发送日志nextIdx
		logs = append([]LogEntry{}, rf.logEntries[nextIdx:]...) //nextid开始到结束的所有日志
		prevLogIndex = nextIdx - 1                              //填入上一个发送日志信息，检查一致性
		prevLogTerm = rf.logEntries[prevLogIndex].Term
	}
	args := HeartBeatArgs{
		Term:         rf.Term,
		LeaderId:     rf.me,
		PrevLogIndex: prevLogIndex,
		PervLogTerm:  prevLogTerm,
		Entries:      logs,
		LeaderCommit: rf.commitIndex,
	}
	return args
}

// 2A 2B 接收心跳
func (rf *Raft) AppendEntries(args *HeartBeatArgs, reply *AppendEntriesReply) {
	rf.mu.Lock()
	reply.FollowerId = rf.me
	reply.Term = rf.Term
	reply.Success = false

	Term, lastLogIndex := rf.lastLogInfo()
	_ = Term

	//如果对方任期小，不予理会
	if rf.Term > args.Term {
		//fmt.Println("节点", rf.me, "[", lastLogIndex, ",", Term, "]", "拒绝日志(任期小) 来自", args.LeaderId, "[", args.PrevLogIndex, ",", args.PervLogTerm, "] 长度", len(args.Entries)) //, args.Entries)
		rf.mu.Unlock()
		return
	}

	//对方任期比自己大，先确认自己的follower地位
	rf.Term = args.Term
	rf.State = Follower
	rf.ResetElectionTimer()
	//一致性判断内容
	reply.NextIndex = lastLogIndex + 1 //当前需要的                                                                                                      //当前index
	if args.PrevLogIndex > lastLogIndex {
		// 缺少中间的 log
		rf.persist()
		rf.mu.Unlock()
		return
	} else if args.PrevLogIndex < lastLogIndex {
		//我的日志比他的多
		if len(args.Entries) == 0 { //如果他实在没有更多的日志了（他分区，缺少日志）
			//自己删除，配合他
			rf.truncateLog(args.PrevLogIndex + 1) //剪切到他有的最多的日志
			rf.persist()
			rf.mu.Unlock()
			return
		} else {
			//回溯过程，先查看他回溯的是否匹配。
			//如果匹配（他给的prevlogindex的日志和我的这个index日志一致），覆盖自己后面的，success=false
			if rf.logEntries[args.PrevLogIndex].Term == args.PervLogTerm {
				rf.logEntries = append(rf.logEntries[:args.PrevLogIndex+1], args.Entries...) //追加日志
				_, lastLogIndex := rf.lastLogInfo()
				reply.NextIndex = lastLogIndex + 1 //更新nextindex
				reply.Success = true
			} else {
				//不匹配 (他的prevlogindex和我的还不一样)：继续回溯---删除prevlogindex后面的失配日志，同时保持reply的nextindex和他的一致
				rf.truncateLog(args.PrevLogIndex)
				reply.NextIndex = args.PrevLogIndex + 1
			}
			rf.persist()
			rf.mu.Unlock()
			return
		}
	} else { //日志号一致,进行term检查
		if args.PervLogTerm != rf.logEntries[lastLogIndex].Term {
			// 日志号一致但 Term 不一致，拒绝日志，让leader直接找到我的任期的index
			// 信号：success==false 但是 nextid一致
			rf.mu.Unlock()
			return
		} else { // Term 和日志号一致，可以追加
			//fmt.Println("节点", rf.me, "[", lastLogIndex, ",", Term, "]", "接收日志 来自", args.LeaderId, "[", args.PrevLogIndex, ",", args.PervLogTerm, "] 长度", len(args.Entries)) //, args.Entries)
			reply.Success = true
			rf.logEntries = append(rf.logEntries, args.Entries...) //追加日志
			_, lastLogIndex := rf.lastLogInfo()
			reply.NextIndex = lastLogIndex + 1 //更新nextindex
		}
		//提交
		rf.persist()
		rf.mu.Unlock()
		if reply.Success {
			//如果有需要提交的
			if rf.commitIndex < args.LeaderCommit {
				rf.commitIndex = args.LeaderCommit
				rf.notifyApplyCh <- struct{}{}
			}
		}
	}
}

// 2A 2B 该函数给index的peer发心跳包
// 修改：节点没响应就一直发个不停
func (rf *Raft) appendEntriesToPeer(peerIndex int) {
	RPCTimer := time.NewTimer(RPCTimeout)
	defer RPCTimer.Stop()
	for !rf.killed() && rf.State == Leader {
		rf.mu.Lock()
		args := rf.getAppendEntriesArgs(peerIndex)
		rf.mu.Unlock()
		RPCTimer.Stop() //控制每个RPC时间，超时限制
		RPCTimer.Reset(RPCTimeout)
		reply := AppendEntriesReply{}
		replyCh := make(chan bool, 1)
		rf.appendEntriesTimers[peerIndex].Reset(HeartBeatTimeLimit)
		go func() {
			ok := rf.peers[peerIndex].Call("Raft.AppendEntries", &args, &reply)
			replyCh <- ok
		}()
		select {
		case <-RPCTimer.C:
			continue
		case ok := <-replyCh:
			if !ok {
				continue
			}
			if ok && rf.checkReply(&args, &reply, peerIndex) {
				return
			}
		}
		time.Sleep(20 * time.Millisecond)
	}
}

// 2B return true:正常，可以退出  false：一致性出错，需要重发
func (rf *Raft) checkReply(args *HeartBeatArgs, reply *AppendEntriesReply, peerIdx int) bool {
	rf.mu.Lock()
	if reply.Term > rf.Term { //如果对方任期号大，自己变为follower
		rf.State = Follower
		rf.ResetElectionTimer()
		rf.Term = reply.Term
		rf.persist()
		rf.mu.Unlock()
		return true
	}
	if rf.State != Leader || rf.Term != args.Term { //可能在其他进程中自己已经不再是leader，先检查一下
		rf.ResetElectionTimer()
		rf.mu.Unlock()
		return true
	}
	//对方接受日志，一致性正常
	if reply.Success {
		//更新一致性相关信息
		rf.nextIndex[peerIdx] = reply.NextIndex
		rf.matchIndex[peerIdx] = reply.NextIndex - 1
		//如果当前心跳存在日志&& 这个日志是当前任期的(可以commit)
		if len(args.Entries) > 0 && args.Entries[len(args.Entries)-1].Term == rf.Term {
			// 更新commit情况
			rf.updateCommitIndex()
		}
		rf.persist()
		rf.mu.Unlock()
		return true
	} else { // success == false，开启一致性检查程序,发送前一条日志
		//对方缺少日志
		if rf.nextIndex[peerIdx] != reply.NextIndex {
			rf.nextIndex[peerIdx] = reply.NextIndex
			rf.persist()
			rf.mu.Unlock()
			return false
		} else {
			//2C
			// nextid无误，意为term失配，找到自己上一个term
			lastTermId := rf.nextIndex[peerIdx] - 1
			term := args.PervLogTerm
			for {
				lastTermId -= 1 //往前走一格
				if rf.logEntries[lastTermId].Term < term {
					break
				}
			}
			rf.nextIndex[peerIdx] = lastTermId
			rf.persist()
			rf.mu.Unlock()
			return false
		}
	}
}

// 2B
// 2C优化逻辑：如果没有当前任期的日志可供提交，不提交任何事
func (rf *Raft) updateCommitIndex() {
	//遍历已提交日志到最新的未提交日志
	commit := false //标志位，一旦出现提交日志的情况，触发提交管道
	for i := rf.commitIndex + 1; i <= len(rf.logEntries); i++ {
		count := 0                        //同意数
		for _, m := range rf.matchIndex { //遍历每一个peer的匹配id号
			if m >= i { //和他的匹配日志>当前需要提交日志，表明这个日志他接收了
				count += 1
				if count > len(rf.peers)/2 { //成功接收数目过半
					//只有自己任期内的日志可以通过计数提交
					rf.okbutnocommit = i
					if rf.logEntries[i].Term == rf.Term {
						rf.commitIndex = i //更新
						commit = true
					}
					break //退出内循环，检查下一个日志
				}
			}
		}
		if rf.okbutnocommit != i {
			//第i个日志已经失配了，后续的不需要再判断
			break
		}
	}
	if commit {
		rf.notifyApplyCh <- struct{}{}
	}
}

// 2B 截断日志，用于在日志错误时截断
func (rf *Raft) truncateLog(index int) {
	if index < len(rf.logEntries) {
		rf.logEntries = rf.logEntries[:index]
	}
}

// 日志记录类，存放每一条日志的必要信息
type LogEntry struct {
	Term    int
	Idx     int         //当前日志号
	Command interface{} //指令，该日志需要执行的动作
}

type Raft struct {
	mu        sync.Mutex
	peers     []*labrpc.ClientEnd
	persister *Persister
	me        int
	dead      int32
	//2A
	ElectionTimer       *time.Timer   //定时器，监视是否选举超时
	appendEntriesTimers []*time.Timer //定时器，定时发送日志
	State               Node_State
	Term                int
	voteFor             votefor //记录自己之前为谁投票，避免一任期内投多个
	//2B
	logEntries    []LogEntry    //存放所有日志信息
	matchIndex    []int         //一致性中，确认匹配的日志index
	nextIndex     []int         //记录对每一个peer，下一个发送的index
	applyCh       chan ApplyMsg //提交日志管道
	notifyApplyCh chan struct{} //管道，检测是否需要提交
	commitIndex   int           //上一个提交日志index
	okbutnocommit int           //老日志，到了但是不能commit
	lastApplied   int           // 此server上次提交的日志 用于避免出现状态上提交但行为上没提交的情况
}

func Make(peers []*labrpc.ClientEnd, me int, persister *Persister, applyCh chan ApplyMsg) *Raft {
	rf := &Raft{}
	rf.mu.Lock()
	defer rf.mu.Unlock()
	rf.peers = peers
	rf.dead = 0
	rf.persister = persister
	rf.me = me
	rf.Term = 0
	rf.voteFor.id = -1
	rf.voteFor.Term = -1
	random_time := time.Duration(rand.Int63()) % ElectionTimeLimit
	rf.ElectionTimer = time.NewTimer(ElectionTimeLimit + random_time)
	go rf.ticker() // 开启选举监视进程
	//part 2B
	rf.logEntries = make([]LogEntry, 1) // 初始化
	rf.applyCh = applyCh
	rf.notifyApplyCh = make(chan struct{}, 100)
	rf.State = Follower
	//part2C 先读取persister
	rf.readPersist(persister.ReadRaftState())

	//发起协程，监控提交进程
	go func() {
		for {
			if rf.killed() {
				return
			}
			select {
			case <-rf.notifyApplyCh:
				rf.startApplyLogs() //提交日志
			}
		}
	}()

	//心跳相关,对每个peer，启动一个监视进程监视心跳
	rf.appendEntriesTimers = make([]*time.Timer, len(rf.peers)) //初始化时钟列表，监控每一个心跳
	for index := range rf.peers {                               //逐一取出peers的index
		rf.appendEntriesTimers[index] = time.NewTimer(HeartBeatTimeLimit)
		if index == rf.me {
			continue
		}
		go rf.conductHeartbeat(index) //开启协程 监视心跳发送
	}
	return rf
}

// 2B
// 客户机调用的接口，如果不是leader，拒绝回复。如果是leader，马上开启一致性检查
func (rf *Raft) Start(command interface{}) (int, int, bool) {
	term, isLeader := rf.GetState()
	rf.mu.Lock()
	_, lastIndex := rf.lastLogInfo() //获取上一个已提交日志的日志号
	index := lastIndex + 1           //当前要提交的日志index
	if isLeader {                    //如果是leader，提交日志
		rf.logEntries = append(rf.logEntries, LogEntry{ //新建当前日志
			Term:    rf.Term,
			Command: command,
			Idx:     index,
		})
		//提交日志及启动一致性代码
		rf.matchIndex[rf.me] = index //更新新数据
		rf.sendHeartBeat()           //马上发送心跳，同步日志
	}
	rf.mu.Unlock()
	return index, term, isLeader
}

// 2B apply
func (rf *Raft) startApplyLogs() {
	rf.mu.Lock()
	var msgs []ApplyMsg
	if rf.commitIndex <= rf.lastApplied { //并没有需要提交的
		msgs = make([]ApplyMsg, 0)
	} else {
		msgs = make([]ApplyMsg, 0, rf.commitIndex-rf.lastApplied)
		for i := rf.lastApplied + 1; i <= rf.commitIndex; i++ {
			msgs = append(msgs, ApplyMsg{
				CommandValid: true,
				Command:      rf.logEntries[i].Command,
				CommandIndex: i,
			})
		}
	}
	rf.mu.Unlock()
	//上传日志
	for _, msg := range msgs {
		i := msg.CommandIndex
		_ = i
		rf.mu.Lock()
		rf.applyCh <- msg
		rf.lastApplied = msg.CommandIndex
		rf.persist()
		rf.mu.Unlock()
	}
}

// 2C
func (rf *Raft) persist() {
	w := new(bytes.Buffer)
	e := labgob.NewEncoder(w)
	e.Encode(rf.Term)
	e.Encode(rf.voteFor.id)
	e.Encode(rf.voteFor.Term)
	e.Encode(rf.commitIndex)
	e.Encode(rf.lastApplied)
	e.Encode(rf.logEntries)
	data := w.Bytes()
	rf.persister.SaveRaftState(data)
}

// 2C
// restore previously persisted state.
func (rf *Raft) readPersist(data []byte) {
	if data == nil || len(data) < 1 { // bootstrap without any state?
		return
	}
	r := bytes.NewBuffer(data)
	d := labgob.NewDecoder(r)
	var term int
	var voteFor votefor
	var commitIndex int
	var lastApplied int
	var logs []LogEntry
	if d.Decode(&term) != nil ||
		d.Decode(&voteFor.id) != nil ||
		d.Decode(&voteFor.Term) != nil ||
		d.Decode(&commitIndex) != nil ||
		d.Decode(&lastApplied) != nil ||
		d.Decode(&logs) != nil {
		log.Fatal("rf read persist err")
	} else {
		rf.Term = term
		rf.voteFor = voteFor
		rf.commitIndex = commitIndex
		rf.lastApplied = lastApplied
		rf.logEntries = logs
	}
}

// 2D
// A service wants to switch to snapshot.  Only do so if Raft hasn't
// have more recent info since it communicate the snapshot on applyCh.
func (rf *Raft) CondInstallSnapshot(lastIncludedTerm int, lastIncludedIndex int, snapshot []byte) bool {

	// Your code here (2D).

	return true
}

// the service says it has created a snapshot that has
// all info up to and including index. this means the
// service no longer needs the log through (and including)
// that index. Raft should now trim its log as much as possible.
func (rf *Raft) Snapshot(index int, snapshot []byte) {
	// Your code here (2D).
}

// the tester doesn't halt goroutines created by Raft after each test,
// but it does call the Kill() method. your code can use killed() to
// check whether Kill() has been called. the use of atomic avoids the
// need for a lock.
//
// the issue is that long-running goroutines use memory and may chew
// up CPU time, perhaps causing later tests to fail and generating
// confusing debug output. any goroutine with a long-running loop
// should call killed() to check whether it should stop.
func (rf *Raft) Kill() {
	atomic.StoreInt32(&rf.dead, 1) //dead为1时死掉
	// Your code here, if desired.
}

func (rf *Raft) killed() bool {
	z := atomic.LoadInt32(&rf.dead)
	return z == 1
}
