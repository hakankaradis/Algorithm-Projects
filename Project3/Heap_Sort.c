#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 100000

int data[MAXSIZE];
int n;
void Max_Heapify(int data[], int i);
void Build_Max_Heap(int data[]);
int Left(int i);
int Right(int i);
void HeapSort(int data[]);
int heap_size;

int main()
{
  int i;
  scanf("%d", &n); /* to specify how many number user will enter */
  for (i = 0; i < n; ++i)
    scanf("%d", &data[i]);  /* takes number from user */

 HeapSort(data);
 
  for (i = 0; i < n; ++i)
    printf("%d\n", data[i]);   /* print out numbers that are implemented insertion sort */
}

void HeapSort(int data[]){
	Build_Max_Heap(data); // aftr calling this function all nodes are sorted in each subtree
	int i;
	int length = n;
	for (i =heap_size; i>=1; i--) { // exchange last and first element to be sorted
		int temp = data[0]; data[0] = data[i]; data[i] = temp;
		heap_size--;
		Max_Heapify(data, 0);
	}
        // after this for loop all subtree will be sorted.
}

void Max_Heapify(int data[], int i){  // sort sub trees to decide parent  by exchanging leaf and root
  int l = Left(i);
  int r = Right(i);
  int largest;
  if(l <= heap_size && data[l] > data[i]){
    largest = l;
  }else{
    largest = i;
  }
  if(r <= heap_size  && data[r] > data[largest]){
    largest = r;
  }
  if(largest != i){ // decide biggest one and exchange with leaf
    int temp = data[i]; data[i] = data[largest]; data[largest] = temp;
    Max_Heapify(data, largest);
  }
}

void Build_Max_Heap(int data[]){  
  int length = n;
  heap_size = n-1;
  int i;
  for(i=length/2-1; i>=0; i--){   // goes through the remaing nodes of the tree
    Max_Heapify(data, i);  // runs Max_Heapify for each node
  }
}


int Left(int i){   // return left node

  return 2*i+1;  
}

int Right(int i){  // return right node
  
  return 2*i+2;
}


