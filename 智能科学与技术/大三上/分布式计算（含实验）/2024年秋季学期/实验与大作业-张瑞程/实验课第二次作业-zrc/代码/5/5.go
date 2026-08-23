package main

import (
	"context"
	"fmt"
	"math/rand"
	"sync"
	"time"
)

const (
	numTasks = 3              // 任务数量
	totalTimeout = 10 * time.Second // 总超时时间
)

// 模拟一个任务函数，任务随机执行 1 到 5 秒钟
func task(id int, duration time.Duration, ctx context.Context, wg *sync.WaitGroup, totalTime *time.Duration, mu *sync.Mutex) {
	defer wg.Done()

	fmt.Printf("Task %d started, will run for %v\n", id, duration)

	select {
	case <-time.After(duration): // 模拟任务执行完成
		mu.Lock()
		*totalTime += duration
		mu.Unlock()
		fmt.Printf("Task %d completed\n", id)
	case <-ctx.Done(): // 如果 context 被取消，提前终止任务
		fmt.Printf("Task %d cancelled: %v\n", id, ctx.Err())
	}
}

func main() {
	rand.Seed(time.Now().UnixNano()) // 随机数种子

	// 创建一个带取消功能的 context
	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()

	var wg sync.WaitGroup
	var mu sync.Mutex
	totalTime := time.Duration(0) // 记录所有任务的总时间

	taskDurations := []time.Duration{
		time.Duration(rand.Intn(5)+1) * time.Second,
		time.Duration(rand.Intn(5)+1) * time.Second,
		time.Duration(rand.Intn(5)+1) * time.Second,
	}

	// 启动多个任务
	for i := 0; i < numTasks; i++ {
		wg.Add(1)
		go task(i+1, taskDurations[i], ctx, &wg, &totalTime, &mu)
	}

	// 监控总超时时间
	go func() {
		wg.Wait()
		if totalTime > totalTimeout {
			cancel() // 超过总时间，取消所有任务
		}
	}()

	// 等待所有任务完成
	wg.Wait()

	// 输出最终信息
	if totalTime > totalTimeout {
		fmt.Printf("Tasks cancelled: Total time %v exceeded %v\n", totalTime, totalTimeout)
	} else {
		fmt.Printf("All tasks completed successfully in %v\n", totalTime)
	}
}

