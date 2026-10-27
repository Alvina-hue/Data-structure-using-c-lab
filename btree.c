
#include <stdio.h>
#include <stdlib.h>

#define M 4

typedef struct BTreeNode {
    int keys[M - 1];
    struct BTreeNode *child[M];
    int n;
    int leaf;
} BTreeNode;

BTreeNode *root = NULL;

// Create a new node
BTreeNode *createNode(int leaf)
{
    BTreeNode *t = (BTreeNode *)calloc(1, sizeof(BTreeNode));

    if (t == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    t->leaf = leaf;
    return t;
}

// Split a full child of the parent
void splitChild(BTreeNode *parent, int i)
{
    BTreeNode *y = parent->child[i];
    BTreeNode *z = createNode(y->leaf);

    int mid = (M - 1) / 2;
    int j;

    z->n = y->n - mid - 1;

    // Copy the larger keys to the new node
    for (j = 0; j < z->n; j++) {
        z->keys[j] = y->keys[mid + 1 + j];
    }

    // Copy child pointers if the node is not a leaf
    if (!y->leaf) {
        for (j = 0; j <= z->n; j++) {
            z->child[j] = y->child[mid + 1 + j];
        }
    }

    // Keep the smaller keys in the original node
    y->n = mid;

    // Shift parent keys and child pointers
    for (j = parent->n; j > i; j--) {
        parent->child[j + 1] = parent->child[j];
        parent->keys[j] = parent->keys[j - 1];
    }

    parent->child[i + 1] = z;
    parent->keys[i] = y->keys[mid];
    parent->n++;
}

// Insert into a node that is not full
void insertNonFull(BTreeNode *x, int key)
{
    int i = x->n - 1;

    if (x->leaf) {
        while (i >= 0 && key < x->keys[i]) {
            x->keys[i + 1] = x->keys[i];
            i--;
        }

        x->keys[i + 1] = key;
        x->n++;
    } else {
        while (i >= 0 && key < x->keys[i]) {
            i--;
        }

        i++;

        if (x->child[i]->n == M - 1) {
            splitChild(x, i);

            if (key > x->keys[i]) {
                i++;
            }
        }

        insertNonFull(x->child[i], key);
    }
}

// Insert a key into the B-Tree
void insert(int key)
{
    if (root == NULL) {
        root = createNode(1);
        root->keys[0] = key;
        root->n = 1;
        return;
    }

    if (root->n == M - 1) {
        BTreeNode *s = createNode(0);

        s->child[0] = root;
        splitChild(s, 0);
        root = s;
    }

    insertNonFull(root, key);
}

// Display keys in sorted order
void traverse(BTreeNode *x)
{
    int i;

    if (x == NULL) {
        return;
    }

    for (i = 0; i < x->n; i++) {
        if (!x->leaf) {
            traverse(x->child[i]);
        }

        printf("%d ", x->keys[i]);
    }

    if (!x->leaf) {
        traverse(x->child[x->n]);
    }
}

// Main function
int main()
{
    int choice, key;

    while (1) {
        printf("\n1. Insert\n");
        printf("2. Traversal\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            break;
        }

        switch (choice) {
            case 1:
                printf("Enter key: ");

                if (scanf("%d", &key) != 1) {
                    printf("Invalid key.\n");
                    return 1;
                }

                insert(key);
                break;

            case 2:
                printf("B-Tree traversal: ");
                traverse(root);
                printf("\n");
                break;

            case 3:
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
