#include <stdio.h>
#include <stdlib.h>

struct patient
{
    int order;
    char status;
    char color;
};

typedef struct patient Patient;

struct node
{
    Patient patient;
    struct node *next;
};

typedef struct node Node;

Node *createLinkedList(Node *pHead);
void printLinkedList(Node *pHead);
Node *deleteNode(Node *pHead, unsigned int order);
void printIndividually(Node *node);
void updateFile(Node *pHead);
Node *addNode(Node *pHead, Node *node);
Node *mergeSortedLists(Node *list1, Node *list2);
Node *sortByOrder(Node *head);
Node *sortLinkedList(Node *pHead);

int main(void)
{
    Node *pHead = NULL;

    pHead = createLinkedList(pHead);
    pHead = sortLinkedList(pHead);
    printLinkedList(pHead);
    printf("\n");

    pHead = deleteNode(pHead, 5);
    printLinkedList(pHead);
    printf("\n");

    pHead = deleteNode(pHead, 4);
    printLinkedList(pHead);
    printf("\n");

    Node *p1 = (Node *)malloc(sizeof(Node));
    p1->patient.color = 'G';
    p1->patient.order = 9;
    p1->patient.status = 'E';
    pHead = addNode(pHead, p1);
    pHead = sortLinkedList(pHead);
    printLinkedList(pHead);

    Node *p2 = (Node *)malloc(sizeof(Node));
    p2->patient.color = 'Y';
    p2->patient.order = 10;
    p2->patient.status = 'E';
    pHead = addNode(pHead, p2);
    pHead = sortLinkedList(pHead);
    printLinkedList(pHead);

    Node *p3 = (Node *)malloc(sizeof(Node));
    p3->patient.color = 'R';
    p3->patient.order = 11;
    p3->patient.status = 'E';
    pHead = addNode(pHead, p3);
    pHead = sortLinkedList(pHead);
    printLinkedList(pHead);

    Node *p4 = (Node *)malloc(sizeof(Node));
    p4->patient.color = 'Y';
    p4->patient.order = 12;
    p4->patient.status = 'E';
    pHead = addNode(pHead, p4);
    pHead = sortLinkedList(pHead);
    printLinkedList(pHead);

    pHead = sortLinkedList(pHead);
    printLinkedList(pHead);
    printf("\n");

    pHead = deleteNode(pHead, 2);
    printLinkedList(pHead);
    printf("\n");

    pHead = deleteNode(pHead, 6);
    printLinkedList(pHead);
    printf("\n");

    pHead = deleteNode(pHead, 3);
    printLinkedList(pHead);
    printf("\n");

    return 0;
}

Node *createLinkedList(Node *pHead)
{
    char *fileName = "db.txt";
    FILE *fp = fopen(fileName, "r");

    if (fp == NULL)
    {
        printf("Deu ruim ao abrir o txt\n");
        return NULL;
    }

    Node *pAux = NULL;
    Node *tail = NULL;

    while (1)
    {
        char status;
        int order;
        char color;

        if (fscanf(fp, "%c%d%c\n", &status, &order, &color) == 3)
        {
            pAux = malloc(sizeof(Node));
            if (pAux == NULL)
            {
                fprintf(stderr, "Deu ruim\n");
                fclose(fp);
                return NULL;
            }

            pAux->patient.status = status;
            pAux->patient.order = order;
            pAux->patient.color = color;
            pAux->next = NULL;

            if (pHead == NULL)
            {
                pHead = pAux;
                tail = pAux;
            }
            else
            {
                tail->next = pAux;
                tail = pAux;
            }
        }
        else
        {
            break;
        }
    }

    fclose(fp);
    return pHead;
}

void printLinkedList(Node *pHead)
{
    int countR = 0, countG = 0, countY = 0;

    Node *current = pHead;

    while (current != NULL)
    {
        printf("%d - %c - %c\n", current->patient.order, current->patient.status, current->patient.color);

        if (current->patient.color == 'R')
        {
            countR++;
        }
        else if (current->patient.color == 'G')
        {
            countG++;
        }
        else if (current->patient.color == 'Y')
        {
            countY++;
        }

        current = current->next;
    }

    printf("\nQuantidade por cor:\n");
    printf("Vermelha: %d, Verde: %d, Amarela: %d\n", countR, countG, countY);
}

Node *mergeSortedLists(Node *list1, Node *list2)
{
    if (list1 == NULL)
        return list2;
    if (list2 == NULL)
        return list1;

    Node *result = NULL;

    if (list1->patient.order <= list2->patient.order)
    {
        result = list1;
        result->next = mergeSortedLists(list1->next, list2);
    }
    else
    {
        result = list2;
        result->next = mergeSortedLists(list1, list2->next);
    }

    return result;
}

Node *sortByOrder(Node *head)
{
    if (head == NULL || head->next == NULL)
    {
        return head;
    }

    Node *slow = head;
    Node *fast = head->next;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    Node *mid = slow->next;
    slow->next = NULL;

    Node *left = sortByOrder(head);
    Node *right = sortByOrder(mid);

    return mergeSortedLists(left, right);
}

Node *sortLinkedList(Node *pHead)
{
    Node *pAux = pHead;
    Node *pRed = NULL, *pRedTail = NULL;
    Node *pYellow = NULL, *pYellowTail = NULL;
    Node *pGreen = NULL, *pGreenTail = NULL;

    while (pAux != NULL)
    {
        Node *nextNode = pAux->next;
        pAux->next = NULL;

        if (pAux->patient.color == 'G')
        {
            if (pGreen == NULL)
            {
                pGreen = pAux;
                pGreenTail = pAux;
            }
            else
            {
                pGreenTail->next = pAux;
                pGreenTail = pAux;
            }
        }
        else if (pAux->patient.color == 'R')
        {
            if (pRed == NULL)
            {
                pRed = pAux;
                pRedTail = pAux;
            }
            else
            {
                pRedTail->next = pAux;
            }
        }
        else if (pAux->patient.color == 'Y')
        {
            if (pYellow == NULL)
            {
                pYellow = pAux;
                pYellowTail = pAux;
            }
            else
            {
                pYellowTail->next = pAux;
                pYellowTail = pAux;
            }
        }

        pAux = nextNode;
    }

    pHead = mergeSortedLists(pRed, pYellow);
    pHead = mergeSortedLists(pHead, pGreen);

    return pHead;
}

Node *deleteNode(Node *pHead, unsigned int order)
{
    Node *current = pHead;
    Node *previous = NULL;

    while (current != NULL && current->patient.order != order)
    {
        previous = current;
        current = current->next;
    }

    if (current == NULL)
    {
        return pHead;
    }

    if (previous == NULL)
    {
        pHead = current->next;
    }
    else
    {
        previous->next = current->next;
    }

    free(current);
    return pHead;
}

Node *addNode(Node *pHead, Node *node)
{
    Node *current = pHead;
    Node *previous = NULL;

    while (current != NULL && current->patient.order < node->patient.order)
    {
        previous = current;
        current = current->next;
    }

    if (previous == NULL)
    {
        node->next = pHead;
        pHead = node;
    }
    else
    {
        previous->next = node;
        node->next = current;
    }

    return pHead;
}

void updateFile(Node *pHead)
{
    FILE *fp = fopen("db.txt", "w");

    if (fp == NULL)
    {
        printf("Deu ruim ao escrever\n");
        return;
    }

    Node *current = pHead;
    while (current != NULL)
    {
        fprintf(fp, "%c%d%c\n", current->patient.status, current->patient.order, current->patient.color);
        current = current->next;
    }

    fclose(fp);
}