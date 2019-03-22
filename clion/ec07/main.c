//
// Created by Darwin on 2019-03-19.
//

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBERS_TO_SCREEN 10
#define OUTPUT_FILE "out.txt"

// NOTE: short can die in a ditch, alone, cold, and afraid.
//       we use long from here on
#define FIELD_TYPE unsigned short
#define BYTES_PER_TYPE (sizeof(FIELD_TYPE) * 8)
#define PRINTF_TYPE "%lu"
#define CEIL_DIV(x, y) 1 + ((x - 1) / y)

#define MASK_ON(start, len) ((FIELD_TYPE)(~(((FIELD_TYPE)~0) << len)) << start)
#define MASK_OFF(start, len) (~MASK_ON(start, len))

#define GET_BIT(n, i) ((n >> i) & 1)

#define ALL_BITS_ON ((FIELD_TYPE)~0)

#define GET_NUMBER(a, i) (a[CEIL_DIV(i + 1, BYTES_PER_TYPE) - 1])

#define GET_FIELD_BIT(f, i) GET_BIT(GET_NUMBER(f, i), (i % BYTES_PER_TYPE))
#define SET_FIELD_BIT_OFF(f, i) GET_NUMBER(f, i) &= MASK_OFF(i % BYTES_PER_TYPE, 1)

//void print(const char *fmt, ...) {
//    static int nth = NUMBERS_TO_SCREEN;
//    static FILE *out = NULL;
//    va_list args;
//    va_start(args, fmt);
//    
//    if (!out) out = stdout;
//    
//    if (nth) {
//        out = fopen() 
//        nth--;
//    }
//    fprintf(fmt, args);
//}

void putbits(FIELD_TYPE n, int m){
    for (int i = BYTES_PER_TYPE - 1; i >= 0; i--) {
        if (i == m) printf("*");
        else
        printf(PRINTF_TYPE, GET_BIT(n, i));
        if (!(i % 8)) printf(" ");
    }
    printf("\n");
}

void turn_field_on(FIELD_TYPE *field, FIELD_TYPE n) {
    while (n) field[--n] = ALL_BITS_ON;
}

void findPrimes(FIELD_TYPE n) {
    int c = 0;
    // think about 0
    FIELD_TYPE i, j;
    // because indexes start with 0
    FIELD_TYPE elements = CEIL_DIV(n, BYTES_PER_TYPE) + 1;
    FIELD_TYPE *byte_field = malloc(elements);
    turn_field_on(byte_field, elements);

    if (n < 2) return;
  
    byte_field[0] &= MASK_OFF(0, 2);

    for (i = 2; i + i <= n; ++i) {
        // putbits(GET_NUMBER(byte_field, i), i % BYTES_PER_LONG);
        if (!GET_FIELD_BIT(byte_field, i)) continue;
        for (j = i + i; j <= n; j += i) {
            SET_FIELD_BIT_OFF(byte_field, j);
        }
        c++;
        printf(PRINTF_TYPE "\n", i);
    }

    for (; i <= n; ++i) {

        if (GET_FIELD_BIT(byte_field, i)) printf(PRINTF_TYPE "\n", i), c++;

    }
    free(byte_field);
    printf("count: %d\n", c);
}

int main(void) {
    // printf("%lu\n", BYTES_PER_LONG);
    findPrimes(10000);
    return 0;
}