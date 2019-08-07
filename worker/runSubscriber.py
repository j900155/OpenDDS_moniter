#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8

"""

"""
#setting topic by config.json

import multiprocessing
import subprocess
import time
# allTopic {"topic":{"status":0,"cmd":"./subscriber -DCPSConfigFile rtps.ini"}}

allTopic = {}
subscriber = {}

class runTopic(object):
    def __init__(self,topic: str,cmd: str):
        self.status = 0
        self.cmd = cmd
        self.process = None

    def runTopic(self) -> int:
        if self.status ==0:
            print("run topic")
            self.status = 1
        else:
            print("running")
        return 0

    def stopTopic(self) -> int:
        if self.status ==1:
            print("stop topic")
            return 0

    def statusTopic(self) -> int:
        return self.status

def runCollect(topic):
    if topic in allTopic:
        allTopic[topic].runTopic()
    else:
        allTopic[topic] = runTopic("123")
        allTopic[topic].runTopic()

def getAllStatus(topics) -> dict:
    tmp = {}
    for i in topics:
        tmp[i] = topics[i].status
    return tmp
"""
dict conf 
{"topic":A,"delayl"}
"""
def writeConfig(conf):
    pass
def main():
    topic = "A"
    allTopic[topic] = runTopic(topic,"123")
    allTopic[topic].runTopic()
    print(getAllStatus(allTopic))

if __name__=="__main__":
    main()
