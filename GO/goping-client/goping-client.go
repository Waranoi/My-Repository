package main

import (
	"bufio"
	"log"
	"net"
)

const (
	addr = "127.0.0.1:1609"
)

func main() {

	// Dial
	log.Println("Dial " + addr)
	conn, err := net.Dial("tcp", addr)
	if err != nil {
		log.Println("Dialing err: ", err)
		return
	}
	log.Println("Succesfully dialed ", addr)

	buf := make([]uint8, 1)
	reader := bufio.NewReader(conn)
	_, err = reader.Read(buf)
	if err != nil {
		log.Println("aborting")
		return
	}
	if buf[0] == 1 {
		log.Println("Pinging back msg")
		conn.Write(buf)
	}
}
