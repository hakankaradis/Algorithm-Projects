// written by Hakan Karadis
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
// structs
typedef struct{
        int src, dest, weight;
}*Nodes;


typedef struct NodeList{
        int dest;
        int random;
        struct NodeList *next;
}NodeList;

typedef struct {
        struct NodeList *head;
}AdjList;

struct GainList{
	int cell_id;
	int size;
	struct GainList *next;
	struct GainList *prev;	
}**headL, **lastL, **headR, **lastR;
 
typedef struct {
	int id;
	int partition; // takes 0 or 1 means left right
	int locked; // takes 0 or 1 means cell is locked or not
	int gain;
	struct GainList *gainList;
}FreeList; 

struct Graph{
        AdjList *list;
        int V;
};

// functions
void fillFreeList();
int findTheMaxDegree();
void initializeGainList();
void gainCell();
void addToGainList(int, int, int);
void display();
void delete_from_middle(int way, int gain, int value);
int findBaseCell();
void moveBaseCell(int);
int computeCut();
int balance(int, int);
void writeResults();

// global variables
int number_of_vertex; // holds number of vertex
int number_of_edge;   // holds number of edge
int sizeL=0;
int sizeR=0;
int maxDegree;
FreeList *freeList;
FreeList *newFreeList;
struct GainList *gainListLeft;
struct GainList *gainListRight;
struct Graph *graph;
FILE *fp;

int main(){
	fp = fopen("partitionResult.txt", "w");  
	int i=0;
        scanf("%d", &number_of_vertex);  //3
        scanf("%d", &number_of_edge);   //2
        
        // construct Graph and allocate spaces for each edge
        graph= malloc(sizeof(struct Graph));
        graph->V = number_of_vertex;
        graph->list = malloc(sizeof(AdjList)*graph->V);
        //graph->edgeList = malloc(sizeof(EdgeList)*number_of_edge);

	// allocate space for FreeList
	freeList = malloc(sizeof(FreeList)*number_of_vertex);  // hold the free nodes
	newFreeList = malloc(sizeof(FreeList)*number_of_vertex); // to hold the new freeList 

        for (i=0; i<graph->V; i++) {
       		graph->list[i].head = NULL;
        }
	for (i=0; i<graph->V; i++) { // allocate spaces and give initial values
		freeList[i].gainList = malloc(sizeof(struct GainList*));
		newFreeList[i].gainList = malloc(sizeof(struct GainList*));
		freeList[i].gainList->cell_id = i;
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
        for(i=0; i<number_of_edge;i++){
                
                NodeList *node = malloc(sizeof*node);
                node->random = rand()%2;
                node->dest = nodes[i]->dest;
                node->next = graph->list[nodes[i]->src].head;
                graph->list[nodes[i]->src].head = node;
		// because of undirect
		NodeList *node2 = malloc(sizeof*node2);
		node2->random = rand()%2;
		node2->dest = nodes[i]->src;
		node2->next = graph->list[nodes[i]->dest].head;
		graph->list[nodes[i]->dest].head = node2;
        }

	// initialize GainList
	maxDegree =	findTheMaxDegree();
	headL = malloc(sizeof(struct GainList*)*2*maxDegree+1);
	headR = malloc(sizeof(struct GainList*)*2*maxDegree+1);
	lastL = malloc(sizeof(struct GainList*)*2*maxDegree+1);
	lastR = malloc(sizeof(struct GainList*)*2*maxDegree+1);
		
	initializeGainList();
		
	// fill FreeList
	fillFreeList();
		
		
	int cut;
	int minCut;
	cut = computeCut();
	minCut = INT_MAX;
	printf("Initial Number of Cut %d\n", cut);
	gainCell();
	// display();
	// gain cell
	int j;
	do{	
		for(i=0; i<graph->V;i++){
			// base cell
			int baseCellId = findBaseCell();
			moveBaseCell(baseCellId);
			cut = computeCut();
			if(cut < minCut){ 
				minCut = cut; 
				for (j=0; j<graph->V; j++) {
					newFreeList[j].gain = freeList[j].gain;
					newFreeList[j].partition = freeList[j].partition;
					newFreeList[j].locked = 0;//freeList[j].locked;
				}
			}
			gainCell();
			
		}
		for (j=0; j<graph->V; j++) {
			freeList[j].gain = newFreeList[j].gain;
			freeList[j].partition = newFreeList[j].partition;
			freeList[j].locked =0;// newFreeList[j].locked;
		}
		gainCell();
	}while(cut != minCut);
	
	printf("Size of Left %d and Size of Right %d\n", sizeL, sizeR);
	printf("Min Cut is %d\n", minCut);
	
	writeResults(); // write final partition to file
	
	fclose(fp);
    	return 0;
}

void writeResults(){
	int net=0;
	int i;
	for (i=0; i< graph->V; i++) { 
    		if(freeList[i].partition == 0){ // if the nodes are in different block, increment 
			fprintf(fp, "%d\n", 0);
		}
		else{
			fprintf(fp,"%d\n", 1);		
		}
	}
}

// after finding baseCellId, move it to other block by changing its partition and then lock it.
void moveBaseCell(int baseCellId){
	freeList[baseCellId].locked = 1;
	freeList[baseCellId].partition = (freeList[baseCellId].partition == 0) ? 1:0;
}
int max(int x, int y){  // find the maximum 
	return (x>y)?0:1;
}

// decide the way to send the node from one block to other
int balance(int sizeLeft, int sizeRight){
	float percentage = graph->V*0.1;
	float total = abs(sizeL-sizeR);
	int way = max(sizeLeft,sizeRight);
	if(total < percentage){ // both block can send node each other
		return 2;
	}
	else{
		return (way==0)?0:1; // only one way is allowed
	}
}


int findBaseCell(){
	// search in FreeList and if it is not locked
	int flag=0;
	int maxGainId=0;
	int maxGain=INT_MIN;
	//printf("Gains of Free List\n");
	//printf("size Left %d and Size Right %d\n", sizeL, sizeR);
	int i;
	
	int *leftArr = malloc(sizeof*leftArr*sizeL);
	int *rightArr = malloc(sizeof*rightArr*sizeR);
	int lCount=0;
	int rCount=0;
	int lMax = INT_MIN;
	int lMaxId =0;
	int rMax = INT_MIN;
	int rMaxId =0;
	for (i=0; i<graph->V; i++) {
		
		if(freeList[i].partition ==0){
			leftArr[lCount] = freeList[i].gain;
			if(leftArr[lCount] > lMax  && freeList[i].locked == 0){
				lMax = leftArr[lCount];
				lMaxId = i;
			}
			lCount++;
		}
		else{
			rightArr[rCount] = freeList[i].gain;
			if(rightArr[rCount] > rMax && freeList[i].locked == 0){
				rMax = rightArr[rCount];
				rMaxId = i;
			}
			rCount++;
		}
	}

	sizeL = lCount;
	sizeR = rCount;
	int result = balance(sizeL, sizeR); // check for status which way is suitable to send node other block
	
	if(result == 0){
		sizeL--; sizeR++;
		return lMaxId;
	}
	else if(result == 1){
		sizeR--; sizeL++;
		return rMaxId;
	}
	else {
		if(lMax>rMax){
			sizeL--; sizeR++;
			return lMaxId;
		}
		sizeR--; sizeL++;
		return rMaxId;
	}

}

// compute gain of each nodes
void gainCell(){
	int old_gain=0;
	int i;
	initializeGainList();
	// zeros to freeList Gain
	for (i=0; i<number_of_vertex; i++) {
		freeList[i].gain = 0;
	}
	
	for (i=0; i<number_of_vertex; i++) {
		NodeList *node = graph->list[i].head;
		while (node) {
			// check for block and if they are in different block increment otherwise decrement the gain
			if(freeList[i].partition != freeList[node->dest].partition )  
				old_gain++;
			else
				old_gain--;
			node = node->next;
		}
		freeList[i].gain = old_gain; // add gain the freeList.gain
		addToGainList(freeList[i].partition, old_gain, i); // add this node to GainList
		// sol taraf icin tuttugun gain arrayine yaz. add array function iki arguman olsun sol icin 0 sag icin 1 gonder canim benim.
		old_gain=0;
	}
}

// display the gainList
void display()
{
	int i=0;
    	struct GainList *temp;
//	printf("--------Left Side Gain List---------\n");
	for (i=2*maxDegree; i>=0; i--) {
		
    		temp=headL[i];
		printf("%d --> ",i);
	    	if(temp==NULL){
	       		printf("List is Empty");
	    	}
	    	while(temp!=NULL){
			if(freeList[temp->cell_id].locked == 0)
	        	printf("-> %d ",temp->cell_id);
	         	temp=temp->next;
	    	}
		printf("\n");
	}
	printf("--------Right Side Gain List---------\n");
	for (i=2*maxDegree; i>=0; i--) {
	    	temp=headR[i];
		printf("%d --> ",i);
		if(temp==NULL){
		        printf("List is Empty");
		}
		while(temp!=NULL){
			printf("-> %d ",temp->cell_id);
		    	temp=temp->next;
		}
		printf("\n");
	}
} 

void delete_from_middle(int way, int gain, int value){
    struct GainList *temp,*var,*t, *temp1;
	if(way ==0){
	    temp=headL[gain];
	    while(temp!=NULL)
	    {
	        if(temp->cell_id == value)
	        {
	             if(temp->prev==NULL)
	             {
	                  free(temp);
	                  headL[gain]=NULL;
	                  lastL[gain]=NULL;

	                  break;
	             }
	             else
	             {
	                  var->next=temp1;
	                  temp1->prev=var;
	                  free(temp);

	                  break;
	             }
	        }
	        else
	        {
	              var=temp;
	              temp=temp->next;
	              temp1=temp->next;
	        }
	    }
	}
	else {
		temp=headR[gain];
	    while(temp!=NULL)
	    {
	        if(temp->cell_id == value)
	        {
	             if(temp->prev==NULL)
	             {
	                  free(temp);
	                  headR[gain]=NULL;
	                  lastR[gain]=NULL;

	                  break;
	             }
	             else
	             {
	                  var->next=temp1;
	                  temp1->prev=var;
	                  free(temp);

	                  break;
	             }
	        }
	        else
	        {
	              var=temp;
	              temp=temp->next;
	              temp1=temp->next;
	        }
	    }
	}
   // printf("data deleted from list is %d",value);
}

// add the node to appropriate block and place
void addToGainList(int way, int gain,int index){
	//printf("Gain is %d for index %d\n", gain, index);
	gain += maxDegree; // add max degree for exact location in gain array
	if(way ==0){ // left block
		struct GainList *var, *temp;
		var = malloc(sizeof(struct GainList));
		var->cell_id = index;// freeList[index].gainList->cell_id;
		if(headL[gain] == NULL){  // if there are no other node in that line
			headL[gain] = var;
			lastL[gain] = headL[gain];
			headL[gain]->prev = NULL;
			headL[gain]->next = NULL;
		}
		else { // add the node in front of next node
			temp = var;
			temp->prev = NULL;
			temp->next =headL[gain];
			headL[gain]->prev = temp;
			headL[gain] = temp;
		}
	}
	// do samething for right block
	else {
		struct GainList *var, *temp;
		var = malloc(sizeof(struct GainList));
		var->cell_id = index;//freeList[index].gainList->cell_id;
		if(headR[gain] == NULL){
			headR[gain] = var;
			lastR[gain] = headR[gain];
			headR[gain]->prev = NULL;
			headR[gain]->next = NULL;
		}
		else {
			temp = var;
			temp->prev = NULL;
			temp->next =headR[gain];
			headR[gain]->prev = temp;
			headR[gain] = temp;
		}
	}
}

void initializeGainList(){ // initialize both left and right gainlists
	int i;
	for (i=0; i<2*maxDegree+1; i++) {
		headL[i] = NULL; headR[i] = NULL; lastL[i] = NULL; lastR[i] = NULL; 
	}
}// this is for initial of freeList
void fillFreeList(){
	int i;
	for (i=0; i<number_of_vertex; i++) {
		freeList[i].gain = 0;
		freeList[i].id = i; // assign id of each freelist node
		
		if(sizeL>sizeR){
			freeList[i].partition = 1; // assign 0  
			sizeR++;
		}
		else {
			freeList[i].partition = 0; // assign 1
			sizeL++;
		}
		freeList[i].locked = 0; // false 		
	}
}
// find the maximum edges that are connected to a node
int findTheMaxDegree(){
	int i;
	int max=0;
	int index=0;
    	for (i=0; i< graph->V; i++) { 
		NodeList *node = graph->list[i].head; 
		while (node){ 
           		node = node->next; 
			index++;
        	} 
		if(max < index)
			max = index;
		index = 0;
        }

	return max;
}

// copute the number of cuts
int computeCut(){
	int net=0;
	int i;
	for (i=0; i< graph->V; i++) { 
    	NodeList *node = graph->list[i].head; 	
        while (node){ 
		if(freeList[i].partition != freeList[node->dest].partition){ // if the nodes are in different block, increment 
			net++;
		}
	     	node = node->next; 
	    } 
	}
	
	return net/2;
}

