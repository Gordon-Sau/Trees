#ifndef BTREE_H
#define BTREE_H
#include <stdbool.h>
typedef struct BTreeRep *BTree;
BTree createBTree(void);
void freeBTree(BTree t);
bool searchBTree(BTree t, int data);
int sortBTree(BTree t, int *arr);
void insertBTree(BTree t, int data);
bool deleteBTree(BTree t, int data);
void showBTree(BTree t);
bool isBTree(BTree t);
#endif