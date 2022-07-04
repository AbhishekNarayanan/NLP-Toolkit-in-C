#include <stdio.h>
#include <stdlib.h>

void print2d(int**x,int m,int n);
void printcsr(int**csr, int m);
int** tosparse(int **x, int m, int n);//returns the sparse matrix and deallocates the original matrix
int** fromsparse( int **csr, int m, int n);//returns the original matrix and deallocates the sparse matrix
struct bow
{
    int** matrix;
    int nrows,ncols;
};
void print2d(int**x,int m,int n){
  int i,j;
  for(i=0;i<m;i++){
	for(j=0;j<n;j++)
	  printf("%d\t", x[i][j]);
	printf("\n");
  }
}

void printcsr(int**csr, int m){
  int i;
  for(i=0;i<csr[1][m];i++)
	printf("%d\t",csr[0][i]);
  printf("\n");
  
  for(i=0;i<m+1;i++)
	printf("%d\t",csr[1][i]);
  printf("\n");

  for(i=0;i<csr[1][m];i++)
	printf("%d\t",csr[2][i]);
  printf("\n");

}

//compressed sparse row
//saves memory only when number of non-zero entites < (m (n − 1) − 1)/2
//csr uses 3 arrays: one to store the non-zero numbers in row-major form, one to store the cumulative number of non-zero entites in that row, and one to store the column numbe of all the non-zero numbers
int** tosparse(int **x, int m, int n){

  int a[n*m];
  int ia[m+1];
  int ja[n*m];

  int i,j,c,k;
 
  k=0;
  ia[0]=0;
  c=0;
  for(i=0;i<m;i++){
	for(j=0;j<n;j++){
	  if(x[i][j]!=0){
		a[k]=x[i][j];
		ja[k]=j;
		k++;
		c++;
	  }
	}
	ia[i+1]=c;
  }
  
  int** csr=malloc(3*sizeof(int*));
  csr[0]=malloc(k*sizeof(int));
  csr[1]=malloc((m+1)*sizeof(int));
  csr[2]=malloc(k*sizeof(int));

  for(i=0;i<k;i++){
	csr[0][i]=a[i];
	csr[2][i]=ja[i];
	if(i<=m)
	  csr[1][i]=ia[i];
  }  
  
  for(i=0;i<m;i++)
	free(x[i]);
  free(x);
  
  //  printcsr(csr,m);
  return csr;
}

struct bow* fromsparse( int **csr, int m, int n){
  int i,j,c;
  //  printcsr(csr,m);
  int** mat= (int**)malloc(m*sizeof(int*));
  for(i=0;i<m;i++){
	mat[i]=(int*)malloc(n*sizeof(int));
	for(j=0;j<n;j++){
	  mat[i][j]=0;
	}
  }
  
  for(i=0;i<m;i++){
	c=csr[1][i+1]-1;
	while(c>=csr[1][i]){
	  mat[i][csr[2][c]]=csr[0][c];
	  c--;
	}
  }
  //  print2d(mat,m,n);
  struct bow *features=(struct bow*)malloc(sizeof(struct bow));
    (*features).matrix=(int**)malloc(sizeof(int*)*ndocs);
    for(int i=0;i<m;i++)
    {
       (*features).matrix[i]=(int*)malloc(sizeof(int*)*vocab_size); 
       for(int j=0;j<n;j++)
       { 
           (*features).matrix[i][j]=mat[i][j];

       }
    }
     (*features).nrows=m;
     (*features).ncols=n;
  free(csr[0]);
  free(csr[1]);
  free(csr[2]);
  free(csr);

  return mat;
}
