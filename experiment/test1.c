#include <stdio.h>
#include <stdlib.h>

int main()
{
    char path[512] = {'\0'}, buf[1024] = {"\n"};
    FILE *sourse, *target;
    int n = 0;
    while (1)
    {
        puts("请输入要复制的文件的文件名:");
        scanf("%s", path);
        if ((sourse = fopen(path, "r")) != NULL)
            break;
        puts("文件路径错误,请重新输入!");
    }
    while (1)
    {
        puts("请输入新文件的文件名:");
        scanf("%s", path);
        if ((target = fopen(path, "w+")) != NULL)
            break;
        puts("文件路径错误,请重新输入!");
    }
    while (1)
    {
        if ((n = fgetc(sourse)) == -1)
            break;
        fputc(n, target);
    }
    puts("写入完成!");
}
