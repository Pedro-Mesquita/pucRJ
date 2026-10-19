#ifndef COMMON_H
#define COMMON_H

#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SIG_IRQ0    SIGUSR1
#define SIG_IRQ1    SIGUSR2
#define SIG_SYS_R   (SIGRTMIN)
#define SIG_SYS_W   (SIGRTMIN + 1)
#define SIG_IO_REQ  (SIGRTMIN + 2)

#define TIMESLICE 1
#define IO_TIME   3
#define MAX_APPS  6

/* Um slot por app na memória compartilhada: é onde mora o PC "vivo" do processo. */
typedef struct {
    volatile int pc;
    volatile int bloqueado;     /* 1 = app pediu E/S e espera o kernel liberar */
} AppSlot;

/* Operação da syscall em D1. O valor de cada constante é a letra usada no
   roteiro ("3R,6W") e no log; OP_NENHUMA (0) = sem syscall pendente. */
typedef enum {
    OP_NENHUMA = 0,
    OP_READ    = 'R',
    OP_WRITE   = 'W'
} Operacao;

#define MAX_SYSCALLS 32

/* Lê um roteiro "3R,7W" (syscall R no PC 3 e W no PC 7) para pcs[]/ops[].
   Devolve quantas syscalls leu, ou -1 se o roteiro for inválido. Roteiro vazio = 0. */
static inline int ler_roteiro(const char *s, int max, int pcs[], Operacao ops[])
{
    int n = 0;
    while (*s) {
        char *fim;
        long pc = strtol(s, &fim, 10);
        if (fim == s || pc < 1 || pc > max || (*fim != OP_READ && *fim != OP_WRITE) || n == MAX_SYSCALLS)
            return -1;
        pcs[n] = pc;
        ops[n] = (Operacao)*fim;
        n++;
        s = fim + 1;
        if (*s == ',')
            s++;
        else if (*s)
            return -1;
    }
    return n;
}

#endif