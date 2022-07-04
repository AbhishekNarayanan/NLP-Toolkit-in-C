// C implementation of search and insert operations 
// on Trie 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h> 
#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0]) 
// Alphabet size (# of symbols) 
#define ALPHABET_SIZE (256) 
  
// Converts key current character into index 
// use only 'a' through 'z' and lower case 
#define CHAR_TO_INDEX(c) ((int)c - (int)'a') 
 #define MAXTOKENS       1024
#define MAXLINE         1024
#define MINLEN          3
#define STMINLEN        2
// trie node 
struct TrieNode 
{ 
    struct TrieNode *children[ALPHABET_SIZE]; 
  
    // isEndOfWord is true if the node represents 
    // end of a word 
    bool isEndOfWord; 
}; 
  
// Returns new trie node (initialized to NULLs) 
struct TrieNode *getNode(void) 
{ 
    struct TrieNode *pNode = NULL; 
  
    pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode)); 
  
    if (pNode) 
    { 
        int i; 
  
        pNode->isEndOfWord = false; 
  
        for (i = 0; i < ALPHABET_SIZE; i++) 
            pNode->children[i] = NULL; 
    } 
  
    return pNode; 
} 
  
// If not present, inserts key into trie 
// If the key is prefix of trie node, just marks leaf node 
static void insert(struct TrieNode *root, const char *key) 
{ 
    int level; 
    int length = strlen(key); 
    int index; 
  
    struct TrieNode *pCrawl = root; 
  
    for (level = 0; level < length; level++) 
    { 
        index = (int)key[level]; 
        if (!pCrawl->children[index]) 
            pCrawl->children[index] = getNode(); 
  
        pCrawl = pCrawl->children[index]; 
    } 
  
    // mark last node as leaf 
    pCrawl->isEndOfWord = true; 
} 
  
// Returns true if key presents in trie, else false 
static bool search(struct TrieNode *root, const char *key) 
{ 
    int level; 
    int length = strlen(key); 
    int index; 
    struct TrieNode *pCrawl = root; 
  
    for (level = 0; level < length; level++) 
    { 
        index = (int)(key[level]); 
  
        if (!pCrawl->children[index]) 
            return false; 
  
        pCrawl = pCrawl->children[index]; 
    } 
  
    return (pCrawl != NULL && pCrawl->isEndOfWord); 
} 
  
static struct TrieNode *buildstoptrie(char *fname, struct TrieNode *p) {
 FILE *fp = {0};
 char line[MAXLINE];
 int len = 0, lcount = 0;

 fp = fopen(fname, "r");
 if(fp == NULL) {
  fprintf(stderr, "Error - fopen(%s)\n", fname);
  return NULL;
 }

 while(fgets(line, MAXLINE, fp) != NULL) {
  len = strlen(line);
  if(len < STMINLEN)
   continue;
  else
   lcount++;

  if(line[len - 1] == '\n')
   line[--len] = '\0';

  insert(p, line);
 }

 if(lcount == 0) {
  fprintf(stderr, "Error - Zero stopwords..\n");
  return NULL;
 }

 fclose(fp);
 return p;
}

/* split string into tokens, return token array */
static char **split(char *string, char *delim) {
 char **tokens = NULL;
 char *working = NULL;
 char *token = NULL;
 int idx = 0;

 tokens  = malloc(sizeof(char *) * MAXTOKENS);
 if(tokens == NULL)
  return NULL;
 working = malloc(sizeof(char) * strlen(string) + 1);
 if(working == NULL)
  return NULL;

 /* to make sure, copy string to a safe place */
 strcpy(working, string);
 for(idx = 0; idx < MAXTOKENS; idx++)
  tokens[idx] = NULL;

 token = strtok(working, delim);
 idx = 0;

 /* always keep the last entry NULL terminated */
 while((idx < (MAXTOKENS - 1)) && (token != NULL)) {
  tokens[idx] = malloc(sizeof(char) * strlen(token) + 1);
  if(tokens[idx] != NULL) {
   strcpy(tokens[idx], token);
   idx++;
   token = strtok(NULL, delim);
  }
 }

 free(working);
 return tokens;
}

static int remove_stop_words(char* input) {
 /* delim does not include \' [\047] quote */ 
 FILE*sp;
 sp=fopen("Stemmed.txt","w");
 char *delim = ".,:;`\"+-_(){}[]<>*&^%$#@!?~/|\\= \t\r\n1234567890";
 char **tokens = NULL;
 char* stopfile="stoplist.txt";
 bool query;
 char line[MAXLINE];
 int i = 0;

 struct TrieNode *root = getNode(); 
 root = buildstoptrie(stopfile, root);
 if(root == NULL)
  return 1;
 FILE *fp=fopen(input,"r");
 while(fgets(line, MAXLINE, fp) != NULL) {
  if(strlen(line) < MINLEN)
   continue;

  tokens = split(line, delim);
  for(i = 0; tokens[i] != NULL; i++) {
   query = search(root, tokens[i]);
   if(query == false)
    fprintf(sp,"%s ",tokens[i]);}
  //fprintf(sp,"\n");
  

  for(i = 0; tokens[i] != NULL; i++)
   free(tokens[i]);
  //free(tokens[i]);
  //printf("\n");
 }
  //fprintf(sp,"\n");
 fclose(sp);
 //freetrie(root);
 return 0;
}

