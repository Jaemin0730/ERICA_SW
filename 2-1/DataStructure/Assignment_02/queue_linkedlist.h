#include <stdio.h>
#include <stdbool.h>

#define MAX_QUEUE_SIZE_LL 10    

typedef struct Node
{
    int data;
    struct Node* next;
}Node;

typedef struct LinkedList_Queue {
    Node* front;
    Node* rear;
}LinkedList_Queue;

void queue_init_lq(LinkedList_Queue* queue);

bool is_empty_lq(LinkedList_Queue* queue);
bool is_full_lq(LinkedList_Queue* queue);

int enqueue_lq(LinkedList_Queue* queue, int data);
int dequeue_lq(LinkedList_Queue* queue);
