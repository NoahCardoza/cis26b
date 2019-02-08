//
//  main.c
//  test-grounds
//
//  Created by Darwin on 1/29/19.
//  Copyright © 2019 Galac Tech. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

typedef struct t_node {
    void *data;
    struct t_node *next;
} NODE;

typedef struct t_person {
    char name[31];
    int age;
} PERSON;

//
//int dot_test(int a, ...) {
//    printf("%lu", sizeof(a));
//    char *p = (char *) &a + sizeof(a);
//    printf("%s\n", p);
//    return a;
//}

NODE *init_node(void *data) {
    NODE *node = malloc(sizeof(NODE));
    
    node->data = data;
    node->next = NULL;
    
    return node;
}

PERSON *init_person(char *name, int age){
    PERSON *p = malloc(sizeof(PERSON));
    
    strcpy(p->name, name);
    p->age = age;
    
    return p;
}

//void freel(int *list) {
//    free(list);
//}

int main(int argc, const char * argv[]) {
//    int *list = malloc(sizeof(int) * 5);
//    int x = 6;
    
//    freel(&list);
    
//    free(&list);
    
//    int     i;
//    char    s[21];
//
//    printf(">");
//    scanf("%20s %*[^\n] %d", s, &i);
//    printf("%s %d\n", s, i);
//    NODE *list = init_node(init_person("Noah", 18));
//    list->next = init_node(init_person("Joe", 21));
//
//    PERSON* p = (PERSON*) list->data;
//    printf("(name: %s, age: %d)\n", p->name, p->age);
}

