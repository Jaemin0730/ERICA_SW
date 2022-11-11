#include <stdio.h>
#include <stdbool.h>

#define MAX_STACK_SIZE 5

typedef struct Node
{
    int data;
    struct Node* next;
}Node;

typedef struct LinkedList_Stack {
    Node* top;
}LinkedList_Stack;

void stack_init_ls(LinkedList_Stack* stack);

bool is_empty_ls(LinkedList_Stack* stack);
bool is_full_ls(LinkedList_Stack* stack);
void push_ls(LinkedList_Stack* stack, int data);
int pop_ls(LinkedList_Stack* stack);

int get_stack_size_ls(LinkedList_Stack* stack);
int get_top_ls(LinkedList_Stack* stack);
void print_stack_ls(LinkedList_Stack* stack);