#include <stdio.h>
#include <stdlib.h>

struct node
{
    int identifier;
    int level;
    int height;
    struct node *parentPointer;
    struct node *leftPointer;
    struct node *rightPointer;
};

typedef struct node Node;

Node *createNode(int identifier, int level, Node *parentPointer);
void insert(Node **root, int identifier, int level, Node *parentPointer);
void createBT(Node **btPointer, int *identifiers, int size);
void showBT(Node *root);
int calculateHeight(Node *node);

int main(void)
{
    int identifiers[] = {10, 5, 15, 3, 7, 13, 20, 1, 4, 6};
    int size = sizeof(identifiers) / sizeof(identifiers[0]);

    Node *btPointer = NULL;

    createBT(&btPointer, identifiers, size);

    calculateHeight(btPointer);

    printf("Print da árvore:\n");
    showBT(btPointer);
    printf("\n");

    return 0;
}

Node *createNode(int identifier, int level, Node *parentPointer)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->identifier = identifier;
    newNode->level = level;
    newNode->height = 0;
    newNode->parentPointer = parentPointer;
    newNode->leftPointer = NULL;
    newNode->rightPointer = NULL;
    return newNode;
}

void insert(Node **root, int identifier, int level, Node *parentPointer)
{
    if (*root == NULL)
    {
        *root = createNode(identifier, level, parentPointer);
        return;
    }

    if (identifier < (*root)->identifier)
    {
        insert(&(*root)->leftPointer, identifier, level + 1, *root);
    }
    else
    {
        insert(&(*root)->rightPointer, identifier, level + 1, *root);
    }
}

void createBT(Node **btPointer, int *identifiers, int size)
{
    for (int i = 0; i < size; i++)
    {
        insert(btPointer, identifiers[i], 0, NULL);
    }
}

int calculateHeight(Node *node)
{
    if (node == NULL)
        return -1;

    int leftHeight = calculateHeight(node->leftPointer);
    int rightHeight = calculateHeight(node->rightPointer);

    node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);

    return node->height;
}

void showBT(Node *root)
{
    if (root == NULL)
        return;

    printf("(%d, Level: %d, Height: %d, Parent: %d)\n",
           root->identifier,
           root->level,
           root->height,
           root->parentPointer ? root->parentPointer->identifier : -1);
    showBT(root->leftPointer);
    showBT(root->rightPointer);
}
