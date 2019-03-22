//
// Created by Darwin on 2019-03-10.
//

#include <stdio.h>
#include <stdarg.h>

typedef struct t_execute {
    int i;
    int (*fn)(int);
} EXE;

/**
 * decrements `i`
 * @param i the number to decremented
 * @return i - 1
 */

int dec(int i) {
    return i - 1;
}

/**
 * increments `i`
 * @param i the number to be incremented
 * @return i + 1
 */

int inc(int i) {
    return i + 1;
}

/**
 * applies functions to their arguments
 * @param foo only used for the position of
 * the following arguments
 * @param ... EXE arguments terminated by NULL
 */

void run_funcs(int foo, ...) {
    EXE *cmd;
    va_list args;
    va_start(args, foo);

    while ((cmd = va_arg(args, EXE*)), cmd)
        printf("%d ", (*cmd->fn)(cmd->i));

    va_end(args);
}

int main(void) {
    EXE sel1 = { 0, &inc }, sel2 = { 1, &dec };
    run_funcs(0, &sel1, &sel2, NULL);
    return 0;
}