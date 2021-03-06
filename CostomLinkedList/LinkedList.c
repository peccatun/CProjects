#include <stdio.h>
#include <stdlib.h>


typedef struct LinkedList LinkedList;
typedef struct Node Node;

struct Node {
	int value;
	Node* next;
};

struct LinkedList {
	Node* head;
	void (*add)(LinkedList** list, int);
	void (*removeValue)(LinkedList** list, int);
	int (*contains)(LinkedList** list, int value);
	int (*count)(LinkedList** link);
	int* (*toArray)(LinkedList** list);
	int (*getFirst)(LinkedList** list);
	int (*getLast)(LinkedList** list);

};

void add(LinkedList** list, int value);
void removeValue(LinkedList** list, int value);
int contains(LinkedList** list, int value);
int count(LinkedList** list);
int* toArray(LinkedList** list);
int getFirst(LinkedList** list);
int getLast(LinkedList** list);

int main() {
	LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
	list->head = NULL;
	list->add = &add;
	list->removeValue = &removeValue;
	list->contains = &contains;
	list->count = &count;
	list->toArray = &toArray;
	list->getFirst = &getFirst;
	list->getLast = &getLast;


	list->add(&list, 10);
	list->add(&list, 24);
	list->add(&list, 45);
	list->add(&list, 12);
	list->add(&list, 4);
	list->add(&list, 2);
	list->removeValue(&list, 2);
	
	int first = list->getFirst(&list);
	int last = list->getLast(&list);


	printf("first: %d", first);
	printf(" last: %d", last);

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

	while (temp != NULL && temp->value != value)
		temp = temp->next;

	if (temp == NULL)
		return 0;

	return temp->value == value;
}

int count(LinkedList** list) {
	int count = 0;

	Node* current = list[0]->head;

	while (current != NULL) {
		current = current->next;
		count++;
	}

	return count;
}

int* toArray(LinkedList** list) {
	int length = count(&list[0]);
	int* arr = (int*)malloc(sizeof(int) * length);
	Node* current = list[0]->head;

	int index = 0;

	while (current != NULL) {
		arr[index] = current->value;
		index++;
		current = current->next;
	}

	return arr;
}

int getFirst(LinkedList** list) {
	Node* current = list[0]->head;

	if (current == NULL)
		return 0;

	return current->value;
}

int getLast(LinkedList** list) {
	Node* current = list[0]->head;

	if (current == NULL)
		return 0;

	while (current->next != NULL)
		current = current->next;
	
	return current->value;
}