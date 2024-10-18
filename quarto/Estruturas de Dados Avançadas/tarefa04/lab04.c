#include <stdio.h>
#include <stdlib.h>

struct node
{
    struct node *left;
    struct node *right;
    int data;
    int qtt;
};

typedef struct node Node;

Node *createNode(int data);
Node *binaryTree(Node *root, int *numbers, int size);
Node *insertNode(Node *root, int data);
int isBinarySearchTree(Node *tree, int min, int max);
void printIsBinarySearchTree(int confirmation);
Node *createManualTree();
Node *createValidBST();
void print_in_order_traversal(Node *node);
void count_print_in_order_traversal(Node *node);
int count(Node *node);

void runTests();

int main(void)
{
    int numbers[] = {15, 17, 3, 5, 2, 20, 25, 13, 10, 16};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    Node *root = NULL;
    root = binaryTree(root, numbers, size);

    printf("Antes:\n");
    print_in_order_traversal(root);
    printf("Depois:\n");
    count_print_in_order_traversal(root);

    printf("\n");

    printf("Verifica Árvore binária de busca\n");
    printf("\n");
    runTests();
    return 0;
}

Node *createNode(int data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->qtt = 0;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

Node *insertNode(Node *root, int data)
{
    if (root == NULL)
    {
        return createNode(data);
    }

    if (data < root->data)
    {
        root->left = insertNode(root->left, data);
    }
    else if (data > root->data)
    {
        root->right = insertNode(root->right, data);
    }

    return root;
}

Node *binaryTree(Node *root, int *numbers, int size)
{
    for (int i = 0; i < size; i++)
    {
        root = insertNode(root, numbers[i]);
    }

    return root;
}

void print_in_order_traversal(Node *node)
{
    if (node)
    {
        print_in_order_traversal(node->left);
        printf("Data: %d, children: %d\n", node->data, node->qtt);
        print_in_order_traversal(node->right);
    }
}

void count_print_in_order_traversal(Node *node)
{
    if (node)
    {
        count_print_in_order_traversal(node->left);
        node->qtt = (count(node) - 1);
        printf("Data: %d, children: %d\n", node->data, node->qtt);
        count_print_in_order_traversal(node->right);
    }
}

int count(Node *node)
{
    if (node == NULL)
    {
        return 0;
    }
    return 1 + count(node->left) + count(node->right);
}

int isBinarySearchTree(Node *tree, int min, int max)
{
    if (tree == NULL)
    {
        return 1;
    }

    if (tree->data <= min || tree->data >= max)
    {
        return 0;
    }

    return isBinarySearchTree(tree->left, min, tree->data) &&
           isBinarySearchTree(tree->right, tree->data, max);
}

void printIsBinarySearchTree(int confirmation)
{
    if (confirmation == 1)
    {
        printf("É uma árvore binária de busca\n");
    }
    else
    {
        printf("Não é uma árvore binária de busca\n");
    }
}

Node *createValidBST()
{
    Node *root = NULL;
    int numbers[] = {15, 10, 20, 8, 12, 18, 25};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    root = binaryTree(root, numbers, size);
    return root;
}

Node *createEmptyTree()
{
    return NULL;
}

Node *createSingleNodeTree()
{
    return createNode(42);
}

void runTests()
{
    Node *validBST = createValidBST();
    printIsBinarySearchTree(isBinarySearchTree(validBST, -1000, 1000));

    Node *emptyTree = createEmptyTree();
    printIsBinarySearchTree(isBinarySearchTree(emptyTree, -1000, 1000));

    Node *singleNodeTree = createSingleNodeTree();
    printIsBinarySearchTree(isBinarySearchTree(singleNodeTree, -1000, 1000));
}
