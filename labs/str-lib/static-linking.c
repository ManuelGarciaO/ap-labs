#include <stdio.h>
#include <string.h>

int mystrlen(char *str);
char *mystradd(char *origin, char *addition);
int mystrfind(char *origin, char *substr);

int main(int argc, char**argv) {
    if(argc==4){
        if(!strcmp(argv[1],"-add")){
            int iniLength=0,newLength=0;
            iniLength=mystrlen(argv[2]);
            char *newString=mystradd(argv[2],argv[3]);
            newLength=mystrlen(newString);
            printf("Initial Lenght      : %d\nNew String          : %s\nNew length          : %d\n",iniLength,newString,newLength);
        }
        else if(!strcmp(argv[1],"-find")){
            int pos;
            pos=mystrfind(argv[2],argv[3]);
            if(pos!=-1){
                printf("['%s'] string was found at [%d] position\n",argv[3],pos);
            }
            else{
                printf("['%s'] string was not found\n",argv[3]);
            }
        }
        else{
            printf("Invalid argument\n");
        }
    }
    else{
        printf("Invalid number of arguments\n");
    }
    return 0;
}
