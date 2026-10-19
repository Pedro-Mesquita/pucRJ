#include "common.h"

int main (void) {
    pid_t kernel = getppid();

    while (1) {
        sleep(TIMESLICE);
        if (kill(kernel, SIG_IRQ0) == -1) {
            perror("intercontroller: kill IRQ0");
            return 1;
        }
    }
    return 0;
}