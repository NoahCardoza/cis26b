/*********************************************************
   CIS 26B -  Advanced C Programming
   Write a function named findMax that finds the maximum of
   any number of integer arguments.

   Extra Credit:
/*********************************************************/

#include <stdio.h>
#include <stdarg.h>

/**
 * classic max value function
 * @param x any number
 * @param y any number
 * @return returns whichever number is greater
 */

int max(int x, int y) {
    if (x > y) return x;
    return y;
}

/**
 * finds the largest number passed to the function
 * @param n the number of arguments passed
 * @param ... the numbers to evaluate
 * @return the largest number passes
 */

int findMax(int n, ...) {
    int m;

    va_list args;
    va_start(args, n);

    n--;
    m = va_arg(args, int);
    for (int i = 0; i < n; ++i) {
        m = max(m, va_arg(args, int));
    }

    return m;
}

int main(void)
{
    printf("The largest is: %d\n", findMax(3, 20, 90, 10));
    printf("The largest is: %d\n", findMax(4, 20, 10, 50, 30));
    printf("The largest is: %d\n", findMax(2, 20, 10));
    printf("The largest is: %d\n", findMax(1, 13));
    printf("The largest is: %d\n", findMax(5, -20, 10, -50, -30, -90));

    return 0;
}
