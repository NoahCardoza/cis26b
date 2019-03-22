//
// Created by Darwin on 2019-03-11.
//

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "../bst/bst.h"
#include "../queue/queue.h"

#define LINE_BUFFER 128
#define GLOBAL_DELEMS " \t\r\n!#%&()*+,-./:;<=>?[\\]^{|}~\v\f"


/**
 *
 * @param filename
 * @param mode
 * @return
 */

FILE *my_fopen(char *filename, char *mode) {
    FILE *fp;

    if (!(fp = fopen(filename, mode))) {
        fprintf(stderr, "Could not open %s : %s\n", filename, strerror(errno));
        exit(errno);
    }

    return fp;
}

/**
 * searches for pattern in a c-string
 * @param s1 string to be searched
 * @param s2 pattern to search for
 * @return 1 if s1 and s2 match otherwise 0
 */

int	match(char *s1, char *s2) {
    if (!*s1 && !*s2)
        return (1);
    if (!*s1 && *s2 == '*')
        return (match(s1, s2 + 1));
    if (*s1 && *s2 == '*')
        return ((match(s1 + 1, s2) || match(s1, s2 + 1)));
    if (*s1 == *s2)
        return (match(s1 + 1, s2 + 1));
    return (0);
}



int validate_args(int argc, char **argv) {
    if (argc != 3) {
        printf("ERROR: required inputs: in_file out_file\n");
        return (0);
    }

    if (!match(argv[1], "*.c")) {
        printf("ERROR: in_file must end in `.c`\n");
        return (0);
    }

    return (1);
}

void cat_with_lines(FILE *fp) {
    char *line = NULL;
    int line_number = 1;
    size_t buffer = LINE_BUFFER;
    while (getline(&line, &buffer, fp) > 0) {
        printf("%05d : %s", line_number++, line);
    }
}

char *strip_comments(char *s) {
    char *p = NULL, *head = NULL;
    static int found = 0;

    found = !!found;

    // search for c comments
    if ((head = strstr(s, "/*"))) {
       found = 2;
    }

    if ((p = strstr(s, "*/"))) {
        if (found) {
            head = found == 2 ? head : s;
            found = 0;
            p = strip_comments(p + 2);
            memmove(head, p, strlen(p) + 1);
        } else {
            s = p + 2;
        }
    } else if (found == 2) {
        *head = 0;
    } else if (found) {
        *s = 0;
    }

    // search for c++ comments and end the line
    if ((p = strstr(s, "//"))) {
       *p = 0;
    }

    return s;
}

char *strip_literals(char *s){
    static int open_literal = 0;
    char *start, *end;

    start = end = s;
    if (open_literal || (end = start = strchr(s, '"'))) {
        while ((end = strchr(end + 1, '"'), end && end[-1] == '\\'));
        if (!end) {
            open_literal = 1;
            end = strrchr(start, '\\');
            *start = 0;
            // nothing left on the line
            return s;
        }
        open_literal = 0;
        end = strip_literals(end + 1);
        memmove(start, end, strlen(end) + 1);
    }

    if ((end = start = strchr(s, '\''))) {
        end = strchr(end + 1, '\'');
        end += end[1] == '\'';
        end = strip_literals(end + 1);
        memmove(start, end, strlen(end) + 1);
    }

    return s;
}

char *strip_carrots(char *s){
    char *start;

    if (strstr(s, "#include") && (start = strchr(s, '<'))) {
         *start = 0;
    }

    return s;
}

char *next_token(char *str) {
    static char *s;
    static char *backlog;
    char *p; // to be used for random tasks


    if (str) {
        s = str;
        s = strip_comments(s);
        s = strip_literals(s);
        s = strip_carrots(s);
        backlog = strtok(s, GLOBAL_DELEMS);
        return NULL;
    }

    p = backlog;
    backlog = strtok(NULL, GLOBAL_DELEMS);

    if (p) {
        if (isnumber(*p))
            return next_token(NULL);
    }
    return p;
}


void parse_tokens(FILE *fp) {
    char *line = NULL, *parser;
    ssize_t line_number = 1, line_length, *last_quote_pos, *next_quote_pos;
    size_t buffer = LINE_BUFFER;

    while ((line_length = getline(&line, &buffer, fp)) > 0) {

        parser = line;
        // next_quote_pos = strpbrk(parser, "'\"");
        // *next_quote_pos = 0;
        parser = strtok(line, GLOBAL_DELEMS);
        while (parser) {
            printf("%s ", parser);
            parser = strtok(NULL, GLOBAL_DELEMS);
        }
        printf("\n");
        // printf("%05d : %s", line_number++, line);
    }
}

struct token {
    char *str;
    int   line;
};

typedef struct t_identifier {
    char  *token;
    QUEUE *queue;
} IDENTIFIER;

int *int__init(int n) {
    int *p;

    p = (int*)malloc(sizeof(int));
    *p = n;

    return p;
}

int compare(IDENTIFIER *identifier, struct token *token) {
    return strcmp(token->str, identifier->token);
}

void *insert(struct token *token){
    IDENTIFIER *identifier;

    identifier = (IDENTIFIER*)calloc(1, sizeof(IDENTIFIER));

    identifier->token = token->str;
    identifier->queue = queue__init();

    queue__enqueue(identifier->queue, int__init(token->line));

    return identifier;
}

void merge(IDENTIFIER **identifier, struct token *token) {

    queue__enqueue((*identifier)->queue, int__init(token->line));
}

void print(const int *n) {
    printf("%d ", *n);
    // TODO: free *n
}

void process(IDENTIFIER *identifier) {
    printf("%-30s : ", identifier->token);
    queue__consume(&identifier->queue, (Q_PROC)print);
    free(identifier);
    printf("\n");
}

int main(int argc, char **argv) {
    if (!validate_args(argc, argv)) return (1);
    char *in_file  = argv[1];
    char *out_file = argv[2];

    char comment_line_tests[10][80] = {
            "this is my line",
            "i am in the middle // hello",
            "here is the other line",
            "5 * /* times */ 7",
            "x /* start",
            "*/ 8",
            "t /* before */ + /* after */ tt",
            "t /* before */ + /* // not a comment */ tt // this is a comment"
    };

    char string_line_tests[10][80] = {
            "    \"a single line\"     ",
            "printf(\"printf(\\\"hello\\\");\n\");",
            "printf(\"hello\");",
            "strcat(s, \"ra\", \"cec\", \"ar\");",
            "\"\"",
            "strchr(\"racecar\", 'e');",
            "strchr(\"Noah's\", '\\'');"

    };

    char line_tests[30][80] = {
            "/**",
            " * testing multiline comments",
            " */",
            "#include <stdeath.h>",
            "while (x) --x;",
            "for (int i = 0; i < 10; ++i) // loop 10 times {",
            "   printf(\"result: %d\"i, i * i);",
            "}",
            "t /* before */ + /* after */ tt",
            "t /* before */ + /* // not a comment */ tt // this is a comment",
            "    \"a single line\"     ",
            "printf(\"printf(\\\"hello\\\");\n\");",
            "printf(\"hello\");",
            "strcat(s, \"ra\", \"cec\", \"ar\");",
            "\"\"",
            "strchr(\"racecar\", 'e');",
            "strchr(\"Noah's\", '\\'');",
            "printf(\"ha\\",
            " %s  ha ha\", string);"
    };

    //for (int i = 0; i < 20; ++i) {
    //    printf("`%s`\n", strip_literals(string_line_tests[i]));
    //}

//    char *token;
    BST_NODE *tree = NULL;
    struct token token;

    for (int i = 0; i < 30; ++i) {
        next_token(line_tests[i]);
        while ((token.str = next_token(NULL))) {
            printf("`%s`\n", token.str);
            token.line = i + 1;
            tree = bst__insert(
                    tree,
                    (BST_CMP)compare,
                    (BST_INS)insert,
                    (BST_MERGE)merge,
                    &token
                    );
        }
    }

    bst__consume(&tree, (BST_PROC)process, BST_IN_ORDER);

    //FILE *in = my_fopen(in_file, "r");
    //parse_tokens(in);
    //cat_with_lines(in);
    //fclose(in);
    return (0);
}
