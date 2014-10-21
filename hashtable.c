/**
 * hashtable.c
 *
 * @author Andreea Pirvulescu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "hash.h"



/**
 * Create new hashtable
 * @in: size hashtable
 * @out: HashP pointer
 */
HashTable *create(int size)
{
    HashP hasht;
    int i;

    if (!(hasht = malloc(sizeof(HashTable))))
    {
        fprintf(stderr, "ERROR: create() failed\n");
        return NULL;
    }

    if(!(hasht->buckets = (BucketsList *)calloc(size,sizeof(BucketsList))))
    {
        fprintf(stderr, "ERROR: create() failed\n");
        return NULL;
    }

    hasht->size = size;

    /**
     * Initialise every list
     	 */
    for (i = 0; i < size; i++)
    {
        hasht->buckets[i] = NULL;
    }

    return hasht;
}



/**
 * Delete the content of a hashtable
 */
void delete_hashtable(HashP hasht)
{
    int i = 0;
    BucketsList *bucket, aux_bucket;

    for(i = 0; i < hasht->size; i++)
    {
        bucket = &(hasht->buckets[i]);
        while ((*bucket) != NULL)
        {
            aux_bucket = *bucket;
            free(aux_bucket->key);
            (*bucket) = (*bucket)->next_bucket;
            free(aux_bucket);
        }
    }

    free(hasht->buckets);
    free(hasht);
}



/**
 * Delete the records from hashtable
 */
void clear(HashP hasht)
{
    int i = 0;
    BucketsList *bucket, aux_bucket;

    for(i = 0; i < hasht->size; i++)
    {
        bucket = &(hasht->buckets[i]);
        while ((*bucket) != NULL)
        {
            aux_bucket = *bucket;
            free(aux_bucket->key);
            (*bucket) = (*bucket)->next_bucket;
            free(aux_bucket);
        }
    }

}



/**
 * Add a new word to a list to hashtable
 * @in: word, HashP pointer
 * @out: 1-fail, 2-success
 */
int add(char *word, HashP hasht)
{
    BucketsList bucketList = NULL, iter = NULL;

    /**
     * Compute word index
     */
    unsigned int index = hash(word, hasht->size);

    iter = hasht->buckets[index];

    if(!(bucketList = calloc(1, sizeof(Bucket))))
    {
        fprintf(stderr, "ERROR: add() failed\n");
        return 1;
    }

    /**
     * If the list is empty, add at the beginning
     * otherwise, add at the end of the list, but check before
     * if the element already exists
     */
    if(iter == NULL)
    {
        bucketList->key = (char*)calloc(strlen(word)+1,sizeof(char));
        strcpy(bucketList->key,word);
        bucketList->next_bucket = NULL;
        hasht->buckets[index]  = bucketList;
    }
    else
    {
        if(strcmp(iter->key, word) == 0)
            return 0;
        while(iter->next_bucket != NULL)
        {
            if(strcmp(word,iter->next_bucket->key) == 0)
            {
                return 0;
            }
            iter = iter->next_bucket;
        }

        bucketList->key = (char*)calloc(strlen(word)+1,sizeof(char));
        strcpy(bucketList->key,word);
        bucketList->next_bucket = NULL;
        iter->next_bucket = bucketList;
    }

    return 0;
}



/**
 * Delete one word from the hashtable
 * @in: word, HashP pointer
 * @out: 1-not found, 0 - deleted
 */
int remove_word(char *word, HashP hasht)
{
    BucketsList bucket;

    /**
     * Compute word index
     */
    unsigned int index = hash(word,hasht->size);

    bucket = hasht->buckets[index];

    /**
     * Check first element, if found is deleted
     * Keep looking for the word
     */
    if(strcmp(bucket->key, word) == 0)
    {
        BucketsList b = hasht->buckets[index];
        hasht->buckets[index] = hasht->buckets[index]->next_bucket;
        free(b->key);
        free(b);
        return 0;
    }
    else
        while(bucket->next_bucket != NULL)
        {
            if(strcmp(bucket->next_bucket->key,word) == 0)
            {
                BucketsList aux_bucket = bucket->next_bucket;
                bucket->next_bucket = aux_bucket->next_bucket;
                free(aux_bucket->key);
                free(aux_bucket);
                return 0;
            }
            else
                bucket = bucket->next_bucket;
        }

    return 1;

}



/**
 * Search for a word
 * @in: word, file, HashP pointer
 * @out: write tostdout or in a file
 */
void find(char *word, char *file, HashTable *hasht)
{
    FILE *f;
    BucketsList bucket;
    unsigned int index = index = hash(word,hasht->size);
    int k = 0;
    int exists=0;

    bucket = hasht->buckets[index];

    if (file != NULL)
    {
        f = fopen(file,"a+");
        if(!f)
            printf("Error at file opening %s\n", file);
    }
    else
    {
        k = 1;
    }

    /**
     * if there is no file given as param
     * write to stdout False or True
     * otherwise write to file True or False
     	 */
    if (k == 1)
    {
        while(bucket)
        {
            if(strcmp(bucket->key, word) == 0)
            {
                printf("True\n");
                exists = 1;
                break;
            }
            else
                bucket = bucket->next_bucket;
        }

        if(exists == 0)
        {
            printf("False\n");
        }

    }
    else
    {
        while(bucket)
        {
            if(!strcmp(bucket->key, word))
            {
                fprintf(f,"True\n");
                exists = 1;
                break;
            }
            else
                bucket = bucket->next_bucket;
        }

        if(exists == 0)
        {
            fprintf(f,"False\n");
            fclose(f);
        }
    }
}



/**
 * Print one bucket(list)
 * @in: bucket index , file, HashP pointer
 * @out: write to stdout or file
 */
void print_bucket(int bucket, char *file, HashP hasht)
{
    FILE *f;
    int k = 0;
    BucketsList bucketList;

    bucketList = hasht->buckets[bucket];

    if (file != NULL)
    {
        f = fopen(file,"a+");
        if(!f)
            printf("Error at opening file %s\n", file);
    }
    else
    {
        k = 1;
    }

    /**
     * if there is no file given as param, write bucket to stdout
     * otherwise write to file
         */
    if (k == 1)
    {
        while(bucketList != NULL)
        {
            printf("%s",bucketList->key);
            if (bucketList->next_bucket != NULL)
            {
                printf(" ");
            }
            bucketList = bucketList->next_bucket;
        }
        printf("\n");
    }
    else
    {
        while(bucketList != NULL)
        {
            fprintf(f,"%s",bucketList->key);
            if (bucketList->next_bucket != NULL)
            {
                fprintf(f," ");
            }
            bucketList = bucketList->next_bucket;
        }
        fprintf(f,"\n");
        fclose(f);
    }
}



/**
 * write all the content of the hashtable to stdout
 * or to file
 */

void print(char *file, HashTable *hasht)
{
    int i;

    for(i = 0; i < hasht->size; i++)
    {
        if(hasht->buckets[i] != NULL)
        {
            print_bucket(i, file, hasht);
        }
    }
}



/**
 * resize hashtable
 * to dimension of 2*old_hashtable
 * and put the reindex the old elements
 * @in,@out: HashP pointer
 */
HashTable resize_double(HashP hasht)
{

    HashTable newHash;
    int i;
    unsigned int new_size;
    BucketsList bucket, next;

    new_size = 2 * hasht->size;
    newHash.size = new_size;

    if(!(newHash.buckets = calloc(new_size, sizeof(BucketsList))))
    {
        fprintf(stderr, "ERROR: resize_double() failed\n");
    }


    for (i = 0; i < hasht->size; i++)
    {
        for(bucket = hasht->buckets[i]; bucket; bucket = next)
        {
            next = bucket->next_bucket;
            add(bucket->key,&newHash);
            remove_word(bucket->key,hasht);
        }

    }

    free(hasht->buckets);
    hasht->size = newHash.size;
    hasht->buckets = newHash.buckets;

    return newHash;
}



/**
 * resize hashtable
 * to dimension of 2*old_hashtable
 * and put the reindex the old elements
 * @in,@out: HashP pointer
 */
HashTable resize_halve(HashP hasht)
{

    HashTable newHash;
    int i;
    unsigned int new_size;
    BucketsList bucket, next;

    new_size = 0.5 * hasht->size;
    newHash.size = new_size;

    if(!(newHash.buckets = calloc(new_size, sizeof(BucketsList))))
    {
        fprintf(stderr, "ERROR: resize_double() failed\n");
    }


    for (i = 0; i < hasht->size; i++)
    {
        for(bucket = hasht->buckets[i]; bucket; bucket = next)
        {
            next = bucket->next_bucket;
            add(bucket->key,&newHash);
            remove_word(bucket->key,hasht);
        }

    }

    free(hasht->buckets);
    hasht->size = newHash.size;
    hasht->buckets = newHash.buckets;

    return newHash;
}
