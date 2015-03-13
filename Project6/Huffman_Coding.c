#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

typedef struct node {	
		struct node *left, *right; //pointer to left or right child
		int freq; //frequency 
		char name; // charcter
		int flag;  // is there tie?
} *nodes;
int number_of_nodes=0;  // holds how many nodes we have currently
int *freq;   //  holds frequency of each node
char *char_array;   // holds char of each node
nodes *Q;       // holds all nodes that contains tied nodes
nodes *C;       // holds initial nodes
char *code[128] = {0}, buf[1024]; //initialize character array with a null character

nodes create_node(int freq, char chr); 
nodes HUFFMAN(nodes *C);
void INSERT(nodes *Q, nodes z);
nodes EXTRACT_MIN(nodes *Q);
void secret_message(nodes n, char *s, int len);

int main(int argc, char *argv[]) {
	
	scanf("%d", &number_of_nodes);   // take node count
	int number_of_nodes2 = number_of_nodes;  // hold same count to use later
	freq = malloc(sizeof(int)*number_of_nodes);  // allocate space for frequency
	char_array = malloc(sizeof(char)*number_of_nodes); // allocate space for char array
	char t;
	int a;
  	// fill frequency, char array, encoded bits, number of encoded bits
	for (a=0; a<number_of_nodes; a++) {
		do {
			t = getchar();
		} while (t == ' ' || t == '\n');
		
		char_array[a] = t;
		scanf("%d", &freq[a]);
	}
	int code_length;
	scanf("%d", &code_length);
	char *c = malloc(sizeof(char)*code_length);
	int zero_one;
	int count_for_encoded=0;
	char bit;
	for (a=0; a<code_length; a++) {
		scanf("%d", &zero_one);
		if(zero_one == 1){
			bit = '1';
		}
		else
			bit = '0';
		c[a] = bit;
	}
	char *c2 = malloc(sizeof(char)*code_length);
	strncpy(c2, c, code_length); // copy to c2 to use later
	c2[code_length] = '\0';
	
	C = malloc(sizeof(nodes)*number_of_nodes);
	for (a=0; a<number_of_nodes; a++) {
		C[a]= create_node(freq[a], char_array[a]);  // create nodes and hold them C array
	}
	
	nodes n=HUFFMAN(C);  // call huffman function to build heap
	secret_message(Q[0], c, 0);	//  call secret_message function to decode message
	
	char *c3 = malloc(sizeof(char)*code_length);
	int b;
	int p;
	int flag2=0;
	for (a=0; a< code_length; a++) {  // find the secret message while searching on encoded bits
		for (b=0; b<number_of_nodes2; b++) {
			strncpy(c3,c2,a+1);
			c3[a+1] = '\0';
			if(strcmp(c3,code[char_array[b]])==0){
				printf("%c ",char_array[b]);
				flag2=1;
				break;
			}
		}
		if(flag2){
			for (p=0; p< code_length-a+1; p++) {
				c2[p] = c2[p+a+1];
			}
			a=0;
		}
		flag2=0;
	}	
    printf("\n");
}

void secret_message(nodes n, char *s, int len){
	
	static char *out = buf;	
		if (n->flag){ // if it is tied go on
			
			s[len] = 0;	
			strcpy(out, s);		
			code[n->name] = out; //copy '0' or '1' into the array code for each symbol
			out += len + 1;	
				
			return;	
		} 	
		s[len] = '0'; secret_message(n->left,  s, len + 1); //assign 0 to a right branch and do build_code in a left sub-tree recursively
		s[len] = '1'; secret_message(n->right, s, len + 1); //assign 1 to a right branch and do build_code in a right sub-tree recursively
}

nodes create_node(int freq, char chr){
	nodes n = (nodes)malloc(sizeof(nodes)); 
	n->freq = (int)malloc(sizeof(int));   // allocate space for each element of node
	n->flag = (int)malloc(sizeof(int)); 
	n->name = chr; n->freq = freq; n->flag=1;
	
	return n;
}

nodes HUFFMAN(nodes *C){
	int n = number_of_nodes;
	Q = malloc(sizeof(nodes)*number_of_nodes);
	Q = C;
	nodes x = malloc(sizeof(nodes));
	nodes y = malloc(sizeof(nodes));
	int a;
	int b;
	for (a=0; a<n-1; a++) {
		nodes z = malloc(sizeof(nodes));	
			
		x = EXTRACT_MIN(Q);  // extract first minimum node
		y = EXTRACT_MIN(Q);  // extract second minumum node
		// tie them to z node
		z->left = x;
		z->right = y;
		z->freq = x->freq + y->freq;
		z->name = x->name;
		z->flag = 0;
		INSERT(Q,z);
		
	}
	return EXTRACT_MIN(Q);
}
// insert tied node to all nodes
void INSERT(nodes *Q, nodes z){
	int j,i;
	i = number_of_nodes;
	number_of_nodes++;
	while ((j = i/2)) {
		if(Q[j]->freq <= z->freq ) break;
		Q[i] = Q[j]; i=j;
	}
	Q[i] = z;
}

nodes EXTRACT_MIN(nodes *Q){	// extract min node
	
	if(number_of_nodes == 1) return Q[0];
	int index=0;
	nodes n = malloc(sizeof(nodes));
	int a;
	
	n = Q[0];
	number_of_nodes--;
	// find the min frequency
	for (a=1; a<=number_of_nodes ;a++) {
		if(Q[a]->freq < n->freq){
			n = Q[a];
			index = a;
		}
	}
	// check if there is any same freq. if so,  find the minimum ascii one
	for (a=0; a<=number_of_nodes; a++) {
		int min_char = n->name;
		if((Q[a]->name < min_char) && (Q[a]->freq == n->freq)){
			n = Q[a];
			index = a;
		}
	}
	// reassign to correct position to extract this min node. 
	for (a=index; a<=number_of_nodes; a++) {
		Q[a] = Q[a+1];
	}
	
	return n; 
}
