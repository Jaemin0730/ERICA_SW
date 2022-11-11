#include "stack_array.h"

// --------------------------------------------------------------------
// ���� ����
// --------------------------------------------------------------------

// MAX_STACK_SIZE 5

// ���� �ʱ�ȭ
void stack_init_as(Array_Stack* stack)
{
    stack->top = 0;
}

// --------------------------------------------------------------------


/*
* ������ ���ǻ���
* 1. �Լ���� �Ű������� �����Ͻ��� ������.
* 2. ��ȯ�� ��Ȯ�ϰ� �������ּ���. (bool, int)
*/

// --------------------------------------------------------------------
// ���� ���� ���� - ��� ���� B
// --------------------------------------------------------------------

// ������ ����ִ��� Ȯ��, true/false ��ȯ
bool is_empty_as(Array_Stack* stack)
{
    if (stack->top == 0) {
        return true;
    }
    return false;
}

/*
* ������ ����á���� Ȯ��, true/false ��ȯ
* ������ �ִ� ������ ������ MAX_STACK_SIZE
*/
bool is_full_as(Array_Stack* stack)
{
    //printf("%d\n",stack->top);
    if ((stack->top) == MAX_STACK_SIZE) {
        return true;
    }
    else {
        return false; 
    }
}

// ���ÿ� ���� ����
void push_as(Array_Stack* stack, int data)
{
    stack->array[stack->top] = data;
    // printf("%d\n",stack->top);
    (stack->top)++;
    //printf("%d\n",stack->top);
}

// ������ �ֻ��� ����(top)�� �����ϰ� ��ȯ
int pop_as(Array_Stack* stack)
{
    int t = stack->array[stack->top - 1];
    (stack->top)--;
    return t;
}


// --------------------------------------------------------------------
// ��ȭ ���� ����
// --------------------------------------------------------------------

// ���� ���� ���� ���� ��ȯ
int get_stack_size_as(Array_Stack* stack)
{
    return stack->top;
}

// ������ �ֻ��� ����(top) �� ��ȯ
int get_top_as(Array_Stack* stack)
{
    return stack->array[stack->top - 1];
}

/*
* ������ �ֻ��� ���Һ��� ������� ��� (pop X)
* top ... bottom 
* ex) 5 10 12 4 3
*/
void print_stack_as(Array_Stack* stack)
{
    for (int i = stack->top-1; i >= 0 ; i--) {
        printf("%d ", stack->array[i]);
    }
}
