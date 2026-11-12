package main

import (
	"fmt"
	"time"
)

func doWork(ch <-chan bool) {
	<-ch
	fmt.Println("channeled received")
}

func main() {
	ch := make(chan bool)
	go doWork(ch)
	time.Sleep(2 * time.Second)
	ch <- true
	time.Sleep(100 * time.Millisecond)
}
