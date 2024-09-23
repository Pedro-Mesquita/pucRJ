#include <stdio.h>
#include <stdlib.h>

void openFile(char pathName[]);
void dump(void *p, int n);

int main(void)
{
    openFile("./utf32_peq.txt");
    return 0;
}

void openFile(char pathName[])
{
    FILE *file = fopen(pathName, "r");
    if (file == NULL)
    {
        printf("Failed to open the file\n");
        exit(1);
    }

    unsigned char BOM[4];
    size_t bytes = fread(BOM, 1, 4, file);
    if (bytes != 4)
    {
        printf("Error reading BOM\n");
        fclose(file);
        exit(1);
    }

    dump(BOM, 4);

    fclose(file);
}

void dump(void *p, int n)
{
    unsigned char *p1 = p;
    while (n--)
    {
        printf("%02X ", *p1);
        p1++;
    }
    printf("\n");
}
