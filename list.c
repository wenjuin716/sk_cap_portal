#include <string.h>
#include "list.h"

/* Function to add a node at the beginning of Linked List.
   This function expects a pointer to the data to be added
   and size of the data type */
void add_node(struct Node** head_ref, void *new_data, size_t data_size, freeFunction freeFn)
{
	// Allocate memory for node
	struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
 
	new_node->data  = malloc(data_size);
	new_node->next = (*head_ref);
	new_node->freeFn = freeFn;
 
	// Copy contents of new_data to newly allocated memory.
	// Assumption: char takes 1 byte.
#if 1
	memcpy(new_node->data, new_data, data_size);
#else
	int i;
	for (i=0; i<data_size; i++)
		*(char *)(new_node->data + i) = *(char *)(new_data + i);
#endif

	// Change head pointer as new node is added at the beginning
	(*head_ref)    = new_node;
}

void list_purge(struct Node** head_ref)
{
	struct Node *current;
	while(*head_ref != NULL) {
		current = *head_ref;
		*head_ref = current->next;

		// free the Node data
		if(current->freeFn) {
			current->freeFn(current->data);
		}

		//free(current->data);
		free(current);
	}
}

int del_node(struct Node** head_ref, void *del_data, size_t data_size)
{
	struct Node *current = *head_ref;
	struct Node *last = NULL;

	printf("delete Node is at %p\n", del_data);
	while(current != NULL){
		// compare memory
		if (memcmp(current->data, del_data, data_size) == 0){
			printf("found Node entry at %p\n", current->data);
			current->freeFn(current->data);

			if(last == NULL)
				*head_ref = current->next;
			else
				last->next = current->next;

			free(current);
			return 1;
		}

		last = current;
		current= current->next;
        }

	return 0;
}
