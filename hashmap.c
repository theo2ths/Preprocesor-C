#include "hashmap.h"

static int hash_function(const char *key)
{
	int i = 0;
	int hash = 0;
	int my_str;

	my_str = strlen(key);
	for (i = 0; i < my_str; ++i)
		hash = (hash * 31 + key[i]) % capacity;
	return hash;
}
static void insert_list_array(struct list_array **head, const char *key,
			const char *value)
{
	struct list_array *new_head;
	struct list_array *n;

	new_head = NULL;
	n = *head;
	while (n != NULL) {
		if (!strcmp(n->key, key)) {
			free(n->value);
			n->value = (char *)malloc(strlen(value) + 1);
			if (n->value == NULL)
				exit(12);
			memcpy(n->value, value, strlen(value) + 1);
			return;
		}
		n = n->next;
	}
	new_head = (struct list_array *)malloc(sizeof(struct list_array));
	if (new_head == NULL)
		exit(12);
	new_head->key = (char *)malloc(strlen(key) + 1);
	if (new_head->key == NULL)
		exit(12);
	new_head->value = (char *)malloc(strlen(value) + 1);
	if (new_head->value == NULL)
		exit(12);
	memcpy(new_head->key, key, strlen(key) + 1);
	memcpy(new_head->value, value, strlen(value) + 1);
	new_head->next = *head;
	*head = new_head;
}

static void delete_list_by_pointer(struct list_array **head,
				struct list_array *prev)
{
	struct list_array *n;

	if (prev == NULL) {
		if (head == NULL)
			return;
		n = *head;
		(*head) = (*head)->next;
		free(n->key);
		free(n->value);
		free(n);
	} else {
		struct list_array *n;

		n = prev->next;
		prev->next = prev->next->next;
		free(n->value);
		free(n->key);
		free(n);
	}
}

static void delete_list(struct list_array **head, const char *key)
{
	struct list_array *n;
	struct list_array *prev = NULL;

	n = *head;
	while (n != NULL) {
		if (!strcmp(n->key, key)) {
			delete_list_by_pointer(head, prev);
			return;
		}
		prev = n;
		n = n->next;
	}
}

static char *get_list(struct list_array *head, const char *key)
{
	struct list_array *n;
	char *value = NULL;

	n = head;
	while (n != NULL) {
		if (!strcmp(n->key, key)) {
			value = (char *)malloc(strlen(n->value) + 1);
			if (value == NULL)
				exit(12);
			memcpy(value, n->value, strlen(n->value) + 1);
			return value;
		}
		n = n->next;
	}
	return NULL;
}

void init_hash(struct hashTable *hashT)
{
	hashT->buckets =
	(struct list_array **)calloc(capacity, sizeof(struct list_array *));
	if (hashT->buckets == NULL)
		exit(12);
}

void insert_hash_table(struct hashTable *hashT, const char *key,
			const char *value)
{
	int hash = hash_function(key);

	insert_list_array(&(hashT)->buckets[hash], key, value);
}

void erase_hash_table(struct hashTable *hashT, const char *key)
{
	int hash = hash_function(key);

	delete_list(&(hashT)->buckets[hash], key);
}

char *get_hash_table(struct hashTable *hashT, const char *key)
{
	int hash = hash_function(key);

	return get_list(hashT->buckets[hash], key);
}

static void free_list(struct list_array *list)
{
	if (list == NULL)
		return;
	if (list->next)
		free_list(list->next);
	if (list->key)
		free(list->key);
	if (list->value)
		free(list->value);
	if (list)
		free(list);
}

void free_hash(struct hashTable *h)
{
	int i = 0;

	if (h == NULL)
		return;
	for (i = 0; i < capacity; ++i)
		if (h->buckets[i])
			free_list(h->buckets[i]);
	free(h->buckets);
	if (h)
		free(h);
}
