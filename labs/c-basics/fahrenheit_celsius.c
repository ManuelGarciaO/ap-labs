#include <stdio.h>
#include <stdlib.h>

/* print Fahrenheit-Celsius convertion*/

int main(int argc, char **argv)
{
    int fahr, lower, upper, step;

    if(argc == 2){
        fahr=atoi(argv[1]);
        printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
    }
    else if (argc == 4){
        lower=atoi(argv[1]);
        upper=atoi(argv[2]);
        step=atoi(argv[3]);
        for (fahr = lower; fahr <= upper; fahr = fahr + step)
        printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
    }

    return 0;
}