#include <stdio.h>

int main(void){

    char * s[] = {"an5a",NULL,"puc","9casa"};
    lenString("topzeira");
    return 1;
}

char * criaSenhar(char * senhas[] ){

}

int lenString(char * string[]){
    int counter = 0;
    for(int i =0;string[i] != '\0'; i++){
        i++;
        counter = i;
    }
    return counter;
}