/*
 * recvData.h
 * Copyright (C) 2019 zack <zack@zack>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef RECVDATA_H
#define RECVDATA_H

struct recvData
{
	long sendSec;
	long sendUsec;
	long dataSize;
	char pubID[500];
	char topicName[10];
};

#endif /* !RECVDATA_H */
