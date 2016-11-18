#include <stdio.h>
#include <stdlib.h>

#define SIZE 64

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
	while (1)
	{
		if (feof(base64) != 0)
			break;
		elem = fgetc(base64);
		printf("elem = %c\n",elem );
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

int Dex_to_bin(int nymber, FILE out)
{
	int c = 0;
	int k = 0;
	char bin[] = "00000";
	while (nymber > 0)
	{
		if (nymber % 2 == 1)
			bin[4 - k] = "1";
		nymber = nymber/2;
		k++;
	}
	printf("%s", bin );
	printf("\n");
}
int From_Base( char *Library)
{
	int nymber;
	char elem[5];
	char symbol;
	FILE *base64;
	FILE *out;
	base64 = fopen("Frombase.txt", "r");
	out = fopen("Frombase.orig.txt", "w+");
	if (base64 == NULL)
		return(1);
	while(1)
	{
		if (feof (base64) != 0)
			break;
		for (int i = 0; i < 5; i++)
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

int bin_to_dex(char *elem)
{
	int nymber = 0;
	int n = 0;
	int c = 1;
	for(int i = 4; i >= 0; i = i - 1)
	{
		if (elem[i] == 1)
		{
			for (int i = 0; i < n ; i++)
				c = c * 2;
			nymber = nymber + c;
		}
		n++;
	}
	return(nymber);
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
	if (n = 1)
		to_Base (Library);
	else
		From_Base (Library);
}