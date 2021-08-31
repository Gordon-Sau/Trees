#include "Queue.h"
#include <stdlib.h>
#include <string.h>
#define INIT_SIZE 16

struct QueueRep {
    int capacity;
    int size;
    int start;
    Data *arr;
};

Queue createQueue(void) {
    Queue q = malloc(sizeof(struct QueueRep));
    q->start = 0;
    q->size = 0;
    q->capacity = INIT_SIZE;
    q->arr = malloc(sizeof(Data) * q->capacity);
    return q;
}

void freeQueue(Queue q) {
    free(q->arr);
    free(q);
}

void pushQueue(Queue q, Data data) {
    if (q->size == q->capacity) {
        q->capacity = q->capacity * 2;
        q->arr = realloc(q->arr, sizeof(Data) * q->capacity);
    }
    if (q->start > 0) {
        memcpy(&(q->arr[q->size]), q->arr, q->start * sizeof(Data));
    }
    int nextIndex = q->start + q->size;
    if (nextIndex >= q->capacity) {
        nextIndex -= q->capacity;
    }
    q->arr[nextIndex] = data;
    q->size++;
}

Data popQueue(Queue q) {
    Data data = q->arr[q->start];
    q->start += 1;
    if (q->start >= q->capacity) {
        q->start -= q->capacity;
    }
    q->size--;
    return data;
}

bool isEmptyQueue(Queue q) {
    return q->size == 0;
}
