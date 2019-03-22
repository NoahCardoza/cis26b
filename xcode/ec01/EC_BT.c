/****************************************************
   CIS 26B                              Extra Credit
   Binary Trees
   
   Count the nodes in a binary tree.

   Write two recursive functions: 
        A. - use static         1 Point
        B. - do not use static  3 Points

   int count(NODE *root);
   
   Run the program and save the output as a comment!

   Name: Noah Cardoza
   Date: March 6th 2019
*****************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100

typedef struct nodeTag{
   int data;
   struct nodeTag *left;
   struct nodeTag *right;
} NODE;

int main( void )
{
   NODE *buildBinaryTree( int n );
   void printTreeInorder( NODE *root );
   int countA(NODE *root);
   int countB(NODE *root);

   NODE *root;
   int   n;
   
   srand(time(NULL));
   n = rand() % 6 + 1;
   root = buildBinaryTree( n );
   printTreeInorder( root );

   printf("Number of nodes: %d\n", countA(root));
   printf("Number of nodes: %d\n", countB(root));
   printf("- - - - - - - - - -\n"); // how would static be helpful?
   printf("Number of nodes: %d\n", countA(root));
   printf("Number of nodes: %d\n", countB(root));

   printf( "\n\n" );
   return 0;
}
/****************************************************************
   Count the nodes in a binary tree
        1. use static
*/
int countA(NODE *root)
{
    static int n = 0;
    
    if (root) {
        n++;
        countA(root->left);
        countA(root->right);
    }
    return n;
}

/****************************************************************
   Count the nodes in a binary tree
        2. do not use static
*/
int countB(NODE *root)
{
    if (root) {
        return countB(root->left) + 1 + countB(root->right);
    }
    return 0;
}

/****************************************************************
   INORDER
   Print a binary tee in Left-Root-Right sequence.
*/
void printTreeInorder( NODE *root )
{
   if( root ){
      printTreeInorder( root->left);
      printf( "%3d\n",  root->data );
	  printTreeInorder( root->right);
   }
   return;
}

/****************************************************************
   Build a random binary tree of integer numbers within the range
   [0, 99]; root is always 50//This is NOT A SEARCH TREE!
*/
NODE *buildBinaryTree( int n )
{
   void  insert( NODE *root, int data );

   NODE *root = NULL;
   int   data;

   // allocate and initialize the root
   root = (NODE *) malloc (sizeof(NODE));
   if (!root)
      printf("Overflow!\n"), exit(100);
   root->data  = 50;
   root->left  = NULL;
   root->right = NULL;
   while(--n){
        data   = rand( ) % 100;
        insert(root, data);
   }
   return root;
}

/****************************************************************
   Insert a node into a random binary tree
*/
void insert( NODE *root, int data )
{
   NODE *newNode;
   NODE *pWalk;
   NODE *parent;
   int  rand_num;

   // allocate the new node
   newNode = (NODE *) malloc (sizeof(NODE));
   if( !newNode )
      printf( "Overflow!\n" ), exit(100);
   newNode->data  = data;
   newNode->left  = NULL;
   newNode->right = NULL;

   // find a "random" parent
   pWalk = root;
   while( pWalk ){
      parent   = pWalk;
      rand_num = rand( ) % 100;
      if( rand_num % 2 ) /* if odd - take left */
         pWalk = pWalk->left;
      else
         pWalk = pWalk->right;
   }

   // insert the new node
   if( !parent->left ) // no left child
       parent->left  = newNode;
   else
       parent->right = newNode;
   return;
}

/* OUTPUT

*/

