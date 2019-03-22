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
 IDE: CLion
 *******************************************************/

#include <stdio.h>

#define MASK_ON(startbit, numbits) ((~(((unsigned short)~0) << numbits)) << startbit)
#define MASK_OFF(startbit, numbits) (~MASK_ON(startbit, numbits))

int  test_bit(unsigned short wrd, int bit_to_test);
void print_bits(unsigned short wrd);
unsigned short rev_bits(unsigned short b);

int main (void) {
    unsigned short wrd = 65, a = 32, b, mask;
    int numbits = 3;
    int startbit = 2;

    print_bits(wrd);

    mask = MASK_ON(startbit, numbits);
    b = a | mask;
    printf("Turn ON %d bits starting at bit#%d\n", numbits, startbit);
    printf("%d\n", a);
    print_bits(a);
    print_bits(b);
    printf("%d\n", b);

    a = 31;
    mask = MASK_OFF(startbit, numbits);
    b = a & mask;
    printf("Turn OFF %d bits starting at bit#%d\n", numbits, startbit);
    printf("%d\n", a);
    print_bits(a);
    print_bits(b);
    printf("%d\n", b);

    a = 85;

    mask = MASK_ON(startbit, numbits); //| ((MASK_OFF(startbit, numbits) & a));
    b = a ^ mask;
    printf("Toggle %d bits starting at bit#%d\n", numbits, startbit);
    printf("%d\n", a);
    print_bits(a);
    print_bits(b);
    printf("%d\n", b);

    printf("Reversing bits from\n");
    print_bits(a);
    printf("to\n");
    b = rev_bits(a);
    print_bits(b);
    printf("\n");
    return 0;
}

/**************************************************************
 Return 1 if bit_to_test is set and 0 if it is unset
 */
int  test_bit(unsigned short wrd, int bit_to_test)
{
    wrd >>= bit_to_test;
    wrd &= 1;
    return wrd;
}

/**************************************************************
 This uses a trick to print an unsigned short as a string of
 16 bits
 */
void print_bits(unsigned short wrd)
{
    int  testbit(unsigned short wrd, int bit_to_test);
    int i;

    for (i = 15; i >= 0; i--)
    {
        printf("%1d", test_bit(wrd, i));
        if (!(i % 4))
            printf(" ");
    }
    printf("\n");
}

/**************************************************************
 * Reverses bits and returns a copy
 * @param b the bit pattern
 * @return b with its bits reversed
 */
unsigned short rev_bits(unsigned short b) {
    unsigned short r = 0;
    for (int i = 0; i < 15; i++) {
        r <<= 1;
        r |= test_bit(b, i);
    }
    r <<= 1;
    return r;
}

/* OUTPUT
 0000 0000 0100 0001
 Turn ON 3 bits starting at bit#2
 32
 0000 0000 0010 0000
 0000 0000 0011 1100
 60
 Turn OFF 3 bits starting at bit#2
 31
 0000 0000 0001 1111
 0000 0000 0000 0011
 3
 Toggle 3 bits starting at bit#2
 85
 0000 0000 0101 0101
 0000 0000 0100 1001
 73
 Reversing bits from
 0000 0000 0101 0101
 to
 1010 1010 0000 0000
 */