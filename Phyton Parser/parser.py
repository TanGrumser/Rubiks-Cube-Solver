from contextlib import nullcontext
from logging import addLevelName
from pickletools import uint8
from tokenize import Number, String
import matplotlib.pyplot as plt

import re
import sys

from numpy import may_share_memory


class Entry:
    def __init__(self, depth: int, time: float, states: int, duplciates: int):
        self.depth = depth
        self.time = time
        self.states = states
        self.duplicates = duplciates

class Solve:
    def __init__(self, shuffle: str, solution: str, totalTime: float, steps: list[Entry]):
        self.shuffle = shuffle
        self.solution = solution
        self.totalTime = totalTime
        self.totalTimeEffective = 0
        self.steps = steps
        self.distance = solution.count(" ") + 1
        
        self.totalStates = 0
        self.totalDuplicates = 0
        
        for step in steps:
            self.totalStates += step.states
            self.totalDuplicates += step.duplicates
            self.totalTimeEffective += step.time

    def toSting(self):
        string = "shuffle: " + self.shuffle + "solution: " + self.solution + "\ntotal solve time: " + str(self.totalTime) + "total states: " + str(self.totalStates) + "\ntotal duplicates: " + str(self.totalDuplicates) + "\n"

        for step in self.steps:
            string += "depth: " + str(step.depth) + ", time: " + str(step.time) + ", states: " + str(step.states) + ", duplicates: " +str(step.duplicates) + "\n"

        return string

def getMedianOfStates(solves: list[Solve]):
    solves.sort(key=lambda x: x.totalStates)
    return solves[int(len(solves) / 2)].totalStates

def getNodesPerSecondSingle(solve: Solve):
    return solve.totalStates / solve.totalTime

def getNodesPerSecond(solves: list[Solve]):
    totalTime = 0
    totalStates = 0

    for solve in solves:
        totalTime += solve.totalTime
        totalStates += solve.totalStates

    return totalStates / totalTime

def getNodesPerSecondEffective(solves: list[Solve]):
    totalTime = 0
    totalStates = 0

    for solve in solves:
        totalTime += solve.totalTimeEffective
        totalStates += solve.totalStates

    return totalStates / totalTime

def getMinMaxOfStates(solves: list[Solve]):
    solves.sort(key=lambda x: x.totalStates)
    return solves[0].totalStates, solves[len(solves) - 1].totalStates

def getAverageOfStates(solves: list[Solve]):
    sum = 0
    for solve in solves:
        sum += solve.totalStates
    return sum / len(solves)

def getAverageOfDistance(solves: list[Solve]):
    sum = 0
    for solve in solves:
        sum += solve.distance
    
    return float(sum) / float(len(solves))

def getMedianOfDistance(solves: list[Solve]):
    solves.sort(key=lambda x: x.distance)
    return solves[int(len(solves) / 2)].distance

def getMedianOfTime(solves: list[Solve]):
    solves.sort(key=lambda x: x.totalTime)
    return solves[int(len(solves) / 2)].totalTime

def getAverageOfTime(solves: list[Solve]):
    sum = 0.0
    for solve in solves:
        sum += solve.totalTime
    
    return float(sum) / float(len(solves))

def addlabels(x, y, labels):
    for i in range(len(x)):
        plt.text(x[i], y[i], labels[i], ha = 'center')

def showDistanceDistributionRelative(solves: list[Solve]):
    x = [13, 14, 15, 16, 17, 18, 19, 20]
    y = [0, 0, 0, 0, 0, 0, 0, 0]
    labels = []
    percentages = []

    for solve in solves:
        y[solve.distance - 13] += 1

    for count in y:
        percentages.append(float(count) / float(len(solves)))
        labels.append(str(round(float(count) / float(len(solves)), 2)) + "")
    
    plt.bar(x, percentages)

    plt.title("Distance Distribution Relative (5000 solves)")
    #plt.margins(y=0.3)
    plt.xlabel("distance")
    plt.ylabel("density")
    
    addlabels(x, percentages, labels)
    plt.show()

def showStatesAtDepth(solves: list[Solve]):
    depthValues = [[] for x in range(7)]
    pos = [14, 15, 16, 17, 18, 19, 20]
    x = []
    y = []
    labels = []

    for solve in solves:
        x.append(solve.distance)
        y.append(solve.totalStates)
        depthValues[solve.distance - 14].append(solve.totalStates)

    #plt.scatter(x, y)

    plt.title("States at Depth")
    plt.xlabel("Distance (moves)")
    plt.ylabel("Traversed States")
    
    #addlabels(x, percentages, labels)
    plt.boxplot(depthValues, positions=pos)
    plt.show()

def showTimesAtDepth(solves: list[Solve]):
    depthValues = [[] for x in range(7)]
    pos = [14, 15, 16, 17, 18, 19, 20]
    x = []
    y = []
    labels = []

    for solve in solves:
        x.append(solve.distance)
        y.append(solve.totalTime)
        depthValues[solve.distance - 14].append(solve.totalTime)

    #plt.scatter(x, y, c="grey", s=150, edgecolors='none', cmap = 'jet', marker='s')

    plt.title("Runtimes at Depth")
    plt.xlabel("Distance (moves)")
    plt.ylabel("Runtime (seconds)")
    
    #addlabels(x, percentages, labels)
    plt.boxplot(depthValues, positions=pos)
    plt.show()

def showDistanceDistributionAbsolute(solves: list[Solve]):
    x = [13, 14, 15, 16, 17, 18, 19, 20]
    y = [0, 0, 0, 0, 0, 0, 0, 0]
    labels = []

    for solve in solves:
        y[solve.distance - 13] += 1

    for count in y:
        labels.append(count)
    
    plt.bar(x, y)

    plt.title("Distance Distribution Absoulte (5000 solves)")

    plt.xlabel("distance")
    plt.ylabel("occuerences")
    
    addlabels(x, y, labels)

    plt.show()

def printTable(solves : list[Solve]):
    solves.sort(key=lambda x: x.distance)
    for solve in solves:
        print(str(solve.distance) + " & " + str(solve.totalStates) + " & " + str(round(solve.totalTime, 2)))

def printStats(solves : list[Solve]):
    print("Median Time: ", getMedianOfTime(solves))
    print("Average Time: ", getAverageOfTime(solves))
    print("Median Nodes: ", getMedianOfStates(solves))
    print("Average Nodes: ", getAverageOfStates(solves))
    print("MedianDistance: ", getMedianOfDistance(solves))
    print("Average Distance: ", getAverageOfDistance(solves))
    print("Nodes/s: ", getNodesPerSecond(solves))
    print("Nodes/s (eff.): ", getNodesPerSecondEffective(solves))
    min, max = getMinMaxOfStates(solves)
    print("Min, Max: ", min, max)

def printStatsTable(solvesList : list[list[Solve]]):
    functions = [getMedianOfTime, getAverageOfTime, getMedianOfStates, getAverageOfStates, getNodesPerSecond]

    for func in functions:
        line = ""
        for i in range(len(solvesList)):
            line += str(round(func(solvesList[i]), 2))
            
            if i != len(solvesList) - 1:
                line += " & "
            else:
                line += " \\\\"
            
        print(line)
    
def main():
    solves = collectData("variosDSLog20.txt")
    #solvesNoDs = collectData("newFiveLogNoDuplicates.txt")

    index7 = solves[0 : 10]
    index8 = solves[10 : 20]
    turn7 = solves[20 : 30]
    turn8 = solves[30 : 40]
    
    sets = [index7, index8, turn7, turn8]

    #printStatsTable(sets)
    for i in range (10):
        if solves[i].totalStates < solves[i + 10].totalStates:
            print (i)
            print (solves[i].totalStates)
            print (solves[i + 10].totalStates)


def getSolvesWithDistance(distance: int, solves: list[Solve]) -> list[Solve]:
    filteredSolves = []

    for solve in solves:
        if solve.distance == distance:
            filteredSolves.append(solve)

    return filteredSolves

def collectData(path, begin = 0, end = 5000) -> list[Solve]:
    file = open(path, "r")
    lines = file.readlines()
    solves = []

    for i in range(len(lines)):
        if lines[i] == "Starting solve of shuffle\n":
            steps = []
            shuffleString = lines[i + 1]
            offset = 3

            while lines[i + offset].startswith("IDA* Finished bound"):
                line = lines[i + offset].removeprefix ("IDA* Finished bound ")
                values = re.split(". Elapsed time: |s. Traversed states at this bound: |duplicate States found: ", line)
                if (len(values) > 3):   
                    steps.append(Entry(values[0], float(values[1]), int(values[2]), int(values[3])))
                else:
                    steps.append(Entry(values[0], float(values[1]), int(values[2]), 0))
                #print(values[0] + " " + values[1] + " " + values[2])
                offset += 1
            
            totalTime = float(lines[i + offset].removeprefix("Time required: ").replace("s", ""))
            solution = lines[i + offset + 2]
            solve = Solve(shuffleString, solution, totalTime, steps)
            
            solves.append(solve)
    
    return solves[begin : end]

main()