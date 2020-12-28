#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_PORT 8888
#define MAX_BUF_SIZE 1024

int flg = -1, len;
pthread_mutex_t fd_lock, std_lock, *fpl = &fd_lock, *spl = &std_lock;
char r_buf[1024] = {0};
char w_buf[1024] = {0};

void *thread_R(void *arg);
void *thread_W(void *arg);
void stop(int sig)
{
    shutdown(flg, SHUT_RDWR);
    flg = 0;
}

void udpc_requ(int sockfd, const struct sockaddr_in *addr, int len)
{
    char buffer[MAX_BUF_SIZE];
    int n;
    /* 从键盘读入,写到服务端 */
    printf("Please input char:\n");
    fgets(buffer, MAX_BUF_SIZE, stdin);
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)addr, len);
    bzero(buffer, MAX_BUF_SIZE);
}

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in addr;
    pthread_t p_w, p_r;

    if (argc != 2)
    {
        fprintf(stderr, "Usage:%s server_ip\n", argv[0]);
        exit(1);
    }
    if (pthread_mutex_init(&fd_lock, NULL) != 0)
    {
        fprintf(stderr, "Server: Mutex error:%s\n", strerror(errno));
        exit(1);
    }
    if (pthread_mutex_init(&std_lock, NULL) != 0)
    {
        fprintf(stderr, "Server: Mutex error:%s\n", strerror(errno));
        exit(1);
    }
    /* 建立 sockfd描述符 */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        fprintf(stderr, "Socket Error:%s\n", strerror(errno));
        exit(1);
    }
    flg = sockfd;
    signal(SIGTSTP, stop);
    /* 填充服务端的资料 */
    bzero(&addr, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    if (inet_aton(argv[1], &addr.sin_addr) < 0) /*inet_aton函数用于把字符串型的IP地址转化成网络2进制数字*/
    {
        fprintf(stderr, "Ip error:%s\n", strerror(errno));
        exit(1);
    }
    len = sizeof(struct sockaddr_in);
    udpc_requ(sockfd, &addr, sizeof(struct sockaddr_in)); // 进行读写操作
    pthread_create(&p_r, NULL, thread_R, &addr);
    pthread_create(&p_w, NULL, thread_W, &addr);
    pthread_join(p_r, NULL);
    pthread_join(p_w, NULL);
    close(sockfd);
}

void *thread_R(void *arg)
{
    struct sockaddr *addr = (struct sockaddr *)arg;
    int n = 0, l = len, fd = flg;
    if (fd < 0)
    {
        fprintf(stderr, "Socket Error:%s\n", strerror(errno));
        exit(1);
    }
    while (flg)
    {
        pthread_mutex_lock(fpl);
        n = recvfrom(fd, r_buf, 1024, MSG_DONTWAIT, addr, &l);
        pthread_mutex_unlock(fpl);
        if (n > 0)
        {
            pthread_mutex_lock(spl);
            printf("I have read:");
            puts(r_buf);
            pthread_mutex_unlock(spl);
            memset(r_buf, '\0', 1024);
        }
        else if (n == -1 && errno != EAGAIN)
        {
            perror("recvfrom");
            flg = 0;
            pthread_exit((void *)1);
        }
        else
            sleep(1);
    }
}

void *thread_W(void *arg)
{
    struct sockaddr *addr = (struct sockaddr *)arg;
    int n = 0, l = len, fd = flg;
    while (flg)
    {
        pthread_mutex_lock(spl);
        puts("请输入要发送的数据:");
        fgets(w_buf, 1024, stdin);
        pthread_mutex_unlock(spl);
        pthread_mutex_lock(fpl);
        n = sendto(fd, w_buf, 1024, MSG_DONTWAIT, addr, l);
        pthread_mutex_unlock(fpl);
        if (n == -1)
        {
            perror("sendto");
            flg = 0;
            pthread_exit((void *)1);
        }
        memset(w_buf, '\0', 1024);
        sleep(1);
    }
}
