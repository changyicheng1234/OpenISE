package raft

//
// this is an outline of the API that raft must expose to
// the service (or tester). see comments below for
// each of these functions for more details.
//
// rf = Make(...)
//   create a new Raft server.
// rf.Start(command interface{}) (index, term, isleader)
//   start agreement on a new log entry
// rf.GetState() (term, isLeader)
//   ask a Raft for its current term, and whether it thinks it is leader
// ApplyMsg
//   each time a new entry is committed to the log, each Raft peer
//   should send an ApplyMsg to the service (or tester)
//   in the same server.
//

import (
	"6.824/labgob"
	"bytes"
	"log"
	"math/rand"
	//	"bytes"
	"sync"
	"sync/atomic"
	"time"

	//	"6.824/labgob"
	"6.824/labrpc"
)

//
// as each Raft peer becomes aware that successive log entries are
// committed, the peer should send an ApplyMsg to the service (or
// tester) on the same server, via the applyCh passed to Make(). set
// CommandValid to true to indicate that the ApplyMsg contains a newly
// committed log entry.
//
// in part 2D you'll want to send other kinds of messages (e.g.,
// snapshots) on the applyCh, but set CommandValid to false for these
// other uses.
//

func init() {
	rand.Seed(150)
}

type Role int

const (
	Role_Follower  = 0
	Role_Candidate = 1
	Role_Leader    = 2
)

const (
	ElectionTimeout   = time.Millisecond * 300 // 选举超时时间/心跳超时时间
	HeartBeatInterval = time.Millisecond * 150 // leader 发送心跳
	ApplyInterval     = time.Millisecond * 100 // apply log
	RPCTimeout        = time.Millisecond * 100
	MaxLockTime       = time.Millisecond * 10 // debug
)

type ApplyMsg struct {
	CommandValid bool
	Command      interface{}
	CommandIndex int

	// For 2D:接收到leader发来的快照，就要发送一条快照命令
	SnapshotValid bool
	Snapshot      []byte
	SnapshotTerm  int
	SnapshotIndex int
}

type LogEntry struct {
	Term    int
	Command interface{}
}

//
// A Go object implementing a single Raft peer.
//
type Raft struct {
	mu        sync.Mutex          // Lock to protect shared access to this peer's state
	peers     []*labrpc.ClientEnd // RPC end points of all peers，每一个clientEnd对应了向该peer通信的端点
	persister *Persister          // Object to hold this peer's persisted state
	me        int                 // this peer's index into peers[]
	dead      int32               // set by Kill()

	// Your data here (2A, 2B, 2C).
	// Look at the paper's Figure 2 for a description of what
	// state a Raft server must maintain.
	role        Role
	currentTerm int
	votedFor    int
	logs        []LogEntry
	commitIndex int
	lastApplied int
	nextIndex   []int
	matchIndex  []int

	electionTimer       *time.Timer
	appendEntriesTimers []*time.Timer
	applyTimer          *time.Timer
	applyCh             chan ApplyMsg //这个chan是用来提交应用的日志，具体的处理在config.go文件中
	notifyApplyCh       chan struct{}
	stopCh              chan struct{}

	lastSnapshotIndex int // 快照中最后一条日志的index，是真正的index，不是存储在logs中的index
	lastSnapshotTerm  int
}

// return currentTerm and whether this server
// believes it is the leader.
func (rf *Raft) GetState() (int, bool) {
	// Your code here (2A).
	flag := false
	if rf.role == Role_Leader {
		flag = true
	}
	return rf.currentTerm, flag
}

func (rf *Raft) getElectionTimeout() time.Duration {
	t := ElectionTimeout + time.Duration(rand.Int63())%ElectionTimeout
	return t
}

func (rf *Raft) resetElectionTimer() {
	rf.electionTimer.Stop()
	rf.electionTimer.Reset(rf.getElectionTimeout())
}

//返回当前状态机的最后一条日志的任期和索引
//索引是一直会增大的，但是我们的日志队列却不可能无限增大，在队列中下标0存储快照
func (rf *Raft) getLastLogTermAndIndex() (int, int) {
	return rf.logs[len(rf.logs)-1].Term, rf.lastSnapshotIndex + len(rf.logs) - 1
}

func (rf *Raft) changeRole(newRole Role) {
	if newRole < 0 || newRole > 3 {
		panic("unknown role")
	}
	rf.role = newRole
	switch newRole {
	case Role_Follower:
	case Role_Candidate:
		rf.currentTerm++
		rf.votedFor = rf.me
		rf.resetElectionTimer()
	case Role_Leader:
		//leader只有两个特殊的数据结构：nextIndex,matchIndex
		_, lastLogIndex := rf.getLastLogTermAndIndex()
		for i := 0; i < len(rf.peers); i++ {
			rf.nextIndex[i] = lastLogIndex + 1
			rf.matchIndex[i] = lastLogIndex
		}
		rf.resetElectionTimer()
	default:
		panic("unknown role")
	}

}

//获取持久化的数据
func (rf *Raft) getPersistData() []byte {
	w := new(bytes.Buffer)
	e := labgob.NewEncoder(w)
	e.Encode(rf.currentTerm)
	e.Encode(rf.votedFor)
	e.Encode(rf.commitIndex)
	e.Encode(rf.lastSnapshotTerm)
	e.Encode(rf.lastSnapshotIndex)
	e.Encode(rf.logs)
	data := w.Bytes()
	return data
}

//
// save Raft's persistent state to stable storage,
// where it can later be retrieved after a crash and restart.
// see paper's Figure 2 for a description of what should be persistent.
//
// 保存持久化状态
func (rf *Raft) persist() {
	// Your code here (2C).
	// Example:
	// w := new(bytes.Buffer)
	// e := labgob.NewEncoder(w)
	// e.Encode(rf.xxx)
	// e.Encode(rf.yyy)
	// data := w.Bytes()
	data := rf.getPersistData()
	rf.persister.SaveRaftState(data)
}

//
// restore previously persisted state.
//
//读取持久化数据
func (rf *Raft) readPersist(data []byte) {
	if data == nil || len(data) < 1 { // bootstrap without any state?
		return
	}
	// Your code here (2C).
	// Example:
	// r := bytes.NewBuffer(data)
	// d := labgob.NewDecoder(r)
	// var xxx
	// var yyy
	// if d.Decode(&xxx) != nil ||
	//    d.Decode(&yyy) != nil {
	//   error...
	// } else {
	//   rf.xxx = xxx
	//   rf.yyy = yyy
	// }
	r := bytes.NewBuffer(data)
	d := labgob.NewDecoder(r)

	var (
		currentTerm                                      int
		votedFor                                         int
		logs                                             []LogEntry
		commitIndex, lastSnapshotTerm, lastSnapshotIndex int
	)

	if d.Decode(&currentTerm) != nil || d.Decode(&votedFor) != nil || d.Decode(&commitIndex) != nil ||
		d.Decode(&lastSnapshotTerm) != nil || d.Decode(&lastSnapshotIndex) != nil || d.Decode(&logs) != nil {
		log.Fatal("rf read persist err!")
	} else {
		rf.currentTerm = currentTerm
		rf.votedFor = votedFor
		rf.commitIndex = commitIndex
		rf.lastSnapshotIndex = lastSnapshotIndex
		rf.lastSnapshotTerm = lastSnapshotTerm
		rf.logs = logs
	}
}

//
// A service wants to switch to snapshot.  Only do so if Raft hasn't
// have more recent info since it communicate the snapshot on applyCh.
//
//其实CondInstallSnapshot中的逻辑可以直接在InstallSnapshot中来完成，让CondInstallSnapshot成为一个空函数，这样可以减少锁的获取和释放
func (rf *Raft) CondInstallSnapshot(lastIncludedTerm int, lastIncludedIndex int, snapshot []byte) bool {
	rf.mu.Lock()
	defer rf.mu.Unlock()
	// Your code here (2D).
	//installLen := lastIncludedIndex - rf.lastSnapshotIndex
	//if installLen >= len(rf.logs)-1 {
	//	rf.logs = make([]LogEntry, 1)
	//	rf.logs[0].Term = lastIncludedTerm
	//} else {
	//	rf.logs = rf.logs[installLen:]
	//}
	_, lastIndex := rf.getLastLogTermAndIndex()
	if lastIncludedIndex > lastIndex {
		rf.logs = make([]LogEntry, 1)
	} else {
		installLen := lastIncludedIndex - rf.lastSnapshotIndex
		rf.logs = rf.logs[installLen:]
		rf.logs[0].Command = nil
	}
	//0处是空日志，代表了快照日志的标记
	rf.logs[0].Term = lastIncludedTerm

	//其实接下来可以读入快照的数据进行同步，这里可以不写

	rf.lastSnapshotIndex, rf.lastSnapshotTerm = lastIncludedIndex, lastIncludedTerm
	rf.lastApplied, rf.commitIndex = lastIncludedIndex, lastIncludedIndex
	//保存快照和状态
	rf.persister.SaveStateAndSnapshot(rf.getPersistData(), snapshot)
	return true
}

// the service says it has created a snapshot that has
// all info up to and including index. this means the
// service no longer needs the log through (and including)
// that index. Raft should now trim its log as much as possible.
//生成一次快照，实现很简单，删除掉对应已经被压缩的 raft log 即可
//index是当前要压缩到的index，snapshot是已经帮我们压缩好的数据
func (rf *Raft) Snapshot(index int, snapshot []byte) {
	// Your code here (2D).
	rf.mu.Lock()
	defer rf.mu.Unlock()
	snapshotIndex := rf.lastSnapshotIndex
	if snapshotIndex >= index {
		DPrintf("{Node %v} rejects replacing log with snapshotIndex %v as current snapshotIndex %v is larger in term %v", rf.me, index, snapshotIndex, rf.currentTerm)
		return
	}
	oldLastSnapshotIndex := rf.lastSnapshotIndex
	rf.lastSnapshotTerm = rf.logs[rf.getStoreIndexByLogIndex(index)].Term
	rf.lastSnapshotIndex = index
	//删掉index前的所有日志
	rf.logs = rf.logs[index-oldLastSnapshotIndex:]
	//0位置就是快照命令
	rf.logs[0].Term = rf.lastSnapshotTerm
	rf.logs[0].Command = nil
	rf.persister.SaveStateAndSnapshot(rf.getPersistData(), snapshot)
	DPrintf("{Node %v}'s state is {role %v,term %v,commitIndex %v,lastApplied %v} after replacing log with snapshotIndex %v as old snapshotIndex %v is smaller", rf.me, rf.role, rf.currentTerm, rf.commitIndex, rf.lastApplied, index, snapshotIndex)
}

//
// the service using Raft (e.g. a k/v server) wants to start
// agreement on the next command to be appended to Raft's log. if this
// server isn't the leader, returns false. otherwise start the
// agreement and return immediately. there is no guarantee that this
// command will ever be committed to the Raft log, since the leader
// may fail or lose an election. even if the Raft instance has been killed,
// this function should return gracefully.
//
// the first return value is the index that the command will appear at
// if it's ever committed. the second return value is the current
// term. the third return value is true if this server believes it is
// the leader.
//
//客户端请求，leader添加日志
func (rf *Raft) Start(command interface{}) (int, int, bool) {
	index := -1
	term := -1
	isLeader := false
	// Your code here (2B).
	rf.mu.Lock()
	defer rf.mu.Unlock()

	if rf.role != Role_Leader {
		return index, term, isLeader
	}

	rf.logs = append(rf.logs, LogEntry{
		Term:    rf.currentTerm,
		Command: command,
	})
	_, lastIndex := rf.getLastLogTermAndIndex()
	index = lastIndex
	rf.matchIndex[rf.me] = lastIndex
	rf.nextIndex[rf.me] = lastIndex + 1

	term = rf.currentTerm
	isLeader = true

	DPrintf("%v start command%v %v：%+v", rf.me, rf.lastSnapshotIndex, lastIndex, command)

	rf.resetAppendEntriesTimersZero()

	return index, term, isLeader
}

//
// the tester doesn't halt goroutines created by Raft after each test,
// but it does call the Kill() method. your code can use killed() to
// check whether Kill() has been called. the use of atomic avoids the
// need for a lock.
//
// the issue is that long-running goroutines use memory and may chew
// up CPU time, perhaps causing later tests to fail and generating
// confusing debug output. any goroutine with a long-running loop
// should call killed() to check whether it should stop.
//
func (rf *Raft) Kill() {
	atomic.StoreInt32(&rf.dead, 1)
	// Your code here, if desired.
}

func (rf *Raft) killed() bool {
	z := atomic.LoadInt32(&rf.dead)
	return z == 1
}

//处理要应用的日志，快照的命令比较特殊，不在这里提交
func (rf *Raft) startApplyLogs() {
	defer rf.applyTimer.Reset(ApplyInterval)

	rf.mu.Lock()
	var msgs []ApplyMsg
	if rf.lastApplied < rf.lastSnapshotIndex {
		//此时要安装快照，命令在接收到快照时就发布过了，等待处理
		msgs = make([]ApplyMsg, 0)
		rf.mu.Unlock()
		rf.CondInstallSnapshot(rf.lastSnapshotTerm, rf.lastSnapshotIndex, rf.persister.snapshot)
		return
	} else if rf.commitIndex <= rf.lastApplied {
		// snapShot 没有更新 commitidx
		msgs = make([]ApplyMsg, 0)
	} else {
		msgs = make([]ApplyMsg, 0, rf.commitIndex-rf.lastApplied)
		for i := rf.lastApplied + 1; i <= rf.commitIndex; i++ {
			msgs = append(msgs, ApplyMsg{
				CommandValid: true,
				Command:      rf.logs[rf.getStoreIndexByLogIndex(i)].Command,
				CommandIndex: i,
			})
		}
	}
	rf.mu.Unlock()

	for _, msg := range msgs {
		rf.applyCh <- msg
		rf.mu.Lock()
		rf.lastApplied = msg.CommandIndex
		rf.mu.Unlock()
	}
}

// The ticker go routine starts a new election if this peer hasn't received
// heartsbeats recently.
func (rf *Raft) ticker() {
	go func() {
		for {
			select {
			case <-rf.stopCh:
				return
			case <-rf.applyTimer.C:
				rf.notifyApplyCh <- struct{}{}
			case <-rf.notifyApplyCh: //当有日志记录提交了，要进行应用
				rf.startApplyLogs()
			}
		}
	}()

	//选举定时
	go func() {
		for rf.killed() == false {

			// Your code here to check if a leader election should
			// be started and to randomize sleeping time using
			// time.Sleep().
			select {
			case <-rf.stopCh:
				return
			case <-rf.electionTimer.C:
				rf.startElection()
			}

		}
	}()

	//leader发送日志定时
	for i, _ := range rf.peers {
		if i == rf.me {
			continue
		}
		go func(cur int) {
			for rf.killed() == false {
				select {
				case <-rf.stopCh:
					return
				case <-rf.appendEntriesTimers[cur].C:
					rf.sendAppendEntriesToPeer(cur)
				}
			}
		}(i)
	}

}

//
// the service or tester wants to create a Raft server. the ports
// of all the Raft servers (including this one) are in peers[]. this
// server's port is peers[me]. all the servers' peers[] arrays
// have the same order. persister is a place for this server to
// save its persistent state, and also initially holds the most
// recent saved state, if any. applyCh is a channel on which the
// tester or service expects Raft to send ApplyMsg messages.
// Make() must return quickly, so it should start goroutines
// for any long-running work.
//
func Make(peers []*labrpc.ClientEnd, me int,
	persister *Persister, applyCh chan ApplyMsg) *Raft {
	DPrintf("make a raft,me: %v", me)
	rf := &Raft{}
	rf.peers = peers
	rf.persister = persister
	rf.me = me

	// Your initialization code here (2A, 2B, 2C).
	rf.role = Role_Follower
	rf.currentTerm = 0
	rf.votedFor = -1
	rf.logs = make([]LogEntry, 1) //下标为0存储快照
	// initialize from state persisted before a crash
	rf.commitIndex = 0
	rf.lastApplied = 0
	rf.nextIndex = make([]int, len(rf.peers))
	rf.matchIndex = make([]int, len(rf.peers))
	//读取持久化数据
	rf.readPersist(persister.ReadRaftState())

	rf.electionTimer = time.NewTimer(rf.getElectionTimeout())
	rf.appendEntriesTimers = make([]*time.Timer, len(rf.peers))
	for i := 0; i < len(rf.peers); i++ {
		rf.appendEntriesTimers[i] = time.NewTimer(HeartBeatInterval)
	}
	rf.applyTimer = time.NewTimer(ApplyInterval)
	rf.applyCh = applyCh
	rf.notifyApplyCh = make(chan struct{}, 100)
	rf.stopCh = make(chan struct{})

	// start ticker goroutine to start elections
	rf.ticker()

	return rf
}


type AppendEntriesArgs struct {
	Term         int
	LeaderId     int
	PrevLogIndex int
	PrevLogTerm  int
	Entries      []LogEntry
	LeaderCommit int
}

type AppendEntriesReply struct {
	Term         int
	Success      bool
	NextLogTerm  int
	NextLogIndex int
}

//立马发送
func (rf *Raft) resetAppendEntriesTimersZero() {
	for _, timer := range rf.appendEntriesTimers {
		timer.Stop()
		timer.Reset(0)
	}
}

func (rf *Raft) resetAppendEntriesTimerZero(peerId int) {
	rf.appendEntriesTimers[peerId].Stop()
	rf.appendEntriesTimers[peerId].Reset(0)
}

//重置单个timer
func (rf *Raft) resetAppendEntriesTimer(peerId int) {
	rf.appendEntriesTimers[peerId].Stop()
	rf.appendEntriesTimers[peerId].Reset(HeartBeatInterval)
}

//判断当前raft的日志记录是否超过发送过来的日志记录
func (rf *Raft) isOutOfArgsAppendEntries(args *AppendEntriesArgs) bool {
	argsLastLogIndex := args.PrevLogIndex + len(args.Entries)
	lastLogTerm, lastLogIndex := rf.getLastLogTermAndIndex()
	if lastLogTerm == args.Term && argsLastLogIndex < lastLogIndex {
		return true
	}
	return false
}

//获取当前存储位置的索引
func (rf *Raft) getStoreIndexByLogIndex(logIndex int) int {
	storeIndex := logIndex - rf.lastSnapshotIndex
	if storeIndex < 0 {
		return -1
	}
	return storeIndex
}

//接收端处理rpc
//主要进行三个处理：
// 	1. 判断任期
// 	2. 判断是否接收数据，success：数据全部接受，或者根本就没有数据
//	3. 判断是否提交数据
func (rf *Raft) AppendEntries(args *AppendEntriesArgs, reply *AppendEntriesReply) {
	rf.mu.Lock()
	DPrintf("%v receive a appendEntries: %+v", rf.me, args)
	reply.Term = rf.currentTerm
	if args.Term < rf.currentTerm {
		rf.mu.Unlock()
		return
	}
	rf.currentTerm = args.Term
	rf.changeRole(Role_Follower)
	rf.resetElectionTimer()

	_, lastLogIndex := rf.getLastLogTermAndIndex()
	//先判断两边，再判断刚好从快照开始，再判断中间的情况
	if args.PrevLogIndex < rf.lastSnapshotIndex {
		//1.要插入的前一个index小于快照index，几乎不会发生
		reply.Success = false
		reply.NextLogIndex = rf.lastSnapshotIndex + 1
	} else if args.PrevLogIndex > lastLogIndex {
		//2. 要插入的前一个index大于最后一个log的index，说明中间还有log
		reply.Success = false
		reply.NextLogIndex = lastLogIndex + 1
	} else if args.PrevLogIndex == rf.lastSnapshotIndex {
		//3. 要插入的前一个index刚好等于快照的index，说明可以全覆盖，但要判断是否是全覆盖
		if rf.isOutOfArgsAppendEntries(args) {
			reply.Success = false
			reply.NextLogIndex = 0 //=0代表着插入会导致乱序
		} else {
			reply.Success = true
			rf.logs = append(rf.logs[:1], args.Entries...)
			_, currentLogIndex := rf.getLastLogTermAndIndex()
			reply.NextLogIndex = currentLogIndex + 1
		}
	} else if args.PrevLogTerm == rf.logs[rf.getStoreIndexByLogIndex(args.PrevLogIndex)].Term {
		//4. 中间的情况：索引处的两个term相同
		if rf.isOutOfArgsAppendEntries(args) {
			reply.Success = false
			reply.NextLogIndex = 0
		} else {
			reply.Success = true
			rf.logs = append(rf.logs[:rf.getStoreIndexByLogIndex(args.PrevLogIndex)+1], args.Entries...)
			_, currentLogIndex := rf.getLastLogTermAndIndex()
			reply.NextLogIndex = currentLogIndex + 1
		}
	} else {
		//5. 中间的情况：索引处的两个term不相同，跳过一个term
		term := rf.logs[rf.getStoreIndexByLogIndex(args.PrevLogIndex)].Term
		index := args.PrevLogIndex
		for index > rf.commitIndex && index > rf.lastSnapshotIndex && rf.logs[rf.getStoreIndexByLogIndex(index)].Term == term {
			index--
		}
		reply.Success = false
		reply.NextLogIndex = index + 1
	}

	if reply.Success {
		DPrintf("%v current commit: %v, try to commit %v", rf.me, rf.commitIndex, args.LeaderCommit)
		if rf.commitIndex < args.LeaderCommit {
			rf.commitIndex = args.LeaderCommit
			rf.notifyApplyCh <- struct{}{}
		}
	}

	rf.persist()
	DPrintf("%v role: %v, get appendentries finish,args = %v,reply = %+v", rf.me, rf.role, *args, *reply)
	rf.mu.Unlock()

}

//获取要向指定节点发送的日志
func (rf *Raft) getAppendLogs(peerId int) (prevLogIndex int, prevLogTerm int, logEntries []LogEntry) {
	nextIndex := rf.nextIndex[peerId]
	lastLogTerm, lastLogIndex := rf.getLastLogTermAndIndex()
	if nextIndex <= rf.lastSnapshotIndex || nextIndex > lastLogIndex {
		//没有要发送的log
		prevLogTerm = lastLogTerm
		prevLogIndex = lastLogIndex
		return
	}
	//这里一定要进行深拷贝，不然会和Snapshot()发生数据上的冲突
	//logEntries = rf.logs[nextIndex-rf.lastSnapshotIndex:]
	logEntries = make([]LogEntry, lastLogIndex-nextIndex+1)
	copy(logEntries, rf.logs[nextIndex-rf.lastSnapshotIndex:])
	prevLogIndex = nextIndex - 1
	if prevLogIndex == rf.lastSnapshotIndex {
		prevLogTerm = rf.lastSnapshotTerm
	} else {
		prevLogTerm = rf.logs[prevLogIndex-rf.lastSnapshotIndex].Term
	}

	return
}

//尝试去提交日志
//会依次判断，可以提交多个，但不能有间断
func (rf *Raft) tryCommitLog() {
	_, lastLogIndex := rf.getLastLogTermAndIndex()
	hasCommit := false

	for i := rf.commitIndex + 1; i <= lastLogIndex; i++ {
		count := 0
		for _, m := range rf.matchIndex {
			if m >= i {
				count += 1
				//提交数达到多数派
				if count > len(rf.peers)/2 {
					rf.commitIndex = i
					hasCommit = true
					DPrintf("%v role: %v,commit index %v", rf.me, rf.role, i)
					break
				}
			}
		}
		if rf.commitIndex != i {
			break
		}
	}

	if hasCommit {
		rf.notifyApplyCh <- struct{}{}
	}
}

//发送端发送数据
func (rf *Raft) sendAppendEntries(server int, args *AppendEntriesArgs, reply *AppendEntriesReply) {
	rpcTimer := time.NewTimer(RPCTimeout)
	defer rpcTimer.Stop()

	ch := make(chan bool, 1)
	go func() {
		//尝试10次
		for i := 0; i < 10 && !rf.killed(); i++ {
			ok := rf.peers[server].Call("Raft.AppendEntries", args, reply)
			if !ok {
				time.Sleep(time.Millisecond * 10)
				continue
			} else {
				ch <- ok
				return
			}
		}
	}()

	select {
	case <-rpcTimer.C:
		DPrintf("%v role: %v, send append entries to peer %v TIME OUT!!!", rf.me, rf.role, server)
		return
	case <-ch:
		return
	}
}

func (rf *Raft) sendAppendEntriesToPeer(peerId int) {
	if rf.killed() {
		return
	}

	rf.mu.Lock()
	if rf.role != Role_Leader {
		rf.resetAppendEntriesTimer(peerId)
		rf.mu.Unlock()
		return
	}
	DPrintf("%v send append entries to peer %v", rf.me, peerId)

	prevLogIndex, prevLogTerm, logEntries := rf.getAppendLogs(peerId)
	args := AppendEntriesArgs{
		Term:         rf.currentTerm,
		LeaderId:     rf.me,
		PrevLogIndex: prevLogIndex,
		PrevLogTerm:  prevLogTerm,
		Entries:      logEntries,
		LeaderCommit: rf.commitIndex,
	}
	reply := AppendEntriesReply{}
	rf.resetAppendEntriesTimer(peerId)
	rf.mu.Unlock()

	rf.sendAppendEntries(peerId, &args, &reply)

	DPrintf("%v role: %v, send append entries to peer finish,%v,args = %+v,reply = %+v", rf.me, rf.role, peerId, args, reply)

	rf.mu.Lock()
	if reply.Term > rf.currentTerm {
		rf.changeRole(Role_Follower)
		rf.currentTerm = reply.Term
		rf.resetElectionTimer()
		rf.persist()
		rf.mu.Unlock()
		return
	}

	if rf.role != Role_Leader || rf.currentTerm != args.Term {
		rf.mu.Unlock()
		return
	}

	//响应：成功了，即：发送的数据全部接收了，或者根本没有数据
	if reply.Success {
		if reply.NextLogIndex > rf.nextIndex[peerId] {
			rf.nextIndex[peerId] = reply.NextLogIndex
			rf.matchIndex[peerId] = reply.NextLogIndex - 1
		}
		if len(args.Entries) > 0 && args.Entries[len(args.Entries)-1].Term == rf.currentTerm {
			//每个leader只能提交自己任期的日志
			rf.tryCommitLog()
		}
		rf.persist()
		rf.mu.Unlock()
		return
	}

	//响应：失败了，此时要修改nextIndex或者不做处理
	if reply.NextLogIndex != 0 {
		if reply.NextLogIndex > rf.lastSnapshotIndex {
			rf.nextIndex[peerId] = reply.NextLogIndex
			//为了一致性，立马发送
			rf.resetAppendEntriesTimerZero(peerId)
		} else {
			//发送快照
			go rf.sendInstallSnapshotToPeer(peerId)
		}
		rf.mu.Unlock()
		return
	} else {
		//reply.NextLogIndex = 0,此时如果插入会导致乱序，可以不进行处理
	}

	rf.mu.Unlock()
	return

}


type InstallSnapshotArgs struct {
	Term              int
	LeaderId          int
	LastIncludedIndex int
	LastIncludedTerm  int
	//Offset            int
	Data []byte
	//Done bool
}

type InstallSnapshotReply struct {
	Term int
}

func (rf *Raft) InstallSnapshot(args *InstallSnapshotArgs, reply *InstallSnapshotReply) {
	rf.mu.Lock()
	defer rf.mu.Unlock()

	reply.Term = rf.currentTerm
	if rf.currentTerm > args.Term {
		return
	}

	if args.Term > rf.currentTerm || rf.role != Role_Follower {
		rf.changeRole(Role_Follower)
		rf.votedFor = -1
		rf.currentTerm = args.Term
		rf.resetElectionTimer()
		rf.persist()
	}

	//如果自身快照包含的最后一个日志>=leader快照包含的最后一个日志，就没必要接受了
	if rf.lastSnapshotIndex >= args.LastIncludedIndex {
		return
	}

	/********以下内容和CondInstallSnapshot的操作是相同的，因为不知道为什么在lab4B中只要调用CondInstallSnapshot函数就会陷入阻塞，因此将操作逻辑复制到这里一份，lab4中就没有调用CondInstallSnapshot函数了***********/

	lastIncludedIndex := args.LastIncludedIndex
	lastIncludedTerm := args.LastIncludedTerm
	_, lastIndex := rf.getLastLogTermAndIndex()
	if lastIncludedIndex > lastIndex {
		rf.logs = make([]LogEntry, 1)
	} else {
		installLen := lastIncludedIndex - rf.lastSnapshotIndex
		rf.logs = rf.logs[installLen:]
		rf.logs[0].Command = nil
	}
	//0处是空日志，代表了快照日志的标记
	rf.logs[0].Term = lastIncludedTerm

	rf.lastSnapshotIndex, rf.lastSnapshotTerm = lastIncludedIndex, lastIncludedTerm
	rf.lastApplied, rf.commitIndex = lastIncludedIndex, lastIncludedIndex
	//保存快照和状态
	rf.persister.SaveStateAndSnapshot(rf.getPersistData(), args.Data)

	/***********************************/

	//接收发来的快照，并提交一个命令处理
	rf.applyCh <- ApplyMsg{
		SnapshotValid: true,
		Snapshot:      args.Data,
		SnapshotTerm:  args.LastIncludedTerm,
		SnapshotIndex: args.LastIncludedIndex,
	}

}

//向指定节点发送快照
func (rf *Raft) sendInstallSnapshotToPeer(server int) {
	rf.mu.Lock()
	args := InstallSnapshotArgs{
		Term:              rf.currentTerm,
		LeaderId:          rf.me,
		LastIncludedIndex: rf.lastSnapshotIndex,
		LastIncludedTerm:  rf.lastSnapshotTerm,
		Data:              rf.persister.ReadSnapshot(),
	}
	rf.mu.Unlock()

	timer := time.NewTimer(RPCTimeout)
	defer timer.Stop()
	DPrintf("%v role: %v, send snapshot  to peer,%v,args = %+v", rf.me, rf.role, server, args)

	for {
		timer.Stop()
		timer.Reset(RPCTimeout)

		ch := make(chan bool, 1)
		reply := &InstallSnapshotReply{}
		go func() {
			ok := rf.peers[server].Call("Raft.InstallSnapshot", &args, reply)
			if !ok {
				time.Sleep(time.Millisecond * 10)
			}
			ch <- ok
		}()

		select {
		case <-rf.stopCh:
			return
		case <-timer.C:
			DPrintf("%v role: %v, send snapshot to peer %v TIME OUT!!!", rf.me, rf.role, server)
			continue
		case ok := <-ch:
			if !ok {
				continue
			}
		}

		rf.mu.Lock()
		defer rf.mu.Unlock()
		if rf.role != Role_Leader || args.Term != rf.currentTerm {
			return
		}
		if reply.Term > rf.currentTerm {
			rf.changeRole(Role_Follower)
			rf.currentTerm = reply.Term
			rf.resetElectionTimer()
			rf.persist()
			return
		}

		if args.LastIncludedIndex > rf.matchIndex[server] {
			rf.matchIndex[server] = args.LastIncludedIndex
		}
		if args.LastIncludedIndex+1 > rf.nextIndex[server] {
			rf.nextIndex[server] = args.LastIncludedIndex + 1
		}
		return
	}
}


//
// example RequestVote RPC arguments structure.
// field names must start with capital letters!
//
type RequestVoteArgs struct {
	// Your data here (2A, 2B).
	Term         int
	CandidateId  int
	LastLogIndex int
	LastLogTerm  int
}

//
// example RequestVote RPC reply structure.
// field names must start with capital letters!
//
type RequestVoteReply struct {
	// Your data here (2A).
	Term        int
	VoteGranted bool
}

//
// example RequestVote RPC handler.
//
func (rf *Raft) RequestVote(args *RequestVoteArgs, reply *RequestVoteReply) {
	// Your code here (2A, 2B).
	rf.mu.Lock()
	defer rf.mu.Unlock()

	//默认失败，返回
	lastLogTerm, lastLogIndex := rf.getLastLogTermAndIndex()
	reply.Term = rf.currentTerm
	reply.VoteGranted = false

	if rf.currentTerm > args.Term {
		return
	} else if rf.currentTerm == args.Term {
		if rf.role == Role_Leader {
			return
		}

		if args.CandidateId == rf.votedFor {
			reply.Term = args.Term
			reply.VoteGranted = true
			return
		}
		if rf.votedFor != -1 && args.CandidateId != rf.votedFor {
			return
		}

		//还有一种情况，没有投过票
	}

	if rf.currentTerm < args.Term {
		rf.currentTerm = args.Term
		rf.changeRole(Role_Follower)
		rf.votedFor = -1
		reply.Term = rf.currentTerm
		rf.persist()
	}

	//判断日志完整性
	if lastLogTerm > args.LastLogTerm || (lastLogTerm == args.LastLogTerm && lastLogIndex > args.LastLogIndex) {
		return
	}

	rf.votedFor = args.CandidateId
	reply.VoteGranted = true
	rf.changeRole(Role_Follower)
	rf.resetElectionTimer()
	rf.persist()
	DPrintf("%v， role：%v，voteFor: %v", rf.me, rf.role, rf.votedFor)
}

//
// example code to send a RequestVote RPC to a server.
// server is the index of the target server in rf.peers[].
// expects RPC arguments in args.
// fills in *reply with RPC reply, so caller should
// pass &reply.
// the types of the args and reply passed to Call() must be
// the same as the types of the arguments declared in the
// handler function (including whether they are pointers).
//
// The labrpc package simulates a lossy network, in which servers
// may be unreachable, and in which requests and replies may be lost.
// Call() sends a request and waits for a reply. If a reply arrives
// within a timeout interval, Call() returns true; otherwise
// Call() returns false. Thus Call() may not return for a while.
// A false return can be caused by a dead server, a live server that
// can't be reached, a lost request, or a lost reply.
//
// Call() is guaranteed to return (perhaps after a delay) *except* if the
// handler function on the server side does not return.  Thus there
// is no need to implement your own timeouts around Call().
//
// look at the comments in ../labrpc/labrpc.go for more details.
//
// if you're having trouble getting RPC to work, check that you've
// capitalized all field names in structs passed over RPC, and
// that the caller passes the address of the reply struct with &, not
// the struct itself.
//
func (rf *Raft) sendRequestVote(server int, args *RequestVoteArgs, reply *RequestVoteReply) {
	if server < 0 || server > len(rf.peers) || server == rf.me {
		panic("server invalid in sendRequestVote!")
	}

	rpcTimer := time.NewTimer(RPCTimeout)
	defer rpcTimer.Stop()

	ch := make(chan bool, 1)
	go func() {
		for i := 0; i < 10 && !rf.killed(); i++ {
			ok := rf.peers[server].Call("Raft.RequestVote", args, reply)
			if !ok {
				continue
			} else {
				ch <- ok
				return
			}
		}
	}()

	select {
	case <-rpcTimer.C:
		DPrintf("%v role: %v, send request vote to peer %v TIME OUT!!!", rf.me, rf.role, server)
		return
	case <-ch:
		return
	}

}

func (rf *Raft) startElection() {
	rf.mu.Lock()
	rf.resetElectionTimer()
	if rf.role == Role_Leader {
		rf.mu.Unlock()
		return
	}

	rf.changeRole(Role_Candidate)
	DPrintf("%v role %v,start election,term: %v", rf.me, rf.role, rf.currentTerm)

	lastLogTerm, lastLogIndex := rf.getLastLogTermAndIndex()
	args := RequestVoteArgs{
		CandidateId:  rf.me,
		Term:         rf.currentTerm,
		LastLogTerm:  lastLogTerm,
		LastLogIndex: lastLogIndex,
	}
	rf.persist()
	rf.mu.Unlock()

	allCount := len(rf.peers)
	grantedCount := 1
	resCount := 1
	grantedChan := make(chan bool, len(rf.peers)-1)
	for i := 0; i < allCount; i++ {
		if i == rf.me {
			continue
		}
		//对每一个其他节点都要发送rpc
		go func(gch chan bool, index int) {
			reply := RequestVoteReply{}
			rf.sendRequestVote(index, &args, &reply)
			gch <- reply.VoteGranted
			if reply.Term > args.Term {
				rf.mu.Lock()
				if reply.Term > rf.currentTerm {
					//放弃选举
					rf.currentTerm = reply.Term
					rf.changeRole(Role_Follower)
					rf.votedFor = -1
					rf.resetElectionTimer()
					rf.persist()
				}
				rf.mu.Unlock()
			}
		}(grantedChan, i)

	}

	for rf.role == Role_Candidate {
		flag := <-grantedChan
		resCount++
		if flag {
			grantedCount++
		}
		DPrintf("vote: %v, allCount: %v, resCount: %v, grantedCount: %v", flag, allCount, resCount, grantedCount)

		if grantedCount > allCount/2 {
			//竞选成功
			rf.mu.Lock()
			DPrintf("before try change to leader,count:%d, args:%+v, currentTerm: %v, argsTerm: %v", grantedCount, args, rf.currentTerm, args.Term)
			if rf.role == Role_Candidate && rf.currentTerm == args.Term {
				rf.changeRole(Role_Leader)
			}
			if rf.role == Role_Leader {
				rf.resetAppendEntriesTimersZero()
			}
			rf.persist()
			rf.mu.Unlock()
			DPrintf("%v current role: %v", rf.me, rf.role)
		} else if resCount == allCount || resCount-grantedCount > allCount/2 {
			DPrintf("grant fail! grantedCount <= len/2:count:%d", grantedCount)
			return
		}
	}

}

