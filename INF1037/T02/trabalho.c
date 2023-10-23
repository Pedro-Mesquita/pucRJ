#include <stdio.h>
#include <ctype.h>

#define ALFABETO "abcdefghijklmnopqrstuvwxyz"
#define TRUE 1
#define FALSE 0

void cipher(char *text, char *key, int pos, int encipher);
int arr_length(char arr[]);
int findIndex(char *text, int size, char target);

int main(void)
{

    char key[] = "KEY";
    char mensagem[] = "Plaintext English";

    cipher(mensagem, key, 0, TRUE);

    return 0;
}

void cipher(char *text, char *key, int pos, int encipher)
{
    int lenMenssage = arr_length(text);
    int lenKey = arr_length(key);
    char encrypted[lenMenssage];

    int index = findIndex(ALFABETO, lenMenssage, text[pos]);
    printf("index: %d\n", index);

    if (encipher)
    {
        text[pos];
    }
    else
    {
    }
}

int arr_length(char arr[])
{
    int i;
    int count = 0;
    for (i = 0; arr[i] != '\0'; i++)
    {
        if (arr[i] != ' ')
        {
            count++;
        }
    }
    return count;
}

int findIndex(char *text, int size, char target)
{
    for (int i = 0; i < size; i++)
    {
        if (text[i] == tolower(target))
        {
            return i;
        }
    }
    return -1;
}