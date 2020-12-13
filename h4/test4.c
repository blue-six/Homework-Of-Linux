#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int pwcr[2], prcw[2]; //pwcr父写子读,prcw父读子写;
    pid_t p;
    if (pipe(pwcr) || pipe(prcw))
    {
        printf("管道创建失败!\n");
        exit(-1);
    }
    if ((p = fork()) == 0)
    {
        char buf[1024];
        close(pwcr[1]), close(prcw[0]);
        sprintf(buf, "child's pid is %u", getpid());
        write(prcw[1], buf, strlen(buf) + 1);
        read(pwcr[0], buf, 1024);
        printf("child get [ %s ] from parent!\n", buf);
    }
    else if (p > 0)
    {
        char buf[1024];
        close(pwcr[0]), close(prcw[1]);
        sprintf(buf, "parent's pid is %u", getpid());
        write(pwcr[1], buf, strlen(buf) + 1);
        read(prcw[0], buf, 1024);
        printf("parent get [ %s ] from child!\n", buf);
    }
    else
    {
        printf("创建子进程失败!\n");
        exit(0);
    }
}
