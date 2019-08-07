#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2019 zack <zack@zack>
#
# Distributed under terms of the MIT license.

"""

"""
from  Dynamic import dynamicSwap,changePoint
from greedy import greedySwap
import node
import random
import time
def changePoint(array, point1, point2):
    tmp = array[point1]
    array[point1] = array[point2]
    array[point2] = tmp
    return array


def main():
    limit = 4.6
    nodeList = []
    nodeList2 = []
    taskNumber = 0
    moveTask=0
    workerNumber = 11
    startTime = 0
    for i in range(workerNumber):
        nodeList.append(node.node(limit))
        nodeList2.append(node.node(limit))
        print ("all node ",nodeList[i].limit)
    task = [0.2,0.6,1.4,1,2,2.6,2.6,2.6,2.6,3,3.2,3.4,3.6,3.6,4.6]
    taskNumber = len(task)
    random.seed()
    
    for i in range(taskNumber):
        j = random.randrange(0,taskNumber)
        task = changePoint(task,i,j)

    print ("task",task)
    print ("Dynamic start =========")
    for i in task:
        startTime = time.time()
        nodeList[0].task.append(i)
        nodeList = dynamicSwap(nodeList)
        print("run time %f"%(time.time()-startTime))
    print("greedy start ==========")
    for i in task:
        nodeList2[0].task.append(i)
        nodeList2 = greedySwap(nodeList2)
    print("-------------end") 
    print ("task",task)
    for i in nodeList:
        print(i.task, " ", i.getTaskSum())

    print("-------------") 
    for i in nodeList2:
        print(i.task, " ", i.getTaskSum())


if __name__=="__main__":
    main()
