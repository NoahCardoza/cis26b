//
// Created by Darwin on 2019-03-12.
//
#include "queue.h"
#include <stdlib.h> // malloc, free
#include <stdio.h>  // perror, printf
#include <errno.h>  // errno

/**
 * initiates a new QUEUE object
 * @return a QUEUE struct
 */

QUEUE *queue__init(void) {
    QUEUE *queue;

    queue = (QUEUE*) malloc(sizeof(QUEUE));
    queue->head = NULL;
    queue->tail  = NULL;

    return queue;
}

/**
 * dynamically allocate a Q_NODE and link the data
 * field
 * @param data the data to link to the Q_NODE
 * @return a newly allocated Q_NODE
 */

Q_NODE *q_node__init(void *data) {
    Q_NODE *node;

    node = (Q_NODE*) malloc(sizeof(Q_NODE));

    if (!node) {
        perror("q_node__init");
        exit(errno);
    }

    node->data = data;
    node->next = NULL;

    return node;
}

/**
 * applies process to each node in the queue
 * @param queue the queue to traverse
 * @param process the function to call
 */

void queue__traverse(QUEUE *queue, Q_PROC process) {
    Q_NODE *node;
    node = queue->head;
    while (node) {
        process(node->data);
        node = node->next;
    }
}

/**
 * similar to traverse except it traverses by
 * dequeuing nodes and thus destroys the queue
 * @param queue the about to be destroyed
 * @param process the function to be applied to each
 * data point, this should also free each pointer if
 * or store it elsewhere lest the pointer be lost and
 * a memory leak is created
 */

void queue__consume(QUEUE **queue, Q_PROC process) {
    while ((*queue)->head) {
        process(queue__dequeue(*queue));
    }

    free(*queue);
    *queue = NULL;
}

/*
 * TODO: write this - maybe?
 void *queue__peek(QUEUE *queue, int n, ) {

}*/

/**
 * frees a node and returns the contents
 * @param node the node to be freed
 * @return the data within the node
 */

void *q_node__free(Q_NODE **node) {
    void *data;

    data = (*node)->data;
    free(*node);
    *node = NULL;
    return data;
}

/**
 * enqueues data into the queue
 * @param queue the queue to insert the data into
 * @param data the data to enqueue
 */

void queue__enqueue(QUEUE *queue, void *data) {
    Q_NODE *node;

    node = q_node__init(data);

    if (!queue->head) {
        queue->head = queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
}

/**
 * dequeues a node and returns the data within
 * @param queue the queue from which to dequeue
 * @return the data within the dequeued node
 */

void *queue__dequeue(QUEUE *queue) {
    Q_NODE *node;

    if (!queue->head) return NULL;

    node = queue->head;
    queue->head = node->next;

    if (!queue->head) queue->tail = NULL;

    return q_node__free(&node);
}

