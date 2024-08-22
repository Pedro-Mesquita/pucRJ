#include <stdio.h>

int is_little(void)
{
    unsigned int i = 0;
    char *c = (char *)&i;
    if (*c)
    {
        printf("%d\n", *c);
        printf("little endia\n");
    }
    else
    {
        printf("%d\n", *c);
        printf("big endia\n");
    }
    return 0;
}

int main()
{
    is_little();
    return 0;
}
