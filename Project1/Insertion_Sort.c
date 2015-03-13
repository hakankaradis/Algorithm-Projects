#include <stdio.h>

#define MAXSIZE 100000

int data[MAXSIZE];
int n;

int main()
{
  int i;

  scanf("%d", &n); /* to specify how many number user will enter */

  for (i = 0; i < n; ++i)
    scanf("%d", &data[i]);  /* takes number from user */

  i=1;
  int a,temp;
  for(;i<n;i++){ /* start from second element of array */
    a=i;
    while(a>0){ /* to compare all elements that are behind the current position */
      if(data[a] < data[a-1]){ /* compare and switch */
        temp = data[a];
        data[a] = data[a-1];
        data[a-1] = temp;
      }
      a--;
     }
  }

  for (i = 0; i < n; ++i)
    printf("%d\n", data[i]);   /* print out numbers that are implemented insertion sort */
}


