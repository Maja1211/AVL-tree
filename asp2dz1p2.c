#include <stdio.h>
#include <stdlib.h>

typedef struct stack {
    struct stack_node *head;
} stack;

typedef struct stack_node {
    struct stack_node *next;
    struct node *node;
    int height;
} stack_node;

typedef struct node {
    int key, balance;
    struct node *left, *right;
} node;

typedef struct set {
    struct node *root;
} set;


stack_node* createStackNode(node *node, int height) {
    stack_node *newStackNode = (stack_node*)malloc(sizeof(stack_node));
    if (newStackNode == NULL) {
        printf("Alokacija memorije za cvor steka nije uspela, prekidam program...");
        exit(0);
    }

    newStackNode->node = node;
    newStackNode->height = height;
    newStackNode->next = NULL;
    return newStackNode;
};

void freeStackNode(stack_node *stackNode) {
    free(stackNode);
}

stack* createStack() {
    stack *newStack = (stack*)malloc(sizeof(stack));
    if (newStack == NULL) {
        printf("Alokacija memorije za stek nije uspela, prekidam program...");
        exit(0);
    }

    newStack->head = NULL;
    return newStack;
}

int isEmpty(stack *stack) {
    return stack->head == NULL;
}

void push(stack *stack, node *node, int height) {
    stack_node *newStackNode = createStackNode(node, height);
    if (stack->head == NULL) {
        stack->head = newStackNode;
        return;
    }

    newStackNode->next = stack->head;
    stack->head = newStackNode;
}

stack_node* pop(stack *stack) {
    if (isEmpty(stack)) return NULL;
    stack_node *stackNode = stack->head;

    stack->head = stack->head->next;
    return stackNode;
}


node* createNode(int key) {
    node* newNode = (node*)malloc(sizeof(node));
    if (newNode == NULL) {
        printf("Alokacija memorije za cvor nije uspela, prekidam program...");
        exit(0);
    }

    newNode->key = key;
    newNode->balance = 0;
    newNode->left = newNode->right = NULL;
    return newNode;
}

set* createSet() {
    set *newSet = (set*)malloc(sizeof(set));
    if (newSet == NULL) {
        printf("Alokacija memorije za skup nije uspela, prekidam program...");
        exit(0);
    }

    newSet->root = NULL;
    return newSet;
}

void rightRotation(node *x) {
    node *y = x->left;
    node *temp = y->right;
    y->right = x;
    x->left = temp;
}

void leftRotation(node *x) {
    node *y = x->right;
    node *temp = y->left;
    y->left = x;
    x->right = temp;
}

void add(set *set, int key) {
    node *fp = NULL, *fx = NULL;
    node *p = set->root;
    node *x = p;

    node *q;
    while (p != NULL) {
        if (key == p->key) {
            return;
        }
        if (key < p->key) {
            q = p->left;
        } else {
            q = p->right;
        }

        if (q != NULL && q->balance != 0) {
            fx = p;
            x = q;
        }

        fp = p;
        p = q;
    }

    q = createNode(key);
    if (set->root == NULL) {
        set->root = q;
        return;
    }

    if (key < fp->key) {
        fp->left = q;
    } else {
        fp->right = q;
    }

    if (key < x->key) {
        p = x->left;
    } else {
        p = x->right;
    }

    node *y = p;
    while (p != q) {
        if (key < p->key) {
            p->balance = 1;
            p = p->left;
        }
        else {
            p->balance = -1;
            p = p->right;
        }
    }

    int imbal;
    if (key < x->key) {
        imbal = 1;
    } else {
        imbal = -1;
    }

    if (x->balance == 0) {
        x->balance = imbal;
        return;
    }

    if (x->balance != imbal) {
        x->balance = 0;
        return;
    }

    if (y->balance == imbal) {
        p = y;
        if (imbal == 1) {
            rightRotation(x);
        } else {
            leftRotation(x);
        }

        x->balance = y->balance = 0;
    } else {
        if (imbal == 1) {
            p = y->right;
            leftRotation(y);
            x->left = p;
            rightRotation(x);
        } else {
            p = y->left;
            x->right = p;
            rightRotation(y);
            leftRotation(x);
        }

        if (p->balance == 0) {
            x->balance = y->balance = 0;
        } else {
            if (p->balance == imbal) {
                x->balance = -imbal;
                y->balance = 0;
            }
            else {
                x->balance = 0;
                y->balance = imbal;
            }
            p->balance = 0;
        }
    }

    if (fx == NULL) {
        set->root = p;
    } else {
        if (x == fx->right) {
            fx->right = p;
        } else {
            fx->left = p;
        }
    }
}

int exists(set *set, int key) {
    node *node = set->root;

    while (node != NULL) {
        if (key == node->key) {
            return 1;
        } else if (key < node->key) {
            node = node->left;
        } else {
            node = node -> right;
        }
    }
    return 0;
}

void printSet(set *set) {
    if (set->root == NULL) return;

    node *node = set->root;
    stack *stack = createStack();

    int height = 0;
    while (1) {
        while (node != NULL) {
            height++;
            push(stack, node, height);
            node = node->right;
        }

        if (isEmpty(stack)) break;

        stack_node *stackNode = pop(stack);
        node = stackNode->node;
        height = stackNode->height;
        free(stackNode);

        for (int i = 0; i < height; i++) {
            printf("    ");
        }
        printf("%2d\n", node->key);

        node = node->left;
    }
    free(stack);
}

set* difference(set *set1, set *set2) {
    stack *stack = createStack();
    node *node = set1->root;

    set *newSet = createSet();

    while (1) {
        while (node != NULL) {
            push(stack, node, 0);
            node = node->right;
        }

        if (isEmpty(stack)) break;

        stack_node *stackNode = pop(stack);
        node = stackNode->node;
        free(stackNode);

        if (!exists(set2, node->key)) {
            add(newSet, node->key);
        }

        node = node->left;
    }

    free(stack);
    return newSet;
}

void freeSet(set *set) {
    stack *stack = createStack();
    node *node = set->root, *temp;

    while(1) {
        while (node != NULL) {
            push(stack, node, 0);
            node = node->right;
        }

        if (isEmpty(stack)) break;

        stack_node *stackNode = pop(stack);
        node = stackNode->node;
        free(stackNode);

        temp = node;
        node = node->left;
        free(temp);
    }

    free(stack);
}


int main() {
    set *set1 = NULL, *set2 = NULL;
    int currSet = 1;
    while (1) {
        printf("1. Napravi novi skup\n2. Dodaj elemente u skup\n3. Proveri pripadnost skupu\n4. Prikazi skup\n5. Izaberi skup\n6. Uradi presek skupova\n7. Prekini program\nIzaberi opciju:");

        int choice;
        scanf("%d", &choice);
        printf("\n");

        if (choice < 1 || choice > 7) {
            printf("Ta opcija ne postoji!\n\n");
            continue;
        } else if (choice != 1 && choice != 7 && choice != 5) {
            if (currSet == 1) {
                if (set1 == NULL) {
                    printf("Prvo morate da kreirate skup!\n\n");
                    continue;
                }
            } else {
                if (set2 == NULL) {
                    printf("Prvo morate da kreirate skup!\n\n");
                    continue;
                }
            }
        }

        switch(choice) {
            case 1:
                if (currSet == 1) {
                    if (set1 != NULL) freeSet(set1);
                    set1 = createSet();
                } else {
                    if (set2 != NULL) freeSet(set2);
                    set2 = createSet();
                }
                printf("Novi skup je uspesno kreiran!\n\n");
                break;
            case 2:
                printf("");
                int brElem;
                printf("Broj elemenata:");
                scanf("%d", &brElem);

                for (int i = 0; i < brElem; i++) {
                    printf("Dodaj element:");

                    int n;
                    scanf("%d", &n);

                    if (currSet == 1) {
                        add(set1, n);
                    } else {
                        add(set2, n);
                    }
                }
                printf("\n");
                break;
            case 3:
                printf("Unesi element:");

                int n;
                scanf("%d", &n);

                int returnValue;
                if (currSet == 1) {
                    returnValue = exists(set1, n);
                } else {
                    returnValue = exists(set2, n);
                }

                if (returnValue) {
                    printf("Element pripada skupu!\n\n");
                } else {
                    printf("Element ne pripada skupu!\n\n");
                }
                break;
            case 4:
                if (currSet == 1) {
                    printSet(set1);
                } else {
                    printSet(set2);
                }
                printf("\n");
                break;
            case 5:
                printf("Unesi izbor:");

                scanf("%d", &n);

                if (n < 1 || n > 2) {
                    printf("Izbor nije validan!\n\n");
                    continue;
                }

                currSet = n;
                break;
            case 6:
                if (set1 == NULL || set2 == NULL) {
                    printf("Nije moguce uraditi presek!\n\n");
                    continue;
                }

                printf("Razlika skupova:\n\n");
                set *set3;
                if (currSet == 1) {
                    set3 = difference(set1, set2);
                } else {
                    set3 = difference(set2, set1);
                }
                printSet(set3);
                freeSet(set3);
                printf("\n");
                break;
            case 7:
                if (set1 != NULL) freeSet(set1);
                if (set2 != NULL) freeSet(set2);
                return 0;
        }
    }
}
