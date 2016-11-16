#include <stdio.h>
#include <stdlib.h>


#define MAX_SIZE 9

int scan(int *a,int *b,int N)
{
	int max = a[0];
	int idmax = -1;
	for (int i = 0; i < N; i++)
	{
		if (a[i] < max)
		{
			max = a[i];
		}
	}
	for (int i = 0; i < N; i++)
	{
		if ( a[i] >= max)
		{
			if ( b[i] == 0)
			{
				if ( (i > 0) && (a[i-1] < a[i]) )
				{
					max = a[i];
					idmax = i;
				}
			}
			else
			{
				if ( ((i + 1) < N) && (a[i+1] < a[i]) )
				{
					max = a[i];
					idmax = i;
				}
			}
		}
	}
	return (idmax);
}

int swap (int *a, int *b,int idmax)
{
	int c;
	int last_element = a[idmax];
	if (b[idmax] == 0)
	{
		c = a[idmax];
		a[idmax] = a[idmax-1];
		a[idmax-1] = c;
		c = b[idmax];
		b[idmax] = b [idmax-1];
		b[idmax -1] = c;
	}
	else
	{		
		c = a[idmax];
		a[idmax] = a[idmax+1];
		a[idmax+1] = c;
		c = b[idmax];
		b[idmax] = b[idmax+1];
		b[idmax+1] = c;
	}
	return (last_element);
}

int change(int *a, int *b, int N, int last_element)
{
	for(int i = 0; i < N; i++)
	{
		if ( a[i] > last_element)
		{
			if (  b[i] == 0 )
			{
				b[i] = 1;
			}
			else
			{
				b[i] = 0;
			}
		}
	}
}

int factorial (int N)
{
	int k = 1;
	for (int i = 1; i <= N; i++)
	{
		k = k * i;
	}
	return (k);
}

int main( int argc , char *argv[])
{
	int N = atoi(argv[1]);
	int last_element = 1;
	int count = 1;
	if ( N < 1 || N > 9)
		return (-1);
	int a[N];
	int b[N];
	for (int i = 0; i < N; i++)
	{
		a[i] = i + 1;
		b[i] = 0;
		printf("%d", a[i] );
	}
	printf("\n");
	while ( count < factorial(N))
	{
		for (int i = 0; i < N ; i++)
		{
			int idmax = scan( a, b, N);
			last_element = swap( a, b, idmax);
			if ( count < factorial(N))
			{
				for (int j = 0; j < N; j++)
				{
					printf("%d", a[j] );
				}
				printf("\n");
			}
			count++;
		}
		change ( a, b, N, last_element);
	}
}