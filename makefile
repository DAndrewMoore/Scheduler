CXX = g++
CXXFLAGS = -std=c++0x

main: project.o processes.o
	$(CXX) $(CXXFLAGS) -o main project.o processes.o

project.o: project.cpp processes.h
	$(CXX) $(CXXFLAGS) -c project.cpp

processes.o: processes.h

clean:
	rm processes.o project.o