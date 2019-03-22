/***
 CIS 26B: Homework 0
 ********************
 NAME: Noah Cardoza
 IDE(compiler): Xcode
 *********************************************************************************************
 This program provides antonyms to common words. An antonym is a word with
 the opposite meaning. For example, the antonym of happy is sad. The program is to
 look for a word and if found, report its antonym. If the word is not found, the
 program is to display an appropriate message.
 
 The input text file has a word and its antonym on each line, as it is shown below:
 
 happy sad
 ugly  attractive
 nice  rude
 cold  warmth
 
 Your task:
 
 1. Code review: read and understand this program (draw charts and diagrams)
 2. Call the following functions in main (in this order):
 printInfo(), getWords(), printList(), searchManager(), freeList(), and printEnd()
 3. Implement the freeList() function
 4. Run the program. Save its output at the end of the program as a comment.
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

const char FILENAME[] = "antonyms_0.txt";
//const char FILENAME[] = "antonyms_1.txt";
//const char FILENAME[] = "antonyms_2.txt";

typedef struct
{
    char *word;     // a pointer to a dynamically allocated string
    char *antonym;  // a pointer to its antonym
} PAIR;

typedef struct
{
    int size;     // the number of words in the list
    int max_len;  // the length of the longest word
    PAIR *pair;     // a pointer to a dynamically allocated array
} LIST;

void printInfo (void);
void printEnd (void);

LIST *createList(int num);
LIST *getWords( const char fileName[] );
char *allocateString( char *inString );
void  readPair(FILE *fpWord, PAIR *pair, PAIR *revPair);
void  insertPair(LIST *list, PAIR pair);

void printList(LIST *list);
void printLine(int max);
void searchManager(LIST *list);
int  binSearch(LIST *list, char *word, char **antonyms);
LIST *freeList(LIST *list);

int main (void)
{
    LIST *list;
    
    printInfo();
    list = getWords(FILENAME);
    printList(list);
    searchManager(list);
    list = freeList(list);
    printEnd();
    
#ifdef _MSC_VER
    printf(_CrtDumpMemoryLeaks() ? "Memory Leak\n" : "No Memory Leak\n");
#endif
    return 0;
    
}
/********************************************************
 Prints information about the project.
 PRE  : nothing
 POST : info printed
 */
void printInfo (void)
{
    printf("\n\n\t\tArrays, Strings, Structures, Sorting, Pointers, and "
           "\n\n\t\t\t\tDynamic Allocation of Memory\n\n");
    printf("This program provides antonyms to common words.\n");
    putchar('\n');
}

/********************************************************
 Prints a farewell message.
 PRE  : nothing.
 POST : farewell message printed
 */
void printEnd (void)
{
    printf("\n\t\tThank you for using the program,"
           "\n\t\tHave a great day!\n");
}

/********************************************************
 Allocates the header of the list and the list of pointers
 to words and their antonyms.
 Pre:  num - number of pairs
 Post: list - empty
 */
LIST *createList(int num)
{
    LIST *list;
    
    // allocate the header of the list
    list = (LIST *)malloc(sizeof(LIST));
    if (!list)
        (void)(printf ("Error allocating the header of the list!\n")), exit (102);
    
    // allocate the list of pointers to words and their antonyms + 1 for an empty word
    // to be placed in the beginning of the list to make insert simpler: it acts as
    // a sentinel value
    list->pair = (PAIR *) calloc(2 * num + 1, sizeof(PAIR));
    if (!list)
        (void)(printf ("Error allocating the list of words and their antonyms!\n")), exit (103);
    
    list->pair->word = allocateString(""); // an empty word
    list->pair->antonym = list->pair->word;
    
    list->size = 1;     // there is one word in the list: the empty word
    list->max_len = 0;
    
    return list;
}

/********************************************************
 Creates a sorted list of structures containing pointers
 to words and their antonyms.
 Pre:  fileName - the name of the input file
 Post: pointer to the list of structures
 */
LIST *getWords( const char fileName[] )
{
    FILE *fpWord;
    LIST *list;
    PAIR  pair, revPair;
    int   i, num;
    
    // open the input file
    fpWord = fopen (fileName, "r");
    if (!fpWord)
        (void)(printf ("Error opening %s!\n", fileName)), exit (101);
    
    // read the number of pairs
    fscanf(fpWord, "%d", &num);
    list = createList(num);
    // read and insert pairs of words
    for( i = 0; i < num; i++ ){
        readPair(fpWord, &pair, &revPair);
        insertPair(list, pair);
        insertPair(list, revPair);
        // update the length of the longest word
        if (strlen(pair.word) > list->max_len )
            list->max_len = (int) strlen(pair.word);
        if (strlen(pair.antonym) > list->max_len )
            list->max_len = (int) strlen(pair.antonym);
    }
    
    fclose(fpWord);
    return list;
}

/********************************************************
 Creates a dynamically allocated string with the same contents
 as the input string
 Pre:  inString - input string
 Post: outString - dynamically allocated
 */
char *allocateString( char *inString )
{
    char *outString;
    int   stringSize;
    
    stringSize =(int) strlen( inString ) + 1;
    outString = (char *) calloc (stringSize, sizeof (char));
    if ( outString == NULL)
        (void)(printf ("ERROR, not enough memory!!!\a\n")), exit (103);
    strcpy (outString, inString);
    
    return outString;
}

/********************************************************
 Reads a pair of words and prepares them for insetions
 Pre:  fpWord
 Post: pair, revPair
 */
void  readPair(FILE *fpWord, PAIR *pair, PAIR *revPair)
{
    char tempWord[100], tempAntonym [100];
    
    fscanf(fpWord, "%s %s", tempWord, tempAntonym);
    pair->word = allocateString(tempWord);
    pair->antonym = allocateString(tempAntonym);
    revPair->word = pair->antonym;
    revPair->antonym = pair->word;
}

/********************************************************
 Inserts an element into a sorded list (like in insertion sort)
 Pre:  list, pair
 Post: list updated
 */
void  insertPair(LIST *list, PAIR pair)
{
    int curr = list->size;
    int i = curr - 1;
    
    while ( i >= 0 && strcmp(pair.word, list->pair[i].word) < 0 ){
        list->pair[i+1] = list->pair[i];  // shift to make room for the new element
        i--;
    }
    list->pair[i+1] = pair; // location found: insert the new element into the list
    list->size++;           // update size
}

/********************************************************
 This function prints the of words and their antonyms
 in a variable format
 */
void printList(LIST *list)
{
    int i;
    char fmt[20];  // the format string
    
    printLine(list->max_len);
    for(i = 0; i < list->max_len - 4; i++)
        printf(" ");
    printf("word  antonym\n");
    printLine(list->max_len);
    
    sprintf(fmt, "%%%ds  %%-%ds\n", list->max_len, list->max_len ); // create the format string
    for( i = 1; i < list->size; i++ )// print list: skip the empty word
        printf(fmt,list->pair[i].word, list->pair[i].antonym);
    
    printLine(list->max_len);
}

/********************************************************
 This function prints a line of "=" for the output's
 header
 */
void printLine(int max)
{
    int i;
    
    for (i = 0; i < max; i++)
        printf("=");
    printf("  ");
    for (i = 0; i < max; i++)
        printf("=");
    printf("\n");
}

/********************************************************
 This function will prompt user to enter an word in order
 to let the program find its antonym.
 Pre:  the pointer list
 Post: nothing
 */
void searchManager(LIST *list)
{
    char targetWord[100];
    char quitWord[5] = "quit", quitWord1[5] = "QUIT";
    char *antonyms[25]; // I can't imagin a word with over 25 antonyms 25. Not the best solution, but time is short!
    int found;
    
    printf("\nPlease enter a word or \"quit\" to stop searching: ");
    scanf("%s", targetWord);
    
    while (strcmp(quitWord, targetWord) != 0 && strcmp(quitWord1, targetWord) != 0) {
        found = binSearch(list, targetWord, antonyms);
        
        if (!found)
            printf("\"%s\" not found\n", targetWord);
        else
            printf("Antonyms for \"%s\" include:\n", targetWord);
        
        while (found--) {
            printf("\t%s\n", antonyms[found]);
        }
        
        printf("\nPlease enter a word or \"quit\" to stop searching: ");
        scanf("%s", targetWord);
    }
}

/********************************************************
 This function will search the list to check if the word
 user enter is in the list using binary search
 Pre:  list - sorted list of words and antonyms
 word - word whose antonym to search for
 Return: word's antonym if found, otherwise NULL
 */
int binSearch(LIST *list, char *word, char **antonyms)
{
    int lo = 0, hi = list->size - 1, mid = 0;
    
    int upper, lower, found = 0;
    
    while (lo <= hi) {
        mid = (lo + hi) / 2;
        int cmp = strcmp(word, list->pair[mid].word);
        if (cmp == 0)
            lo = hi + 1;
        else if (cmp < 0)
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    
    found = !strcmp(word, list->pair[mid].word);
    
    if (!found) return found; // if mid isn't a match, stop looking
    
    upper = lower = mid;
    
    antonyms[0] = list->pair[mid].antonym;
    
    //     prevents seg faults
    while (upper + 1 < list->size && !strcmp(word, list->pair[++upper].word)) // search above the mid
        antonyms[found++] = list->pair[upper].antonym;
    
    //     no danger of seg faults since the first pair is empty `lower >= 1`
    while (!strcmp(word, list->pair[--lower].word)) // search below mid
        antonyms[found++] = list->pair[lower].antonym;
    
    return found;
}

/********************************************************
 This function will free the list which has been allocated
 before.
 Pre:  the pointer list
 Post: nothing
 */

LIST *freeList(LIST *list)
{
    int i;
    
    i = list->size;
    while (--i) {
        free(list->pair[i].word);
    }
    
    free(list->pair);
    free(list);
    return NULL;
}

/************ Output:
 
 
   Arrays, Strings, Structures, Sorting, Pointers, and 
 
   Dynamic Allocation of Memory
 
 This program provides antonyms to common words.
 
 ======  ======
   word  antonym
 ======  ======
    big  small 
    big  little
  large  small 
  large  little
   left  right 
 little  big   
 little  large 
 little  much  
   much  little
  right  left  
  right  wrong 
  small  big   
  small  large 
  wrong  right 
 ======  ======
 
 Please enter a word or "quit" to stop searching: wrong
 Antonyms for "wrong" include:
   right
 
 Please enter a word or "quit" to stop searching: little
 Antonyms for "little" include:
   big
   much
   large
 
 Please enter a word or "quit" to stop searching: bad
 "bad" not found
 
 Please enter a word or "quit" to stop searching: much
 Antonyms for "much" include:
   little
 
 Please enter a word or "quit" to stop searching: quit
 
   Thank you for using the program,
   Have a great day!
*/
