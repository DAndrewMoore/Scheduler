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
            if(tVec[i].cycleCount < tVec[j].cycleCount)
                swap(tVec, i, j);
    }
}

void SRT(vector<processStruct> pVec, int cores){
		vector<processStruct> tVec; //queue of processes
        int total = 0;
        int deleted = 0;
	
        for(int i=0; ; i++){ //just loop
            
			//Checks for new entrance
            if( (i%50) == 0 && (i/50) <= pVec.size()){ //if we've gotten a new entrance
                tVec.push_back(pVec[i/50]); //put it on the back
                checkTVec(tVec); //sort based on remaining time (SRT)
            } //only check when a new process enters scope
            
            //update total wait time if processes are waiting
            if(tVec.size() > cores)
                total++;
            
            //decrement amount of cycles for active processes
			for(int j=0; j<cores && j<tVec.size(); j++){
				tVec[j].cycleCount = tVec[j].cycleCount - 1;
			}
            
			//parse through active processes and delete if finished
			for(int j=0; j<cores && j<tVec.size(); j++){
				//if process is done then delete
				if(tVec[j].cycleCount == 0){
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
}

int main(){
		const string seeds[] = {"Foo", "Bar", "Simpsons", "Some seeds don't work"};
		
		for(string x: seeds){
			vector<processStruct> pVec = genProcs(50, x); //get vector of processes
			
			char *tSeed = (char *) x.c_str();
			printf("SRT for seed %s\n", tSeed);
			
			for(int i=1; i<=8; i = i*2)
				SRT(pVec, i);
			printf("\n");
		}
	return 0;
}