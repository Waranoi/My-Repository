package main

import (
	"log"
	"net"
	"time"
	"bufio"
	"bytes"
	"image"
	"os"
	"image/jpeg"
)

const (
	addr = "127.0.0.1:1609"
	sec = 1
	bufSize = 10000000
	snapshotDelay = 0

	ECHO = 0
	JPEG = 1
	BEAT = 2
)

func main() {
	// Delay
	for i := 0; i < sec; i++ {
		log.Println("Dialing in ", sec-i)
		time.Sleep(2 * time.Second)
	}

	// Dial
	log.Println("Dial " + addr)
	conn, err := net.Dial("tcp", addr)
	if err != nil {
		log.Println("Dialing err: ", err)
		return
	}
	log.Println("Succesfully dialed ", addr)
	log.Println("Waiting for messages...")


	for {
		// Read msg
		for i := 0; i < snapshotDelay+1; i++ {
			buf := make([]uint8, bufSize)
			reader := bufio.NewReader(conn)
			n, err := reader.Read(buf)
			if err != nil {
				log.Println("aborting");
				return
			}

			if i == snapshotDelay {
				// Handle msg
				log.Println("Packet size: ", n);
				HandleMsg(buf, n, conn)
			}
		}
	}

	// Close the connection
	err = conn.Close()
	if err != nil {
		log.Println("Close connection error ", err)
	}
}

func HandleMsg(packet []uint8, size int, conn net.Conn) {
	var msgType uint8
	var msgLen int
	var msgBody []uint8
	var msgSize int
	
	msgType = packet[0]
	log.Println("msgType: ", msgType)

	switch msgType {
	case ECHO:
		msgLen = BytesToInt32(packet[1:5])
		msgBody = packet[5:5+msgLen]
		msgSize = 5 + msgLen
		HandleEcho(msgLen, msgBody)
	case JPEG:
		msgLen = BytesToInt32(packet[1:5])
		msgBody = packet[5:5+msgLen]
		msgSize = 5 + msgLen
		//HandleJPEG(msgLen, msgBody)
	case BEAT:
		msgSize = 1
		HandleHeartbeat(msgType, conn)
	default:
		log.Println("Unsupported type")		
	}

	// If header + message length is below packet size, there are more messages to be handled
	if (msgSize < size) {
		//HandleMsg(packet[msgSize:size], size-msgSize, conn)
	}
}

func HandleEcho(msgLen int, msg []uint8) {
	log.Println("msgLen: ", msgLen)
	log.Println("Echo:\n", string(msg))
}

func HandleJPEG(msgLen int, msg []uint8) {
	//w: 801, h: 358
	log.Println("msgLen: ", msgLen)

	img, _, err := image.Decode(bytes.NewReader(msg))
	if err != nil {
		panic(err)
	}

	f, err := os.Create("img.jpg")
	if err != nil {
		panic(err)
	}
	defer f.Close()
	jpeg.Encode(f, img, nil)
}

func HandleHeartbeat(heartbeat uint8, conn net.Conn) {
	log.Println("Heartbeat recieved, sending responce.")
	conn.Write([]byte{heartbeat})
}

func BytesToInt32(bytes []uint8) int {
	var result int
	for i := 0; i < len(bytes); i++ {
		result *= 10
		result += int(bytes[i] - '0')
	}
	return result
}