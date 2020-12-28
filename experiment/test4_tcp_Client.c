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

#define PORTNUMBER 9520

int flg = 1;
pthread_mutex_t fd_lock, std_lock, *fpl = &fd_lock, *spl = &std_lock;
// pid_t pid = getpid();
char r_buf[1024] = {0};
char w_buf[1024] = {0};

void *thread_R(void *arg);
void *thread_W(void *arg);
void stop(int sig)
{
    flg = 0;
}

int main(int argc, char **argv)
{
    pthread_t p_w, p_r;
    const char name[] = "Client";
    int socket_fd;
    char buffer[512];
    struct sockaddr_in server_addr;
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
    signal(SIGTSTP, stop);
    memset(buffer, '\0', 512);
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        puts("Client: socket创建失败!");
        fprintf(stderr, "Client: Socket error:%s\n", strerror(errno));
        exit(1);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORTNUMBER);
    if (inet_aton(argv[1], &addr.sin_addr) < 0) /*inet_aton函数用于把字符串型的IP地址转化成网络2进制数字*/
    {
        fprintf(stderr, "Ip error:%s\n", strerror(errno));
        exit(1);
    }
    if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        fprintf(stderr, "Client: Connect error:%s\n", strerror(errno));
        close(socket_fd);
        exit(1);
    }
    // if (read(socket_fd, buffer, 512) == -1)
    // {
    //     fprintf(stderr, "Client: Read error:%s\n", strerror(errno));
    //     close(socket_fd);
    //     exit(1);
    // }
    printf("Client: I heave received:%s\n", buffer);
    pthread_create(&p_r, NULL, thread_R, &socket_fd);
    pthread_create(&p_w, NULL, thread_W, &socket_fd);
    pthread_join(p_r, NULL);
    pthread_join(p_w, NULL);
    close(socket_fd);
    return 0;
}

void *thread_R(void *arg)
{
    int fd = *(int *)arg;
    int n = 0;
    while (flg)
    {
        pthread_mutex_lock(fpl);
        n = recv(fd, r_buf, 1024, MSG_DONTWAIT);
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
            perror("recv");
            pthread_exit((void *)1);
        }
        else
        {
            sleep(1);
        }
    }
}

void *thread_W(void *arg)
{
    int fd = *(int *)arg;
    int n = 0;
    while (flg)
    {
        // n = send(fd, &w_buf, 1024, MSG_DONTWAIT);
        pthread_mutex_lock(spl);
        puts("请输入要发送的数据:");
        fgets(w_buf, 1024, stdin);
        pthread_mutex_unlock(spl);
        pthread_mutex_lock(fpl);
        n = send(fd, w_buf, 1024, MSG_DONTWAIT);
        pthread_mutex_unlock(fpl);
        if (n == -1)
        {
            perror("send");
            pthread_exit((void *)1);
        }
        memset(w_buf, '\0', 1024);
        sleep(1);
    }
}
