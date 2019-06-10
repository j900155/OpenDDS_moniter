#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2019 zack <zack@zack>
#
# Distributed under terms of the MIT license.

"""

"""
from  pymongo import MongoClient
import json
import socket

socketFile = "./connect.socket"
TopicLog = {"topic":"A","dataSize":0}

class insertMongo(object):
    def __inti__(self,mongoIP,dbName):
        self.IP= mongoIP
        self.dbName = dbName
    def connect(self):
        self.client = MongoClient(self.IP,27017)
        self.db = client[self.dbName]
    def insertData(self, data):
        return self.db.insert_one(data)
 class:
def main():
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    sock.bind(socketFile)
    sock.listen(30)
    clinetConnect, clientAddr = sock.accept()
    print (clientAddr)





if __name__=="__main__":
    main()
