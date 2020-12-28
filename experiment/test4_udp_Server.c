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
#define MAX_MSG_SIZE 1024

int flg = 1, len;
pthread_mutex_t fd_lock, std_lock, *fpl = &fd_lock, *spl = &std_lock;
char r_buf[1024] = {0};
char w_buf[1024] = {0};

void *thread_R(void *arg);
void *thread_W(void *arg);
void stop(int sig)
{
    flg = 0;
}

struct sockaddr_in *udps_respon(int sockfd)
{
    struct sockaddr_in *addr;
    int addrlen, n;
    char msg[MAX_MSG_SIZE];
    signal(SIGTSTP, stop);
    /* 从网络上读,并写到网络上 */
    bzero(msg, sizeof(msg)); // 初始化,清零
    addrlen = sizeof(struct sockaddr_in);
    addr = (struct sockaddr_in *)malloc(addrlen);
    memset(addr, 0, addrlen);
    n = recvfrom(sockfd, msg, MAX_MSG_SIZE, 0, (struct sockaddr *)addr, &addrlen); // 从客户端接收消息
    msg[n] = 0;
    /* 显示服务端已经收到了信息 */
    fprintf(stdout, "Server have received %s", msg); // 显示消息
    return addr;
}

int main(void)
{
    int sockfd;
    struct sockaddr_in addr, *new_addr;
    pthread_t p_w, p_r;
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

    /* 服务器端开始建立socket描述符 */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        fprintf(stderr, "Socket Error:%s\n", strerror(errno));
        exit(1);
    }
    signal(SIGTSTP, stop);
    /* 服务器端填充 sockaddr结构 */
    bzero(&addr, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(SERVER_PORT);
    flg = sockfd;
    /* 捆绑sockfd描述符 */
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0)
    {
        fprintf(stderr, "Bind Error:%s\n", strerror(errno));
        exit(1);
    }
    new_addr = udps_respon(sockfd); // 进行读写操作
    pthread_create(&p_r, NULL, thread_R, new_addr);
    pthread_create(&p_w, NULL, thread_W, new_addr);
    pthread_join(p_r, NULL);
    pthread_join(p_w, NULL);
    free(new_addr);
    close(sockfd);
}

void *thread_R(void *arg)
{
    struct sockaddr *addr = (struct sockaddr *)arg;
    int n = 0, l = len, fd = flg;
    while (flg)
    {
        pthread_mutex_lock(fpl);
        n = recvfrom(fd, r_buf, 1024, MSG_DONTWAIT, addr, &l);
        pthread_mutex_unlock(fpl);
        if (n > 0)
        {
            pthread_mutex_lock(spl);
            printf("I have received:");
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
    struct sockaddr_in *addr = (struct sockaddr_in *)arg;
    int n = 0, l = len, fd = flg;
    while (flg)
    {
        // n = send(fd, &w_buf, 1024, MSG_DONTWAIT);
        pthread_mutex_lock(spl);
        puts("请输入要发送的数据:");
        fgets(w_buf, 1024, stdin);
        pthread_mutex_unlock(spl);
        pthread_mutex_lock(fpl);
        n = sendto(fd, w_buf, 1024, MSG_DONTWAIT, (struct sockaddr *)addr, sizeof(*addr));
        pthread_mutex_unlock(fpl);
        if (n == -1)
        {
            perror("sendto");
            printf("%d", errno);
            flg = 0;
            pthread_exit((void *)1);
        }
        memset(w_buf, '\0', 1024);
        sleep(1);
    }
}
