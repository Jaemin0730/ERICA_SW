#include "queue_linkedlist.h"
#include <stdlib.h>


// --------------------------------------------------------------------
// 변경 금지
// --------------------------------------------------------------------

// 초기화
void queue_init_lq(LinkedList_Queue* queue)
{
    queue->front = NULL;
    queue->rear = NULL;
}

// --------------------------------------------------------------------


/*
* 구현시 유의사항
* 1. 함수명과 매개변수는 수정하시지 마세요.
* 2. 반환값 정확하게 구현해주세요. (bool, int)
* 3. 발생할 수 있는 모든 예외사항을 처리해주세요.  (underflow/overflow)
*/

// --------------------------------------------------------------------
// Queue 구현
// --------------------------------------------------------------------

// true/false 반환
bool is_empty_lq(LinkedList_Queue* queue)
{
    if (queue->front == NULL) {
        return true;
    }
    else {
        return false;
    }
}

// true/false 반환
bool is_full_lq(LinkedList_Queue* queue)
{
    int count = 0;
    Node *p;
    for (p = queue->front; p != NULL; p = p->next) {
        count++;
    }
    if (count == MAX_QUEUE_SIZE_LL) {
        return true;
    }
    else {
        return false;
    }
}

// Enqueue 성공 : 0 반환 / 실패 : -1 반환
int enqueue_lq(LinkedList_Queue* queue, int data)
{
    // full인 경우에는 실패인 -1을 반환해줘야됨!
    Node *new = (Node *)malloc(sizeof(Node));
    new->data = data;
    new->next = NULL;
    if (is_empty_lq(queue)) {
        queue->front = queue->rear = new;
        return 0;
    }
    else {
        if (is_full_lq(queue)) {
            return -1;
        }
        queue->rear->next = new;
        queue->rear = queue->rear->next;
        return 0;
    }
}

// Dequeue 성공 : Dequeue 값 반환 / 실패 : -1 반환
int dequeue_lq(LinkedList_Queue* queue)
{
    if (is_empty_lq(queue)) {
        return -1;
    }
    else {
        Node *del = queue->front;
        int p = del->data;
        queue->front = queue->front->next;
        free(del);
        return p;
    }
}
