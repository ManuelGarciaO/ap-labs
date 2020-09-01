#include <stdio.h>
#include <stdlib.h>

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday){
    //Es bisiesto?
    int daytab[2][13] = { {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31} };
    int leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
    int days=0,i;
    for (i = 1; i <= 12 && days<yearday; i++){
        if(days<yearday)
            days += daytab[leap][i];
    }
    *pmonth=--i;
    *pday=daytab[leap][i]-(days -yearday);
}

int main(int argc, char **argv) {
    if(argc==3){
        int year=atoi(argv[1]);
        int day=atoi(argv[2]);
        if((!(year%4 == 0 && year%100 != 0 || year%400 == 0) && day > 365) || day > 366 || day < 1){ //validate day
            printf("Invalid input of day\n"); return 0;
        }
        int pmonth, pday;
        month_day(year, day, &pmonth, &pday);
        char months[][10] = {"","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
        printf("%s %d, %d \n", months[pmonth], pday, year);
    }
    else{
        printf("Invalid number of arguments \n");
    }
    
    return 0;
}