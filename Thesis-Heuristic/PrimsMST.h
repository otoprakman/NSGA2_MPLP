#pragma once
// C / C++ program for Prim's MST for adjacency list representation of graph 

//Adapted from https://www.geeksforgeeks.org/prims-mst-for-adjacency-list-representation-greedy-algo-6/

#include <limits.h> 
#include <stdio.h> 
#include <stdlib.h> 


// A structure to represent a node in adjacency list 
struct AdjListNode {
	int dest;
	float weight;
	struct AdjListNode* next;
};

// A structure to represent an adjacency list 
struct AdjList {
	struct AdjListNode* head; // pointer to head node of list 
};

// A structure to represent a graph. A graph is an array of adjacency lists. 
// Size of array will be V (number of vertices in graph) 
struct PGraph {
	int V;
	struct AdjList* array;
};

// A utility function to create a new adjacency list node 
struct AdjListNode* newAdjListNode(int dest, float weight)
{
	struct AdjListNode* newNode = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
	newNode->dest = dest;
	newNode->weight = weight;
	newNode->next = NULL;
	return newNode;
}

// A utility function that creates a graph of V vertices 
struct PGraph* createGraph(int facilitynum)
{
	struct PGraph* graph = (struct PGraph*)malloc(sizeof(struct PGraph));
	graph->V = facilitynum;

	// Create an array of adjacency lists. Size of array will be V 
	graph->array = (struct AdjList*)malloc(facilitynum * sizeof(struct AdjList));

	// Initialize each adjacency list as empty by making head as NULL 
	for (int i = 0; i < facilitynum; ++i)
		graph->array[i].head = NULL;

	return graph;
}

// Adds an edge to an undirected graph 
void addEdge(struct PGraph* graph, int src, int dest, float weight)
{
	// Add an edge from src to dest. A new node is added to the adjacency 
	// list of src. The node is added at the begining 
	struct AdjListNode* newNode = newAdjListNode(dest, weight);
	newNode->next = graph->array[src].head;
	graph->array[src].head = newNode;

	// Since graph is undirected, add an edge from dest to src also 
	newNode = newAdjListNode(src, weight);
	newNode->next = graph->array[dest].head;
	graph->array[dest].head = newNode;
}

// Structure to represent a min heap node 
struct MinHeapNode {
	int v;
	float key;
};

// Structure to represent a min heap 
struct MinHeap {
	int size; // Number of heap nodes present currently 
	int capacity; // Capacity of min heap 
	int* pos; // This is needed for decreaseKey() 
	struct MinHeapNode** array;
};

// A utility function to create a new Min Heap Node 
struct MinHeapNode* newMinHeapNode(int v, int key)
{
	struct MinHeapNode* minHeapNode = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
	minHeapNode->v = v;
	minHeapNode->key = key;
	return minHeapNode;
}

// A utilit function to create a Min Heap 
struct MinHeap* createMinHeap(int capacity)
{
	struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
	minHeap->pos = (int*)malloc(capacity * sizeof(int));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array = (struct MinHeapNode**)malloc(capacity * sizeof(struct MinHeapNode*));
	return minHeap;
}

// A utility function to swap two nodes of min heap. Needed for min heapify 
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
	struct MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}

// A standard function to heapify at given idx 
// This function also updates position of nodes when they are swapped. 
// Position is needed for decreaseKey() 
void minHeapify(struct MinHeap* minHeap, int idx)
{
	int small, left, right;
	small = idx;
	left = 2 * idx + 1;
	right = 2 * idx + 2;

	if (left < minHeap->size && minHeap->array[left]->key < minHeap->array[small]->key)
		small = left;

	if (right < minHeap->size && minHeap->array[right]->key < minHeap->array[small]->key)
		small = right;

	if (small != idx) {
		// The nodes to be swapped in min heap 
		MinHeapNode* smallestNode = minHeap->array[small];
		MinHeapNode* idxNode = minHeap->array[idx];

		// Swap positions 
		minHeap->pos[smallestNode->v] = idx;
		minHeap->pos[idxNode->v] = small;

		// Swap nodes 
		swapMinHeapNode(&minHeap->array[small], &minHeap->array[idx]);

		minHeapify(minHeap, small);
	}
}

// A utility function to check if the given minHeap is ampty or not 
int isEmpty(struct MinHeap* minHeap)
{
	return minHeap->size == 0;
}

// Standard function to extract minimum node from heap 
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
	if (isEmpty(minHeap))
		return NULL;

	// Store the root node 
	struct MinHeapNode* root = minHeap->array[0];

	// Replace root node with last node 
	struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
	minHeap->array[0] = lastNode;

	// Update position of last node 
	minHeap->pos[root->v] = minHeap->size - 1;
	minHeap->pos[lastNode->v] = 0;

	// Reduce heap size and heapify root 
	--minHeap->size;
	minHeapify(minHeap, 0);

	return root;
}

// Function to decrease key value of a given vertex v. This function 
// uses pos[] of min heap to get the current index of node in min heap 
void decreaseKey(struct MinHeap* minHeap, int v, float key)
{
	// Get the index of v in heap array 
	int i = minHeap->pos[v];

	// Get the node and update its key value 
	minHeap->array[i]->key = key;

	// Travel up while the complete tree is not hepified. 
	// This is a O(Logn) loop 
	while (i && minHeap->array[i]->key < minHeap->array[(i - 1) / 2]->key) {
		// Swap this node with its parent 
		minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
		minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
		swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

		// move to parent index 
		i = (i - 1) / 2;
	}
}

// A utility function to check if a given vertex 
// 'v' is in min heap or not 
bool isInMinHeap(struct MinHeap* minHeap, int v)
{
	if (minHeap->pos[v] < minHeap->size)
		return true;
	return false;
}

void addFacility(individual& ind, facility fac1, facility fac2) {
	facility temp_fac;
	temp_fac.CoordX = (fac1.CoordX + fac2.CoordX) / 2;
	temp_fac.CoordY = (fac1.CoordY + fac2.CoordY) / 2;
	temp_fac.facCov = 0;
	temp_fac.nfacCov = 0;
	ind.facilitySet.push_back(temp_fac);
}

// The main function that constructs Minimum Spanning Tree (MST) 
// using Prim's algorithm 
int PrimMST(struct PGraph* graph, individual &ind)
{
	int V = graph->V; // Get the number of vertices in graph 
	std::vector<int> parent; // Array to store constructed MST 
	std::vector<float> key; // Key values used to pick minimum weight edge in cut 

	// minHeap represents set E 
	struct MinHeap* minHeap = createMinHeap(V);

	// Initialize min heap with all vertices. Key value of 
	// all vertices (except 0th vertex) is initially infinite 
	for (int v = 0; v < V; ++v) {
		parent.push_back (-1);
		key.push_back (INT_MAX);
		minHeap->array[v] = newMinHeapNode(v, key[v]);
		minHeap->pos[v] = v;
	}


	// Make key value of 0th vertex as 0 so that it 
	// is extracted first 
	key[0] = 0;
	minHeap->array[0] = newMinHeapNode(0, key[0]);
	minHeap->pos[0] = 0;

	// Initially size of min heap is equal to V 
	minHeap->size = V;

	// In the followin loop, min heap contains all nodes 
	// not yet added to MST. 
	while (!isEmpty(minHeap)) {
		// Extract the vertex with minimum key value 
		struct MinHeapNode* minHeapNode = extractMin(minHeap);
		int u = minHeapNode->v; // Store the extracted vertex number 

		// Traverse through all adjacent vertices of u (the extracted 
		// vertex) and update their key values 
		struct AdjListNode* pCrawl = graph->array[u].head;
		while (pCrawl != NULL) {
			int v = pCrawl->dest;

			// If v is not yet included in MST and weight of u-v is 
			// less than key value of v, then update key value and 
			// parent of v 
			if (isInMinHeap(minHeap, v) && pCrawl->weight < key[v]) {
				key[v] = pCrawl->weight;
				parent[v] = u;
				decreaseKey(minHeap, v, key[v]);
			}
			pCrawl = pCrawl->next;
		}
	}

	int DC = 1;
	float total = 0;
	int min = 99999;

	for (int i = 1; i < key.size(); i++)
	{
		total += key[i];
		//std::cout << parent[i] << " - " << i << std::endl;
		//cout << "Distance of " << key[i] << endl;
		if (key[i] > rc)
		{
			if (improvement && key[i] < 2*rc)
			{
				addFacility(ind, ind.facilitySet[i], ind.facilitySet[parent[i]]);
				DC -= 1;
			}

			DC += 1;			
		}
	}
	ind.avg_dis = total / (float)key.size();
	//std::cin >> DC;
	//cout << index << ". eff " << eadj[index] << " Edges: " << key.size() << " DC: " << DC << endl;
	return DC;
	
}

void findCost(population* pop_ptr) {

	starting = clock();
	int temp_numFac;

	for (int i = 0; i < popSize; i++)
	{
		temp_numFac = pop_ptr->ind[i].facilitySet.size();
		
		pop_ptr->ind[i].avg_dis = 0;

		if (temp_numFac > 1)
		{

			struct PGraph* graph = createGraph(temp_numFac);

			for (int j = 0; j < temp_numFac; j++)
			{
				facility* temp_facility = &(pop_ptr->ind[i].facilitySet[j]);

				for (int k = j; k < temp_numFac; k++)
				{
					if (j != k)
					{
						addEdge(graph, j, k, findDistance(
							temp_facility->CoordX,
							temp_facility->CoordY,
							pop_ptr->ind[i].facilitySet[k].CoordX,
							pop_ptr->ind[i].facilitySet[k].CoordY));
						/*cout << "Distance btw " << j << "-" << k << findDistance(
							temp_facility->CoordX,
							temp_facility->CoordY,
							pop_ptr->ind_ptr->facilitySet[k].CoordX,
							pop_ptr->ind_ptr->facilitySet[k].CoordY) << endl;*/
							/*std::cout << j << ". X: " << temp_facility->CoordX << "-- Y: " << temp_facility->CoordY << std::endl;
							std::cout << k << ". X: " << ind.facilitySet[k].CoordX << "-- Y: " << ind.facilitySet[k].CoordY << std::endl;
							std::cout << findDistance(
								temp_facility->CoordX,
								temp_facility->CoordY,
								ind.facilitySet[k].CoordX,
								ind.facilitySet[k].CoordY) << std::endl;*/
					}
				}

			}

			pop_ptr->ind[i].numDC = PrimMST(graph, pop_ptr->ind[i]);
			pop_ptr->ind[i].numRS = pop_ptr->ind[i].facilitySet.size() - pop_ptr->ind[i].numDC;
			pop_ptr->ind[i].fitness[0] = (pop_ptr->ind[i].numRS) * costRS + (pop_ptr->ind[i].numDC) * costDC;

		}
		else
		{
			pop_ptr->ind[i].numDC = 1;
			pop_ptr->ind[i].numRS = 0;
			pop_ptr->ind[i].fitness[0] = (pop_ptr->ind[i].numRS) * costRS + (pop_ptr->ind[i].numDC) * costDC;
		}

		//std::cout << i << ". SOL: " << ind.fitness[0] << std::endl;

	}

	ending = clock();

	costTime += ExecTime(starting, ending);

}
