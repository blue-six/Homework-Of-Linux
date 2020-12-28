#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *thread_1(void *arg);
void *thread_2(void *arg);

int main(int argc, char **argv)
{
    void *temp;
    pthread_t t1 = -1;
    if (argc != 2)
    {
        fprintf(stderr, "Usage:%s number 1-3\n", argv[0]);
        exit(1);
    }
    pthread_create(&t1, NULL, thread_1, &(argv[1][0]));
    pthread_join(t1, &temp);
    printf("main get %d\n", (int)temp);
    puts("main exit");
    return 0;
}

void *thread_1(void *arg)
{
    char c = *(char *)arg;
    printf("thread_1:I get char %c\n", c);
    switch (c)
    {
    case '1':
        puts("thread_1:use return");
        return (void *)8;
    case '2':
        puts("thread_1:use pthread_exit");
        pthread_exit((void *)5);
    default:
        puts("thread_1:use exit");
        exit(0);
    }
}
