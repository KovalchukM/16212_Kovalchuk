#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int nymeric_verification(int from, char *nymeric)
{
	char library[] ="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int l = strlen(nymeric);
	int count = 0;
	int i = 0;
	if (nymeric[0] == '-')
	{
		count++;
		i++;
	}
	while( i < l )
	{
		for (int k = 0; k < from ;k++)
		{
			if (library[k] == nymeric[i])
				count++;
		}
		i++;
	}
	if (count == l)
		return(0);
	return(1);
}

int read_from()
{
	int from;
	printf("%s\n","insert from" );
	scanf("%d",&from);
	while (from > 36 || from < 2)
	{
		printf("%s\n","invalid redix" );
		printf("%s\n","insert from" );
		scanf("%d",&from);
	}
	printf("\n");
	return(from);
}

char *read_nymeric(int from)
{
	printf("%s\n","insert nymeric" );
	char read [100] = {0};
	scanf("%s",&read);
	printf("\n");
	char *nymeric = (char*)calloc(strlen(read), sizeof(char));
	for (int i = 0; i < strlen(read); i++)
		nymeric[i] = read[i];
	int check = 0;
	printf("aaa  %s\n", nymeric );
	check = nymeric_verification(from, nymeric);
	if (check == 1)
	{
		printf("%s\n","invalid nymeric" );
		exit(0);
	}
	return(nymeric);
}

int read_to()
{
	int to;
	printf("%s\n", "insert to" );
	scanf("%d",&to);
	while (to > 36 || to < 2)
	{
		printf("%s","invalid selected redix" );
		printf("%s\n", "insert to" );
		scanf("%d",&to);
	}
	printf("\n");
	return(to);
}

void string_reverse(int *string, int lenght , int c)
{
	if( lenght > c)
	{
		int t = string[c];
		string[c] = string[lenght];
		string[lenght] = t;
		string_reverse( string, --lenght , ++c);
	}
}

int to_new_redix(int *new_nymeric_int ,int dex, int to)
{
	int count = 0;
	while (dex >= to)
	{
		if (count > strlen((char*) new_nymeric_int))
			new_nymeric_int = realloc(new_nymeric_int , sizeof(int) * strlen((char*)new_nymeric_int) * 2);
		new_nymeric_int[count] = dex % to;
		dex = dex / to;
		count++;
	}
	if (count > strlen((char*) new_nymeric_int))
		new_nymeric_int = realloc(new_nymeric_int , sizeof(int) * strlen((char*)new_nymeric_int) * 2);
	new_nymeric_int[count] = dex;
	int c = 0;
	string_reverse(new_nymeric_int, count , c);
	return(count);
}

int to_dex(int *nymeric_int, int from , int size)
{
	int dex = 0;
	int exponent = 0;
	for (int i = size - 1; i > -1; i--)
	{
		int elem = nymeric_int[i];
		for(int k = 0; k < exponent; k++)
			elem = elem * from;
		dex = dex + elem;
		exponent++;
	}
	return(dex);
}

void translation(char *new_nymeric , int from, int to, char *nymeric)
{
	char library[] ="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int nymeric_int [strlen(nymeric)];
	for (int i = 0; i < strlen(nymeric); i++)
	{
		for (int k = 0; k < from ; k++)
		{
			if (library[k] == nymeric[i])
			{
				nymeric_int[i] = k;
			}
		}
	}
	int dex = to_dex(nymeric_int, from , strlen(nymeric));
	int *new_nymeric_int = (int*)calloc((strlen(new_nymeric)), sizeof(int));
	int lenght = to_new_redix(new_nymeric_int , dex, to);
	if (lenght > strlen(new_nymeric))
	{
		new_nymeric =(char*)realloc(new_nymeric, strlen((char*)new_nymeric_int) * sizeof(char));
	}
	for (int i = 0; i < lenght+1; i++)
	{
		new_nymeric[i] = library[new_nymeric_int[i]];
	}
	printf("%s\n", new_nymeric );
	free(new_nymeric_int);
}

int main()
{
	int from = read_from();
	char *nymeric = read_nymeric(from);
	int to = read_to();
	int flag = 0;
	printf("1)%s\n",nymeric );
	if (nymeric[0] == '-')
	{
		flag = 1;
		for (int i = 0; i < strlen(nymeric) - 1; i++)
		{
			nymeric[i] = nymeric[i+1];
		}
		nymeric = (char*)realloc(nymeric , sizeof(char) * (strlen(nymeric) - 1));
	}
	char *new_nymeric = (char*)calloc(20, sizeof(char));
	printf("2)%s\n", nymeric );
	translation(new_nymeric , from , to , nymeric);
	printf("%s\n","AAAA" );
	if (flag == 1)
	{
		new_nymeric =(char*)realloc(new_nymeric, sizeof(char) *(strlen(new_nymeric) + 1));
		for (int i = strlen(new_nymeric) - 1; i > 0 ; i--)
		{
			new_nymeric[i] = new_nymeric[i-1];
		}
		new_nymeric[0] = '-';
	}
	printf("nymeric %s", nymeric );
	printf(" of the base system %d", from );
	printf(" translated to base system %d", to );
	printf(" a %s\n",new_nymeric );
	free (new_nymeric);
	free (nymeric);
}
