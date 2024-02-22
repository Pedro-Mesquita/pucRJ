#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>



typedef unsigned int uint; // definir em headers
typedef unsigned short int ushort;

struct struct_ponto {
	float x; // CAMPO x
	float y; // CAMPO y
};
typedef struct struct_ponto Ponto; // criei um tipo Ponto

struct struct_retangulo {
	Ponto ponto1;
	Ponto ponto2;
	char* nome;
	uint cor;
};
typedef struct struct_retangulo Retangulo;


void _exibePonto(Ponto ponto);
void exibePonto(Ponto* p);
void exibeVariosPontos(Ponto* vPonto, int qtd);

int main(void) {
	Ponto p1;
	Ponto p2;
	Ponto vPonto[10];
	Ponto* pPonto;
	Ponto delta;

	printf("Entre com os valores de x e y de p1: ");
	scanf("%f%f", &p1.x, &p1.y);
	printf("Entre com os valores de x e y de p2: ");
	scanf("%f%f", &p2.x, &p2.y);
	printf("P1: (%f - %f)\n", p1.x, p1.y);
	printf("P2: (%f - %f)\n", p2.x, p2.y);


	delta.x = (p2.x - p1.x) / 11;
	delta.y = (p2.y - p1.y) / 11;
	
	for (int i = 0; i < 10; i++) {
		vPonto[i].x = p1.x + delta.x * (i + 1);
		vPonto[i].y = p1.y + delta.y * (i + 1);
		//exibePonto(&vPonto[i]);
	}

	Retangulo r;

	r.cor = 0x00FF00;
	r.nome = malloc(100);
	strcpy(r.nome, "area retangular");
	r.ponto1.x = 2;
	r.ponto1.y = 3;
	r.ponto2.x = 4;
	r.ponto2.y = 5;
	
	return 0;
}


/*
* Não use a função assim
*/

void _exibePonto(Ponto ponto) {
	printf("(%f - %f)\n", ponto.x, ponto.y);
}

/*
* Use a função assim
*/

void exibePonto(Ponto *p) {
	printf("(%f - %f)\n", (*p).x, p->y );
}


void exibeVariosPontos(Ponto * vPonto, int qtd) {
	for (int i = 0; i < qtd; i++) {
		exibePonto(&vPonto[i]);
		//exibePonto(&(vPonto + i));
	}
}

