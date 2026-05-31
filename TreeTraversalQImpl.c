/*Binary tree traversal implementation using Queue
Author: Sisir
Date: May 31 2026*/

#include <stdio.h>
#include <stdlib.h>

// Define the structure for a tree node
struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

// --- QUEUE IMPLEMENTATION FOR LEVEL-ORDER INSERTION ---
// A simple queue node to store tree node pointers
struct QueueNode {
    struct Node* treeNode;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode *front, *rear;
};

struct Queue* createQueue() {
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(struct Queue* q, struct Node* treeNode) {
    struct QueueNode* temp = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    temp->treeNode = treeNode;
    temp->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
    q->rear->next = temp;
    q->rear = temp;
}

struct Node* dequeue(struct Queue* q) {
    if (q->front == NULL) return NULL;
    struct QueueNode* temp = q->front;
    struct Node* treeNode = temp->treeNode;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    return treeNode;
}

int isQueueEmpty(struct Queue* q) {
    return q->front == NULL;
}

void freeQueue(struct Queue* q) {
    while (!isQueueEmpty(q)) {
        dequeue(q);
    }
    free(q);
}
// --- END OF QUEUE IMPLEMENTATION ---


/* Function to create a new node */
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

/* 
   MODIFIED: Function to insert a node into the binary tree using a Queue.
   This scans the tree level-by-level to find the first open spot.
*/
struct Node* insert(struct Node* root, int value) {
    // If the tree is empty, create the root node
    if (root == NULL) {
        return createNode(value);
    }

    // Initialize the queue and start processing from the root
    struct Queue* q = createQueue();
    enqueue(q, root);

    while (!isQueueEmpty(q)) {
        struct Node* temp = dequeue(q);

        // Check if the left child is empty
        if (temp->left == NULL) {
            temp->left = createNode(value);
            break; // Open slot found and filled, exit loop!
        } else {
            // Left is occupied, push it to the queue to look at its children later
            enqueue(q, temp->left);
        }

        // Check if the right child is empty
        if (temp->right == NULL) {
            temp->right = createNode(value);
            break; // Open slot found and filled, exit loop!
        } else {
            // Right is occupied, push it to the queue to look at its children later
            enqueue(q, temp->right);
        }
    }

    // Clean up queue memory to avoid leaks
    freeQueue(q);
    return root;
}

/* 1. In-order Traversal: Left -> Root -> Right */
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
    insert(root, 45);       // Level 3 Left-Left-Right

    /* This creates a Binary Tree of the following form:
                    20
                 /      \
                2        5
               /  \     /  \
              27  43   28  21
            /    \
           35    45
    */

    printf("In-order Traversal of the Queue-Based Tree:\n");
    inorderTraversal(root);
    printf("\n\n");

    printf("Pre-order Traversal of the Queue-Based Tree:\n");
    preorderTraversal(root);
    printf("\n\n");

    printf("Post-order Traversal of the Queue-Based Tree:\n");
    postorderTraversal(root);
    printf("\n");

    freeTree(root);
    return 0;
}
