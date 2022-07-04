#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define min3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

//given 2 strings, they return the edit distance
int levenshteinEdits(char *a, char *b);//also prints the edits required to change string2 to string1
int levenshteinDistance(char *a, char *b);

//given the 2 vectors, it returns the cosine similarity between the two strings which are represented by the vectors
//takes the weight vectors as input
double cosine_similarity(double *A, double *B, int len);
//takes binary vectors(output of bag of words)
double jaccard_similarity(int *A, int *B, int len);



//min edit distance(3 options of editing: insert, remove or replace)
int levenshteinEdits(char *a, char *b) {
  int x, y, alen, blen, i, j;
  alen = strlen(a);
  blen = strlen(b);
  int matrix[blen+1][alen+1];
  matrix[0][0] = 0;
  for (x = 1; x <= blen; x++)
	matrix[x][0] = matrix[x-1][0] + 1;
  for (y = 1; y <= alen; y++)
	matrix[0][y] = matrix[0][y-1] + 1;
  for (x = 1; x <= blen; x++)
	for (y = 1; y <= alen; y++)
	  matrix[x][y] = min3(matrix[x-1][y] + 1, matrix[x][y-1] + 1, matrix[x-1][y-1] + (a[y-1] == b[x-1] ? 0 : 1));

  
  for(int i=0;i<blen+1;i++){
	for(int j=0;j<alen+1;j++){
	  printf("%d, ", matrix[i][j]);
	}
	printf("\n");
  }
  

  i=blen;
  j=alen;

  while(i!=0 && j!=0){
	//	printf("i=%d, j=%d\n", i,j);
	if(b[i-1]==a[j-1]){
	  i--;
	  j--;
	}
	//replace
	else if(matrix[i][j]==matrix[i-1][j-1]+1){
	  printf("change %c to %c in %s at position %d\n", b[i-1], a[j-1], b, i-1);
	  i--;
	  j--;
	}
	//delete
	else if(matrix[i][j]==matrix[i-1][j]+1){
	  printf("delete %c from %s at position %d\n", b[i-1], b, i-1);
	  i--;
	}
	//add
	else if(matrix[i][j]==matrix[i][j-1]+1){
	  printf("add %c to %s after position %d\n", a[j-1], b, i-1);
	  j--;
	}
  }

  while(i>0){
	printf("delete %c from %s at position %d\n", b[i-1], b, i-1);
	i--;
  }
  
  while(j>0){
	printf("add %c to %s after position %d\n", a[j-1], b, i);
	j--;
  }
  
  return(matrix[blen][alen]);
}

int levenshteinDistance(char *a, char *b) {
  int alen, blen, x, y, lastdiag, olddiag;
  alen = strlen(a);
  blen = strlen(b);
  int column[alen+1];
  for (y = 1; y <= alen; y++)
	column[y] = y;
  for (x = 1; x <= blen; x++) {
	column[0] = x;
	for (y = 1, lastdiag = x-1; y <= alen; y++) {
	  olddiag = column[y];
	  column[y] = min3(column[y] + 1, column[y-1] + 1, lastdiag + (a[y-1] == b[x-1] ? 0 : 1));
	  lastdiag = olddiag;
	}
  }
  return(column[alen]);
}


double cosine_similarity(double *A, double *B, int len){
    double dot = 0.0, denom_a = 0.0, denom_b = 0.0 ;
     for(int i = 0; i < len; i++) {
        dot += A[i] * B[i] ;
        denom_a += A[i] * A[i] ;
        denom_b += B[i] * B[i] ;
    }
    return dot / (sqrt(denom_a) * sqrt(denom_b)) ;
}

double jaccard_similarity(int *A, int *B, int len){
  int c=0;
  for(int i=0;i<len;i++){
	if(A[i]==1 && B[i]==1)
	  c++;
  }
  return (float)c/len;
}
