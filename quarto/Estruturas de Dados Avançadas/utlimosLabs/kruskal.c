#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int u, v, peso;
} Aresta;

typedef struct
{
    int *pai, *rank;
    int n;
} Conjunto;

Conjunto *criarConjunto(int n)
{
    Conjunto *conjunto = (Conjunto *)malloc(sizeof(Conjunto));
    conjunto->n = n;
    conjunto->pai = (int *)malloc(n * sizeof(int));
    conjunto->rank = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        conjunto->pai[i] = i;
        conjunto->rank[i] = 0;
    }

    return conjunto;
}

int encontrar(Conjunto *conjunto, int x)
{
    if (conjunto->pai[x] != x)
    {
        conjunto->pai[x] = encontrar(conjunto, conjunto->pai[x]);
    }
    return conjunto->pai[x];
}

void unir(Conjunto *conjunto, int x, int y)
{
    int raizX = encontrar(conjunto, x);
    int raizY = encontrar(conjunto, y);

    if (raizX != raizY)
    {
        if (conjunto->rank[raizX] < conjunto->rank[raizY])
        {
            conjunto->pai[raizX] = raizY;
        }
        else if (conjunto->rank[raizX] > conjunto->rank[raizY])
        {
            conjunto->pai[raizY] = raizX;
        }
        else
        {
            conjunto->pai[raizY] = raizX;
            conjunto->rank[raizX]++;
        }
    }
}

int compararArestas(const void *a, const void *b)
{
    return ((Aresta *)a)->peso - ((Aresta *)b)->peso;
}

void kruskal(Aresta *arestas, int V, int E)
{
    qsort(arestas, E, sizeof(Aresta), compararArestas);

    Conjunto *conjunto = criarConjunto(V);
    Aresta *resultado = (Aresta *)malloc((V - 1) * sizeof(Aresta));
    int contador = 0, i = 0;

    while (contador < V - 1 && i < E)
    {
        Aresta aresta = arestas[i++];
        int u = aresta.u;
        int v = aresta.v;

        if (encontrar(conjunto, u) != encontrar(conjunto, v))
        {
            resultado[contador++] = aresta;
            unir(conjunto, u, v);
        }
    }

    printf("Arestas da Árvore Geradora Mínima:\n");
    int custoTotal = 0;
    for (i = 0; i < contador; i++)
    {
        printf("%d -- %d == %d\n", resultado[i].u, resultado[i].v, resultado[i].peso);
        custoTotal += resultado[i].peso;
    }
    printf("Custo total da MST: %d\n", custoTotal);

    free(conjunto->pai);
    free(conjunto->rank);
    free(conjunto);
    free(resultado);
}

int main()
{
    int V = 4;
    int E = 5;

    Aresta arestas[] = {
        {0, 1, 10},
        {0, 2, 6},
        {0, 3, 5},
        {1, 3, 15},
        {2, 3, 4}};

    kruskal(arestas, V, E);

    return 0;
}
