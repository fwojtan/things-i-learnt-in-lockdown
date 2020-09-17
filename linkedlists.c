/*
* The aim here is to learn how to impliment linked lists in C.
* I'm following the tutorial from learn-c.org
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int value;
	struct node *next; //apparently recursive structure definitions are fine
} node_t;

void print_list(node_t * head){
	node_t * current = head;
	while (current != NULL) {
		printf("%d\n", current->value);
		current = current->next;
	}
}

void push_end(node_t * head, int val) {
	node_t * current = head;		// creates current pointer to type node_t and sets it to head
	while (current->next != NULL) { // iterates through entire list and sets current to the final next pointer
		current = current -> next;
	}
	current->next = (node_t *) malloc(sizeof(node_t)); // allocates memory for a new node at current
	current->next->value = val; 		// stores the value given in the new node
	current->next->next = NULL;		// makes the node the end of the list
}

void push_start(node_t ** head, int val) {
	node_t * new_node;				// creates a new pointer for type node_t
	new_node = (node_t *) malloc(sizeof(node_t));

	new_node->value = val;			// set value of new node
	new_node->next = *head;			// set the next value of new node to point to the old head node
	*head = new_node; 				// the head variable is set to be the new node
}

int pop_first(node_t ** head) { // node_t ** head is a pointer to a pointer to a node_t variable
	int retval = -1; // create a value to be returned so we can return the data from the node
	node_t * next_node = NULL;

	if (*head == NULL) {
		return -1;
	}

	next_node = (*head)->next;	// reads out the next pointer from the node
	retval = (*head)->value;		// (*_____)-> notation means deref then access value from a pointer to a struct
	free(*head);
	*head = next_node; // replace old head with new head

	return retval;
}

int pop_last(node_t * head) { // only a normal pointer this time??
	int retval = 0;				// I suppose we don't need to replace the head pointer
	if (head -> next == NULL) { // if only one item, remove it
		retval = head->value;
		free(head);
		return retval;
	}

	node_t * current = head;
	while (current->next->next != NULL) { // check two places ahead for end condition
		current = current->next;
	}

	// current should now point to the penultimate item, so we can remove current->next
	retval = current->next->value;
	free(current->next);
	current->next = NULL;
	return retval;
}

int pop_by_index(node_t ** head, int n) {
	int i = 0;
	int retval = -1;
	node_t * current = *head;
	node_t * temp_node = NULL;

	if (n==0) {
		return pop_first(head);
	}

	for (i=0; i<n-1; i++) {
		if (current->next == NULL){
			return -1;
		}
		current = current->next;
	}

	temp_node = current->next;
	retval = temp_node->value;
	current->next = temp_node->next;
	free(temp_node);

	return retval;


}

void init_list(node_t ** head, int val1, int val2){
	// in the line below, the * appears before a variable being declared. In this context
	// * acts as a type modifier and makes the variable declared a pointer
	// in other contexts it can act to dereference or multiply two numbers
	//node_t * head = NULL; //creates a pointer called head that doesn't point anywhere
	*head = (node_t *) malloc(sizeof(node_t)); //allocates memory and gets head to point to it
	if (*head == NULL) { // check malloc gives a pointer back
		printf("Error allocating pointer for list head");
	}
	(*head)->next = (node_t *) malloc(sizeof(node_t));
	(*head)->value = val1;
	(*head)->next->next = NULL;
	(*head)->next->value = val2;

}



int main() {

	node_t * head = NULL;
	init_list(&head, 0, 1);

	
	int i = 0;
	for (i=0; i<10; i++) {
		push_end(head, i+2);
	}

	print_list(head);
	

	return 0;
}