#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include "../listen/recvData.h"
#include <unistd.h>
#include <sys/time.h>
#define socketFile "/var/lib/dds-mointer/server.sock"


int main(int argc, char *argv[])
{
	struct sockaddr_un addr;
	struct sockaddr_un clientAddr;
	char buffer[600];
	int ret;
	int server;
	int client;
	int dataLen;
	struct recvData data;
	int i;
	struct timeval tv;
	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, socketFile);

	client = socket(AF_UNIX, SOCK_STREAM, 0);
	if(client == -1)
	{
		printf("socket clinet error");
		return -1;
	}
	ret = connect(client, (struct sockaddr *) &addr, sizeof(struct sockaddr_un));
	if(ret == -1)
	{
		printf("connect error");
		return -1;
	}
	//strcpy(buffer,"1234");
	//dataLen = strlen(buffer);
	dataLen = sizeof(data);
	strcpy(data.pubID,"123;456");
	strcpy(data.topicName,"test");
	printf("%s%s\n",data.pubID, data.topicName);
	data.dataSize = 10;
	for(i = 0 ;i<10;i++)
	{
		gettimeofday(&tv,NULL);
		data.sendSec = tv.tv_sec;
		data.sendUsec = tv.tv_usec;
		memset(buffer, 0, sizeof(buffer));
		memcpy(buffer, &data, sizeof(data));
		//send(client, "1234",dataLen,0);
		send(client, buffer,sizeof(data),0);
		printf("dataLen %d buffer %s\n",dataLen, buffer);
		usleep(1000000);
	}
	send(client,0,sizeof(int),0);
	return 0;
}
