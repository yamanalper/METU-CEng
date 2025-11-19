CXXFLAGS=-g

all: testrun

testrun: testrun.o hw2.o
	g++ -o testrun testrun.o hw2.o -lpthread

testrun.o: hw2.h testrun.cpp

clean:
	rm testrun.o hw2.o testrun
