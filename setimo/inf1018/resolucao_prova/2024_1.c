#include <stdio.h>
    void dump (void *p, int n) {
    unsigned char *p1 = (unsigned char *) p;
    while (n--) {
    printf("%p - %02x\n", p1, *p1);
    p1++;
    }
}

struct S {
int a;
char c;
char *p;
};

int main(void) {
    struct S s;
    s.a = -16;
    s.c = ’d’;
    s.p = &s.c;
    dump(&s, sizeof(struct S));
    return 0;
}