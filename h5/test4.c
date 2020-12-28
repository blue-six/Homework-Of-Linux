#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *thread_1(void *arg);
void *clean(void *arg);

int main()
{
    pthread_t t1 = -1;
    pthread_create(&t1, NULL, thread_1, NULL);
    pthread_join(t1, NULL);
    puts("main exit");
    return 0;
}

void *thread_1(void *arg)
{
    char *c = (char *)malloc(1000);
    puts("this is thread1");
    pthread_cleanup_push(clean, c);
    puts("start clean");
    pthread_cleanup_pop(1);
    puts("thread1 exit");
    pthread_exit((void *)0);
}

void *clean(void *arg)
{
    puts("clean is run");
    free(arg);
    puts("clean end");
}
