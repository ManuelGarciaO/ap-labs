package main

import (
	"golang.org/x/tour/wc"
	"strings"
)

func WordCount(s string) map[string]int {
	var m map[string]int
	m = make(map[string]int)
	words := strings.Split(s, " ")
	for i := 0; i < len(words); i++ {
		m[words[i]]++
	}
	return m
}

func main() {
	wc.Test(WordCount)
}
