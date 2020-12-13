#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void start_or_end(int sig);

int fd;

int main()
{
    int flg, p_fd[2];
    char n_s[8];
    pid_t p;
    if ((flg = shmget(IPC_PRIVATE, 1024, S_IRUSR | S_IWUSR)) == -1)
    {
        puts("创建共享内存失败!");
        exit(-1);
    }
    if (pipe(p_fd))
    {
        puts("管道创建失败!");
        exit(-1);
    }
    if ((p = fork()) == 0)
    {
        int m = 0, n, num;
        char *buf, mod[2], path[200];
        memset(path, '\0', 200);
        signal(SIGUSR1, start_or_end);
        signal(SIGQUIT, start_or_end);
        close(p_fd[1]);
        buf = shmat(flg, NULL, 0);
        pause();
        if (read(p_fd[0], path, 200) == -1)
        {
            puts("读取文件名失败!");
            exit(-1);
        }
        kill(getppid(), SIGUSR1);
        pause();
        if (read(p_fd[0], mod, 2) == -1)
        {
            puts("读取文件打开模式失败!");
            exit(-1);
        }
        // putchar(mod[0]), putchar(mod[1]);
        m = mod[0] == 'c' ? (O_CREAT | (mod[1] == 'a' ? O_APPEND | O_WRONLY : O_RDWR)) : (mod[0] == 'a' ? O_APPEND | O_WRONLY : O_RDWR);
        // printf("%d   %d,%s\n", m, O_APPEND, path);
        if ((fd = open(path, m, S_IRUSR | S_IWUSR)) == -1)
        {
            puts("文件打开失败!");
            exit(-1);
        }
        m = mod[0] == 'c' ? mod[1] == 'r' ? 0 : 1 : mod[0] == 'r' ? 0 : 1;
        kill(getppid(), SIGUSR1);
        while (1)
        {
            pause();
            if (m)
            {
                n = write(fd, buf, strlen(buf));
            }
            else
            {
                read(p_fd[0], n_s, 8);
                sscanf(n_s, "%d", &num);
                read(fd, buf, num);
                kill(getppid(), SIGUSR1);
            }
        }
    }
    else if (p > 0)
    {
        int n;
        char *buf, path[200], f, n_s[8], c;
        signal(SIGUSR1, start_or_end);
        close(p_fd[0]);
        buf = shmat(flg, NULL, 0);
        memset(buf, '\0', 1024);
        memset(path, '\0', 200);
        puts("请输入文件路径:");
        while (1)
        {
            scanf("%s", path), setbuf(stdin, NULL);
            if (access(path, F_OK) == -1)
            {
                puts("文件不存在!\n是否创建该文件:[y/n]");
                if (getchar() == 'n')
                    puts("请重新输入正确的文件路径:");
                else
                {
                    write(p_fd[1], path, strlen(path));
                    kill(p, SIGUSR1);
                    pause();
                    f = 'c';
                    write(p_fd[1], &f, 1);
                    getchar();
                    break;
                }
            }
            else
            {
                write(p_fd[1], path, strlen(path));
                kill(p, SIGUSR1);
                pause();
                break;
            }
        }
        puts("请输入打开模式:[读取(r)/写入(w)/追加(a)]");
        f = getchar();
        while ((c = getchar()) != EOF && c != '\n')
            ;
        write(p_fd[1], &f, 1);
        kill(p, SIGUSR1);
        pause();
        while (1)
        {
            if (f == 'r')
            {
                puts("请输入读入的字符数:[0<n<1024]");
                scanf("%d", &n);
                setbuf(stdin, NULL);
                sprintf(n_s, "%d", n);
                write(p_fd[1], n_s, strlen(n_s));
                kill(p, SIGUSR1);
                pause();
                puts(buf);
            }
            else if (f == 'w' || f == 'a')
            {
                puts("请输入要写入/追加的内容:");
                fgets(buf, 1024, stdin);
                kill(p, SIGUSR1);
            }
            else
            {
                puts("打开模式错误!");
                kill(p, SIGQUIT);
                exit(-1);
            }
            puts("是否继续?[y/n]");
            if (getchar() == 'n')
                break;
            getchar();
        }
        kill(p, SIGQUIT);
        shmdt(buf);
        shmctl(flg, IPC_RMID, NULL);
    }
}

void start_or_end(int sig)
{
    if (sig == SIGQUIT)
    {
        close(fd);
        exit(0);
    }
    else
        return;
}
