#include <stdio.h>
#include <stdlib.h>

int main(void){

    FILE * input;
    FILE * output;
    char string[127];
    char ** matriz;



    input = fopen("./livros.txt", "r");
    if(input == NULL){
        printf("deu ruim\n");
        fclose(input);
        exit(1);
    }

    output = fopen(".livrosSaida.txt", "w");
    if(output == NULL){
        printf("deu ruim\n");
        fclose(output);
        exit(1);
    }

    while(fgets(string, sizeof(string), input) != NULL){
        for(int i =0; string[i]!= "/0"; i++){

        }
    }

    return 1;
}

