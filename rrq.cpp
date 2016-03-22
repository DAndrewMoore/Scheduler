//CSCE 4600
//Project #1

//Emory King


//#include <stdlib>
//#include <unistd>
#include <iostream>
#include <cstdlib>
#include <queue>
#include <time.h>

using namespace std;



class tuple {
public:
        int pid, arrival_t, start_flag, started, waited, stalled, cycles, memory;
	tuple(int, int,int,int);
};

tuple::tuple (int count, int at, int cyc, int mem) {
    pid = count;
    arrival_t = at;
	cycles = cyc; //cycles remaining
    memory = mem;
	waited = 0;	
	stalled = 0;
	start_flag = 0;
}



int main (){

int k = 50; 		//number of processes
int q = 50; 		//RoundRobin quantum
int cts = 10; 		//context switch
int i = 0;		//tuples index
int j = 0;		//arrival t
int c = 0;		//completed processes index
int t = 0;		//t elapsed in scheduler
int finished = 0;	//# processes finished
tuple *tuples[k];	//initial processes
tuple *completed[k];	//completed processed
tuple * current;	//running process
queue<tuple*> fifo;	//FIFO queue

///////////////////////////////////////////////////////////////////////////////////////
srand(time(NULL));
cout << "spawning " << k << " processes" << endl;
//create processes with arrival ts at intervals of 50
for (j=0; j<2500; j+=50){
	tuples[i] = new tuple(i, j, rand() % 1000 + 1, 20);	// j = arrival t
	i++;
}
//print process info
i =0;
cout << "pid cycles arival  " << endl;
for(i;i<k;i++){
	cout << tuples[i]->pid <<"   " << tuples[i]->cycles <<"   "<< tuples[i]->arrival_t << endl;
}
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
//				RR SCHEDULER SIMULATION 									///////////
///////////////////////////////////////////////////////////////////////////////////////
i = 1;
fifo.push(tuples[0]);

//until all processes are finished
while (finished < 50){
		// fast forward if empty queue awaiting arrival
		if (fifo.empty()){
			t = tuples[i]->arrival_t;
			fifo.push(tuples[i]);
			i++;
		}
		current = fifo.front();
		if (current->cycles <= q){	
			////////////////////////////////////////////
        	//process will finish
        	////////////////////////////////////////////
			
			//add to waiting t for stalled process
			if (current->stalled){
				current->waited += (t - current->stalled);
			}	
			//set the start t on first iteration
			if (current->start_flag == 0){
				current->started = t;
				current->start_flag = 1;
			}
		
			t += current->cycles;				
			current->cycles = 0;	//no more cycles; process finished
			finished++;
			cout << "process " << current->pid  << " finished @ "<< t << " and waited "  << current->waited << " from stalls" <<  endl;
			cout << "-------------------------------------------------------------" << endl;
			completed[c++] = current;
			fifo.pop();

			//add arrived processes to queue
			while (i<50 && tuples[i]->arrival_t <= t){
				fifo.push(tuples[i]);
				i++;
			}		
		}
		else{
		    ////////////////////////////////////////////
        	//process will pre-empt
        	////////////////////////////////////////////					

        	//add to waiting t for stalled processes
			if (current->stalled){
				current->waited += (t - current->stalled);
			}

			//set the start t on first iteration
	      	if (current->start_flag == 0){
                current->started = t;
                current->start_flag = 1;
            }

			current->cycles -= q;
			t += q;
			current->stalled = t;
			cout << "process " << current->pid  << " pre-empted @ "<< t << " and has "  << current->cycles << " cycles left" <<  endl;
			
			//add arrived processes to queue first, then add pre-empted process after
			while (i < 50 && tuples[i]->arrival_t <= t){
				fifo.push(tuples[i]);
				i++;
			}	
			fifo.push(current);	//back of the line sucka
			fifo.pop();
			t += cts;		//context switch
		}
}//END


/////////////////////////////////////////////////////
//   			DISPLAY DATA
i =0;
int total_wait = 0;
cout << "pid waited started arrival startdelay" << endl;
for(i;i<50;i++){
	//adjust waiting time by adding initial wait
	completed[i]->waited += (completed[i]->started - completed[i]->arrival_t);
    cout << completed[i]->pid <<"   " << completed[i]->waited << "      " << completed[i]->started << "     " << completed[i]->arrival_t  << "       "<<(completed[i]->started - completed[i]->arrival_t) << endl;
	total_wait+=completed[i]->waited;
}

cout << endl << "Total wait time: " << total_wait << endl << "Average wait time: " << total_wait/50 << endl << endl;
//
/////////////////////////////////////////////////////
return 0;
}
