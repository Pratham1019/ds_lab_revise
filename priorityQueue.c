#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;             // The value stored in the queue.
    int priority;         // Smaller numbers are served before larger numbers.
    struct Node *next;    // Link to the next item in priority order.
} Node;

typedef struct {
    Node *front, *rear;   // front is removed first; rear helps track the last node.
} PriorityQueue;

// Start with an empty queue. A NULL pointer is ignored safely.
void initQueue(PriorityQueue *pq) {
    if (pq == NULL) return;
    pq->front = pq->rear = NULL;
}

// A queue is empty if it is missing or has no first node.
int isEmpty(const PriorityQueue *pq) {
    return pq == NULL || pq->front == NULL;
}

// Allocate and initialize one node; return NULL if memory allocation fails.
Node *createNode(int value, int priority) {
    Node *newNode = malloc(sizeof *newNode);
    if (newNode == NULL) return NULL;

    newNode->data = value;
    newNode->priority = priority;
    newNode->next = NULL;
    return newNode;
}

// Insert the new node in ascending priority order (smallest number first).
// Nodes with equal priority stay in insertion order because we pass existing ties.
// Returns 1 on success, 0 if the queue is invalid or allocation fails.
int enqueue(PriorityQueue *pq, int value, int priority) {
    if (pq == NULL) {
        return 0;
    }

    Node *newNode = createNode(value, priority);

    if (newNode == NULL) {
        return 0;
    }

    if (isEmpty(pq)) {
        pq->front = pq->rear = newNode;
        return 1;
    }

    // If this priority comes before the current front, prepend the node.
    if (priority < pq->front->priority) {
        newNode->next = pq->front;
        pq->front = newNode;
        return 1;
    }

    // Otherwise find the last node whose priority is <= the new priority.
    Node *current = pq->front;
    while (current->next != NULL && current->next->priority <= priority) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;

    if (newNode->next == NULL) {
        pq->rear = newNode;
    }

    return 1;
}

// Removes and returns the highest-priority item (the front node), or -1 if empty.
// This simple version assumes -1 is not a valid value in the queue.
int dequeue(PriorityQueue *pq) {
    if (isEmpty(pq)) {
        return -1;
    }

    Node *temp = pq->front;
    int value = temp->data;

    // Advance the front; if it became NULL, the queue is now fully empty.
    pq->front = temp->next;
    if (pq->front == NULL) {
        pq->rear = NULL;
    }

    free(temp);
    return value;
}

void displayQueue(const PriorityQueue *pq) {
    if (isEmpty(pq)) {
        printf("Priority queue is empty.\n");
        return;
    }

    // Walk through the linked list from highest to lowest priority.
    for (const Node *current = pq->front; current != NULL; current = current->next) {
        printf("(value=%d, priority=%d) -> ", current->data, current->priority);
    }
    printf("NULL\n");
}

void freeQueue(PriorityQueue *pq) {
    if (pq == NULL) return;

    // Release every node so the queue does not leave allocated memory behind.
    while (pq->front != NULL) {
        Node *oldFront = pq->front;
        pq->front = oldFront->next;
        free(oldFront);
    }
    pq->rear = NULL;
}

int main(void) {
    PriorityQueue pq;
    initQueue(&pq);

    // Smaller number means higher priority.
    if (!enqueue(&pq, 100, 3) ||
        !enqueue(&pq, 200, 1) ||
        !enqueue(&pq, 300, 2) ||
        !enqueue(&pq, 400, 1)) {
        fprintf(stderr, "Could not allocate a priority queue node.\n");
        freeQueue(&pq);
        return EXIT_FAILURE;
    }

    printf("Queue ordered by priority (front is served first):\n");
    displayQueue(&pq);

    printf("\nDequeue order:\n");
    int value;
    while ((value = dequeue(&pq)) != -1) {
        printf("Removed value: %d\n", value);
    }

    printf("\nAfter removing all items:\n");
    displayQueue(&pq);
    freeQueue(&pq);
    return EXIT_SUCCESS;
}
