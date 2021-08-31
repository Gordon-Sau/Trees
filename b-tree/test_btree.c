#include "2_3_4tree.h"
#include <stdio.h>

void print_arr(int *arr, int n);

int n;
int main(void) {
    int arr[100];
    BTree t = createBTree();

    while (scanf("%d", &n) == 1) {
        printf("insert: %d\n", n);
        insertBTree(t, n);
        showBTree(t);
        isBTree(t);
    }
    showBTree(t);
    
    int len = sortBTree(t, arr);
    print_arr(arr, len);

    getchar();
    
    while (scanf("%d", &n) == 1) {
        printf("delete: %d\n", n);
        deleteBTree(t, n);
        showBTree(t);
        isBTree(t);
    }
    
    len = sortBTree(t, arr);
    print_arr(arr, len);
    
    freeBTree(t);
    return 0;
}

void print_arr(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    putchar('\n');
}
