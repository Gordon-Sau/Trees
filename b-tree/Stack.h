#ifndef STACK_H
#define STACK_H
#include <stdbool.h>
typedef struct Item {
    int index;
    struct Node *parent;
} Item;
typedef struct StackRep* Stack;
Stack createStack(void);
void freeStack(Stack s);
void pushStack(Stack s, Item data);
Item popStack(Stack s);
bool isEmptyStack(Stack s);
#endif