#include <stdio.h>
#include <stdbool.h>

// rear 가 하나의 공간을 차지하고 있기 때문에, 
// 큐에 저장할 수 있는 자료의 갯수는 MAX_QUEUE_SIZE-1
#define MAX_QUEUE_SIZE_ARR 11    

typedef struct Array_Queue {
    int array[MAX_QUEUE_SIZE_ARR];
    int front;
    int rear;
}Array_Queue;

void queue_init_aq(Array_Queue* queue);

bool is_empty_aq(Array_Queue* queue);
bool is_full_aq(Array_Queue* queue);

int enqueue_aq(Array_Queue* queue, int data);
int dequeue_aq(Array_Queue* queue);
