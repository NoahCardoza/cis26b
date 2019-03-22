//
// Created by Noah Cardoza on 2019-03-11.
// IDE: CLion
//

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "../lib/bst/bst.h"
#include "../lib/queue/queue.h"
#include "parser.h"

#define LINE_BUFFER 128
#define IDENTIFIER_PADDING "30"

struct token {
    char *str;
    int   line;
};

typedef struct t_identifier {
    char  *token;
    QUEUE *queue;
} IDENTIFIER;

/**
 * mallocs a size bytes and copies size number of
 * bytes from src into the newly malloced memory
 * @param src a pointer the the memory to be copied
 * @param size the number of bytes to be copied
 * @return returns a pointer to a copy the it
 * data in src
 */

void *make_copy(void *src, size_t size) {
    void *ret;

    ret = malloc(size);

    memcpy(ret, src, size);

    return ret;
}

/**
 * opens a file and handles errors
 * @param filename the file to open
 * @param mode the mode to open the file in
 * @return always returns a FILE pointer
 */

FILE *my_fopen(char *filename, char *mode) {
    FILE *fp;

    if (!(fp = fopen(filename, mode))) {
        fprintf(stderr, "Could not open %s : %s\n", filename, strerror(errno));
        exit(errno);
    }

    return fp;
}

/**
 * searches for pattern in a c-string
 * @param s1 string to be searched
 * @param s2 pattern to search for
 * @return 1 if s1 and s2 match otherwise 0
 */

int	match(char *s1, char *s2) {
    if (!*s1 && !*s2)
        return (1);
    if (!*s1 && *s2 == '*')
        return (match(s1, s2 + 1));
    if (*s1 && *s2 == '*')
        return ((match(s1 + 1, s2) || match(s1, s2 + 1)));
    if (*s1 == *s2)
        return (match(s1 + 1, s2 + 1));
    return (0);
}

/**
 * validates the program's arguments
 * @param argc the count
 * @param argv the arguments passed to the program
 * @return returns true or false depending on the
 * status of the arguments
*/

int validate_args(int argc, char **argv) {

    if (argc != 3) {
        printf("ERROR: required inputs: in_file out_file\n");
        return (0);
    }



    return (1);
}

void cat_with_lines(FILE *fp) {
    char *line = NULL;
    size_t buffer;
    int line_number;

    line_number = 1;
    buffer = LINE_BUFFER;
    while (getline(&line, &buffer, fp) > 0) {
        printf("%05d : %s", line_number++, line);
    }
    free(line);
}

/**
 * passed to the tree object to sort tokens with
 * <code>strcmp</code>
 * @param identifier the existing data type in the tree node
 * @param token the token to be inserted
 * @return a number &gt; 0 to go right, < 0 to go left,
 * or = 0 to merge the node with the token
 */

int compare(IDENTIFIER *identifier, struct token *token) {
    return strcmp(token->str, identifier->token);
}

/**
 * passed to the tree object to transform the token
 * into an IDENTIFIER
 * @param token the token to be transformed
 * @return an IDENTIFIER with a copy of the token token data and
 * a newly initiated QUEUE
 */

void *insert(struct token *token){
    IDENTIFIER *identifier;

    identifier = (IDENTIFIER*)malloc(sizeof(IDENTIFIER));

    /*
     * we make a copy of the token data because the pointer will be
     * freed and realloced when we read the next line
     */
    identifier->token = make_copy(token->str, sizeof(char) * (strlen(token->str) + 1));
    identifier->queue = queue__init();

    queue__enqueue(identifier->queue, make_copy(&token->line, sizeof(int)));

    return identifier;
}

/**
 * passed to the tree object to define how to merge duplicate
 * keys into a tree node
 * @param identifier the existing data in the node
 * @param token the token to be merged
 */

void merge(IDENTIFIER **identifier, struct token *token) {
    int *last_line;

    last_line = (int*)(*identifier)->queue->tail->data;
    if (*last_line != token->line)
        queue__enqueue((*identifier)->queue, make_copy(&token->line, sizeof(int)));
}

/**
 * passed to queue__consume to display the line
 * numbers on which a identifier was found wrapping
 * every 5 numbers
 *
 * also frees each number from the heap
 * @param n the line number
 */

void free_queue_node(int *n) {
    static int i = 0;

    if (!n) {
        i = 0;
        return;
    }

    printf("%6d", *n);
    if (!(++i % 5)) {
        printf("\n%" IDENTIFIER_PADDING "s   ", "");
    }
    free(n);
}

/**
 * passed to bst__consume to display each token and
 * it's respective line numbers
 *
 * it also frees the data associated with each tree node
 * @param identifier the data stored in each node
 */

void free_tree_node(IDENTIFIER *identifier) {
    printf("%-" IDENTIFIER_PADDING "s : ", identifier->token);
    queue__consume(&identifier->queue, (Q_PROC) free_queue_node);
    free_queue_node(NULL);
    free(identifier->token);
    free(identifier);
    printf("\n");
}

/**
 * traverses a file line by line ignoring strings,
 * c++ style comments and multiline/inline comments
 * and parsing out
 * @param fp a FILE object pointing to the file
 * whose tokens are to be read into memory
 * @return returns the root of a binary tree
*/

BST_NODE *parse_tokens(FILE *fp) {
    char *line = NULL;
    size_t buffer = LINE_BUFFER;
    struct token token;
    BST tree;

    tree = bst__init(
            (BST_CMP)compare,
            (BST_INS)insert,
            (BST_MER)merge
    );

    token.str  = NULL;
    token.line = 1;

    while (getline(&line, &buffer, fp) > 0) {
        next_token(line);
        while ((token.str = next_token(NULL))) {
            bst__insert(&tree, &token);
        }
        token.line++;
    }
    free(line);

    return tree.root;
}

/**
 * prints a timestamped header
 */

void print_header(){
    struct tm   *ptime;
    time_t       vtime;
    char         buffer[80];

    time(&vtime);
    ptime = localtime(&vtime);
    strftime(buffer, 80, "%A, %B %d, %T, %Y", ptime);
    printf(
          "/************************  Cross Reference Listing  ************************/\n"
          "\n"
          "Cross reference listing made at: %s\n"
          "\n", buffer);
}

/**
 * determines if the arguments are valid and initializes variables
 * @param argc the number of args - 1
 * @param argv the same argv passed to the main
 * @param in a file object to read from
 * @return the error number to return
 */

int args_init(int argc, char **argv, FILE **in) {
    // printf("%s", argv[0]);
    switch (argc) {
        case 0: // no arguments
            *in = stdin;
            break;
        case 2: // two arguments
            printf("INFO: redirecting output to %s\n", argv[2]);
        case 1: // one argument
            if (!strcmp(argv[1], "-h")) {
                printf("usage : ./program [?input *.c] [?output *]\n"
                       "note  : if no arguments provided\n"
                       "        STDIN will be read from instead\n");
                return 0;
            }
            if (!match(argv[1], "*.c")) {
                printf("ERROR: input must end in `.c`\n");
                return 1;
            }

            // otherwise the above error is written to the file
            if (argc == 2) {
                if (!freopen(argv[2], "w", stdout)) {
                    perror("output file could not be opened");
                    return errno;
                }
            }
            *in = my_fopen(argv[1], "r");
            break;
        default:
            printf("ERROR: program only processes up to 2 arguments\n");
            return 1;
    }

    return 0;
}

int main(int argc, char **argv) {
    FILE *in = NULL;
    int err;

    err = args_init(argc - 1, argv, &in);
    if (err) return err;

    BST_NODE *tree_root = parse_tokens(in);
    print_header();
    bst__consume(&tree_root, (BST_PROC) free_tree_node, InOrder);
    /*
     * if the fd is 0 we know it is stdin
     * we cannot rewind stdin and we should not
     * close stdin as the behaviour is undefined
     */
    if (fileno(in) != 0) {
        rewind(in);
        cat_with_lines(in);
        fclose(in);
    } else {
        printf("INFO: cannot rewind STDIN\n");
    }
    return (0);
}

// ~/projects/c/lion/cis26B/cmake-build-debug/hw05 input.c
// /************************  Cross Reference Listing  ************************/
//
// Cross reference listing made at: Tuesday, March 19, 21:25:13, 2019
//
// DUMMY_TRAILER                  :     11    93   195
// NULL                           :     95   140   145   153   160
// 165   172   174   199
// REQUEST                        :     16   130   160   192
// SERVICE                        :     25    36    38    39    41
// 61    63    66    81   125
// 127   128   129   140   190
//
// char                           :     19    28    37    38    42
// 72    87   106   108   125
// 145   165
// curr                           :    127   133   135   136   138
// 151   157   180   181
// define                         :     11
// else                           :    115   155   178
// exit                           :     70    76    85    91   143
// 148   163   168
// for                            :    199
// front                          :     29   153   174   176   199
//
// getinfo                        :     37    46   106
// gets                           :    111
// if                             :     67    73    82    88   113
// 138   140   145   160   165
// 174
// include                        :      7     8     9
// init_list                      :     36    45    61
// insert                         :     38    48   125
// int                            :     34
// job_id                         :     20    37    38    43    46
// 48   106   116   125   171
// 201
// line                           :    108   111   113   116
// long                           :     20    37    38    43   106
// 125
// main                           :     34
// malloc                         :     66    72    81    87   140
// 145   160   165
// newreq                         :    130   160   165   170   171
// 172   176   180
// newserv                        :    129   140   145   150   151
// 152   153   157   174   176
//
// next                           :     21    31    81    82    87
// 88    93    94    95   127
// 136   151   152   172   180
// 181   194   199   204
// prev                           :    128   135   152
// printf                         :     69    75    84    90   110
// 142   147   162   167   197
// 198   201
// putchar                        :    203
// qname                          :     28    37    38    42    46
// 48    72    73    78    87
// 88    93    94   106   114
// 116   125   133   138   145
// 150   195   197
// rear                           :     30   176   180   181
// reqptr                         :    192   199   201
// request                        :     16    17    21    29    30
//
// return                         :     52    97   117   183
// serv_ptr                       :     38    39    41    45    48
// 50    63    66    67    72
// 73    78    81    82    87
// 88    93    94    95    97
// 125   127   128   190   194
// 195   197   199   204
// service                        :     25    26    31
// sizeof                         :     66    81   140   160
// sscanf                         :    116
// strcmp                         :     46   113   133   138
// strcpy                         :    114   150   170
// strlen                         :    145   165
// struct                         :     16    17    21    25    26
// 29    30    31
// traverse                       :     39    50   190
// typedef                        :     16    25
// username                       :     19    37    38    42    46
// 48   106   116   125   165
// 170   201
// void                           :     34    36    37    38    39
// 61   106   125   190
// while                          :     46   133   195
// 00001 : /*******************************************************
// 00002 :  Examples: 3.20 - 3.23
// 00003 :  Driver program to demonstrate the OS Queues Program:
// 00004 :  a singly-linked list of queues
// 00005 :  *******************************************************/
// 00006 :
// 00007 : #include <stdio.h>
// 00008 : #include <stdlib.h>  // malloc(), free(), exit()
// 00009 : #include <string.h>
// 00010 :
// 00011 : #define DUMMY_TRAILER '\177'
// 00012 : // octal ASCII code of the
// 00013 : // last character in the ASCII table
// 00014 :
// 00015 : // queue node
// 00016 : typedef struct request REQUEST;
// 00017 : struct request
// 00018 : {
// 00019 :     char *username;
// 00020 :     long job_id;
// 00021 :     struct request *next;
// 00022 : };
// 00023 :
// 00024 : // linked list node
// 00025 : typedef struct service SERVICE;
// 00026 : struct service
// 00027 : {
// 00028 :     char *qname;
// 00029 :     struct request *front;
// 00030 :     struct request *rear;
// 00031 :     struct service *next;  // next node in the list
// 00032 : };
// 00033 :
// 00034 : int main (void)
// 00035 : {
// 00036 :     SERVICE *init_list(void);
// 00037 :     void getinfo(char *qname, char *username, long *job_id);
// 00038 :     void insert(SERVICE *serv_ptr, char *qname, char *username, long job_id);
// 00039 :     void traverse(SERVICE *serv_ptr);
// 00040 :
// 00041 :     SERVICE *serv_ptr;
// 00042 :     char qname[30], username[20];
// 00043 :     long job_id;
// 00044 :
// 00045 :     serv_ptr = init_list();
// 00046 :     while (getinfo(qname, username, &job_id), strcmp(qname, "quit") != 0)
// 00047 :     {
// 00048 :         insert(serv_ptr, qname, username, job_id);
// 00049 :     }
// 00050 :     traverse(serv_ptr);
// 00051 :
// 00052 :     return 0;
// 00053 : }
// 00054 :
// 00055 : /************************************************************************
// 00056 :  Initialization of a linked list with two
// 00057 :  sentinel nodes
// 00058 :     PRE: nothing
// 00059 :     POST: serv_ptr - a pointer to the head of the list
// 00060 :  */
// 00061 : SERVICE *init_list(void)
// 00062 : {
// 00063 :     SERVICE *serv_ptr;
// 00064 :
// 00065 :     // allocate the first sentinel node
// 00066 :     serv_ptr = (SERVICE *) malloc(sizeof (SERVICE));
// 00067 :     if (!serv_ptr)
// 00068 :     {
// 00069 :         printf("Error in init_list!\n");
// 00070 :         exit(1);
// 00071 :     }
// 00072 :     serv_ptr->qname = (char *) malloc(1);
// 00073 :     if (!serv_ptr->qname)
// 00074 :     {
// 00075 :         printf("Error in init_list!\n");
// 00076 :         exit(1);
// 00077 :     }
// 00078 :     *serv_ptr->qname = '\0'; // dummy header value
// 00079 :
// 00080 :     // allocate the last sentinel node
// 00081 :     serv_ptr->next = (SERVICE *) malloc(sizeof (SERVICE));
// 00082 :     if (!(serv_ptr->next))
// 00083 :     {
// 00084 :         printf("Error in init_list!\n");
// 00085 :         exit(1);
// 00086 :     }
// 00087 :     serv_ptr->next->qname = (char *) malloc(2);
// 00088 :     if (!serv_ptr->next->qname)
// 00089 :     {
// 00090 :         printf("Error in init_list!\n");
// 00091 :         exit(1);
// 00092 :     }
// 00093 :     *serv_ptr->next->qname = DUMMY_TRAILER;
// 00094 :     serv_ptr->next->qname[1] = '\0';
// 00095 :     serv_ptr->next->next = NULL;
// 00096 :
// 00097 :     return serv_ptr;
// 00098 : }
// 00099 :
// 00100 :
// 00101 : /************************************************************************
// 00102 :  Get user input
// 00103 :     PRE: nothing
// 00104 :     POST: qname, username, job_id
// 00105 :  */
// 00106 : void getinfo(char *qname, char *username, long *job_id)
// 00107 : {
// 00108 :     char line[512];
// 00109 :
// 00110 :     printf("Enter qname username job-id (such as Print1 John 1234): ");
// 00111 :     gets(line);
// 00112 :
// 00113 :     if (strcmp(line, "quit") == 0)
// 00114 :         strcpy(qname, "quit");
// 00115 :     else
// 00116 :         sscanf(line, "%s %s %ld", qname, username, job_id);
// 00117 :     return;
// 00118 : }
// 00119 :
// 00120 :
// 00121 : /************************************************************************
// 00122 :  Insert a new request
// 00123 :     PRE: serv_ptr, qname, username, job_id
// 00124 :  */
// 00125 : void insert(SERVICE *serv_ptr, char *qname, char *username, long job_id)
// 00126 : {
// 00127 :     SERVICE *curr = serv_ptr->next;
// 00128 :     SERVICE *prev = serv_ptr;
// 00129 :     SERVICE *newserv;
// 00130 :     REQUEST *newreq;
// 00131 :
// 00132 :     // look for qname
// 00133 :     while (strcmp(qname, curr->qname) > 0)
// 00134 :     {
// 00135 :         prev = curr;
// 00136 :         curr = curr->next;
// 00137 :     }
// 00138 :     if (strcmp(qname, curr->qname) != 0) // Not found: new service!
// 00139 :     {                                    // add a new node to the linked list
// 00140 :         if ((newserv = (SERVICE *)malloc(sizeof(SERVICE))) == NULL)
// 00141 :         {
// 00142 :             printf("Fatal malloc error!\n");
// 00143 :             exit(1);
// 00144 :         }
// 00145 :         if ((newserv->qname = (char *)malloc(strlen(qname) + 1)) == NULL)
// 00146 :         {
// 00147 :             printf("Fatal malloc error!\n");
// 00148 :             exit(2);
// 00149 :         }
// 00150 :         strcpy(newserv->qname, qname);
// 00151 :         newserv->next = curr;
// 00152 :         prev->next = newserv;
// 00153 :         newserv->front = NULL;
// 00154 :     }
// 00155 :     else // server found
// 00156 :     {
// 00157 :         newserv = curr;
// 00158 :     }
// 00159 :     // add a new node to the matching queue
// 00160 :     if ((newreq = (REQUEST *)malloc(sizeof(REQUEST))) == NULL)
// 00161 :     {
// 00162 :         printf("Fatal malloc error!\n");
// 00163 :         exit(3);
// 00164 :     }
// 00165 :     if ((newreq->username = (char *)malloc(strlen(username) + 1)) == NULL)
// 00166 :     {
// 00167 :         printf("Fatal malloc error!\n");
// 00168 :         exit(4);
// 00169 :     }
// 00170 :     strcpy(newreq->username, username);
// 00171 :     newreq->job_id = job_id;
// 00172 :     newreq->next = NULL;
// 00173 :
// 00174 :     if (newserv->front == NULL)
// 00175 :     {
// 00176 :         newserv->rear = newserv->front = newreq;
// 00177 :     }
// 00178 :     else
// 00179 :     {
// 00180 :         curr->rear->next = newreq;
// 00181 :         curr->rear = curr->rear->next;
// 00182 :     }
// 00183 :     return;
// 00184 : }
// 00185 :
// 00186 : /************************************************************************
// 00187 :  Traverse a list of queues
// 00188 :     PRE: serv_ptr
// 00189 :  */
// 00190 : void traverse(SERVICE *serv_ptr)
// 00191 : {
// 00192 :     REQUEST *reqptr;
// 00193 :
// 00194 :     serv_ptr = serv_ptr->next; // skip dummy node
// 00195 :     while (*serv_ptr->qname != DUMMY_TRAILER)
// 00196 :     {
// 00197 :         printf("Service name: %s\n", serv_ptr->qname);
// 00198 :         printf("--------------------\n");
// 00199 :         for (reqptr = serv_ptr->front; reqptr != NULL; reqptr = reqptr->next)
// 00200 :         {
// 00201 :             printf("%-15s %ld\n", reqptr->username, reqptr->job_id );
// 00202 :         }
// 00203 :         putchar('\n');
// 00204 :         serv_ptr = serv_ptr->next;
// 00205 :     }
// 00206 : }
