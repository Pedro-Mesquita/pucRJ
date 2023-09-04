#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
    int dia_entrada;
    int dia_saida;
    printf("Digite o dia de entrada: ");
    scanf("%d", &dia_entrada);
    printf("Digite o dia da saida: ");
    scanf("%d", &dia_saida);
    printf("Voceh ficou %d com o livro\n", dia_saida - dia_entrada);
    return 0;
}

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
	int qtdDias;
	printf("Escreava a quantidade dias: ");
	scanf("%d", &qtdDias);
	int semanas = qtdDias / 7;
	int dias = qtdDias % 7;

	printf("%d semanas e %d dia(s)", semanas, dias);
	return 0;
}


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
    float primeiraNota, segundaNota;
    printf("Insira a primeira nota: ");
    scanf("%f", &primeiraNota); 
    printf("Insira a segunda nota: ");
    scanf("%f", &segundaNota); 
    float res = (3 * primeiraNota + 7 * segundaNota) / 10;
    printf("A sua media é %.2f", res); 
    return 0;
}


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
    float num1, num2, result;
    char operator;

    printf("Entre com o primeiro numero: ");
    scanf("%f", &num1);

    printf("Entre com o segundo numero: ");
    scanf("%f", &num2);

    printf("Entre com a operação (+, -, *, /): ");
    scanf(" %c", &operator); 

    switch (operator) {
    case '+':
        result = num1 + num2;
        break;
    case '-':
        result = num1 - num2;
        break;
    case '*':
        result = num1 * num2;
        break;
    case '/':
        result = num1 / num2;
        break;
    default:
        printf("Operacao invalida!\n");
        return 1; 
    }

    printf("Resultado: %.2f\n", result);

    return 0;
}
