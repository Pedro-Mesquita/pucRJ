#include <stdio.h>

void foo (int a[], int n) ;

int main (void) {
    int n = 4;
    int array[4] = {3, 2, 0, 4};
    foo(array, 4);

    for (int i = 0; i<n; i++) {
        printf("%d\n", array[i]);
    }
    return 0;
}