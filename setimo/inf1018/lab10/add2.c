#include <stdio.h>

struct X {
  int val;
  struct X *next;
};

int add2 (struct X *x) {
  if (x == NULL) return 0;
  else return x->val + add2(x->next);
}