#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
// structs
typedef struct Edge{
  	int dest;
  	int flow;
  	int capacity;
  	struct Edge *next;
  	struct Edge *mirror;
}Edge;

typedef struct {
	struct Edge *head;
}AdjList; 
struct Graph{
        AdjList *list;
        int V;
};

typedef struct{
        int src, dest, weight;
}*Nodes;

// global variables
int number_of_vertex; // holds number of vertex
int number_of_edge;   // holds number of edge
int *parent;
int queue_counter=0;

// functions 
int* pop(int *arr, int size);
int BFS(struct Graph *graph, int s, int t);
int Edmands_Karp(struct Graph *graph, int source, int sink);
int min(int a, int b);

int main(){
	int i=0;
	scanf("%d", &number_of_vertex);  //3
	scanf("%d", &number_of_edge);   //2
	
	// construct Graph and allocate spaces for each edge
	struct Graph *graph= malloc(sizeof(struct Graph));
	graph->V = number_of_vertex;
	graph->list = malloc(sizeof(AdjList)*graph->V);
	for (i=0; i<graph->V; i++) {
		graph->list[i].head = NULL;
	}
	
	Nodes *nodes = malloc(sizeof(Nodes)*number_of_edge);
	// take source, destinaton and weight from user and hold them into Nodes list
	for(i=0; i<number_of_edge; i++){
	    Nodes node = malloc(sizeof(Nodes));
	    scanf("%d", &node->src); 
	    scanf("%d", &node->dest);
	    scanf("%d", &node->weight);
	    nodes[i] = node;
	}
	
	// add edges, make connection between nodes and hold them all in graph->list
	for(i=number_of_edge-1; i>=0;i--){
		Edge *edge = malloc(sizeof*edge);
		edge->dest = nodes[i]->src;
		edge->flow = nodes[i]->weight; 
		edge->capacity = nodes[i]->weight;
		edge->next = graph->list[nodes[i]->dest].head;
		graph->list[nodes[i]->dest].head = edge;
		
		
		Edge *edge2 = malloc(sizeof*edge2);
		edge2->dest = nodes[i]->dest;
		edge2->capacity = 0;//nodes[i]->weight;
		edge2->flow = 0;
		edge2->next = graph->list[nodes[i]->src].head;
		graph->list[nodes[i]->src].head = edge2;
		
		graph->list[nodes[i]->dest].head->mirror = graph->list[nodes[i]->src].head;
		//graph->list[nodes[i]->dest].head->mirror = graph->list[nodes[i]->src].head;
	}
        // print out the max flow from source to destination
	printf("max flow is %d\n",Edmands_Karp(graph, 0, number_of_vertex-1));
	return 0;
}

// find the maximum source to destination cut and return it
int Edmands_Karp(struct Graph *graph, int source, int sink){
	int max_flow = 0;
	int u, v;
	parent = malloc(sizeof(int)*number_of_vertex); // allocates spaces for parent array to hold which node is visited
	
	// create residual graph
	struct Graph *rgraph = graph;
	
	while (BFS(rgraph, source, sink)) {
		int path_flow =INT_MAX;
		for (v=sink; v!=source; v = parent[v]) {
			u=parent[v];
			Edge *now = rgraph->list[v].head;
			while(now){
				// find the minimum residual capacity of edges.
				if(now->dest == u){
					path_flow = min(path_flow, now->flow);
				}
		   		now= now->next;
			}
			
		}
		for (v=sink; v!=source; v = parent[v]) {
			u=parent[v];
			Edge *now = rgraph->list[v].head;
			while(now){
				if(now->dest == u){
					// update residual capacities of edges and reverse edges				
					now->flow -= path_flow;
					now->mirror->flow += path_flow;
				}
				now= now->next;
				if(rgraph->list[v].head->flow == 0) break;
			}
		}
		max_flow += path_flow;
	}
	
	return max_flow;
}

int BFS(struct Graph *graph, int s, int t){
	int i=0; int v=0;
    	int *visited = malloc(sizeof(int)*number_of_vertex);
	for (i=0; i<number_of_vertex; i++) { // give initial values zero that means "not visited"
		visited[i] = 0;
  	}	
	int *q = malloc(sizeof(int)*number_of_vertex); // allocates space for q array to hold each vertex
	q[queue_counter] =s; // add source to queue
	queue_counter++;
	visited[s]=1;
	parent[s] = -1;
	
	while(queue_counter != 0){ // loop for all elements in q array
		int u = q[0];  // take first one from q array
		q = pop(q, queue_counter);
		queue_counter--;
		for (v=0; v<graph->V; v++) { // search for all vertex
			Edge *now = graph->list[v].head;
			while (now) {
				// but its destination has to be equal first element in the q array
				// that means mark the correct node as it is visited
				if(visited[v] == 0 && now->flow>0 && now->dest == u){ 
					q[queue_counter] = v;
					queue_counter++;
					parent[v] = u;
					visited[v] = 1;
				}
				else // search for next node
				    now = now->next;
			}
		}
	}
	return (visited[t]==1); // return true if the node is visited
}
// to pop first element from array
int* pop(int *arr, int size){
	int i=0;
	for (i=0; i<size-1; i++) {
		arr[i] = arr[i+1];
	}
	
	return arr;
}
// find minimum
int min(int a, int b){
	if (a>b) return b;
	else return a;
}
