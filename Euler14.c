#include <stdio.h>
#include <conio.h>
int main()
{
	int maxChain = 1;
	int chain = 1;
	int beg;
	int p1;
	int a;
	int p2;
	int nymber;
	for(beg=1;beg<1000000;beg++)
	{
		p1=beg;
		p2=p1%10000;
		p1=p1/10000;
			while(p2>1)	
			{
				a=p2%2;
				if(a==0)
				{
					p2=p2/2;
					a=p1%2;
					if (a==1)
					{
						p2=p2+5000;
					}
					p1=p1/2;
				}
				else
				{
					p1=3*p1;
					p2=3*p2+1;
					if(p2>=10000)
					{
						a=p2/10000;
						p1=p1+a;
						p2=p2%10000;
					}
				}
				chain=chain+1;
			}
		if (chain>maxChain)
		{
			maxChain=chain;
			nymber=beg;
		}
		chain=1;
	}
	printf("%s\n","The end" );
	printf("%d\n",maxChain);
	printf("%s\t","answer:" );
	printf("%d\n",nymber );
}