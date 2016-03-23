#ifndef STDIO_H
#include <stdio.h>
#endif

#ifndef RANDOM
#include <random>
#endif

#ifndef MATH_H
#include <math.h>
#endif

#ifndef VECTOR
#include <vector>
#endif

#ifndef STDLIB_H
#include <stdlib.h>
#endif

#ifndef CSTDLIB
#include <cstdlib>
#endif

#include "processes.h"

using namespace std;

void swap(vector<processStruct> &tVec, int i, int j){
    processStruct temp = tVec[i];
    tVec[i] = tVec[j];
    tVec[j] = temp;
}

void checkTVec(vector<processStruct> &tVec){
    int size = tVec.size();
    
    for(int i=0; i<size; i++){
        for(int j=i+1; j<size; j++)
            if(tVec[i].cycleCount > tVec[j].cycleCount)
                swap(tVec, i, j);
    }
}

void fifo(vector<processStruct> pVec, int cores, int entranceT){
	vector<processStruct> readyQueue;
	vector<processStruct> activeQueue;
	int total = 0;
	int finished = 0;
	
	for(int i=0; ;i++){
		if( (i%entranceT) == 0 && (i/entranceT) <= pVec.size() ){
			readyQueue.push_back(pVec[i/entranceT]);
		}
		
		if(activeQueue.size() <= cores && readyQueue.size() > 0){
			processStruct temp = readyQueue[0];
			temp.cycleCount = temp.cycleCount;
			activeQueue.push_back(temp);
			readyQueue.erase(readyQueue.begin());
		}
		
		for(int j=0; j<cores && j<activeQueue.size(); j++){
			activeQueue[j].cycleCount--;
		}
		
		total += readyQueue.size();
		
		for(int j=0; j<cores && j<activeQueue.size(); j++){
			if(activeQueue[j].cycleCount == 0){
				activeQueue.erase(activeQueue.begin()+j);
				j--;
				finished++;
			}
		}
		
		if(finished == 50)
			break;
	}
	double avgT = ((double) total) / 50.0;
	int penalty = 50*10;
	if(cores == 1){
		printf("Average wait time was %lf for %d core\n", avgT, cores);
		printf("Total run time was %d\n", total);
	}
	else{
		printf("Average wait time was %lf for %d cores\n", avgT, cores);
		printf("Total run time was %d\n", total);
	}
}

void SJF(vector<processStruct> pVec, int cores, int entranceT){
	vector<processStruct> readyQueue;
	vector<processStruct> activeList;
	int total = 0;
	int finished = 0;
	
	for(int i=0; ;i++){
		if((i%entranceT) == 0 && (i/entranceT) <= pVec.size()){
			readyQueue.push_back(pVec[i/entranceT]);
			checkTVec(readyQueue);
		}
		
		if(activeList.size() <= cores && readyQueue.size() > 0){
			processStruct temp = readyQueue[0];
			temp.cycleCount = temp.cycleCount;
			activeList.push_back(temp);
			readyQueue.erase(readyQueue.begin());
		}
		
		for(int j=0; j<cores && j<activeList.size(); j++){
			activeList[j].cycleCount--;
		}
		
		total += readyQueue.size();
		
		for(int j=0; j<cores && j<activeList.size(); j++){
			if(activeList[j].cycleCount == 0){
				activeList.erase(activeList.begin()+j);
				j--;
				finished++;
			}
		}
		
		if(finished == 50)
			break;
	}
	double avgT = ((double) total) / 50.0;
	int penalty = 50*10;
	if(cores == 1){
		printf("Average wait time was %lf for %d core\n", avgT, cores);
		//printf("Total penalty time was %d\n", penalty);
	}
	else{
		printf("Average wait time was %lf for %d cores\n", avgT, cores);
		//printf("Total penalty time was %d\n", penalty);
	}
}


/*
void SRT(vector<processStruct> pVec, int cores){
	vector<processStruct> tVec; //queue of processes
	int total = 0;
	int deleted = 0;
	int *activeProcesses = (int*) malloc(cores*sizeof(int));
	for(int i=0; i<cores; i++)
		activeProcesses[i] = -11; //-11 means open cpu

	for(int i=0; ; i++){ //just loop
		//Checks for new entrance
		if( (i%50) == 0 && (i/50) <= pVec.size()){ //if we've gotten a new entrance
			tVec.push_back(pVec[i/50]); //put it on the back
			checkTVec(tVec, cores); //sort innactive processes based on remaining time (SRT)
		}
		
		//update context switch times
		for(int j=0; j<cores; j++){
			if(activeProcesses[j] == -11)
		}
		
		//update total wait time if processes are waiting
		int actProc=0;
		for(int j=0; j<cores; j++)
			if(activeProcesses[j] >= 0)
				actProc++;
		if(temp <= cores && tVec.size() > cores)
			total = total + tVec.size() - actProc;
		
		//decrement amount of cycles for active processes
		for(int j=0; j<cores && j<tVec.size(); j++){ //TODO update to parse through activeProcesses
			tVec[j].cycleCount = tVec[j].cycleCount - 1;
		}
		
		//parse through active processes and decrement
		for(int j=0; j<cores && j<tVec.size(); j++){
			//if process is done then delete
			if(j >= 0 && tVec[j].cycleCount == 0){
				tVec.erase(tVec.begin()+j);
				deleted++;
			}
		}
		//Break when out of processes
		if(deleted == 50)
			break;
	}
	double avgT = ((double) total) / 50.0;
	if(cores == 1)
		printf("Average wait time was %0.4lf for %d core\n", avgT, cores);
	else
		printf("Average wait time was %0.4lf for %d cores\n", avgT, cores);
}*/

void meh(vector<processStruct> pVec, int cores){
	vector<processStruct> readyQueue;
	vector<processStruct> activeQueue;
	int total = 0;
	int finished = 0;
	int penalty = 0;
	
	for(int i=0; ; i++){
		//push new process onto readyqueue and sort
		if( (i%50) == 0 && (i/50) <= pVec.size()){
			readyQueue.push_back(pVec[i/50]);
			checkTVec(readyQueue);
		}
		
		//if activeQueue has capacity then push
		if(activeQueue.size() <= cores){
			activeQueue.push_back(readyQueue.front());
			readyQueue.erase(readyQueue.begin());
		}
		
		//find lowest time in readyQueue and check against activeQueue
		int min = 20000;
		int minPos = -1;
		while(minPos == -1){
			for(int j=0; j<readyQueue.size(); j++)
				if(readyQueue[j].cycleCount < min){
					min = readyQueue[j].cycleCount;
					minPos = j;
				}
			for(int j=0; j<activeQueue.size(); j++) //Extemely unsatisfied
				if(min < activeQueue[j].cycleCount){
					processStruct contextSwitch;
					contextSwitch.cycleCount = -10;
					contextSwitch.pid = -1;
					readyQueue.push_back(activeQueue[j]);
					activeQueue.erase(activeQueue.begin()+j);
					activeQueue.push_back(contextSwitch);
					min = 20000;
					minPos = -1;
					break;
				}
		}
		
		//probably re-sort readyQueue
		checkTVec(readyQueue);
		
		//update total wait time
		total = total + readyQueue.size();
		
		//update activeQueue
		for(int j=0; j<cores; j++){
			if(activeQueue[j].cycleCount > 0)
				activeQueue[j].cycleCount--;
			else if(activeQueue[j].cycleCount < 0)
				activeQueue[j].cycleCount++;
		}
		
		//check if any processes or context switches are finished
		for(int j=0; j<cores; j++){
			if(activeQueue[j].cycleCount == 0){ //if finished
				activeQueue.erase(activeQueue.begin()+j);
				if(activeQueue[j].pid != -1) //if context switch
					finished++;
				else
					penalty++;
			}
		}
		
		if(finished == 50)
			break;
	}
	double avgT = ((double) total) / 50.0;
	if(cores == 1)
		printf("Average wait time was %0.4lf for %d core\n", avgT, cores);
	else
		printf("Average wait time was %0.4lf for %d cores\n", avgT, cores);
}

void writePvec(vector<processStruct> pVec, string x){
	FILE *fp = fopen( (char *) x.c_str(), "w");
	checkTVec(pVec);
	for(int i=0; i<50; i++){
		fprintf(fp, "%d,",i);
		fprintf(fp, ",%d\n",pVec[i].cycleCount);
	}		
}

int main(){
	const string seeds[] = {"Foo", "Bar", "Simpsons", "Some seeds don't work"};
	
	for(string x: seeds){
		vector<processStruct> pVec = genProcs(50, x); //get vector of processes
		
		writePvec(pVec, x);
		
		char *tSeed = (char *) x.c_str();
		printf("SRT for seed %s\n", tSeed);
		
		printf("fifo\n");
		for(int i=1; i<=4; i = i*4)
			fifo(pVec, i, 50);
		
		printf("SJF\n");
		for(int i=1; i<=4; i = i*4)
			SJF(pVec, i, 50);
		
		printf("\n");
	}
	return 0;
}