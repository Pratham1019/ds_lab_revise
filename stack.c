#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node *top = NULL;

void push(int value) {
    Node *node = malloc(sizeof *node);
    if (!node) {
        return;
    }

    node->data = value;
    node->next = NULL;

    if (top == NULL) {
        top = node;
        return;
    }

    node->next = top;
    top = node;
    return;
}

int pop() {
    if (top == NULL) {
        return -1;
    }

    Node *temp = top;
    int value = temp->data;

    top = top->next;
    free(temp);
    return value;
}

int peek() {
    if (top == NULL) {
        return -1;
    }

    return top->data;
}

void display() {
    if (!top) {
        printf("Stack is empty\n");
        return;
    }

    Node *current = top;
    printf("Top -> ");
    while (current) {
        printf("%d", current->data);
        if (current->next) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf(" -> NULL\n");
}

int isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}

int precedence(char op) {
    switch (op) {
        case '^': return 3;
        case '*':
        case '/': return 2;
        case '+':
        case '-': return 1;
        default: return 0;
    }
}

/* Converts an expression with single-character operands to postfix notation. */
void infixToPostfix(char infix[], char postfix[]) {
    int out = 0;
    for (int i = 0; infix[i] != '\0'; i++) {
        char ch = infix[i];

        if (ch == ' ' || ch == '\t' || ch == '\n') {
            continue;
        }
        if (isalnum(ch)) {
            postfix[out++] = ch;
        } else if (ch == '(') {
            push(ch);
        } else if (ch == ')') {
            while (top != NULL && peek() != '(') {
                postfix[out++] = (char)pop();
            }
            pop(); /* discard '(' */
        } else {
            while (top != NULL && isOperator((char)peek()) &&
                   (precedence((char)peek()) > precedence(ch) ||
                    (precedence((char)peek()) == precedence(ch) && ch != '^'))) {
                postfix[out++] = (char)pop();
            }
            push(ch);
        }
    }

    while (top) {
        postfix[out++] = (char)pop();
    }
    postfix[out] = '\0';
}

/* Converts an expression with single-character operands to prefix notation. */
void infixToPrefix(char infix[], char prefix[]) {
    char reversed[256];
    char postfix[256];
    int length = 0;

    while (infix[length] != '\0') {
        length++;
    }

    /* Reverse the input and swap parentheses. */
    for (int i = 0; i < length; i++) {
        char ch = infix[length - 1 - i];
        if (ch == '(') {
            reversed[i] = ')';
        } else if (ch == ')') {
            reversed[i] = '(';
        } else {
            reversed[i] = ch;
        }
    }
    reversed[length] = '\0';

    int out = 0;
    for (int i = 0; i < length; i++) {
        char ch = reversed[i];

        if (ch == ' ' || ch == '\t' || ch == '\n') {
            continue;
        }
        if (isalnum(ch)) {
            postfix[out++] = ch;
        } else if (ch == '(') {
            push(ch);
        } else if (ch == ')') {
            while (top != NULL && peek() != '(') {
                postfix[out++] = (char)pop();
            }
            pop(); /* discard '(' */
        } else {
            while (top != NULL && isOperator((char)peek()) &&
                   (precedence((char)peek()) > precedence(ch) ||
                    (precedence((char)peek()) == precedence(ch) && ch == '^'))) {
                postfix[out++] = (char)pop();
            }
            push(ch);
        }
    }

    while (top) {
        postfix[out++] = (char)pop();
    }
    postfix[out] = '\0';

    /* Reverse postfix to get prefix. */
    for (int i = 0; i < out; i++) {
        prefix[i] = postfix[out - 1 - i];
    }
    prefix[out] = '\0';
}

int main() {
    char infix[256];
    char prefix[256];
    char postfix[256];

    printf("Enter infix expression (single-character operands): ");
    if (scanf("%s", infix) != 1) {
        return 1;
    }

    infixToPostfix(infix, postfix);
    infixToPrefix(infix, prefix);

    printf("Postfix: %s\n", postfix);
    printf("Prefix: %s\n", prefix);
    return 0;
}
