#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define capacity 100

struct list_array {
	char *key;
	char *value;
	struct list_array *next;
};

struct hashTable {
	struct list_array **buckets;
};

void init_hash(struct hashTable *hashT);
void insert_hash_table(struct hashTable *hashT, const char *key,
			const char *value);
void erase_hash_table(struct hashTable *hashT, const char *key);
char *get_hash_table(struct hashTable *hashT, const char *key);
void free_hash(struct hashTable *h);
