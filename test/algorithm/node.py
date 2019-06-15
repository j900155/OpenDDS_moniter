#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2019 zack <zack@zack>
#
# Distributed under terms of the MIT license.

"""

"""
class node(object):
    def __init__(self,limit):
        self.limit = limit
        self.task = []
    def getTaskSum(self):
        return sum(self.task)
    def delTask(self,taskIndex):
        del self.task[taskIndex]
    def addTask(self, task):
        self.task.append(task)

    def overload(self):
        return sum(self.task) > self.limit
def main():
    pass

if __name__=="__main__":
    main()
