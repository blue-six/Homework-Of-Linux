#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

void t1();
void t2();

int main(int argc, char *argv[])
{
    int flg;
    if (argc < 2)
    {
        printf("请输入第几问---1，2\n");
        scanf("%d", &flg);
    }
    else
        flg = atoi(argv[1]);
    if (flg != 1 && flg != 2)
    {
        puts("请输入1，2中的一个");
        exit(1);
    }
    else
        switch (flg)
        {
        case 1:
            t1();
            break;
        case 2:
            t2();
            break;
        }
    exit(0);
}

void t1()
{
    time_t lt;
    lt = time(NULL);
    printf("The Local time is: %s", ctime(&lt));
}

void t2()
{
    struct timeval start, end;
    int sum, i;
    gettimeofday(&start, NULL);
    for (i = 0; i < 90000; i++)
        sum += i;
    gettimeofday(&end, NULL);
    printf("耗费时间:%ld us\n", end.tv_usec - start.tv_usec);
}
