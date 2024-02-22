#include <stdio.h>
#include <math.h>
#include <ctype.h>


int main(void){
    char alpha = 'e';
    char upper = toupper(alpha);
    int lower = isupper(upper);
    printf("%c - %d\n", upper, lower);

    return 0;
}