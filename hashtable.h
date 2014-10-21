/**
 * hashtable.h
 * @author: Andreea Pirvulescu
 */

#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_



/**
 * Structure for node of a list
 */
typedef struct bucket {
	char *key;
	struct bucket *next_bucket;
} Bucket;

typedef Bucket *BucketsList;



/**
 * structure for hashtable
 */
typedef struct {
	int size;
	BucketsList *buckets;
} HashTable;



/**
 * Pointer to HashTable
 */
typedef HashTable *HashP;


HashTable *create(int size);
void delete_hashtable(HashP hashtable);
void clear(HashP hashtable);
int add(char *word, HashP hashtable);
int remove_word(char *word, HashP hashtable);
void find(char *word, char *file, HashP hashtable);
void print_bucket(int bucket, char *file, HashP hashtable);
void print(char *file, HashP table);
HashTable resize_double(HashP hashtable);
HashTable resize_halve(HashP hashtable);

#endif
