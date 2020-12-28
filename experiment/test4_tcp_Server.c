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

char r_buf[1024] = {0};
char w_buf[1024] = {0};

void *thread_R(void *arg);
void *thread_W(void *arg);
void stop(int sig)
{
    shutdown(flg, SHUT_RDWR);
    flg = 0;
}

int main()
{
    const char name[] = "Server";
    int new_fd;
    int socket_fd;
    pthread_t p_w, p_r;
    socklen_t size = sizeof(client_addr);
    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;
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
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        puts("Server: socket创建失败!");
        fprintf(stderr, "Server: Socket error:%s\n", strerror(errno));
        exit(1);
    }
    flg = socket_fd;
    signal(SIGTSTP, stop);
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORTNUMBER);
    if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        fprintf(stderr, "Server: Bind error:%s\n", strerror(errno));
        close(socket_fd);
        exit(1);
    }
    if (listen(socket_fd, 10) == -1)
    {
        fprintf(stderr, "Server: Listen error:%s\n", strerror(errno));
        close(socket_fd);
        exit(1);
    }
    if ((new_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &size)) == -1)
    {
        if (errno != 22)
        {
            fprintf(stderr, "Server: Accept error:%s\n", strerror(errno));
            close(socket_fd);
            exit(1);
        }
        else
            close(socket_fd);
    }
    fprintf(stderr, "Server: Server get connection from %s\n", inet_ntoa(client_addr.sin_addr));
    // if (write(new_fd, "Do not connect me!", 18) == -1)
    // {
    //     fprintf(stderr, "Server: Write error:%s\n", strerror(errno));
    //     close(socket_fd);
    //     exit(1);
    // }
    pthread_create(&p_r, NULL, thread_R, &new_fd);
    pthread_create(&p_w, NULL, thread_W, &new_fd);
    pthread_join(p_r, NULL);
    pthread_join(p_w, NULL);
    close(new_fd);
    close(socket_fd);
    exit(0);
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
