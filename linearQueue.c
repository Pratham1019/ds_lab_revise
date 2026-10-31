#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int val;
    struct Node* next;
} Node;

typedef struct {
    Node *front, *rear;
} Queue;

void initQueue(Queue* q) { q->front = q->rear = NULL; }

int isEmpty(Queue* q) { return q->front == NULL; }

// Only time this returns 0 is when malloc fails.
int enqueue(Queue* q, int value) {
    Node* node = malloc(sizeof *node);
    if (node == NULL) return 0;

    node->val = value;
    node->next = NULL;

    if (isEmpty(q)) {
        q->front = q->rear = node;
    } else {
        q->rear->next = node;
    }

    q->rear = node;
    return 1;
}

// Returns -1 when empty, so this version assumes queued values are nonnegative.
int dequeue(Queue* q) {
    if (isEmpty(q)) {
        return -1;
    }

    Node* oldFront = q->front;
    int value = oldFront->val;

    q->front = oldFront->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(oldFront);
    return value;
}

void freeQueue(Queue* q) {
    while (!isEmpty(q)) {
        dequeue(q);
    }
}

void displayQueue(const Queue *q) {
    if (isEmpty(q)) {
        printf("Queue is empty.\n");
        return;
    }

    for (Node *ptr = q->front; ptr != NULL; ptr = ptr->next) {
        printf("%d -> ", ptr->val);
    }

    printf("END\n");
}

int main() {
    Queue q;
    initQueue(&q);

    printf("New queue:\n");
    displayQueue(&q);

    printf("\nEnqueue 10, 20, 30:\n");
    if (!enqueue(&q, 10) || !enqueue(&q, 20) || !enqueue(&q, 30)) {
        fprintf(stderr, "Failed to allocate a queue node.\n");
        freeQueue(&q);
        return EXIT_FAILURE;
    }
    displayQueue(&q);

    printf("\nDequeue: ");
    int value = dequeue(&q);
    if (value == -1) {
        printf("queue is empty\n");
    } else {
        printf("%d\n", value);
    }
    displayQueue(&q);

    printf("\nDequeue remaining values:\n");
    while (!isEmpty(&q)) {
        printf("%d\n", dequeue(&q));
    }
    displayQueue(&q);

    printf("\nDequeue from empty queue: %d\n", dequeue(&q));

    // Also demonstrates freeing any nodes left in a nonempty queue.
    enqueue(&q, 40);
    enqueue(&q, 50);
    freeQueue(&q);
    printf("After freeQueue:\n");
    displayQueue(&q);

    return EXIT_SUCCESS;
}

