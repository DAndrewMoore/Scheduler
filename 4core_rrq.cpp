#include <iostream>
#include <cstdlib>
#include <queue>
#include <time.h>

using namespace std;



class tuple {
public:
        int pid, arrival_t, start_flag, started, waited, stalled, cycles, memory, tlip, fin, switching, switch_time;
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
	fin = 0;
	tlip = 50;
	switching = 0;

}


int main (){

int k = 50; 		//number of processes
int q = 50; 		//RoundRobin quantum
int cts = 10; 		//context switch
int i = 0;			//tuples index
int j = 0;			//arrival t
int c = 0;			//completed processes index
int current_time = 0;//time elapsed in scheduler
int finished = 0;	//# processes finished
int lowest;			//processing time interval
int l = 0;
int a = 0;			//context switch count
int p = 0; 
int next = 0;		//index for arriving processes

tuple *tuples[k];	//initial processes
tuple *completed[k];//completed processed
tuple *proc[4];		//4 cores
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

//initialize cores
for (i=0; i<4; i++){
	proc[i] = NULL;
}

//ROUND ROBIN SCHEDULER
//until not finished
for (current_time=0; finished<50; current_time+=lowest){ 		// process in lowest current_time increments
	lowest = 50;
	// add arrived processes
	if (next < 50 && tuples[next]->arrival_t == current_time){
			fifo.push(tuples[next]);
			next++;
	}
	////////////////////////////////////////////////////////////////////////////////////
	//fill idle processors 
	cout << endl << "---------------------------------------" << current_time << endl;
	for (i=0; i<4; i++){
		if (fifo.empty() != 1 && (fifo.front()->arrival_t <= current_time) && proc[i] == 0){
			
				proc[i] = fifo.front();
				fifo.pop();
				cout << i << " process " << proc[i]->pid << " entering core " << i  << " at time " << current_time << endl;

				//set the start time on first iteration
	      			if (proc[i]->start_flag == 0){
            			    proc[i]->started = current_time;
            			    proc[i]->start_flag = 1;
            			}
				if (proc[i]->stalled){
					proc[i]->waited += current_time - proc[i]->stalled;
					cout << "wait time updated" << endl;
				}
				//set current_time left in processor for added process
				if (proc[i]->cycles<50){
					proc[i]->tlip = proc[i]->cycles;
				}
				else{
					proc[i]->tlip = 50;
				}
		}
	}//end filling

		////////////////////////////////////////////////////////////////////////////////////
		//set processing interval "lowest"
		for (i=0; i<4; i++){
			if (proc[i] != 0 && proc[i]->tlip < lowest){
				lowest = proc[i]->tlip;
			}
		}
		// if interval overruns next arrival time, lower interval 
		if (next < 50 && current_time+lowest > tuples[next]->arrival_t){
			lowest = tuples[next]->arrival_t - current_time;
		}
		//end setting lowest
		////////////////////////////////////////////////////////////////////////////////////
		cout << "/////////////////////////////////////" << endl << endl;


	////////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////////////
	//start processing
	for (i=0; i<4; i++){
		if (proc[i]!=NULL && proc[i]->tlip == lowest){
			//start context switch
			if (proc[i]->cycles > 50 && proc[i]->switching == 0){
				//update
				proc[i]->cycles -= 50;
				//pre-empt
				proc[i]->stalled = current_time+lowest;
				cout << i << " process " << proc[i]->pid << " will start pre-emption at time " << current_time+lowest << endl; 
				proc[i]->switching = 1;
				proc[i]->switch_time = current_time + lowest + cts;

				proc[i]->tlip = cts;
				
				a++; // count cts

			}
			else if (proc[i]->cycles > 50 && proc[i]->switching == 1){
			//finally, push back after context switch	
					cout << i << " process " << proc[i]->pid << " will end context switch at time " << current_time+lowest << endl; 
					proc[i]->switching = 0;
					fifo.push(proc[i]); //end of the line
					proc[i] = NULL;
			}
			else{//tlip <= 50, finish
				//update 
				proc[i]->cycles = 0;
				//finish
				completed[c++] = proc[i];
				proc[i]->tlip = 0;
				proc[i] = NULL;
				finished++;	
				cout << i << " process " << completed[c-1]->pid << " finished " << finished << "th at time " << current_time+lowest << endl;
			}
		}
		else if (proc[i]!=NULL && proc[i]->tlip != lowest){
			cout << i << " keep processing" << endl;
			proc[i]->tlip -= lowest;
		}
		else cout << i << " proc[i] empty" << endl;
	}//end processing 
	////////////////////////////////////////////////////////////////////////////////////
}//finished

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

cout << endl << "Total wait time: " << total_wait << endl << "Average wait time: " << total_wait/50 << endl << "# cts: " << a << endl;
//
/////////////////////////////////////////////////////

return 0;
}
