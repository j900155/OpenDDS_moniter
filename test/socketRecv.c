/*
 * socketRecv.cpp
 */

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include "../listen/recvData.h"
#include<pthread.h>

#define socketFile "/var/lib/dds-mointer/server.sock"
#define clientLimit 10
void *getData(void *vClient)
{
	char buffer[600];
	int dataLen;
	int *client = (int*)vClient;
	struct recvData data;
	while(1)
	{
		dataLen = recv(*client, buffer,sizeof(buffer),0);
		printf("dataLen %d\n",dataLen);
		if (0 == dataLen)
		{
			printf("client end\n");
			break;
		}
		else if(dataLen == sizeof(struct recvData))
		{
			printf("get struct\n");
			memcpy(&data, buffer, sizeof(struct recvData));
			printf("%s\n",data.pubID);
			printf("%s\n",data.topicName);
			printf("%ld\n",data.dataSize);
			printf("%ld\n",data.sendSec);
			printf("%ld\n",data.sendUsec);
		}
		else
		{
			buffer[dataLen+1]='\0';
			printf("clinet %d dataLen %d buffer %s\n",*client, dataLen, buffer);
		}
		
	}
	return 0;
}
int main(int argc, char *argv[])
{
	struct sockaddr_un addr;
	struct sockaddr_un clientAddr;
	char buffer[100];
	int ret;
	int server;
	int client[clientLimit];
	int dataLen;
	int clientNumber;
	struct recvData data;
	pthread_t thread[clientLimit];

	unlink(socketFile);

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, socketFile);

	server= socket(AF_UNIX, SOCK_STREAM, 0);
	if(ret == -1)
	{
		printf("connect error");
		return -1;
	}
	if(bind(server, (struct sockaddr *) &addr, sizeof(struct sockaddr_un))==-1)
	{
		printf("bind error");
		return -1;
	}
	listen(server, clientLimit);
	int i = sizeof(struct sockaddr_un);
	printf("server created\n");
	while(1)
	{
		client[clientNumber] = accept(server,(struct sockaddr *) &clientAddr, &i);
		printf("new clinet %d \n",client[clientNumber]);
		pthread_create(&thread[clientNumber],NULL,getData, &client[clientNumber]);
		if (clientNumber < clientLimit)
		{
			clientNumber ++;
		}
	}
	return 0;
}
