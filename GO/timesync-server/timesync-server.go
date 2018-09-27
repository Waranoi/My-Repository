package main

import (
	"bufio"
	"encoding/binary"
	"log"
	"net"
	"time"
)

const (
	port = ":1666"
)

func main() {
	var err error
	listener, err := net.Listen("tcp", port)
	if err != nil {
		log.Println("Unable to listen on port ", port)
		return
	}

	log.Println("Listen on", listener.Addr().String())
	for {
		log.Println("Waiting for sync request.")
		conn, err := listener.Accept()
		if err != nil {
			log.Println("Failed to accept sync request:", err)
			continue
		}

		log.Println("Sync request accepted")
		go sync(conn)
	}
}

func sync(conn net.Conn) {
	reader := bufio.NewReader(conn)
	sendBuf := make([]uint8, 5)
	sendBuf[0] = 1

	for {
		// Send timestamp
		binary.LittleEndian.PutUint64(sendBuf[1:5], uint64(time.Now().UnixNano()))
		conn.Write(sendBuf)

		// Confirm that timestamp was recieved
		rec := make([]uint8, 1)
		_, err := reader.Read(rec)
		if err != nil {
			log.Println("Sync stopped")
			return
		}
		if rec[0] == 1 {
			log.Println("Sync sent")
		}

		// Sleep
		time.Sleep(1 * time.Minute)
	}
}
