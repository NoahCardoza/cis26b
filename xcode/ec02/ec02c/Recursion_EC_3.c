/****************************************************
   CIS 26B                              Extra Credit
   RECURSION
   
   Exercise 6, page 201
   Write two functions: 
        1. iterative
        2. recursive  

   double power (int exponent, double number);
   
   Run the program and save the output as a comment!

   Name: Noah Cardoza
   Date: March 6th 2019
*****************************************************/

#include <stdio.h>
#include <stdlib.h>

#define ABS(n) n > 0 ? n : -n

double powerI (int exponent, double number);
double powerR (int exponent, double number);

void test_pow(int exp, int start, int end){
    printf("Iterative                      Recursive\n");
    while (start < end) {
        printf("%-30f %-30f\n", powerI(exp, start), powerR(exp, start));
        start++;
    }
}

int main ( void )
{
    test_pow(2, 1, 10);
    test_pow(-3, 1, 10);
    test_pow(-10, 1, 10);
    test_pow(10, 1, 10);
    return 0;
}

// Iterative
double powerI (int exponent, double number)
{
    int isneg = exponent < 0;
    double ret = 1;
    exponent = ABS(exponent);
    while (exponent-- > 0)
        ret *= number;
    return isneg ? 1 / ret : ret;
}

// Recursive
double powerR (int exponent, double number)
{
    if (exponent == 1) return number;
    if (exponent == -1) return 1 / number;
    if (exponent < 0)
        return (1 / number) * powerR(exponent + 1, number);
    return number * powerR(exponent - 1, number);
}

/* OUTPUT
 Iterative                      Recursive
 1.000000                       1.000000
 4.000000                       4.000000
 9.000000                       9.000000
 16.000000                      16.000000
 25.000000                      25.000000
 36.000000                      36.000000
 49.000000                      49.000000
 64.000000                      64.000000
 81.000000                      81.000000
 Iterative                      Recursive
 1.000000                       1.000000
 0.125000                       0.125000
 0.037037                       0.037037
 0.015625                       0.015625
 0.008000                       0.008000
 0.004630                       0.004630
 0.002915                       0.002915
 0.001953                       0.001953
 0.001372                       0.001372
 Iterative                      Recursive
 1.000000                       1.000000
 0.000977                       0.000977
 0.000017                       0.000017
 0.000001                       0.000001
 0.000000                       0.000000
 0.000000                       0.000000
 0.000000                       0.000000
 0.000000                       0.000000
 0.000000                       0.000000
 Iterative                      Recursive
 1.000000                       1.000000
 1024.000000                    1024.000000
 59049.000000                   59049.000000
 1048576.000000                 1048576.000000
 9765625.000000                 9765625.000000
 60466176.000000                60466176.000000
 282475249.000000               282475249.000000
 1073741824.000000              1073741824.000000
 3486784401.000000              3486784401.000000
 */
