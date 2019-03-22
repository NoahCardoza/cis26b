/*********************************************************************************
 ** CIS 26B                                                           Extra Credit
 ** Advanced C
 ******************

 Multi-dimensional Arrays


 Find a path of maximum value.

 The figure below shows a number triangle. Write a program that
 calculates the highest sum of numbers passed on a route that
 starts at the top and ends somewhere on the base. Each step can
 go either down or diagonally down to the right. The number of
 rows in the triangle is >1 but <=100. The numbers in the triangle,
 all integers, are between 0 and 99.

 Input Data

 Data about the number of rows in the triangle are first read from
 the INPUT.TXT file. In our example, INPUT.TXT appears as follows:

 5

 7
 3 8
 8 1 0
 2 7 4 4
 4 5 2 6 5

 Output Data
 The highest sum is written as an integer in the OUTPUT.TXT file.
 In our example: 30 = 7 + 3 + 8 + 7 + 5

 In addition to printing the sum,
 print the path too, for instance:
 (0,0)->(1,0)->(2,0)->(3,1)->(4,1)


 Name: Noah Cardoza

 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define MAX_DEPTH 100
#define FIRST_COL 0
#define GETTER(a, x, y) *(((a + ((x) * MAX_DEPTH))) + (y))

int max(int x, int y);
int path_finder(int *table, int depth);
void internal_test(void);
void test_from_file();

int main(void)
{
    test_from_file();
    // internal_test();
    return 0;
}

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
 * - finds the path `which` provides the largest sum.
 *
 * - prints the path taken to the screen
 * @param table
 * @param depth
 * @return
 */

int path_finder(int *table, int depth) {
    int v, m, row, col, path[depth];
    path[0] = 0; // the first node in the path is always the first top node
    for (row = depth - 1; row >= 1; --row) { // iterate rows
        // row max defaults to fist col value/index
        m = GETTER(table, row, FIRST_COL);
        path[row] = FIRST_COL;
        for (col = row; col >= 1 ; --col) { // iterate cols
            v = GETTER(table, row, col);
            if (m <= v) {
               m = v;
               path[row] = col;
            }
            // update the row above adding either the node directly below
            // or the one below and to the right – whichever is greater
            GETTER(table, row - 1, col - 1) += max(GETTER(table, row, col - 1), v);
        }
    }
    // traverse `path` to print the path it took to the
    // largest sum
    for (m = 0; m < depth - 1; ++m) printf("(%d, %d)->", m, path[m]);
    // the last node is a little tricky since we need to know where
    // the node above it is - we get the latter row's largest col's
    // index and decide if either the node directly below it, or
    // the node below it and to the right is larger
    v = path[depth - 2];
    if (v == m) path[m] = m;
    else if (GETTER(table, m, v) > GETTER(table, m, v + 1)) path[m] = v;
    else path[m] = v + 1;
    printf("(%d, %d)\n", m, path[m]);

    // the largest sum will be summed up in the first index
    return *table;
}

/**
 * run a set of three default tests on the path_finder function
 */

void internal_test() {
    const int DEPTH = 5;
    int testTables[3][MAX_DEPTH][MAX_DEPTH] =
    {
        {
            {7},
            {3, 8},
            {8, 1, 0},
            {2, 7, 4, 4},
            {4, 5, 2, 6, 5}
        },
        {
            {7},
            {8, 3},
            {8, 1, 0},
            {2, 7, 4, 4},
            {4, 5, 2, 6, 500}

        },
        {
            {7},
            {8, 9},
            {8, 1, 100},
            {2, 7, 400, 4},
            {4000, 5, 2, 6, 5}
        }
    };

    for(int i = 0; i < 3; i++) {
        int res;

        res = path_finder((int*)testTables[i], DEPTH);
        printf("%d\n", res);
    }
}

/**
 * reads a triangle into memory from `INPUT.TXT`
 * and passes it to `path_finder`
 */

void test_from_file() {
    int table[MAX_DEPTH][MAX_DEPTH] = {0};
    int depth, res;
    FILE *fp = fopen("INPUT.TXT", "r");

    if (!fp) {
        printf("Error opening file.\n");
        exit(1);
    }

    fscanf(fp, " %d ", &depth);

    for (int row = 0; row < depth; ++row) {
        for (int col = 0; col <= row; ++col) {
            fscanf(fp, " %d ", &table[row][col]);
        }
    }

    res = path_finder((int *) table, depth);
    printf("%d\n", res);
    fclose(fp);
}

