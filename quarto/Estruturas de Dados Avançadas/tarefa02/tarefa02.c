#include <stdio.h>

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

int createLinkedList(Node *pHead);

int main(void)
{
    Node *pHead;

    createLinkedList(pHead);
    return 0;
}

int createLinkedList(Node *pHead)
{
    int counter = 0;
    char *fileName = "db.txt";

    FILE *fp = fopen(fileName, "r");

    if (fp == NULL)
    {
        printf("Deu ruim, amigão\n");
        return 1;
    }

    char ch;
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch != '\n' && ch != '\r' && ch != 32)
        {
            printf("%c \n", ch);
            counter++;
        }
    }

    fclose(fp);
}