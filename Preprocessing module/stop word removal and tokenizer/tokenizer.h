#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXTOKENS       256
#define MAXLINE         1024
#define MINLEN          3
#define STMINLEN        2
/* split string into tokens, return token array */
char **split(char *string, char *delim) {
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



char** tokenize_line(char* line) {
 /* delim does not include \' [\047] quote */ 
 char *delim = ".,:;`\"+-_(){}[]<>*&^%$#@!?~/|\\= \t\r\n1234567890";
 char **tokens = NULL;

 int i = 0;

  tokens = split(line, delim);
  
return tokens;
}

void tokenize_file(const char* filename) {
 /* delim does not include \' [\047] quote */ 
 char *delim = ".,:;`\"+-_(){}[]<>*&^%$#@!?~/|\\= \t\r\n1234567890";
 char **tokens = NULL;
 char line[MAXLINE];
 int i = 0;
 int nlines=0;

FILE *fp=fopen(filename,"r");
 while(fgets(line, MAXLINE, fp) != NULL) {

  tokens = split(line, delim);
  for(i = 0; tokens[i] != NULL; i++) {

printf("%s\n",tokens[i]);}
  nlines++;
}

}
