#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct List{
        int dest,weight;
        struct List *next;
}*NodeList;
typedef struct {
        NodeList *head;
        int V;
}HeadList;

typedef struct{
        int src, dest, weight; // hold destination, source and weight for each node
}*Nodes;

typedef struct{ // hold value and id of nodes
	int id;
	int value;
}*MinHeap;

int number_of_vertex; // holds number of vertex
int number_of_edge;   // holds number of edge
Nodes *nodes;
NodeList *node_array;  // to keep track all nodes
HeadList *head_list;  // holds all nodes after dijsktra algorithm implemented
int heapsize;  // holds size of all nodes
int *position; // holds position of each node
int *values;  // holds values of each node

void dijkstra (HeadList *, int );
MinHeap extractMin(MinHeap * );
void Min_Heapify(MinHeap *, int );
void relax(MinHeap *, int , int );

int Left(int );
int Right(int );
int main(){
        if(scanf("%d", &number_of_vertex)==1); //3
        if(scanf("%d", &number_of_edge)==1);  //2
        int i;
	int x;
	// allocate space for node, nodelist and head_list
        nodes = malloc(sizeof(Nodes)*number_of_edge);
        head_list = malloc(sizeof(HeadList));
	head_list->V = number_of_vertex;
        head_list->head = (NodeList*)malloc(sizeof(NodeList)*number_of_vertex+1);
	for (x=0; x<number_of_vertex+1; x++) {
		head_list->head[x] = NULL;
	}

	// create node and asign src, dest and weight value with data came from user
        for(i=0; i<number_of_edge; i++){
                Nodes node = malloc(sizeof(Nodes));
                if(scanf("%d", &node->src)==1); 
                if(scanf("%d", &node->dest)==1);
                if(scanf("%d", &node->weight)==1);
                nodes[i] = node;
        }
	// create nodelist and add each node with their destination and weight values
        for(i=0; i<number_of_edge;i++){
                NodeList node = malloc(sizeof(NodeList));
                node->dest = nodes[i]->dest;
                node->weight = nodes[i]->weight;
		// from source to destination
                node->next = head_list->head[nodes[i]->src];
                head_list->head[nodes[i]->src] = node;  
                // undirected way, implement opposite way
		NodeList node2 = malloc(sizeof(NodeList));
		node2->dest = nodes[i]->src;
		node2->weight = nodes[i]->weight;
		// from source to destination
                node2->next = head_list->head[nodes[i]->dest];
                head_list->head[nodes[i]->dest] = node2;   
        }

	dijkstra(head_list, 0); // call dijksta algorithm with source 0
	int max = values[0];
	for(i=1; i< number_of_vertex; i++){
		if(values[i]>max){
			max = values[i];
		}
	}
	printf("%d\n", max); //print out the last node

}

void dijkstra (HeadList *head_list, int src){
	int V = head_list->V;
	position = malloc(sizeof(int)*V);  // allocates spaces for position array
	values = malloc(sizeof(int)*V);    // allocates spaces for values array
	int y;
	MinHeap *heap = malloc(sizeof*heap*V);  // allocates space for Min Heap struct 
	for (y=0; y<V;y++) {
		heap[y] = malloc(sizeof(MinHeap));
	}
	heapsize = V;
	int v;
	// assign infinite to weight of all node
	for (v=0; v<V; v++) {
		values[v] = INT_MAX;
		heap[v]->id = v;
		heap[v]->value = INT_MAX;
		position[v] = v;
	}
	//except source 0.
	// assign it zero
	heap[src]->value = src;
	heap[src]->id = src;
	position[src] = src;
	values[src] = 0;
	relax(heap, src, values[src]);
	 while (heapsize!=0){
		MinHeap uNode = extractMin(heap);  // return the min node
		int u = uNode->id;
		NodeList node = head_list->head[u]; // to check the node has next node
		while(node != NULL){
			int v = node->dest;
			if (position[v] < heapsize && values[u] != INT_MAX && node->weight + values[u] < values[v]){
				//printf("u is %d and v is %d\n",u,v);
	        	values[v] = values[u] + node->weight;

	            relax(heap, v, values[v]);
	        }
		    node = node->next;
		}
	}
}
void relax(MinHeap *heap, int src, int val){
	int i = position[src];
	heap[i]->value = val;
	int j;

	while ((j = i)) {
	// check node with its parent and swap it if it is small
		if(heap[i]->value < heap[(i - 1) / 2]->value){
			position[heap[i]->id] = (i-1)/2;
	        	position[heap[(i-1)/2]->id] = i;
			MinHeap temp = heap[i];
			heap[i] = heap[(i - 1) / 2];
			heap[(i - 1) / 2] = temp;
		}
		i = (i - 1) / 2;
	}

}
// this code is used from project 6
MinHeap extractMin(MinHeap *heap){

	if(heapsize == 0) return NULL;
	int index=0;
	MinHeap n = malloc(sizeof(MinHeap));
	int a;

	n = heap[0];
	heapsize--;

	for (a=1; a<=heapsize ;a++) {
		if(heap[a]->value < n->value){
			n = heap[a];  // find the small node
			index = a;
		}
	}

	heap[0] = heap[heapsize];
	// change node with its new position
	position[heap[0]->id]=heapsize;
	position[heap[heapsize]->id]=0;
	Min_Heapify(heap, 0); // call min heapify function with parent node (0)

	return n;
}
// this code is used from project 3
void Min_Heapify(MinHeap *heap, int id){ // sort sub trees to decide parent  by exchanging leaf and root
		int l = Left(id);
	  int r = Right(id);
	  int smallest;
	  if(l <= heapsize && heap[l]->value < heap[id]->value){
	    smallest = l;
	  }else{
	    smallest = id;
	  }
	  if(r <= heapsize  && heap[r]->value < heap[smallest]->value){
	    smallest = r;
	  }
	  if(smallest != id){  // decide smallest one and exchange with leaf
	    position[heap[smallest]->id] = id;
	    position[heap[id]->id] = smallest;
	    MinHeap temp = heap[id]; heap[id] = heap[smallest]; heap[smallest] = temp;
	    Min_Heapify(heap, smallest);
	  }
}
// this code is used from project 3
int Left(int i){ // return left node

  	return 2*i+1; 
}
// this code is used from project 3 
int Right(int i){ // return right node

   	return 2*i+2; 
}
