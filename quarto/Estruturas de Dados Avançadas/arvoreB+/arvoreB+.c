#include <stdio.h>
#include <stdlib.h>

#define MAX_KEYS 2
#define MAX_CHILDREN 3

typedef struct Node
{
    int key1, key2;
    struct Node *pointer1, *pointer2, *pointer3;
    int isLeaf;
} Node;

Node *create_node(int isLeaf);
void free_tree(Node *node);
void print_tree(Node *node, int level);
Node *search_key(Node *node, int key);
void split_node(Node **root, Node *parent, Node *node, int key, Node *new_pointer);
void insert_key(Node **root, int key);
void remove_key(Node **root, Node *parent, Node *node, int key);

int main()
{
    Node *root = NULL;

    insert_key(&root, 3);
    insert_key(&root, 18);
    insert_key(&root, 12);
    insert_key(&root, 13);
    insert_key(&root, 8);
    insert_key(&root, 20);
    insert_key(&root, 11);
    insert_key(&root, 2);
    insert_key(&root, 9);
    insert_key(&root, 5);

    printf("Árvore com inserções:\n");
    print_tree(root, 0);

    int key = 18;
    Node *result = search_key(root, key);
    if (result)
    {
        printf("\n%d encontrada", key);
        if (result->key1 != -1)
            printf("%d ", result->key1);
        if (result->key2 != -1)
            printf("%d ", result->key2);
        printf("\n");
    }
    else
    {
        printf("\nChave %d não encontrada\n", key);
    }

    remove_key(&root, NULL, root, key);
    printf("\n%d removido\n", key);
    print_tree(root, 0);

    free_tree(root);
    return 0;
}

Node *create_node(int isLeaf)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node)
    {
        fprintf(stderr, "Erro ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }
    new_node->key1 = -1;
    new_node->key2 = -1;
    new_node->pointer1 = NULL;
    new_node->pointer2 = NULL;
    new_node->pointer3 = NULL;
    new_node->isLeaf = isLeaf;
    return new_node;
}

void free_tree(Node *node)
{
    if (!node)
        return;
    free_tree(node->pointer1);
    free_tree(node->pointer2);
    free_tree(node->pointer3);
    free(node);
}

void print_tree(Node *node, int level)
{
    if (!node)
        return;

    printf("Nível %d: ", level);

    if (node->key1 != -1)
        printf("%d ", node->key1);
    if (node->key2 != -1)
        printf("%d ", node->key2);
    printf("\n");

    if (!node->isLeaf)
    {
        print_tree(node->pointer1, level + 1);
        print_tree(node->pointer2, level + 1);
        print_tree(node->pointer3, level + 1);
    }
}

Node *search_key(Node *node, int key)
{
    while (node)
    {
        if (key == node->key1 || key == node->key2)
        {
            return node;
        }
        if (key < node->key1)
        {
            node = node->pointer1;
        }
        else if (node->key2 == -1 || key < node->key2)
        {
            node = node->pointer2;
        }
        else
        {
            node = node->pointer3;
        }
    }
    return NULL;
}

void split_node(Node **root, Node *parent, Node *node, int key, Node *new_pointer)
{
    int keys[MAX_CHILDREN] = {node->key1, node->key2, key};
    Node *children[4] = {node->pointer1, node->pointer2, node->pointer3, new_pointer};

    for (int i = 0; i < MAX_KEYS; i++)
    {
        for (int j = i + 1; j < MAX_CHILDREN; j++)
        {
            if (keys[i] > keys[j])
            {
                int temp = keys[i];
                keys[i] = keys[j];
                keys[j] = temp;

                Node *temp_ptr = children[i + 1];
                children[i + 1] = children[j + 1];
                children[j + 1] = temp_ptr;
            }
        }
    }

    Node *new_node = create_node(node->isLeaf);
    new_node->key1 = keys[MAX_KEYS];
    new_node->pointer1 = children[MAX_KEYS];
    new_node->pointer2 = children[MAX_CHILDREN];

    node->key1 = keys[0];
    node->key2 = -1;
    node->pointer1 = children[0];
    node->pointer2 = children[1];
    node->pointer3 = NULL;

    if (!parent)
    {
        *root = create_node(0);
        (*root)->key1 = keys[1];
        (*root)->pointer1 = node;
        (*root)->pointer2 = new_node;
    }
    else
    {
        if (parent->key2 == -1)
        {
            if (keys[1] < parent->key1)
            {
                parent->key2 = parent->key1;
                parent->key1 = keys[1];
                parent->pointer3 = parent->pointer2;
                parent->pointer2 = new_node;
            }
            else
            {
                parent->key2 = keys[1];
                parent->pointer3 = new_node;
            }
        }
        else
        {
            split_node(root, NULL, parent, keys[1], new_node);
        }
    }
}

void insert_key(Node **root, int key)
{
    if (!(*root))
    {
        *root = create_node(1);
        (*root)->key1 = key;
        return;
    }

    Node *current = *root;
    Node *parent = NULL;

    while (!current->isLeaf)
    {
        parent = current;
        if (key < current->key1)
        {
            current = current->pointer1;
        }
        else if (current->key2 == -1 || key < current->key2)
        {
            current = current->pointer2;
        }
        else
        {
            current = current->pointer3;
        }
    }

    if (current->key2 == -1)
    {
        if (key < current->key1)
        {
            current->key2 = current->key1;
            current->key1 = key;
        }
        else
        {
            current->key2 = key;
        }
    }
    else
    {
        split_node(root, parent, current, key, NULL);
    }
}

void remove_key(Node **root, Node *parent, Node *node, int key)
{
    if (!node)
        return;

    if (node->isLeaf)
    {
        if (node->key1 == key)
            node->key1 = node->key2;
        node->key2 = -1;
        if (node->key1 == -1 && parent)
        {
            if (parent->pointer1 == node)
                parent->pointer1 = NULL;
            else if (parent->pointer2 == node)
                parent->pointer2 = NULL;
            else
                parent->pointer3 = NULL;
            free(node);
        }
        return;
    }

    if (node->key1 == key || node->key2 == key)
    {
        Node *successor = node->pointer2;
        while (!successor->isLeaf)
            successor = successor->pointer1;

        int successor_key = successor->key1;
        remove_key(root, node, successor, successor_key);
        if (node->key1 == key)
            node->key1 = successor_key;
        else
            node->key2 = successor_key;
    }
    else if (key < node->key1)
    {
        remove_key(root, node, node->pointer1, key);
    }
    else if (node->key2 == -1 || key < node->key2)
    {
        remove_key(root, node, node->pointer2, key);
    }
    else
    {
        remove_key(root, node, node->pointer3, key);
    }
}
