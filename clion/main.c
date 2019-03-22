//
// Created by Darwin on 2019-03-21.
//

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char **checkDelete(char **filenames) {
     char *filename;
     char **ret = NULL, **tmp;
     int  found = 0, size = 0;

     ret = malloc(sizeof(char*));
     if (!ret) exit(11);

     *ret = NULL;
     while ((filename = *filenames)){
         if(access(filename, F_OK) != -1) {
             found += 1;
             size += strlen(filename) + 1;
             tmp = (char**)realloc(ret, found * sizeof(char*));
             if (!tmp) {
                 free(ret);
                 exit(11);
             }
             ret = tmp;
             ret[found - 1] = (char*)malloc(size * sizeof(char));
             if (!ret[found - 1]) exit(11);
             strcpy(ret[found - 1], filename);
             // ret[found - 1] = filename;
             ret[found] = NULL;
         }
         filenames++;
     }
    return ret;
}

int main(void){
    char *files[80] = {
            "26B_Cardoza_Noah_H5.zip",
            "test",
            NULL
    };

    char **o = checkDelete(files);

    while (*o) {
        printf("%s\n", *o);
        o++;
    }

}

