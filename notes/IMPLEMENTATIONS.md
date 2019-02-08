# Implementations

1. Linked List
2. Sentinel Search
3. Stack
4. Queue
5. Get Node

# Linked List

```c
/***************************************************
    Examples: 3.7 - 3.12
    This program initializes, inserts into, deletes
    from, searches, and traverses an ordered linked
    list with a string key.
/***************************************************/

#include <stdio.h>
#include <stdlib.h> // malloc(), free(), exit()
#include <string.h>

#define DUMMY_TRAILER '\177'
// octal ASCII code of the
// last character in the ASCII table

typedef struct node NODE;
struct node {
  char data[20];
  NODE * next;
};

int main(void) {
  NODE * init_list(void);
  int insert(NODE * list, char * data);
  int delete(NODE * list, char * data);
  NODE * find(NODE * list, char * data);
  void traverse(NODE * list);

  NODE * list, * nodeptr;
  char data[20];
  int found, duplicate;

  // build list
  list = init_list();
  while (printf("Enter string(or quit): "), gets(data),
    strcmp(data, "quit")) {
    duplicate = insert(list, data);
    if (duplicate) {
      printf("\"%s\" already in the list!\n", data);
    }
  }
  // print out strings in list nodes
  traverse(list);

  printf("\n\nTesting INSERT: \n");
  while (printf("Enter string(or quit): "), gets(data),
    strcmp(data, "quit")) {
    duplicate = insert(list, data);
    if (duplicate) {
      printf("\"%s\" already in the list!\n", data);
    }
  }
  // print out strings in list nodes
  traverse(list);

  printf("\n\nTesting DELETE:\n");
  while (printf("Enter string(or quit): "), gets(data),
    strcmp(data, "quit")) {
    found = delete(list, data);
    printf("\"%s\" %s\n", data, found ? "deleted!" : "not found!");
  }

  // print out strings in list nodes
  traverse(list);

  printf("\n\nTesting SEARCH:\n");
  while (printf("Enter string(or quit): "), gets(data),
    strcmp(data, "quit")) {
    nodeptr = find(list, data);
    if (nodeptr) {
      printf("\"%s\" found\n", nodeptr - > data);
    } else {
      printf("\"%s\" not found\n", data);
    }

  }

  // memory leak: call the destroy function here
  return 0;
}
/***************************************************
    Initialization of a linked list with two
    sentinel nodes
*/
NODE * init_list(void) {
  NODE * list;

  // allocate the first sentinel node
  list = (NODE * ) malloc(sizeof(NODE));
  if (!list) {
    printf("Error in init_list!\n");
    exit(1);
  }
  list - > data[0] = '\0';

  // allocate the last sentinel node
  list - > next = (NODE * ) malloc(sizeof(NODE));
  if (!(list - > next)) {
    printf("Error in init_list!\n");
    exit(2);
  }
  list - > next - > data[0] = DUMMY_TRAILER;
  list - > next - > data[1] = '\0';
  list - > next - > next = NULL;

  return list;
}

/***************************************************
    Insert a node in a sorted linked list
        return 1 - if duplicate
        return 0 - otherwise
*/
int insert(NODE * list, char * data) {
  NODE * curr = list - > next;
  NODE * prev = list;
  NODE * pnew;
  int duplicate = 1;

  // search
  while (strcmp(data, curr - > data) > 0) {
    prev = curr;
    curr = curr - > next;
  }

  if (strcmp(data, curr - > data)) {
    duplicate = 0; // not a duplicate
    pnew = (NODE * ) malloc(sizeof(NODE));
    if (!pnew) {
      printf("Fatal memory allocation error in insert!\n");
      exit(3);
    }
    strcpy(pnew - > data, data);
    pnew - > next = curr;
    prev - > next = pnew;
  }
  return duplicate;
}

/***************************************************
    Delete a node from a sorted linked list
        return 0 - not found
        return 1 - found and deleted
*/
int delete(NODE * list, char * data) {
  NODE * curr = list - > next;
  NODE * prev = list;
  int found = 0;

  // search
  while (strcmp(data, curr - > data) > 0) {
    prev = curr;
    curr = curr - > next;
  }
  if (strcmp(data, curr - > data) == 0) {
    found = 1;
    prev - > next = curr - > next;
    free(curr);
  }
  return found;
}

/***************************************************
    Find a node in a sorted linked list
        return NULL - not found
        return pointer to the node - found
*/
NODE * find(NODE * list, char * data) {
  list = list - > next; // skip the dummy node
  while (strcmp(data, list - > data) > 0) {
    list = list - > next;
  }
  return strcmp(data, list - > data) == 0 ? list : NULL;
}

/***************************************************
    Traverse a linked list to print out the
    contents of each node
*/
void traverse(NODE * list) {
  list = list - > next; // skip the dummy node
  while (list - > data[0] != DUMMY_TRAILER) {
    printf("%s\n", list - > data);
    list = list - > next;
  }
  return;
}
```

# Sentinel Search

```c
/*  Knuth (the author of "The Art of Computer Programming") states:

    "When the inner loop of a program tests two or more conditions, we should
    try to reduce the testing to just one condition."  

    To eliminate the test for the end of the list we copy the target at the end
    of the list (the target is now a "sentinel"). We determine after the loop
    completes whether we found actual data or the sentinel.
*/

#include <stdio.h>

#define MAX_ARY_SIZE 100

int main (void){
    int *sentinel_search (int  *list, int n, int target);

    int	ary[MAX_ARY_SIZE + 1] = { 89, 72,  3, 15, 21};
                       // ^^^ + 1 for the sentinel value
	int size = 5;
	//int target = 15;
	int target = 10;
	int *ploc;

	if (ploc = sentinel_search (ary, size, target))
	    printf("%3d found at location:       %2d\n", *ploc, ploc - ary);
	else
	    printf("%3d NOT found!\n", target);

	printf("End of search.\n");
	return 0;
}

/*************************************************************
	Sentinel Search: Locate the target in an unordered list
	of n elements. Returns NULL if not found; returns pointer
	to the first element equal to the target value if found.
*/
int *sentinel_search (int  *list, int  n, int  target){
	int *psent = list + n;
	int *ploc = list;

    *psent = target;  // copy target(sentinel) at the end of the list
	while (target != *ploc)
	   ploc++;
    return ploc == psent ? NULL : ploc;
}
```

# Stacks

```c
/***************************************************
    Example 3.6
    Driver program for stack functions
/***************************************************/

#include <stdio.h>
#include <stdlib.h>

  typedef struct node NODE;
struct node {
  int data;
  struct node * next;
};

int main(void) {
  NODE * push(NODE * stack, int data);
  NODE * pop(NODE ** stack);

  NODE * stack = NULL;
  NODE * top;

  stack = push(stack, 3);
  stack = push(stack, 4);
  stack = push(stack, 5);

  top = pop( & stack);
  if (top) {
    printf("Top node of stack is %d\n", top - > data);
    free(top);
  } else printf("Empty stack!\n");

  top = pop( & stack);
  if (top) {
    printf("Top node of stack is %d\n", top - > data);
    free(top);
  } else printf("Empty stack!\n");

  top = pop( & stack);
  if (top) {
    printf("Top node of stack is %d\n", top - > data);
    free(top);
  } else printf("Empty stack!\n");

  top = pop( & stack);
  if (top) {
    printf("Top node of stack is %d\n", top - > data);
    free(top);
  } else printf("Empty stack!\n");

  return 0;
}
/***************************************************
    Stack Insert: insert in the beginning
*/
NODE * push(NODE * stack, int data) {
  NODE * pnew;

  pnew = (NODE * ) malloc(sizeof(NODE));
  if (!pnew) {
    printf("... error in push!\n");
    exit(1);
  }
  pnew - > data = data;
  pnew - > next = stack;
  stack = pnew;

  return stack;
}

/***************************************************
    Stack Delete: delete the first node
*/
NODE * pop(NODE ** stack) {
  NODE * first;

  if ( * stack == NULL) return NULL;
  first = * stack;
  * stack = ( * stack) - > next;
  first - > next = NULL;

  return first;
}
```

# Queue

```c
/***************************************************
    Example 3.6
    Driver program for queue functions
/***************************************************/

#include <stdio.h>
#include <stdlib.h>

  typedef struct node NODE;
struct node {
  int data;
  struct node * next;
};

int main(void) {
  NODE * dequeue(NODE ** queue, NODE ** rear);
  void enqueue(NODE ** queue, NODE ** rear, int data);

  NODE * queue = NULL, * rear = NULL;
  NODE * front;

  enqueue( & queue, & rear, 3);
  enqueue( & queue, & rear, 4);
  enqueue( & queue, & rear, 5);

  front = dequeue( & queue, & rear);
  if (front) {
    printf("Front node of queue is %d\n", front - > data);
    free(front);
  } else printf("Empty queue!\n");

  front = dequeue( & queue, & rear);
  if (front) {
    printf("Front node of queue is %d\n", front - > data);
    free(front);
  } else printf("Empty queue!\n");

  front = dequeue( & queue, & rear);
  if (front) {
    printf("Front node of queue is %d\n", front - > data);
    free(front);
  } else printf("Empty queue!\n");

  front = dequeue( & queue, & rear);
  if (front) {
    printf("Front node of queue is %d\n", front - > data);
    free(front);
  } else printf("Empty queue!\n");

  return 0;
}
/***************************************************
    Queue Insert: insert at the end
*/
void enqueue(NODE ** queue, NODE ** rear, int data) {
  NODE * pnew;

  pnew = (NODE * ) malloc(sizeof(NODE));
  if (!pnew) {
    printf("... error in enqueue!\n");
    exit(1);
  }
  pnew - > data = data;
  pnew - > next = NULL;
  if ( * queue == NULL) * queue = pnew;
  else( * rear) - > next = pnew;
  * rear = pnew;
  return;
}

/***************************************************
    Queue Delete: remove the first node
*/
NODE * dequeue(NODE ** queue, NODE ** rear) {
  NODE * first;

  if ( * queue == NULL) return NULL;
  first = * queue;
  * queue = ( * queue) - > next;
  if ( * queue == NULL) * rear = NULL;
  first - > next = NULL;

  return first;
}
```

# Get Node

```c
/***************************************************
    Examples: 3.14
    Programmer-Controlled Memory Management
/***************************************************/

#include <stdio.h>
#include <stdlib.h>  // malloc(), free(), exit()
#include <string.h>

#define DUMMY_TRAILER '\177'
                // octal ASCII code of the
                // last character in the ASCII table

typedef struct node NODE;
struct node
{
    char  data[20];
    NODE *next;
};

int main (void){
    NODE *get_node(NODE **stack); // Programmer-Controlled Memory Management

    //NODE *init_list(void);
    NODE *init_list(NODE **stack);
    //int   insert(NODE *list, char *data);
    int   insert(NODE *list, NODE **stack, char *data);
    //int   delete(NODE *list, char *data);
    int   delete(NODE *list, NODE **stack, char *data);
    void  traverse(NODE *list);

    NODE *stack = NULL;   // to keep track of the deleted nodes
    NODE *list, *nodeptr;
    char data[20];
    int  found, duplicate;

    // build list
    //list = init_list();
    list = init_list(&stack);
    while (printf("Enter string(or quit): "), gets(data),
           strcmp(data, "quit")){
        //duplicate = insert(list, data);
        duplicate = insert(list, &stack, data);
        if(duplicate){
            printf("\"%s\" already in the list!\n", data);
        }
    }
    // print out strings in list nodes
    traverse(list);

    printf("\n\nTesting DELETE:\n");
    while (printf("Enter string(or quit): "), gets(data),
           strcmp(data, "quit")){
        //found = delete(list, data);
        found = delete(list, &stack, data);
        printf("\"%s\" %s\n", data, found ? "deleted!" : "not found!");
    }

    // print out strings in list nodes
    traverse(list);

    printf("\n\nTesting INSERT: \n");
    while (printf("Enter string(or quit): "), gets(data),
           strcmp(data, "quit")){
        //duplicate = insert(list, data);
        duplicate = insert(list, &stack, data);
        if(duplicate){
            printf("\"%s\" already in the list!\n", data);
        }
    }
    // print out strings in list nodes
    traverse(list);

    printf("\n\nTesting DELETE again:\n");
    while (printf("Enter string(or quit): "), gets(data),
           strcmp(data, "quit")){
        //found = delete(list, data);
        found = delete(list, &stack, data);
        printf("\"%s\" %s\n", data, found ? "deleted!" : "not found!");
    }

    // print out strings in list nodes
    traverse(list);
    // memory leak: call the destroy function here
    return 0;
}
/***************************************************
    Gets a free node from the free stacl (if not
    empty) or from a malloc'ed block of structs
*/
NODE *get_node(NODE **stack){
    //#define BLOCKSIZE 100
  #define BLOCKSIZE 3

    static NODE *block = NULL, *blockrear;
    NODE *first;

    if (*stack != NULL) // pop node off the free stack
    {
        first = *stack;
        *stack = (*stack)->next;
        first->next = NULL; // detach from stack
    }
    else if (block != NULL) // get node from block
    {
        first = block;
        if (block == blockrear) block = NULL;
        else block++;   // move to the next struct
    }
    else // stack and block are NULL!
    {
        if ((block = (NODE*) malloc(BLOCKSIZE * sizeof(NODE))) == NULL){
            printf("Fatal malloc error in get_node!\n");
            exit(1);
        }
        memset(block, 0, BLOCKSIZE * sizeof(NODE));
        blockrear = block + BLOCKSIZE - 1;
        first = block;
        block++;
    }
    return first;
}


/***************************************************
    Initialization of a linked list with two
    sentinel nodes
*/
//NODE *init_list(void)
NODE *init_list(NODE **stack){
    NODE *list;

    // allocate the first sentinel node
    /*
    list = (NODE*) malloc(sizeof (NODE));
    if (!list){
        printf("Error in init_list!\n");
        exit(1);
    }
    */
    list = get_node(stack);
    list->data[0] = '\0';

    // allocate the last sentinel node
    /*
    list->next = (NODE*) malloc(sizeof (NODE));
    if (!(list->next)){
        printf("Error in init_list!\n");
        exit(2);
    }
    */
    list->next = get_node(stack);
    list->next->data[0] = DUMMY_TRAILER;
    list->next->data[1] = '\0';
    list->next->next = NULL;

    return list;
}

/***************************************************
    Insert a node in a sorted linked list
        return 1 - if duplicate
        return 0 - otherwise
*/
//int insert(NODE *list, char *data)
int   insert(NODE *list, NODE **stack, char *data){
    NODE *curr = list->next;
    NODE *prev = list;
    NODE *pnew;
    int   duplicate = 1;

    // search
    while (strcmp(data,curr->data) > 0){
        prev = curr;
        curr = curr->next;
    }

    if (strcmp(data, curr->data)){
        duplicate = 0; // not a duplicate
        /*
        pnew = (NODE*) malloc(sizeof (NODE));
        if (!pnew){
            printf("Fatal memory allocation error in insert!\n");
            exit(3);
        }
        */
        pnew = get_node(stack);
        strcpy(pnew->data, data);
        pnew->next = curr;
        prev->next = pnew;
    }
    return duplicate;
}

/***************************************************
    Delete a node from a sorted linked list
        return 0 - not found
        return 1 - found and deleted
*/
//int delete(NODE *list, char *data)
int   delete(NODE *list, NODE **stack, char *data){
    NODE *curr = list->next;
    NODE *prev = list;
    int   found = 0;

    // search
    while (strcmp(data, curr->data) > 0){
        prev = curr;
        curr = curr->next;
    }
    if (strcmp(data, curr->data) == 0){
        found = 1;
        prev->next = curr->next;
        //free(curr);         // DO NOT FREE!
        curr->next = *stack;  // push the node onto the free stack!
        *stack = curr;
    }
    return found;
}

/***************************************************
    Traverse a linked list to print out the
    contents of each node
*/
void traverse(NODE *list){
    list = list->next; // skip the dummy node
    while (list->data[0] != DUMMY_TRAILER){
        printf("%s\n", list->data);
        list = list->next;
    }
    return;
}
```

# Get Node

```c
/***************************************************
    Examples: 3.14
    Programmer-Controlled Memory Management
/***************************************************/

#include <stdio.h>
#include <stdlib.h> // malloc(), free(), exit()
#include <string.h>

#define DUMMY_TRAILER '\177'
// octal ASCII code of the
// last character in the ASCII table

typedef struct node NODE;
struct node {
  char data[20];
  NODE * next;
};

int main(void) {
  NODE * get_node(NODE ** stack); // Programmer-Controlled Memory Management

  //NODE *init_list(void);
  NODE * init_list(NODE ** stack);
  //int   insert(NODE *list, char *data);
  int insert(NODE * list, NODE ** stack, char * data);
  //int   delete(NODE *list, char *data);
  int delete(NODE * list, NODE ** stack, char * data);
  void traverse(NODE * list);

  NODE * stack = NULL; // to keep track of the deleted nodes
  NODE * list, * nodeptr;
  char data[20];
  int found, duplicate;

  // build list
  //list = init_list();
  list = init_list( & stack);
  while (printf("Enter string(or quit): "), gets(data),
    strcmp(data, "quit")) {
    //duplicate = insert(list, data);
    duplicate = insert(list, & stack, data);
    if (duplicate) {
      printf("\"%s\" already in the list!\n", data);
    }
  }
  // print out strings in list nodes
  traverse(list);

  printf("\n\nTesting DELETE:\n");
  while (printf("Enter string(or quit): "), gets(data),
    strcmp(data, "quit")) {
    //found = delete(list, data);
    found = delete(list, & stack, data);
    printf("\"%s\" %s\n", data, found ? "deleted!" : "not found!");
  }

  // print out strings in list nodes
  traverse(list);

  printf("\n\nTesting INSERT: \n");
  while (printf("Enter string(or quit): "), gets(data),
    strcmp(data, "quit")) {
    //duplicate = insert(list, data);
    duplicate = insert(list, & stack, data);
    if (duplicate) {
      printf("\"%s\" already in the list!\n", data);
    }
  }
  // print out strings in list nodes
  traverse(list);

  printf("\n\nTesting DELETE again:\n");
  while (printf("Enter string(or quit): "), gets(data),
    strcmp(data, "quit")) {
    //found = delete(list, data);
    found = delete(list, & stack, data);
    printf("\"%s\" %s\n", data, found ? "deleted!" : "not found!");
  }

  // print out strings in list nodes
  traverse(list);
  // memory leak: call the destroy function here
  return 0;
}
/***************************************************
    Gets a free node from the free stacl (if not
    empty) or from a malloc'ed block of structs
*/
NODE * get_node(NODE ** stack) {
  //#define BLOCKSIZE 100
  #
  define BLOCKSIZE 3

  static NODE * block = NULL, * blockrear;
  NODE * first;

  if ( * stack != NULL) // pop node off the free stack
  {
    first = * stack;
    * stack = ( * stack) - > next;
    first - > next = NULL; // detach from stack
  } else if (block != NULL) // get node from block
  {
    first = block;
    if (block == blockrear) block = NULL;
    else block++; // move to the next struct
  } else // stack and block are NULL!
  {
    if ((block = (NODE * ) malloc(BLOCKSIZE * sizeof(NODE))) == NULL) {
      printf("Fatal malloc error in get_node!\n");
      exit(1);
    }
    memset(block, 0, BLOCKSIZE * sizeof(NODE));
    blockrear = block + BLOCKSIZE - 1;
    first = block;
    block++;
  }
  return first;
}

/***************************************************
    Initialization of a linked list with two
    sentinel nodes
*/
//NODE *init_list(void)
NODE * init_list(NODE ** stack) {
  NODE * list;

  // allocate the first sentinel node
  /*
  list = (NODE *) malloc(sizeof (NODE));
  if (!list)
  {
      printf("Error in init_list!\n");
      exit(1);
  }
  */
  list = get_node(stack);
  list - > data[0] = '\0';

  // allocate the last sentinel node
  /*
  list->next = (NODE *) malloc(sizeof (NODE));
  if (!(list->next))
  {
      printf("Error in init_list!\n");
      exit(2);
  }
  */
  list - > next = get_node(stack);
  list - > next - > data[0] = DUMMY_TRAILER;
  list - > next - > data[1] = '\0';
  list - > next - > next = NULL;

  return list;
}

/***************************************************
    Insert a node in a sorted linked list
        return 1 - if duplicate
        return 0 - otherwise
*/
//int insert(NODE *list, char *data)
int insert(NODE * list, NODE ** stack, char * data) {
  NODE * curr = list - > next;
  NODE * prev = list;
  NODE * pnew;
  int duplicate = 1;

  // search
  while (strcmp(data, curr - > data) > 0) {
    prev = curr;
    curr = curr - > next;
  }

  if (strcmp(data, curr - > data)) {
    duplicate = 0; // not a duplicate
    /*
    pnew = (NODE *) malloc(sizeof (NODE));
    if (!pnew)
    {
        printf("Fatal memory allocation error in insert!\n");
        exit(3);
    }
    */
    pnew = get_node(stack);
    strcpy(pnew - > data, data);
    pnew - > next = curr;
    prev - > next = pnew;
  }
  return duplicate;
}

/***************************************************
    Delete a node from a sorted linked list
        return 0 - not found
        return 1 - found and deleted
*/
//int delete(NODE *list, char *data)
int delete(NODE * list, NODE ** stack, char * data) {
  NODE * curr = list - > next;
  NODE * prev = list;
  int found = 0;

  // search
  while (strcmp(data, curr - > data) > 0) {
    prev = curr;
    curr = curr - > next;
  }
  if (strcmp(data, curr - > data) == 0) {
    found = 1;
    prev - > next = curr - > next;
    //free(curr);         // DO NOT FREE!
    curr - > next = * stack; // push the node onto the free stack!
    * stack = curr;
  }
  return found;
}

/***************************************************
    Traverse a linked list to print out the
    contents of each node
*/
void traverse(NODE * list) {
  list = list - > next; // skip the dummy node
  while (list - > data[0] != DUMMY_TRAILER) {
    printf("%s\n", list - > data);
    list = list - > next;
  }
  return;
}
```
