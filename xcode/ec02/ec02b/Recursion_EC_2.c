/****************************************************
   CIS 26B                              Extra Credit 
   RECURSION
   
   Get strings from the keyboard and call a recursive 
   function to calculate their length 
   
   Run the program and save the output as a comment!

   Name: Noah Cardoza
   Date: March 6th 2019
*****************************************************/

#include <stdio.h>
#include <string.h>

int main ( void )
{
    int str_len(char *s);
    char s[100];

    while (printf("Enter a string or \"quit\"\n"), gets(s), strcmp(s, "quit"))
    {
        printf("%d\n", str_len(s));
    }
    return 0;
}

int str_len(char *s)    
{    
    if (*s)
        return 1 + str_len(s + 1);
    return 0;
}


/* OUTPUT
 Enter a string or "quit"
 warning: this program uses gets(), which is unsafe.
 hello
 5
 Enter a string or "quit"
 hell
 4
 Enter a string or "quit"
 
 0
 Enter a string or "quit"
 1
 1
 Enter a string or "quit"
 123
 3
 Enter a string or "quit"
 12
 2
 Enter a string or "quit"
 
 0
 Enter a string or "quit"
 quit
 */
