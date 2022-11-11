#include "dll_address.h"
#include <string.h>
#include <stdlib.h>

// --------------------------------------------------------------------
// 변경 금지
// --------------------------------------------------------------------

// 초기화	--- 변경금지
void list_init(DoublyLinkedList* list)
{
	list->head = NULL;
	list->tail = NULL;
}

// 주소록 출력	--- 변경금지
void print_list(DoublyLinkedList* list)
{
	if (is_empty_list(list))
		printf("XXXXXXXXX 빈 주소록 XXXXXXXXXX\n");
	else
	{
		printf("--------------------------\n");
		printf("---- 주소록 출력 시작 ----\n");
		printf("--------------------------\n");
		printf("  학번 \t\t 이름\n");
		printf("--------------------------\n");
		Node* tmp = list->head;
		while (tmp != NULL)
		{
			printf("%d\t%s\n", tmp->id, tmp->name);
			tmp = tmp->next;
		}

		printf("\n\n");
	}
}
// --------------------------------------------------------------------

/*
* is_empty
* return true/false
*/
bool is_empty_list(DoublyLinkedList* list)
{
	if (list->head == NULL && list->tail == NULL) {
		return true;
	}
	else {
		return false;
	}
}

/*
* is_full
* return true/false
*/
bool is_full_list(DoublyLinkedList* list)
{
	int count = 0;
	Node *p;
	for (p = list->head; p != NULL; p = p->next) {
		count++;
	}
	if (count == MAX_ADDRESS_SIZE) {
		return true;
	}
	else {
		return false;
	}
}

/*
* insert_head
* 리스트 앞에 삽입
* return 성공 0 / 실패 -1
*/
int insert_head(DoublyLinkedList* list, char* name, int id)
{
	if (is_full_list(list)) {
		return -1;
	}
	Node *new = (Node *)malloc(sizeof(Node));
	new->prev = NULL;
	new->next = NULL;
	new->id = id;
	strcpy_s(new->name, NAME_ARRAY_SIZE, name);
	if (list->head == NULL) {
		list->head = new;
		list->tail = new;
	}
	else {
		new->next = list->head;
		list->head->prev = new;
		list->head = new;
	}
	return 0;
}

/*
* insert_head
* 리스트 뒤에 삽입
* return 성공 0 / 실패 -1
*/
int insert_tail(DoublyLinkedList* list, char* name, int id)
{
	if (is_full_list(list)) {
		return -1;
	}
	Node *new = (Node *)malloc(sizeof(Node));
	new->prev = NULL;
	new->next = NULL;
	new->id = id;
	strcpy_s(new->name, NAME_ARRAY_SIZE, name);
	if (list->head == NULL) {
		list->head = new;
		list->tail = new;
	}
	else {
		list->tail->next = new;
		new->prev = list->tail;
		list->tail = new;
	}
	return 0;
}

/*
* insert_head
* 리스트 앞에 삽입
* return 성공 0 / 실패 -1
*/
int delete_head(DoublyLinkedList* list) 
{
	if (is_empty_list(list)) {
		return -1;
	}
	if (list->head == list->tail) {
		list->head = NULL;
		list->tail = NULL;
	}
	else {
		list->head = list->head->next;
		list->head->prev->next = NULL;
		list->head->prev = NULL;
	}
	return 0;
}

/*
* insert_head
* 리스트 앞에 삽입
* return 성공 0 / 실패 -1
*/
int delete_tail(DoublyLinkedList* list)
{
	if (is_empty_list(list)) {
		return -1;
	}
	if (list->head == list->tail) {
		list->head = NULL;
		list->tail = NULL;
	}
	else {
		list->tail = list->tail->prev;
		list->tail->next->prev = NULL;
		list->tail->next = NULL;
	}
	return 0;
}

// ------------- 심화 ---------------
/*
* 학번으로 검색
* printf 출력 "이름"
* return 성공 0 / 실패 -1
*/
int search_name(DoublyLinkedList* list, int id)
{
	Node *p;
	for (p = list->head; p != NULL; p = p->next) {
		if (p->id == id) {
			printf("ID : %d -> Name : %s\n", p->id, p->name);
			return 0;
		}
	}
	return -1;
}

/*
* 이름으로 검색 
* printf 출력 "학번" (해당이름 모든 학번 출력)
* return 성공 0 / 실패 -1
*/
int search_id(DoublyLinkedList* list, char* name)
{
	int sol = 0; // 정답 경우 확인 여부
	int cnt = 0; // 문자가 다른 경우의 확인 여부
	Node* p;
	for (p = list->head; p != NULL; p = p->next) {
		for (int i = 0; name[i] != '\0'; i++) {
			if (p->name[i] != name[i]) {
				cnt++;
			}
			// 우리가 찾고자 하는 name과 *p에 있는 name이 같은지 비교해본다.
			// 비교하기 위해 문자 하나하나를 비교해주면서, 다르면 cnt(문자)를 더해준다.
		}
		if (cnt == 0) {
			printf("Name : %s -> ID : %d\n", p->name, p->id);
			sol++;
			// cnt가 0이어야지 같은거니까 그 경우를 출력해준다.
			// 그리고 출력한 케이스에 sol을 더해주어서 정답경우를 확인한다.
		}
		cnt = 0;
		// cnt를 초기화해준다.
	}
	if (sol == 0) {
		return -1;
	}
	else {
		// sol이 0이 아니어야지 정답!
		return 0;
	}
}