package main

import (
	"fmt"
	"runtime"
	"time"
)

func main() {

	var memoryStats runtime.MemStats
	runtime.ReadMemStats(&memoryStats)
	pipe := make(chan int)
	maxStages := 0

	defer func() {
		if goroutine := recover(); goroutine != nil {
			fmt.Println("Maximum number of pipeline stages   : ", maxStages)
			initialTime := time.Now()
			for ; maxStages > 0; maxStages-- {
				pipe <- maxStages
			}
			duration := time.Since(initialTime)
			fmt.Println("Time to transit trough the pipeline : ", duration)
		}

	}()
	for {
		go func() {
			maxStages++
			<-pipe
		}()
		runtime.ReadMemStats(&memoryStats)
		if memoryStats.TotalAlloc/1024/1024 > 256 {
			panic("Out of memory")
		}
	}
}