// A program to find k most frequent words in a file 
#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 
#include<stdlib.h>
#include<stdbool.h>
#include <time.h> //link with -lrt
#include <string.h>
#include <math.h>

# define MAX_CHARS 256
# define MAX_WORD_SIZE 30 

struct bow
{
    int** matrix;
    int nrows,ncols;
};

static void print2d(int**x,int m,int n){
  int i,j;
  for(i=0;i<m;i++){
	for(j=0;j<n;j++)
	  printf("%d\t", x[i][j]);
	printf("\n");
  }
}

static void printcsr(int**csr, int m){
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
static int** tosparse(int **x, int m, int n){

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
  
  /*for(i=0;i<m;i++)
	free(x[i]);
  free(x);*/
  
  //printcsr(csr,m);
  return csr;
}

static struct bow* fromsparse( int **csr, int m, int n){
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
  
  //free(csr[0]);
  //free(csr[1]);
  //free(csr[2]);
  //free(csr);
    struct bow *features=(struct bow*)malloc(sizeof(struct bow));
    (*features).matrix=(int**)malloc(sizeof(int*)*m);
    for(int i=0;i<m;i++)
    {
       (*features).matrix[i]=(int*)malloc(sizeof(int*)*n); 
       for(int j=0;j<n;j++)
       { 
           (*features).matrix[i][j]=mat[i][j];

       }
    }
     (*features).nrows=m;
     (*features).ncols=n;
     return features;

}

static void freeMat(void** a, int m, int n){
  int i;
  for(i=0;i<m;i++);
	//free(a[i]);
  //free(a);
}

static double** tf_idf(int **v, int n, int len){
  int i,j,c;

  //print2d(v,n,len);
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
  
  

  double **ret=(double **)malloc(n*sizeof(double*));
  for(i=0;i<n;i++)
	ret[i]=(double *)malloc(len*sizeof(double));
  for(i=0;i<n;i++){
	for(j=0;j<len;j++){
	  ret[i][j]=tf[i][j]*itf[j];
	}
  }


  /*for(i=0;i<n;i++){
	for(j=0;j<len;j++)
	  printf("%f ", ret[i][j]);
	printf("\n");}
  //}*/
  return ret;
}

  struct suffix 
{ 
    int index; 
    int rank[2];

}; 

static int cmp(const void* a,const void* b) 
{ 
    return (((struct suffix*)a)->rank[0] == ((struct suffix*)b)->rank[0])? (((struct suffix*)a)->rank[1] < ((struct suffix*)b)->rank[1] ?-1: 1): (((struct suffix*)a)->rank[0] < ((struct suffix*)b)->rank[0] ?-1: 1); 
} 

static int *buildSuffixArray(char *txt, int n) 
{ 
    struct suffix suffixes[n]; 
    for (int i = 0; i < n; i++) 
    { 
        suffixes[i].index = i; 
        suffixes[i].rank[0] = txt[i] - 'a'; 
        suffixes[i].rank[1] = ((i+1) < n)? (txt[i + 1] - 'a'): -1; 
    } 
    qsort((void*)suffixes, n, sizeof(suffixes[0]), cmp); 
    int ind[n];  
    for (int k = 4; k < 2*n; k = k*2) 
    { 
        int rank = 0; 
        int prev_rank = suffixes[0].rank[0]; 
        suffixes[0].rank[0] = rank; 
        ind[suffixes[0].index] = 0; 
        for (int i = 1; i < n; i++) 
        {  
            if (suffixes[i].rank[0] == prev_rank && 
                    suffixes[i].rank[1] == suffixes[i-1].rank[1]) 
            { 
                prev_rank = suffixes[i].rank[0]; 
                suffixes[i].rank[0] = rank; 
            } 
            else 
            { 
                prev_rank = suffixes[i].rank[0]; 
                suffixes[i].rank[0] = ++rank; 
            } 
            ind[suffixes[i].index] = i; 
        }   
        for (int i = 0; i < n; i++) 
        { 
            int nextindex = suffixes[i].index + k/2; 
            suffixes[i].rank[1] = (nextindex < n)? 
                                  suffixes[ind[nextindex]].rank[0]: -1; 
        } 
    qsort((void*)suffixes, n, sizeof(suffixes[0]), cmp); 

    } 
    int *suffixArr =(int*)malloc(sizeof(int)*n); 
    for (int i = 0; i < n; i++) 
        suffixArr[i] = suffixes[i].index; 
    return  suffixArr; 
} 
  
static int search_bow(char *pat, char *txt, int *suffArr, int n) 
{ 
    int m = strlen(pat);  // get length of pattern, needed for strncmp() 
  
    // Do simple binary search_bow for the pat in txt using the 
    // built suffix array 
    int l = 0, r = n-1;  // Initilize left and right indexes 
    while (l <= r) 
    { 
        // See if 'pat' is prefix of middle suffix in suffix array 
        int mid = l + (r - l)/2; 
        int res = strncmp(pat, txt+suffArr[mid], m); 
  
        // If match found at the middle, print it and return 
        if (res == 0) 
        { 
            return 1; 
        } 
  
        // Move to left half if pattern is alphabtically less than 
        // the mid suffix 
        if (res < 0) r = mid - 1; 
  
        // Otherwise move to right half 
        else l = mid + 1; 
    } 
  
    // We reach here if return statement in loop is not executed 
    return 0; 
}

static int first(char *pat, char *txt, int *suffArr, int n,int *pos,int *k, int low,int high) 
{ 
  if(high >= low) 
  { 
    int mid = (low + high)/2; 
    int m=strlen(pat);
    int res = strncmp(pat, txt+suffArr[mid], m); 
    int res1 = strncmp(pat, txt+suffArr[mid-1], m); 
 
    if( ( mid == 0 || res1>0) && res==0) 
      return mid; 
    else if(res>0) 
      return first(pat, txt, suffArr, n, pos, k,(mid + 1), high); 
    else
      return first(pat, txt, suffArr, n, pos, k, low, (mid -1)); 
  } 
  return -1; 
} 
  

static int last(char *pat, char *txt, int *suffArr, int n,int *pos,int *k,int low,int high) 
{ 
  if (high >= low) 
  { 
  	int m=strlen(pat);
    int mid = (low + high)/2; 
    int res = strncmp(pat, txt+suffArr[mid], m); 
    int res1 = strncmp(pat, txt+suffArr[mid+1], m); 
    if( ( mid == n-1 || res1<0) && res==0 ) 
      return mid; 
    else if(res<0) 
      return last(pat, txt, suffArr, n, pos, k, low, (mid -1)); 
    else
      return last(pat, txt, suffArr, n, pos, k, (mid + 1), high);       
  } 
  return -1; 
} 
  

static void find_occurences(char *pat, char *txt, int *suffArr, int n,int *pos,int *k) 
{ 
  int i; 
  int j; 
  i = first(pat, txt, suffArr, n, pos, k,0, n-1); 
  if(i == -1) 
    return; 
  j = last(pat, txt, suffArr, n, pos, k,i, n-1);  
  for(int p=i;p<=j;p++)
  {
  	pos[(*k)++]=suffArr[p];
  }    
} 
// A Trie node 
typedef struct TrieNodebow 
{ 
    bool isEnd; // indicates end of word 
    unsigned frequency;  // the number of occurrences of a word 
    int indexMinHeap; // the index of the word in minHeap 
    struct TrieNodebow* child[MAX_CHARS]; // represents 26 slots each for 'a' to 'z'. 
}TrieNodebow; 
  
// A Min Heap node 
typedef struct MinHeapNode 
{ 
    TrieNodebow* root; // indicates the leaf node of TRIE 
    unsigned frequency; //  number of occurrences 
    char* word; // the actual word stored 
}MinHeapNode; 
  
// A Min Heap 
typedef struct MinHeap 
{ 
    unsigned capacity; // the total size a min heap 
    int count; // indicates the number of slots filled. 
    MinHeapNode* array; //  represents the collection of minHeapNodes 
}MinHeap; 
  
// A utility function to create a new Trie node 
static TrieNodebow* newTrieNode() 
{ 
    // Allocate memory for Trie Node 
    TrieNodebow* trieNode = (TrieNodebow*)malloc(sizeof(TrieNodebow)); 
  
    // Initialize values for new node 
    trieNode->isEnd = 0; 
    trieNode->frequency = 0; 
    trieNode->indexMinHeap = -1; 
    for( int i = 0; i < MAX_CHARS; ++i ) 
        trieNode->child[i] = NULL; 
  
    return trieNode; 
} 
  
// A utility function to create a Min Heap of given capacity 
static MinHeap* createMinHeap( int capacity ) 
{ 
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap)); 
  
    minHeap->capacity = capacity; 
    minHeap->count  = 0; 
  
    // Allocate memory for array of min heap nodes 
    minHeap->array = (MinHeapNode*)malloc(sizeof(MinHeapNode)*minHeap->capacity); 
  
    return minHeap; 
} 
  
// A utility function to swap two min heap nodes. This function 
// is needed in minHeapify 
static void swapMinHeapNodes ( MinHeapNode* a, MinHeapNode* b ) 
{ 
    MinHeapNode temp = *a; 
    *a = *b; 
    *b = temp; 
} 
  
// This is the standard minHeapify function. It does one thing extra. 
// It updates the minHapIndex in Trie when two nodes are swapped in 
// in min heap 
static void minHeapify( MinHeap* minHeap, int idx ) 
{ 
    int left, right, smallest; 
  
    left = 2 * idx + 1; 
    right = 2 * idx + 2; 
    smallest = idx; 
    if ( left < minHeap->count && 
         minHeap->array[ left ]. frequency < 
         minHeap->array[ smallest ]. frequency 
       ) 
        smallest = left; 
  
    if ( right < minHeap->count && 
         minHeap->array[ right ]. frequency < 
         minHeap->array[ smallest ]. frequency 
       ) 
        smallest = right; 
  
    if( smallest != idx ) 
    { 
        // Update the corresponding index in Trie node. 
        minHeap->array[ smallest ]. root->indexMinHeap = idx; 
        minHeap->array[ idx ]. root->indexMinHeap = smallest; 
  
        // Swap nodes in min heap 
        swapMinHeapNodes (&minHeap->array[ smallest ], &minHeap->array[ idx ]); 
  
        minHeapify( minHeap, smallest ); 
    } 
} 
  
// A standard function to build a heap 
static void buildMinHeap( MinHeap* minHeap ) 
{ 
    int n, i; 
    n = minHeap->count - 1; 
  
    for( i = ( n - 1 ) / 2; i >= 0; --i ) 
        minHeapify( minHeap, i ); 
} 
  
// Inserts a word to heap, the function handles the 3 cases explained above 
static void insertInMinHeap( MinHeap* minHeap, TrieNodebow** root, const char* word ) 
{ 
    // Case 1: the word is already present in minHeap 
    if( (*root)->indexMinHeap != -1 ) 
    { 
        ++( minHeap->array[ (*root)->indexMinHeap ]. frequency ); 
  
        // percolate down 
        minHeapify( minHeap, (*root)->indexMinHeap ); 
    } 
  
    // Case 2: Word is not present and heap is not full 
    else if( minHeap->count < minHeap->capacity ) 
    { 
        int count = minHeap->count; 
        minHeap->array[ count ]. frequency = (*root)->frequency; 
        minHeap->array[ count ]. word = (char*)malloc(sizeof(char)*(strlen(word)+1)); 
        strcpy( minHeap->array[ count ]. word, word ); 
  
        minHeap->array[ count ]. root = *root; 
        (*root)->indexMinHeap = minHeap->count; 
  
        ++( minHeap->count ); 
        buildMinHeap( minHeap ); 
    } 
  
    // Case 3: Word is not present and heap is full. And frequency of word 
    // is more than root. The root is the least frequent word in heap, 
    // replace root with new word 
    else if ( (*root)->frequency > minHeap->array[0]. frequency ) 
    { 
  
        minHeap->array[ 0 ]. root->indexMinHeap = -1; 
        minHeap->array[ 0 ]. root = *root; 
        minHeap->array[ 0 ]. root->indexMinHeap = 0; 
        minHeap->array[ 0 ]. frequency = (*root)->frequency; 
  
        // delete previously allocated memoory and 
        free(minHeap->array[ 0 ]. word); 
        minHeap->array[ 0 ]. word = (char*)malloc(sizeof(char)*(strlen(word)+1));  
        strcpy( minHeap->array[ 0 ]. word, word ); 
  
        minHeapify ( minHeap, 0 ); 
    } 
} 
  
// Inserts a new word to both Trie and Heap 
static void insertUtil ( TrieNodebow** root, MinHeap* minHeap, 
                        const char* word, const char* dupWord ) 
{ 
    // Base Case 
    if ( *root == NULL ) 
        *root = newTrieNode(); 
  
    //  There are still more characters in word 
    if ( *word != '\0' ) 
        insertUtil ( &((*root)->child[ tolower( *word ) - 97 ]), 
                         minHeap, word + 1, dupWord ); 
    else // The complete word is processed 
    { 
        // word is already present, increase the frequency 
        if ( (*root)->isEnd ) 
            ++( (*root)->frequency ); 
        else
        { 
            (*root)->isEnd = 1; 
            (*root)->frequency = 1; 
        } 
  
        // Insert in min heap also 
        insertInMinHeap( minHeap, root, dupWord ); 
     
      }
} 
  
  
// add a word to Trie & min heap.  A wrapper over the insertUtil 
static void insertTrieAndHeap(const char *word, TrieNodebow** root, MinHeap* minHeap) 
{ 
    insertUtil( root, minHeap, word, word ); 
} 
  
// A utility function to show results, The min heap 
// contains k most frequent words so far, at any time 
static void displayMinHeap( MinHeap* minHeap,char** vocabulary, int* vocabulary_size ) 
{ 
    int i; 
 
    // print top K word with frequency 
    for( i = 0; i < minHeap->count; ++i ) 
    { 
        //printf( "%s : %d\n", minHeap->array[i].word,minHeap->array[i].frequency ); 
        strcpy(vocabulary[(*vocabulary_size)++],minHeap->array[i].word);
    } 
} 
  
// The main funtion that takes a file as input, add words to heap 
// and Trie, finally shows result from heap 
static void printKMostFreq( FILE* fp, int k, char** vocabulary, int* vocabulary_size ) 
{ 
    // Create a Min Heap of Size k 
    MinHeap* minHeap = createMinHeap( k ); 
     
    // Create an empty Trie 
    TrieNodebow* root = NULL; 
  
    // A buffer to store one word at a time 
    char buffer[MAX_WORD_SIZE]; 
  
    // Read words one by one from file.  Insert the word in Trie and Min Heap 
    while( fscanf( fp, "%s", buffer ) != EOF ) 
        insertTrieAndHeap(buffer, &root, minHeap); 
  
    // The Min Heap will have the k most frequent words, so print Min Heap nodes 
    displayMinHeap( minHeap,vocabulary,vocabulary_size ); 
} 
// Driver program to test above functions 

static int** compute_features(int k,const char* train,int isbinary) 
{ 

    char* vocabulary[100000];int vocab_size=0;
    for(int i=0;i<100000;i++)
    {
        vocabulary[i]=(char*)malloc(sizeof(char)*1000);
    }

    FILE *fp = fopen(train, "r"); 
    if (fp == NULL){ 
        printf ("File doesn't exist "); return 0;}
    else{

        printKMostFreq (fp,k,vocabulary,&vocab_size); }
     
FILE *fp1=fopen(train,"r");
    char* buffer=(char*)malloc(sizeof(char)*1000000);
    int ndocs;
    while(!feof(fp1))
    {
    	fgets(buffer,1000000,fp1);ndocs++;
    }
    ndocs--;
    int doc=0;
    int* feature_matrix[ndocs];
    FILE *fp2=fopen(train,"r");
    while(!feof(fp2))
    {

        feature_matrix[doc]=(int*)malloc(sizeof(int)*vocab_size);

    	fgets(buffer,100000,fp2);
    	int* suffArray=buildSuffixArray(buffer,strlen(buffer));
    	int len=strlen(buffer); 
    	for(int i=0;i<vocab_size;i++)
    	{
    		if(isbinary==0)
    		{
    			int flag=search_bow(vocabulary[i],buffer,suffArray,len);
                feature_matrix[doc][i]=flag>0?1:0;
            }
            else
            {
            	int pos[100000],k=0;
                find_occurences(vocabulary[i],buffer,suffArray,len,pos,&k);
                feature_matrix[doc][i]=k;
            }
        }
        doc++;


    }
    /*struct bow *features=(struct bow*)malloc(sizeof(struct bow));
    (*features).matrix=(int**)malloc(sizeof(int*)*ndocs);
    for(int i=0;i<ndocs;i++)
    {
       (*features).matrix[i]=(int*)malloc(sizeof(int*)*vocab_size); 
       for(int j=0;j<vocab_size;j++)
       { 
           printf("%d ",feature_matrix[i][j]);
           (*features).matrix[i][j]=feature_matrix[i][j];

       }
    }
     (*features).nrows=ndocs;
     (*features).ncols=vocab_size;
     return features;*/
     return tosparse(feature_matrix,ndocs,vocab_size);
}    
