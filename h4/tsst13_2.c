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
    char path[1024] key_t key;
    int msgid;
    M data;
    data.mtype = 1;
    if ((key = ftok(getcwd(path, 1024), 0)) == -1)
    {
        puts("获取id失败!");
        exit(-1);
    }
    if ((msgid = msgget(key, IPC_CREAT | S_IRUSR | S_IWUSR)) == -1)
    {
        puts("创建消息队列失败!");
        exit(-1);
    }
    if (msgrcv(msgid, &data, 512, 1, 0) == -1)
    {
        puts("数据接收失败!");
        exit(-1);
    }
    puts("接收到的消息为:");
    puts(data.mtext);
    exit(0);
}
