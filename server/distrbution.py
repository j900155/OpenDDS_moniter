#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2019 zack <zack@zack>
#
# Distributed under terms of the MIT license.

"""

"""

from flask import Flask
import pymongo
import json
from algorithm import node

mongoDB = ""
nodeList = []
runningTopic = []
app = Flask(__name__)

@app.route("/")
def home():
    return "home"

@app.route("/updateDistrubtion")
def updateDistrubtion():
    for topic in runningTopic:
        print(topic)
        topicData = mongoDB["topicLog"].find({"topicName":topic}).sort([("insertTime",-1)]).limit(2)
        print(topicData[0])
        #TODO count task by dataSize and run greddy 
    return "new distrubtion"

@app.route("/getNode")
def getNode():
    mongoData = mongoDB["swarmNode"].find()
    allNode = []
    for i in mongoData:
        allNode.append(i)
    print(allNode)
    return str(allNode)

@app.route("/newTopic/<topic>")
def newTopic(topic):
    newTopic = 0
    if topic in runningTopic:
        newTopic = 1
    else:
        nowTopic = 0
        runningTopic.append(topic)
        nodeList[0].addTask([topic,20])
    return str(newTopic)

@app.route("/runTopic")
def runTopic():
    return str(runningTopic)
def newDistrubtion():
    pass

def dbInit(configFile):
    global mongoDB
    with open(configFile,"r") as f:
        config = json.load(f)

    mongoClient = pymongo.MongoClient(config["ip"],config["port"])
    mongoDB = mongoClient[config["db"]]

def NodeInit():
    global nodeList
    mongoData = mongoDB["swarmNode"].find({"rule":"worker"})
    mongoData.sort([("nodeID",pymongo.ASCENDING)])
    for i in mongoData:
        print("node ID {} limkt {}".format(i["nodeID"],i["limit"]))
        limit = i["limit"]
        nodeList.append(node.node(limit))

if __name__ == "__main__":
    dbInit("./db.json")
    NodeInit()
    app.run(host="0.0.0.0",port=5000, debug=True)
