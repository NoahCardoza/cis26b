//
// Created by Darwin on 2019-03-12.
//

#ifndef CIS26B_QUEUE_H
#define CIS26B_QUEUE_H

typedef struct t_q_node {
    void            *data;
    struct t_q_node *next;
} Q_NODE;

typedef struct t_queue {
    Q_NODE *head;
    Q_NODE *tail;
} QUEUE;

typedef void (*Q_PROC)(void *);

Q_NODE  *q_node__init(void *data);
void    *q_node__free(Q_NODE **node);


QUEUE   *queue__init(void);
void     queue__consume(QUEUE **queue, Q_PROC process);
void     queue__traverse(QUEUE *queue, Q_PROC process);

void     queue__enqueue(QUEUE *queue, void *data);
void    *queue__dequeue(QUEUE *queue);

#endif //CIS26B_QUEUE_H

