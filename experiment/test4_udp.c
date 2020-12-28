#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORTNUMBER 9520

int flg = 0;

void stop(int sig)
{
    shutdown(flg, SHUT_RDWR);
}

int server()
{
    const char name[] = "Server";
    int sockfd;
    struct sockaddr_in addr;
    struct sockaddr_in claddr;
    int addrlen, n;
    char msg[1024];

    /* 服务器端开始建立socket描述符 */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        fprintf(stderr, "Socket Error:%s\n", strerror(errno));
        exit(1);
    }

    /* 服务器端填充 sockaddr结构 */
    bzero(&addr, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORTNUMBER);

    /* 捆绑sockfd描述符 */
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0)
    {
        fprintf(stderr, "Bind Error:%s\n", strerror(errno));
        exit(1);
    }

    while (1)
    {                            /* 从网络上读,并写到网络上 */
        bzero(msg, sizeof(msg)); // 初始化,清零
        addrlen = sizeof(struct sockaddr);
        n = recvfrom(sockfd, msg, 1024, 0, (struct sockaddr *)&claddr, &addrlen); // 从客户端接收消息
        msg[n] = 0;
        /* 显示服务端已经收到了信息 */
        fprintf(stdout, "Server have received %s", msg); // 显示消息
    }
    close(sockfd);
}

int client()
{
    const char name[] = "Client";
    int sockfd;
    struct sockaddr_in addr;
    char buffer[1024];
    int n;

    /* 建立 sockfd描述符 */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        fprintf(stderr, "Socket Error:%s\n", strerror(errno));
        exit(1);
    }

    /* 填充服务端的资料 */
    bzero(&addr, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORTNUMBER);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    while (1)
    { /* 从键盘读入,写到服务端 */
        printf("Please input char:\n");
        fgets(buffer, 1024, stdin);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
        bzero(buffer, 1024);
    }
    close(sockfd);
}

int main()
{
    pid_t p;
    if ((p = fork()) == 0)
    {
        server();
    }
    else if (p > 0)
    {
        sleep(1);
        client();
        sleep(1);
        kill(p, SIGUSR1);
    }
    else
    {
        fprintf(stderr, "Fork error:%s\n", strerror(errno));
        exit(1);
    }
    exit(0);
}
