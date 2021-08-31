#ifndef AVLTREE_H
#define AVLTREE_H
#include <stdbool.h>
typedef int T;
typedef int(* compar_fn_t) (T, T);

typedef struct AVLNode *AVLTree;
struct range {
    struct AVLNode *start, *end;
};

struct Item {
    struct AVLNode *node;
    bool direction;
};

void AVL_init(AVLTree *t);
void AVL_destroy(AVLTree *t);

int AVL_print(AVLTree root);

struct AVLNode *AVL_search(AVLTree root, T element, compar_fn_t cmp);
struct AVLNode *AVL_rsearch(AVLTree root, T element, compar_fn_t cmp);
struct range AVL_range_search(AVLTree root, T element, compar_fn_t cmp);

void AVL_insert(AVLTree* tree, T element, compar_fn_t cmp);

bool AVL_delete(AVLTree *tree, T element, compar_fn_t cmp);

AVLTree AVL_merge(AVLTree *tree1, AVLTree *tree2, compar_fn_t cmp);

bool is_balanced(AVLTree tree);
bool is_ordered(AVLTree tree);

#endif