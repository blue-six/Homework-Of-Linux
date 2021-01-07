#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int flg;
    char *buf;
    if ((flg = shmget(2354, 1024, IPC_CREAT | S_IRUSR | S_IWUSR)) == -1)
    {
        puts("创建共享内存失败!");
        exit(-1);
    }
    buf = shmat(flg, NULL, 0);
    memset(buf, '\0', 1024);
    puts("请输入要写入/追加的内容:");
    fgets(buf, 1024, stdin);
    shmdt(buf);
}
