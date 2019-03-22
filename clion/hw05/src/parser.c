//
// Created by Darwin on 2019-03-13.
//

#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * a wrapper for <code>strtok</code> which ignores
 * text found in comments, string/character literals,
 * and include/import carrots
 *
 * similar API to <code>strtok</code> except the first
 * call returns <code>NULL</code> and consecutive calls
 * where <code>str</code> is <code>NULL</code>
 *
 * resets on new string passed to <code>str</code>
 * @param str the string to tokenize
 * @return <code>NULL</code> on the first call and the
 * result of <code>strtok</code> on the following calls
 */

char *next_token(char *str) {
    static char *s;
    // used because the first call to strtok needs
    // to be the string and consecutive calls should
    // pass NULL
    static char *backlog;
    char *p; // to be used for random tasks

    if (str) {
        s = str;
        s = strip_comments(s);
        s = strip_literals(s);
        s = strip_carrots(s);
        backlog = strtok(s, GLOBAL_DELIMITERS);
        return NULL;
    }

    p = backlog;
    backlog = strtok(NULL, GLOBAL_DELIMITERS);

    if (p) {
        if (isnumber(*p))
            return next_token(NULL);
    }
    return p;
}

/**
 * strips comments out of <code>s</code> by shifting characters
 * inplace
 *
 * lines are assumed to be passed in order - otherwise
 * multiline comment detection behaviour is undefined
 * @param s the line to strip
 * @return returns a pointer to the new beginning of the comment-less string
 */

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

/**
 * strips both string and character literals
 * from <code>s</code> by shifting string
 * inplace
 *
 * lines are assumed to be passed in order - otherwise
 * multiline string detection is undefined
 * @param s the line to strip
 * @return a pointer to the beginning of
 * the literal free line
 */

char *strip_literals(char *s) {
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

/**
 * strips the carrots succeeding a pre processor
 * include and import statements in <code>s</code> by
 * null terminating the string
 * @param s the line to strip
 * @return <code>s</code>
 */

char *strip_carrots(char *s) {
    char *start;

    if ((strstr(s, "#include") || strstr(s, "#import")) && (start = strchr(s, '<'))) {
        *start = 0;
    }

    return s;
}
