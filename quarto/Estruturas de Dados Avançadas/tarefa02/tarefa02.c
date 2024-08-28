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

int main(void)
{
    Node *pHead = NULL;

    pHead = createLinkedList(pHead);
    printLinkedList(pHead);
    return 0;
}

Node *createLinkedList(Node *pHead)
{
    char *fileName = "db.txt";
    FILE *fp = fopen(fileName, "r");

    if (fp == NULL)
    {
        printf("Failed to open file\n");
        return NULL;
    }

    Node *pAux = NULL;
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
                fprintf(stderr, "deu ruim\n");
                fclose(fp);
                return NULL;
            }

            pAux->patient.status = status;
            pAux->patient.order = order;
            pAux->patient.color = color;
            pAux->next = pHead;
            pHead = pAux;
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
    Node *pAux;
    pAux = pHead;

    while (pAux)
    {
        printf("Patient: %c - %d - %c\n", pAux->patient.status, pAux->patient.order, pAux->patient.color);

        pAux = pAux->next;
    }
}

Node *sortLinkedList(Node *pHead)
{
    Node *pAux;
    pAux = pHead;
    Node *pRed;
    Node *pYellow;
    Node *pGreen;

    while (pAux)
    {
        if
    }
}