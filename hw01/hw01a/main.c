/***
 CIS 26B: Homework 1A
 ********************
 NAME:
 IDE(compiler):
 *************************************************************************
 Review: Functions, Structures, Pointers, Arrays, and Sorting
 Sort an array of STUDENT structures using the insertion sort algorithm.
 
 Your task:
 1. Write function named printList() that displays the list to the
 screen and call it in main() as needed to print the sorted and unsorted data.
 Display the sections for each course on the same line.
 2. Write a second sort function that sorts the array in descending order
 by the final exam score, then call it.
 3. Replace the "Sample Output" below with the updated output
 */

#include <stdio.h>
#include <string.h>

#define NUM_CLS 9

typedef struct
{
    char  course[10];
    int   noSections;
    int   sections[16];
} CIS_CLASSES;

void insertionSort (CIS_CLASSES  list[], CIS_CLASSES *pLast);
void mySort (CIS_CLASSES  list[], CIS_CLASSES *pLast);
void printList(char *heading, CIS_CLASSES  *list, CIS_CLASSES *pLast);

int main (void)
{
    CIS_CLASSES  *pLast;
    CIS_CLASSES  clsList[NUM_CLS] =
    {
        {"CIS 35A", 2, {61, 63}},
        {"CIS 35B", 1, {62}},
        {"CIS 41A", 3, {1, 2, 61}},
        {"CIS 28",  1, {61}},
        {"CIS 22C", 4, {3, 4, 61, 63}},
        {"CIS 26B", 1, {61}},
        {"CIS 22B", 8, {1, 2, 3, 4, 6, 61, 62, 63}},
        {"CIS 29",  1, {61}},
        {"CIS 22A", 8, {1, 3, 5, 6, 7, 8, 61, 63}},
    };
    
    pLast = clsList + NUM_CLS - 1;
    
    printList("Unsorted data", clsList, pLast);
    insertionSort(clsList, pLast);
    printList("Sorted data", clsList, pLast);
    mySort(clsList, pLast);
    printList("My Sorted data", clsList, pLast);
    
    return 0;
}

/*    ========================================================
 Print all the classes in the class list.
 Pre
    *heading : the text to print out describing the reason for the list print
    *clsList : points to the first  index in the class list
    *pLast   : points to the last index in the class list
 Post  List items are printed to screen.
 */

void printList(char *heading, CIS_CLASSES  *clsList, CIS_CLASSES *pLast) {
    printf("%s:\n", heading);
    for (CIS_CLASSES *pCls = clsList; pCls <= pLast; pCls++)
    {
        printf("%-10s %2d : ", pCls->course, pCls->noSections);
        for (int i = 0; i < pCls->noSections; i++)
            printf("%02d ", pCls->sections[i]);
        printf("\n");
    }
    printf("\n");
}

/*    ========================================================
 Sort list using Insertion Sort.
 Pre   list[] must contain at least one element
 size is index to last element in list
 Post  list has been rearranged.
 */
void insertionSort (CIS_CLASSES  list[], CIS_CLASSES *pLast)
{
    CIS_CLASSES  temp;
    CIS_CLASSES *pCurr;
    CIS_CLASSES *pWalk;
    
    for (pCurr = list + 1; pCurr <= pLast; pCurr++)
    {
        temp    = *pCurr;
        pWalk = pCurr - 1;
        while (pWalk >= list && strcmp(temp.course, pWalk->course) < 0)
        {
            *(pWalk + 1) = *pWalk;
            pWalk--;
        }
        *(pWalk + 1)  = temp;
    }
    
}

/*    ========================================================
 Sort list by number of sections using Insertion Sort.
 Pre   list[] must contain at least one element
 size is index to last element in list
 Post  list has been rearranged.
 */

void mySort(CIS_CLASSES  list[], CIS_CLASSES *pLast) {
    CIS_CLASSES  temp;
    CIS_CLASSES *pCurr;
    CIS_CLASSES *pWalk;
    
    for (pCurr = list + 1; pCurr <= pLast; pCurr++)
    {
        temp    = *pCurr;
        pWalk = pCurr - 1;
        while (pWalk >= list && (temp.noSections > pWalk->noSections))
        {
            *(pWalk + 1) = *pWalk;
            pWalk--;
        }
        *(pWalk + 1)  = temp;
    }
}

/*    ================= Sample Output ================= */
/*
 Unsorted data:
 CIS 35A     2
 CIS 35B     1
 CIS 41A     3
 CIS 28      1
 CIS 22C     4
 CIS 26B     1
 CIS 22B     8
 CIS 29      1
 CIS 22A     8
 
 Sorted data:
 CIS 22A     8
 CIS 22B     8
 CIS 22C     4
 CIS 26B     1
 CIS 28      1
 CIS 29      1
 CIS 35A     2
 CIS 35B     1
 CIS 41A     3
 
 */
/*    ================= Updated Output ================= */
/*
 Unsorted data:
 CIS 35A     2 : 61 63
 CIS 35B     1 : 62
 CIS 41A     3 : 01 02 61
 CIS 28      1 : 61
 CIS 22C     4 : 03 04 61 63
 CIS 26B     1 : 61
 CIS 22B     8 : 01 02 03 04 06 61 62 63
 CIS 29      1 : 61
 CIS 22A     8 : 01 03 05 06 07 08 61 63
 
 Sorted data:
 CIS 22A     8 : 01 03 05 06 07 08 61 63
 CIS 22B     8 : 01 02 03 04 06 61 62 63
 CIS 22C     4 : 03 04 61 63
 CIS 26B     1 : 61
 CIS 28      1 : 61
 CIS 29      1 : 61
 CIS 35A     2 : 61 63
 CIS 35B     1 : 62
 CIS 41A     3 : 01 02 61
 
 My Sorted data:
 CIS 22A     8 : 01 03 05 06 07 08 61 63
 CIS 22B     8 : 01 02 03 04 06 61 62 63
 CIS 22C     4 : 03 04 61 63
 CIS 41A     3 : 01 02 61
 CIS 35A     2 : 61 63
 CIS 26B     1 : 61
 CIS 28      1 : 61
 CIS 29      1 : 61
 CIS 35B     1 : 62 

 */
