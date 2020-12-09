#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    pid_t p;
    int count = 0;
    p = fork();
    if (p == 0)
    {
        pid_t id = getpid();
        printf("fork(%d):child process is running!\n", id);
        printf("fork(%d):child->count=%d\n", id, ++count);
        exit(0);
    }
    else if (p > 0)
    {
        pid_t id = getpid();
        printf("fork(%d):parent process is running!\n", id);
        printf("fork(%d):parent->count=%d\n", id, ++count);
    }
    else
    {
        printf("创建子进程失败！\n");
    }
    count = 0;
    p = vfork();
    if (p == 0)
    {
        pid_t id = getpid();
        printf("vfork(%d):child process is running!\n", id);
        printf("vfork(%d):child->count=%d\n", id, ++count);
        exit(0);
    }
    else if (p > 0)
    {
        pid_t id = getpid();
        printf("vfork(%d):parent process is running!\n", id);
        printf("vfork(%d):parent->count=%d\n", id, ++count);
    }
    else
    {
        printf("创建子进程失败！\n");
    }
    return 0;
}
