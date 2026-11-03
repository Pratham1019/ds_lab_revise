#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;             // Value stored in this queue item.
    struct Node* next;    // Points to the next item; the last points back to the front.
} Node;

// Keeping only the rear pointer lets us find both ends:
// the front is always rear->next.
Node *rear = NULL;

// Add a value at the rear of the queue.
void enqueue(int value) {
    Node *node = malloc(sizeof *node);
    if (node == NULL) {
        // The queue is unchanged if memory could not be allocated.
        return;
    }
    node->data = value;

    if (rear == NULL) {
        // The first node points to itself, forming a one-node circle.
        rear = node;
        node->next = rear;
        return;
    }

    // Insert after the old rear and before the front, then move rear forward.
    node->next = rear->next;
    rear->next = node;
    rear = node;
    return;
}

// Remove and return the front value, or -1 when the queue is empty.
int dequeue() {
    if (rear == NULL) {
        return -1;
    }

    Node *front = rear->next; // In a circular queue, the node after rear is the front.
    int temp = front->data;

    if (rear == front) {
        // Removing the only node makes the queue empty.
        rear = NULL;
    } else {
        // Skip the removed front node while keeping the circle connected.
        rear->next = front->next;
    }


    free(front);
    return temp;
}

// Read the front value without removing it, or -1 when empty.
int peek() {
    if (rear == NULL) {
        return -1;
    }

    return rear->next->data;
}

