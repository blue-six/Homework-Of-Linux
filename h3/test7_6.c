#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    pid_t p;
    printf("child use exit(0):\n");
    if ((p = fork()) == 0)
    {
        printf("child process");
        exit(0);
    }
    else if (p > 0)
    {
        wait(NULL);
        printf("\nparent process\n");
    }
    else
        printf("创建子进程失败！\n");
    printf("child use _exit(0):\n");
    if ((p = fork()) == 0)
    {
        printf("child process");
        _exit(0);
    }
    else if (p > 0)
    {
        wait(NULL);
        printf("\nparent process\n");
    }
    else
        printf("创建子进程失败！\n");
    exit(0);
}
