#! /usr/bin/env python3
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#

"""

"""


import random
import node

swapCount=0

def changePoint(array, point1, point2):
    tmp = array[point1]
    array[point1] = array[point2]
    array[point2] = tmp
    return array

def greddySwap(nodeList,task):
    global swapCount
    for i in task:
            nodeList[0].task.append(i)
            print(nodeList[0].task)
            print(nodeList[0].getTaskSum())
            if(nodeList[0].limit < nodeList[0].getTaskSum()):
                nodeList[0].task.sort()
                moveTask = len(nodeList[0].task)-1
                while nodeList[0].overload():
                    print("moveTask ",moveTask,"node 0 task ",nodeList[0].task[moveTask])
                    for j in range(1,len(nodeList)):
                        if ((nodeList[j].getTaskSum()+nodeList[0].task[moveTask]) <= nodeList[j].limit):
                            print("moveTask ", moveTask)
                            nodeList[j].addTask(nodeList[0].task[moveTask])
                            nodeList[0].delTask(moveTask)
                            moveTask=-1
                            break

                    if moveTask == -1:
                        swapCount +=1
                        moveTask = len(nodeList[0].task)-1
                        for i in nodeList:
                            print(i.task, " ", i.getTaskSum())
                    else:
                        moveTask+=-1
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
    nodeList = greddySwap(nodeList,task)
    print("-------------") 
    for i in nodeList:
        print(i.task, " ", i.getTaskSum())
    print("swapCount ", swapCount)
if __name__=="__main__":
    main()
