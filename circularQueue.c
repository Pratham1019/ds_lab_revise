#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node *rear = NULL;

void enqueue(int value) {
    Node *node = malloc(sizeof *node);
    node->data = value;

    if (rear == NULL) {
        rear = node;
        node->next = rear;
        return;
    }

    node->next = rear->next;
    rear->next = node;
    rear = node;
    return;
}

int dequeue() {
    if (rear == NULL) {
        return -1;
    }

    Node *front = rear->next;
    int temp = front->data;

    if (rear == front) {
        rear = NULL;
    } else {
        rear->next = front->next;
    }


    free(front);
    return temp;
}

int peek() {
    if (rear == NULL) {
        return -1;
    }

    return rear->next->data;
}

