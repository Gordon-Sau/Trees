#include <stdio.h>
#include "AVLTree.h"
#include <assert.h>
#include <stdbool.h>
static int compare(T, T);

int main(void) {
    AVLTree t;
    AVL_init(&t);
    int n;
    int num = 0;
    int c = getchar();
    while (c != EOF){
        if (c == 'i') {
            while (scanf("%d", &n) == 1) {
                printf("insert: %d\n", n);
                AVL_insert(&t, n, &compare);
                num += 1;
                assert(is_balanced(t));
                assert(is_ordered(t));
                assert(AVL_print(t) == num);
            }
        } else if (c == 'd') {
            while (scanf("%d", &n) == 1) {
                printf("delete: %d\n", n);
                num -= AVL_delete(&t, n, &compare);
                assert(is_balanced(t));
                assert(is_ordered(t));
                assert(AVL_print(t) == num);
            }
        } else if (c == 'q') break;
        c = getchar();
    }
    AVL_destroy(&t);
    return 0;
}

int compare(T a , T b) {
    return a - b;
}

