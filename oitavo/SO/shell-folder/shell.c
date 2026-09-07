#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    char line[1024];
    char *args[64];

    while (1) {
        printf("shell top%% ");
        fflush(stdout);
        if (fgets(line, sizeof(line), stdin) == NULL) break;

        int n = 0;
        for (char *tok = strtok(line, " \t\n"); tok; tok = strtok(NULL, " \t\n"))
            args[n++] = tok;
        args[n] = NULL;

        if (fork() == 0) {
            execvp(args[0], args);
            exit(1);
        }
        wait(NULL);
    }
    return 0;
}
