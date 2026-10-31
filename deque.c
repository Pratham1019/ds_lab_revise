#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node *front = NULL, *rear = NULL;

int isEmpty() {
    return front == NULL;
}

void enqueueFront(int value) {
    Node *node = malloc(sizeof *node);
    node->data = value;
    if (isEmpty()) {
        front = rear = node;
        return;
    }

    node->next = front;
    front = node;
}

int dequeueFront() {
    if (isEmpty()) {
        return -1;
    }

    Node *temp = front;
    int value = temp->data;
    if (front == rear) {
        front = rear = NULL;
    } else {
        front = front->next;
    }

    free(temp);
    return value;
}

void enqueueRear(int value) {
    Node *node = malloc(sizeof *node);
    node->data = value;
    if (isEmpty()) {
        front = rear = node;
        return;
    }

    Node *temp = front;
    while (temp->next != rear) {
        
    }
}

int dequeueRear() {

}

