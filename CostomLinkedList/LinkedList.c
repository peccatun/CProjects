#include <stdio.h>
#include <stdlib.h>


typedef struct LinkedList LinkedList;
typedef struct Node Node;

struct Node {
	int value;
	Node* next;
};

void add(LinkedList** list, int value);

struct LinkedList {
	Node* head;
	void (*add)(LinkedList** list, int);
};

int main() {
	LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
	list->head = NULL;
	list->add = &add;

	list->add(&list, 5);
	list->add(&list, 10);
	list->add(&list, 15);
	return 0;
}

void add(LinkedList** list, int value) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->value = value;
	newNode->next = NULL;

	if (list[0]->head == NULL) {
		list[0]->head = newNode;
		return;
	}

	Node** current = &list[0]->head;

	while (current[0]->next != NULL) {
		current = &current[0]->next;
	}

	current[0]->next = newNode;
}