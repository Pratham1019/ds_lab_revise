#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Node {
    int data;             // Value stored at this stack position.
    struct Node* next;    // Link to the item below this one.
} Node;

// The top pointer always refers to the most recently pushed node.
Node *top = NULL;

// Add a value to the top of the stack. If allocation fails, leave it unchanged.
void push(int value) {
    Node *node = malloc(sizeof *node);
    if (!node) {
        return;
    }

    node->data = value;
    node->next = NULL;

    // Link the new node to the old top, then make it the new top.
    node->next = top;
    top = node;
}

// Remove and return the top value; -1 means the stack was empty.
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

// Read the top value without removing it; -1 means the stack was empty.
int peek() {
    if (top == NULL) {
        return -1;
    }

    return top->data;
}

// Print from top to bottom so the linked-list structure is visible.
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

// The expression converters use this global stack. Clear leftover items first
// so an earlier conversion cannot affect the next one.
void clearStack(void) {
    while (top != NULL) {
        pop();
    }
}

// Check whether a character is one of the supported binary operators.
int isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}

// Larger numbers mean tighter binding (for example, * before +).
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

/* Convert single-character operands to postfix (operator follows its operands).
   Return 1 on success, or 0 for mismatched parentheses / output overflow. */
int infixToPostfix(const char infix[], char postfix[], size_t capacity) {
    int out = 0;
    int valid = 1;
    clearStack();

    for (int i = 0; infix[i] != '\0'; i++) {
        char ch = infix[i];

        if (ch == ' ' || ch == '\t' || ch == '\n') {
            continue;
        }
        if (isalnum(ch)) {
            if ((size_t)out + 1 >= capacity) {
                valid = 0;
                break;
            }
            postfix[out++] = ch;
        } else if (ch == '(') {
            push(ch);
        } else if (ch == ')') {
            while (top != NULL && peek() != '(') {
                if ((size_t)out + 1 >= capacity) {
                    valid = 0;
                    break;
                }
                postfix[out++] = (char)pop();
            }
            if (!valid || top == NULL) {
                valid = 0; // No matching opening parenthesis.
                break;
            }
            pop(); // Discard the matching '('.
        } else {
            if (!isOperator(ch)) {
                valid = 0;
                break;
            }
            // Pop operators that bind more tightly, and equal-precedence
            // operators when the current one is left-associative.
            while (top != NULL && isOperator((char)peek()) &&
                   (precedence((char)peek()) > precedence(ch) ||
                    (precedence((char)peek()) == precedence(ch) && ch != '^'))) {
                if ((size_t)out + 1 >= capacity) {
                    valid = 0;
                    break;
                }
                postfix[out++] = (char)pop();
            }
            if (!valid) break;
            push(ch);
        }
    }

    // Any remaining '(' means the expression had an unmatched opening bracket.
    while (top) {
        if (peek() == '(' || (size_t)out + 1 >= capacity) {
            valid = 0;
            break;
        }
        postfix[out++] = (char)pop();
    }
    clearStack();
    if (capacity > 0) postfix[out] = '\0';
    return valid;
}

/* Convert to prefix by reversing the infix expression, converting it to
   postfix with reversed associativity, and reversing that result. */
int infixToPrefix(const char infix[], char prefix[], size_t capacity) {
    char reversed[256];
    char postfix[256];
    int length = 0;
    clearStack();

    while (infix[length] != '\0') {
        if (length >= (int)sizeof reversed - 1) return 0;
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
    int valid = 1;
    for (int i = 0; i < length; i++) {
        char ch = reversed[i];

        if (ch == ' ' || ch == '\t' || ch == '\n') {
            continue;
        }
        if (isalnum(ch)) {
            if ((size_t)out + 1 >= sizeof postfix) {
                valid = 0;
                break;
            }
            postfix[out++] = ch;
        } else if (ch == '(') {
            push(ch);
        } else if (ch == ')') {
            while (top != NULL && peek() != '(') {
                if ((size_t)out + 1 >= sizeof postfix) {
                    valid = 0;
                    break;
                }
                postfix[out++] = (char)pop();
            }
            if (!valid || top == NULL) {
                valid = 0;
                break;
            }
            pop(); // Discard the matching '('.
        } else {
            if (!isOperator(ch)) {
                valid = 0;
                break;
            }
            // Reversed input needs the opposite equal-precedence rule so
            // operators such as subtraction keep their original grouping.
            while (top != NULL && isOperator((char)peek()) &&
                   (precedence((char)peek()) > precedence(ch) ||
                    (precedence((char)peek()) == precedence(ch) && ch == '^'))) {
                if ((size_t)out + 1 >= sizeof postfix) {
                    valid = 0;
                    break;
                }
                postfix[out++] = (char)pop();
            }
            if (!valid) break;
            push(ch);
        }
    }

    while (valid && top) {
        if (peek() == '(' || (size_t)out + 1 >= sizeof postfix) {
            valid = 0;
            break;
        }
        postfix[out++] = (char)pop();
    }
    postfix[out] = '\0';
    clearStack();
    if (!valid || (size_t)out + 1 > capacity) return 0;

    // Reverse postfix to get prefix.
    for (int i = 0; i < out; i++) {
        prefix[i] = postfix[out - 1 - i];
    }
    prefix[out] = '\0';
    return 1;
}

int main() {
    char infix[256];
    char prefix[256];
    char postfix[256];

    printf("Enter infix expression (single-character operands): ");
    // Limit input to the buffer size and stop at whitespace.
    if (scanf("%255s", infix) != 1) {
        return 1;
    }

    if (!infixToPostfix(infix, postfix, sizeof postfix) ||
        !infixToPrefix(infix, prefix, sizeof prefix)) {
        fprintf(stderr, "Invalid expression or expression is too long.\n");
        clearStack();
        return EXIT_FAILURE;
    }

    printf("Postfix: %s\n", postfix);
    printf("Prefix: %s\n", prefix);
    return 0;
}
