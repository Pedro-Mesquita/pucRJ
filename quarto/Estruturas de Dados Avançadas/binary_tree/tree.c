#include <stdio.h>
#include <stdlib.h>

struct node
{
    struct node *left;
    struct node *right;
    int data;
};

typedef struct node Node;

Node *createNode(int data);
void simetric_traversal(Node *node);
void post_order_traversal(Node *node);
void pre_order_traversal(Node *node);
int treeHeight(Node *node);

int main(void)
{
    Node *root = createNode(0);
    Node *first = createNode(1);
    Node *second = createNode(2);
    Node *third = createNode(3);
    Node *fourth = createNode(4);

    root->left = first;
    root->right = second;
    first->left = third;
    first->right = fourth;

    /*
                    0
                 /      \
              1           2
            /   \
           3     4
    */

    int h = treeHeight(root);
    printf("%d\n", h);
    return 0;
}

int treeHeight(Node *node)
{
    if (node == NULL)
    {
        return 0;
    }
    unsigned int hleft = 0;
    unsigned int hright = 0;
    hleft = treeHeight(node->left);
    hright = treeHeight(node->right);

    if (hright > hleft)
    {
        return hright + 1;
    }
    return hleft + 1;
}

void simetric_traversal(Node *node)
{
    if (node)
    {
        simetric_traversal(node->left);
        printf("%d\n", node->data);
        simetric_traversal(node->right);
    }
}

void post_order_traversal(Node *node)
{
    if (node)
    {
        post_order_traversal(node->left);
        post_order_traversal(node->right);
        printf("%d\n", node->data);
    }
}

void pre_order_traversal(Node *node)
{
    if (node)
    {
        printf("%d\n", node->data);
        pre_order_traversal(node->left);
        pre_order_traversal(node->right);
    }
}

Node *createNode(int data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}
