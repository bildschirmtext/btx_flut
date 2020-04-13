#!/bin/bash

#Port to listen for UDP packets
UDPPORT=1234

#Port the Terminal will connect to
TPORT=12345


gcc -o btx-flut btx-flut.c

while true
do
	./btx-flut $UDPPORT | nc -l -p $TPORT
done
