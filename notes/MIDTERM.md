<!-- replace "*" with * before print -->

# Table Of Contents

* __I/O Operations__
  * printf
  * scanf
  * Flush
* __C Strings__
  * Declarations
  * Functions
* __Style__
  * Whitespace
  * Documentation
  * Comments
  * Functions
  * Functional Cohesion
  * Structured programming
* __Structures__
* __Pointer / Lists__
  * Pointer Arithmetic
  * Lists / Arrays
  * Array Comparing / Copying / Setting
  * Functions
  * Examples
* __Stacks__
  * Basic Operations
  * Applications
* __Queues__
  * Basic Operations
  * Applications
* __Linked Lists__
  * Header Structure / Node
  * Sentinel
* __Memory Management__
* __Warnings Not Errors__
* __Random Function Declarations__

<!-- <p style="page-break-before: always"></p> -->

<p style="page-break-before: always"></p>

# I/O Operations

|     | TEXT FILES                                                | BINARY FILES                                                         |
|:---:| --------------------------------------------------------- | -------------------------------------------------------------------- |
|  1  | Consists of characters.                                   | Consists of internal computer format.                                |
|  2  | Data must be  converted to internal formats.              | No format conversion is necessary (much faster to input and output). |
|  3  | Are organized around lines.                               | Are organized around "records".                                      |
|  4  | Are created and used by either text editors  or programs. | Are created and used by programs only.                               |

| Type Specifier | Type           |
| -------------- | -------------- |
| d              | int            |
| c              | char           |
| s              | char*          |
| hd             | short int      |
| ld             | long int       |
| u              | unsigned int   |
| f              | float / double |
| p              | pointer addy   |

| Modifier Type Specifier | Type  |
| ----------------------- | ----- |
| h                       | short |
| l                       | long  |



### printf

| Modifier | Explanation |
| -------- | ----------- |
| +        | use sign    |
| -        | left aline  |
| {int}    | min width   |
| .{int}f  | round place |
| .{int}s  | char limit  |
| 0        | leading 0's |

### scanf

`scanf` skips leading whitespace with the exception of `%c` but not `_%c` It doesn't matter how many spaces are there between 100 and A:
the only one space between %d and %c is sufficient to discard them all!

| Modifier  | Explanation           |
| --------- | --------------------- |
| *         | discard one           |
| {int}s    | min chars to read     |
| [^{char}] | read until {char}     |
| [{char}]  | read until not {char} |

`fscanf(fpDates, "%d/%d/%d", &year, &month, &day);`

### Flush

```c
#define FLUSH while( getchar() != '\n' )
```

```c
#include <stdio.h>
int fgetc(FILE *stream);
int getc(FILE *stream);  
int fputc(int c, FILE *stream);
int putc(int c, FILE *stream);  
int ungetc(int c, FILE *stream);
int fprintf(FILE *stream, char *format, ...);
int fscanf(FILE *stream, char *format, ...);
```

```c
FILE *f = fopen(FILENAME, MODE); // MODE = "r" | "w" | "a"
if (!f) exit(101);

if (fclose(f) == EOF) exit(201);
// other functions
rewind(f); //	moves the file pointer to the beginning of the file
rename(OLD_FILENAME, NEW_FILENAME); // changes the name of a file, 0 on success
remove(FILENAME); // erases a file, 0 upon success
```

# C Strings

# Declarations

NOTE: Standalone `''` is invalid.

```c
// Constant pointers but memory is settable
char s[6] = {'B', 'e', 'n'};        // B e n _ _ _
char s[6] = {'B', 'e', 'n', '\0'};  // B e n 0 0 0
char s[6] = "Ben";                  // B e n 0 0 0
char s[]  = "Ben";                  // B e n 0

// Moveable pointer but constant memory
char *p   = "Ben";                  // B e n 0
```

### Functions

```c
#include <string.h>
size_t  strlen(char *s); // doens't count `\0`
char    *strcpy(char * dst, char * src); // ignores `\0`
char    *stpncpy(char * dst, char * src, size_t len); // will not add `\0`
char    *strcat(char *s1, char *s2);
char    *strncat(char *s1, char *s2, size_t n); // doesn't guarantee `\0`
int     strcmp(char *s1, char *s2); // s1 - s2
int     strncmp(char *s1, char *s2, size_t n); // up to n characters
char    *strchr(char *s, int c);  // locates the first occurrence of `c` even if `c = 0` returns NULL otherwise
char    *strrchr(char *s, int c); // locates the last occurrence of `c`
#include <stdio.h>
char    *fgets(char *str, int size, FILE *stream); // '\n' replaced by '\0'
char    *gets(char *str); // '\n' kept, '\0' added to end
int     fputs(char *s, FILE *stream); // '\n' not added
int     puts(char *s); // '\n' added
int     sprintf(char *str, char *format, ...);  // write to string
int     sscanf(char *s, char *format, ...); // reads from string
```

# Style

### Whitespace

1.  Write one statement per line.
2.  Use **spaces** around binary operators.
3.  No **spaces** for unary operators.
4.  Leave one blank line between the local definition section and statements section.
5.  Leave one space after comma and no space in front of it.

### Documentation

Each program must begin with a comment to briefly explain
the purpose of the program, the name of the programmer
and the date the program was finished.

```c
/*
  Summer {year}}
  {class}

  {description}

  {firstname} {lastname}
   MM/DD/YYYYY
*/
```

For each function definition clearly document the declarative portion:

```
/* ================================ */
/* {purpose}
  PRE:
    {explanation of all input parameters}
  POST
    {explanation of all output actions and return value}
*/
```

#### Comments

1. Comments should be written in clear English not high-tech jargon.
2. Commenting styles leading to more comment than code detract from program readability.

#### Functions

1. A function's name should contain a verb, such as: getTime, calcFee, printBill
2. Write values passing parameters first and pointer parameters last.
3. When only one value is to be passed back use return.
  + Otherwise use pointers.

##### Functional Cohesion

A measure of how closely the processes in a function are related.

Each function should do only one thing.
In general functions should be relatively short:
 * not more than one page or screen.
 * The one thing a function does, should be done in only one place.

Reasons for using cohesive functions:
 * Correctness
 * Maintainability
 * Reusability

##### Structured programming

One of the structured programming requirements is to write any algorithm
using only three constructs (without using break, continue, and goto).

# Structures

*NOTE:* Use `const` when defining a puts_{struct} function

```c
typedef struct {
    int month;
    int day;
    int year;
} DATE;

typedef struct {
    char   name[21];
    DATE   bdate;   // nested structures
    double avescore;
    char   grade;
} STU;
```

# Pointer / Lists

Never do: `*(array + i)` use `array[i]` instead.

### Pointer Arithmetic

| Operator            | Explanation                                                      |
| ------------------- | ---------------------------------------------------------------- |
| `pointer - pointer` | returns the number of elements in between the pointer addresses. |
| `pointer - const`   | returns the memory location `const` elements behind `pointer`    |
| `pointer + pointer` | error C2110: cannot add two pointers                             |
| `pointer + const`   | returns the memory location `const` elements ahead of `pointer`  |
| `++pointer++`       | increment the memory location                                    |
| `--pointer--`       | decrement the memory location                                    |
| `pointer +=`        | work as expected                                                 |
| `pointer -=`        | work as expected                                                 |
| `*pointer`          | dereference / access the value to which the pointer points       |
| `&pointer`          | reference / access the address of the pointer                    |
| < <= == >= > !=     | only work with pointers of the same type                         |

### Lists / Arrays

> A linear list is a list in which each element has a
unique successor.

Linear lists:
 * Array: a fixed sized collection of elements of the same type.
 * Linked List: is  an ordered collection of data in which each
   element contains the location of the next element.

Non-linear lists:
 * Tree: a list in which each element may have multiple successors and one predecessor, except for one node, that does not have a predecessor, called the root of the tree.
 * Graph: a list in which each element may have multiple successors and multiple predecessors.

|     | Arrays                                       | Linked Lists                                       |
|:---:| -------------------------------------------- | -------------------------------------------------- |
|  1  | Direct Access: `list[i]`                     | "Walk" to the `i`th node                           |
|  2  | **Sorted list** : binary search              | **Sorted list** :  linear search                   |
|  3  | **Insert** : shift elements to make room     | **Insert** : create a new node and update pointers |
|  4  | **Delete** : shift elements to eliminate gap | **Delete** : update pointers and release memory    |

```c
typedef struct node
{
    int          data;
    struct node *next;
} NODE;
```

### Array Comparing / Copying / Setting

#### Functions

```c
#include <strings.h>
void *memcpy(void *dst, void *src, size_t n);
int  memcmp(void *s1, void *s2, size_t n);
void *memset(void *b, int c, size_t len);
```

#### Examples

```c
int a[10] = {10, 10, 10, 999, 999, 999};
int b[10];

memcpy(b, a, 3 * sizeof (int));

int a[10] = {10, 20, 30, 90, 50};
int b[10] = {10, 20, 30, 90, 50};

memcmp(a, b, 10 * sizeof (int))
```

# Stacks

> Last In First Out
> Insert/Delete are restricted to one end called top.

### Basic Operations
 * **Insert** : push
 * **Delete** : pop

### Applications

1. Computer stack
2. Parsing and evaluating arithmetic expressions (compiler)
3. Backtracking (finding a path in a maze – games)
4. Process data in reverse order (such as undo-delete)

# Queues

> First In First Out
> Insert  - end Delete - beginning

### Basic Operations
 * **Insert** : enqueue
 * **Delete** : dequeue

### Applications

1. Operating systems often maintain a queue of processes that are ready to execute or that are waiting for a particular event to occur (print queues, mail queues).
2. Computer systems must often provide a "holding area" for messages between two processes, two programs, or even two systems. This holding area is usually called a "buffer" and is often implemented as a queue.

# Linked Lists

```c
NODE *init_list(void);
int insert(NODE *list, char *s);
int delete(NODE *list, char *s);
NODE *find(NODE *list, char *s);
void traverse(NODE *list);
NODE *destroy(NODE *list);
```

### Header Structure / Node

`LIST -> NODE -> ... -> NULL`

```c
typedef struct t_node {
    int             data;
    struct t_node   *link;
} NODE;

typedef struct {
    int   count;
    NODE *head;
} LIST;
```

Advantages:

* The header of the list contains metadata about the list, such as a counter, a pointer to the first node in the list, etc.
* Since list never changes, it does not have to be updated by insert/delete

### Sentinel

#### Linked List

`LIST -> SENTINEL NODE -> NODE -> ... -> NULL`

In a linked list the sentinel node is at the beginning of the array or pointed
to by the header.

  * Insert/delete are more efficient: no if statement to check if the new node is to be inserted in the beginning.
  * The first or last node in a singly-linked list.
  * It does not carry any data.
  * It is an empty data node.

### Array

`{ VALUE, ... VALUE, SENTINEL VALUE }`

In an array the sentinel is at the end helping you detect the end of
the array without keeping track of indexes.

* Search is more efficient (less testing per iteration)

# Memory Management

```
                                 Stack             Heap
+-----------------------------------------------------------+
| Executable || Global    || Local Variables  || Dynamic    |
| Program    || Variables ||                  || Allocation |
|            ||           || Parameters       || Of Memory  |
|            ||           ||                  ||            |
|            ||           || Stack Allocation ||            |
|            ||           || Of Memory        ||            |
+-----------------------------------------------------------+
              +---------------------------------------------+
                                  Data
```

A **stack-frame** contains four different elements:

* Parameters.
* Local variables.
* Return value (if any).
* Where it should return when its processing is done.

### Memory Functions


```c
// Function Declarations
#include <stdlib.h>
void *malloc(size_t size);               // Allocate `size` bytes
void *calloc(size_t count, size_t size); // Allocate `count * size` bytes and initialize them to 0
void *realloc(void *ptr, size_t size);   // Extend the size of a previously allocated pointer `ptr` to `size` bytes.
void free(void *ptr);                     // Returns memory back to the OS

// Example
int *i;
i = (int*) malloc(5 * sizeof(int)); // allocate an 5 ints in a row
if (!i) {
  fputs(stderr, "An error occurred allocating the required space.") // stdio.h
  exit(3);                                                          // stdlib.h
}
free(i); // make sure to free
```

# Warnings Not Errors

```c
while (p = 6 != 6) { ... }  // p will be set to 6 first
while (p++, 6 != 6) { ... } // but only the value of the right most expression is used
```

# Random Function Declarations

```c
#include <stdio.h>
int getchar(void); // can return `EOF`
#include <ctype.h>
int toupper(int c);
```
