/**
CIS 26B - Advanced C Programming
Homework #3: 
 Hashing to a file and using advanced string manipulation functions.
 
 This program allows additions to, deletions from, or displays of a database records.

 NAME: Noah Cardoza
 IDE: Xcode
*/
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


#define HASHFILE "hashtable.bin"
#define TAB_SIZE 30
#define TAB_BUF 10
#define BUCKET_SIZE 3 // structs per bucket
#define RECORD_SIZE sizeof(RECORD)
#define SCAN_RECORD(fn, in, r) (fn(in, " %4[0123456789] , %20[^:] : %d", r.id, r.name, &r.qty) == 3)

#define DEFALUT_FILENAME "hardware.txt"
#define REJECTED_FILENAME "rejected.txt"

#define PARSER_SYNTAX_ERROR_MSG "SyntaxError: unable to parse INSERT\n\t"
#define EXIT_SEQ ".q"
#define ERROR_DUPLICATE_HASH 1
#define ERROR_TABLE_OVERFLOW 2

#define DIGITS     "0123456789"

#define RECORD_FORMAT "%s,%s:%d"
#define RECORD_SPREAD(access, r) r access id, r access name, r access qty

#define ASSERT_WITH_ERROR(expression, message) \
if (expression) {                              \
    printf(PARSER_SYNTAX_ERROR_MSG message);   \
    return 0;                                  \
}

//
//def a {
//    return
//}
//
//b() {
//    a
//    print("hell")
//}
//
//b()

#define REJECT_RECORD(fp, access, r) \
    printf("WARNING: (TABLE OVERFLOW) (" RECORD_FORMAT ")\n", RECORD_SPREAD(access, r)); \
    fprintf(fp, "" RECORD_FORMAT "\n", RECORD_SPREAD(access, r)); \


typedef struct {
    char id[5];
    char name[21];
    int  qty;
} RECORD;

FILE *my_fopen(char *filename, char *mode);
void my_fseek(FILE *stream, long offset, int whence);
void rtrim(char *s);
char *ltrim(char *s);
void *my_alloc(size_t size, char *traceback);
char *getlined(FILE *fd, char *def);
void strupr(char * s);
FILE *init_fs_store(char *filename);
long shash(char *key, int size);
int select_by_id(FILE *fp, char *id, RECORD *lookup);
int drop_by_id(FILE *fp, char *id);
int insert_record(FILE* fp, RECORD *r, int *_errno);
//void handle_rejected_record(FILE* fp, RECORD *r);
int parse_record(char *parser, RECORD *record, const char *delems);
void read_record(char *line, FILE *hashfile, FILE *errfp);
void insert_from_file(char *filename, FILE *hashfile);
void ui_loop(FILE *hashfile);
void print_hash_file(FILE* hash_file);

int main(int argc, char *argv[]) {
    FILE *hashfile = init_fs_store(HASHFILE);
    
    if (!*++argv) {
        // if no file arguments open the default file
        insert_from_file(DEFALUT_FILENAME, hashfile);
    } else {
        // otherwise open the files passes in argv
        while (*argv) insert_from_file(*argv++, hashfile);
    }
    
    // start the REPL
    ui_loop(hashfile);
    
    fclose(hashfile);
    return 0;
}


// Wrappers

/*========================================================
 @ wrapper that handles fopen errors
 
 > *filename : the file to be opened
 > *mode     : the mode argument for fopen
 
 < returns a FILE struct
 */

FILE *my_fopen(char *filename, char *mode) {
    FILE *fp;
    
    if (!(fp = fopen(filename, mode))) {
        fprintf(stderr, "Could not open %s : %s\n", filename, strerror(errno));
        exit(errno);
    }
    
    return fp;
}

/*========================================================
 @ wrapper that handles fseak errors
 
 > *stream : a FILE struct
 > offset  : the byte offset
 > whence  : from where to move `offset` bytes
 
 < N/A
 */

void my_fseek(FILE *stream, long offset, int whence){
    if (!fseek(stream, offset, whence)) return;
    perror("Fatal Error : fseek() failed");
    exit(errno);
}

/*  ========================================================
 @ wrapper that handles malloc errors
 
 > size       : how many bytes to malloc
 > *traceback : function name to be dispalyed with the
                error message
 
 < size-many bytes of contiguous memory
 */

void *my_alloc(size_t size, char *traceback) {
    void *ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Error allocating memory in: %s\n", traceback);
        perror("");
        exit(errno);
    }
    return ptr;
}

/*  ========================================================
 @ wrapper for the getline function
 
 > *fd   : pointer to file object
 > *def  : default string to be returned if nothing is
           entered by the user
 
 < A dynamically allocated c-string of either user input or
   the default c-string
 */

char *getlined(FILE *fd, char *def) {
    char *ret = NULL;
    size_t buffer = 32;
    size_t chars;
    
    chars = getline(&ret, &buffer, fd);
    ret[chars - 1] = 0;
    
    if (!*ret) {
        free(ret);
        ret = my_alloc(strlen(def) + 1, "getlined");
        strcpy(ret, def);
    }
    
    return ret;
}

// Utils

/*========================================================
 @ trims a c-string from the right
 
 > *s : the c-string to trim
 
 < sets all whitespace up till the first non-whitespace
 character from the right
 */

void rtrim(char *s) {
    unsigned long end = strlen(s) - 1;
    while (end >= 0 && isspace(s[end]))
        s[end--] = 0;
}

/*========================================================
 @ trims a c-string from the left
 
 > *s : the c-string to trim
 
 < returns a pointer moved to the first non-whitespace
 character from the left
 */

char *ltrim(char *s) {
    while (isspace(*s)) s++;
    return s;
}

/*========================================================
 @ traverses c-string calling toupper
 
 > *s : the c-string to iterate over
 
 < modifies `s` converting all characters to uppercase
 */

void strupr(char * s) {
    for (; *s; s++) *s = (char) toupper(*s);
}

/*========================================================
 @ hashes the key c-string between 0 and `size`
 
 > *key : the c-string to hash
 > size : max number returned
 
 < sums the cube of each characters ASCII value restricted
   to [0, size]
 */

long shash(char *key, int size) {
    long address = 0;
    for (; *key; key++) address += *key * *key * *key;
    return address % size;
}

/*========================================================
 @ opens and writes an empty hashmap to `filename`
 
 > *filename : the file to be opened
 
 < a FILE struct with an empty hashtable written to it
 */

FILE *init_fs_store(char *filename) {
    FILE *fp = my_fopen(filename, "w+b");
    RECORD hashtable[TAB_BUF][BUCKET_SIZE] = { 0 };
    
    for (int i = TAB_SIZE / TAB_BUF; i > 0; i--) {
        if (fwrite(*hashtable, RECORD_SIZE, TAB_BUF * BUCKET_SIZE, fp) < TAB_BUF){
            perror("Hash table could not be created. Abort!");
            exit(errno);
        };
    }
    
    rewind(fp);
    return fp;
}

/*========================================================
 @ searchs the hashmap for a record by `id`
 
 > *fp     : the hashmap
 > *id     : c-string of the id to search for
 > *lookup : the record to be filled with the queried data
 
 < 1/0 if the record was found
 */

int select_by_id(FILE *fp, char *id, RECORD *lookup) {
    long address = shash(id, TAB_SIZE);
    
    fseek(fp, address * RECORD_SIZE * BUCKET_SIZE, SEEK_SET);
    
    for (int i = BUCKET_SIZE; i--;) {
        fread(lookup, RECORD_SIZE, 1, fp);
        if (!strcmp(lookup->id, id))
            return 1;
    }
    
    return 0;
}

/*========================================================
 @ deletes a record by `id` from the hashmap
 
 > *fp : the hashmap
 > *id : c-string of the id to search for
 
 < 1/0 if the record was found/dropped
 */


int drop_by_id(FILE *fp, char *id) {
    RECORD lookup;
    long address = shash(id, TAB_SIZE);
    
    fseek(fp, address * RECORD_SIZE * BUCKET_SIZE, SEEK_SET);
    
    for (int i = BUCKET_SIZE; i--;) {
        fread(&lookup, RECORD_SIZE, 1, fp);
        if (!strcmp(lookup.id, id)) {
            fseek(fp, -RECORD_SIZE, SEEK_CUR);
            memset(&lookup, 0, RECORD_SIZE);
            fwrite(&lookup, RECORD_SIZE, 1, fp);
            return 1;
        }
    }
    return 0;
}

/*========================================================
 @ inserts a record by `r->id` into the hashmap
 
 > *fp     : the hashmap
 > *r      : a record holding the data to be copied
 > *_errno : used to determin what caused the insertion
             failure
 
 < 1/0 if the record was successfully inserted
 */

int insert_record(FILE* fp, RECORD *r, int *_errno) {
    RECORD lookup;
    long address = shash(r->id, TAB_SIZE);
    
    my_fseek(fp, address * RECORD_SIZE * BUCKET_SIZE, SEEK_SET);
    
    for (int i = 0; i < BUCKET_SIZE; i++) {
        fread(&lookup, RECORD_SIZE, 1, fp);
        
        if (!*lookup.id) {
            my_fseek(fp, -1 * RECORD_SIZE, SEEK_CUR);
            fwrite(r, RECORD_SIZE, 1, fp);
            return 1;
        } else {
            if (!strcmp(lookup.id, r->id)) {
                if (_errno) *_errno = ERROR_DUPLICATE_HASH;
                return 0;
            }
        }
    }
    if (_errno) *_errno = ERROR_TABLE_OVERFLOW;
    return 0;
}

/*========================================================
 @ parses a c-string providing verbose errors
 
 > *parser : the c-string to parse
 > *record : a record to copy the data to
 > *delems : used to break up the information
             "ID {delem[0]} NAME {delem[1]} QTY"
 
 < 1/0 if the record was parsed successfully
 */

int parse_record(char *parser, RECORD *record, const char *delems) {
    char *tokens[] = {
        parser,
        strrchr(parser, delems[0]),
        strrchr(parser, delems[1]),
        NULL
    };
    
    // make sure the delimiters exist and are in the correct order
    for (const char *token = delems; *token; token++) {
        if (!(parser = strchr(parser, *token))) {
            printf(PARSER_SYNTAX_ERROR_MSG "the delimiter `%c` is missing or out of order\n", *token);
            return 0;
        } else if (parser + 1 && strchr(parser + 1, *token)) { // check if there are two
            printf(PARSER_SYNTAX_ERROR_MSG "the delimiter `%c` can only be used once\n", *token);
            return 0;
        }
    }

    // break up string and trim tokens
    *tokens[1]++ = *tokens[2]++ = 0;
    for (char **token = tokens; *token; token++) {
        *token = ltrim(*token);
        rtrim(*token);
    }
    
    // parse / validate id
    parser = tokens[0];
    ASSERT_WITH_ERROR(strlen(parser) != 4, "<int: id[4]> must be exactly 4 digits long\n")
    ASSERT_WITH_ERROR(strspn(parser, DIGITS) != 4, "<int: id[4]> must be digits only\n")
    sscanf(parser, " %4[" DIGITS "] ", record->id);
    
    // parse / validate name
    parser = tokens[1];
    ASSERT_WITH_ERROR(!*parser, "<str: name[:20]> is too sort! It must be more than 1 character\n")
    ASSERT_WITH_ERROR(strlen(parser) > 20,                      "<str: name[:20]> is too long! It must be less than 20 characters\n")
    strcpy(record->name, parser);
    
    // parse / validate qty
    parser = tokens[2];
    ASSERT_WITH_ERROR(!*parser || strspn(parser, DIGITS) != strlen(parser), "<int: qty> must be a number\n")
    sscanf(parser, "%d", &record->qty);
    
    return 1;
}

/*========================================================
 @ reads and inserts a record from a c-string
 
 > *line     : the c-string to read the record from
 > *hashfile : the hashmap
 > *errfp    : file to log overflows to
 
 < N/A
 */

void read_record(char *line, FILE *hashfile, FILE *errfp) {
    RECORD record;
    int myerrno;
    if (parse_record(line, &record, ",:")) {
        strupr(record.name);
        if (!insert_record(hashfile, &record, &myerrno)){ // handle overflow
            if (myerrno == ERROR_TABLE_OVERFLOW) { REJECT_RECORD(errfp, ., record) }
            else if (myerrno == ERROR_DUPLICATE_HASH)
                printf("WARNING: (DUPLICATE ID) (" RECORD_FORMAT ")\n", RECORD_SPREAD(., record));
        }
    }
}

/*========================================================
 @ reads records from a file and inserts them into the
   hastmap
 
 > *filename : the file to read from
 > *hashfile : the hashmap
 
 < N/A
 */

void insert_from_file(char *filename, FILE *hashfile) {
    RECORD record;
    FILE *hash_fp = my_fopen(filename, "r");
    FILE *rejc_fp = my_fopen(REJECTED_FILENAME, "a");
    
    while (SCAN_RECORD(fscanf, hash_fp, record)){
        rtrim(record.name);
        strupr(record.name);
        
        if (!insert_record(hashfile, &record, NULL)) { REJECT_RECORD(rejc_fp, ., record) } // handle overflow - files should not include duplicate ids
    }
    
    fclose(rejc_fp);
    fclose(hash_fp);
}

/*========================================================
 @ a basic REPL allowing the user to preform basic CR(U)D
   operations
 
 > *hashfile : the hashmap
 
 < N/A
 */

void ui_loop(FILE *hashfile){
    char *propmt;
    char *parser;
    RECORD record;
    FILE *rejc_fp = my_fopen(REJECTED_FILENAME, "a");
    char  id[5];
    
    printf("Hashmap Lookup Helper         \n"
           "> .help # for more information\n");
    
    // could be optimized
    while ((void)printf("> "), (void)((propmt = getlined(stdin, EXIT_SEQ))), strncmp(propmt, EXIT_SEQ, strlen(EXIT_SEQ))) {
        parser = propmt;
        if        (!strncmp(propmt, "INSERT", 6)) {
            parser += 6;
            read_record(parser, hashfile, rejc_fp);
        } else if (!strncmp(propmt, "SELECT BY id=", 13)) {
            parser = ltrim(parser + 13);
            sscanf(parser, " %s %*[^\n]", id);
            strupr(id);
            
            if (select_by_id(hashfile, id, &record)) {
                printf(RECORD_FORMAT "\n", RECORD_SPREAD(., record));
            } else {
                printf("LookUpError: cannot find record with id=`%s`\n", id);
            }
        } else if (!strncmp(propmt, "DROP WHERE id=", 14)) {
            parser += 14;
            sscanf(parser, " %s %*[^\n]", id);
            strupr(id);
            if (!drop_by_id(hashfile, id)) {
                printf("DropWarning: cannot find record with id=`%s`\n", id);
            }
        } else if (!strncmp(propmt, ".help", 5)) {
            parser += 5;
            printf("==================================================\n"
                   "Hashmap Lookup Helper                             \n"
                   "==================================================\n"
                   "INSERT <int: id[4]>, <str: name[:20]> : <int: qty>\n"
                   "\tInserts a record into the hashmap.              \n\n"
                   "SELECT BY id=<int: id[4]>                         \n"
                   "\tSelects a record by id.                         \n\n"
                   "DROP WHERE id=<int: id[4]>                        \n"
                   "\tDeletes a record by id from the hashmap.        \n\n"
                   ".help                                             \n"
                   "\tDisplays this message.                          \n\n"
                   ".q                                                \n"
                   "\tQuits the program. An empty line is also        \n"
                   "\tsufficient.                                     \n");
        } else {
            printf("SyntaxError: cannot understand `%s`\n", propmt);
        }
        free(propmt);
        propmt = NULL;
    }
    free(propmt);
    propmt = NULL;
    fclose(rejc_fp);
}

/* Error Testing
 = ./program
 ---------------
 Hashmap Lookup Helper
 > .help # for more information
 > INSERT
 SyntaxError: unable to parse INSERT
 the delimiter `,` is missing or out of order
 > INSERT 4563
 SyntaxError: unable to parse INSERT
 the delimiter `,` is missing or out of order
 > INSERT 3790,hello:
 SyntaxError: unable to parse INSERT
 <int: qty> must be a number
 > INSERT 3790,hello: 8674
 > INSERT 3790,hello: 8674
 WARNING: (DUPLICATE ID) (3790,HELLO:8674)
 > INSERT 3790,: 8674
 SyntaxError: unable to parse INSERT
 <str: name[:20]> is too sort! It must be more than 1 character
 > INSERT ,:8674
 SyntaxError: unable to parse INSERT
 <int: id[4]> must be exactly 4 digits long
 > INSERT 7482,1234567890123456789012345: 8674
 SyntaxError: unable to parse INSERT
 <str: name[:20]> is too long! It must be less than 20 characters
 > INSERT 1234,Item:50
 > INSERT 4321   ,   Item  :   50
 > INSERT 3421   ,   Item  :   50
 WARNING: (TABLE OVERFLOW) (3421,ITEM:50)
 > INSERT 3412,   Item:50
 WARNING: (TABLE OVERFLOW) (3412,ITEM:50)
 > INSERT ,:8
 SyntaxError: unable to parse INSERT
 <int: id[4]> must be exactly 4 digits long
 > SELECT
 SyntaxError: cannot understand `SELECT`
 > SELECT BY
 SyntaxError: cannot understand `SELECT BY`
 > SELECT BY id=1234
 1234,ITEM:50
 > SELECT BY id=12347
 LookUpError: cannot find record with id=`12347`
 > SELECT BY id=1111
 LookUpError: cannot find record with id=`1111`
 > DROP WITH id=1111
 SyntaxError: cannot understand `DROP WITH id=1111`
 > DROP WHERE id=1111
 > DROP WHERE id=1234
 DropWarning: cannot find record with id=`1234`
 > SELECT BY id=1234
 LookUpError: cannot find record with id=`1234`
 > .q
 */

/* Functionality Test
 = ./program hardware.txt more_hardware.txt
 --------------------
 Hashmap Lookup Helper
 > .help # for more information
 > .help
 ==================================================
 Hashmap Lookup Helper
 ==================================================
 INSERT <int: id[4]>, <str: name[:20]> : <int: qty>
    Inserts a record into the hashmap.
 
 SELECT BY id=<int: id[4]>
    Selects a record by id.
 
 DROP WHERE id=<int: id[4]>
    Deletes a record by id from the hashmap.
 
 .help
    Displays this message.
 
 .q
    Quits the program. An empty line is also
    sufficient.
 > SELECT BY id=4717
 4717,BRACE:234
 > DROP WHERE id=4717
 > SELECT BY id=4717
 LookUpError: cannot find record with id=`4717`
 > INSERT 1234, Product Name 123 : 42
 > SELECT BY id=1234
 1234,PRODUCT NAME 123:42
 */
