/*Binary tree traversal implementation
Author: Sisir
Date: May 31 2026*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define Color States
#define RED   0
#define BLACK 1

// Define the Red-Black Tree Node structure
struct Node {
    int data;
    int color;            // Tracks RED (0) or BLACK (1)
    struct Node* left;
    struct Node* right;
    struct Node* parent;  // Crucial for moving upwards to fix violations
};

// Global Sentinel Node representing all empty leaf entries (Ground Truth Black)
struct Node* NIL;

// --- INITIALIZATION UTILITIES ---

// Initializes the global NIL sentinel node block
void initNIL() {
    NIL = (struct Node*)malloc(sizeof(struct Node));
    NIL->data = 0;
    NIL->color = BLACK; // Rule 3: All leaves are considered Black
    NIL->left = NIL;
    NIL->right = NIL;
    NIL->parent = NIL;
}

// Spawns a fresh Red node on the Heap
struct Node* createNode(int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->data = value;
    newNode->color = RED; // Standard rule: New nodes are initially inserted as RED
    newNode->left = NIL;
    newNode->right = NIL;
    newNode->parent = NIL;
    return newNode;
}

// --- ROTATION UTILITIES (Pointer-Based Framework) ---

// Left Rotate around node x
void leftRotate(struct Node** root, struct Node* x) {
    struct Node* y = x->right;
    x->right = y->left;

    if (y->left != NIL) {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == NIL) {
        *root = y; // y becomes the new absolute root
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

// Right Rotate around node y
void rightRotate(struct Node** root, struct Node* y) {
    struct Node* x = y->left;
    y->left = x->right;

    if (x->right != NIL) {
        x->right->parent = y;
    }

    x->parent = y->parent;

    if (y->parent == NIL) {
        *root = x; // x becomes the new absolute root
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }

    x->right = y;
    y->parent = x;
}

// --- CORE RED-BLACK FIXATION LOGIC ---

// Iteratively walks up the parent pointers to eliminate Double-Red Violations
void fixInsertionViolation(struct Node** root, struct Node* z) {
    // While the parent node is RED (violating Rule 4: No adjacent Red nodes)
    while (z->parent->color == RED) {
        
        // CASE CLUSTER A: z's Parent is a LEFT child of Grandparent
        if (z->parent == z->parent->parent->left) {
            struct Node* uncle = z->parent->parent->right; // Uncle is the Right child

            // Case 1: Uncle is RED -> Pure Recolor
            if (uncle->color == RED) {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent; // Move check up to Grandparent
            } 
            else {
                // Case 2: Uncle is BLACK & z is a Right Child (Zig-Zag '<')
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(root, z); // Straightens the zig-zag into a line
                }
                // Case 3: Uncle is BLACK & z is a Left Child (Straight Line)
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(root, z->parent->parent);
            }
        } 
        // CASE CLUSTER B: z's Parent is a RIGHT child of Grandparent (Exact Mirror)
        else {
            struct Node* uncle = z->parent->parent->left; // Uncle is the Left child

            // Case 1 Mirror: Uncle is RED -> Pure Recolor
            if (uncle->color == RED) {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent; // Move check up to Grandparent
            } 
            else {
                // Case 2 Mirror: Uncle is BLACK & z is a Left Child (Zig-Zag '>')
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(root, z); // Straightens the zig-zag into a line
                }
                // Case 3 Mirror: Uncle is BLACK & z is a Right Child (Straight Line)
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(root, z->parent->parent);
            }
        }
    }
    (*root)->color = BLACK; // Rule 2 Safety Check: Ensure root stays Black
}

// Core Insertion Command
void insert(struct Node** root, int value) {
    struct Node* z = createNode(value);
    struct Node* y = NIL;
    struct Node* x = *root;

    // Standard BST routing down to find the leaf position
    while (x != NIL) {
        y = x;
        if (z->data < x->data) {
            x = x->left;
        } else if (z->data > x->data) {
            x = x->right;
        } else {
            free(z); // Ignore duplicates
            return;
        }
    }

    z->parent = y;
    if (y == NIL) {
        *root = z; // Tree was empty, z is root
    } else if (z->data < y->data) {
        y->left = z;
    } else {
        y->right = z;
    }

    // Fix any double-red violations caused by adding this node
    fixInsertionViolation(root, z);
}

// --- TRAVERSALS & CLEANUP ---

// In-order traversal tracking data alongside its active structural color
void inorderTraversal(struct Node* root) {
    if (root == NIL) return;
    inorderTraversal(root->left);
    printf("%d(%s) ", root->data, (root->color == RED) ? "RED" : "BLACK");
    inorderTraversal(root->right);
}

// Clear heap spaces safely
void freeTree(struct Node* root) {
    if (root == NIL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// --- DRIVER CONSOLE ---
int main() {
    initNIL(); // Crucial: Initialize sentinel ground state before doing anything
    struct Node* root = NIL;
    char inputBuffer[256];

    printf("Enter elements to create a Red-Black tree: ");
    if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL) {
        free(NIL);
        return 1;
    }

    char* token = inputBuffer;
    int value, numBytesRead;
    while (sscanf(token, "%d%n", &value, &numBytesRead) == 1) {
        insert(&root, value);
        token += numBytesRead;
    }

    printf("Red-Black Tree Output (In-order): ");
    inorderTraversal(root);
    printf("\n");

    if (root != NIL) {
        printf("Absolute Root Node of the RB-Tree is: %d (%s)\n", 
               root->data, (root->color == RED) ? "RED" : "BLACK");
    }

    freeTree(root);
    free(NIL); // Wipe out global sentinel block on final program teardown
    return 0;
}
