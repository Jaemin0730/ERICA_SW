#include "deque.h"
#include <stdlib.h>


// --------------------------------------------------------------------
// 변경 금지
// --------------------------------------------------------------------

// 초기화
void deque_init(LinkedList_Deque* deque)
{
    deque->front = NULL;
    deque->rear = NULL;
}

// --------------------------------------------------------------------


/*
* 구현시 유의사항
* 1. 함수명과 매개변수는 수정하시지 마세요.
* 2. 반환값 정확하게 구현해주세요. (bool, int)
* 3. 발생할 수 있는 모든 예외사항을 처리해주세요. (underflow/overflow)
*/

// --------------------------------------------------------------------
// Deque 구현
// --------------------------------------------------------------------

// true/false 반환
bool is_empty_deque(LinkedList_Deque* deque)
{
    if (deque->front == NULL && deque->rear == NULL) {
        return true;
    }
    else {
        return false;
    }
}

// true/false 반환
bool is_full_deque(LinkedList_Deque* deque)
{
    int count = 0;
    Node_Deque *p;
    for (p = deque->front; p != NULL; p = p->next) {
        count++;
    }
    if (count == MAX_DEQUE_SIZE) {
        return true;
    }
    else {
        return false;
    }
}


// Input 성공 : 0 반환 / 실패 : -1 반환
int addfront_deque(LinkedList_Deque* deque, int data)
{
    Node_Deque *new = (Node_Deque *)malloc(sizeof(Node_Deque));
    new->data = data;
    if (is_empty_deque(deque)) {
        deque->front = new;
        deque->rear = new;
        new->next = NULL;
        return 0;
    }
    else {
        if (is_full_deque(deque)) {
            return -1;
        }
        new->next = deque->front;
        deque->front = new;
        return 0;
    }
}

int addrear_deque(LinkedList_Deque* deque, int data)
{
    Node_Deque *new = (Node_Deque *)malloc(sizeof(Node_Deque));
    new->data = data;
    if (is_empty_deque(deque)) {
        deque->front = new;
        deque->rear = new;
        new->next = NULL;
        return 0;
    }
    else {
        if (is_full_deque(deque)) {
            return -1;
        }
        deque->rear->next = new;
        new->next = NULL;
        deque->rear = new;
        return 0;
    }
}

// Output 성공 : Output 값 반환 / 실패 : -1 반환
int delfront_deque(LinkedList_Deque* deque)
{
    Node_Deque *save = deque->front;
    int sol = deque->front->data;
    if (is_empty_deque(deque)) {
        return -1;
    }
    deque->front = deque->front->next;
    free(save);
    return sol;
}

int delrear_deque(LinkedList_Deque* deque)
{
    Node_Deque *save = deque->rear;
    Node_Deque *p;
    int sol = deque->rear->data;
    if (is_empty_deque(deque)) {
        return -1;
    }
    for (p = deque->front; p != NULL; p = p->next) {
        if (p->next == deque->rear) {
            p->next = NULL;
            deque->rear = p;
        }
    }
    free(save);
    return sol;
}