#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int centerID;
    char name[50], location[50];
    struct TreeNode *left, *right;
} TreeNode;

typedef struct Node {
    int requestID, centerID, quantity, priority;
    char item[50];
    struct Node *next;
} Node;


