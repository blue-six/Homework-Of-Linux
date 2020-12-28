#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *thread_1(void *arg);
void *thread_2(void *arg);

int g_flg = 0;
pthread_t t1 = -1, t2 = -1;

int main()
{
    puts("main is runing!");
    pthread_create(&t1, NULL, thread_1, &t2);
    puts("main wait for child thread!");
    pthread_join(t1, NULL);
    puts("main run again!");
    puts("main exit!");
    return 0;
}

void *thread_1(void *arg)
{
    int n = 3;
    while (n-- > 0)
    {
        puts("child thread is running!");
        sleep(1);
    }
    puts("child thread exit!");
}
