#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 1201
#define CPF_LENGTH 12

typedef struct
{
    char cpf[CPF_LENGTH];
    int occupied;
} HashEntry;

unsigned int hash(char *cpf);
unsigned int hash2(char *cpf);
void insertCPF(char *cpf, int *insertionCount);
void loadCPFs(const char *filename);
void displayStatistics(int insertionCount);

HashEntry hashTable[TABLE_SIZE];
unsigned int collisions = 0;
unsigned int empty_positions = TABLE_SIZE;

int main()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        hashTable[i].occupied = 0;
    }

    loadCPFs("cpfs.txt");

    return 0;
}

void loadCPFs(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Erro ao abrir arquivo\n");
        return;
    }

    char cpf[CPF_LENGTH + 10];
    int insertionCount = 0;

    while (fgets(cpf, sizeof(cpf), file))
    {
        cpf[strcspn(cpf, "\r\n")] = '\0';

        insertCPF(cpf, &insertionCount);
    }

    fclose(file);
}

unsigned int hash(char *cpf)
{
    unsigned int hashValue = 0;
    for (int i = 0; i < CPF_LENGTH - 1; i++)
    {
        hashValue = (hashValue * 37 + cpf[i]) % TABLE_SIZE;
    }
    return hashValue;
}

unsigned int hash2(char *cpf)
{
    unsigned int hashValue = 0;
    for (int i = 0; i < CPF_LENGTH - 1; i++)
    {
        hashValue = (hashValue * 19 + cpf[i]) % (TABLE_SIZE - 1);
    }
    return hashValue + 1;
}

void insertCPF(char *cpf, int *insertionCount)
{
    unsigned int index = hash(cpf);
    unsigned int step = hash2(cpf);

    while (hashTable[index].occupied)
    {
        if (strcmp(hashTable[index].cpf, cpf) == 0)
        {
            return;
        }
        collisions++;
        index = (index + step) % TABLE_SIZE;
    }

    strncpy(hashTable[index].cpf, cpf, CPF_LENGTH - 1);
    hashTable[index].cpf[CPF_LENGTH - 1] = '\0';
    hashTable[index].occupied = 1;
    empty_positions--;
    (*insertionCount)++;

    if (*insertionCount % 100 == 0)
    {
        displayStatistics(*insertionCount);
    }
}

void displayStatistics(int insertionCount)
{
    printf("Depois de %d inserções:\n", insertionCount);
    printf("Colisões: %u\n", collisions);
    printf("Posições vazias: %u\n\n", empty_positions);
}