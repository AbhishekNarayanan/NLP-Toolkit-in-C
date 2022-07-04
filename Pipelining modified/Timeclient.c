#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "Fileclean.h"
#include "Stem.h"
#include "stopwords_trie.h"
#include "tokenizer.h"

#define FREQ 2.60e9

unsigned long long rdtsc()
{
  unsigned long long int x;
  unsigned a, d;

  __asm__ volatile("rdtsc" : "=a" (a), "=d" (d));

  return ((unsigned long long)a) | (((unsigned long long)d) << 32);
}

int main(int argc,char *argv[])
{
    unsigned long long t0,t1;
    char**tokenized_line;
    t0=rdtsc();
    read(argv[1]);
    t1=rdtsc();
    double time1=(t1-t0)/FREQ;
    printf("File read and cleaned\n");
    FILE*fp,*fp2,*fp3;
    fp=fopen(argv[1],"r");
    fp2=fopen("Afterprocess.txt","w");
    char*str;
    str=(char*)malloc(sizeof(char)*100000);
    double time2=0;
    double time3=0;
    double time4=0;
    t0=rdtsc();
    int c=remove_stop_words(argv[1]);
    t1=rdtsc();
    time2+=(t1-t0)/FREQ;
    fp3=fopen("Stemmed.txt","r");
    while(!feof(fp3))
    {
        fgets(str,100000,fp3);
        //printf("%s",str);
        t0=rdtsc();
        tokenized_line=tokenize_line(str);

        t1=rdtsc();
        time3+=(t1-t0)/FREQ;
        for(int i=0;tokenized_line[i]!=NULL;i++)
        {
            //printf("%s ",tokenized_line[i]);
            t0=rdtsc();
            tokenized_line[i]=Stem(tokenized_line[i],0,strlen(tokenized_line[i])-1);
            t1=rdtsc();
            time4+=(t1-t0)/FREQ;
            // printf("%s ",tokenized_line[i]);
            fprintf(fp2,"%s ",tokenized_line[i]);
        }
        fprintf(fp2,"\n");
    }
    free(str);
    fclose(fp);
    fclose(fp2);
    fclose(fp3);
    printf("Time taken for File cleaning: %lf sec\n",time1);
    printf("Time taken for Stop word removal: %lf sec\n",time2);
    printf("Time taken for Tokenising: %lf sec\n",time3);
    printf("Time taken for Stemming: %lf sec\n",time4);


    return 0;
}
