#include <stdio.h>
#include <stdlib.h>

int bubble_sort(int *array, const int len)
{
    int i, j, temp, flg = 1;
    for (i = 0; i < len - 1 && flg; i++)
    {
        flg = 0;
        for (j = 0; j < len - i - 1; j++)
        {
            if (array[j] > array[j + 1])
            {
                flg == 0 && flg++;
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
    return 1;
}

int *input(int *len)
{
    int n = -1, c;
    int *a = (int *)malloc(sizeof(int) * 1024);
    char b = 0;
    printf("请输入数组:以#结尾，n<1024\n");
    while (++n >= 0)
    {
        while (1)
        {
            c = scanf("%d", &(a[n]));
            if (c != 0)
                break;
            else
            {
                scanf("%c", &b);
                if (b == '#')
                    break;
                printf("请输入数字!\n");
            }
        }
        if (b == '#')
            break;
    }
    *len = n;
    return a;
}

int out(int *array, int len)
{
    printf("排序好的数组为:");
    for (int i = 0; i < len; i++)
    {
        printf(" %3d", array[i]);
    }
    printf("\n");
    return 1;
}

int main()
{
    int len;
    int *array = input(&len);
    bubble_sort(array, len);
    out(array, len);
    free(array);
    return 0;
}
