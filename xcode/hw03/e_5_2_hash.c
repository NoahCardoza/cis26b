/*********************************************************
    Examples: 5.2
    Write to and Reads from a Hashed File
/*********************************************************/

#include <stdio.h>
#include <stdlib.h>  // exit
#include <string.h>

#define FILENAME "e_5_2.dat"

struct record
{
    char key[50]; // a very simple record
    int  num;     // a random number
};

long hash ( char *key, int size);

//int main(int argc, char *argv[])
int main(void)
{
    FILE *fp;
    struct record hashtable[100] = {""}; // all bytes set to 0!
    struct record temprec;
    long address;
    char line[100];

    //if ((fp = fopen(argv[1]), "w+b")) == NULL)
    if ((fp = fopen(FILENAME, "w+b")) == NULL)
    {
        //printf("Could not open %s.\n", argv[1]);
        printf("Could not open %s.\n", FILENAME);
        exit(1);
    }
    while(printf("Enter key: "), gets(line), strcmp(line, "quit") != 0)
    {
        address = hash(line, 100);
        strcpy(hashtable[address].key, line);
        hashtable[address].num = rand()%99 + 5000;
        printf("Hash value on output is %ld \n", address);
    }

    // Write entire hash table to disk
    fwrite(hashtable, sizeof(struct record), 100, fp);
    printf("\nEnd of write. Beginning of reads.\n\n");

    while(printf("Enter key: "), gets(line), strcmp(line, "quit") != 0)
    {
        address = hash(line, 100);
        fseek(fp, address * sizeof (struct record), SEEK_SET);
        fread(&temprec, sizeof (struct record), 1, fp);
        // if the first byte of the requested record is 0
        // then the record is not there!
        if (memcmp(&temprec, "", 1) == 0)
        {
            printf("Sorry! Record not found!\n");
        }

        printf("\tHash value on input is: %ld\n"
               "\tKey is: %s\n"
               "\tRandom number is: %d \n",
                 address, temprec.key, temprec.num);
    }

    fclose(fp);

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

