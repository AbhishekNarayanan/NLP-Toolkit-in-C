#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "Fileclean.h"
#include "Stem.h"
#include "stopwords_trie.h"
#include "tokenizer.h"

#include "feature_extraction.h"
#include "similarity.h"

double time_elapsed(struct timespec *start, struct timespec *end) {
	double t;
	t = (end->tv_sec - start->tv_sec); 
	t += (end->tv_nsec - start->tv_nsec) * 0.000000001; //diff in nanoseconds
	return t;
}

int main(int argc,char *argv[])
{

    char**tokenized_line;
    struct timespec start1, end1,start2,end2;
    clock_gettime(CLOCK_REALTIME, &start1);
    double time1=0;
    double time2=0;
    double time3=0;
    double time4=0;
    double time5=0;
    double ftime=0;
    read(argv[1]);
    //printf("File read and cleaned\n");
    FILE*fp,*fp2,*fp3;
    fp=fopen(argv[1],"r");
    fp2=fopen("doc1_Afterprocess.txt","w");
    char*str;
    str=(char*)malloc(sizeof(char)*100000);
    clock_gettime(CLOCK_REALTIME, &start2);
    int c=remove_stop_words(argv[1]);
    clock_gettime(CLOCK_REALTIME, &end2);
    time1=time_elapsed(&start2,&end2);
    fp3=fopen("Stemmed.txt","r");
    while(!feof(fp3))
    {
        fgets(str,100000,fp3);
        //printf("%s",str);
        clock_gettime(CLOCK_REALTIME, &start2);
        tokenized_line=tokenize_line(str);
        clock_gettime(CLOCK_REALTIME, &end2);
        time2=time_elapsed(&start2,&end2);
        for(int i=0;tokenized_line[i]!=NULL;i++)
        {
            //printf("%s ",tokenized_line[i]);
            clock_gettime(CLOCK_REALTIME, &start2);
            tokenized_line[i]=Stem(tokenized_line[i],0,strlen(tokenized_line[i])-1);
            clock_gettime(CLOCK_REALTIME, &end2);
            time3=time_elapsed(&start2,&end2);
            // printf("%s ",tokenized_line[i]);
            fprintf(fp2,"%s ",tokenized_line[i]);
        }
        fprintf(fp2,"\n");
    }
    free(str);
    fclose(fp);
    fclose(fp2);
    fclose(fp3);

    fp=fopen(argv[2],"r");
    fp2=fopen("doc2_Afterprocess.txt","w");
    str=(char*)malloc(sizeof(char)*100000);
    c=remove_stop_words(argv[2]);
    fp3=fopen("Stemmed.txt","r");
    while(!feof(fp3))
    {
        fgets(str,100000,fp3);
        //printf("%s",str);
        tokenized_line=tokenize_line(str);
        for(int i=0;tokenized_line[i]!=NULL;i++)
        {
            //printf("%s ",tokenized_line[i]);
            tokenized_line[i]=Stem(tokenized_line[i],0,strlen(tokenized_line[i])-1);
            // printf("%s ",tokenized_line[i]);
            fprintf(fp2,"%s ",tokenized_line[i]);
        }
        fprintf(fp2,"\n");
    }
    free(str);
    fclose(fp);
    fclose(fp2);
    fclose(fp3);
    FILE *f1=fopen("doc1_Afterprocess.txt","r");
    FILE *f2=fopen("doc2_Afterprocess.txt","r");  
    char *str1=(char*)malloc(sizeof(char)*100000);
    char *str2=(char*)malloc(sizeof(char)*100000);
    fgets(str1,100000,f1);
    fgets(str2,100000,f2);
    FILE *f3=fopen("similaritycombine.txt","w");
    fputs(str1,f3);
    //fputs("\n",f3);
    fputs(str2,f3);
    fclose(f3); 
    clock_gettime(CLOCK_REALTIME, &start2);
    struct bow* bow1=fromsparse(compute_features(50,"similaritycombine.txt",1),2,50);
    clock_gettime(CLOCK_REALTIME, &end2);
    time4=time_elapsed(&start2,&end2);
    //printf("Jaccard Similarity : %lf %d %d\n",jaccard_similarity(bow1->matrix[0],bow2->matrix[0], 30),bow1->nrows,bow1->ncols);
   
    clock_gettime(CLOCK_REALTIME, &start2);
    double** tfidf_matrix1=tf_idf(bow1->matrix,bow1->nrows,bow1->ncols);
    clock_gettime(CLOCK_REALTIME, &end2);
    time5=time_elapsed(&start2,&end2);

    
    printf("Cosine Similarity : %lf\n",cosine_similarity(tfidf_matrix1[0],tfidf_matrix1[1], 50));
    
    printf("levenshtein Distance is %d\n", levenshteinDistance(str1,str2)); 
    printf("levenshtein Edits needed for strings \"Hello there\" and \"Helo der\"  are : %d\n", levenshteinEdits("Hello there","Helo der"));
    fclose(f1);
    fclose(f2); 
    printf("Time taken\n");
    printf("Time taken for removing stop words: %lf nsec\n",time1);        printf("Time taken for removing stop words: %lf sec\n",time1);
    printf("Time taken for tokenizing: %lf nsec\n",time2);
    printf("Time taken for stemming: %lf nsec\n",time3);
    printf("Time taken for vectorising: %lf nsec\n",time1);

    clock_gettime(CLOCK_REALTIME, &end1);
    ftime=time_elapsed(&start1,&end1);
    printf("Complete time taken for all tasks: %lf nsec\n",ftime);

    return 0;
}
