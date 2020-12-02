#include <stdio.h>
#include <stdlib.h>
#include <string.h>
main()
{
	char my_string[] = "hello there";
	my_print(my_string);
	my_print2(my_string);
}
my_print(char *string)
{
	printf("The string is %s\n", string);
}
my_print2(char *string)
{
	char *string2;
	int size, i;
	size = strlen(string);
	string2 = (char *)malloc(size + 1);
	for (i = 0; i < size; i++)
		string2[size - i - 1] = string[i];
	string2[size] = '\0';
	printf("The string printed backward is %s\n", string2);
}
