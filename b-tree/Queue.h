#ifndef QUEUE_H
#define QUEUE_H
#include <stdbool.h>
typedef struct Node *Data;
typedef struct QueueRep *Queue;
Queue createQueue(void);
void freeQueue(Queue q);
void pushQueue(Queue q, Data data);
Data popQueue(Queue q);
bool isEmptyQueue(Queue q);
#endif