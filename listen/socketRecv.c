/*
 * socketRecv.cpp
 */

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include "../listen/recvData.h"
#include<pthread.h>

#define socketFile "/var/lib/dds-mointer/server.sock"
#define serverIP "0.0.0.0"
#define clientLimit 10
void *getData(void *vClient)
{
	char buffer[600];
	int dataLen;
	int *client = (int*)vClient;
	struct recvData data;
	int charLen = 0;	
	int i,j;
	int socketClinet;
	int connectStatus = 0;
	struct sockaddr_in remote_addr;
	memset(&remote_addr, 0, sizeof(struct sockaddr_in));
	remote_addr.sin_family=AF_INET;
	remote_addr.sin_addr.s_addr=inet_addr(serverIP);
	remote_addr.sin_port=htons(6543);
	socketClinet = socket(AF_INET,SOCK_STREAM,0);
	if(connect(socketClinet,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr))<0)
	{
		printf("socketClinet connect err\n");
		connectStatus = 0;
	}
	else
	{
		connectStatus = 1;
	}
	while(connectStatus)
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
			printf("pubID %s\n",data.pubID);
			printf("topic %s\n",data.topicName);
			printf("usec %ld\n",data.dataSize);
			printf("sec %ld\n",data.sendSec);
			memset(buffer, 9 ,sizeof(buffer));
			if(data.dataSize <20)
			{
				charLen = strlen(data.pubID)-1;
				printf("char Len data.pubID %d\n",charLen);
				char *first = ";first";
				for(j=0;j< strlen(first); j++)
				{
					data.pubID[charLen+j]=first[j];
				}
				data.pubID[charLen+6]='\0';
				printf("%s\n\n",data.pubID);
				printf("first time\n");
			}
			sprintf(buffer, "%ld/%ld/%ld/%s/%s",data.sendSec, data.sendUsec, data.dataSize,
					data.pubID, data.topicName);
			charLen = strlen(buffer);
			printf("%s %d", buffer,charLen);
			send(socketClinet,buffer,charLen,0);

		}
		
	}
	printf("client %d end\n ",*client);
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
