#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8

"""

"""
import pymongo
import json
import time
def main():
    mongoClinet = pymongo.MongoClient("127.0.0.1",27017)
    collection = mongoClinet["test"]["test"]
    for i in collection.find({}):
        print(i)
    data = {"sendSec":0,"sendUsec":0,"dataSize":0,"PubID":['1','2'],"topicName":"test"}
    collection.insert(data)


if __name__=="__main__":
    main()
