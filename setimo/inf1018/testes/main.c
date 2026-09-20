#include <stdio.h>

int main() {
    int vetor[2] = {1, 2};

    printf("%p\n", vetor);
    printf("%d\n", *(vetor + 1));

    return 0;
}