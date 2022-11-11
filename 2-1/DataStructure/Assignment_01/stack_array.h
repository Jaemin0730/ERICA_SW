#include <stdio.h>
#include <stdbool.h>

#define MAX_STACK_SIZE 5

typedef struct Array_Stack {
    int array[MAX_STACK_SIZE];
    int top;
}Array_Stack;

void stack_init_as(Array_Stack* stack);

bool is_empty_as(Array_Stack* stack);
bool is_full_as(Array_Stack* stack);
void push_as(Array_Stack* stack, int data);
int pop_as(Array_Stack* stack);

int get_stack_size_as(Array_Stack* stack);
int get_top_as(Array_Stack* stack);
void print_stack_as(Array_Stack* stack);