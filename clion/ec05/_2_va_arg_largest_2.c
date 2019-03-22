/*********************************************************
   CIS 26B -  Advanced C Programming
   Write a function named findMax that finds the maximum of
   any number of integer arguments. Use as a sentinel value 0.
   The first parameter is to represent one of the numbers.

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
 * finds and returns the biggest argument passed to it
 * @param n just one of the numbers
 * @param ... an unknown number of numbers delimited by a 0
 * @return the largest argument
 */

int findMax(int n, ...) {
    int m;

    m = n;
    va_list args;
    va_start(args, n);

    while ((n = va_arg(args, int))) {
        m = max(m, n);
    }

    return m;
}

int main(void)
{
    printf("The largest is: %d\n", findMax(99, 20, 90, 10, 0));
    printf("The largest is: %d\n", findMax(40, 20, 10, 50, 30, 0));
    printf("The largest is: %d\n", findMax(25, 20, 10, 0));
    printf("The largest is: %d\n", findMax(10, 13, 0));
    printf("The largest is: %d\n", findMax(5, -20, 2, -50, -30, -90, 0));

    return 0;

}

/**
  Finds the maximum of any number of integer arguments.
*/
