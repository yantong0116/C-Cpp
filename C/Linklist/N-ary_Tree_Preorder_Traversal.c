#include <stdio.h>
#include <stdlib.h>

struct Node {
    int val;
    int numChildren;
    struct Node** children;
};

void search(struct Node* root, int* arr, int* count) {
    if (!root) return;
    arr[(*count)++] = root->val;
    for (int i = 0; i < root->numChildren; i++) {
        search(root->children[i], arr, count);
    }
}

int* preorder(struct Node* root, int* returnSize) {
    int* arr = (int*) malloc(10000 * sizeof(int));
    int count = 0;
    search(root, arr, &count);
    *returnSize = count;
    return arr;
}

int main() {
    // Create the following tree:
    //        1
    //       / \
    //      3   2
    //     / \   \
    //    5   6   4
    struct Node* root = (struct Node*) malloc(sizeof(struct Node));
    root->val = 1;
    root->numChildren = 2;
    root->children = (struct Node**) malloc(root->numChildren * sizeof(struct Node*));

    struct Node* node1 = (struct Node*) malloc(sizeof(struct Node));
    node1->val = 3;
    node1->numChildren = 2;
    node1->children = (struct Node**) malloc(node1->numChildren * sizeof(struct Node*));

    struct Node* node2 = (struct Node*) malloc(sizeof(struct Node));
    node2->val = 2;
    node2->numChildren = 1;
    node2->children = (struct Node**) malloc(node2->numChildren * sizeof(struct Node*));

    struct Node* node3 = (struct Node*) malloc(sizeof(struct Node));
    node3->val = 5;
    node3->numChildren = 0;
    node3->children = NULL;

    struct Node* node4 = (struct Node*) malloc(sizeof(struct Node));
    node4->val = 6;
    node4->numChildren = 0;
    node4->children = NULL;

    struct Node* node5 = (struct Node*) malloc(sizeof(struct Node));
    node5->val = 4;
    node5->numChildren = 0;
    node5->children = NULL;

    root->children[0] = node1;
    root->children[1] = node2;
    node1->children[0] = node3;
    node1->children[1] = node4;
    node2->children[0] = node5;

    int returnSize;
    int* result = preorder(root, &returnSize);

    printf("Preorder traversal: ");
    for (int i = 0; i < returnSize; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");

    free(root->children);
    free(node1->children);
    free(root);
    free(node1);
    free(node2);
    free(node3);
    free(node4);
    free(node5);
    free(result);
    return 0;
}
