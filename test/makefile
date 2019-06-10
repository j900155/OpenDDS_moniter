cc = gcc

all: buildServer buildClient buildDataTest
buildServer:
	$(cc) -lpthread socketRecv.c -o socketRecv.o 
buildClient:
	$(cc) socketClinet.c -o socketClinet.o
buildDataTest:
	$(cc) recvDataTest.c -o recvDataTest.o 
