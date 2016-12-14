#include <stdio.h>
#include <stdlib.h>

#define SIZE 64

int to_Base(char *Library, char *input)
{
	char elem[3] = {0};
	char base_elem[4] = {0};
	FILE *base64 = fopen ( input, "r");
	FILE *out = fopen ( "out.base64.txt", "w+");
	  if (base64 == NULL) 
	  {
	  	printf("open error.\n");
	  	return(1);
	  }
	while (1)
	{
		int end_check = fread(elem, sizeof(char) , 3, base64);
		if (end_check == 0)
		{
			break;
		}
		if (end_check == 1)
		{
			base_elem[0] = elem[0] >> 2;
			base_elem[1] = ((elem[0] << 4)& 0x3F);
			fprintf(out, "%c", Library[base_elem[0]] );
			fprintf(out, "%c", Library[base_elem[1]] );
			fprintf(out, "%s", "==" );
			break;
		}
		if (end_check == 2)
		{
			base_elem[0] = elem[0] >> 2;
			base_elem[1] = (((elem[0] << 4) | (elem[1] >> 4)) & 0x3F);
			base_elem[2] = ((elem[1] << 2) & 0x3F);
			for (int i = 0; i < 3; i++)
				fprintf(out, "%c",Library[base_elem[i]] );
			fprintf(out, "%c", '=' );
			break;
		}
		if (end_check == 3)
		{
			base_elem[0] = elem[0] >> 2;
			base_elem[1] = (((elem[0] << 4) | (elem[1] >> 4)) & 0x3F);
			base_elem[2] = (((elem[1] << 2) | (elem[2] >> 6)) & 0x3F);
			base_elem[3] = (elem[2] & 0x3F);
			for (int i = 0 ; i < 4 ; i++)
			{
				printf("%c", base_elem[i] );
				fprintf(out, "%c", Library[base_elem[i]] );
			}
			printf("\n");
		}
	}
	printf("%s\n","coding complete" );
	fclose (base64);
	fclose (out);
}

int From_Base(char *Library, char *input)
{
	char base_elem[4] = {0};
	int base_elem_index[4];
	char elem[3];
	FILE *base64 = fopen(input, "r");
	FILE *out = fopen("out.base64.orig.txt", "w+");
	  if (base64 == NULL) 
	  {
	  	printf("open error\n");
	  	return(1);
	  }
	while(1)
	{
		int end_check = fread(base_elem, sizeof(char) , 4, base64);
		for (int i = 0; i < 4; i++)
		{
			for(int k = 0; k < SIZE; k++)
			{
				if (base_elem[i] == Library[k])
					base_elem_index[i] = k;
			}
		}
		if (end_check == 0)
			break;
		if (base_elem[1] == '=')
		{
			elem[0] = (base_elem_index[0] << 2);
			fprintf(out, "%c", elem[0] );
			break;
		}
		elem[0] = (base_elem_index[0] << 2) | (base_elem_index[1] >> 4);
		if ( base_elem[2] == '=')
		{
			elem[1] = (base_elem_index[1] << 4);
			fprintf(out, "%c", elem[0] );
			fprintf(out, "%c", elem[1] );
			break;
		}
		elem[1] = (base_elem_index[1] << 4) | (base_elem_index[2] >> 2);
		if ( base_elem[3] == '=')
		{
			elem[2] = (base_elem_index[2] << 6);
			for (int i = 0; i < 3; i++)
				fprintf(out, "%c", elem[i] );
			break;
		}
		elem[2] = (base_elem_index[2] << 6) | base_elem_index[3];
		for (int i = 0; i < 3; i++)
			fprintf(out, "%c", elem[i] );
	}
	printf("%s\n","decoding complete" );
	fclose(base64);
	fclose(out);
}

int main (int argc , char *argv[])
{
	char Library[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	printf("%s\n","press 1 to coding" );
	printf("%s\n", "press 2 to decoding" );
	int n = 0;
	scanf ("%d",&n);
	if (n == 1)
		to_Base (Library, argv[1]);
	if (n == 2)
		From_Base (Library, argv[1]);
}
