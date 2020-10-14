package main

import (
	"bufio"
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"strings"
)

func displayClock(dst io.Writer, src io.Reader, city string) {
	scan := bufio.NewScanner(src)
	for scan.Scan() {
		fmt.Printf("%s\t: %s\n", city, scan.Text())
	}
}

func main() {
	for i := 1; i < len(os.Args); i++ {
		arr := strings.Split(os.Args[i], "=")
		if len(arr) != 2 {
			fmt.Println("Invalid arguments")
			return
		}
		conn, err := net.Dial("tcp", arr[1])
		if err != nil {
			log.Fatal(err)
		}
		defer conn.Close()
		go displayClock(os.Stdout, conn, arr[0])
	}
	ch := make(chan int)
	<-ch
}