#include <stdio.h>
#include <math.h>

double foo3(double *a, int n);

int main(void) {
    double a[] = {0.0, 0.5, 1.0, 1.5, 2.0};
    int n = 5;

    /* referência em C */
    double ref = 0.0;
    for (int i = 0; i < n; i++)
        ref += sin(a[i]);

    double got = foo3(a, n);

    printf("foo3 = %.10f\n", got);
    printf("ref  = %.10f\n", ref);
    printf("%s\n", (got == ref) ? "OK" : "DIFERENTE");
    return 0;
}
