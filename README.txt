compile with:
	g++ -std=c++0x project.cpp processes.cpp -o fifosj
	g++ -std=c++0x rrq.cpp -o rrq
	g++ -std=c++-x 4core_rrq.cpp -o 4rrp

run with:
	./fifosjf
	./rrq
	./4rrp
------------------------------------------------------------------------------------------------------------
seeds: "Foo", "Bar", "Simpsons", "Some seeds don't work"
------------------------------------------------------------------------------------------------------------
main in project.cpp takes no arguements, has predefined seeds for distributions, calculates average wait times for each seed for 1 and 4 cores for sjf and fifo for each seed.

processes contains the generator function genProcs(int num2generate, string seed). The function will return a vector<processStruct>.

processStruct is a structure defined in processes.h that has structure

typedef struct{
int pid;
int cycleCount;
int memoryPrint;
int entranceTime;
} processStruct;

Usage for a processStruct atom is tempProcessStruct.pid or tempProcessStruct.cycleCount or etc.
------------------------------------------------------------------------------------------------------------
rrq is fully contained to run and calculate round robin for single core on all seeds (respectively).

just run and see results
-------------------------------------------------------------------------------------------------------------
4rrq is fully contained to run and calculate round robin for quad core on all seeds (respectively).

run and see results. Has all information needed except seed.
-------------------------------------------------------------------------------------------------------------