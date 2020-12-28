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
    int socket_fd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    signal(SIGUSR1, stop);
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        puts("Server: socket创建失败!");
        fprintf(stderr, "Server: Socket error:%s\n", strerror(errno));
        exit(1);
    }
    flg = socket_fd;
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
    while (flg)
    {
        socklen_t size = sizeof(client_addr);
        int new_fd;
        if ((new_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &size)) == -1)
        {
            if (errno != 22)
                fprintf(stderr, "Server: Accept error:%s\n", strerror(errno));
            close(socket_fd);
            exit(1);
        }
        fprintf(stderr, "Server: Server get connection from %s\n", inet_ntoa(client_addr.sin_addr));
        if (write(new_fd, "Do not connect me!", 18) == -1)
        {
            fprintf(stderr, "Server: Write error:%s\n", strerror(errno));
            close(socket_fd);
            exit(1);
        }
        close(new_fd);
    }
    close(socket_fd);
    exit(0);
}

int client()
{
    const char name[] = "Client";
    int socket_fd;
    char buffer[512];
    struct sockaddr_in server_addr;
    memset(buffer, '\0', 512);
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        puts("Client: socket创建失败!");
        fprintf(stderr, "Client: Socket error:%s\n", strerror(errno));
        exit(1);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORTNUMBER);
    if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        fprintf(stderr, "Client: Connect error:%s\n", strerror(errno));
        close(socket_fd);
        exit(1);
    }
    if (read(socket_fd, buffer, 512) == -1)
    {
        fprintf(stderr, "Client: Read error:%s\n", strerror(errno));
        close(socket_fd);
        exit(1);
    }
    printf("Client: I heave received:%s\n", buffer);
    close(socket_fd);
    return 0;
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
