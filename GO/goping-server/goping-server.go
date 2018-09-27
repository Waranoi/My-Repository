package main

import (
	"bufio"
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
		log.Println("Accept a connection request.")
		conn, err := listener.Accept()
		if err != nil {
			log.Println("Failed accepting a connection request:", err)
			continue
		}
		go ping(conn)
	}
}

func ping(conn net.Conn) {
	buf := make([]uint8, 1)
	reader := bufio.NewReader(conn)

	count := 5
	var elapsedTot float64
	for i := 0; i < count; i++ {
		start := time.Now()
		conn.Write([]byte{1})
		_, err := reader.Read(buf)
		elapsed := time.Since(start).Seconds() / 1000
		elapsedTot += elapsed
		if err != nil {
			log.Println("Failed while reading responce")
			return
		}
		if buf[0] == 1 {
			log.Println("Ping: (ms)", elapsed)
		}
	}

	log.Println("Average ping: (ms)", elapsedTot/float64(count))

}
