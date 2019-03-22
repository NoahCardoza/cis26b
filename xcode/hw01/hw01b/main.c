/***
 CIS 26B: Homework 1B
 ********************
 NAME: Noah Cardoza
 IDE (compiler): Xcode (gcc)
 *************************************************************************
 
 Review: Functions, Structures, Pointers, Arrays, Sorting, Data Files,
 and Dynamic Allocation of Memory
 
 Create and process a dynamically allocated array of structures
 Save the output as a comment at the end of the program!
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifdef _MSC_VER
#include <crtdbg.h>  // needed to check for memory leaks (Windows only!)
#endif

typedef struct {
    char symbol[25];
    int shares;
} COMPANY;

void swap(COMPANY *a, COMPANY *b);
void insertCompany(COMPANY *companies, int end);
int readStocksFile(char *in, COMPANY **companies);
void printStocksList(FILE *fd, COMPANY *companies, int records, int header);
char *my_getline(FILE *fd);
void *select_or(void *a, void *b);

int main(void) {
    const char *DEFAULT_IFILE = "in.txt";
    const char *DEFAULT_OFILE = "out.txt";
    
    COMPANY *companies;
    char *ifile, *ofile;
    int records;
    FILE *fout;
    
    printf("Input File: ");
    ifile = select_or(my_getline(stdin), (void*) DEFAULT_IFILE);
    printf("Output File: ");
    ofile = select_or(my_getline(stdin), (void*) DEFAULT_OFILE);

    if ((records = readStocksFile(ifile, &companies)) == -1) {
        perror("An error occured while reading the input file");
        free(companies);
        if (ifile != DEFAULT_IFILE) free(ifile);
        if (ofile != DEFAULT_OFILE) free(ofile);
        exit(errno);
    }
    
    // prove sort works
    printStocksList(stdout, companies, records, 1);
    
    // save results to file
    if (!(fout = fopen(ofile, "w"))) {
        perror("An error occured while writing to the output file");
        free(companies);
        if (ifile != DEFAULT_IFILE) free(ifile);
        if (ofile != DEFAULT_OFILE) free(ofile);
        exit(errno);
    }
    
    printStocksList(fout, companies, records, 0);
    fclose(fout);
    
    // free all malloced memory
    free(companies);
    if (ifile != DEFAULT_IFILE) free(ifile);
    if (ofile != DEFAULT_OFILE) free(ofile);
    
    #ifdef _MSC_VER
        printf( _CrtDumpMemoryLeaks() ? "Memory Leak\n" : "No Memory Leak\n");
    #endif
    
    return 0;
}

/*  ========================================================
 Swaps COMPANY pointers
 Pre
 *a  : a pointer to a company in the company list
 *b  : a pointer to a company in the company list
 Post
 N/A
 */

void swap(COMPANY *a, COMPANY *b){
    COMPANY temp = *a;
    *a = *b;
    *b = temp;
}

/*  ========================================================
 modified insertion sort
 Sorts the latest company added to the list into place
 Pre
    *companies  : a list of companies
    end         : the last initalized company location in the list
 Post
 N/A
 */

void insertCompany(COMPANY *companies, int end) {
    for (int i = end; i > 0; i--) {
        if (strcmp(companies[i - 1].symbol, companies[i].symbol) > 0) {
            swap(companies + i - 1, companies + i);
        } else return; // since the array is already sorted we can stop here
    }
}

/*  ========================================================
 Dumps the consolidated stock data to a specified output
 Pre
    *in      : string path to the file to open
    **companies  : points to the list of companies
 Post
    the number of elements in the cpmms array
 */

int readStocksFile(char *in, COMPANY **companies) {
    int records, count;
    FILE *inf;
    
    inf = fopen(in, "r");
    
    if (!inf) {
        return -1;
    }
    
    fscanf(inf, "%d", &records);
    (*companies) = (COMPANY*) calloc(records, sizeof(COMPANY));
    
    for (int i = 0; i < records; i++) {
        count = fscanf(inf, "%s %d", (*companies)[i].symbol, &(*companies)[i].shares);
        
        if (count == EOF || count != 2) return -1;
        insertCompany((*companies), i);
    }
    
    fclose(inf);
    
    return records;
}

/*  ========================================================
 Dumps the consolidated stock data to a specified output
 Pre
    *fd         : the output to write to
    *companies  : the list of companies
    records     : the number of companies
    header      : 0 or 1 depending on if the header should be preinted
 Post
 N/A
 */

void printStocksList(FILE *fd, COMPANY *companies, int records, int header) {
    int totalShares = 0;
    
    if (header) fprintf(fd, "Symbol    Shares\n");
    
    for (int i = 0; i < records; i++) {
        totalShares = companies[i].shares;
        while (i + 1 < records && !strcmp(companies[i].symbol, companies[i + 1].symbol)) {
            totalShares += companies[++i].shares;
        }
        fprintf(fd, "%-10s%d\n", companies[i].symbol, totalShares);
    }
}

/*  ========================================================
 Get a line of input
 Pre
 *fd : the input to read from
 Post
 a malloced c string or null if no input was given
 */

char *my_getline(FILE *fd) {
    char *ret;
    ssize_t chars;
    size_t bufsize = 32;
    
    chars = getline(&ret, &bufsize, fd);
    ret[chars - 1] = 0;                     // remove trailing newline character
    if (*ret == 0) {
        free(ret);
        return NULL;
    }
    return ret;
}

/*  ========================================================
 Returns the "true" argument
 Pre
 *a : a pointer to random memory
 *b : a pointer to random memory
 Post
 return the non-NULL pointer if any, otherwise return `*b`
 */

void *select_or(void *a, void *b) {
    if (a) return a;
    return b;
}
/*    ================= Output ================= */
/*
 Input File:
 Output File:
 Symbol    Shares
 AAL       369
 AAPL      2879
 BBBY      471
 CHTR      423
 EBAY      899
 GOOG      123
 GOOGL     123
 MSFT      380
 NLFX      1474
 SIRI      1908
 TSLA      1383
 XLNX      350
 YHOO      458
 */
