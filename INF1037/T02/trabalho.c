
#define ALPHABETO "abcdefghijklmnopqrstuvwxyz"
#define TRUE 1
#define FALSE 0

int main(void){

    char key[] = "KEY";
    char mensagem[] = "Plaintext English";

    cipher(mensagem, key, 0, TRUE);

    return 0;
}

void cipher(char* text, char * key, int pos, int encipher){

}