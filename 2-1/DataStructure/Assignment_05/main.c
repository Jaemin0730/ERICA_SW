#include <stdio.h>
#include "array_sort.h"

void test_sorting(void)
{
	Array array;
	init_array(&array);

	insert_array(&array, 1);
	insert_array(&array, 5);
	insert_array(&array, 3);
	insert_array(&array, 10);
	insert_array(&array, 9);
	insert_array(&array, 6);

	print_array(&array);

	sequence_sort(&array);
	//selection_sort(&array);
	//insertion_sort(&array);
	//bubble_sort(&array);
	//quick_sort(&array)

	print_array(&array);
}

int main(void)
{
	test_sorting();

	return 0;
}