#include <stdio.h>
#include <stdbool.h>

#define MAX_DEQUE_SIZE 10    

typedef struct Node_Deque
{
    int data;
    struct Node_Deque* next;
}Node_Deque;

typedef struct LinkedList_Deque {
    Node_Deque* front;
    Node_Deque* rear;
}LinkedList_Deque;

void deque_init(LinkedList_Deque* deque);

bool is_empty_deque(LinkedList_Deque* deque);
bool is_full_deque(LinkedList_Deque* deque);

int addfront_deque(LinkedList_Deque* deque, int data);
int addrear_deque(LinkedList_Deque* deque, int data);

int delfront_deque(LinkedList_Deque* deque);
int delrear_deque(LinkedList_Deque* deque);
