#include <stdio.h>
#include <stdbool.h>

#define MAX_ADDRESS_SIZE 15
#define NAME_ARRAY_SIZE 20

typedef struct Node
{
	char name[NAME_ARRAY_SIZE];
	int id;
	struct Node* next;
	struct Node* prev;
}Node;

typedef struct DoublyLinkedList {
	Node* head;
	Node* tail;
}DoublyLinkedList;

void list_init(DoublyLinkedList* list);
void print_list(DoublyLinkedList* list);

bool is_empty_list(DoublyLinkedList* list);
bool is_full_list(DoublyLinkedList* list);

int insert_head(DoublyLinkedList* list, char* name, int id);
int insert_tail(DoublyLinkedList* list, char* name, int id);

int delete_head(DoublyLinkedList* list);
int delete_tail(DoublyLinkedList* list);

int search_name(DoublyLinkedList* list, int id);
int search_id(DoublyLinkedList* list, char* name);
