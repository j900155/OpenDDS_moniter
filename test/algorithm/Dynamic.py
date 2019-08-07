#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2019 zack <zack@zack>
#
# Distributed under terms of the MIT license.

"""

"""
import random
import node

swapCount = 0

def changePoint(array, point1, point2):
    tmp = array[point1]
    array[point1] = array[point2]
    array[point2] = tmp
    return array

def dynamicSwap(nodeList):
    global swapCount
    nodeList[0].task.sort()
    nodeTaskPoint = len(nodeList[0].task)-1
    checkTmp = [nodeTaskPoint,0,500] # [mainChangeTaskPoint,node,size]
    tmpLimit = -1
    while( nodeList[0].overload()):
        print(nodeList[0].task)
        for TaskNode in range(1,len(nodeList)):
            nodeTaskPoint = len(nodeList[0].task)-1
            if nodeTaskPoint <0:
                break
            if nodeList[TaskNode].getTaskSum() ==0:
                #print("32 TaskNode", TaskNode,"checkTmp",checkTmp)
                checkTmp[0] = len(nodeList[0].task)-1
                nodeList[TaskNode].addTask(nodeList[0].task[checkTmp[0]])
                nodeList[0].delTask(checkTmp[0])
                checkTmp[1] = TaskNode
                checkTmp[2] = -1
                break
            else:
                #print("TaskNode task", TaskNode)
                #print("nodeList[0]",nodeList[0].task)
                while(nodeTaskPoint>-1 and nodeList[0].overload):
                    tmpLimit = nodeList[TaskNode].limit-(nodeList[TaskNode].getTaskSum()+nodeList[0].task[nodeTaskPoint])
                    #print("TaskNode ",TaskNode," nodeTaskPoint ", nodeTaskPoint)
                    if( tmpLimit>=0 and tmpLimit<checkTmp[2] ):
                        checkTmp[0] = nodeTaskPoint
                        checkTmp[1] = TaskNode
                        checkTmp[2] = tmpLimit
                        print("tmpLimit", tmpLimit, " coheckTmp ",checkTmp)
                        if checkTmp[2] ==0:
                            break

                    nodeTaskPoint +=-1
                #if tmpLimit >-1:
                print("checkTmp ", checkTmp)
                nodeList[checkTmp[1]].addTask(nodeList[0].task[checkTmp[0]])
                nodeList[0].delTask(checkTmp[0])
                swapCount+=1
                checkTmp[2] = 500
                #nodeTaskPoint +=-1

            #print("nodeList[0]",nodeList[0].task)
            print("TaskNode ", nodeList[TaskNode].task, " checkTmp ", checkTmp)
        for i in range(0,len(nodeList)):
            print(i ," ",nodeList[i].task, " ", nodeList[i].getTaskSum())

    for i in range(0,len(nodeList)):
        print(i ," ",nodeList[i].task, " ", nodeList[i].getTaskSum())

    print("-------------") 


    return nodeList
def main():
    global swapCount
    limit = 10
    nodeList = []
    taskNumber = 0
    moveTask=0
    for i in range(4):
        nodeList.append(node.node(limit))
        print ("all node ",nodeList[i].limit)
    task = [1,2,3,4,5,6,7,8]
    random.seed()
    
    for i in range(8):
        j = random.randrange(0,8)
        task = changePoint(task,i,j)

    print ("task",task)
    for i in task:
        nodeList[0].task.append(i)
        nodeList = dynamicSwap(nodeList)
        #print(nodeList)
    print("-------------") 
    for i in nodeList:
        print(i.task, " ", i.getTaskSum())
    print("swapCount ", swapCount)

if __name__=="__main__":
    main()
