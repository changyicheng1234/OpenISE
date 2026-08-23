package main

import (
	"fmt"
	"sync"
)

// Map 阶段：将字符串映射为其长度
func mapPhase(words []string, results chan<- int, wg *sync.WaitGroup) {
	defer wg.Done()
	for _, word := range words {
		results <- len(word)
	}
}

// Reduce 阶段：汇总所有长度
func reducePhase(results <-chan int, wg *sync.WaitGroup) int {
	total := 0
	for length := range results {
		total += length
	}
	wg.Done()
	return total
}

func mapReduce(words []string) int {
	// 创建通道和 WaitGroup
	results := make(chan int, len(words))
	var mapWg sync.WaitGroup
	var reduceWg sync.WaitGroup

	// Map 阶段
	mapWg.Add(1)
	go mapPhase(words, results, &mapWg)

	// Reduce 阶段
	reduceWg.Add(1)
	var total int
	go func() {
		total = reducePhase(results, &reduceWg)
	}()

	// 等待 Map 阶段完成并关闭通道
	mapWg.Wait()
	close(results)

	// 等待 Reduce 阶段完成
	reduceWg.Wait()
	return total
}

func main() {
	words := []string{"Where", "did", "I", "put", "my", "lighter"}
	totalLength := mapReduce(words)
	fmt.Printf("Total length: %d\n", totalLength)
}
