#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/time.h>

#define QTDFORKS 3

int main()
{
    struct timeval T;
    pid_t filhos[QTDFORKS];

    for (int i = 0; i < QTDFORKS; i++)
    {
        pid_t pid = fork();
        
        if (pid < 0)
        {
            perror("Erro ao criar o fork");
            exit(1);
        }
        else if (pid == 0)
        {
            while (1)
            {
                printf("Processo P%d (PID %d) executando...\n", i + 1, getpid());
            }
        }
        else
        {
            filhos[i] = pid;
            kill(pid, SIGSTOP); 
        }
    }

    gettimeofday(&T, NULL);
    int seg_atual = T.tv_sec % 60;
    
    if (seg_atual != 0)
    {
       int tempo_de_espera = 60 - seg_atual;
        while(tempo_de_espera > 0){
            printf("Sincronizando... Aguardando %d segundos...\n", tempo_de_espera);
            sleep(1); 
            tempo_de_espera--;
        }
    }

    printf("Iniciando...\n");

    while (1)
    {
        gettimeofday(&T, NULL);
        seg_atual = T.tv_sec % 60;
        
        printf("--- Tempo: %d segundos ---\n", seg_atual);

        if (seg_atual >= 5 && seg_atual < 25)
        {
            kill(filhos[0], SIGCONT);
            kill(filhos[1], SIGSTOP);
            kill(filhos[2], SIGSTOP);
        }
        else if (seg_atual >= 45 && seg_atual < 60)
        {
            kill(filhos[1], SIGCONT);
            kill(filhos[0], SIGSTOP);
            kill(filhos[2], SIGSTOP);
        }
        else
        {
            kill(filhos[2], SIGCONT);
            kill(filhos[0], SIGSTOP);
            kill(filhos[1], SIGSTOP);
        }
    }
    
    return 0;
}