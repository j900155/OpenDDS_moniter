#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2019 zack <zack@zack>
#
# Distributed under terms of the MIT license.

"""

"""

import socket
import threading
from runSubscriber import runTopic
HOST = "0.0.0.0"
PORT = 3215
serverID = ""
allTopic = {}
def RunServer(conn):
    global allTopic
    point = -1
    while(1):
        recvData = conn.recv(1024)
        if len(recvData) <1:
            break
        s = str(recvData, encoding="utf-8")
        res = "err"
        if "create:" in s:
            topic = ""
            print("create topic")
            point = s.find(":")+1
            topic = s[point:]
            print(topic)
            if topic in allTopic:
                allTopic[topic].runTopic()
                res = "run:"+topic
            else:
                allTopic[topic] = runTopic(topic,"cmd")
                allTopic[topic].runTopic()
                res = "create:"+topic

        elif "delete:" in s:
            print("del topic")
            point = s.find(":")+1
            topic = s[point:]
            print(topic)
            if topic in allTopic:
                allTopic[topic].stopTopic()
                res = "stop:"+topic
            else:
                res = "stop:NULL"

        elif "status:" in s:
            print("get status")
            res = "status:"
            for i in allTopic:
                res+= str(i) + "/" + str(allTopic[i].status)
            
        conn.send(res.encode())
def main():
    global serverID
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((HOST, PORT))
    server.listen(5)
    while(1):
        conn, addr = server.accept()
        data = conn.recv(1024)
        serverID = str(data, encoding="utf-8") 
        print(serverID)
        conn.send(serverID.encode())
        RunServer(conn)

if __name__=="__main__":
    main()
