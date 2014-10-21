/**
 * HashtTable
 *
 * @author Andreea Pirvulescu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "hash.h"

#define ADD "add"
#define RM "remove"
#define CL "clear"
#define FN "find"
#define PB "print_bucket"
#define PR "print"
#define R "resize"
#define D "double"
#define H "halve"


/**
 * Parse one line and call the requested function
 * @in: string(one line from a file), HashP pointer
 */
void execute_command(char *buf, HashP ht)
{
    char *first, *second, *third;
    int index;

    first = strtok(buf, " ");

    if (strcmp(ADD, first) == 0)
    {
        second = strtok(NULL, " ");
        add(second, ht);
    }
    else if (strcmp(RM, first) == 0)
    {
        second = strtok(NULL, " ");
        remove_word(second, ht);
    }
    else if (strcmp(CL, first) == 0)
    {
        clear(ht);
    }
    else if (strcmp(FN, first) == 0)
    {
        second = strtok(NULL, " ");
        third = strtok(NULL, " ");
        find(second, third, ht);
    }
    else if (strcmp(PB, first) == 0)
    {
        second = strtok(NULL, " ");
        third = strtok(NULL, " ");
        sscanf(second, "%d", &index);
        print_bucket(index, third, ht);
    }
    else if (strcmp(PR, first) == 0)
    {
        second = strtok(NULL, " ");
        print(second, ht);
    }
    else if (strcmp(R, first) == 0)
    {
        second = strtok(NULL, " ");
        if (strcmp(D, second) == 0)
        {
            resize_double(ht);
        }
        else if (strcmp(H, second) == 0)
        {
            resize_halve(ht);
        }
    }
    else
    {
        printf("Unrecognized command.");
    }
}



/**
 * Open file and send one line at a time to execute_command()
 * @in: file location, number of files, HashP pointer
 * @out: 1-fail, 0-success
 */
int handle_file(char **names, int n, HashP ht)
{
    int i;
    FILE *f;
    char buffer[20001];

    for (i = 0; i < n; i++)
    {
        f = fopen(names[i],"r");
        if (!f)
        {
            printf("Error at opening file %s!\n", names[i]);
            return 1;
        }
        else
        {
            while (fscanf(f, "%[^\n]\n)", buffer) == 1)
            {
                execute_command(buffer, ht);
            }
        }

        fclose(f);
    }

    return 0;
}



int main(int argc, char **argv)
{
    char **name_files;
    unsigned int size;
    int i;
    HashP hasht;

    if(argc > 2)
        name_files = malloc((argc-1) * sizeof(char*));

    for(i = 2; i < argc; i++)
    {
        name_files[i-2] = strdup(argv[i]);
    }


    size = atoi(argv[1]);

    if(!(hasht = create(size)))
    {
        fprintf(stderr, "ERROR: create() failed\n");
    }

    handle_file(name_files, argc-2, hasht);


    for (i = 0; i < argc-2; i++)
    {
        free(name_files[i]);
    }

    free(name_files);
    delete_hashtable(hasht);

    return 0;
}
