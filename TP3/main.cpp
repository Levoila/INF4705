#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <thread>
#include <mutex>
#include <queue>

extern "C" {
	#include "Graph.h"
	#include "HamiltonianCycle.h"
	#include "VertexOrder.h"
}

using namespace std;

vector<int> translation;
bool displayPath = false;
string filename = "";
int globalMax = 0;
mutex maxMutex;

struct Node
{
	vector<int> c;
};


void getCycle(Vertex *c, UInt pts, vector<int>& cycle)
{
	int i = 0;
    Vertex u=1, pu=0, v;
    do {
         v = c[u];
         if ( v != pu) {
             pu = u;
             u  = v;
         } else { 
             pu = u;
             u  = c[pts + u];
         }
         cycle[i++] = u;
    } while (u != 1);
}

int pathReward(const vector<int>& path, int offset)
{	
	int pts = 0;
	int max = 0;
	int index = 0;
	for (unsigned int i = 0; i < path.size(); ++i) {
		index = (i+offset)%path.size();
		if (path[index] > max) {
			++pts;
			max = path[index];
		}
	}
	
	return pts;
}

vector<Node> loadData()
{	
	ifstream file(filename.c_str());
	if (!file) {
		cout << "File not existing : " << filename << endl;
		return vector<Node>();
	}
	
	int n = 0;
	int c = 0;
	
	file >> n >> c;
	
	vector<pair<int, int>> ecoliers(n);
	for (int i = 0; i < n; ++i) {
		file >> ecoliers[i].first;
		ecoliers[i].second = i;
	}
	
	sort(ecoliers.begin(), ecoliers.end());
	
	vector<int> t(n);
	for (int i = 0; i < n; ++i) {
		translation.push_back(ecoliers[i].second);
		t[ecoliers[i].second] = i;
	}
	
	vector<pair<int, int>> validPairs(c);
	int e1 = 0, e2 = 0;
	for (int i = 0; i < c; ++i) {
		file >> e1 >> e2;
		validPairs[i] = pair<int, int>(t[e1-1], t[e2-1]);
	}
	
	vector<Node> graph(n);
	for (auto p : validPairs) {
		graph[p.first].c.push_back(p.second);
		graph[p.second].c.push_back(p.first);
	}
	
	return graph;
}

Graph* constructGraph(const vector<Node>& graph, bool extraNode = false)
{
	char name[] = "Graph";
	Status s = STATUS_OK;
	Graph* g = initGraph(allocateGraph(name, graph.size() + (extraNode ? 1 : 0), &s));
	
	for (unsigned int i = 0; i < graph.size(); ++i) {
		if (graph[i].c.size() == 0) {
			continue;
		}
		VArray edges;
		edges.length = graph[i].c.size();
		edges.array = new Vertex[edges.length];
		
		for (unsigned int j = 0; j < edges.length; ++j) {
			edges.array[j] = graph[i].c[j]+1;
		}
		
		createEdges(g, &edges, i+1);
		
		delete edges.array;
	}
	
	if (extraNode) {
		VArray edges;
		edges.length = graph.size();
		edges.array = new Vertex[edges.length];
		
		for (unsigned int i = 0; i < edges.length; ++i) {
			edges.array[i] = i+1;
		}
		
		createEdges(g, &edges, graph.size()+1);
		
		delete edges.array;
	}
	
	return g;
}

void printPath(const vector<int>& path, int offset = 0)
{
	for (unsigned int i = 0; i < path.size(); ++i) {
		cout << translation[path[(i+offset) % path.size()]-1]+1 << '\n';
	}
	cout << "fin" << endl;
}

void findPathsFromCycle(const vector<int>& cycle)
{
	int r = 0;
	for (unsigned int i = 0; i < cycle.size(); ++i) {
		r = pathReward(cycle, i);
		
		if (r > globalMax) {
			lock_guard<mutex> lock(maxMutex);
			if (r > globalMax) {
				globalMax = r;
				
				if (displayPath) {
					printPath(cycle, i);
				} else {
					cout << cycle.size() - r << endl;
				}
			}
		}
	}
}

void checkPath(const vector<int>& path)
{
	int r = pathReward(path, 0);
		
	if (r > globalMax) {
		lock_guard<mutex> lock(maxMutex);
		if (r > globalMax) {
			globalMax = r;
			
			if (displayPath) {
				printPath(path, 0);
			} else {
				cout << path.size() - r << endl;
			}
		}
	}
}

void findPathsFromCycles(HCStateRef hc, const int size)
{
	vector<int> cycleVec(size);
	Vertex* cycle = new Vertex[size * 2 + 1];
	do {
		memset(cycle, 0, sizeof(Vertex) * (size * 2) + 1);
		getCurrentHamiltonianCycle(hc, cycle);
		
		getCycle(cycle, size, cycleVec);
		
		findPathsFromCycle(cycleVec);
		
		reverse(cycleVec.begin(), cycleVec.end());
		
		findPathsFromCycle(cycleVec);
		
	} while (nextHamiltonianCycleWithPruning(hc));
	
	delete cycle;
}

void findPaths(HCStateRef hc, const int size)
{
	vector<int> cycleVec(size);
	vector<int> path(size-1);
	Vertex* cycle = new Vertex[size * 2 + 1];
	
	do {
		memset(cycle, 0, sizeof(Vertex) * (size * 2) + 1);
		getCurrentHamiltonianCycle(hc, cycle);
		getCycle(cycle, size, cycleVec);
		
		int offset = 0;
		for (unsigned int i = 0; i < size; ++i) {
			if (cycleVec[i] == size) {
				offset = i;
				break;
			}
		}
		
		for (unsigned int i = 0; i < size-1; ++i) {
			path[i] = cycleVec[(i + offset + 1) % (size-1)];
		}
		
		checkPath(path);
		reverse(path.begin(), path.end());
		checkPath(path);
		
	} while (nextHamiltonianCycleWithPruning(hc));
}

void parseArgs(int argc, char** argv)
{
	displayPath = false;
	for (int i = 1; i < argc; ++i) {
		string arg = argv[i];
		
		if (arg == "-p") {
			displayPath = true;
		} else if (arg == "-f" && argc > (i+1)) {
			filename = argv[i+1];
		}
	}
}

int main(int argc, char** argv)
{
	parseArgs(argc, argv);
	
	vector<thread> workers;
	vector<Node> graph = loadData();
	
	Status stat = STATUS_OK;
    StatusRef s = &stat;  
	
	Graph* g1 = constructGraph(graph);
	Graph* g2 = constructGraph(graph);
	Graph* g3 = constructGraph(graph, true);
	Graph* g4 = constructGraph(graph, true);
        
	int pts = g1->vertex_count;
	
	VArray* vo1 = sortVerticesDegreeAsc(initVArray(allocateVArray(pts, s)), g1->degree);
	HCStateRef hc1 = initHCState(allocateHCState(pts, s), g1->degree, g1->adj_lists, vo1);
	
	VArray* vo2 = sortVerticesDegreeDesc(initVArray(allocateVArray(pts, s)), g2->degree);
	HCStateRef hc2 = initHCState(allocateHCState(pts, s), g2->degree, g2->adj_lists, vo2);
	
	VArray* vo3 = sortVerticesDegreeAsc(initVArray(allocateVArray(pts+1, s)), g3->degree);
	HCStateRef hc3 = initHCState(allocateHCState(pts+1, s), g3->degree, g3->adj_lists, vo3);
	
	VArray* vo4 = sortVerticesDegreeDesc(initVArray(allocateVArray(pts+1, s)), g4->degree);
	HCStateRef hc4 = initHCState(allocateHCState(pts+1, s), g4->degree, g4->adj_lists, vo4);
	
	if (firstHamiltonianCycleWithPruning(hc1)) {
		workers.push_back(thread(findPathsFromCycles, hc1, graph.size()));
	}
	
	if (firstHamiltonianCycleWithPruning(hc2)) {
		workers.push_back(thread(findPathsFromCycles, hc2, graph.size()));
	}
	
	if (firstHamiltonianCycleWithPruning(hc3)) {
		workers.push_back(thread(findPaths, hc3, graph.size()+1));
	}
	
	if (firstHamiltonianCycleWithPruning(hc4)) {
		workers.push_back(thread(findPaths, hc4, graph.size()+1));
	}
	
	for (thread& t : workers) {
		t.join();
	}
	
	return 0;
}
