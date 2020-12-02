#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void t2();
void t3();

int main(int argc, char *argv[])
{
    int flg;
    if (argc < 2)
    {
        printf("请输入第几问---2，3\n");
        scanf("%d", &flg);
    }
    else
        flg = atoi(argv[1]);
    if (flg != 2 && flg != 3)
    {
        puts("请输入2，3中的一个");
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
        }
    exit(0);
}

void t2()
{
    char path[200];
    char string[1024];
    FILE *fp;
    printf("请输入文件路径:\n");
    scanf("%s", path);
    if ((fp = fopen(path, "r")) == NULL)
    {
        puts("文件不存在！");
        exit(1);
    }
    fgets(string, 1024, fp);
    while (!feof(fp))
    {
        printf("%s", string);
        fgets(string, 1024, fp);
    }
}

void t3()
{
    int i = 0;
    char path[200];
    char string[1024];
    FILE *fp;
    printf("请输入要写入的文件路径:\n");
    scanf("%s", path);
    if ((fp = fopen(path, "w+")) == NULL)
    {
        puts("文件不存在！");
        exit(1);
    }
    puts("请输入要写入的内容");
    scanf("%s", string);
    for (i = 0; i < strlen(string); i++)
        fputc(string[i], fp);
}
