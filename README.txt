compile with:
	g++ -std=c++0x project.cpp processes.cpp

run with:
	./a.out unless otherwise specified

main in project.cpp takes no arguements, has predefined seeds for distributions, calculates average wait times for each seed for 1,2,4,8 cores.

processes contains the generator function genProcs(int num2generate, string seed). The function will return a vector<processStruct>.

processStruct is a structure defined in processes.h that has structure

typedef struct{
int pid;
int cycleCount;
int memoryPrint;
int entranceTime;
} processStruct;

Usage for a processStruct atom is tempProcessStruct.pid or tempProcessStruct.cycleCount or etc.