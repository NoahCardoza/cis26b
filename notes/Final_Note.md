## STRING
> #include <string.h>
#### Copy Strings
`char* stpcpy(char* dst, const char* src);`
`char* stpncpy(char* dst, const char* src, size_t len);`


#### String length
`size_t strlen(const char* string);`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; the number of characters that precede the terminating NUL character

#### Searching in string
##### For 1 character
`char* strchr(char* string, int c);`
`char* strrchr(char* string, int c);`

##### For a substring
`char* strstr(char *string, char *substring);`

##### For first character in/not in a set
`size_t strspn(const char *s, const char *charset);`  -> not in charset
`size_t strcspn(const char *s, const char *charset);` -> in charset
`char* strpbrk(char* string, char* charset);` -> in charset

#### Parsing
`char* strtok(char* string, char* delim);`  
`double strtod (const char *s, char **endp);`

#### Converting Numeric Strings to Numbers
`long strtol(const char *restrict str, char **restrict endptr, int base);`   (example in book)


## ARRAYS


##### Copy from array src to array dst
`void* memcpy(void *dst, const void *src, size_t n);`

```c
int a[10] = {10, 10, 10, 999, 999, 999, 999};
int c[10], d[10];

memcpy(c, a, 3 * sizeof (int)); // copy three 10 to c
memcpy(d, a + 3, 4 * sizeof(int)); // copy four 9999
```



## IO

`ssize_t getline(char ** restrict linep, size_t * restrict linecapp, FILE * restrict stream);`

```c
char *line = NULL;
size_t linecap = 120; // max buffer
ssize_t linelen;
while ((linelen = getline(&line, &linecap, fp)) > 0){
    fwrite(line, linelen, 1, stdout);
  }
```
While using getline() to read from input, do not manipulate line. Use another pointer to move inside the line because getline() has to free memory allocated and pointed to by line.

## FILE

### Open and close
```c
#include <stdio.h>
#include <stdlib.h>
FILE *fp = = fopen( "DATA.TXT", "r");
if ( fp == NULL ){
    printf ("Error opening for reading");
    exit(101);
}
// Process
if ( fclose( fp) == EOF ){
    printf ("Error closing!");
    exit(201);
}
```
"r"  - read text data
"w"  - write text data. Overwrite file if file exists
"a"  - append text.
"rb" - read binary data
"wb" - write binary data
"ab" - append binary data to existing binary file
"r+b" – read and write an existing binary file
"w+b" – read and write a new binary file
"a+b" – read and append binary data to existing binary file

### Process
#### Read from file
`int fscanf(FILE *restrict stream, const char *restrict format, ...);`
&nbsp;&nbsp;&nbsp;&nbsp;&nbspm;&nbsp; scan from *stream* according to *format* into char* in ...
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; scan stops when an input character does not match such a format character.
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; return the number of input items assigned
```c
fscanf (stdin, "%d %f", &year, &price);   // example 1
while (fscanf(fpIn, "%c %d %d%*c", &op, &n1, &n2) != EOF )
{ ... }     // *c to discard '\n'   // example 2
```
`char fgets(char *string, int numchars, FILE *fp);`
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; `numchars` can be `sizeof(string)`
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Read from `fp` and stops after `numchars - 1` characters are read. Retains '\n' and '\0' is put after it
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Doesn't do any malloc() so pass `char s[35]` as argument
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Return a pointer to the `string`.  If EOF occurs, return NULL

`ssize_t getline(char** restrict linep, size_t * restrict linecapp,FILE * restrict stream);`
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Reads a line from stream, delimited by '\n'
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; '\n' is included as part of the line, unless the end of the file is reached.
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; return the number of characters written, excluding the terminating NUL character.  The value -1 is
returned if an error occurs, or if end-of-file is reached.
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; if modifying line, use a separate pointer to traverse line.

```c
char *line = NULL;
size_t linecap = 120; // max buffer capacity
while ( (getline(&line, &linecap, fp)) > 0){
      printf("%s\n", line);
}
```



#### Write to file
`int fprintf(FILE * restrict stream, const char * restrict format, ...);`
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; produces output to stream according to a format and variables in ...
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; return the number of characters printed (not including the trailing `\0')


## Void Pointer
The void pointer can be used with any pointer, and any pointer can be assigned to a void pointer.

It may NOT be de-referenced!

## QUICKSORT

PSUEDOCODE (always choose last element as pivot)
```
/* low  --> Starting index of array,  high  --> Ending index */
quickSort(arr[], low, high){
    if (low < high)  {
        pi = partition(arr, low, high);  	   /* pi is partitioning index, arr[pi] is now  at right place */
        quickSort(arr, low, pi - 1);  // Before pi
        quickSort(arr, pi + 1, high); // After pi
    }
}

PSEUDO CODE FOR PARTITION()
/* This function takes last element as pivot, place the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot) to left of pivot and all greater elements to right
   of pivot */
partition (arr[], low, high) {

    pivot = arr[high];			    // pivot (Element to be placed at right position)

    i = (low - 1)  // Index of smaller element

    for (j = low; j <= high- 1; j++) {
        if (arr[j] <= pivot) {
            i++;    					// increment index of smaller element
            swap arr[i] and arr[j]
        }
    }
    swap arr[i + 1] and arr[high])
    return (i + 1)
}
```

##### C's Quick Sort:
```c
#include <stdlib.h>
void qsort(void *start_address,size_t num_elements, size_t element_size,
int (*compare)(const void *key, const void* current_element);  
```

Example:
```c
typedef struct {
    char partname[20];
    int quantity;
}HARDWARE;

HARDWARE inventory[numparts] = {{"A", 10}, {"B", 2}, {"C", 1}};

qsort(inventory, numparts, sizeof(HARDWARE), compare);

int compare(const void *part1, const void *part2){
  const HARDWARE *p1 = part1, *p2 = part2;

  int cmpres = strcmp(p1->partname, p2->partname)

  if (cmpres > 0) return 1;
  else if (cmpres < 0) return -1;
  else return 0;
}
```

## Binary Search
##### Condition: List must be sorted
PSEDOCODE
```c
/*
list must contain at least one element, size is the number of elements in list, target is the value of element being sought. If found: return 1; locn assigned index to target element. If not found: return 0; locn assigned index of element above target
*/

int binarySearch (int list[], int size, int target, int *locn){
  int first = 0, mid, last = size - 1;

  while (first <= last) {
    mid = (first + last) / 2;
    if (target > list[mid]) 	// look in upper half
       first = mid + 1;
    else if (target < list[mid]) // look in lower half
       last = mid - 1;
    else // target found: force exit
       first = last + 1;
  } // end while
  *locn = mid;
  return target == list [mid];
} // binarySearch

```
Example:
```c
#include <stdlib.h>
void *bsearch(void *key, const void *start_address, size_t num_elements, size_t element_size, int (*compare)(const void *key, const void *current_element);

HARDWARE temp;
found = (HARDWARE *)bsearch(&temp, inventory, numparts, sizeof (HARDWARE), compare);

int compare(const void* part1, const void* part2){
    const HARDWARE* p1 = part1, * p2 = part2;

    if (strcmp(p1->partname, p2->partname) > 0) return 1;
    else if (strcmp(p1->partname, p2->partname) < 0) return -1;
    else return 0;
}

```

## Bit
#### To print bit
```c
void printBinary(unsigned short a){
    for(int i = 15; i >= 0; i--)
        printf("%hu", (a >> i) & 1);
}
```
## Misc. Functions
**Give a random number between 0 and max**:
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`int num = rand() % max;`

**Macros:**
```c
#define ASSERT_GENERAL(exp, msg, exe) \
if (!(exp)) {                         \
    printf(msg);                      \
    exe;                              \
}


#define ASSERT_RETURN(exp, msg, status) ASSERT_GENERAL(exp, msg, return status)
#define ASSERT_EXIT(exp, msg, status)   ASSERT_GENERAL(exp, msg, exit(status))
#define ASSERT_BREAK(exp, msg)          ASSERT_GENERAL(exp, msg, break)
#define ASSERT_CONTINUE(exp, msg)       ASSERT_GENERAL(exp, msg, continue)
```

**Remove whitespace from string and convert uppercase letters to lowercase.**
```c
void format(char* s) {
    char* fast = s, * slow = s;

    while (*fast){
        if (!isspace(*fast))  *slow++ = tolower(* fast);
        fast++;
    }
    *slow = '\0';
}
```

## Memory: Stack and Heap

A stack-frame contains four different elements:
Parameters, Local variables, Return value (if any) and Where it should return when its processing is done.

## Command Line Argument
```c
int main( int argc, char* argv[])
{          // argc - count;
           // argv – vector
    int i;
    char** mover;

    for (i = 0; i < argc; i++)
    {
        printf("Argument %d is %s\n", i, argv[i]);
    }
    printf("\n");
    for (mover = argv; *mover != NULL; mover++)
    {
        printf("Argument pointed at by mover is %s\n", * mover);
    }
    return 0;
}
```

## cdecl
Neat program in command line to translate C code to human language
