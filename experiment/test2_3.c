#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int flg = 1;

void stop(int sig)
{
    flg = 0;
}

int main()
{
    pid_t p;
    if ((p = fork()) == 0)
    {
        printf("A:当前进程pid为:%d\n", getpid());
        signal(SIGUSR1, stop);
        while (flg)
        {
            puts("A:等待进程B退出中.");
            sleep(1);
        }
        puts("A进程退出!");
        exit(0);
    }
    else if (p > 0)
    {
        puts("B:进程开始运行");
        sleep(6);
        puts("B:进程运行结束");
        kill(p, SIGUSR1);
        exit(0);
    }
    else
    {
        puts("创建子进程失败!");
    }
}
