#include <stdio.h>
#include <stdlib.h>

// a common function used to free malloc'd objects
typedef void (*freeFunction)(void *);

struct Node{
	void *data;
	struct Node *next;
	freeFunction freeFn;
};

/* Function to add a node at the beginning of Linked List.
   This function expects a pointer to the data to be added
   and size of the data type */
void add_node(struct Node** head_ref, void *new_data, size_t data_size, freeFunction freeFn);
int del_node(struct Node** head_ref, void *del_data, size_t data_size);
void list_purge(struct Node** head_ref);
