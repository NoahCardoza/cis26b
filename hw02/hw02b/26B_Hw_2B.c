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

#define OUTPUT_JSON          1
#define OUTPUT_JS            2
#define OUTPUT_USER_FRIENDLY 3

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
     *node  : a pointer to the node holding the data
     format : an integer value determining the format in which
              to be displayed
 Post
    Prints out the data within the node to the prompt without a newline
 */

void plop_lnode(L_NODE *node, int format) {
    //    char start = '<', end  = '>', *delem = "";
    char start, end, *delem;
    S_NODE *stack = NULL;
    
    
    if (format == OUTPUT_JSON) {
        start = '{';
        end = '}';
        delem = "\"";
    } else if (format == OUTPUT_JS) {
        start = '(';
        end = ')';
        delem = "";
    } else if (format == OUTPUT_USER_FRIENDLY) {
        printf("(location: %-30s | averageTemperature: %6.2f | lastRecordedTemperature: %-3d)", node->location, ((double) node->totalTemp) / node->tempNodes, node->temperatures->data);
        while (stack) {
            printf("%d", stack->data);
            stack = stack->next;
            if (stack) printf(", ");
        }
        return;
    } else return; // silences warnings of uninitialized variables
    
    stack = node->temperatures;
    putchar(start);
    printf("%1$slocation%1$s: %1$s%2$s%1$s, %1$saverageTemperature%1$s: %3$.2f, %1$srecordedTemperatures%1$s: [", delem, node->location, ((double) node->totalTemp) / node->tempNodes);
    while (stack) {
        printf("%d", stack->data);
        stack = stack->next;
        if (stack) printf(", ");
    }
    printf("]%c", end);
}

/*  ========================================================
     Prints and formats the data in an L_NODE (including the
     inner S_NODE)
 Pre
     *node  : a pointer to the node holding the data
     format : an integer value determining the format in which
              to be displayed
 Post
    Prints out the data within the node to the prompt with a newline
 */

void put_lnode(L_NODE *node, int mode) {
    plop_lnode(node, mode);
    printf("\n");
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
    format : an integer value determining the format in which
             to be displayed
    offest : a number used to select the which propertie
             in which the pointer to the next node is stored
 Post
    Prints all the nodes in a list
 */

void traverse(L_NODE *list, int format, size_t offset) {
    do
        put_lnode(list = ACCESS_BY_OFFSET(L_NODE, list, offset), format);
    while (ACCESS_BY_OFFSET(L_NODE, list, offset)->location);
}

/*  ========================================================
    Loops and calls `callback` on each nodes in the linked list
 Pre
    *list                 : a pointer to sentinel node in
                            the list
    (*callback)(L_NODE *) : function to be called with each
                            list item
    offest                : a number used to select the
                            which propertie in which the
                            pointer to the next node is
                            stored
Post
 Applies `callback` to all the items in the list
 */

void foreach(L_NODE *list, void (*callback)(L_NODE *), size_t offset) {
    do
        callback(list = ACCESS_BY_OFFSET(L_NODE, list, offset));
    while (ACCESS_BY_OFFSET(L_NODE, list, offset)->location);
}

// Project Functions

/*  ========================================================
    Handles all the user data
 Pre
    N/A
 Post
    Collects all the user data and returns a linked list
    containing temperature data
 */

L_NODE *read_data(char *defaultLocation) {
    size_t MAX_LOCATION_LEN = 100, chars;
    char *ifile, *line = NULL;
    L_NODE *list;
    FILE *fin;
    int temp;
    
    if (!defaultLocation) printf("Path to input: ");
    ifile = defaultLocation ? defaultLocation : getlined(stdin, DEFAULT_INPUT_FILE);
    fin = fopen(ifile, "r");
    if (!defaultLocation) free(ifile);
    
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
        if (!strcmp(line, EXIT_KEYWORD)) break;
        else {
            if ((found = seach_list_by_location(list, line)))
                put_lnode(found, OUTPUT_USER_FRIENDLY);
            else
                fprintf(stderr, "Records for the city \"%s\" could not be found.\n", line);
        }
        free(line);
        line = NULL;
    }
    free(line);
    line = NULL;
}

/*  ========================================================
    Formats output of a linked list item in JSON
 Pre
    *list : an link in the list (not a sentinel node)
 Post
    Indent and display the details of a singular node in JSON
 */

void json_format_list_item(L_NODE *list) {
    if (list) {
        printf("\t");
        plop_lnode(list, OUTPUT_JSON);
    }
}

/*  ========================================================
    Formats output of a linked list item in JSON with a new
    line (optional)
 Pre
    *list  : an link in the list (not a sentinel node)
    ifendl : 0 or 1 deciding if a newline is needed
 Post
    Indent and display the details of a singular node in JSON
    maybe with a new line
 */

void json_format_list_item_oendl(L_NODE *list, int ifendl) {
    json_format_list_item(list);
    if (list && ifendl) printf(",\n");
}

/*  ========================================================
 Formats output of a linked list item in JSON with a new line
 Pre
    *list : an link in the list (not a sentinel node)
 Post
    Indent and display the details of a singular node in JSON
    with a new line
 */

void json_format_list_item_endl(L_NODE *list) {
    json_format_list_item(list);
    printf(",\n");
}


/*  ========================================================
 Formats output of the linked list in JSON
 Pre
    *list       : sentinel node to the linked list
    **locations : a list of locations to search for
    and display – if empty, print all
    nodes
 Post
    Either prints the information for the city state combos
    in the locations list or all the nodes in the linked
    list
 Notes
    JSON spec ignored if the last item isn't found in the
    linked list.
 
    Possible fix - recursion
 */

void json_format_list(L_NODE *list, char **locations){
    printf("[\n");
    if (*locations) {
        while (*locations++)
            json_format_list_item_oendl(seach_list_by_location(list, locations[-1]), !!*locations);
    } else {
        foreach(list, &json_format_list_item_endl, TRAVERSE_FORWARD);
    }
    printf("\n]\n");
}

int main(int argc, char **argv) {
    // Flags
    argv++; // skip the program argument
    int verbose = !--argc;
    
    // Initalize and read data
    L_NODE *list = read_data((argc-- ? *argv++ : NULL));
    
    
    // Output
    if (verbose) {
        puts("\nData in ascending order:");
        traverse(list, OUTPUT_USER_FRIENDLY, TRAVERSE_FORWARD);
        puts("\nData in descending order:");
        traverse(list, OUTPUT_USER_FRIENDLY, TRAVERSE_BACKWARD);
        puts("");
    }

    // Search Loop
    if (verbose) {
        ui_search_loop(list);
    } else {
        json_format_list(list, argv);
    }

    // Free
    free_list(list);
    
#ifdef _MSC_VER
    printf( _CrtDumpMemoryLeaks() ? "Memory Leak\n" : "No Memory Leak\n");
#endif
    
    return 0;
}

/**
 Examples
 ===================
 ./program                            # run activates UI – search loop etc.
 ./program <filename>                 # converts the data from <filename> to JSON – no UI
 ./program <filename> <location> ...  # ... selecting only the speifid <location(s)>
 */

/**
 Output 00
 ===================
 ./program
 ===================
 Path to input:
 
 Data in ascending order:
 (location: Arizona,Flagstaff              | averageTemperature:  82.50 | lastRecordedTemperature: 84 )
 (location: Arizona,Phoenix                | averageTemperature: 108.00 | lastRecordedTemperature: 107)
 (location: Arizona,Tucson                 | averageTemperature: 104.50 | lastRecordedTemperature: 109)
 (location: California,Cupertino           | averageTemperature:  88.00 | lastRecordedTemperature: 88 )
 (location: California,Los Angeles         | averageTemperature:  84.50 | lastRecordedTemperature: 97 )
 (location: California,San Francisco       | averageTemperature:  72.25 | lastRecordedTemperature: 82 )
 (location: California,Yreka               | averageTemperature: 100.50 | lastRecordedTemperature: 100)
 (location: Nevada,Reno                    | averageTemperature: 108.00 | lastRecordedTemperature: 108)
 (location: Oregon,Portland                | averageTemperature:  80.50 | lastRecordedTemperature: 79 )
 (location: Oregon,Salem                   | averageTemperature:  86.00 | lastRecordedTemperature: 85 )
 (location: Pennsylvania,Philadelphia      | averageTemperature:  88.50 | lastRecordedTemperature: 86 )
 (location: Pennsylvania,Pittsburgh        | averageTemperature:  89.00 | lastRecordedTemperature: 89 )
 
 Data in descending order:
 (location: Pennsylvania,Pittsburgh        | averageTemperature:  89.00 | lastRecordedTemperature: 89 )
 (location: Pennsylvania,Philadelphia      | averageTemperature:  88.50 | lastRecordedTemperature: 86 )
 (location: Oregon,Salem                   | averageTemperature:  86.00 | lastRecordedTemperature: 85 )
 (location: Oregon,Portland                | averageTemperature:  80.50 | lastRecordedTemperature: 79 )
 (location: Nevada,Reno                    | averageTemperature: 108.00 | lastRecordedTemperature: 108)
 (location: California,Yreka               | averageTemperature: 100.50 | lastRecordedTemperature: 100)
 (location: California,San Francisco       | averageTemperature:  72.25 | lastRecordedTemperature: 82 )
 (location: California,Los Angeles         | averageTemperature:  84.50 | lastRecordedTemperature: 97 )
 (location: California,Cupertino           | averageTemperature:  88.00 | lastRecordedTemperature: 88 )
 (location: Arizona,Tucson                 | averageTemperature: 104.50 | lastRecordedTemperature: 109)
 (location: Arizona,Phoenix                | averageTemperature: 108.00 | lastRecordedTemperature: 107)
 (location: Arizona,Flagstaff              | averageTemperature:  82.50 | lastRecordedTemperature: 84 )
 
 Search records by state/city. Type "quit" or press RETURN to exit.
 > Pennsylvania,Pittsburgh
 (location: Pennsylvania,Pittsburgh        | averageTemperature:  89.00 | lastRecordedTemperature: 89 )
 > Arizona,Flagstaff
 (location: Arizona,Flagstaff              | averageTemperature:  82.50 | lastRecordedTemperature: 84 )
 > California,San Francisco
 (location: California,San Francisco       | averageTemperature:  72.25 | lastRecordedTemperature: 82 )
 > California,San Jose
 Records for the city "California,San Jose" could not be found.
 > quit
 Program ended with exit code: 0
 */

/**
 Output 01
 ===================
 ./program temperatures.txt
 ===================
 [
    {"location": "Arizona,Flagstaff", "averageTemperature": 82.50, "recordedTemperatures": [84, 81]},
    {"location": "Arizona,Phoenix", "averageTemperature": 108.00, "recordedTemperatures": [107, 109]},
    {"location": "Arizona,Tucson", "averageTemperature": 104.50, "recordedTemperatures": [109, 103, 99, 107]},
    {"location": "California,Cupertino", "averageTemperature": 88.00, "recordedTemperatures": [88]},
    {"location": "California,Los Angeles", "averageTemperature": 84.50, "recordedTemperatures": [97, 82, 81, 78]},
    {"location": "California,San Francisco", "averageTemperature": 72.25, "recordedTemperatures": [82, 68, 64, 75]},
    {"location": "California,Yreka", "averageTemperature": 100.50, "recordedTemperatures": [100, 101]},
    {"location": "Nevada,Reno", "averageTemperature": 108.00, "recordedTemperatures": [108, 108]},
    {"location": "Oregon,Portland", "averageTemperature": 80.50, "recordedTemperatures": [79, 82]},
    {"location": "Oregon,Salem", "averageTemperature": 86.00, "recordedTemperatures": [85, 83, 90]},
    {"location": "Pennsylvania,Philadelphia", "averageTemperature": 88.50, "recordedTemperatures": [86, 91]},
    {"location": "Pennsylvania,Pittsburgh", "averageTemperature": 89.00, "recordedTemperatures": [89]}
 ]
 */

/**
 Output 02
 ===================
 ./program temperatures.txt "California,San Francisco" Oregon,Portland Arizona,Flagstaff
 ===================
 [
    {"location": "California,San Francisco", "averageTemperature": 72.25, "recordedTemperatures": [82, 68, 64, 75]},
    {"location": "Oregon,Portland", "averageTemperature": 80.50, "recordedTemperatures": [79, 82]},
    {"location": "Arizona,Flagstaff", "averageTemperature": 82.50, "recordedTemperatures": [84, 81]}
 ]
 */
