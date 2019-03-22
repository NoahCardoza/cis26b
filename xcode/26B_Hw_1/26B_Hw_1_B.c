/***
 CIS 26B: Homework 1B
 ********************
 NAME:
 IDE (compiler):
 *************************************************************************
 
   Review: Functions, Structures, Pointers, Arrays, Sorting, Data Files,
        and Dynamic Allocation of Memory
    
   Create and process a dynamically allocated array of structures
   Save the output as a comment at the end of the program!
*/
#include <stdio.h>
#include <stdlib.h>

#ifdef _MSC_VER
#include <crtdbg.h>  // needed to check for memory leaks (Windows only!)
#endif

int main( void )
{



    #ifdef _MSC_VER
    printf( _CrtDumpMemoryLeaks() ? "Memory Leak\n" : "No Memory Leak\n");
    #endif

    return 0;
}

