#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "Stack.h"
// #include "Queue.h"
#include "2_3_4tree.h"
#include <assert.h>

typedef struct Node *Node;

struct Node {
    // alternatives: int start;
    // can use circular buffer with a start index for faster shifting
    int size;
    int arr[3];
    struct Node *children[4];
    // alternatives: if node is leaf, we can store prev and next
    // Notice that we don't really need extra memory, as we can identify the node is a leaf if its first child is NULL
};

struct BTreeRep {
    Node root;
    int nNodes;
    int height;
};

static void sortNode(Node root, int *arr, int *index);
static void freeNode(Node root);
// static bool searchNode(Node root, int data);
static void rebalance(Node node, BTree t, Stack s);

BTree createBTree(void) {
    BTree t = malloc(sizeof(struct BTreeRep));
    t->root = NULL;
    t->nNodes = 0;
    t->height = 0;
    return t;
}

struct PromoteData {
    int data;
    Node left, right;
};

static struct PromoteData promoteBTree(Node node, struct PromoteData promoteData, int index) {
    // 4 cases
    Node left = node, right = malloc(sizeof(struct Node));
    left->size = 1;
    right->size = 2;
    int promote;
    if (index == 1) {
        promote = promoteData.data;
        right->children[0] = promoteData.right;
        right->children[1] = node->children[2];
        right->children[2] = node->children[3];
        // left->children[0] = node->children[0];
        left->children[1] = promoteData.left;
    } else if (index == 0) {
        promote = node->arr[0];
        node->arr[0] = promoteData.data;
        right->children[0] = node->children[1];
        right->children[1] = node->children[2];
        right->children[2] = node->children[3];
        left->children[0] = promoteData.left;
        left->children[1] = promoteData.right;
    } else {
        promote = node->arr[1];
        if (index == 2) {
            node->arr[1] = promoteData.data;
            right->children[0] = promoteData.left;
            right->children[1] = promoteData.right;
            right->children[2] = node->children[3];
            // left->children[0] = node->children[0];
            // left->children[1] = node->children[1];
        } else {
            node->arr[1] = node->arr[2];
            node->arr[2] = promoteData.data;
            right->children[0] = node->children[2];
            right->children[1] = promoteData.left;
            right->children[2] = promoteData.right;
        }
    }
    
    right->arr[0] = node->arr[1];
    right->arr[1] = node->arr[2];

    return (struct PromoteData){
        .data = promote,
        .left = left,
        .right = right
    };
}

static Node insertNode(BTree t, int data) {
    Node root = t->root;
    int *nNodes = &(t->nNodes);

    Stack s = createStack();
    Node node = root;

    int index;
    while (true) {
        for (index = 0; index < node->size; index++) {
            if (node->arr[index] == data) {
                freeStack(s);
                return root;
            }
            if (node->arr[index] > data) break;
        }


        if (node->children[index] == NULL) break;
        else {
            pushStack(s, (struct Item) {
                .index = index,
                .parent = node
            });
            node = node->children[index];
        }
    }

    struct PromoteData promoteData = {
        .data = data,
        .left = NULL,
        .right = NULL
    };

    while (!isEmptyStack(s) && node->size == 3) {

        promoteData = promoteBTree(node, promoteData, index);
        *nNodes += 1;
        
        struct Item it = popStack(s);
        node = it.parent;
        index = it.index;
    }

    if (node->size == 3) {
        promoteData = promoteBTree(node, promoteData, index);
        node = malloc(sizeof(struct Node));
        node->size = 1;
        node->arr[0] = promoteData.data;
        node->children[0] = promoteData.left;
        node->children[1] = promoteData.right;
        *nNodes += 2;
        t->height += 1;
        root = node;
    } else {
        for (int i = node->size; i > index; i--) {
            node->arr[i] = node->arr[i - 1];
            node->children[i+1] = node->children[i];
        }
        node->arr[index] = promoteData.data;
        node->children[index] = promoteData.left;
        node->children[index + 1] = promoteData.right;
        node->size += 1;
    }
    freeStack(s);
    return root;
}

void insertBTree(BTree t, int data) {
    if (t->root == NULL) {
        Node newRoot = malloc(sizeof(struct Node));
        newRoot->size = 1;
        newRoot->arr[0] = data;
        newRoot->children[0] = NULL;
        newRoot->children[1] = NULL;
        t->root = newRoot;
        t->nNodes = 1;
        t->height = 1;
    } else {
        t->root = insertNode(t, data);
    }
}

void freeBTree(BTree t) {
    freeNode(t->root);
    free(t);
}

static void freeNode(Node root) {
    if (root == NULL) return;

    for (int i = 0; i < root->size; i++) freeNode(root->children[i]);

    free(root);
}

int sortBTree(BTree t, int *arr) {
    if (t->root == NULL) return 0;
    int index = 0;
    sortNode(t->root, arr, &index);
    return index;
}

static void sortNode(Node root, int *arr, int *index) {
    if (root == NULL) return;
    sortNode(root->children[0], arr, index);
    for (int i = 0; i < root->size && i < 4; i++) {
        arr[*index] = root->arr[i];
        *index += 1;
        sortNode(root->children[i + 1], arr, index);
    }
}

bool searchBTree(BTree t, int data) {
    Node node = t->root;
    while (node != NULL) {
        int i;
        for (i = 0; i < node->size && i < 3; i++) {
            if (node->arr[i] == data) return true;
            else if (node->arr[i] > data) break;
        }
        node = node->children[i];
    }
    return false;
}

// static bool searchNode_recur(Node root, int data) {
//     if (root == NULL) return false;
//     // search the index for the next children
//     int i;
//     for (i = 0; i < root->size; i++) {
//         if (root->arr[i] == data) return true;
//         else if (root->arr[i] < data) break;
//     }
//     return searchNode_recur(root->children[i], data);
// }

static void showArr(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("|%d", arr[i]);
    }
    printf("|\n");
}

static void showNode(Node node, int depth) {
    if (node == NULL) return;
    for (int i = 0; i < depth; i++){
        printf("   ");
    }
    showArr(node->arr, node->size);
    for (int i = 0; i <= node->size; i++) {
        showNode(node->children[i], depth + 1);
    }
}

void showBTree(BTree t) {
    printf("number of nodes: %d\n", t->nNodes);
    printf("height: %d\n", t->height);
    showNode(t->root, 0);
    printf("---------\n");
}

bool deleteBTree(BTree t, int data) {
    // find the first node that have the data
    Node node = t->root;
    Stack s = createStack();
    int i;
    while (node != NULL) {
        for (i = 0; i < node->size && i < 3; i++) {
            if (node->arr[i] == data) goto FOUND_NODE;
            else if (node->arr[i] > data) break;
        }
        pushStack(s, (Item){
            .index = i,
            .parent = node
        });
        node = node->children[i];
    }
    freeStack(s);
    return false;

FOUND_NODE:
    assert(node->arr[i] == data);
    // leaf case
    if (node->children[i] == NULL) {
        for (int j = i + 1; j < node->size; j++) {
            node->arr[j - 1] = node->arr[j];
        }
    } else {
    // internal node: substitute with the leftmost node in the right subtree
        Node delNode = node;
        int index = i;

        pushStack(s, (Item) {
            .index = i + 1,
            .parent = node
        });
        node = node->children[i + 1];

        // leftmost leaf node of the right subtree
        while (node->children[0] != NULL) {
            pushStack(s, (Item) {
                .index = 0,
                .parent = node
            });
            node = node->children[0];
        }
        // node is the leftmost leaf node

        delNode->arr[index] = node->arr[0];
        for (int j = 1; j < node->size; j++) {
            node->arr[j - 1] = node->arr[j];
        }
    }
    node->size -= 1;
    if (node->size == 0) {
        if (node == t->root) {
            free(node);
            t->root = NULL;
            t->nNodes = 0;
            t->height = 0;
        } else {
            rebalance(node, t, s);
        }
    }
    freeStack(s);
    return true;
}

static void rebalance(Node node, BTree t, Stack s) {
    bool cont = true;
    while (cont) {
        cont = false;
        Item parentIndex = popStack(s);
        Node parent = parentIndex.parent;
        int index = parentIndex.index;

        bool isRightMost = (index == parent->size);
        // left rotation
        if (!isRightMost &&
            parent->children[index + 1]->size > 1) {

            Node sibling = parent->children[index + 1];
            node->arr[0] = parent->arr[index];
            node->size = 1;
            node->children[1] = sibling->children[0];
            parent->arr[index] = sibling->arr[0];
            for (int j = 1; j < sibling->size; j++) {
                sibling->arr[j - 1] = sibling->arr[j];
                sibling->children[j - 1] = sibling->children[j];
            }
            sibling->children[sibling->size - 1] = sibling->children[sibling->size];
            sibling->size -= 1;
        // right rotation
        } else if (
            index > 0 &&
            parent->children[index - 1]->size > 1) {

            Node sibling = parent->children[index - 1];
            node->arr[0] = parent->arr[index - 1];
            node->children[1] = node->children[0];
            node->children[0] = sibling->children[sibling->size];
            node->size = 1;
            parent->arr[index - 1] = sibling->arr[sibling->size - 1];
            sibling->size -= 1;
        // merge
        } else {
            Node left, right;
            if (isRightMost) {
                left = parent->children[index - 1];
                right = node;
                left->arr[1] = parent->arr[index - 1];
                left->children[2] = right->children[0];
                left->size = 2;
                parent->size -= 1;
            } else {
                right = parent->children[index + 1];
                left = node;
                left->arr[0] = parent->arr[index];
                left->arr[1] = right->arr[0];
                left->children[1] = right->children[0];
                left->children[2] = right->children[1];
                left->size = 2;

                // shifting elements in the parent node
                for (int i = index + 1; i < parent->size; i++) {
                    parent->arr[i - 1] = parent->arr[i];
                    parent->children[i - 1] = parent->children[i];
                }
                parent->children[parent->size - 1] = parent->children[parent->size];
                parent->children[index] = left;
                parent->size -= 1;
                for (int i = 0; i < parent->size; i++) {
                    assert(parent->children[i]->arr[parent->children[i]->size - 1] <= parent->arr[i]);
                    assert(parent->children[i + 1]->arr[parent->children[i + 1]->size - 1] >= parent->arr[i]);
                }
            }

            free(right);
            t->nNodes -= 1;

            if (parent->size == 0) {
                if (parent == t->root) {
                    t->root = left;
                    t->height -= 1;
                    t->nNodes -= 1;
                    free(parent);
                } else {
                    node = parent;
                    cont = true;
                }
            }

        }
    }
}

static bool validNode(Node root) {
    if (root == NULL) return true;
    for (int i = 0; i <= root->size; i++) {
        assert(validNode(root->children[i]));
    }
    if (root->children[0] != NULL) {
        for (int i = 0; i <= root->size; i++) {
            assert(root->children[i] != NULL);
        }
        for (int i = 0; i < root->size; i++) {
            assert(root->children[i]->arr[root->children[i]->size - 1] <= root->arr[i]);
            assert(root->children[i + 1]->arr[root->children[i + 1]->size - 1] >= root->arr[i]);
        }
    } else {
        for (int i = 0; i <= root->size; i++) {
            assert(root->children[i] == NULL);
        }
    }
    for (int i = 1; i < root->size; i++) {
        assert(root->arr[i - 1] <= root->arr[i]);
    }
    for (int i = 0; i < root->size; i++) {
        assert(root->arr[i] >= 1 && root->arr[i] <= 100);
    }
    return true;
}

bool isBTree(BTree t) {
    return validNode(t->root);
}
