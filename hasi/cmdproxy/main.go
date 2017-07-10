package main

import (
	"bufio"
	"io"
	"io/ioutil"
	"log"
	"net/http"
	"os/exec"
	"strings"
)

func run(cmd string) {
	c := exec.Command("/bin/bash", "-s")
	c.Stdin = strings.NewReader(cmd)
	stdoutReader, err1 := c.StdoutPipe()
	if err1 != nil {
		log.Fatalf("exec.Command StdoutPipe Error: %v ; %s", err1, cmd)
	}
	stderrReader, err2 := c.StderrPipe()
	if err2 != nil {
		log.Fatalf("exec.Command StderrPipe Error: %v ; %s", err2, cmd)
	}
	go func() {
		r := bufio.NewReader(stdoutReader)
		for {
			line, _, err := r.ReadLine()
			if err == io.EOF || err == io.ErrClosedPipe {
				break
			} else if err != nil {
				panic(err)
			}
			log.Println("> " + string(line))
		}
	}()
	go func() {
		r := bufio.NewReader(stderrReader)
		for {
			line, _, err := r.ReadLine()
			if err == io.EOF || err == io.ErrClosedPipe {
				break
			} else if err != nil {
				panic(err)
			}
			log.Println("# " + string(line))
		}
	}()
	err3 := c.Run()
	if err3 != nil {
		log.Printf("exec.Command Run Error: %v ; %s\n", err3, cmd)
	} else {
		log.Printf("exec.Command Run Success ; %s\n", cmd)
	}
}

func handler(w http.ResponseWriter, r *http.Request) {
	body, err := ioutil.ReadAll(r.Body)
	if err != nil {
		log.Printf("handler ioutil.ReadAll Error: %v\n", err)
	}
	go run(string(body))
}

func main() {
	http.HandleFunc("/", handler)

	if err := http.ListenAndServe(":8080", nil); err != nil {
		log.Fatal(err)
	}
}
