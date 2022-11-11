#include <stdio.h>
#include <stdbool.h>

#define MAX_ARRAY_SIZE 50

typedef struct Array {
	int array[MAX_ARRAY_SIZE];
	int len;
}Array;

void init_array(Array* array);
bool is_empty(Array* array);
bool is_full(Array* array);

void print_array(Array* array);

int insert_array(Array* array, int data);

void sequence_sort(Array* array);
void selection_sort(Array* array);
void insertion_sort(Array* array);
void bubble_sort(Array* array);
void quick_sort(Array* array);