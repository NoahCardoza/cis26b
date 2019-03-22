//
// Created by Darwin on 2019-03-12.
//

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "bst.h"

/**
 * initiates a BST struct with all the necessary fields
 * @param comparer the function used to compare void pointers when inserting
 * @param inserter a transformer function used when inserting data
 * @param merger called when the comparer returns 0 (found a duplicate node key)
 * @return a BST object
 */

BST bst__init(BST_CMP comparer, BST_INS inserter, BST_MER merger) {
    BST tree;

    tree.root = NULL;
    tree.cmp  = comparer;
    tree.ins  = inserter;
    tree.mer  = merger;

    return tree;
}

/**
 * allocated and initializes a BST_NODE
 * @param data the pointer to assign to the
 * data field in the BST_NODE
 * @return a newly allocated BST_NODE
 */

BST_NODE *bst_node__init(void *data){
    BST_NODE *node;

    node = malloc(sizeof(BST_NODE));

    if (!node) {
        perror("bst_node__init");
        exit(errno);
    }

    node->data = data;
    node->left = NULL;
    node->right = NULL;

    return node;
}

/**
 * insets data into a binary tree
 * @param tree the tree to inset the data into
 * @param data the data to be passed the the
 * compare/insert/merge functions
 */

void bst__insert(BST *tree, void *data) {
    tree->root = __bst__insert(tree->root, tree->cmp, tree->ins, tree->mer, data);
}

/**
 * the function which bst__insert calls internally
 * @param root the root to the tree
 * @param compare the function to compare the void pointers
 * @param insert a transformer function which decided what
 * will be stored into the new node
 * @param merge if a duplicate node key is found
 * (if compare returns 0) the will function be called
 * @param data the data to be passed to all three functions
 * @return returns the root node
 */

BST_NODE *__bst__insert(BST_NODE *root, BST_CMP compare, BST_INS insert, BST_MER merge, void *data){
    int test;

    if (!root) return bst_node__init(insert(data));

    test = compare(root->data, data);

    if (test < 0) {
        root->left = __bst__insert(root->left, compare, insert, merge, data);
    } else if (test > 0) {
        root->right = __bst__insert(root->right, compare, insert, merge, data);
    } else {
        merge(&root->data, data);
    }


    return root;
}

/**
 * traverses a tree in a certain order calling process on each node
 * @param root the root of the tree
 * @param process the function to apply to each node's data
 * @param mode the traversal mode [ InOrder | PreOrder | PostOrder ]
 */

void bst__traverse(BST_NODE *root, BST_PROC process, enum traversal_order mode) {
    switch (mode) {
        case InOrder:
            return bst__traverse_in_order(root, process);
        case PreOrder:
            return bst__traverse_pre_order(root, process);
        case PostOrder:
            return bst__traverse_post_order(root, process);
        default:
            printf("bst__traverse: invalid (mode = %d)\n", mode);
    }
}

/**
 * traverse a tree from a root node in order
 * @param root the root of the tree
 * @param process the function to apply to each node's void data
 */

void bst__traverse_in_order(BST_NODE *root, BST_PROC process) {
    if (!root) return;

    bst__traverse_in_order(root->left, process);
    process(root->data);
    bst__traverse_in_order(root->right, process);
}

/**
 * traverse a tree from a root node pre order
 * @param root the root of the tree
 * @param process the function to apply to each node's void data
 */

void bst__traverse_pre_order(BST_NODE *root, BST_PROC process) {
    if (!root) return;

    process(root->data);
    bst__traverse_pre_order(root->left, process);
    bst__traverse_pre_order(root->right, process);
}

/**
 * traverse a tree from a root node post order
 * @param root the root of the tree
 * @param process the function to apply to each node's void data
 */

void bst__traverse_post_order(BST_NODE *root, BST_PROC process) {
    if (!root) return;

    bst__traverse_post_order(root->left, process);
    bst__traverse_post_order(root->right, process);
    process(root->data);
}

/**
 * similar to traversing a tree, except the tree is destroyed
 * as it is traversed, each node being freed, the process function
 * is expected to free the data contained within each node
 * or otherwise store it somewhere else lest the pointers be lost
 * forever
 * @param root the root of the tree
 * @param process the function to be applied to each node's void data -
 * also expected to free the or track the pointers passes to it or else
 * create a memory leak
 * @param mode the mode in which to traverse the tree
 */

void bst__consume(BST_NODE **root, BST_PROC process, enum traversal_order mode) {
     switch (mode) {
        case InOrder:
            return bst__consume_in_order(root, process);
        case PreOrder:
            return bst__consume_pre_order(root, process);
        case PostOrder:
            return bst__consume_post_order(root, process);
        default:
            printf("bst__consume: invalid (mode = %d)\n", mode);
    }
}

/**
 * to be called internally by bst__consume to traverse a
 * tree in order
 * @param root the root of the tree
 * @param process the function to be applied to each data point
 */

void bst__consume_in_order(BST_NODE **root, BST_PROC process) {
    if (!(*root)) return;

    bst__consume_in_order(&(*root)->left, process);
    process((*root)->data);
    bst__consume_in_order(&(*root)->right, process);

    (*root)->data = (*root)->left = (*root)->right = NULL;
    free(*root);
    *root = NULL;
}

/**
 * to be called internally by bst__consume to traverse a
 * tree pre order
 * @param root the root of the tree
 * @param process the function to be applied to each data point
 */

void bst__consume_pre_order(BST_NODE **root, BST_PROC process) {
    if (!(*root)) return;

    process((*root)->data);
    bst__consume_pre_order(&(*root)->left, process);
    bst__consume_pre_order(&(*root)->right, process);

    (*root)->data = (*root)->left = (*root)->right = NULL;
    free(*root);
    *root = NULL;
}

/**
 * to be called internally by bst__consume to traverse a
 * tree post order
 * @param root the root of the tree
 * @param process the function to be applied to each data point
 */

void bst__consume_post_order(BST_NODE **root, BST_PROC process) {
    if (!(*root)) return;

    bst__consume_post_order(&(*root)->left, process);
    bst__consume_post_order(&(*root)->right, process);
    process((*root)->data);

    (*root)->data = (*root)->left = (*root)->right = NULL;
    free(*root);
    *root = NULL;
}

