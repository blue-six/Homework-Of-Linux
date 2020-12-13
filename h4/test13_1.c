#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct msg__
{
    long mtype;
    char mtext[512];
} M;

int main()
{
    char path[1024];
    key_t key;
    int msgid;
    M data;
    data.mtype = 1;
    if ((key = ftok(getcwd(path, 1024), 0)) == -1)
    {
        puts("获取id失败!");
        exit(-1);
    }
    if ((msgid = msgget(key, IPC_CREAT | 0666)) == -1)
    {
        puts("创建消息队列失败!");
        fprintf(stderr, "%d\n", errno);
        exit(-1);
    }
    puts("请输入要发送的数据:");
    fgets(data.mtext, 512, stdin);
    if (msgsnd(msgid, &data, 512, 0) == -1)
    {
        puts("数据发送失败!");
        exit(-1);
    }
    sleep(3);
    exit(0);
}
