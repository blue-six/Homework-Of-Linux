#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *thread_1(void *arg);
void *thread_2(void *arg);

int main()
{
    int n = 9527;
    char c = 'q';
    pthread_t t1 = -1, t2 = -1;
    pthread_create(&t1, NULL, thread_1, &c);
    pthread_create(&t2, NULL, thread_2, &n);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    puts("main exit");
    return 0;
}

void *thread_1(void *arg)
{
    char c = *(char *)arg;
    printf("thread_1:I get char %c\n", c);
    pthread_exit((void *)0);
}

void *thread_2(void *arg)
{
    int n = *(int *)arg;
    printf("thread_2:I get int %d\n", n);
    pthread_exit((void *)0);
}
