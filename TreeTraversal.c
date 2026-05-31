/*Binary tree traversal implementation
Author: Sisir
Date: May 31 2026*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Define the structure for a tree node
struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

/*Function to create a new node*/
struct Node* createNode(int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

/*function to count the total nodes in a subtree*/
int countNodes(struct Node* root) {
    if (root == NULL) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

/*function to find the maximum height/depth of a tree*/
int getHeight(struct Node* root) {
    if (root == NULL) return 0;
    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

/*Function to insert a node into the binary tree*/
struct Node* insert(struct Node* root, int value) {
    if (root == NULL) {
        return createNode(value);
    }

    int leftCount = countNodes(root->left);
    int rightCount = countNodes(root->right);
    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);

    // Calculate max capacity for the current height of the left subtree
    // Formula: 2^height - 1
    int leftMaxCapacity = (int)pow(2, leftHeight) - 1;
    int rightMaxCapacity = (int)pow(2, rightHeight) - 1;

    // Condition 1: Left subtree is not full yet. Keep filling it.
    if (leftCount < leftMaxCapacity) {
        root->left = insert(root->left, value);
    }
    // Condition 2: Left subtree is full, but Right subtree is not full yet.
    else if (rightCount < rightMaxCapacity) {
        root->right = insert(root->right, value);
    }
    // Condition 3: Both subtrees are full to their current capacity.
    // If left height equals right height, we must grow the tree downwards by going left.
    else if (leftHeight == rightHeight) {
        root->left = insert(root->left, value);
    }
    // Otherwise, the left side grew first, so now the right side needs to catch up in height.
    else {
        root->right = insert(root->right, value);
    }

    return root;
}

/* 1. In-order Traversal: Left -> Root -> Right (Prints sorted numbers)*/
void inorderTraversal(struct Node* root) {
    if (root == NULL) return;
    inorderTraversal(root->left);
    printf("%d ", root->data);
    inorderTraversal(root->right);
}

/* 2. Pre-order Traversal: Root -> Left -> Right */
void preorderTraversal(struct Node* root) {
    if (root == NULL) return;
    printf("%d ", root->data);
    preorderTraversal(root->left);
    preorderTraversal(root->right);
}

/* 3. Post-order Traversal: Left -> Right -> Root */
void postorderTraversal(struct Node* root) {
    if (root == NULL) return;
    postorderTraversal(root->left);
    postorderTraversal(root->right);
    printf("%d ", root->data);
}

void freeTree(struct Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    struct Node* root = NULL;

    // Insert elements sequentially
    root = insert(root, 20); // Root
    insert(root, 2);        // Level 1 Left
    insert(root, 5);        // Level 1 Right
    insert(root, 27);       // Level 2 Left-Left
    insert(root, 43);       // Level 2 Left-Right
    insert(root, 28);       // Level 2 Right-Left
    insert(root, 21);       // Level 2 Right-Right
    insert(root, 35);       // Level 3 Left-Left-Left
    insert(root, 45);       // Level 4 Left-Left-Right

    /* This creates a Binary Tree of the following form:
                    20
                 /      \
                2        5
               /  \     /  \
              27  43   28  21
            /    \
           35    45
    */

    printf("In-order Traversal of the Balanced Tree:\n");
    inorderTraversal(root);
    printf("\n");

    printf("Pre-order Traversal of the Balanced Tree:\n");
    preorderTraversal(root);
    printf("\n");

    printf("Post-order Traversal of the Balanced Tree:\n");
    postorderTraversal(root);
    printf("\n");

    freeTree(root);
    return 0;
}
