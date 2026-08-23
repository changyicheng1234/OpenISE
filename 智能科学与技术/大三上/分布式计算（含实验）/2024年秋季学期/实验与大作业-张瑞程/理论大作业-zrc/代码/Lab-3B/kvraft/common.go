package kvraft

const (
	OK             = "OK"
	ErrNoKey       = "ErrNoKey"
	ErrWrongLeader = "ErrWrongLeader"
	ErrTimeOut     = "ErrTimeOut"
	PUT            = "Put"
	APPEND         = "Append"
	GET            = "Get"
)


type Err string //标识错误信息,在上面常量自己定义

// Put or Append
type PutAppendArgs struct {
	Key   string
	Value string
	Op    string // "Put" or "Append"
	// You'll have to add definitions here.
	//2A
	MsgId    int64 //用于唯一标识一个Args
	ClientId int64
}

type PutAppendReply struct {
	Err Err
}

type GetArgs struct {
	Key string
	// You'll have to add definitions here.
	//2A 暂时不知道干嘛的
	MsgId    int64 //同样添加标识符
	ClientId int64
}

type GetReply struct {
	Err   Err //string，标注错误信息
	Value string
}
