#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstring>

extern "C" {
	#include "Graph.h"
	#include "HamiltonianCycle.h"
	#include "VertexOrder.h"
}

using namespace std;

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

int pathReward(const vector<int>& path, const vector<Node>& nodes, int offset)
{	
	int pts = 0;
	int max = 0;
	int index = 0;
	for (unsigned int i = 0; i < path.size(); ++i) {
		index = (i+offset)%path.size();
		if (path[index] >= max) {
			++pts;
			max = path[index];
		}
	}
	
	return pts;
}

vector<Node> loadData(int argc, char** argv)
{	
	char* filename = argv[1];
	ifstream file(filename);
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

Graph* constructGraph(const vector<Node>& graph)
{
	char name[] = "Graph";
	Status s;
	Graph* g = initGraph(allocateGraph(name, graph.size(), &s));
	
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
	
	return g;
}

void findPathsFromCycles(HCStateRef hc, const vector<Node>& graph)
{
	int pts = graph.size();
	int max = 0;
	Vertex* cycle = new Vertex[pts * 2 + 1];
	do {
		memset(cycle, 0, sizeof(Vertex) * (pts * 2) + 1);
		getCurrentHamiltonianCycle(hc,cycle);
		
		vector<int> cycleVec(graph.size());
		
		getCycle(cycle, pts, cycleVec);
		
		vector<int> path(graph.size());
		
		int offset = 0;
		while (cycleVec[offset++] != graph.size()) {}
		
		for (unsigned int i = 0; i < path.size(); ++i) {
			path[i] = cycleVec[(i + offset) % cycleVec.size()] - 1;
		}
		
		for (unsigned int i = 0; i < path.size(); ++i) {
			int r = pathReward(path, graph, i);
			
			if (r > max) {
				max = r;
				cout << r << endl;
			}
		}
		
		reverse(path.begin(), path.end());
		
		for (unsigned int i = 0; i < path.size(); ++i) {
			int r = pathReward(path, graph, i);
			
			if (r > max) {
				max = r;
				cout << r << endl;
			}
		}
		
		
	} while (nextHamiltonianCycleWithPruning(hc));
	
	delete cycle;
}


int main(int argc, char** argv)
{
	vector<Node> graph = loadData(argc, argv);
	
	Status stat = STATUS_OK;
    StatusRef s = &stat;  
	
	Graph* g = constructGraph(graph);
        
	int pts = g->vertex_count;
	VArray* vo = sortVerticesDegreeDesc(initVArray(allocateVArray(pts, s)), g->degree);
	HCStateRef hc = initHCState(allocateHCState(pts, s), g->degree, g->adj_lists, vo);
	
	bool hasCycles = firstHamiltonianCycleWithPruning(hc);
	
	if (hasCycles) {
		findPathsFromCycles(hc, graph);
	}
	
	cout << "Done" << endl;
}
