#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void t2(); //第二问
void t3(); //第三问
void t6(); //第六问
void t7(); //第七问

int main(int argc, char *argv[])
{
    int flg;
    if (argc < 2)
    {
        printf("请输入第几问---2，3，6，7\n");
        scanf("%d", &flg);
    }
    else
        flg = atoi(argv[1]);
    if (flg != 2 && flg != 3 && flg != 6 && flg != 7)
    {
        puts("请输入2，3，6，7中的一个");
        exit(1);
    }
    else
        switch (flg)
        {
        case 2:
            t2();
            break;
        case 3:
            t3();
            break;
        case 6:
            t6();
            break;
        default:
            t7();
            break;
        }
    exit(0);
}

void t2()
{
    int fd;
    char text[30] = {0};
    fd = open("/home/wpy_test/homework/h2/new.txt", O_RDONLY);
    read(fd, text, 21);
    close(fd);
    printf("%s\n", text);
}

void t3()
{
    int fd;
    char text[] = "Hello,welcome to Beijing!";
    fd = open("/home/wpy_test/homework/h2/new1.txt", O_RDWR);
    printf("%d\n", fd);
    printf("%ld\n", write(fd, text, sizeof(text) - 1));
    close(fd);
}

void t6()
{
    char path[200];
    printf("请输入文件路径：\n");
    scanf("%s", path);
    if (access(path, F_OK) == 0)
        printf("文件存在\n");
    else
        printf("文件不存在\n");
}

void t7()
{
    int fd, len = 0;
    char text[50];
    fd = open("/home/wpy_test/homework/h2/log.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    printf("请输入字符：\n");
    scanf("%s", text);
    printf("%ld\n", write(fd, text, strlen(text)));
    close(fd);
}
