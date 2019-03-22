//
// Created by Darwin on 2019-03-13.
//

#include <stdio.h>
#include <stdlib.h>
#include "../bst/bst.h"
#include "../queue/queue.h"

#define VAL(v, type) *((type*)v)

void q_process(int *n) {
    printf("q_process: %d\n", *n);
}

void queue_test(void) {
    QUEUE *q;

    int n[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    q = queue__init();

    for (int i = 0; i < 10; ++i) {
        queue__enqueue(q, n + i);
        if (i % 2) {
            printf("odd(i): %d\n", *((int*)queue__dequeue(q)));
        }
    }

    queue__traverse(q, (Q_PROC)q_process);

    for (int j = 0; j < 5; ++j) {
        printf("(j): %d\n", *((int*)queue__dequeue(q)));
    }

    free(q);
}

int bst_compare(int *existing, int *new) {
    return *new - *existing;
}

void bst_merge(int **exiting, int *new) {
    printf("duplicate data (%d, %d)\n", **exiting, *new);
    **exiting += *new;
}

void *identity(void *v) {
    return v;
}

void bst_process(int *n) {
    printf("%d ", *n);
}

void bst_test(void) {
    BST_NODE *root;

    int n[] = { 25, -7, -7, 20, -9, 0, 3, 6, 90, -50 };

    root = NULL;
    //bst_node__init(n);

    for (int i = 0; i < 10; ++i) {
        root = __bst__insert(
                root,
                (BST_CMP) bst_compare,
                (BST_INS) identity,
                (BST_MER) bst_merge,
                n + i);
    }

    bst__traverse(root, (BST_PROC)bst_process, BST_IN_ORDER);
}

typedef struct t_record {
    int *n;
    QUEUE *q;
} REC;

int compare(REC *r, int *n) {
    return *n - *r->n;
}

void *insert(int *n){
    REC *r;

    r = (REC*)calloc(1, sizeof(REC));

    r->n = n;
    r->q = queue__init();

    queue__enqueue(r->q, n);

    return r;
}

void merge(REC **r, int *n) {
    queue__enqueue((*r)->q, n);
}

void print(const int *n) {
    printf("%d ", *n);
}

void process(REC *r) {
    queue__consume(&(r->q), (Q_PROC)print);
    free(r);
    r = NULL;
    printf("\n");
}

int bst_queue_test(void) {

    int n[] = {
            1, 2, 3, 4, 5,
            1, 1, 2, 5, 4,
            3, 4, 5, 6, 8,
            3, 2, 5, 6, 8
    };

    BST_NODE *tree = NULL;

    for (int i = 0; i < 20; ++i) {
        tree = __bst__insert(
                tree,
                (BST_CMP) compare,
                (BST_INS) insert,
                (BST_MER) merge,
                &n[i]
        );
    }

    bst__consume(&tree, (BST_PROC)process, BST_IN_ORDER);

}

int main(void) {
    queue_test();

    return 0;
}