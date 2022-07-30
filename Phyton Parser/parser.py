from logging import addLevelName
from pickletools import uint8
from tokenize import Number, String
#import matplotlib.pyplot as plt

import re
import sys


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
        self.steps = steps
        self.distance = solution.count(" ") + 1
        
        self.totalStates = 0
        self.totalDuplicates = 0
        
        for step in steps:
            self.totalStates += step.states
            self.totalDuplicates += step.duplicates

    def toSting(self):
        string = "shuffle: " + self.shuffle + "solution: " + self.solution + "\ntotal solve time: " + str(self.totalTime) + "total states: " + str(self.totalStates) + "\ntotal duplicates: " + str(self.totalDuplicates) + "\n"

        for step in self.steps:
            string += "depth: " + str(step.depth) + ", time: " + str(step.time) + ", states: " + str(step.states) + ", duplicates: " +str(step.duplicates) + "\n"

        return string

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
        percentages.append(float(count) / float(len(solves)) * 100)
        labels.append(str(round(float(count) / float(len(solves)) * 100, 2)) + "%")
    
    plt.bar(x, percentages)

    plt.title("Distance Distribution Relative (5000 solves)")
    #plt.margins(y=0.3)
    plt.xlabel("distance")
    plt.ylabel("probability (%)")
    
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

def main():
    solves = collectData()

    #for solve in solves:
    #    if solve.distance == 14:
    #        print(solve.shuffle + "\n" + solve.solution)
    with open('dsResults.txt', 'a') as the_file:
        for solve in solves:
            the_file.write(solve.shuffle + solve.solution)
        


def getSolvesWithDistance(distance: int, solves: list[Solve]) -> list[Solve]:
    filteredSolves = []

    for solve in solves:
        if solve.distance == distance:
            filteredSolves.append(solve)

    return filteredSolves

def collectData() -> list[Solve]:
    file = open("../newFiveLog.txt", "r")
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
    
    return solves

main()