package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

const (
	numDataSources = 3 // 数据源数量
	dataCount      = 5 // 每个数据源发送的数据个数
)

func generateData(sourceID int, ch chan<- int, wg *sync.WaitGroup) {
	defer wg.Done()
	for i := 0; i < dataCount; i++ {
		data := rand.Intn(100) // 随机数据
		ch <- data
		fmt.Printf("DataSource %d produced %d\n", sourceID, data)
		time.Sleep(time.Millisecond * 500) // 模拟数据源产生数据的时间
	}
	close(ch) // 数据源完成后关闭自己的通道
}

func main() {
	rand.Seed(time.Now().UnixNano()) // 初始化随机数种子

	var wg sync.WaitGroup
	channels := make([]chan int, numDataSources) // 用于存放每个数据源的通道
	aggregateChannel := make(chan int)          // 聚合通道，用于收集所有数据源的结果

	// 启动多个数据源（goroutines）
	for i := 0; i < numDataSources; i++ {
		channels[i] = make(chan int)
		wg.Add(1)
		go generateData(i+1, channels[i], &wg)
	}

	// 启动一个协程来聚合数据
	go func() {
		// 使用select从多个通道收集数据
		for {
			activeSources := 0
			for _, ch := range channels {
				select {
				case data, ok := <-ch:
					if ok {
						aggregateChannel <- data
						activeSources++
					}
				}
			}
			if activeSources == 0 {
				break // 如果所有通道都关闭，则退出循环
			}
		}
		close(aggregateChannel) // 所有数据处理完成后关闭聚合通道
	}()

	// 从聚合通道收集最终结果
	go func() {
		for data := range aggregateChannel {
			fmt.Printf("Aggregated data: %d\n", data)
		}
	}()

	wg.Wait() // 等待所有数据源的生产者完成
	time.Sleep(time.Second * 1) // 确保所有数据处理完成后主程序退出
}

