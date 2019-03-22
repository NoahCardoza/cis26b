/*
 
 Binary Files
 
 There are a number of errors (about 6) in the following program.
 Locate all errors, fix them (as shown below), run the program and save its output.
 
 Here is an example:
 
 FILE fp; // <== Error: Comment the line and write the correct line below
 
 // FILE fp; // Error #1
 FILE *fp;
 
 NAME:
 IDE:
 */
#include <stdio.h>
#include <stdlib.h>  // exit
#include <string.h>
#include <ctype.h>

#define FILENAME "out.dat"

#define RECORD_SIZE sizeof(RECORD)
#define SHOW_RECORDS(x) rewind(fp); \
fread(temp, RECORD_SIZE, x, fp); \
for (k = 0; k < x; k++ ) printf("%s  %s\n", temp[k].id, temp[k].name); \
rewind(fp);

typedef struct
{
//  Error #1 & #2
    char id[5];
    char name[32];
//  char id[4];
} RECORD;

int main(void)
{
    FILE *fp;
    RECORD list[5] = {{"1111", "Alpha"}, {"2222", "Beta"}, {"3333", "Gamma"}, {"4444", "Delta"}, {"5555","Epsilon"}};
    RECORD myList[3] = {{"7777", "Eta"}, {"8888", "Theta"}, {"9999", "Iota"}};
    RECORD temp[5], item = {"6666", "Zeta"}, newItem;
    long pos, cnt;
    int k, n = 5;
    
    if ((fp = fopen(FILENAME, "w+b")) == NULL)
    {
        printf("Could not open %s.\n", FILENAME);
        exit(1);
    }
    
    fwrite(list, 5, RECORD_SIZE, fp);
    SHOW_RECORDS(5);
    
    // calculate the number of records in the file
    fseek(fp, 0, SEEK_END);
    pos = ftell(fp);
    printf("\nThere are %ld records in this file.\n", pos / RECORD_SIZE );
    
    // append one item
    fseek(fp, 0, SEEK_END);
//  Error #3
//  fwrite(item, RECORD_SIZE, 1, fp);
    fwrite(&item, RECORD_SIZE, 1, fp);
    
    // calculate the number of records in the file
    fseek(fp, 0, SEEK_END);
    pos = ftell(fp);
    printf("\nNow there are %ld records in this file.\n", pos / RECORD_SIZE );
    
    // Display the last record
    fseek(fp, -1 * RECORD_SIZE, SEEK_END);
    fread(&newItem, RECORD_SIZE, 1, fp);
    printf("\nLast item is %s.\n", newItem.name);
    
    // append myList
    fseek(fp, 0, SEEK_END);
    fwrite(myList, RECORD_SIZE, 3, fp);
    
    // calculate the number of records in the file
    fseek(fp, 0, SEEK_END);
    pos = ftell(fp);
//  Error #4
//  cnt =  pos / sizeof("RECORD");
    cnt =  pos / RECORD_SIZE;
    printf("\n\nThere are %ld records in this file.\n", cnt );
    SHOW_RECORDS(cnt);
    
    // replace the n-th element by {"0000", "ZERO"} // newItem
//  Error : #5
//  fseek(fp, n * RECORD_SIZE, SEEK_SET); //
    fseek(fp, (n - 1) * RECORD_SIZE, SEEK_SET); //
    strcpy(item.id, "0000");
    strcpy(item.name, "ZERO");
//  Error #6
//
    fseek(fp, -1 * RECORD_SIZE, SEEK_END);
    fwrite(&item, RECORD_SIZE, 1, fp);
    fseek(fp, -1 * RECORD_SIZE, SEEK_CUR);
    fread(&newItem, RECORD_SIZE, 1, fp);
    printf("\n\nThe %d-th record has changed: %s %s.\n", n, newItem.id, newItem.name);
    SHOW_RECORDS(cnt);
    
    fclose(fp);
    
    return 0;
}

/*	================= Sample Output ================= */
/*	Results:
 111  Alpha
 2222  Beta
 3333  Gamma
 4444  Delta
 5555  Epsilon
 
 There are 5 records in this file.
 
 Now there are 6 records in this file.
 
 Last item is Zeta.
 
 
 There are 9 records in this file.
 1111  Alpha
 2222  Beta
 3333  Gamma
 4444  Delta
 5555  Epsilon
 6666  Zeta
 7777  Eta
 8888  Theta
 9999  Iota
 
 
 The 5-th record has changed: 0000 ZERO.
 1111  Alpha
 2222  Beta
 3333  Gamma
 4444  Delta
 5555  Epsilon
 6666  Zeta
 7777  Eta
 8888  Theta
 0000  ZERO
 */
