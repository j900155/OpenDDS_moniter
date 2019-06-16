#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2019 zack <zack@zack>
#
# Distributed under terms of the MIT license.

"""
data collect module

"""

import socket
import pymongo
import json
import time
import threading
import queue

"""
 recv data format sendSec/sendUsec/dataSize/pubID/topicName
"""
saveData = queue.Queue()
def saveToMongo():
    mongoClinet = pymongo.MongoClient("127.0.0.1",27017)
    collection = mongoClinet["test"]["topicLog"]
    print("saveToMongo")
    while(1):
        data = saveData.get()
        collection.find_one({"topic":data["topicName"]})

        data["insertTime"] = time.time()
        print(data)
        collection.insert(data)

def findFirstTopic(topic):
    print("topic ",topic)
def getClientData(conn):
    print("getClinetData")
    while(1):
        try:
            data = conn.recv(2048)
        except ConnectionError:
            print("ConnectError")
            break
        arrayData = str(data, encoding="utf8").split("/")
        print(arrayData)
        data = {"sendSec":0,"sendUsec":0,"dataSize":0,"PubID":[],"topicName":""}
        data["sendSec"]=arrayData[0]
        data["sendUsec"]=arrayData[1]
        data["dataSize"]=arrayData[2]
        data["PubID"]=arrayData[3].split(";")
        if data["PubID"][-1] == "":
            del data["PubID"][-1]
        data["topicName"]=arrayData[4]
        print(data["PubID"])
        if "first" in data["PubID"]:
            print("first topic {}".format( data["topicName"]))
            findFirstTopic(data["topicName"])
        saveData.put(data) 
        conn.send(b"OK")

def createSocket(HOST, collectPort):
    clientThread = []
    server = ""
    try:
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    except socket.err:
        msg = socket.err
        print("create socket error ", msg)
        return -1
    server.bind((HOST,collectPort))
    server.listen(10)
    print("collect socket start")
    mongoThread = threading.Thread(target=saveToMongo)
    mongoThread.start()
    while(1):
        conn, addr = server.accept()
        print("connect {} addr {}".format(conn, addr))
        t = threading.Thread(target=getClientData,args=[conn])
        t.start()
        clientThread.append(t)



def main():
    print("port ",6543)
    createSocket("0.0.0.0",6543)

if __name__=="__main__":
    main()
