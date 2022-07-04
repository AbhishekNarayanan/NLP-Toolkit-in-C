#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sparse.h"
#include "bow.h"
//frees a 2d array with size m*n
void freeMat(void** a, int m, int n);

//finds the tf*itf value for each word in each of the text vectors
double** tf_idf(struct bow* bagofwords);

//printa 2d int array
void print2d(int**x,int m,int n);

void print2d(int**x,int m,int n){
  int i,j;
  for(i=0;i<m;i++){
	for(j=0;j<n;j++)
	  printf("%d\t", x[i][j]);
	printf("\n");
  }
}


void freeMat(void** a, int m, int n){
  int i;
  for(i=0;i<m;i++)
	free(a[i]);
  free(a);
}

double** tf_idf(struct bow* bagofwords){
  int i,j,c;
  int **v, int n, int len;
  v=bagofwords.matrix;
  n=bagofwords.n;
  len=bagofwords.len;
  print2d(v,n,len);
  double tf[n][len];
  for(i=0;i<n;i++){
	c=0;
	for(j=0;j<len;j++){
	  c+=v[i][j];
	}
	for(j=0;j<len;j++){
	  tf[i][j]=(float)v[i][j]/c;
	}
  }
  /*
  for(i=0;i<n;i++){
	for(j=0;j<len;j++)
	  printf("%f\t", tf[i][j]);
	printf("\n");
  }
  */
  double itf[len];
  for(j=0;j<len;j++){
	c=0;
	for(i=0;i<n;i++){
	  if(v[i][j]>0)
		c++;
	}
	if(c>0)
	  itf[j]=1+log((float)len/c);
	else
	  itf[j]=1;
  }
  /*
  for(j=0;j<len;j++)
	printf("%f\t", itf[j]);
  printf("\n");
  */
  double **ret=malloc(n*sizeof(int*));
  for(i=0;i<n;i++)
	ret[i]=malloc(len*sizeof(int));
  for(i=0;i<n;i++){
	for(j=0;j<len;j++){
	  ret[i][j]=tf[i][j]*itf[j];
	}
  }

  /*printf("\n");
  for(i=0;i<n;i++){
	for(j=0;j<len;j++)
	  printf("%f\t", ret[i][j]);
	printf("\n");
  }*/
  return ret;
}
