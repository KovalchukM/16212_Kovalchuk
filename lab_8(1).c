#include <stdio.h>
#include <stdlib.h>
/* 
1) создасть список
2) сортировка
3) создать дерево
*/
struct list
{
	int x;
	struct list *next;
};

void make_list ( char argv[1] )
{
	FILE *input = fopen (argv[1] , "r");
	int numeric = 0;
	char read = fgetc(input);
	while (read != " ")
	{
		numeric = numeric * 10 + atoi(read);
		read = fgetc(input);
	}
	struct list *head = (struct list*)malloc(sizeof(struct list));
	head ->x = numeric;
	while

	fclose (input);
	return(head);
}

int main ( int argc , char *argv[])
{
	struct list *head = make_list( argv[1]);
}
