#include "stack_linkedlist.h"
#include <stdlib.h>

// --------------------------------------------------------------------
// ���� ����
// --------------------------------------------------------------------

// ���� �ʱ�ȭ
void stack_init_ls(LinkedList_Stack* stack)
{
    stack->top = NULL;
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
bool is_empty_ls(LinkedList_Stack* stack)
{
    if (stack->top == NULL) {
        return true;
    }
    return false;
}

/*
* ������ ����á���� Ȯ��, true/false ��ȯ
* ������ �ִ� ������ ������ MAX_STACK_SIZE
*/
bool is_full_ls(LinkedList_Stack* stack)
{
    int count = 0;
    Node *list = stack->top;
    while (list != NULL)
    {
        list = list->next;
        count++;
    }
    if (count == MAX_STACK_SIZE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// ���ÿ� ���� ����
void push_ls(LinkedList_Stack* stack, int data)
{
    Node *list = (Node *)malloc(sizeof(Node));
    list->data = data;
    list->next = stack->top;
    stack->top = list;
}

// // ������ �ֻ��� ����(top)�� �����ϰ� ��ȯ
int pop_ls(LinkedList_Stack* stack)
{
    Node *list = stack->top;
    int p = list->data;
    stack->top = stack->top->next;
    free(list);
    return p;
}


// --------------------------------------------------------------------
// ��ȭ ���� ����
// --------------------------------------------------------------------

// ���� ���� ���� ���� ��ȯ
int get_stack_size_ls(LinkedList_Stack* stack)
{
    int count = 0;
    Node *list = stack->top;
    while (list != NULL)
    {
        list = list->next;
        count++;
    }
    return count;
}

// ������ �ֻ��� ����(top) �� ��ȯ
int get_top_ls(LinkedList_Stack* stack)
{
    Node *list = stack->top;
    int p = list->data;
    // printf("%d\n",p);
    return p;
}

/*
* ������ �ֻ��� ���Һ��� ������� ��� (pop X)
* top ... bottom
* ex) 5 10 12 4 3
*/
void print_stack_ls(LinkedList_Stack* stack)
{
    for (Node *i = stack->top; i != NULL; i = i->next) {
        printf("%d ", i->data);
    }
}
