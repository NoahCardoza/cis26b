/**************************************************
   CIS 26B                              Extra Credit 
   RECURSION
   
   Get strings from the keyboard and prints them 
   in reverse order using printBack, 
   a recursive function. 
   
   Run the program and save the output as a comment!

   Name: Noah Cardoza
   Date: March 6th 2019
***************************************************/

#include <stdio.h>
#include <string.h>

int main ( void )
{
    void printBack(char *s);    
    char s[100];

    while (printf("Enter a string or \"quit\"\n"), gets(s), strcmp(s, "quit"))
    {
        printBack(s);
        putchar('\n');
    }

    return 0;
}

void printBack(char *s)
{    
    if (*s) {
        printBack(s + 1);
        putchar(*s);
    }

    return ; 
}

/* OUTPUT
 Enter a string or "quit"
 warning: this program uses gets(), which is unsafe.
 olleh
 hello
 Enter a string or "quit"
 hello
 olleh
 Enter a string or "quit"
 12345
 54321
 Enter a string or "quit"
 1
 1
 Enter a string or "quit"
 12
 21
 Enter a string or "quit"
 quit
*/
