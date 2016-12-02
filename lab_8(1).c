#include <stdio.h>
#include <stdlib.h>

struct tree
{
	int x;
	struct tree *left;
	struct tree *right;
};

struct list
{
	int x;
	struct list *next;
};

struct list *make_list ( FILE *input )
{
	int numeric = 0;
	char read[10];
	for (int i = 0; i < 10; i++)
	{
		read[i] = ' ';
	}
	for(int i = 0; i < 10; i++)
	{
		read[i] = fgetc(input);
		if (read[i] == ' ')
			break;
	}
	numeric = atoi(read);
	struct list *head = (struct list*)malloc(sizeof(struct list));
	head ->x = numeric;
	struct list *tail = head;
	while (1)
	{
		int i = 0;
		while (i < 10)
			{
				read[i] = fgetc(input);
				if (read[i] == ' ' || read[i] == EOF)
					break;
				i++;
			}
		if (read[0] == EOF)
			break;
		numeric = atoi(read);
		struct list *temp = (struct list*)malloc(sizeof(struct list));
		temp -> x = numeric;
		tail -> next = temp;
		tail = temp;
	}
	tail -> next = NULL;
	return (head);
}

void list_insert(struct list *head, struct list *temp)
{
	struct list *tail = head;
	tail = head;
	while(tail -> next != NULL)
	{
		if (temp -> x < tail -> next -> x)
		{
			temp -> next = tail -> next;
			tail -> next = temp;
			break;
		}
		tail = tail -> next;
	}
	if (temp ->x > tail ->x && tail ->next == NULL)
		tail ->next = temp;
}

struct list* list_sort( struct list *head)
{
	struct list *tail = head;
	while( tail -> next != NULL)
	{
		if ((tail -> x) > (tail -> next -> x))
		{
			struct list *temp = tail -> next;
			tail -> next = (tail -> next) -> next;
			if ((temp -> x) < (head -> x))
			{
				temp -> next = head;
				head = temp;
			}
			else
			{
				list_insert(head, temp);
				tail = head;
			}
		}
		else
			tail = tail -> next;
	}
	return(head);
}

int list_size(struct list *head)
{
	int c = 1;
	for (struct list *tail = head; tail -> next != NULL; tail = tail -> next)
		c++;
	return(c);
}

struct tree* make_root(struct list *head, int size)
{
	int mid = size/2;
	if (size == 0)
		return(NULL);
	struct tree *root = (struct tree*)malloc(sizeof(struct tree));
	int i = 0;
	struct list *temp;
	for (temp = head; i < mid ; i++ )
		temp = temp -> next;
	root -> x = temp -> x;
	printf("%d     ", root -> x);
		root -> left = make_root( head, mid);
	if (size % 2 != 0)
		root -> right = make_root( temp ->next, mid);
	else
		root -> right = make_root( temp ->next, mid - 1);
	return(root);
}

void tree_print(struct tree *root)
{
	printf("%d", root ->x );
	if (root -> left != NULL || root -> right != NULL)
		printf("%s","(" );
	if (root -> left != NULL)
	{
		tree_print(root -> left);
		if (root -> right != NULL)
			printf("%s","," );
		else printf("%s",")" );
	}
	if (root -> right != NULL)
	{
		tree_print(root -> right);
		printf("%s", ")");
	}
}

int main ( int argc , char *argv[])
{
	FILE *input = fopen ( argv[1], "r");
	struct list *head = make_list(input);
	fclose(input);
	head = list_sort(head);
	struct list *tail = head;
	for (tail = head; tail -> next != NULL; tail = tail ->next)
		printf("%d   ", tail ->x );
	printf("%d\n", tail ->x );
	int size = list_size(head);
	struct tree *root = make_root(head, size);
	printf("%s\n","tree:" );
	tree_print(root);
}
