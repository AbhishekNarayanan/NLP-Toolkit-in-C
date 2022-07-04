#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>


//Function to clean the user file
char* clean(char*str)
{
    int l=strlen(str);
    char*newt=(char*)malloc(sizeof(char)*100000);
    int k=0;
    int flg=0;
    for(int i=0;i<l;i++)
    {
        if(isalnum(str[i]))
        {
            flg=0;
            newt[k]=tolower(str[i]);
            k++;
        }
        else if(isspace(str[i]))
        {
            if(flg==0)
            {
                newt[k]=' ';
                k++;
                flg=1;
            }
        }
    }
    newt[k++]='\n';
    newt[k]='\0';
    return newt;
}

//Function to read user file and clean it
void read(char*str)
{
    FILE*fp1,*fp2;
    fp1=fopen(str,"r");
    fp2=fopen("Temp.txt","w");
    char*buf=(char*)malloc(sizeof(char)*100000);
    while(!feof(fp1))
    {
        fgets(buf,100000,fp1);
        buf=clean(buf);
        fputs(buf,fp2);
    }
    fclose(fp1);
    fclose(fp2);
    remove(str);
    rename("Temp.txt",str);
}

