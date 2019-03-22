/*********************************************************************************
** CIS 26B
** Advanced C
******************
**
** Homework 4:
**        Bit Manipulation
**
**********************************************************************************

   This program...

   Save the output as a comment at the end of the program!
**********************************************************
**
**  Written By: Noah Cardoza
**
**  IDE: CLion
***************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MASK_ON(start, len) ((ushort)(~(((ushort)~0) << len)) << start)
#define MASK_OFF(start, len) ((ushort)~MASK_ON(start, len))
#define GET_BIT(n, i) ((n >> i) & 1)
#define ADD_OPT(mask, fn) { (ushort) mask, &fn }

#define MID (5, 6)
#define LEFT (11, 5)
#define RIGHT (0, 5)

#define ASSERT_CONT(exp, msg) \
if (exp) {\
    printf(msg " Try again!\n");\
    continue;\
}

typedef unsigned short ushort;

typedef struct t_operation {
    ushort mask;
    ushort (*fn)(ushort, ushort);
} OPERATION;

ushort and(ushort x, ushort y);
ushort or(ushort x, ushort y);
void put_bits(ushort n);
char *ltrim(char *s);

int main(void) {
    unsigned long blength;
    ushort light_rack = 0, user_mask;
    size_t buff = 64;
    char *input = NULL, *parser;
    long choice, bit_shift;
    OPERATION operations[] = {
            ADD_OPT(~0, or),
            ADD_OPT(MASK_ON MID, or),
            ADD_OPT(MASK_ON LEFT, or),
            ADD_OPT(MASK_ON RIGHT, or),
            ADD_OPT(0, and),
            ADD_OPT(MASK_OFF MID, and),
            ADD_OPT(MASK_OFF LEFT, and),
            ADD_OPT(MASK_OFF RIGHT, and)
    };

    printf("1)  turn on all lights\n"
           "2)  turn on center stage lights (lights 5-10)\n"
           "3)  turn on left stage lights (lights 11-15)\n"
           "4)  turn on right stage lights (lights 0-4)\n"
           "5)  turn off all lights\n"
           "6)  turn off center stage lights\n"
           "7)  turn off left stage lights\n"
           "8)  turn off right stage lights\n"
           "9)  overlay on/off pattern onto light configuration\n"
           "10) quit the menu\n");

    while ((void)(printf("> ")), getline(&input, &buff, stdin) > 0) {
        parser = input;
        choice = strtol(input, &parser, 10);
        parser = ltrim(parser);

        ASSERT_CONT(isnumber(*parser), "Only one menu selection allowed!")
        ASSERT_CONT(*parser, "Illegal menu selection!")
        ASSERT_CONT(choice < 1 || choice > 10, "Illegal range!")

        if (choice == 10) break;

        if (choice == 9) {
            printf("-> ");
            if (getline(&input, &buff, stdin) <= 0) break;
            parser = input;
            parser = ltrim(parser);
            ASSERT_CONT(!isnumber(*parser), "Illegal start shift!")
            bit_shift = strtol(parser, &parser, 10);
            ASSERT_CONT(bit_shift < 1 || bit_shift > 16, "The shift must be inclusively between 1 and 16!")
            bit_shift--; // indexes start at 0
            parser = ltrim(parser);
            ASSERT_CONT(!isnumber(*parser), "Illegal start bit!")
            blength = strspn(parser, "01");
            ASSERT_CONT(*ltrim(parser + blength), "Illegal mask!")
            ASSERT_CONT(bit_shift + blength > 16, "The shift plus the number of bits in the mask must not be longer than 16!")
            user_mask = (ushort) strtol(parser, &parser, 2);
            user_mask <<= bit_shift;
            light_rack &= MASK_OFF(bit_shift, blength);
            light_rack |= user_mask;
        } else {
            choice--;
            light_rack = operations[choice].fn(light_rack, operations[choice].mask);
        }

        put_bits(light_rack);
    }
    free(input);
    return 0;
}

/**
 * Applies the & operator to both x and y
 * @param x
 * @param y
 * @return x & y
 */

ushort and(ushort x, ushort y){
    return y & x;
}

/**
 * Applies the | operator to both x and y
 * @param x
 * @param y
 * @return y | x
 */

ushort or(ushort x, ushort y){
    return y | x;
}

/**
 * Prints out each bit in a unsigned short
 * @param n the number whose bits are to be printed
 */

void put_bits(ushort n){
    for (int i = 15; i >= 0; i--) {
        printf("%d", GET_BIT(n, i));
        if (i == 5 || i == 11) printf(" ");
    }
    printf("\n");
}

/**
 * trims a c-string from the left
 * @param s the c-string to trim
 * @return returns a pointer moved to the first
 * non-whitespace character from the left
 */

char *ltrim(char *s) {
    while (isspace(*s)) s++;
    return s;
}

/* OUTPUT
 1)  turn on all lights
 2)  turn on center stage lights (lights 5-10)
 3)  turn on left stage lights (lights 11-15)
 4)  turn on right stage lights (lights 0-4)
 5)  turn off all lights
 6)  turn off center stage lights
 7)  turn off left stage lights
 8)  turn off right stage lights
 9)  overlay on/off pattern onto light configuration
 10) quit the menu
 > 1
 11111 111111 11111
 > 8
 11111 111111 00000
 > 7
 00000 111111 00000
 > 6
 00000 000000 00000
 >  4
 00000 000000 11111
 > 3
 11111 000000 11111
 > 2
 11111 111111 11111
 > 5
 00000 000000 00000
 > 9
 -> 6 001100
 00000 001100 00000
 > 9
 -> 1 11011
 00000 001100 11011
 > 9
 -> 12 11011
 11011 001100 11011
 > 10
 */

/* ERROR TEST
 1)  turn on all lights
 2)  turn on center stage lights (lights 5-10)
 3)  turn on left stage lights (lights 11-15)
 4)  turn on right stage lights (lights 0-4)
 5)  turn off all lights
 6)  turn off center stage lights
 7)  turn off left stage lights
 8)  turn off right stage lights
 9)  overlay on/off pattern onto light configuration
 10) quit the menu
 > 432kj4
 Illegal menu selection! Try again!
 > k34
 Illegal menu selection! Try again!
 >  432k 44
 Illegal menu selection! Try again!
 > 9
 -> 17 110
 The shift must be inclusively between 1 and 16! Try again!
 > 10 010101010101010101
 Only one menu selection allowed! Try again!
 > 9
 -> 10 101010101011010101
 The shift plus the number of bits in the mask must not be longer than 16! Try again!
 > 9
 -> h2 10
 Illegal start shift! Try again!
 > 9
 ->       1   10 i
 Illegal mask! Try again!
 > 9
 -> 1 11102
 Illegal mask! Try again!
 > 10
 */