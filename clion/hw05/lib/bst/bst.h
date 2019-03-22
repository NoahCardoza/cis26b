//
// Created by Darwin on 2019-03-12.
//

#ifndef CIS26B_BST_H
#define CIS26B_BST_H

#define BST_IN_ORDER 0

enum traversal_order { InOrder, PostOrder, PreOrder };

/**
 * a function determining whether to go right, left,
 * or to call the merger function
 * @param existing the existing data in the node
 * @param data the data passed to the insert function
 * @return returns a number similar to <code>strcpm</code>
 *
 * -> &lt; 0 : go left
 *
 * -> > 0 : go right
 *
 * -> = 0 : call merger
 */

typedef int   (*BST_CMP) (void *existing, void *data);

/**
 * a transformer function called on right before inserting
 * data into a tree
 *
 * useful when inserting structs which hold data in which
 * not all fields are to be tested when determining the order
 * of the tree
 *
 * if no transformation is required consider passing the
 * <code> bst__identity</code> function
 * @param data the data passed to the insert function
 * @return a pointer the data to be inserted into the tree
 */

typedef void* (*BST_INS) (void *data);

/**
 * a function which defines how to handle duplicate keys
 *
 * if duplicate keys are not allowed in your tree this
 * function should alert the user of any errors
 * @param existing the data in the node evaluating as a duplicate
 * @param data the data passed to the insert function
 * @return nothing - this function is expected to mutate the existing data
 */

typedef void  (*BST_MER) (void **existing, void *data);

/**
 * a general type of function called on data points
 * inside a tree - could be used to mutate data
 * @param data a pointer to the data within the node
 *
 * TODO: @param index pass index of data point
 */

typedef void  (*BST_PROC)(void *data);

typedef struct t_bst_node {
    void              *data;
    struct t_bst_node *left;
    struct t_bst_node *right;
} BST_NODE;

typedef struct t_bst {
    BST_NODE    *root;
    BST_CMP      cmp;
    BST_INS      ins;
    BST_MER      mer;
} BST;


BST          bst__init(BST_CMP comparer, BST_INS inserter, BST_MER merger);
BST_NODE    *bst_node__init(void *data);

void         bst__insert(BST *tree, void *data);
BST_NODE    *__bst__insert(BST_NODE *root, BST_CMP compare, BST_INS insert, BST_MER merge, void *data);
void         bst__traverse(BST_NODE *root, BST_PROC process, enum traversal_order mode);
void         bst__traverse_in_order(BST_NODE *root, BST_PROC process);
void         bst__traverse_pre_order(BST_NODE *root, BST_PROC process);
void         bst__traverse_post_order(BST_NODE *root, BST_PROC process);
void         bst__consume(BST_NODE **root, BST_PROC process, enum traversal_order mode);
void         bst__consume_in_order(BST_NODE **root, BST_PROC process);
void         bst__consume_pre_order(BST_NODE **root, BST_PROC process);
void         bst__consume_post_order(BST_NODE **root, BST_PROC process);

#endif //CIS26B_BST_H
