#include <stdio.h>
#include <stdlib.h>

struct node
{
    int identifier;
    struct node *leftPointer;
    struct node *rightPointer;
};

typedef struct node Node;

void createBT(Node **btPointer, int *identifiers, int size);
Node *createNode(int identifier);
void showBT(Node *root);

int main(void)
{

    int identifiers[] = {10, 5, 15, 3, 7, 13, 20, 1, 4, 6};
    int size = sizeof(identifiers) / sizeof(identifiers[0]);

    Node *btPointer = NULL;

    createBT(&btPointer, identifiers, size);

    printf("Print da árvore: ");
    showBT(btPointer);
    printf("\n");

    return 0;
}

Node *createNode(int identifier)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->identifier = identifier;
    newNode->leftPointer = NULL;
    newNode->rightPointer = NULL;
    return newNode;
}

void createBT(Node **btPointer, int *identifiers, int size)
{
    if (size == 0)
        return;

    *btPointer = createNode(identifiers[0]);

    Node *queue[10];
    int front = 0, rear = 0;

    queue[rear++] = *btPointer;

    for (int i = 1; i < size; i++)
    {
        Node *current = queue[front++];

        if (current->leftPointer == NULL)
        {
            current->leftPointer = createNode(identifiers[i]);
            queue[rear++] = current->leftPointer;
        }
        else if (current->rightPointer == NULL)
        {
            current->rightPointer = createNode(identifiers[i]);
            queue[rear++] = current->rightPointer;
        }
    }
}

void showBT(Node *root)
{
    if (root == NULL)
    {
        return;
    }

    showBT(root->leftPointer);
    printf("%d ", root->identifier);
    showBT(root->rightPointer);
}
