#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROW 10001
#define COLUMN 1001
int count=0;
int c[ROW][COLUMN];
char common[ROW];
char b[ROW][COLUMN];
void LCS_LENGTH(char*, char*);
void PRINT_LCS(char b[][COLUMN], char *, int, int);

int main()
{
  char s1[10001];
  char s2[1001];
  scanf("%s %s",s1,s2);  // take strings from user
  int i = strlen(s1);  // holds the size of string1
  int j = strlen(s2); // holds the size of string2
  LCS_LENGTH(s1, s2);
  PRINT_LCS(b, s1, i, j);
  printf("%d\n", count);  // print out how many character same
  printf("%s\n",common);  // print out the common characters

  return 0;
}
void LCS_LENGTH(char *s1, char *s2){
  int m = strlen(s1);
  int n = strlen(s2);
  int i;
  for (i=1; i<=m; i++) {  // assign 0 the first column
    c[i][0] = 0;
  }
  int j;
  for (j=0; j<=n; j++) {c[0][j] = 0;} // assign 0 the first row
  // compare the character by character 
  // and if they are equal assign * that means character are sam
  // if not assign + or - that depends which elements of matrix is bigger
  for(i=1; i<=m; i++){
    for(j=1; j<=n; j++){
        if(s1[i-1] == s2[j-1]){ 
          c[i][j] = c[i-1][j-1] + 1;
          b[i][j] = '*';
        }
	else if (c[i-1][j] >= c[i][j-1]) {
	  c[i][j] = c[i-1][j];
	  b[i][j] = '+';
	}
	else {
	  c[i][j] = c[i][j-1];
	  b[i][j] = '-';
	}
    }
  }
}

void PRINT_LCS(char b[][COLUMN], char *X, int i, int j){
  if(i == 0 || j == 0)
    return;
  if(b[i][j] == '*'){  // print out the all *s and call function again
    PRINT_LCS(b, X, i-1, j-1);
    common[count] = X[i-1];
    count++; 
  }
  else if (b[i][j] == '+') {
    PRINT_LCS(b, X, i-1, j);
  }
  else {
    PRINT_LCS(b, X, i, j-1);
  }
}
