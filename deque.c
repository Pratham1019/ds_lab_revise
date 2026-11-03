#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;             // Value stored at this position in the deque.
    struct Node* next;    // Link to the next node from front toward rear.
} Node;

// The deque is a singly linked list, so removing the rear requires a walk
// from the front to find the node immediately before it.
Node *front = NULL, *rear = NULL;

// A NULL front means there are no nodes in the deque.
int isEmpty() {
    return front == NULL;
}

// Add a value at the front (the next value removed by dequeueFront).
void enqueueFront(int value) {
    Node *node = malloc(sizeof *node);
    if (node == NULL) return;
    node->data = value;
    node->next = NULL;
    if (isEmpty()) {
        front = rear = node;
        return;
    }

    node->next = front;
    front = node;
}

// Remove and return the front value, or -1 when empty.
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
    if (node == NULL) return;
    node->data = value;
    node->next = NULL;
    if (isEmpty()) {
        front = rear = node;
        return;
    }

    // Attach after the old rear, then update the rear pointer.
    rear->next = node;
    rear = node;
}

// Remove and return the rear value, or -1 when empty.
int dequeueRear() {
    if (isEmpty()) {
        return -1;
    }

    Node *removed = rear;
    int value = removed->data;

    if (front == rear) {
        // The only node was removed, so both ends become empty.
        front = rear = NULL;
    } else {
        // In a singly linked list, walk to the node before rear.
        Node *previous = front;
        while (previous->next != rear) {
            previous = previous->next;
        }
        previous->next = NULL;
        rear = previous;
    }

    free(removed);
    return value;
}

