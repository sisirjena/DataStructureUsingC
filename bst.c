/*Binary tree traversal implementation
Author: Sisir
Date: May 31 2026*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the BST Node structure
struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

// 1. CREATE NODE
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

// 2. INSERT
struct Node* insert(struct Node* root, int value) {
    if (root == NULL) {
        return createNode(value);
    }
    if (value < root->data) {
        root->left = insert(root->left, value);
    } else if (value > root->data) {
        root->right = insert(root->right, value);
    }
    return root;
}

// 3. SEARCH
struct Node* search(struct Node* root, int key) {
    if (root == NULL || root->data == key) {
        return root;
    }
    if (key < root->data) {
        return search(root->left, key);
    }
    return search(root->right, key);
}

// 4. MINIMUM
struct Node* findMin(struct Node* root) {
    if (root == NULL) return NULL;
    struct Node* current = root;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

// 5. MAXIMUM
struct Node* findMax(struct Node* root) {
    if (root == NULL) return NULL;
    struct Node* current = root;
    while (current->right != NULL) {
        current = current->right;
    }
    return current;
}

// 6. DELETE
struct Node* deleteNode(struct Node* root, int key) {
    if (root == NULL) return root;

    if (key < root->data) {
        root->left = deleteNode(root->left, key);
    } 
    else if (key > root->data) {
        root->right = deleteNode(root->right, key);
    } 
    else {
        // Case 1 & 2: No child or Only One child
        if (root->left == NULL) {
            struct Node* temp = root->right;
            free(root);
            return temp; 
        } 
        else if (root->right == NULL) {
            struct Node* temp = root->left;
            free(root);
            return temp;
        }

        // Case 3: Two children
        struct Node* temp = findMin(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
    }
    return root;
}

// In-order traversal helper
void inorderTraversal(struct Node* root) {
    if (root == NULL) return;
    inorderTraversal(root->left);
    printf("%d ", root->data);
    inorderTraversal(root->right);
}

void freeTree(struct Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    struct Node* root = NULL;
    char inputBuffer[1024];

    // Step 1: Input line of elements to build the tree
    printf("Enter elements to create a bst: ");
    if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL) {
        return 1;
    }

    // Parse the entire line string into individual integers
    char* token = inputBuffer;
    int value, numBytesRead;
    while (sscanf(token, "%d%n", &value, &numBytesRead) == 1) {
        root = insert(root, value);
        token += numBytesRead; // Move pointer past the parsed number
    }

    // Step 2: Show In-order, Min, and Max
    printf("BST Sorted Output (In-order): ");
    inorderTraversal(root);
    printf("\n");

    struct Node* minNode = findMin(root);
    struct Node* maxNode = findMax(root);
    if (minNode) printf("Minimum Value in BST: %d\n", minNode->data);
    if (maxNode) printf("Maximum Value in BST: %d\n", maxNode->data);

    // Step 3: Search Operation
    int searchKey;
    printf("Enter the element to search: ");
    if (scanf("%d", &searchKey) == 1) {
        struct Node* found = search(root, searchKey);
        if (found != NULL) {
            printf("Search Result: Value %d was found in the BST!\n", searchKey);
        } else {
            printf("Search Result: Value %d was NOT found.\n", searchKey);
        }
    }

    // Step 4: Deletion Operation
    int deleteKey;
    printf("Enter the element to delete: ");
    if (scanf("%d", &deleteKey) == 1) {
        root = deleteNode(root, deleteKey);
        printf("Updated BST Output (In-order): ");
        inorderTraversal(root);
        printf("\n");
    }

    // Clean up memory
    freeTree(root);
    return 0;
}
