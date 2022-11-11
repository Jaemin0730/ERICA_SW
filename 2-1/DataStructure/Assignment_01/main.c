#include <stdio.h>
#include "stack_array.h"
#include "stack_linkedlist.h"

// main.c 소스는 수정하지 않고 결과 확인용으로만 사용합니다.
// 평가시에는 스택에 push/pop의 값이나 횟수가 변경됩니다.

void test_array_stack(void)
{
    printf("1. TEST START ARRAY STACK -------------------------------------\n");
    Array_Stack test_stack;

    stack_init_as(&test_stack);

    bool bool_result = is_empty_as(&test_stack);
    if (bool_result == true)
        printf("OOO - Success :\tis_empty()\n");
    else
        printf("XXX - is_empty() Error\n");

    push_as(&test_stack, 3);
    push_as(&test_stack, 5);
    push_as(&test_stack, 10);
    push_as(&test_stack, 9);
    push_as(&test_stack, 7);

    bool_result = is_full_as(&test_stack);

    if (bool_result == true)
        printf("OOO - Success :\tis_full()\n");
    else
        printf("XXX - Error : \tis_full()\n");

    int pop_element = pop_as(&test_stack);
    pop_element = pop_as(&test_stack);

    if (pop_element == 9)
        printf("OOO - Success :\tpop()\n");
    else
        printf("XXX - Error : \tpop()\n");

    int stack_size = get_stack_size_as(&test_stack);

    if (stack_size == 3)
        printf("OOO - Success :\tget_stack_size()\n");
    else
        printf("XXX - Error : \tget_stack_size()\n");


    int top_element = get_top_as(&test_stack);

    if (top_element == 10)
        printf("OOO - Success :\tget_top()\n");
    else
        printf("XXX - Error : \tget_top()\n");

    print_stack_as(&test_stack);
    printf("\nTEST END ARRAY STACK ------------------------------------------\n\n");
}

void test_linkedlist_stack()
{
    printf("2. TEST START LINKEDLIST STACK --------------------------------\n");
    LinkedList_Stack test_stack;

    stack_init_ls(&test_stack);

    bool bool_result = is_empty_ls(&test_stack);
    if (bool_result == true)
        printf("OOO - Success :\tis_empty()\n");
    else
        printf("XXX - is_empty() Error\n");

    push_ls(&test_stack, 3);
    push_ls(&test_stack, 5);
    push_ls(&test_stack, 10);
    push_ls(&test_stack, 9);
    push_ls(&test_stack, 7);

    bool_result = is_full_ls(&test_stack);

    if (bool_result == true)
        printf("OOO - Success :\tis_full()\n");
    else
        printf("XXX - Error : \tis_full()\n");

    int pop_element = pop_ls(&test_stack);
    pop_element = pop_ls(&test_stack);

    if (pop_element == 9)
        printf("OOO - Success :\tpop()\n");
    else
        printf("XXX - Error : \tpop()\n");

    int stack_size = get_stack_size_ls(&test_stack);

    if (stack_size == 3)
        printf("OOO - Success :\tget_stack_size()\n");
    else
        printf("XXX - Error : \tget_stack_size()\n");


    int top_element = get_top_ls(&test_stack);

    if (top_element == 10)
        printf("OOO - Success :\tget_top()\n");
    else
        printf("XXX - Error : \tget_top()\n");

    print_stack_ls(&test_stack);

    printf("\nTEST END LINKEDLIST STACK -------------------------------------\n\n");
}


int main(void)
{
    // 결과 확인용 (평가 시 값 변경이 있음)
    test_array_stack();
    test_linkedlist_stack();
}