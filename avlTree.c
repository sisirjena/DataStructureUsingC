/*AVL Tree implementation
Author: Sisir
Date: May 31 2026*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the AVL Tree Node structure
struct Node {
    int data;
    struct Node* left;
    struct Node* right;
    int height; // Crucial for instant Balance Factor evaluations
};

// --- HELPER UTILITIES ---

// Returns the height of a node, safely handling NULL pointers
int getHeight(struct Node* n) {
    if (n == NULL) return 0;
    return n->height;
}

// Returns the maximum of two values
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Computes the Balance Factor of a node
int getBalanceFactor(struct Node* n) {
    if (n == NULL) return 0;
    return getHeight(n->left) - getHeight(n->right);
}

// Spawns a fresh node on the system Heap
struct Node* createNode(int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1; // Leaf node starts at height 1
    return newNode;
}

// Finds the absolute minimum node in a given subtree
struct Node* findMin(struct Node* root) {
    if (root == NULL) return NULL;
    struct Node* current = root;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

// Finds the absolute maximum node in a given subtree
struct Node* findMax(struct Node* root) {
    if (root == NULL) return NULL;
    struct Node* current = root;
    while (current->right != NULL) {
        current = current->right;
    }
    return current;
}

// Executed when a node is searched along the BST paths
struct Node* search(struct Node* root, int key) {
    if (root == NULL || root->data == key) {
        return root;
    }
    if (key < root->data) {
        return search(root->left, key);
    }
    return search(root->right, key);
}

// --- ROTATION PARADIGMS ---

// Right Rotate: Fixes Left-Left (LL) heavy skews
struct Node* rightRotate(struct Node* y) {
    struct Node* x = y->left;
    struct Node* T2 = x->right;

    // Perform pointer swapping
    x->right = y;
    y->left = T2;

    // Recalculate heights after structural modification
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x; // New root of this subtree
}

// Left Rotate: Fixes Right-Right (RR) heavy skews
struct Node* leftRotate(struct Node* x) {
    struct Node* y = x->right;
    struct Node* T2 = y->left;

    // Perform pointer swapping
    y->left = x;
    x->right = T2;

    // Recalculate heights after structural modification
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y; // New root of this subtree
}

// --- CORE AVL OPERATIONS ---

// Insert a node with automated self-balancing
struct Node* insert(struct Node* node, int value) {
    // 1. Standard BST insertion cascade
    if (node == NULL) {
        return createNode(value);
    }
    if (value < node->data) {
        node->left = insert(node->left, value);
    } else if (value > node->data) {
        node->right = insert(node->right, value);
    } else {
        return node; // Deduplication: duplicates are ignored
    }

    // 2. Track updated heights up the unwinding call stack
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    // 3. Inspect Balance Factor for safety boundary breaches
    int balance = getBalanceFactor(node);

    // Case 1: Left-Left (LL) Heavy
    if (balance > 1 && value < node->left->data) {
        return rightRotate(node);
    }

    // Case 2: Right-Right (RR) Heavy
    if (balance < -1 && value > node->right->data) {
        return leftRotate(node);
    }

    // Case 3: Left-Right (LR) Heavy (Zig-Zag '<')
    if (balance > 1 && value > node->left->data) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Case 4: Right-Left (RL) Heavy (Zig-Zag '>')
    if (balance < -1 && value < node->right->data) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Delete a node with continuous structural rebalancing
struct Node* deleteNode(struct Node* root, int key) {
    // 1. Perform standard recursive BST deletion
    if (root == NULL) return root;

    if (key < root->data) {
        root->left = deleteNode(root->left, key);
    } 
    else if (key > root->data) {
        root->right = deleteNode(root->right, key);
    } 
    else {
        // Target Node isolated! Evaluate structural state
        if ((root->left == NULL) || (root->right == NULL)) {
            struct Node* temp = root->left ? root->left : root->right;

            if (temp == NULL) { // Case A: Leaf node removal
                temp = root;
                root = NULL;
            } else { // Case B: Single child preservation
                *root = *temp; // Copy content of child into root
            }
            free(temp);
        } 
        else {
            // Case C: Target has 2 children. Extract In-order Successor
            struct Node* temp = findMin(root->right);
            root->data = temp->data; // Swap values
            root->right = deleteNode(root->right, temp->data); // Purge original block
        }
    }

    if (root == NULL) return root;

    // 2. Refresh heights up the unwinding trace
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    // 3. Evaluate balance factors
    int balance = getBalanceFactor(root);

    // Case 1: Left-Left Heavy (LL)
    if (balance > 1 && getBalanceFactor(root->left) >= 0) {
        return rightRotate(root);
    }

    // Case 2: Left-Right Heavy (LR)
    if (balance > 1 && getBalanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Case 3: Right-Right Heavy (RR)
    if (balance < -1 && getBalanceFactor(root->right) <= 0) {
        return leftRotate(root);
    }

    // Case 4: Right-Left Heavy (RL)
    if (balance < -1 && getBalanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// In-order Traversal prints perfectly sorted sequences for confirmation
void inorderTraversal(struct Node* root) {
    if (root == NULL) return;
    inorderTraversal(root->left);
    printf("%d ", root->data);
    inorderTraversal(root->right);
}

// Clean up memory space
void freeTree(struct Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// --- INTERACTIVE DRIVER ENGINE ---
int main() {
    struct Node* root = NULL;
    char inputBuffer[256];

    // Read single-line sequence of values from terminal
    printf("Enter elements to create an AVL tree: ");
    if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL) {
        return 1;
    }

    // Process space-separated string integers systematically
    char* token = inputBuffer;
    int value, numBytesRead;
    while (sscanf(token, "%d%n", &value, &numBytesRead) == 1) {
        root = insert(root, value);
        token += numBytesRead;
    }

    printf("AVL Sorted Output (In-order): ");
    inorderTraversal(root);
    printf("\n");

    // Print Root and total Height attributes
    if (root != NULL) {
        printf("Current balanced Root of the tree is: %d\n", root->data);
        printf("Total calculated Height of the tree is: %d\n", root->height);
    }

    // Extreme tracking testing
    struct Node* minNode = findMin(root);
    struct Node* maxNode = findMax(root);
    if (minNode) printf("Minimum Value in AVL: %d\n", minNode->data);
    if (maxNode) printf("Maximum Value in AVL: %d\n", maxNode->data);

    // Dynamic search lookup test
    int searchKey;
    printf("Enter the element to search: ");
    if (scanf("%d", &searchKey) == 1) {
        struct Node* found = search(root, searchKey);
        if (found != NULL) {
            printf("Search Result: Value %d was found in the AVL tree!\n", searchKey);
        } else {
            printf("Search Result: Value %d was NOT found.\n", searchKey);
        }
    }

    // Dynamic self-balancing deletion test
    int deleteKey;
    printf("Enter the element to delete: ");
    if (scanf("%d", &deleteKey) == 1) {
        root = deleteNode(root, deleteKey);
        printf("Updated AVL Output (In-order): ");
        inorderTraversal(root);
        printf("\n");
        if (root != NULL) {
            printf("New Root after deletion is: %d\n", root->data);
            printf("New Height after deletion is: %d\n", root->height);
        }
    }

    freeTree(root);
    return 0;
}
