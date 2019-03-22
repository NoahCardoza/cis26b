/*******************************************************
 Homework 4: Part A
 
 Biwise Operators
 
 There are three requirements:
 1. Define and use a macro to turn on a range of bits
 2. Define and use a macro to turn off a range of bits
 3. Define and use a macro to toggle a range of bits
 4. Write a function that takes an unsigned short parameter and returns an unsigned short 
 with the bits in the parameter turned backwards.
 Thus, if the parameter has a binary value of
 1111 1111 1111 0000 0000 0000 0000 0011
 your function will return a value whose binary representation is
 11000000000011111111111111111111
 
 NAME: Noah Cardoza
 IDE: Xcode
 *******************************************************/

#include <stdio.h>

#define MASK_ON(startbit, numbits) ((~(((unsigned short)~0) << numbits)) << startbit)
#define MASK_OFF(startbit, numbits) (~MASK_ON(startbit, numbits))

//#define BIT(a,b,c,d,e,f,g,h) (~MASK_ON(startbit, numbits))
//0x000101
//BIT(0,0,0,0,0,1,1,1)

int  testbit(unsigned short wrd, int bit_to_test);
void printbits(unsigned short wrd);


unsigned short revbits(unsigned short b) {
    unsigned short r = 0;
    for (int i = 0; i < 15; i++) {
        r <<= 1;
        r |= testbit(b, i);
    }
    r <<= 1;
    return r;
}

int main (void)
{
    unsigned short wrd = 65, a = 32, b, mask;
    int numbits = 3;
    int startbit = 2;
    
    printbits(wrd);
    
    // replace this with a macro with arguments
    mask = MASK_ON(startbit, numbits);
    b = a | mask;
    printf("Turn ON %d bits starting at bit#%d\n", numbits, startbit);
    printf("%d\n", a);
    printbits(a);
    printbits(b);
    printf("%d\n", b);
    
    a = 31;
    // replace this with a macro with arguments
    mask = MASK_OFF(startbit, numbits);
    b = a & mask;
    printf("Turn OFF %d bits starting at bit#%d\n", numbits, startbit);
    printf("%d\n", a);
    printbits(a);
    printbits(b);
    printf("%d\n", b);
    
    a = 85;
    
    // (MASK_ON(startbit, numbits) & a
    mask = MASK_ON(startbit, numbits); //| ((MASK_OFF(startbit, numbits) & a));
    printf("--------\n");
    printbits(MASK_ON(startbit, numbits));
    printbits((MASK_OFF(startbit, numbits) & a));
    printbits(a);
    printbits(mask);
    printbits(a ^ mask);
    printf("--------\n");
//    (
//           (MASK_ON(startbit, numbits) & a)
//           ^ (MASK_ON(startbit, numbits) & a
//           ))
//           | (MASK_OFF(startbit, numbits) & a);
//
    
    
    // mask = ??? // define this mask as a macro with arguments
    // c = ???
    printf("Toggle %d bits starting at bit#%d\n", numbits, startbit);
    printf("%d\n", a);
    printbits(a);
    printbits(b);
    printf("%d\n", b);
    printf("-----------\n");
    printbits(a);
    printbits(revbits(a));
    
    return 0;
}

/**************************************************************
 Return 1 if bit_to_test is set and 0 if it is unset
 */
int  testbit(unsigned short wrd, int bit_to_test)
{
    wrd >>= bit_to_test;
    wrd &= 1;
    return wrd;
}

/**************************************************************
 This uses a trick to print an unsigned short as a string of
 16 bits
 */
void printbits(unsigned short wrd)
{
    int  testbit(unsigned short wrd, int bit_to_test);
    int i;
    
    for (i = 15; i >= 0; i--)
    {
        printf("%1d", testbit(wrd, i));
        if (!(i % 4))
            printf(" ");
    }
    printf("\n");
}
