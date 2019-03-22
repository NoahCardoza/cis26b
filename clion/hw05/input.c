/*******************************************************
 Examples: 3.20 - 3.23
 Driver program to demonstrate the OS Queues Program:
 a singly-linked list of queues
 *******************************************************/

#include <stdio.h>
#include <stdlib.h>  // malloc(), free(), exit()
#include <string.h>

#define DUMMY_TRAILER '\177'
// octal ASCII code of the
// last character in the ASCII table

// queue node
typedef struct request REQUEST;
struct request
{
    char *username;
    long job_id;
    struct request *next;
};

// linked list node
typedef struct service SERVICE;
struct service
{
    char *qname;
    struct request *front;
    struct request *rear;
    struct service *next;  // next node in the list
};

int main (void)
{
    SERVICE *init_list(void);
    void getinfo(char *qname, char *username, long *job_id);
    void insert(SERVICE *serv_ptr, char *qname, char *username, long job_id);
    void traverse(SERVICE *serv_ptr);
    
    SERVICE *serv_ptr;
    char qname[30], username[20];
    long job_id;
    
    serv_ptr = init_list();
    while (getinfo(qname, username, &job_id), strcmp(qname, "quit") != 0)
    {
        insert(serv_ptr, qname, username, job_id);
    }
    traverse(serv_ptr);
    
    return 0;
}

/************************************************************************
 Initialization of a linked list with two
 sentinel nodes
    PRE: nothing
    POST: serv_ptr - a pointer to the head of the list
 */
SERVICE *init_list(void)
{
    SERVICE *serv_ptr;
    
    // allocate the first sentinel node
    serv_ptr = (SERVICE *) malloc(sizeof (SERVICE));
    if (!serv_ptr)
    {
        printf("Error in init_list!\n");
        exit(1);
    }
    serv_ptr->qname = (char *) malloc(1);
    if (!serv_ptr->qname)
    {
        printf("Error in init_list!\n");
        exit(1);
    }
    *serv_ptr->qname = '\0'; // dummy header value
    
    // allocate the last sentinel node
    serv_ptr->next = (SERVICE *) malloc(sizeof (SERVICE));
    if (!(serv_ptr->next))
    {
        printf("Error in init_list!\n");
        exit(1);
    }
    serv_ptr->next->qname = (char *) malloc(2);
    if (!serv_ptr->next->qname)
    {
        printf("Error in init_list!\n");
        exit(1);
    }
    *serv_ptr->next->qname = DUMMY_TRAILER;
    serv_ptr->next->qname[1] = '\0';
    serv_ptr->next->next = NULL;
    
    return serv_ptr;
}


/************************************************************************
 Get user input
    PRE: nothing
    POST: qname, username, job_id
 */
void getinfo(char *qname, char *username, long *job_id)
{
    char line[512];
    
    printf("Enter qname username job-id (such as Print1 John 1234): ");
    gets(line);
    
    if (strcmp(line, "quit") == 0)
        strcpy(qname, "quit");
    else
        sscanf(line, "%s %s %ld", qname, username, job_id);
    return;
}


/************************************************************************
 Insert a new request
    PRE: serv_ptr, qname, username, job_id
 */
void insert(SERVICE *serv_ptr, char *qname, char *username, long job_id)
{
    SERVICE *curr = serv_ptr->next;
    SERVICE *prev = serv_ptr;
    SERVICE *newserv;
    REQUEST *newreq;
    
    // look for qname
    while (strcmp(qname, curr->qname) > 0)
    {
        prev = curr;
        curr = curr->next;
    }
    if (strcmp(qname, curr->qname) != 0) // Not found: new service!
    {                                    // add a new node to the linked list
        if ((newserv = (SERVICE *)malloc(sizeof(SERVICE))) == NULL)
        {
            printf("Fatal malloc error!\n");
            exit(1);
        }
        if ((newserv->qname = (char *)malloc(strlen(qname) + 1)) == NULL)
        {
            printf("Fatal malloc error!\n");
            exit(2);
        }
        strcpy(newserv->qname, qname);
        newserv->next = curr;
        prev->next = newserv;
        newserv->front = NULL;
    }
    else // server found
    {
        newserv = curr;
    }
    // add a new node to the matching queue
    if ((newreq = (REQUEST *)malloc(sizeof(REQUEST))) == NULL)
    {
        printf("Fatal malloc error!\n");
        exit(3);
    }
    if ((newreq->username = (char *)malloc(strlen(username) + 1)) == NULL)
    {
        printf("Fatal malloc error!\n");
        exit(4);
    }
    strcpy(newreq->username, username);
    newreq->job_id = job_id;
    newreq->next = NULL;
    
    if (newserv->front == NULL)
    {
        newserv->rear = newserv->front = newreq;
    }
    else
    {
        curr->rear->next = newreq;
        curr->rear = curr->rear->next;
    }
    return;
}

/************************************************************************
 Traverse a list of queues
    PRE: serv_ptr
 */
void traverse(SERVICE *serv_ptr)
{
    REQUEST *reqptr;
    
    serv_ptr = serv_ptr->next; // skip dummy node
    while (*serv_ptr->qname != DUMMY_TRAILER)
    {
        printf("Service name: %s\n", serv_ptr->qname);
        printf("--------------------\n");
        for (reqptr = serv_ptr->front; reqptr != NULL; reqptr = reqptr->next)
        {
            printf("%-15s %ld\n", reqptr->username, reqptr->job_id );
        }
        putchar('\n');
        serv_ptr = serv_ptr->next;
    }
}
