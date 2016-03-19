#include <stdio.h>
#include <random>
#include <math.h>
#include <vector>

typedef struct{
int pid;
int cycleCount;
int memoryPrint;
int entranceTime;
} processStruct;

std::vector<processStruct> genProcs(int numP);