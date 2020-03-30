// Program to print BFS traversal from a given 
// BFS(int s) traverses vertices reachable from s. 
#include <iostream> 
#include <list> 
#include <stdio.h>
#include<time.h>

using namespace std; 
const int MAX_LINE_LEN = 100;


// This class represents a directed graph using 
// adjacency list representation 
class Graph 
{ 
	int V; // No. of vertices 

	// Pointer to an array containing adjacency 
	// lists 
	list<int> *adj; 
public: 
	Graph(int V); // Constructor 

	// function to add an edge to graph 
	void addEdge(int v, int w); 

	// prints BFS traversal from a given source s 
	void BFS(int s); 
}; 

Graph::Graph(int V) 
{ 
	this->V = V; 
	adj = new list<int>[V]; 
} 

void Graph::addEdge(int v, int w) 
{ 
	adj[v].push_back(w); // Add w to v’s list. 
} 

void Graph::BFS(int s) 
{ 
	// Mark all the vertices as not visited 
	bool *visited = new bool[V]; 
	for(int i = 0; i < V; i++) 
		visited[i] = false; 

	// Create a queue for BFS 
	list<int> queue; 

	// Mark the current node as visited and enqueue it 
	visited[s] = true; 
	queue.push_back(s); 

	// 'i' will be used to get all adjacent 
	// vertices of a vertex 
	list<int>::iterator i; 

	while(!queue.empty()) 
	{ 
		// Dequeue a vertex from queue and print it 
		s = queue.front(); 
		// cout << s << " "; 
		queue.pop_front(); 

		// Get all adjacent vertices of the dequeued 
		// vertex s. If a adjacent has not been visited, 
		// then mark it visited and enqueue it 
		for (i = adj[s].begin(); i != adj[s].end(); ++i) 
		{ 
			if (!visited[*i]) 
			{ 
				visited[*i] = true; 
				queue.push_back(*i); 
			} 
		} 
	} 
} 

Graph * pG = NULL;

// Driver program to test methods of graph class 
int main() 
{ 
	char str[MAX_LINE_LEN + 5];
	
	freopen("ssca2.gr","r",stdin);

	char a;
	char tmp[10];
	int u,v,w;
	int num_v; // number of vertices
	int num_e; // number of edges

	int i =0;

	
    while (cin.getline(str, MAX_LINE_LEN + 5)){
        if(str[0]=='p'){
        	sscanf(str,"%c %s %d %d", &a, tmp, &num_v, &num_e);
        	Graph g(num_v+1);
        	pG = &g;
        	printf("N=%d, M=%d \n", num_v, num_e);
        }else if(str[0]=='a'){
        	sscanf(str,"%c %d %d %d", &a, &u, &v, &w);
        	// cout<<"edge:"<<u<<" "<<v<<" "<<w<<endl;
			pG->addEdge(u, v);
        }
    }
	cout<<"read file done"<<endl;

	clock_t startTime,endTime;
	startTime = clock();
    pG->BFS(1);
	endTime = clock();
	cout<<"bfs time:"<< (double)(endTime - startTime) / CLOCKS_PER_SEC << " s"<<endl;

	return 0; 
} 
