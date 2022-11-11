#include <stdio.h>
#include "queue_array.h"
#include "queue_linkedlist.h"
#include "deque.h"

// main.c 소스는 수정하지 않고 결과 확인용으로만 사용합니다.
// 평가시에는 입력 값이나 횟수가 변경됩니다.

// 자유롭게 함수 테스트 하는 공간입니다.
void test_free(void)
{

}

void test_array_queue(void)
{
	printf("1. TEST START ARRAY QUEUE -------------------------------------\n");
	Array_Queue test_queue;
	bool bool_result;
	int int_result;

	queue_init_aq(&test_queue);
	bool_result = is_empty_aq(&test_queue);
	if (bool_result == true)
		printf("OOO - Success :\tis_empty()\n");
	else
		printf("XXX - is_empty() Error\n");

	for (int i = 10; i < 30; i++)
	{
		int_result = enqueue_aq(&test_queue, i);
		if (int_result == -1)
		{
			printf("OOO - Success :\tenqueue exception\n");
			bool_result = is_full_aq(&test_queue);
			if (bool_result == true)
				printf("OOO - Success :\tis_full()\n");
			else
				printf("XXX - Error : \tis_full()\n");

			break;
		}
		else
		{
			printf("OOO - Success :\tenqueue() %d\n", i);
		}
	}

	for (int i = 0; i < 11; i++)
	{
		int_result = dequeue_aq(&test_queue);
		if(int_result == -1)
		{
			printf("OOO - Success :\tdequeue exception\n");
			break;
		}
		else
			printf("OOO - Success :\tdequeue data : %d\n", int_result);

	}

	printf("\nTEST END ---------------------------------------------------------\n\n");
}

void test_linkedlist_queue(void)
{
	printf("2. TEST START LinkedList Queue --------------------------------\n");
	LinkedList_Queue test_queue;
	bool bool_result;
	int int_result;

	queue_init_lq(&test_queue);
	bool_result = is_empty_lq(&test_queue);
	if (bool_result == true)
		printf("OOO - Success :\tis_empty()\n");
	else
		printf("XXX - is_empty() Error\n");

	for (int i = 10; i < 30; i++)
	{
		int_result = enqueue_lq(&test_queue, i);
		if (int_result == -1)
		{
			printf("OOO - Success :\tenqueue exception\n");
			bool_result = is_full_lq(&test_queue);
			if (bool_result == true)
				printf("OOO - Success :\tis_full()\n");
			else
				printf("XXX - Error : \tis_full()\n");

			break;
		}
		else
		{
			printf("OOO - Success :\tenqueue() %d\n", i);
		}
	}

	for (int i = 0; i < 11; i++)
	{
		int_result = dequeue_lq(&test_queue);
		if (int_result == -1)
		{
			printf("OOO - Success :\tdequeue exception\n");
			break;
		}
		else
			printf("OOO - Success :\tdequeue data : %d\n", int_result);

	}

	printf("\nTEST END -----------------------------------------------------\n\n");
}

void test_deque()
{
	printf("3. TEST START LinkedList Deque --------------------------------\n");
	LinkedList_Deque test_deque;
	bool bool_result;
	int int_result;

	deque_init(&test_deque);

	bool_result = is_empty_deque(&test_deque);
	if (bool_result == true)
		printf("OOO - Success :\tis_empty()\n");
	else
		printf("XXX - is_empty() Error\n");

	if(addfront_deque(&test_deque, 6) == -1)
		printf("OOO : isfull deque\n");
	if (addfront_deque(&test_deque, 5) == -1)
		printf("OOO : isfull deque\n");
	if (addfront_deque(&test_deque, 4) == -1)
		printf("OOO : isfull deque\n");
	if (addfront_deque(&test_deque, 3) == -1)
		printf("OOO : isfull deque\n");
	if (addfront_deque(&test_deque, 2) == -1)
		printf("OOO : isfull deque\n");
	if (addfront_deque(&test_deque, 1) == -1)
		printf("OOO : isfull deque\n");

	if (addrear_deque(&test_deque, 91) == -1)
		printf("OOO : isfull deque\n");
	if (addrear_deque(&test_deque, 92) == -1)
		printf("OOO : isfull deque\n");
	if (addrear_deque(&test_deque, 93) == -1)
		printf("OOO : isfull deque\n");
	if (addrear_deque(&test_deque, 94) == -1)
		printf("OOO : isfull deque\n");
	if (addrear_deque(&test_deque, 95) == -1)
		printf("OOO : isfull deque\n");
	if (addrear_deque(&test_deque, 96) == -1)
		printf("OOO : isfull deque\n");

	printf("delfront : %d, front : %d\n", delfront_deque(&test_deque), test_deque.front->data);
	Node_Deque* tmp = test_deque.front;
	while (tmp != NULL)
	{
		printf("%d -> ", tmp->data);
		tmp = tmp->next;
	}
	printf("\n");

	printf("delfront : %d, front : %d\n", delfront_deque(&test_deque), test_deque.front->data);
	tmp = test_deque.front;
	while (tmp != NULL)
	{
		printf("%d -> ", tmp->data);
		tmp = tmp->next;
	}
	printf("\n");

	printf("delrear : %d, rear : %d\n", delrear_deque(&test_deque), test_deque.rear->data);
		
	printf("delrear : %d, rear : %d\n", delrear_deque(&test_deque), test_deque.rear->data);

	printf("\nTEST END -----------------------------------------------------\n\n");
}

int main(void)
{
	test_free();
	test_array_queue();
	test_linkedlist_queue();
	test_deque();
	return 0;
}