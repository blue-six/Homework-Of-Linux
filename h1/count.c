#include <stdio.h>
#include <stdlib.h>

int *input(); //输入函数

int main()
{
    int *score = input();
    int a = 0, b = 0, c = 0;
    for (int i = 0; i < 20; i++)
    {
        if (score[i] >= 80)
            a++;
        else if (score[i] >= 60)
            b++;
        else
            c++;
    }
    printf("A : %d\nB : %d\nC : %d\n", a, b, c);
    free(score);
    return 0;
}

int *input()
{
    int n = 20, c;
    int *a = (int *)malloc(sizeof(int) * 20);
    printf("请输入学生成绩:\n");
    while (n-- > 0)
    {
        while (1)
        {
            c = scanf("%d", &(a[n]));
            if (c != 0 && a[n] >= 0 && a[n] <= 100)
                break;
            else
            {
                if (c == 0)
                    scanf("%*c");
                printf("请输入正确的成绩!\n");
            }
        }
    }
    return a;
}
