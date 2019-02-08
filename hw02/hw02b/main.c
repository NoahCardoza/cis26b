/*********************************************************************************
 ** CIS 26B: Advanced C
 ******************
 ** Homework 2B:
 **        A Circularly Doubly Linked List of Stacks
 **********************************************************************************
 This program reads location / temperature data, formats the data and allows the user to search the entries
 
 Save the output as a comment at the end of the program.
 **********************************************************
 **  Written By:    Noah Cardoza
 **  IDE:           Xcode
 ***************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#ifdef _MSC_VER
#include <crtdbg.h>  // needed to check for memory leaks (Windows only!)
#endif

#define DEFAULT_INPUT_FILE "temperatures.txt"
#define EXIT_KEYWORD "quit"


#define OFFSET(TYPE, PROPERTY) ((size_t)&(((TYPE*)0)->PROPERTY))
#define ACCESS_BY_OFFSET(type, ptr, offset) (*((type **)(((char *) ptr) + offset)))
#define TRAVERSE_FORWARD  OFFSET(L_NODE, next)
#define TRAVERSE_BACKWARD OFFSET(L_NODE, prev)

typedef struct l_node L_NODE; // List Node
typedef struct s_node S_NODE; // Stack Node

struct l_node {
    char    *location;
    int     tempNodes;
    int     totalTemp;
    S_NODE *temperatures;
    L_NODE *prev;
    L_NODE *next;
};

struct s_node {
    int data;
    S_NODE *next;
};

// Utils

/*  ========================================================
    Allocates memory and terminates the program if the
    required space is not found
 Pre
    size       : how many bytes to malloc
    *traceback : function name to be dispalyed with the
                 error message
 Post
    Size-many bytes of contiguous memory
 */

void *myalloc(size_t size, char *traceback) {
    void *ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Error allocating memory in: %s\n", traceback);
        perror("");
        exit(errno);
    }
    return ptr;
}

/*  ========================================================
    A wrapper for the getline function
 Pre
    *fd   : pointer to file object
    *def  : default string to be returned if nothing is
            entered by the user
 Post
    A dynamically allocated c-string of either user input or
    the default c-string
 */

char *getlined(FILE *fd, char *def) {
    char *ret = NULL;
    size_t buffer = 32;
    size_t chars;
    
    chars = getline(&ret, &buffer, fd);
    ret[chars - 1] = 0;
    
    if (!*ret) {
        free(ret);
        ret = myalloc(strlen(def) + 1, "getlined");
        strcpy(ret, def);
    }
    
    return ret;
}

// Stack Functions

/*  ========================================================
    Initiates an S_NODE (stack node)
 Pre
    N/A
 Post
    A dynamically allocated S_NODE with properties
    initialized to 0 or NULL
 */

S_NODE *init_snode(){
    S_NODE *node;
    
    node = (S_NODE *) myalloc(sizeof(S_NODE), "init_snode");
    node->data = 0;
    node->next = NULL;
    
    return node;
}

/*  ========================================================
    Pushes an S_NODE onto the top of another S_NODE
 Pre
    *stack : the top of the stack
    data   : the data to store in the new top node
 Post
    A dynamically allocated S_NODE pushed to the top of the
    stack
 */

S_NODE *push(S_NODE *stack, int data){
    S_NODE *node = init_snode();
    
    node->data = data;
    node->next = stack;
    
    return node;
}

/*  ========================================================
    Frees the whole stack when given the tail node
 Pre
    **stack : the tail of the stack
 Post
    Frees all the nodes used on the stack
 */

void free_snodes(S_NODE **node){
    S_NODE *next = *node;
    while (*node) {
        next = (*node)->next;
        free(*node);
        *node = next;
    }
}

// Linked List Functions

/*  ========================================================
    Initiates an L_NODE (list node)
 Pre
    N/A
 Post
    A dynamically allocated L_NODE with properties
    initialized to 0 or NULL
 */

L_NODE *init_lnode(){
    L_NODE *node;
    
    node = (L_NODE *) myalloc(sizeof(L_NODE), "init_lnode");
    node->location = NULL;
    node->temperatures = NULL;
    node->tempNodes = 0;
    node->totalTemp = 0;
    node->prev = NULL;
    node->next = NULL;
    
    return node;
}

/*  ========================================================
    Initiates a sentinel L_NODE
 Pre
    N/A
 Post
    A dynamically allocated L_NODE with properties
    initialized in order to be a sentinel node
 */

L_NODE *init_list(){
    L_NODE *list = init_lnode();
    
    list->prev = list;
    list->next = list;
    
    return list;
}

/*  ========================================================
    Frees the hole stack when given the tail node
 Pre
    *list     : the sentinel node
    *location : the location at which the temperature was
                recorded
    temp      : the temperature to save
 Post
    Inserted the location and temp in a sorted manor either pushing
    to and existing node or allocating a new one
 */

void insert_sort(L_NODE *list, char *location, int temp) {
    int test = 1;
    L_NODE *newnode;
    list = list->next;
    
    while (list->location && (test = strcmp(list->location, location)) < 0) list = list->next;
    if (!test) {
        free(location);
        list->tempNodes += 1;
        list->totalTemp += temp;
        list->temperatures = push(list->temperatures, temp);
    } else {
        // create new node
        newnode = init_lnode();
        newnode->location = location;
        newnode->tempNodes += 1;
        newnode->totalTemp += temp;
        newnode->temperatures = push(newnode->temperatures, temp);
        
        // splice list
        newnode->prev = list->prev;
        newnode->next = list;
        list->prev->next = newnode;
        list->prev = newnode;
    }
}

/*  ========================================================
    Prints and formats the data in an L_NODE (including the
    inner S_NODE)
 Pre
    *node : a pointer to the node holding the data
 Post
    Prints out the data within the node to the prompt
 */

void put_lnode(L_NODE *node) {
    S_NODE *stack;
    stack = node->temperatures;
//    printf("%-30s {%6.2f} ->", node->location, ((double) node->totalTemp) / node->tempNodes);
    printf("{location: \"%s\", averageTemperature: %.f, recordedTemperatures: [", node->location, ((double) node->totalTemp) / node->tempNodes);
    while (stack) {
        printf("%d", stack->data);
        stack = stack->next;
        if (stack) printf(", ");
    }
    printf("]}\n");
}

/*  ========================================================
    Frees a singular node
 Pre
    **node : a pointer to the node to be freed
 Post
    Frees a singular node and the data contained within it
 */

void free_lnode(L_NODE **node){
    free((*node)->location);
    (*node)->location = NULL;
    free_snodes(&(*node)->temperatures);
    (*node)->next = NULL;
    (*node)->prev = NULL;
    free(*node);
    *node = NULL;
}

/*  ========================================================
    Frees a whole list
 Pre
    *list : a pointer to sentinel node in the list
 Post
    Frees all the nodes in the linked list
 */

void free_list(L_NODE *list){
    L_NODE *next = NULL;
    list = list->next;
    while (list->location) {
        next = list->next;
        free_lnode(&list);
        list = next;
    }
    free_lnode(&next);
}

/*  ========================================================
    Loops through and prints all the nodes in the linked list
 Pre
    *list  : a pointer to sentinel node in the list
    offest : a number used to select the which propertie
             in which the pointer to the next node is stored
 Post
    Prints all the nodes in a list
 */

void traverse(L_NODE *list, size_t offset) {
//    puts("Location\t \t \t \t \t\tAve. Temp\t Recorded Temps");
    do
        put_lnode(list = ACCESS_BY_OFFSET(L_NODE, list, offset));
    while (ACCESS_BY_OFFSET(L_NODE, list, offset)->location);
}

//void traverse_next(L_NODE *list) {
//    do put_lnode(list = list->next); while (list->next->location);
//}
//
//void traverse_prev(L_NODE *list) {
//    do put_lnode(list = list->prev); while (list->prev->location);
//}

// Project Functions

/*  ========================================================
    Handles all the user data
 Pre
    N/A
 Post
    Collects all the user data and returns a linked list
    containing temperature data
 */

L_NODE *read_data() {
    size_t MAX_LOCATION_LEN = 64, chars;
    char *ifile, *line = NULL;
    L_NODE *list;
    FILE *fin;
    int temp;

    printf("Path to input: ");
    ifile = getlined(stdin, DEFAULT_INPUT_FILE);
    fin = fopen(ifile, "r");
    free(ifile);
    
    if (!fin) {
        perror("An error occured while opening the input file");
        exit(errno);
    }
    
    list = init_list();
    
    while ((chars = getdelim(&line, &MAX_LOCATION_LEN, ':', fin)) > 0){
        line[chars - 1] = 0;
        fscanf(fin, "%d", &temp);
        insert_sort(list, line, temp);
        line = NULL;
        if (fgetc(fin) == EOF) break;
    }
    
    fclose(fin);
    
    return list;
}

/*  ========================================================
    Traverses the linked list looking for a node by location
 Pre
    *list     : sentinel node to the linked list
    *location : location to check against each node while
                looking for a match
 Post
    Return the node whose location property matched the
    location argument else return NULL
 */

L_NODE *seach_list_by_location(L_NODE *list, char *location){
    int test;
    
    do {
        if ((test = strcmp((list = list->next)->location, location)) >= 0 && test == 0) return list;
    } while (list->next->location);
    return NULL;
}

/*  ========================================================
    Creates a loop letting the user look up data with a city
    state combo
 Pre
    *list     : sentinel node to the linked list
 Post
    Either prints the information for the city state combo
    or an error message if the record cannot be found
 */

void ui_search_loop(L_NODE *list) {
    L_NODE *found;
    char *line = NULL;
    
    puts("Search records by state/city. Type \"" EXIT_KEYWORD "\" or press RETURN to exit.");
    while (42) {
        printf("> ");
        line = getlined(stdin, EXIT_KEYWORD);
        if (!strcmp(line, EXIT_KEYWORD)){
            free(line); // free the pointer before we loose it
            break;
        } else {
            if ((found = seach_list_by_location(list, line))) {
                put_lnode(found);
                free(line);
                line = NULL;
            } else {
                fprintf(stderr, "Records for the city \"%s\" could not be found.\n", line);
            }
        }
    }
}

int main()
{
    // Initalize and read data
    L_NODE *list = read_data();
    
    // Output
    puts("\nData in ascending order:");
    traverse(list, TRAVERSE_FORWARD);
    puts("\nData in descending order:");
    traverse(list, TRAVERSE_BACKWARD);
    
    // Search Loop
    puts("");
    ui_search_loop(list);
    puts("Bye!");
    
    // Free
    free_list(list);
    
    #ifdef _MSC_VER
        printf( _CrtDumpMemoryLeaks() ? "Memory Leak\n" : "No Memory Leak\n");
    #endif
    
    return 0;
}

