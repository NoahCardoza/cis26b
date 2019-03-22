//
//  main.c
//  ec00
//
//  Created by Darwin on 2/24/19.
//  Copyright © 2019 Galac Tech. All rights reserved.
//  NAME: Noah Cardoza
//  IDE(compiler): Xcode(gcc)
//

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <errno.h>
#include <ctype.h>

#define ASSERT_CONT(exp, msg)  \
if (exp) {                     \
printf(msg);                   \
continue;                      \
}                              \

#define STR_BUFF 127
#define LINE_BUFF 32 // LINE_BUFF >= 2

void *my_realloc(void **ptr, size_t n, size_t size, char *traceback) {
    // does this not need to be casted?
    *ptr = realloc(*ptr, n * size);
    if (!ptr) {
        fprintf(stderr, "Error allocating memory in: %s\n", traceback);
        perror("");
        exit(errno);
    }
    return ptr;
}

static int insert_lines(char *buffer, long **lines, long head, long count){
    static long line_alloc = LINE_BUFF;
    
    int i;
    long line_buffer[LINE_BUFF];
    char *buff_head = buffer;
    
    
    for (i = 0; i < LINE_BUFF; i++) {
        buffer = strchr(buffer, '\n');
        if (!buffer) break;
        line_buffer[i] = head + (++buffer - buff_head);
    }

    // 1 becasue we skip index 0
    if (line_alloc <= count + i) {
        line_alloc += LINE_BUFF;
        my_realloc((void**)lines, line_alloc, sizeof(long), "insert_lines");
    }
    // 1 becasue we skip index 0
    memcpy(*lines + count + 1, line_buffer, sizeof(long) * i);
    if (i == LINE_BUFF) {
        return i + insert_lines(buffer, lines, head + (buffer - buff_head), count + i);
    }
    return i;
}

long index_newlines(FILE *fp, long **lines) {
    char buff[STR_BUFF + 1] = {0};
    size_t ret;
    size_t char_total = 0;
    long line_count = 1;
    
    *lines = (long*) calloc(LINE_BUFF, sizeof(long));
    
    do {
        char_total += (ret = fread(buff, sizeof(char), STR_BUFF, fp));
        buff[ret] = 0;
        line_count += insert_lines(buff, lines, char_total - ret, line_count);
    } while (ret == STR_BUFF);
    
    my_realloc((void**)lines, line_count + 1, sizeof(long), "main");
    
    return line_count;
}

int main(int argc, const char * argv[]) {
    long start, end, range, count;
    
    char *input, *endp, *parser;
    size_t linebuff = 128;
    FILE *fp;
    long *line_indexs;
    
    printf("File Range Grabber\n"
           "filename> ");
    
    if ((count = getline(&input, &linebuff, stdin)) == EOF) {
        perror("Error");
        exit(errno);
    }

    input[count - 1] = 0;
    
    if (!(fp = fopen(input, "r"))) {
        perror("Error");
        exit(errno);
    }
    
    count = index_newlines(fp, &line_indexs);
    
    printf("\n%ld lines found\n"
           "# use\n"
           "> start end\n"
           "# to print a range of lines to the screen\n",
           count);
    while ((void)(printf("\n> ")), getline(&input, &linebuff, stdin) != EOF) {
        if (*input == '\n') {
            free(input);
            break;
        }
        parser = input;
        errno = 0;
        start = strtol(parser, &endp, 10);
        ASSERT_CONT(!isspace(*endp), "Numbers must be separated by whitespace\n")
        ASSERT_CONT(errno == EINVAL, "Could not parse the first number\n")
        parser = endp;
        errno = 0;
        end = strtol(parser, &endp, 10);
        ASSERT_CONT(errno == EINVAL, "Could not parse the second number\n")
        ASSERT_CONT(*endp != '\n', "Invalid input\n")
        parser = endp;
        
        if (start <= end && (start >= 1 && start <= count) && (end >= 1 && end <= count)) {
            range = end - start + 1;
            for (long i = 0; i < range; i++) {
                fseek(fp, line_indexs[start + i], SEEK_SET);
                getline(&input, &linebuff, fp);
                printf("%05ld : %s", start + i, input);
            }
        } else {
            printf("The number must be between 1 and %ld and the range must not be negative.\n", count);
        }
    }
    
    fclose(fp);
    return 0;
}

/*****************
 Output
 *****************/
/*****************
 File Range Grabber
 filename> temperatures.txt
 
 30 lines found
 # use
 > start end
 # to print a range of lines to the screen
 
 > 1 2
 00001 : Pennsylvania,Philadelphia:91
 00002 : California,San Francisco:75
 
 > 1
 Could not parse the second number
 
 > 5 10
 00005 : California,Yreka:101
 00006 : Arizona,Tucson:107
 00007 : California,Los Angeles:78
 00008 : California,Los Angeles:81
 00009 : Pennsylvania,Pittsburgh:89
 00010 : Oregon,Salem:90
 
 > 1h1
 Numbers must be separated by whitespace
 
 > 1 1
 00001 : Pennsylvania,Philadelphia:91
 
 > 1   7
 00001 : Pennsylvania,Philadelphia:91
 00002 : California,San Francisco:75
 00003 : Nevada,Reno:108
 00004 : Arizona,Flagstaff:81
 00005 : California,Yreka:101
 00006 : Arizona,Tucson:107
 00007 : California,Los Angeles:78
 
 > 1 1
 00001 : Pennsylvania,Philadelphia:91
 
 > 1
 Could not parse the second number
 
 >    1 0
 The number must be between 1 and 30 and the range must not be negative.
 
 > 4 1
 The number must be between 1 and 30 and the range must not be negative.
 
 > 1 4
 00001 : Pennsylvania,Philadelphia:91
 00002 : California,San Francisco:75
 00003 : Nevada,Reno:108
 00004 : Arizona,Flagstaff:81
 
 > 1 29
 00001 : Pennsylvania,Philadelphia:91
 00002 : California,San Francisco:75
 00003 : Nevada,Reno:108
 00004 : Arizona,Flagstaff:81
 00005 : California,Yreka:101
 00006 : Arizona,Tucson:107
 00007 : California,Los Angeles:78
 00008 : California,Los Angeles:81
 00009 : Pennsylvania,Pittsburgh:89
 00010 : Oregon,Salem:90
 00011 : California,Los Angeles:82
 00012 : Arizona,Flagstaff:84
 00013 : California,San Francisco:64
 00014 : Oregon,Salem:83
 00015 : California,San Francisco:68
 00016 : Arizona,Tucson:99
 00017 : California,Yreka:100
 00018 : Arizona,Phoenix:109
 00019 : Oregon,Portland:82
 00020 : Arizona,Tucson:103
 00021 : Oregon,Portland:79
 00022 : Arizona,Phoenix:107
 00023 : California,Cupertino:88
 00024 : California,San Francisco:82
 00025 : Arizona,Tucson:109
 00026 : Oregon,Salem:85
 00027 : Pennsylvania,Philadelphia:86
 00028 : California,Los Angeles:97
 00029 : Nevada,Reno:108
 
 > 1 30
 00001 : Pennsylvania,Philadelphia:91
 00002 : California,San Francisco:75
 00003 : Nevada,Reno:108
 00004 : Arizona,Flagstaff:81
 00005 : California,Yreka:101
 00006 : Arizona,Tucson:107
 00007 : California,Los Angeles:78
 00008 : California,Los Angeles:81
 00009 : Pennsylvania,Pittsburgh:89
 00010 : Oregon,Salem:90
 00011 : California,Los Angeles:82
 00012 : Arizona,Flagstaff:84
 00013 : California,San Francisco:64
 00014 : Oregon,Salem:83
 00015 : California,San Francisco:68
 00016 : Arizona,Tucson:99
 00017 : California,Yreka:100
 00018 : Arizona,Phoenix:109
 00019 : Oregon,Portland:82
 00020 : Arizona,Tucson:103
 00021 : Oregon,Portland:79
 00022 : Arizona,Phoenix:107
 00023 : California,Cupertino:88
 00024 : California,San Francisco:82
 00025 : Arizona,Tucson:109
 00026 : Oregon,Salem:85
 00027 : Pennsylvania,Philadelphia:86
 00028 : California,Los Angeles:97
 00029 : Nevada,Reno:108
 00030 :
 >
 *****************/
