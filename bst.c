#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int val;
    struct TreeNode *right, *left;
} TreeNode;

TreeNode* createNode(int val) {
    TreeNode* node = malloc(sizeof(TreeNode));
    node->val = val;
    node->left = node->right = NULL;

    return node;
}

TreeNode* insert(TreeNode *root, int val) {
    if (root == NULL) {
        return createNode(val);
    }

    if (root->val > val) {
        root->left = insert(root->left, val);
    } else if (root->val < val) {
        root->right = insert(root->right, val);
    }

    return root;
}

// Searches for a key element in a BST. If found, returns that node, otherwise returns NULL
TreeNode* search(TreeNode *root, int key) {
    if (root == NULL || root->val == key) {
        return root;
    }

    if (key < root->val) {
        return search(root->left, key);
    }

    return search(root->right, key);
}

// Gives the min value from the BST. If found returns that node, otherwise returns NULL (only during empty tree)
TreeNode* minInBST(TreeNode *root) {
    if (root == NULL || root->left == NULL) {
        return root;
    }

    return minInBST(root->left);
}

// Gives the max value from the BST. If found returns that node, otherwise returns NULL (only during empty tree)
TreeNode* maxInBST(TreeNode *root) {
    if (root == NULL || root->right == NULL) {
        return root;
    }

    return maxInBST(root->right);
}

void inorder(TreeNode *root) {
    if (root == NULL) return;

    inorder(root->left);
    printf("%d ", root->val);
    inorder(root->right);
}

void preorder(TreeNode *root) {
    if (root == NULL) return;

    printf("%d ", root->val);
    preorder(root->left);
    preorder(root->right);
}

void postorder(TreeNode *root) {
    if (root == NULL) return;

    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->val);
}

void levelOrder(TreeNode *root) {
    if (root == NULL) return;

    TreeNode* queue[1000];
    int front = 0, rear = 0;

    queue[rear++] = root;
    while (front < rear) { // basically till the queue is not empty
        TreeNode* curr = queue[front++];
        printf("%d ", curr->val);

        if (curr->left != NULL) {
            queue[rear++] = curr->left;
        }

        if (curr->right != NULL) {
            queue[rear++] = curr->right;
        }
    }
}

void inorderIterative(TreeNode* root) {
    TreeNode *stack[1000];
    int top = -1;
    TreeNode *curr = root;

    while (top != -1 || curr != NULL) {
        while (curr != NULL) {
            stack[++top] = curr;
            curr = curr->left;
        }

        curr = stack[top--];
        printf("%d ", curr->val);

        curr = curr->right;
    }
}

TreeNode* delete(TreeNode *root, int key) {
    if (root == NULL) {
        return root;
    }

    if (key < root->val) {
        root->left = delete(root->left, key);
    } else if (key > root->val) {
        root->right = delete(root->right, key);
    } else {
        // Case 1 : Leaf Node
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }

        // Case 2a : One degree Node (right child there)
        if (root->left == NULL) {
            TreeNode* temp = root->right;
            free(root);
            return temp;
        }

        // Case 2b : One degree Node (left child there)
        if (root->right == NULL) {
            TreeNode* temp = root->left;
            free(root);
            return temp;
        }

        // Case 3 : Two children -> replace with min node from rst, delete that node again.
        TreeNode* temp = minInBST(root->right);
        root->val = temp->val;
        root->right = delete(root->right, temp->val);
    }

    return root;
}

int main() {
    TreeNode* root = NULL;
    int choice, val;

    // Initial BST
    int values[] = {50, 30, 70, 20, 40, 60, 80};

    for (int i = 0; i < 7; i++) {
        root = insert(root, values[i]);
    }

    while (1) {
        printf("\n\n--- BST OPERATIONS ---\n");
        printf("1. Insert\n");
        printf("2. Delete\n");
        printf("3. Search\n");
        printf("4. Minimum\n");
        printf("5. Maximum\n");
        printf("6. Inorder\n");
        printf("7. Preorder\n");
        printf("8. Postorder\n");
        printf("9. Level Order\n");
        printf("0. Exit\n");

        printf("\nEnter choice: ");
        if (scanf("%d", &choice) != 1)
            break;

        TreeNode* temp;

        switch (choice) {
            case 1:
                printf("Enter value: ");
                scanf("%d", &val);
                root = insert(root, val);
                printf("Insertion completed.\n");
                break;

            case 2:
                printf("Enter value: ");
                scanf("%d", &val);
                root = delete(root, val);
                printf("Deletion completed.\n");
                break;

            case 3:
                printf("Enter value: ");
                scanf("%d", &val);
                temp = search(root, val);
                printf(temp ? "Found!\n" : "Not found!\n");
                break;

            case 4:
                temp = minInBST(root);
                if (temp != NULL)
                    printf("Minimum: %d\n", temp->val);
                else
                    printf("Tree is empty.\n");
                break;

            case 5:
                temp = maxInBST(root);
                if (temp != NULL)
                    printf("Maximum: %d\n", temp->val);
                else
                    printf("Tree is empty.\n");
                break;

            case 6:
                printf("Inorder: ");
                inorder(root);
                printf("\n");
                break;

            case 7:
                printf("Preorder: ");
                preorder(root);
                printf("\n");
                break;

            case 8:
                printf("Postorder: ");
                postorder(root);
                printf("\n");
                break;

            case 9:
                printf("Level Order: ");
                levelOrder(root);
                printf("\n");
                break;

            case 0:
                return 0;

            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}

