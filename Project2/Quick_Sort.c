#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 100000
int data[MAXSIZE];
int n;
void QUICKSORT(int data[], int p, int length);
int PARTITION(int data[], int p, int length);
int RANDOMIZED_PARTITION(int data[], int p, int length);
int RANDOM(int p, int length);
int HOARE_PARTITION(int data[], int p, int length);
int main()
{
  int i;
  scanf("%d", &n); /* to specify how many number user will enter */
  for (i = 0; i < n; ++i)
    scanf("%d", &data[i]);  /* takes number from user */

  QUICKSORT(data, 0, n-1);

  for (i = 0; i < n; ++i)
    printf("%d\n", data[i]);   /* print out numbers that are implemented insertion sort */
}

void QUICKSORT(int data[], int p, int length){
  int q;
  if(p < length){
    q = RANDOMIZED_PARTITION(data, p, length);
//      q = HOARE_PARTITION(data, p, length);   // hoare partition without randomized partition
    QUICKSORT(data, p, q);
    QUICKSORT(data, q+1, length);
  }
}

int HOARE_PARTITION(int data[], int p, int length){
  int x = data[p];  // assign first element as a pivot
  int i = p-1;
  int j = length+1; // length of array
  while(1){
    do{  
      j--;
    }while(data[j]>x); // search form last element and find the first bigger element than pivot
    do{
      i++;
    }while(data[i]<x); // search from beginning of array and the first smaller element than pivot

    if(i<j){
      int temp = data[i]; data[i] = data[j]; data[j] = temp;  // swap the elements
    }
    else
	
      return j;
    }
}

int RANDOM(int p, int length){  //  returns random variable between first and last elements in array

  return rand() % (length-p);  
}

int RANDOMIZED_PARTITION(int data[], int p, int length){   // specify the random pivot and call Hoare_partition function with new pivot

  int a = RANDOM(p, length);
  int temp = data[p];
  data[p] = data[a+p];
  data[a+p] = temp;

  return HOARE_PARTITION(data, p, length);
}
