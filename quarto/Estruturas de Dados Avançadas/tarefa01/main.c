#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define MIN 0
#define MAX 10000

float randomNumber(void);
void populateList(float *list);
void sortList(float *list);
int findNumber(float *firstList, float *secondList, float *thirdList);
void printList(float *list, int n);

int main()
{
    float firstList[MAX];
    float secondList[MAX];
    float thirdList[MAX];

    populateList(firstList);
    populateList(secondList);

    sortList(firstList);

    struct timeval start, end;
    long seconds, microseconds;
    double elapsed;

    gettimeofday(&start, NULL);

    int thirdListSize = findNumber(firstList, secondList, thirdList);

    gettimeofday(&end, NULL);

    seconds = end.tv_sec - start.tv_sec;
    microseconds = end.tv_usec - start.tv_usec;
    elapsed = seconds + microseconds * 1e-6;

    printList(thirdList, thirdListSize);
    printf("Tempe gasto: %.6f segundos\n", elapsed);

    return 0;
}

void populateList(float *list)
{
    for (int i = MIN; i < MAX; i++)
    {
        list[i] = randomNumber();
    }
}

void sortList(float *list)
{
    int i, j;
    for (i = 0; i < MAX; i++)
    {
        int exitFlag = 0;
        for (j = 0; j < MAX; j++)
        {
            if (list[j] > list[j + 1])
            {
                float current = list[j];
                list[j] = list[j + 1];
                list[j + 1] = current;
                exitFlag = 1;
            }
        }
        if (exitFlag == 0)
        {
            break;
        }
    }
}

int findNumber(float *firstList, float *secondList, float *thirdList)
{
    int quantity = 0;
    for (int i = MIN; i < MAX; i++)
    {
        int found = 0;
        for (int j = MIN; j < MAX; j++)
        {
            if (firstList[i] == secondList[j])
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            thirdList[quantity++] = firstList[i];
        }
    }
    return quantity;
}

float randomNumber(void)
{
    return (float)rand() / (float)(RAND_MAX)*MAX;
}

void printList(float *list, int n)
{
    for (int i = MIN; i < n; i++)
    {
        printf("%f\n", list[i]);
    }
    printf("Quantidade de números diferentes: %d\n", n);
}
