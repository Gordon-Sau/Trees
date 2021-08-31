#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "AVLTree.h"

struct AVLNode {
    struct AVLNode *left, *right;
    int height;
    T data;
};

static int get_height(struct AVLNode *root);
static int max_height(struct AVLNode *left, struct AVLNode *right);
static struct AVLNode *rotate_left(struct AVLNode *root);
static struct AVLNode *rotate_right(struct AVLNode *root);
static void AVL_destroy_recur(AVLTree root);
static int AVL_print_recur(AVLTree root, int depth);
static struct AVLNode *newNode(T element);
static struct AVLNode *AVL_insert_recur(struct AVLNode *root, T element, compar_fn_t cmp);
static void fix_height(struct AVLNode *node);
// static struct AVLNode *leftmost_node_parent(struct AVLNode *root);
static struct AVLNode *AVL_delete_recur(struct AVLNode *root, T element, compar_fn_t cmp, bool *found);
static struct AVLNode *real_AVL_delete_recur(struct AVLNode **root, struct AVLNode * node);
static struct AVLNode *balance_height(struct AVLNode *root);
bool is_balanced(AVLTree t);
bool is_ordered(AVLTree t);

static int get_height(struct AVLNode *root) {
    if (root == NULL) return -1;
    else return root->height;
}

static int max_height(struct AVLNode *left, struct AVLNode *right) {
    if (left == NULL && right == NULL) return -1;
    if (left == NULL) return right->height;
    if (right == NULL) return left->height;
    return right->height > left->height? right->height: left->height;
}

static struct AVLNode *rotate_left(struct AVLNode *root) {
    struct AVLNode *new_root = root->right;
    struct AVLNode *pivot = new_root->left;
    root->right = pivot;
    new_root->left = root;
    root->height = max_height(root->left, root->right) + 1;
    new_root->height = max_height(new_root->left, new_root->right) + 1;
    return new_root;
}

static struct AVLNode *rotate_right(struct AVLNode *root) {
    struct AVLNode *new_root = root->left;
    struct AVLNode *pivot = new_root->right;
    root->left = pivot;
    new_root->right = root;
    root->height = max_height(root->left, root->right) + 1;
    new_root->height = max_height(new_root->left, new_root->right) + 1;
    return new_root;
}

void AVL_init(AVLTree *t) {
    *t = NULL;
}

void AVL_destroy(AVLTree *t) {
    AVL_destroy_recur(*t);
    *t = NULL;
}

static void AVL_destroy_recur(AVLTree root) {
    if (root == NULL) return;
    AVL_destroy_recur(root->left);
    AVL_destroy_recur(root->right);
    free(root);
}

int AVL_print(AVLTree root) {
    int n = AVL_print_recur(root, 0);
    printf("---------------\n");
    return n;
}

static int AVL_print_recur(AVLTree root, int depth) {
    if (root == NULL) return 0;
    int n = 0;
    n += AVL_print_recur(root->right, depth + 1);
    for (int i = 0; i < depth; i++) {
        printf("   ");
    }
    printf("%d\n", root->data);
    n += 1;
    n += AVL_print_recur(root->left, depth + 1);
    return n;
}

struct AVLNode *AVL_search(AVLTree root, T element, compar_fn_t cmp) {
    struct AVLNode *curr = root;
    while (curr != NULL) {
        int cmp_result = cmp(element, curr->data);
        if (cmp_result == 0) {
            struct AVLNode *next = curr->left;
            while (curr != NULL && cmp(element, next->data) == 0) {
                curr = next;
                next = curr->left;
            }
            break;
        } else if (cmp_result < 0) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    return curr;
}

struct AVLNode *AVL_rsearch(AVLTree root, T element, compar_fn_t cmp) {
    struct AVLNode *curr = root;
    while (curr != NULL) {
        int cmp_result = cmp(element, curr->data);
        if (cmp_result == 0) {
            struct AVLNode *next = curr->right;
            while (curr != NULL && cmp(element, next->data) == 0) {
                curr = next;
                next = curr->right;
            }
            break;
        } else if (cmp_result < 0) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    return curr;
}

struct range AVL_range_search(AVLTree root, T element, compar_fn_t cmp) {
    struct range ret;
    struct AVLNode *curr = root;
    while (curr != NULL) {
        int cmp_result = cmp(element, curr->data);
        if (cmp_result == 0) {
            break;
        } else if (cmp_result < 0) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }

    ret.start = curr;
    ret.end = curr;
    
    if (curr != NULL) {
        curr = ret.start->left;
        while (curr != NULL && cmp(element, curr->data) == 0) {
            ret.start = curr;
            curr = curr->left;
        }

        curr = ret.end->right;
        while (curr != NULL && cmp(element, curr->data) == 0) {
            ret.start = curr;
            curr = curr->right;
        }
    }
    return ret;
}

static struct AVLNode *newNode(T element) {
    struct AVLNode *node = malloc(sizeof(struct AVLNode));
    node->data = element;
    node->height = 0;
    node->left = node->right = NULL;
    return node;
}

void AVL_insert(AVLTree* tree, T element, compar_fn_t cmp) {
    if (tree == NULL) return;
    *tree = AVL_insert_recur(*tree, element, cmp);
}

static void fix_height(struct AVLNode *node) {
    node->height = max_height(node->left, node->right) + 1;
}

static struct AVLNode *AVL_insert_recur(struct AVLNode *root, T element, compar_fn_t cmp) {
    if (root == NULL) {
        return newNode(element);
    } else {
        int cmp_result = cmp(element, root->data);
        if (cmp_result <= 0) {
            root->left = AVL_insert_recur(root->left, element, cmp);
            if (get_height(root->left) > get_height(root->right) + 1) {
                if (get_height(root->left->right) > get_height(root->left->left)) {
                    root->left = rotate_left(root->left);
                    root->height = root->left->height + 1;
                }
                return rotate_right(root);
            }
        } else {
            root->right = AVL_insert_recur(root->right, element, cmp);
            if (get_height(root->right) > get_height(root->left) + 1) {
                if (get_height(root->right->left) > get_height(root->right->right)) {
                    root->right = rotate_right(root->right);
                    root->height = root->right->height + 1;
                }
                return rotate_left(root);
            }
        }
        fix_height(root);
        return root;
    }
}

bool AVL_delete(AVLTree *tree, T element, compar_fn_t cmp) {
    bool found = false;
    *tree = AVL_delete_recur(*tree, element, cmp, &found);
    return found;
}

static struct AVLNode *AVL_delete_recur(struct AVLNode *root, T element, compar_fn_t cmp, bool *found) {
    if (root == NULL) return NULL;
    int cmp_result = cmp(element, root->data);

    if (*found && cmp_result != 0) {
        *found = false;
        return root;
    } else if (cmp_result == 0) {
        *found = true;
        struct AVLNode *newLeft = AVL_delete_recur(root->left, element, cmp, found);
        if (!(*found) || newLeft == NULL) {
            *found = true;
            // this is the node to be deleted
            if (root->left == NULL) {
                struct AVLNode *ret = root->right;
                free(root);
                return ret;
            } else if (root->right == NULL) {
                struct AVLNode *ret = root->left;
                free(root);
                return ret;
            } else if (root->right->left == NULL) {
                struct AVLNode *ret = root->right;
                ret->left = root->left;
                free(root);
                fix_height(ret);
                return balance_height(ret);
            } else {
               struct AVLNode *node = real_AVL_delete_recur(&root, root->right);
               root->right = node;
            }
        } else {
            root->left = newLeft;
        }
    } else if (cmp_result < 0) {
        root->left = AVL_delete_recur(root->left, element, cmp, found);
    } else {
        root->right = AVL_delete_recur(root->right, element, cmp, found);
    }
    fix_height(root);
    return balance_height(root);
}

static struct AVLNode *real_AVL_delete_recur(struct AVLNode **root, struct AVLNode * node) {
    if (node->left->left == NULL) {
        // node->left will be the new root
        // and this node is currently the parent of the it
        struct AVLNode *new_root = node->left;
        node->left = new_root->right;
        new_root->right = (*root)->right;
        new_root->left = (*root)->left;
        free(*root);
        *root = new_root;
        fix_height(node);
        return balance_height(node);
    } else {
        node->left = real_AVL_delete_recur(root, node->left);
        fix_height(node);
        return balance_height(node);
    }
}

// static struct AVLNode *leftmost_node_parent(struct AVLNode *root) {
//     assert(root != NULL);
//     assert(root->left != NULL);
//     if (root->left->left == NULL) return root;
//     return leftmost_node_parent(root->left);
// }

static struct AVLNode *balance_height(struct AVLNode *root) {
    if (get_height(root->left) > get_height(root->right) + 1) {
        if (get_height(root->left->right) > get_height(root->left->left)) {
            root->left = rotate_left(root->left);
            fix_height(root);
        }
        return rotate_right(root);
    } else if (get_height(root->right) > get_height(root->left) + 1) {
        if (get_height(root->right->left) > get_height(root->right->right)) {
            root->right = rotate_right(root->right);
            fix_height(root);
        }
        return rotate_left(root);
    } else {
        return root;
    }
}

bool is_balanced(AVLTree t) {
    if (t == NULL) return true;
    assert(is_balanced(t->right));
    assert(is_balanced(t->left));
    assert(t->height == max_height(t->left, t->right) + 1);
    return (abs(get_height(t->left) - get_height(t->right)) < 2);
}

bool is_ordered(AVLTree t) {
    if (t == NULL) return true;
    assert(is_ordered(t->left));
    assert(is_ordered(t->right));
    if (t->left) {
        assert(t->left->data <= t->data);
    }
    if (t->right) {
        assert(t->right->data >= t->data);
    }
    return true;
}
