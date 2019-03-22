/*********************************************************
    Examples: 5.3
    Hashing to Disk
/*********************************************************/

#include <stdio.h>
#include <stdlib.h>  // exit
#include <string.h>

#define FILENAME "e_5_3.dat"
#define TABSIZE 100
#define OFLOWSIZE 100
#define BUCKETSIZE 4 // structs per bucket
#define INSERT 1
#define SEARCH 0

typedef struct record RECORD;
struct record
{
    char key[50]; // a very simple record
    int  num;     // a random number
};

long hash ( char *key, int size);

//int main(int argc, char *argv[])
int main(void)
{
    FILE *create_hash_file(char *filename);
    void search_or_insert(FILE *fp, int insertflag);

    FILE *fp;

    //fp = create_hash_file(argv[1]);
    fp = create_hash_file(FILENAME);
    search_or_insert(fp, INSERT);
    search_or_insert(fp, SEARCH);

    return 0;
}

/*******************************************************
   Hash formula is the same as used in Chapter 3
*/
long hash ( char *key, int size)
{
    long address = 0;
    for (   ; *key != '\0'; key++)
    {
        address += *key * *key * *key;
    }
    return address % size;
}

/********************************************************
   Create an empty hash file
*/
FILE *create_hash_file(char *filename)
{
    FILE *fp;
    //RECORD hashtable[TABSIZE][BUCKETSIZE] = {{"", "", "", ""}};
    RECORD hashtable[TABSIZE][BUCKETSIZE] = {""};
    RECORD overflow[OFLOWSIZE] = {""};

    if ((fp = fopen(filename, "w+b")) == NULL)
    {

        printf("Could not open %s.\n", filename);
        exit(1);
    }

    // check for write errors just to be safe.
    if (fwrite(&hashtable[0][0], sizeof (RECORD), TABSIZE * BUCKETSIZE, fp) < TABSIZE)
    {
        printf("Hash table could not be created. Abort!\n");
        exit(2);
    }

    if (fwrite(overflow, sizeof (RECORD), OFLOWSIZE, fp) < OFLOWSIZE)
    {
        printf("Could not create overflow area. Abort!\n");
        exit(3);
    }
    rewind(fp);
    return fp;
}

/*****************************************************************
   Hashing to disk: search or insert
*/
void search_or_insert(FILE *fp, int insertflag)
{
    void insert_record(char *key, long address, FILE *fp);
    void search_record(char *key, long address, FILE *fp);

    char line[100], *token;
    long address;

    printf("%s\n", insertflag ? "INSERT" : "SEARCH");
    while(printf("Enter key: "), gets(line), strcmp(line, "quit") != 0)
    {
        token = strtok(line, "\040\t");
        if (!token) continue;
        address = hash(line, 100);
        if (insertflag)
        {
            insert_record(token, address, fp);
        }
        else
        {
            search_record(token, address, fp);
        }
    }
    return;
}

/******************************************************
   Hashing to Disk: Insert
*/
void insert_record(char *key, long address, FILE *fp)
{
    RECORD detect, temp;
    int i;

    strcpy(temp.key, key);
    temp.num = rand()%99 + 5000;
    // go to beginning of hash bucket
    if (fseek(fp, address * BUCKETSIZE * sizeof (RECORD), SEEK_SET) != 0)
    {
        printf("Fatal seek error! Abort!\n");
        exit(4);
    }
    // find first available slot in the bucket.
    for (i = 0; i < BUCKETSIZE; i++)
    {
        fread(&detect, sizeof(RECORD), 1, fp);
        if (*detect.key == '\0') // available slot
        {
            fseek(fp, -1L * sizeof(RECORD), SEEK_CUR);
            fwrite(&temp, sizeof (RECORD), 1, fp);
            printf("Record: %s :added to bucket %ld.\n", temp.key, address);
            return; // nothing left to do
        }
    }

    // bucket full: insert into the overflow area
    fseek(fp, TABSIZE * BUCKETSIZE * sizeof(RECORD), SEEK_SET);
    for (i = 0; i < OFLOWSIZE; i++)
    {
        fread(&detect, sizeof(RECORD), 1, fp);
        if (*detect.key == '\0') // available slot
        {
            fseek(fp, -1L * sizeof (RECORD), SEEK_CUR);
            fwrite(&temp, sizeof (RECORD), 1, fp);
            printf("Record: %s : added to the overflow slot %d.\n", temp.key, i);
            return; // nothing left to do
        }
    }
    // item not inserted!
    printf("Hash table overflow! Abort!\n");
    exit(5);
}

/******************************************************
   Hashing to Disk: Search
*/
void search_record(char *key, long address, FILE *fp)
{
    RECORD detect, temp;
    int i;

    if (fseek(fp, address * BUCKETSIZE * sizeof (RECORD), SEEK_SET) != 0)
    {
        printf("Fatal seek error! Abort");
        exit(4);
    }
    // find first available slot in bucket
    for (i = 0; i < BUCKETSIZE; i++)
    {
        fread(&detect, sizeof (RECORD), 1, fp);
        if (strcmp(detect.key, key) == 0) // found it!
        {
            printf("\t%s found \n\tat hash bucket %ld.\n", key, address);
            printf("\tRandom number is %d\n", detect.num);
            return; // nothing left to do
        }
    }
    // check the overflow area
    fseek(fp, TABSIZE * BUCKETSIZE * sizeof (RECORD), SEEK_SET);
    for (i = 0; i < OFLOWSIZE; i++)
    {
        fread(&detect, sizeof (RECORD), 1, fp);
        if (strcmp(detect.key, key) == 0) // found it!
        {
            printf("\t%s found in overflow area.\n", key);
            printf("\tRandom number is %d\n", detect.num);
            return; // nothing left to do
        }
    }
    // not found
    printf("Recors with key: %s : not found.\n", key);
    return;
}

