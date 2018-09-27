package main

import (
	"log"
	"net"
	"time"
)

const (
	port = ":1609"
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
		go connHandler(conn)
	}
}

func connHandler(conn net.Conn) {
	log.Println("Connection accepted")

	start := time.Now()
	var dt float64

	lastSent := 3.0

	for {
		start = time.Now()

		if lastSent >= 3.0 {
			var beat uint8 = 3
			timestamp := start.Format("2018.09.27T23:59:59.999")
			conn.Write()
		}
		lastSent += dt

		dt = time.Since(start).Seconds()
	}
}
