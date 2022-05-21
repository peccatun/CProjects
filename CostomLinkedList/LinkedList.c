#include <stdio.h>
#include <stdlib.h>


typedef struct LinkedList LinkedList;
typedef struct Node Node;

struct Node {
	int value;
	Node* next;
};

void add(LinkedList** list, int value);
void removeValue(LinkedList** list, int value);
int contains(LinkedList** list, int value);

struct LinkedList {
	Node* head;
	void (*add)(LinkedList** list, int);
	void (*removeValue)(LinkedList** list, int);
	int (*contains)(LinkedList** list, int value);
};

int main() {
	LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
	list->head = NULL;
	list->add = &add;
	list->removeValue = &removeValue;
	list->contains = &contains;

	list->add(&list, 5);

	list->removeValue(&list, 5);

	list->add(&list, 10);

	if (list->contains(&list, 10)) {
		printf("Has 10");
	}


	if (list->contains(&list, 11)) {
		printf("Has 11");
	}
	list->add(&list, 15);


	if (list->contains(&list, 15)) {
		printf("Has 15");
	}

	list->removeValue(&list, 15);
	list->removeValue(&list, 10);
	list->removeValue(&list, 14);


	if (list->contains(&list, 10)) {
		printf("Has 10");
	}

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

void removeValue(LinkedList** list, int value) {
	Node* temp = list[0]->head;

	if (temp != NULL && temp->value == value) {
		list[0]->head = temp->next;
		free(temp);
		return;
	}

	Node* prev = NULL;

	while (temp != NULL && temp->value != value) {
		prev = temp;
		temp = temp->next;
	}

	if (temp == NULL) {
		return;
	}

	prev->next = temp->next;
	free(temp);
}

int contains(LinkedList** list, int value) {
	Node* temp = list[0]->head;

	while (temp != NULL && temp->value != value) {
		temp = temp->next;
	}

	if (temp == NULL) {
		return 0;
	}

	return temp->value == value;
}