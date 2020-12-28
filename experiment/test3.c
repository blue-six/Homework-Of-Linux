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
    printf("main:g_flg=%d\n", g_flg);
    pthread_create(&t1, NULL, thread_1, &t2);
    pthread_create(&t2, NULL, thread_2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    puts("main exit");
    return 0;
}

void *thread_1(void *arg)
{
    puts("this is thread1");
    g_flg = 1;
    printf("thread_1:g_flg=%d\n", g_flg);
    while (t2 == -1)
        continue;
    pthread_join(t2, NULL);
    puts("thread1 exit");
    pthread_exit((void *)0);
}

void *thread_2(void *arg)
{
    puts("this is thread2");
    g_flg = 2;
    printf("thread_2:g_flg=%d\n", g_flg);
    sleep(2);
    puts("thread2 exit");
    pthread_exit((void *)0);
}
