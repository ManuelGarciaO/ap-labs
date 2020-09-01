#include <stdio.h>
#include <string.h>

void main()
{
    int i = 0, f = 0;
    char word[1000], final[100000]; 
    while((word[i] = getchar())!= EOF){
            if(word[i]==' '){
                for(int y=i-1;y>=0;y--){
                    final[f]=word[y];
                    f++;
                }
                final[f]=' ';
                f++;
                i=0;
                word[i]='\0';
            }
            else if(word[i]=='\n'){
                for(int y=i-1;y>=0;y--){
                    final[f]=word[y];
                    f++;
                }
                final[f++]='\n';
                final[f]='\0';
                printf("%s", final);
                i=0;
                word[i]='\0';
                f=0;
                final[f]='\0';
            }else{
                i++;
                word[i]='\0';
            }
            
    }
}
