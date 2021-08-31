#include <stdlib.h>
#include "Stack.h"

#define INIT_SIZE 16

struct StackRep {
    int index;
    int capacity;
    Item *arr;
};

Stack createStack(void) {
    Stack s = malloc(sizeof(struct StackRep));
    s->index = 0;
    s->capacity = INIT_SIZE;
    s->arr = malloc(sizeof(Item) * s->capacity);
    return s;
}

void freeStack(Stack s) {
    free(s->arr);
    free(s);
}

void pushStack(Stack s, Item data) {
    if (s->index >= s->capacity) {
        s->capacity *= 2;
        s->arr = realloc(s->arr, s->capacity);
    }
    s->arr[s->index] = data;
    s->index += 1;
}

Item popStack(Stack s) {
    s->index -= 1;
    return s->arr[s->index];
}

bool isEmptyStack(Stack s) {
    return (s->index == 0);
}
