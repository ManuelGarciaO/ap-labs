#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define MAXLINES 5000
#define BUF_SIZE 1
char *lineptr[MAXLINES];

int readlines(char *input);
void writeLines(char *output, int lines);
int mystrlen(char *str);
char *mystradd(char *origin, char *addition);
int cmp(char *s1, char *s2,int type);
void mergeSort(int l, int r,int type);
void merge(int l, int m, int r, int type);

void mergeSort(int l, int r,int type){
    if(l<r){
        int m = l + (r - l) / 2;
        mergeSort(l,m,type);
        mergeSort(m+1,r,type);
        merge(l,m,r,type);
    }
}

void merge(int l, int m, int r, int type){
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 = r - m; 

    char *L[n1], *R[n2]; 
  
    for (i = 0; i < n1; i++) 
        L[i] = lineptr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = lineptr[m + 1 + j];

    i = 0; 
    j = 0;  
    k = l; 
    while (i < n1 && j < n2) { 
        if (cmp(L[i],R[j],type) == -1 || cmp(L[i],R[j],type) == 0) { 
            lineptr[k] = L[i]; 
            i++; 
        } 
        else { 
            lineptr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
    while (i < n1) { 
        lineptr[k] = L[i]; 
        i++; 
        k++; 
    } 

    while (j < n2) { 
        lineptr[k] = R[j]; 
        j++; 
        k++; 
    } 
}

int main(int argc, char **argv)
{
    int lines;
    int numeric=0;
    if(argc>1){
        if(strcmp(argv[1], "-n")==0 && argc == 3 ){
            numeric = 1;
            if((lines = readlines(argv[2]))>=0){
                mergeSort(0,lines-1,numeric);
                writeLines(argv[2],lines);
            }
            else{
                printf("Te input can not be sorted\n");
                return 0;
            }
        }else if(argc == 2){
            if(strcmp(argv[1], "-n")==0){
                printf("Error. Incomplete parameters\n");
                return 0;
            }
            if((lines = readlines(argv[1]))>=0){
                mergeSort(0,lines-1,numeric);
                writeLines(argv[1],lines);
            }
            else{
                printf("Te input can not be sorted\n");
                return 0;
            }
        }
    }
    else{
        printf("Error. Missing parameters\n");
        return 0;
    }
    
    return 0;
}

int readlines(char *input){
    int inputFd, c=0;
    ssize_t numRead;
    char buf[BUF_SIZE];
    inputFd = open(input, O_RDONLY);
    if(inputFd == -1)
    {
        printf("error opening file %s to sort\n", input);
        return 1;
    }
    char *newString="";
    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
    {
        if(strcmp(buf,"\n") != 0){
            newString=mystradd(newString,buf);
        }
        if(strcmp(buf,"\n") == 0){
            if(mystrlen(newString)>0){
                lineptr[c]=newString;
                c++;
                newString="";
            }   
        }
    }
    printf("new string %s\n", newString);
    lineptr[c]=newString;
    c++;
    newString="";
    if (close(inputFd) == -1)
        printf("error closing the fle input\n");
    return c;
}

void writeLines(char *output, int lines){
    int outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];
    char report[1000];
    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; 
    outputFd = open(output, openFlags, filePerms);
    if (outputFd == -1)
        printf("error opening file %s\n", output);

    for (int i = 0;i < lines-1; i++){
        sprintf(report, "%s\n",lineptr[i]);
        numRead=mystrlen(report);
        if (write(outputFd, report, numRead) < 0 ){
                printf("write returned an error\n");
        } 
    }
    sprintf(report, "%s",lineptr[lines-1]);
    numRead=mystrlen(report);
    if (write(outputFd, report, numRead) < 0 ){
            printf("write returned an error\n");
    }

    if (close(outputFd) == -1)
        printf("error closing the fle output\n");
}

char *mystradd(char *origin, char *addition){
    int newSize=mystrlen(origin)+mystrlen(addition);
    char *newString=malloc(newSize + 1);
    int aux=0;
    for(int i=0;origin[i]!='\0';i++,aux++){
        newString[aux]=origin[i];
    }
    for(int i=0;addition[i]!='\0';i++,aux++){
        newString[aux]=addition[i];
    }
    newString[newSize]='\0';
    return newString;
}

int mystrlen(char *str){
    int i=0;
    while(str[i]!='\0'){
        i++;
    }
    return i;
}

int cmp(char *s1, char *s2,int type){
    if(type==0){
        if(strcmp(s1,s2)>0)
            return 1;
        else if(strcmp(s1,s2)<0)
            return -1;
        else
            return 0;
    }
    else if(type==1){
        double v1,v2;
        v1 = atof(s1);
        v2 = atof(s2);
        if(v1<v2)
            return -1;
        else if(v1>v2)
            return 1;
        else 
            return 0;
    }
}