// C / C++ program for Dijkstra's shortest path algorithm 
// for adjacency list representation of graph 
#include <chrono>
#include <stdio.h> 
#include <stdlib.h> 
#include <limits.h> 
#include <vector>
#include <cstdlib>

#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

#include <stdio.h> 
#include <stdlib.h> 
#include <limits.h> 
#include <algorithm>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <math.h>       /* sqrt */
#include <random>

#include <list> 


 // Modified from https://www.geeksforgeeks.org/dijkstras-algorithm-for-adjacency-list-representation-greedy-algo-8/
using namespace std::chrono;
using namespace std;

// A structure to represent a node in adjacency list 
struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode* next;
};

// A structure to represent an adjacency list 
struct AdjList
{
    struct AdjListNode* head;  // pointer to head node of list 
};

// A structure to represent a graph. A graph is an array of adjacency lists. 
// Size of array will be V (number of vertices in graph) 
struct Graph
{
    int V;
    struct AdjList* array;
};

// A utility function to create a new adjacency list node 
struct AdjListNode* newAdjListNode(int dest, int weight)
{
    struct AdjListNode* newNode =
        (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// A utility function that creates a graph of V vertices 
struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;

    // Create an array of adjacency lists.  Size of array will be V 
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));

    // Initialize each adjacency list as empty by making head as NULL 
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

// Adds an edge to an undirected graph 
void addEdge(struct Graph* graph, int src, int dest, int weight)
{
    // Add an edge from src to dest.  A new node is added to the adjacency 
    // list of src.  The node is added at the beginning 
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // Since graph is undirected, add an edge from dest to src also 

    // commented by Huan
    //newNode = newAdjListNode(src, weight);
    //newNode->next = graph->array[dest].head;
    //graph->array[dest].head = newNode;
}

// Structure to represent a min heap node 
struct MinHeapNode
{
    int  v;
    int dist;
};

// Structure to represent a min heap 
struct MinHeap
{
    int size;      // Number of heap nodes present currently 
    int capacity;  // Capacity of min heap 
    int* pos;     // This is needed for decreaseKey() 
    struct MinHeapNode** array;
};

// A utility function to create a new Min Heap Node 
struct MinHeapNode* newMinHeapNode(int v, int dist)
{
    struct MinHeapNode* minHeapNode =
        (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

// A utility function to create a Min Heap 
struct MinHeap* createMinHeap(int capacity)
{
    struct MinHeap* minHeap =
        (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->pos = (int*)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =
        (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
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
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size &&
        minHeap->array[left]->dist < minHeap->array[smallest]->dist)
        smallest = left;

    if (right < minHeap->size &&
        minHeap->array[right]->dist < minHeap->array[smallest]->dist)
        smallest = right;

    if (smallest != idx)
    {
        // The nodes to be swapped in min heap 
        MinHeapNode* smallestNode = minHeap->array[smallest];
        MinHeapNode* idxNode = minHeap->array[idx];

        // Swap positions 
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        // Swap nodes 
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
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

    //MinHeapNode  root2 = *root;

    //delete root;

    return root;
}

// Function to decreasy dist value of a given vertex v. This function 
// uses pos[] of min heap to get the current index of node in min heap 
void decreaseKey(struct MinHeap* minHeap, int v, int dist)
{
    // Get the index of v in  heap array 
    int i = minHeap->pos[v];

    // Get the node and update its dist value 
    minHeap->array[i]->dist = dist;

    // Travel up while the complete tree is not hepified. 
    // This is a O(Logn) loop 
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
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

// A utility function used to print the solution 
void printArr(int dist[], int n)
{
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
}


int dijkstra(struct Graph* graph, int src, int end)
{
    int V = graph->V;// Get the number of vertices in graph 
    // int dist[V];	 // dist values used to pick minimum weight edge in cut 
    int* dist = new int[V];

    // minHeap represents set E 
    struct MinHeap* minHeap = createMinHeap(V);

    // Initialize min heap with all vertices. dist value of all vertices 
    for (int v = 0; v < V; ++v)
    {
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    // Make dist value of src vertex as 0 so that it is extracted first 
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);

    // Initially size of min heap is equal to V 
    minHeap->size = V;

    // In the followin loop, min heap contains all nodes 
    // whose shortest distance is not yet finalized. 
    while (!isEmpty(minHeap))
    {
        // Extract the vertex with minimum distance value 
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v; // Store the extracted vertex number 

        // Traverse through all adjacent vertices of u (the extracted 
        // vertex) and update their distance values 
        struct AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;

            // If shortest distance to v is not finalized yet, and distance to v 
            // through u is less than its previously calculated distance 
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX &&
                pCrawl->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl->weight;

                // update distance value in min heap also 
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }

    // print the calculated shortest distances 
    // printArr(dist, V); 
    int res = dist[end];
    delete[] dist;
    return res;
}

int dijkstra_2nodes(struct Graph* graph, int src, int dest)
{
    int V = graph->V;// Get the number of vertices in graph 
    //int dist[V];
    int* dist = new int[V];      // dist values used to pick minimum weight edge in cut 
    //std::vector<int> dist[V];


    // minHeap represents set E 
    struct MinHeap* minHeap = createMinHeap(V);

    // Initialize min heap with all vertices. dist value of all vertices  
    for (int v = 0; v < V; ++v)
    {
        dist[v] = INT_MAX;

        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    // Make dist value of src vertex as 0 so that it is extracted first 
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);

    // Initially size of min heap is equal to V 
    minHeap->size = V;

    // In the followin loop, min heap contains all nodes 
    // whose shortest distance is not yet finalized. 
    while (!isEmpty(minHeap))
    {
        // Extract the vertex with minimum distance value 
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v; // Store the extracted vertex number 
        if (u == dest)
        {
            printf("Found the path: %d, distance: %d \n", u, dist[u]);
            int d = dist[u];

            delete[] dist;
            delete[] minHeap;

            return d;
        }

        // Traverse through all adjacent vertices of u (the extracted 
        // vertex) and update their distance values 
        struct AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;

            // If shortest distance to v is not finalized yet, and distance to v 
            // through u is less than its previously calculated distance 
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX &&
                pCrawl->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl->weight;

                // update distance value in min heap also 
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }

        delete[] minHeapNode;
        //delete u;

        delete[] pCrawl;
    }

    delete[] dist;
    delete[] minHeap;
    // print the calculated shortest distances 
   //printArr(dist, V);
}

struct edge {
    long iStart;
    long iEnd;
    double dW;
};


struct OneRow {
    int row[3];
};


bool compare(OneRow& l, OneRow& r)
{
    int col_cnt = 4;
    for (int i = 0; i < col_cnt; i++)
    {
        //cout << i << endl;
        if (l.row[i] < r.row[i])
        {
            //cout << l.row[i] << endl;
            return true;
        }

        else if (l.row[i] > r.row[i])
            return false;
    }
}


int  selectMinWeight(OneRow* Rows, OneRow* Rows_final, int m)
{
    int m_final = 0;

    Rows_final[m_final] = Rows[0];
    m_final++;

    for (int i = 1; i < m; i++)
    {
        if (Rows_final[m_final - 1].row[0] != Rows[i].row[0])
        {
            Rows_final[m_final] = Rows[i];
            m_final++;
        }
        else if (Rows_final[m_final - 1].row[1] != Rows[i].row[1])
        {
            Rows_final[m_final] = Rows[i];
            m_final++;
        }
    }

    return m_final;
}

// Driver program to test above functions 
int main()
{

    std::cout << "program started";

    ifstream read_file("R-MAT/rmat.gr");


    string line;
    // skip the first 7 lines
    for (int i = 0; i < 7; i++)
    {
        getline(read_file, line);
    }

    int n;
    int m;
    int u;
    int v;
    int w;
    string t;  // tempary variable.
    read_file >> t;
    read_file >> t;

    read_file >> n;
    read_file >> m;

    cout << "n:" << n << endl;
    cout << "m:" << m << endl;

    //struct Graph* graph = createGraph(n);

    OneRow* Rows = new OneRow[m];
    OneRow* Rows_final = new OneRow[m];

    //edge* edges = new edge[m];
    //int* dist = new int[V];

    time_t start_time = time(NULL);

    clock_t ct;
    ct = clock();

    int lLine_cnt = 0;
    while (getline(read_file, line))
    {
        //cout << "line:" << line.c_str() << endl;
        read_file >> t;
        read_file >> u;
        read_file >> v;
        read_file >> w;

        //cout << "u, v, w: " << u << ", " << v << ", " << w << endl;

        //addEdge(graph, u, v, w);

        Rows[lLine_cnt].row[0] = u;
        Rows[lLine_cnt].row[1] = v;
        Rows[lLine_cnt].row[2] = w;


        lLine_cnt++;


        if (lLine_cnt % 100000 == 0)
        {
            cout << "Current line: " << lLine_cnt << endl;
            //cout << "u, v: " << u << ", " << v << endl;
            cout << "u, v, w: " << u << ", " << v << ", " << w << endl;
        }

        //cout << "u, v: " << u << ", " << v << endl;

    }

    //sort(Rows, Rows + m, compare);

    time_t end_time = time(NULL);
    cout << "Seconds for data reading (s): " << (double)(clock() - ct) / CLOCKS_PER_SEC << endl;
    ct = clock();

    start_time = time(NULL);

    int m_final = 0;

    //m_final = selectMinWeight(Rows, Rows_final, m);
    //cout << "m_final: " << m_final << endl;

    struct Graph* graph = createGraph(n + 1);

    // print edges to check
        //for (int i = 0; i < m_final; i++)
    //{
    //    for (int j = 0; j < 3; j++)
    //        cout << Rows_final[i].row[j] << " ";
    //    cout << "" << endl;
    //}

    addEdge(graph, 1, 0, 0);

    for (int i = 0; i < m; i++)
    {
        //for (int j = 0; j < 3; j++)
        //    //cout << Rows_final[i].row[j] << " ";
        //    cout << Rows[i].row[j] << " ";

        //cout << Rows_final[i].row[0] << " ";
        //cout << Rows_final[i].row[1] << " ";
        //cout << Rows_final[i].row[2] << " ";
        //cout << " " << endl;
        //addEdge(graph, Rows_final[i].row[0], Rows_final[i].row[1], Rows_final[i].row[2]);
        addEdge(graph, Rows[i].row[0], Rows[i].row[1], Rows[i].row[2]);

        if (lLine_cnt % 100000 == 0)
        {
            cout << "Current edge: " << lLine_cnt << endl;
            //cout << "u, v: " << u << ", " << v << endl;
            cout << "u, v, w: " << Rows[i].row[0] << ", " << Rows[i].row[1] << ", " << Rows[i].row[2] << endl;
        }
    }

    end_time = time(NULL);

    cout << "Seconds for graph constructing: " << (double)(clock() - ct) / CLOCKS_PER_SEC << endl;
    ct = clock();
    
    start_time = time(NULL);

    //dijkstra(graph, 1);
    for (int i = 0; i < 200; i++)
    {

        //int iDest = rand() % n;
        auto now = std::chrono::system_clock::now();
        auto now_ms = time_point_cast<std::chrono::milliseconds>(now);
        auto epoch = now_ms.time_since_epoch();
        auto value = duration_cast<std::chrono::milliseconds>(epoch);
        long duration = value.count();
        srand((unsigned int)duration);

        long value_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch()).count();

        std::random_device rd;
        std::mt19937 mt(rd());

        float a = (float)mt() / 2 / (float)INT_MAX;
        float b = (float)mt() / 2 / (float)INT_MAX;
        int iDest = a * n;
        int iSrc = b * n;

        printf("%d, iSrc: %d, iDest: %d \n", i, iSrc, iDest);
        int distance = dijkstra_2nodes(graph, iSrc, iDest);

        //dijkstra(graph, iSrc, iDest);

        now = std::chrono::system_clock::now();
        now_ms = time_point_cast<std::chrono::milliseconds>(now);
        epoch = now_ms.time_since_epoch();
        value = duration_cast<std::chrono::milliseconds>(epoch);
        duration = value.count();
        srand((unsigned int)(iDest * value_ms));

    }

    end_time = time(NULL);

    cout << "Seconds for dijkstra() 200 times (s): " << (double)(clock() - ct) / CLOCKS_PER_SEC << endl;

    return 0;
}