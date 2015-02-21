#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int flag=0;
int add(int *q, int i, int j);
int MEMOIZED_MATRIX_CHAIN(int *, int);
int LOOKUP_CHAIN(int **m, int *, int i, int j);

int main(int argc, char *argv[]) {
 if(argc != 2){
    printf("Please enter the input file name\n");   // check for user entered input file name or not
    return 0;
  }

  char *fileName = (char *)argv[1];  // take file name from user
  int *p;  // holds the numbers 
  int size; // holds the size of array
  int x=0;
  FILE *file = fopen(fileName, "r");  // open file
  if(file == NULL){
    printf("Unable to open that file\n"); 
    return (-1);
  }
  else{
    char line[60]; /* or other suitable maximum line size */
    while (fgets (line,sizeof line,file ) != NULL ){ /* read a line */
      if(flag == 0){
	flag=1;
	size = atoi(line); // take first element of input file
	p = malloc(sizeof(int) * size); // allocate space for array
      }
      else{
        p[x] = atoi(line);  // assign p array
	x++;
      }
    }
    fclose(file);
  }

  printf("cost is is %d\n", MEMOIZED_MATRIX_CHAIN(p,size)); 

  return 0;
}

int MEMOIZED_MATRIX_CHAIN(int *p,int n){
  int i,j,a;	
  int **m;
  m = malloc(sizeof(int*)*n);  // allocate space for 2 dimesional array m 
  for(a=0; a<n; a++){
    m[a] = malloc(sizeof(int)*n);
  }
  for (i=0; i<n; i++) {
    for (j=0; j<n; j++) {
	m[i][j] = INT_MAX;  // assign the maximum number for each element of array
    }
  }

  return LOOKUP_CHAIN(m,p,0,n-1);
}

int LOOKUP_CHAIN(int **m, int *p, int i, int j){
  if(m[i][j] < INT_MAX){  // check for the element of array has maximum number 
    return m[i][j];
  }
  int k;	
  if(i == j)
    m[i][j] = 0;
  else {
    for (k=i; k<j; k++) {
      int q = LOOKUP_CHAIN(m,p,i,k) + LOOKUP_CHAIN(m,p,k+1,j) + add(p,i,j);  // sum from i to j in add function
	if(q < m[i][j]){
	  m[i][j] = q;   // assign for the optimal way
	}
    }
  }

  return m[i][j]; // return the optimal way
}

int add(int *p, int i, int j){ // read from p array and add from i to j to sum	
  int sum = 0;
  int k=i;
  for(; k<=j; k++){
    sum += p[k];
  }

  return sum;
}
