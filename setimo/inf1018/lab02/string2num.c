#include <ctype.h>
#include <stdio.h>

int string2num (char *s, int base) {
  int a = 0;
  int dig;

  for (; *s; s++) {
    if (isdigit(*s))
      dig = *s - '0';
    else
      dig = tolower(*s) - 'a' + 10;

    a = a*base + dig;
  }

  return a;
}
int main (void) {
  printf("==> %d\n", string2num("1234", 2));
  printf("==> %d\n", string2num("1234", 16) + 1);
  printf("==> %d\n", string2num("1234", 10) + string2num("1", 2));
  return 0;
}