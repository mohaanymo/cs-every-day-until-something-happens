package main

import (
	"context"
	"fmt"
	"time"
)

func worker(ctx context.Context) {
	for {
		fmt.Println("worker")
		select {
		case <-ctx.Done():
			fmt.Println("context cancled")
			return
		default:
			time.Sleep(10 * time.Millisecond)
		}
	}
}

func main() {
	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()

	go worker(ctx)

	time.Sleep(3 * time.Second)

	cancel()
	time.Sleep(1 * time.Second)
}
