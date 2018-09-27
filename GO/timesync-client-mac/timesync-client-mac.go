package main

import (
	"bufio"
	"encoding/binary"
	"log"
	"net"
	"time"
)

const (
	addr = "127.0.0.1:1666"
)

func main() {

	// Request sync
	log.Println("Connect to " + addr)
	conn, err := net.Dial("tcp", addr)
	if err != nil {
		log.Println("Connection err: ", err)
		return
	}
	log.Println("Succesfully connected ", addr)

	reader := bufio.NewReader(conn)
	buf := make([]uint8, 5)
	for {
		_, err = reader.Read(buf)
		if err != nil {
			log.Println("Sync read error")
			return
		}
		if buf[0] == 1 {
			stamp := int64(binary.LittleEndian.Uint64(buf[1:5]))
			log.Println("Syncing to " + time.Unix(0, stamp).String())
			conn.Write(buf[:1])
		}
	}

}
