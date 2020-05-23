#pragma once

//#include "Parameters.h"

using namespace std;

/////***** ADAPTED FROM A WEBSITE, STL IS USED FOR DEFINING NODES AND EDGES *******//////

#define graph_edge pair<int,int>			// Hold indices of nodes 	

class Graph {								
private:
	int V;									// number of nodes in graph
	vector<pair<float, graph_edge>> G;		// vector for graph
	vector<pair<float, graph_edge>> T;		// vector for mst
	int* parent;

public:
	Graph(int V);
	void AddEdge(int u, int v, float wt);
	int find_set(int i);
	void union_set(int u, int v);
	void kruskal_algorithm();
	int returnVars();
	void display_mst();
};

Graph::Graph(int V) {
	parent = new int[V];

	for (int i = 0; i < V; i++)
		parent[i] = i;

	G.clear();
	T.clear();
}
void Graph::AddEdge(int u, int v, float wt) {
	G.push_back(make_pair(wt, graph_edge(u, v)));
}
int Graph::find_set(int i) {
	// If i is the parent of itself
	if (i == parent[i])
		return i;
	else
		//else recursively find the parent of i
		return find_set(parent[i]);
}
void Graph::union_set(int u, int v) {
	parent[u] = parent[v];
}
void Graph::kruskal_algorithm() {
	int i, uSt, vEd;
	sort(G.begin(), G.end()); // sort the edges ordered on increasing weight
	for (i = 0; i < G.size(); i++) {
		uSt = find_set(G[i].second.first);
		vEd = find_set(G[i].second.second);
		if (uSt != vEd) {
			T.push_back(G[i]); // add to mst vector
			union_set(uSt, vEd);
		}
	}
}

 int Graph::returnVars() {
	 int DC=1;
	 for (int i = 0; i < T.size(); i++) 
	 {
		 if (T[i].first > fp)
		 {
			 DC++;
		 }
	 }
	 return DC;
}

void Graph::display_mst() {
	cout << "Edge :" << " Weight" << endl;
	for (int i = 0; i < T.size(); i++) {
		cout << T[i].second.first << " - " << T[i].second.second << " : "
			<< T[i].first;
		cout << endl;
	}
}

/*
Minimum Spanning Algorithm is utilized for evaluating the Cost objective which is calculated with
number of RS and number of DC.
*/


