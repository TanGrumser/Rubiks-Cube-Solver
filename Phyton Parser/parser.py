from pickletools import uint8
from tokenize import Number, String

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


def main():
    index = 0

    if (len(sys.argv) > 1):
        index = int(sys.argv[1])

    solves = collectData()
    
    solveLenghts = []
    lengthsCount = {}
    
    for solve in solves:
        lengthsCount[solve.distance] = lengthsCount.setdefault(solve.distance, 0) + 1
        solveLenghts.append(solve.distance)
    
    shortestSolves = getSolvesWithDistance(14, solves)
    states = 0
    time = 0.0

    for solve in shortestSolves:
        time += solve.totalTime
        states += solve.totalStates

    print(float(states) / float(time))
        


def getSolvesWithDistance(distance: int, solves: list[Solve]) -> list[Solve]:
    filteredSolves = []

    for solve in solves:
        if solve.distance == distance:
            filteredSolves.append(solve)

    return filteredSolves

def collectData() -> list[Solve]:
    file = open("fiveShufflesLog.txt", "r")
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