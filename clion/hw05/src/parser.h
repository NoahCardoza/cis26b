//
// Created by Darwin on 2019-03-13.
//

#ifndef CIS26B_PARSER_H
#define CIS26B_PARSER_H

#define GLOBAL_DELIMITERS " \t\r\n!#%&()*+,-./:;<=>?[\\]^{|}~\v\f"

char *strip_comments(char *s);
char *strip_literals(char *s);
char *next_token(char *str);
char *strip_carrots(char *s);

#endif //CIS26B_PARSER_H
