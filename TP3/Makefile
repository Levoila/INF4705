
CC     =  gcc
CFLAGS =   -Wall -pipe -O3

all: lib
	g++ main.cpp -std=c++11 -L lib/ -l graph -lpthread -o rang

Graph.o: Graph.c Arch.h Graph.h

VertexOrder.o: Graph.o VertexOrder.h VertexOrder.c

DFSAlgorithms.o: Graph.o DFSAlgorithms.c DFSAlgorithms.h

HamiltonianCycle.o: Graph.o DFSAlgorithms.o HamiltonianCycle.c \
                    HamiltonianCycle.h 

graph_algs = Graph.o DFSAlgorithms.o VertexOrder.o HamiltonianCycle.o
		
lib: $(graph_algs)
	ar -cvq lib/libgraph.a *.o

clean:
	rm -f *.o rang
