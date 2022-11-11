#include "array_sort.h"
#include <stdlib.h>
#include <stdbool.h>

void init_array(Array* array)
{
	array->len = 0;
}

bool is_empty(Array* array)
{
	if (array->len == 0)
		return true;
	return false;
}

bool is_full(Array* array)
{
	if (array->len == MAX_ARRAY_SIZE)
		return true;
	return false;
}

void print_array(Array* array)
{
	if (!is_empty(array))
	{
		for (int i = 0; i < array->len; i++)
			printf("%d -> ", array->array[i]);

		printf("\n");
	}
}

/*
* return 0	: success
* return -1	: fail
*/
int insert_array(Array* array, int data)
{
	if (!is_full(array))
	{
		array->array[array->len] = data;
		array->len++;
		return 0;
	}
	return -1;
}


//-----------------------------------------------------------
// If you need a another function, write it here.

// 퀵소트 관련 함수 사용 필요 ㅠㅠ
void quicksort (Array *array, int start, int end) {
	if (start >= end) {
		// 원소가 1개인 Case!
		return;
	}
	int piv = start, i = piv + 1, j = end;
	// i는 왼쪽 시작, j는 오른쪽
	while (i <= j) {
		// i와 j가 엇갈릴 때까지 반복
		while (i <= end && array->array[i] <= array->array[piv]) {
			i++;
		}
		while (j > start && array->array[j] >= array->array[piv]) {
			j--;
		}
		if (i > j) {
			int temp = array->array[j];
			array->array[j] = array->array[piv];
			array->array[piv] = temp;
		}
		else {
			int temp = array->array[j];
			array->array[j] = array->array[i];
			array->array[i] = temp;
		}
	}
	quicksort(array, start, j - 1);
	quicksort(array, j + 1, end);
}



//-----------------------------------------------------------


/*
* -----------------------------------------------------------------------------------
* Sorting Algorithm
* -----------------------------------------------------------------------------------
*/

void sequence_sort(Array* array)
{
	// 우리가 일상적으로 생각하는 정렬!
	for (int i = 0; i < array->len; i++) {
		for (int j = i + 1; j < array->len; j++) {
			if (array->array[i] > array->array[j]) {
				int p = array->array[i];
				array->array[i] = array->array[j];
				array->array[j] = p;
			}
		}
	}
}

void selection_sort(Array* array)
{
	// 남은 원소들 중에서 최솟값을 찾아서 앞으로 보내면서 정렬
	int min;
	for (int i = 0; i < array->len; i++) {
		min = i;
		for (int j = i + 1; j < array->len; j++) {
			if (array->array[j] < array->array[min]) {
				min = j;
			}
		}
		int temp = array->array[min];
		array->array[min] = array->array[i];
		array->array[i] = temp;
	}
}

void insertion_sort(Array* array)
{
	// 하나의 key값을 잡아서 그 값이 어디에 들어갈지 찾기
	// key 와 array[j], array[j-1]을 비교하면서, 큰 값을 뒤로 보내준다.
	int j;
	for (int i = 0; i < array->len; i++) {
		int key = array->array[i];
		for (j = i; j > 0 && array->array[j - 1] > key; j--) {
			//printf("%d %d ?? \n", array->array[j], array->array[j - 1]);
			array->array[j] = array->array[j - 1];
		}
		array->array[j] = key;
		/*
			for (int i = 0; i < array->len; i++) {
				printf("%d -> ", array->array[i]);
			}
			printf("--------------------------\n");
		*/
	}
}

void bubble_sort(Array* array)
{
	// 인접한 숫자들을 비교하면서 위치를 바꿔나가는 정렬
	for (int i = 0; i < (array->len) - 1; i++) {
		for (int j = 0; j < (array->len) - 1 - i; j++) {
			if (array->array[j] > array->array[j + 1]) {
				int temp = array->array[j];
				array->array[j] = array->array[j + 1];
				array->array[j + 1] = temp;
			}
		}
	}
}

void quick_sort(Array* array)
{
	quicksort(array, 0, ((array->len) - 1));
}