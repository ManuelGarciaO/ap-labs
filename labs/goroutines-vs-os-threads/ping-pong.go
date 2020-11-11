package main

import (
	"fmt"
	"time"
)

func putBack(in chan int, out chan int) {
	for {
		out <- (1 + <-in)
	}
}

func main() {

	chan1 := make(chan int)
	chan2 := make(chan int)
	go putBack(chan1, chan2)
	go putBack(chan2, chan1)
	communications := 0

	for i := 0; i < 1; i++ {
		chan1 <- 0
		time.Sleep(time.Duration(1) * time.Second)
		messages := <-chan1 
		communications += messages
	}

	fmt.Println("Communications Per Second : ", communications)
}