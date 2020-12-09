#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    pid_t p;
    p = fork();
    if (p == 0)
    {
        printf("child process is running!\n");
    }
    else if (p > 0)
    {
        wait(NULL);
        printf("parent process is running!\n");
    }
    else
    {
        printf("创建子进程失败！\n");
    }
    return 0;
}
