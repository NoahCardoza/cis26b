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
**  IDE: Xcode
***************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// #define ushort unsigned short

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

ushort and(ushort mask, ushort n){
    return n & mask;
}

ushort or(ushort mask, ushort n){
    return n | mask;
}

void putbits(ushort n){
    for (int i = 15; i >= 0; i--) {
        printf("%d", GET_BIT(n, i));
        if (i == 5 || i == 11) printf(" ");
    }
    printf("\n");
}

/*========================================================
 @ trims a c-string from the left
 
 > *s : the c-string to trim
 
 < returns a pointer moved to the first non-whitespace
   character from the left
 */

char *ltrim(char *s) {
    while (isspace(*s)) s++;
    return s;
}

int main( void )
{
    unsigned long blength;
    ushort light_rack = 0, user_mask;
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
    
    printf("1) turn on all lights\n"
           "2) turn on center stage lights (lights 5-10)\n"
           "3) turn on left stage lights (lights 11-15)\n"
           "4) turn on right stage lights (lights 0-4)\n"
           "5) turn off all lights\n"
           "6) turn off center stage lights\n"
           "7) turn off left stage lights\n"
           "8) turn off right stage lights\n"
           "9) overlay on/off pattern onto light configuration\n"
           "10) quit the menu\n");

    size_t buff = 64;
    char *input = NULL, *parser;
    long choice, bit_shift;
    
    while ((void)(printf("> ")), getline(&input, &buff, stdin) > 0) {
        parser = input;
        choice = strtol(input, &parser, 10);
        parser = ltrim(parser);
        
        ASSERT_CONT(isnumber(*parser), "Only one menu selection allowed!")
        ASSERT_CONT(*parser, "Illegal menu selection!")
        ASSERT_CONT(choice < 1 || choice > 10, "Illegal range!")
        
        if (choice == 10) {
            free(input);
            break;
        }
        
        if (choice == 9) {
            printf("-> ");
            if (getline(&input, &buff, stdin) <= 0) exit(0);
            parser = input;
            parser = ltrim(parser);
            ASSERT_CONT(!isnumber(*parser), "Illegal start shift!")
            bit_shift = strtol(parser, &parser, 10) - 1;
            parser = ltrim(parser);
            ASSERT_CONT(!isnumber(*parser), "Illegal start bit!")
            blength = strspn(parser, "01");
            ASSERT_CONT(*ltrim(parser + blength + 1), "Illegal mask!")
            ASSERT_CONT(bit_shift + blength > 16, "shift + mask must not be longer than 16 bits!")
            user_mask = strtol(parser, &parser, 2);
            user_mask <<= bit_shift;
            light_rack &= MASK_OFF(bit_shift, blength);
            light_rack |= user_mask;
        } else {
            choice--;
            light_rack = operations[choice].fn(light_rack, operations[choice].mask);
        }
        
        putbits(light_rack);
    }

    return 0;
}

