#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void stop(int sig)
{
    if (sig == SIGTSTP)
        printf("\nI get the SIGSTOP !\n");
}

int main()
{
    printf("Waiting for signal SIGSTOP !\n");
    signal(SIGTSTP, stop);
    pause();
    exit(0);
}
