/*********************************************************
   CIS 26B -  Advanced C Programming
   Write a function named findAvg that finds the average of
   of the integer arguments. It also displays the parameters
   greater than or equal to the average.

   Extra Credit:
/*********************************************************/

#include <stdio.h>
#include <stdarg.h>

double findAvg(int n, ...);

int main(void)
{
    printf("The average is: %.1f\n", findAvg(3, 20, 90, 10));
    printf("The average is: %.1f\n", findAvg(4, 20, 10, 50, 31));
    printf("The average is: %.1f\n", findAvg(2, 22, 13));
    printf("The average is: %.1f\n", findAvg(1, 13));
    printf("The average is: %.1f\n", findAvg(5, -20, 11, -53, -30, -90));

    return 0;

}

/**
 * finds the average and prints out all values greater
 * than said average
 * @param n the number of numbers to average
 * @param ... the numbers to average
 * @return the average of the passed arguments
 */

double findAvg(int n, ...) {
    int argv[n];
    double avg = 0;
    va_list args;

    va_start(args, n);
    for (int i = 0; i < n; ++i)
        avg += (argv[i] = va_arg(args, int));

    avg /= n;
    printf("arguments >= avg -> [ ");
    for (int i = 0; i < n; ++i)
        if (argv[i] >= avg)
            printf("%d ", argv[i]);
    printf("]\n");


    return avg;
}
