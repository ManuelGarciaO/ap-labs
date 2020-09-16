#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 
#include <stdlib.h>

#define MAX 1000000
#define BUF_SIZE 1

void analizeLog(char *logFile, char *report);
int processFile(char *input, int *installed, int *upgraded, int *removed);
void generateReport(char *output,int installed, int upgraded, int removed, int current);

int mystrlen(char *str);
char *mystradd(char *origin, char *addition);
int mystrfind(char *origin, char *substr);

char *getDate(char *line, int end);
char *getName(char *line, int index);

struct package {
   char  name[50];
   char  install_date[50];
   char  lupdate_date[50];
   char  removal_date[50];
   int  updates;
   int   package_id;
} arr_package[MAX];

int main(int argc, char **argv) {

    if (argc < 5) {
	    printf("Invalid number of arguments\n");
	    return 1;
    }
    analizeLog(argv[2], argv[4]);

    return 0;
}


void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    int installed_p=0,removed_p=0, upgraded_p=0, current_p;

    //read and process information
    if(processFile(logFile,&installed_p,&upgraded_p,&removed_p)==0){
        current_p=installed_p-removed_p;
        //write report
        generateReport(report, installed_p,upgraded_p,removed_p, current_p);
        printf("Report is generated at: [%s]\n", report);
    }
    else
        printf("program failed at processing input file %s\n",logFile);

    
}

int processFile(char *input, int *installed, int *upgraded, int *removed){
    int inputFd, openFlags, index, startAction;
    ssize_t numRead;
    char buf[BUF_SIZE];

    inputFd = open(input, O_RDONLY);
    if(inputFd == -1)
    {
        printf("error opening file %s\n", input);
        return 1;
    }
    char *newString="";
    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
    {
        newString=mystradd(newString,buf);
        //line readed, analize
        if(strcmp(buf,"\n") == 0 ){ 
            if((startAction = mystrfind(newString,"installed"))!=-1){
            int endDate = mystrfind(newString,"]");
                if(endDate+9==startAction){
                    strcpy(arr_package[*installed].name,getName(newString, startAction+10));
                    strcpy(arr_package[*installed].install_date,getDate(newString, endDate-1));
                    strcpy(arr_package[*installed].lupdate_date,"-");
                    strcpy(arr_package[*installed].removal_date,"-");
                    arr_package[*installed].updates=0;
                    *installed+=1;
                }                
            }
            else if((startAction = mystrfind(newString,"upgraded"))!=-1){
                int endDate = mystrfind(newString,"]");
                if(endDate+9==startAction){
                    
                    char *name = getName(newString, startAction+9);//get name
                    for(int i = 0;i < *installed;i++){//search by name
                        if(strcmp(arr_package[i].name,name) == 0){
                            strcpy(arr_package[i].lupdate_date,getDate(newString, endDate-1));
                            if(arr_package[i].updates==0)
                                *upgraded+=1;
                            arr_package[i].updates+=1;
                        }
                    }
                }           
            }
            else if((startAction = mystrfind(newString,"removed"))!=-1){
                int endDate = mystrfind(newString,"]");
                if(endDate+9==startAction){
                    char *name = getName(newString, startAction+8);//get name
                    for(int i = 0;i < *installed;i++){//search by name
                        if(strcmp(arr_package[i].name,name) == 0){
                            strcpy(arr_package[i].removal_date,getDate(newString, endDate-1));
                        }
                    }
                    *removed+=1;
                } 
            }            
            newString="";
        }
    }
    if (close(inputFd) == -1)
        printf("error closing the fle input\n");
    return 0;
}

void generateReport(char *output,int installed, int upgraded, int removed, int current){
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

    sprintf(report,
    "Pacman Packages Report\n"
    "----------------------\n"
    "- Installed packages : %d\n"
    "- Removed packages   : %d\n"
    "- Upgraded packages  : %d\n"
    "- Current installed  : %d\n\n"
    "List of packages\n"
    "----------------\n"
    ,installed,removed,upgraded,current);
    numRead=mystrlen(report);
    if (write(outputFd, report, numRead) < 0 ){
                printf("write returned an error\n");
    } 
    for(int i=0;i<installed;i++){
        sprintf(report,
        "- Package Name         : %s \n"
        "   - Install date      : %s\n"
        "   - Last update date  : %s\n"
        "   - How many updates  : %d\n"
        "   - Removal date      : %s\n"
        
        ,arr_package[i].name,arr_package[i].install_date,arr_package[i].lupdate_date,arr_package[i].updates,arr_package[i].removal_date);
        numRead=mystrlen(report);
        if (write(outputFd, report, numRead) < 0 ){
                printf("write returned an error\n");
        } 
    }
    if (close(outputFd) == -1)
        printf("error closing the fle output\n");
}
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

char *getDate(char *line, int size){
    char *date = malloc(size+1);
    for(int i=0;i<size;i++){
        date[i]=line[i+1];
    }
    date[size]='\0';
    return date;    
}

char *getName(char *line, int index){
    int size=0;
    for(int i=index;line[i] != ' ' && line[i] != '\n' && line[i] != '\0';i++){
        size++;
    }
    char *name = malloc(size + 1);
    for(int i=0;i<size;i++){
        name[i]=line[i+index];
    }
    name[size]='\0';
    return name;
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