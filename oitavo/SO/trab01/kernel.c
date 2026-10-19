#include "common.h"
static volatile sig_atomic_t irq0_flag = 0;

static void on_irq0(int sig)
{
    (void)sig;
    irq0_flag = 1;
}

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    struct sigaction sa = {0};
    sa.sa_handler = on_irq0;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIG_IRQ0, &sa, NULL) == -1)
    {
        perror("kernel: sigaction IRQ0");
        return 1;
    }

    pid_t ic = fork();

    if (ic == -1) {
        perror("kernel: fork");
        return 1;
    }

    if (ic == 0) {
        execl("./intercontroller", "intercontroller", (char *)NULL);
        perror("kernel: exec intercontroller");
        _exit(1);
    }

    printf("kernel pid=%d | intercontroller pid=%d\n", getpid(), ic);

    while (1) {
        pause();
        if (irq0_flag) {
            irq0_flag = 0;
            printf("IRQ0\n");
        }
    }
}
