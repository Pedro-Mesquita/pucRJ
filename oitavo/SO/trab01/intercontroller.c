#include "common.h"
#include <errno.h>
#include <time.h>

static double agora(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec / 1e9;
}

int main(void)
{
    pid_t kernel = getppid();

    /* O aviso de E/S fica bloqueado: em vez de um handler, ele é "recolhido"
       pelo sigtimedwait. Bloqueado, ele espera na fila de sinais do processo
       e não se perde, mesmo chegando enquanto o InterController manda um IRQ. */
    sigset_t io;
    sigemptyset(&io);
    sigaddset(&io, SIG_IO_REQ);
    sigprocmask(SIG_BLOCK, &io, NULL);

    /* Horários absolutos: um aviso de E/S que chega no meio da espera não
       adianta o próximo IRQ0, então o time slice fica regular. */
    double prox_irq0 = agora() + TIMESLICE;
    int pendentes = 0;      /* pedidos esperando o D1 ficar livre */
    int ocupado = 0;        /* D1 está atendendo um pedido? */
    double t_fim = 0;       /* quando o pedido em atendimento termina */

    while (1) {
        /* Espera até o próximo evento: o IRQ0 ou o fim da E/S em andamento. */
        double prazo = prox_irq0;
        if (ocupado && t_fim < prazo)
            prazo = t_fim;
        double falta = prazo - agora();
        if (falta < 0)
            falta = 0;
        struct timespec ts = { (time_t)falta, (long)((falta - (time_t)falta) * 1e9) };

        if (sigtimedwait(&io, NULL, &ts) == SIG_IO_REQ)
            pendentes++;
        else if (errno != EAGAIN && errno != EINTR) {
            perror("intercontroller: sigtimedwait");
            return 1;
        }

        double t = agora();

        /* E/S em série: o D1 só termina o pedido atual e só então pega o próximo. */
        if (ocupado && t >= t_fim) {
            ocupado = 0;
            if (kill(kernel, SIG_IRQ1) == -1) {
                perror("intercontroller: kill IRQ1");
                return 1;
            }
        }
        if (!ocupado && pendentes > 0) {
            pendentes--;
            ocupado = 1;
            t_fim = t + IO_TIME;
        }

        /* IRQ1 sai antes do IRQ0 quando os dois vencem juntos. */
        if (t >= prox_irq0) {
            prox_irq0 += TIMESLICE;
            if (kill(kernel, SIG_IRQ0) == -1) {
                perror("intercontroller: kill IRQ0");
                return 1;
            }
        }
    }
    return 0;
}
