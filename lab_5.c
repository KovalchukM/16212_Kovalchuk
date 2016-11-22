#include <stdio.h>
#include <stdlib.h>

#define SIZE 64

int Dex_to_bin(int nymber, FILE *out)
{
	int count = 0;
	char bin[] = "000000";
	while (nymber > 0)
	{
		if (nymber % 2 == 1)
			bin[5 - count] = '1';
		nymber = nymber / 2;
		count++;
	}
	fprintf(out, "%s", bin );
}

int bin_to_dex(char *elem)
{
	int nymber = 0;
	int n = 0;
	int c = 1;
	for(int i = 5; i >= 0; i = i - 1)
	{
		if (elem[i] == '1')
		{
			for (int k = 0; k < n ; k++)
				c = c * 2;
			printf("%d\n", c);
			nymber = nymber + c;
		}
		n++;
		c = 1;
	}
	printf("%s\n","AAAAAA" );
	return(nymber);
}

int to_Base(char *Library)
{
	char elem;
	int nymber;
	FILE *base64;
	FILE *out;
	out = fopen ( "out.base64.txt", "w+");
	base64 = fopen ( "tobase.txt", "r");
	if (base64 == NULL)
		return (1);
	while (1 > 0)
	{
		elem = fgetc(base64);
		if ( elem == EOF)
		{
			break;
		}
		for (int i = 0; i < SIZE; i++)
			if (Library[i] == elem)
			{
				nymber = i;
				break;
			}
		Dex_to_bin(nymber, out);
	}
	fclose (base64);
	fclose (out);
}

int From_Base( char *Library)
{
	int nymber;
	char elem[6];
	char symbol;
	FILE *base64;
	FILE *out;
	base64 = fopen("Frombase.txt", "r");
	out = fopen("Frombase.orig.txt", "w+");
	while(feof (base64) == 0)
	{
		for (int i = 0; i < 6; i++)
		{
			elem[i] = fgetc(base64);
		}
		nymber = bin_to_dex(elem);
		symbol = Library[nymber];
		fprintf(out, "%c", symbol );
	}
	fclose (base64);
	fclose (out);
}

int main ()
{
	char Library[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	printf("%s\n","press 1 to coding" );
	printf("%s\n", "press 2 to decoding" );
	int n;
	scanf ("%d",&n);
	if (n != 1 && n != 2)
		return(1);
	if (n == 1)
		to_Base (Library);
	else
		From_Base (Library);
}