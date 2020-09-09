#include <stdlib.h>

int mystrlen(char *str){
    int i=0;
    while(str[i]!='\0'){
        i++;
    }
    return i;
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

int mystrfind(char *origin, char *substr){
    int aux=0, encontrado=1;
    int l=mystrlen(substr);
    for(int i=0;origin[i]!='\0';i++,aux++){
        aux=0;
        for(int j=i,k=0;origin[j]!='\0' && substr[k]!='\0';j++,k++){
            if(origin[j]!=substr[k]){
                break;
            }
            else{
                aux++;
            }
        }
        if(aux==l)
        return i;
    }
    return -1;
}
