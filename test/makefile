cc = gcc

.PHONY: clean

objtects = socketClient socketRecv recvDataTest
all: socketRecv socketClient recvDataTest
socketRecv:
	$(cc) -lpthread $@.c -o $@ 
socketClient:
	$(cc) $@.c -o $@
recvDataTest:
	$(cc) $@.c -o $@
clean:
	rm $(objtects)
