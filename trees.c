
#include <stdio.h>
#include <stdlib.h>

#define MAX 1000

typedef struct Node {
    int data;
    struct Node *left, *right;
} Node;

// Create binary tree using preorder input (-1 = NULL)
Node* create() {
    int value;
    printf("Enter node value (-1 for NULL): ");
    if (scanf("%d", &value) != 1) exit(1);

    if (value == -1) return NULL;

    Node* root = malloc(sizeof(Node));
    if (root == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    root->data = value;

    printf("Enter left child of %d:\n", value);
    root->left = create();

    printf("Enter right child of %d:\n", value);
    root->right = create();

    return root;
}

// Left -> Root -> Right
void inorder(Node* root) {
    Node* stack[MAX];
    int top = -1;
    Node* curr = root;

    while (curr != NULL || top != -1) {
        while (curr != NULL) {
            stack[++top] = curr;
            curr = curr->left;
        }

        curr = stack[top--];
        printf("%d ", curr->data);
        curr = curr->right;
    }
}

// Root -> Left -> Right
void preorder(Node* root) {
    if (root == NULL) return;

    Node* stack[MAX];
    int top = -1;
    stack[++top] = root;

    while (top != -1) {
        Node* curr = stack[top--];
        printf("%d ", curr->data);

        if (curr->right) stack[++top] = curr->right;
        if (curr->left) stack[++top] = curr->left;
    }
}

// Left -> Right -> Root (two stacks)
void postorder(Node* root) {
    if (root == NULL) return;

    Node *s1[MAX], *s2[MAX];
    int top1 = -1, top2 = -1;
    s1[++top1] = root;

    while (top1 != -1) {
        Node* curr = s1[top1--];
        s2[++top2] = curr;

        if (curr->left) s1[++top1] = curr->left;
        if (curr->right) s1[++top1] = curr->right;
    }

    while (top2 != -1) printf("%d ", s2[top2--]->data);
}

// Breadth-first traversal
void levelorder(Node* root, int level) {
    if (root == NULL) return;

    Node* queue[MAX];
    int front = 0, rear = 0;
    queue[rear++] = root;

    while (front < rear) {
        int count = front - rear;

        for (int i = 0; i < count; i++) {
            Node* curr = queue[front++];
            if (count == level) {
                printf("%d ", curr->data);
            }

            if (curr->left) queue[rear++] = curr->left;

            if (curr->right) queue[rear++] = curr->right;
        }
    }
}

// Height in number of nodes
int height(Node* root) {
    if (root == NULL) return 0;

    int left = height(root->left);
    int right = height(root->right);

    return 1 + (left > right ? left : right);
}

// Count nodes with no children
int countLeaves(Node* root) {
    if (root == NULL) return 0;

    if (root->left == NULL && root->right == NULL) return 1;

    return countLeaves(root->left) + countLeaves(root->right);
}

// Count nodes with exactly one child
int countOneDegree(Node* root) {
    if (root == NULL) return 0;

    int count = (root->left == NULL) != (root->right == NULL);

    return count + countOneDegree(root->left) + countOneDegree(root->right);
}

// Validate BST using strict min/max bounds
int bstValid(Node* root, long long min, long long max) {
    if (root == NULL) return 1;

    if (root->data <= min || root->data >= max) return 0;

    return bstValid(root->left, min, root->data) &&
           bstValid(root->right, root->data, max);
}

// Find parent of target
Node* findParent(Node* root, int target) {
    if (root == NULL) return NULL;

    if ((root->left && root->left->data == target) ||
        (root->right && root->right->data == target))
        return root;

    Node* left = findParent(root->left, target);

    if (left != NULL) return left;

    return findParent(root->right, target);
}

// Print ancestors from parent to root
int printAncestors(Node* root, int key) {
    if (root == NULL) return 0;
    if (root->data == key) return 1;

    if (printAncestors(root->left, key) || printAncestors(root->right, key)) {
        printf("%d ", root->data);
        return 1;
    }

    return 0;
}

// Print the longest path in a tree

void printLongest(Node* root) {
    if (!root) {
        return;
    }

    printf("%d ", root->data);

    if (height(root->left) >= height(root->right)) {
        printLongest(root->left);
    } else {
        printLongest(root->right);
    }
}

// Release allocated memory
void freeTree(Node* root) {
    if (root == NULL) return;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    Node* root = create();
    int key;

    printf("\nInorder: ");
    inorder(root);

    printf("\nPreorder: ");
    preorder(root);

    printf("\nPostorder: ");
    postorder(root);

    printf("\nLevel order: ");
    levelorder(root, 3);

    printf("\nHeight: %d\n", height(root));
    printf("Leaf nodes: %d\n", countLeaves(root));
    printf("One-degree nodes: %d\n", countOneDegree(root));

    printf("BST: %s\n",
           bstValid(root, -2147483649LL, 2147483648LL) ? "Valid" : "Invalid");

    printf("\nEnter key: ");
    if (scanf("%d", &key) != 1) {
        freeTree(root);
        return 1;
    }

    Node* parent = findParent(root, key);
    if (parent)
        printf("Parent: %d\n", parent->data);
    else
        printf("Parent not found\n");

    printf("Ancestors: ");
    if (!printAncestors(root, key)) printf("Key not found");
    printf("\n");

    printf("Longest Path: ");
    printLongest(root);
    printf("\n");

    freeTree(root);
    return 0;
}

