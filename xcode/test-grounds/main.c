//
//  main.c
//  test-grounds
//
//  Created by Darwin on 1/29/19.
//  Copyright © 2019 Galac Tech. All rights reserved.
//

#include <unistd.h>
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

void freel(void *list) {
    free(list);
}

#include <strings.h>

#include <errno.h>
#include <stdio.h>

#define MATCH_BUFF 32

long *fun(char *fname) {
    long m_size = MATCH_BUFF;
    size_t line_buff = 64;
    long matched = 0;
    long line = 1;
    char *char_line;
    long *matches = calloc(m_size, sizeof(long));
    FILE *f = fopen(fname, "r");
    if (!f) perror("Failed to open file");
    while (getline(&char_line, &line_buff, f) > 0) {
        if (strpbrk(char_line, "0123456789")) matches[matched++] = line;
        
        if (matched >= m_size) {
            m_size += MATCH_BUFF;
            matches = realloc(matches, sizeof(long) * m_size);
            if (!matches) perror("Could not realloc array");
        }
        
        line++;
    }
    matches = realloc(matches, sizeof(long) * (matched + 1));
    if (!matches) perror("Could not realloc array");
    matches[matched] = 0;
    return matches;
}

#define GET_BIT(n, i) ((n >> i) & 1)

void printbits(short int x) {
    for (short int i = 15; i >= 0; i--)
        printf("%d", GET_BIT(x, i));
    printf("\n");
}

void readback(){
    char c = fgetc(stdin);
    if (c != '\n') readback();
    putchar(c);
}

//double safe_powr(int exp, double n) {
//    if (exp == 1 || exp == -1) return n;
//    if (exp < 0)
//        return safe_powr(exp + 1, 1 / (n * 2));
//    return safe_powr(exp - 1, n * 2);
//}
//
//double powr(int exp, double n) {
//    if (!exp) return 1;
//    return safe_powr(exp, n);
//}


double powr(int exp, double n) {
    if (exp == 1 || exp == -1) return n;
    if (exp < 0)
        return (1 / n) * powr(exp + 1, 1 / n);
    return n * powr(exp - 1, n);
}

int main(int argc, const char * argv[]) {
//    int **x = (int[5][4]) malloc(20 * sizeof(int));
    
//    printf("%d\n", x[0]);
//    
//    free(x);
//    printf("%f\n", powr(0, 2));
//    printf("%f\n", powr(2, 2));
//    printf("%f\n", powr(3, 2));
//    printf("%f\n", powr(2, 10));
//    printf("%f\n", powr(10, 2));
//    printf("%f\n", powr(-2, 2));
//    printf("> ");
//    readback();
//    printf("\n");
//    int d = -5;
//    printbits(d);
//    printf("%d\n", GET_BIT(d, 0));
//    printf("%d\n", GET_BIT(d, 1));
//    printf("%d\n", GET_BIT(d, 2));
//
    
//    long *stuff = fun("more_hardware.txt");
//    while (*stuff) printf("%ld\n", *stuff++);
    
    
//    int fd = open("antonyms_2.txt", O_RDONLY);
//    int *list = malloc(sizeof(int) * 5);
//    int x = 6;
    
//    freel(&list);
    
//    free(&list);
    
//    int     i;
//    char    s[21];

//    printf(">");
//    scanf("%20s %*[^\n] %d", s, &i);
//    printf("%s %d\n", s, i);
//    NODE *list = init_node(init_person("Noah", 18));
//    list->next = init_node(init_person("Joe", 21));
//
//    PERSON* p = (PERSON*) list->data;
//    printf("(name: %s, age: %d)\n", p->name, p->age);
//    freel(p);
//    free(p);
//    p->age = 5;
//    printf("(name: %s, age: %d)\n", p->name, p->age);
}

