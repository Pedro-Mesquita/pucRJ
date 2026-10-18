#include <stdio.h>
    #include <stdlib.h>
    #include <signal.h>
    #include <sys/sem.h>
    #include <pthread.h>
    #include <semaphore.h>

    #define TAM 10000
    #define NUM_THREADS 100

    int vetor[TAM];

    void inicializaVetor(){
        int i = 0;
        while(i < TAM){
            vetor[i] = 10;
            i++;
        }
    }

    void* rotina_da_tarefa(void *arg){
        int id_tarefa = *(int *)arg;

        for (int i = 0; i < TAM; i++) {
        if(id_tarefa % 2 == 0){
            vetor[i] = vetor[i] + id_tarefa;
        }
        else{
            vetor[i] = vetor[i] - id_tarefa;
        }}
        pthread_exit(NULL);
    }

    int main(){
        pthread_t threads[NUM_THREADS];
        int ids_tarefas[NUM_THREADS];
        inicializaVetor();

        for(int i = 0; i < NUM_THREADS; i++) {
            ids_tarefas[i] = i + 1;
            pthread_create(&threads[i], NULL, rotina_da_tarefa, &ids_tarefas[i]);
        }
        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }

        int erros = 0;
        for(int i = 0; i < TAM; i++) {
            if(vetor[i] != 60) {
              // VERIFICAÇÃO DE ERROS DE CONCORRÊNCIA
             // Gabarito 60: Valor inicial 10 + 50 (saldo de 50 pares de tarefas onde -ímpar +par resulta em +1).
            // Qualquer valor diferente de 60 indica perda de cálculos devido à condição de corrida na memória.
            printf("vetor[%d] = %d (esperado 60)\n", i, vetor[i]);
            erros++;
            }
        }

        printf("Verificacao concluida!\n");
        printf("Total de posicoes com erro de concorrencia: %d de %d\n", erros, TAM);

        return 0;
    }