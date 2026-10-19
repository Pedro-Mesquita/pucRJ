#ifndef COMMON_H
#define COMMON_H

#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIG_IRQ0    SIGUSR1
#define SIG_IRQ1    SIGUSR2
#define SIG_SYS_R   (SIGRTMIN)
#define SIG_SYS_W   (SIGRTMIN + 1)
#define SIG_IO_REQ  (SIGRTMIN + 2)

#define TIMESLICE 1
#define IO_TIME   3
#define MAX_APPS  6

#endif