#include <stdio.h>

int main(void)
{
  int number = 1;
  char *bytePointer = (char *)&number;
  int i = 0;
  while (i < sizeof(int))
  {
    printf("Byte #%d - %d\n", i, bytePointer[i]);
    i++;
  }

  if (*bytePointer)
  {
    printf("little %d\n", *bytePointer);
  }
  else
  {
    printf("big %d\n", *bytePointer);
  }
}
