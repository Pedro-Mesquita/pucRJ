#include <stdio.h>
#include "temp1.h"
#include "temp2.h"

int a = 1024;

int main (void) {
  foo();
  printf("%d\n", a);
  return 0;
}