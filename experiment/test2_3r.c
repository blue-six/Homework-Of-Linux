#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main()
{
    char buf[512];
    struct sembuf bf_add = {0, 1, SEM_UNDO};
    struct sembuf bf_sub = {0, -1, SEM_UNDO};
    union semun sem_union = {1, NULL, NULL};
    int sem = semget(ftok(getcwd(buf, 512), 0), 1, IPC_CREAT | 0666);
    // printf("%d\n", sem);
    if (semctl(sem, 0, SETVAL, sem_union) == -1)
    {
        perror("sem init");
        exit(1);
    }
    pid_t p;
    if ((p = fork()) == 0)
    {
        // printf("%d\n", sem);
        sleep(1);
        printf("A:当前进程pid为:%d\n", getpid());
        puts("A:等待进程B退出中");
        semop(sem, &bf_sub, 1);
        puts("A进程退出!");
        semctl(sem, 0, IPC_RMID);
        exit(0);
    }
    else if (p > 0)
    {
        semop(sem, &bf_sub, 1);
        puts("B:进程开始运行");
        sleep(6);
        puts("B:进程运行结束");
        semop(sem, &bf_add, 1);
        exit(0);
    }
    else
    {
        puts("创建子进程失败!");
    }
}
