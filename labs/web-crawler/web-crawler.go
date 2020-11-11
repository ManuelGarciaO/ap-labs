// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 241.

// Crawl2 crawls web links starting with the command-line arguments.
//
// This version uses a buffered channel as a counting semaphore
// to limit the number of concurrent calls to links.Extract.
//
// Crawl3 adds support for depth limiting.
//
package main

import (
	"fmt"
	"log"
	"os"
	"flag"

	"gopl.io/ch5/links"
)

type url_depth struct {
	urls  []string
	depth int
}

//!+sema
// tokens is a counting semaphore used to
// enforce a limit of 20 concurrent requests.
var tokens = make(chan struct{}, 20)

func crawl(url string,file *os.File) []string {
	fmt.Fprintln(file,url)
	tokens <- struct{}{} // acquire a token
	list, err := links.Extract(url)
	<-tokens // release the token

	if err != nil {
		log.Print(err)
	}
	return list
}

//!-sema

//!+
func main() {

	depthIn := flag.Int("depth", -1, "web crawler depth")
	results := flag.String("results","","file with the logs")
	flag.Parse()
	url := flag.Args()
	depth := *depthIn + 1
	resultFile := *results
	
	if len(url) == 0 {
		fmt.Println("You must specify an URL.")
		return
	}
	if(*depthIn == -1 || resultFile == ""){
		fmt.Println("Error. Incomplete parameters")
		return
	}

	worklist := make(chan url_depth)
	var n int // number of pending sends to worklist

	// Start with the command-line arguments.
	n++
	go func() { worklist <- url_depth{urls: url, depth: depth} }()

	// Create and open file
	f, err := os.Create(resultFile)
	if err != nil{
		fmt.Println(err)
		return
	}
	
	// Crawl the web concurrently.
	seen := make(map[string]bool)
	for ; n > 0; n-- {
		list := <-worklist
		for _, link := range list.urls {
			if !seen[link] && list.depth != 0{
				seen[link] = true
				n++
				go func(link string) {
					worklist <- url_depth{urls: crawl(link,f), depth: list.depth - 1}
				}(link)
			}
		}
	}
	// Close file
	f.Close()
}

//!-
