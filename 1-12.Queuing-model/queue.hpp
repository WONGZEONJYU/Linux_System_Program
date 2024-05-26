#ifndef QUEUE_H
#define QUEUE_H

#include "list.hpp"

typedef ListNode  QueueNode;
typedef void* Queue;

Queue Queue_Create();
void Queue_Destroy(Queue );
int Queue_IsEmpty(Queue );
int Queue_IsContained(Queue , QueueNode* );
void Queue_Add(Queue , QueueNode* );
QueueNode* Queue_Front(Queue );
QueueNode* Queue_Remove(Queue );
int Queue_Length(Queue );
void Queue_Rotate(Queue );

#endif
