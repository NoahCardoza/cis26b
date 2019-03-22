/*********************************************************
   CIS 26B -  Advanced C Programming
   Write a function named display that takes any number of arguments.
   The first argument must be an integer.
   The remaining arguments will be strings.
   The first argument specifies the number of strings.

   Extra Credit:
/*********************************************************/

#include <stdio.h>
#include <stdarg.h>

/**
 * prints all the c-strings passed to the function
 * @param n the number of strings passed
 * @param ... any number of strings
 */

void display(int n, ...) {
    va_list args;
    va_start(args, n);
    for (int i = 0; i < n; ++i) {
        printf("%03d : %s\n", i + 1, va_arg(args, char*));
    }
}

int main(void)
{
    display(5, "one", "two", "three", "four", "five");
    display(2, "dog", "cat");
    display(4, "dog", "cat", "mouse", "bird");
    display(3, "cat", "mouse", "bird");
    return 0;

}

