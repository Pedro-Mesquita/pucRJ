#include <stdio.h>
#include <math.h>

float  foo  (double a, float b);
float  foo1 (float a, float b);
double foo2 (float a, float b);
double foo3 (double *a, int n);

int main(void) {
    {
        double a = 5.0;
        float  b = 3.0f;
        float r = foo(a, b);
        printf("foo(%.2f, %.2f) = %.4f  (esperado %.4f)\n",
               a, b, r, (float)((a + b) * (a - b)));
    }

    {
        float a = 0.0f;     
        float b = 2.0f;
        float r = foo1(a, b);
        printf("foo1(%.2f, %.2f) = %.4f  (esperado %.4f)\n",
               a, b, r, (float)(sin(a) + b));
    }

    {
        float a = 0.0f;     
        float b = 0.0f;     
        double r = foo2(a, b);
        printf("foo2(%.2f, %.2f) = %.4f  (esperado %.4f)\n",
               a, b, r, sin(a) + cos(b));
    }

    {
        double v[] = {0.0, 1.0, 2.0, 3.0};
        int n = 4;
        double ref = 0.0;
        for (int i = 0; i < n; i++)
            ref += sin(v[i]);
        double r = foo3(v, n);
        printf("foo3 = %.4f  (esperado %.4f)\n", r, ref);
    }

    return 0;
}
