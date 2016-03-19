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

int main(){
	vector<processStruct> pVec = genProcs(50); //get vector of processes
	vector<processStruct> tVec; //queue of processes
        int total = 0;
        
        
        printf("We have the process list size %d\n", pVec.size());
        
        
        for(int i=0; ; i++){ //just loop
            //Checks for new entrance
            if( (i%50) == 0 && (i/50) <= pVec.size()){ //if we've gotten a new entrance
                printf("Adding new process\n");
                tVec.push_back(pVec[i/50]); //put it on the back
                checkTVec(tVec); //sort based on remaining time (SRT)
            } //only check when a new process enters scope
            
            //update total wait time if processes are waiting
            if(tVec.size() > 1){
                //printf("Updating total\n");
                total++;
            }
            
            //decrement amount of cycles for active process(es)
            tVec[0].cycleCount = tVec[0].cycleCount - 1;
            
            //if process is done then delete
            if(tVec[0].cycleCount == 0){
                printf("We are killing process %d\n", tVec[0].pid);
                tVec.erase(tVec.begin());
            }
            //Break when out of processes
            if((i/50) > pVec.size() && tVec.size() < 1){
                printf("Did we ever even try to break?\n");
                break;
            }
        }
        
        double avgT = ((double) total) / 50.0;
        
        printf("Average wait time was %0.4lf", avgT);
        
	return 0;
}