package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

const (
	numProducers = 3 // 生产者数量
	numConsumers = 3 // 消费者数量
	numItems     = 10 // 每个生产者生产的随机数个数
)

func producer(id int, ch chan<- int, wg *sync.WaitGroup) {
	defer wg.Done()
	for i := 0; i < numItems; i++ {
		num := rand.Intn(100)
		ch <- num // 向通道发送数据
		fmt.Printf("Producer %d produced %d\n", id, num)
		time.Sleep(time.Millisecond * 500) // 模拟生产过程的延迟
	}
}

func consumer(id int, ch <-chan int, wg *sync.WaitGroup) {
	defer wg.Done()
	for num := range ch { // 从通道中读取数据，直到通道关闭
		fmt.Printf("Consumer %d consumed %d\n", id, num)
		time.Sleep(time.Millisecond * 500) // 模拟消费过程的延迟
	}
}

func main() {
	rand.Seed(time.Now().UnixNano()) // 初始化随机数种子
	ch := make(chan int, 10)        // 带缓冲的通道，缓冲大小为10

	var producerWG sync.WaitGroup
	var consumerWG sync.WaitGroup

	// 启动生产者
	for i := 1; i <= numProducers; i++ {
		producerWG.Add(1)
		go producer(i, ch, &producerWG)
	}

	// 启动消费者
	for i := 1; i <= numConsumers; i++ {
		consumerWG.Add(1)
		go consumer(i, ch, &consumerWG)
	}

	// 启动一个协程等待所有生产者完成，并关闭通道
	go func() {
		producerWG.Wait() // 等待生产者完成
		close(ch)         // 关闭通道，通知消费者不再有新数据
	}()

	consumerWG.Wait() // 等待所有消费者完成
}
