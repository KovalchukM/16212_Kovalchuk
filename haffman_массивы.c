#include <stdio.h>
#include <stdlib.h>

#define READ_SIZE 8192

typedef struct symbol_info{
	char sym;
	int count;
	struct symbol_info *parent;
	struct symbol_info *left;
	struct symbol_info *right;
} sum_info;

typedef struct new_code{
	char sym;
	int lenght;
	unsigned char *code;
} new_code;

void sort(sum_info **symbols, int size)
{
	int i = 1;
	while(i < size){
		if( symbols[i] -> count > symbols[i-1] -> count){
			sum_info *n = symbols[i];
			symbols[i]= symbols[i-1];
			symbols[i-1] = n;
			if (i > 1)
				i--;
		}
		else
			i++;
	}
}

sum_info *build_tree(sum_info **symbols, int size)
{
	sort(symbols, size+1);
	while( size > 0){
		sum_info *new = (sum_info*)malloc(sizeof(sum_info));
		new -> count = symbols[size] -> count + symbols[size-1] -> count;
		new -> left = symbols[size];
		new -> right = symbols[size-1];
		symbols[size] -> parent = symbols[size-1] ->parent = new;
		symbols[size] = NULL;
		symbols[size-1] = new;
		size--;
		sort(symbols, size+1);
	}
	return(symbols[0]);
}

int sum_check( sum_info **symbols, char s , int records)
{
	for(int k = 0; k < records; k++){
		if (symbols[k] -> sym == s){
			return(k);
		}
	}
	return (-1);
}

sum_info **stream_read(FILE *input, int *records)
{
	int size = 50;
	sum_info **symbols = (sum_info**)calloc(size, sizeof(sum_info*));
	while(1){
		char s = fgetc(input);
		if( feof(input))
			break;
		int check = sum_check(symbols, s , *records);
		if (check > -1)
			symbols[check] -> count += 1;
		if(check == -1){
			symbols[*records] = (sum_info*)malloc(sizeof(sum_info));
			symbols[*records] -> sym = s;
			symbols[*records] -> count = 1;
			symbols[*records] -> parent = NULL;
			symbols[*records] -> left = NULL;
			symbols[*records] -> right = NULL;
			*records+= 1;
			if(*records == size){
				size += 50;
				symbols = realloc(symbols, sizeof(sum_info*) * size);
			}
		}
	}
	return(symbols);
}

void tree_detour(sum_info *root, new_code **code_table, int *id , unsigned char *code , int bit, int lenght , int *code_size)
{
	int n = 0;
	if ( root -> parent != NULL){
		while(lenght > (n+1) * 8 )
			n++;
		if( n+1 > *code_size){
			*code_size += 1;
			code = realloc(code , sizeof(int)*(*code_size));
			for(int i = 0 ; i < *id ; i++)
				code_table[*id] -> code = realloc(code_table[*id] -> code, sizeof(int)*(*code_size));
		}
		code[n] = ((code[n] << 1) | bit);
	}
	if ((root -> left == NULL) && (root -> right == NULL)){
		code_table[*id] = (new_code*)malloc(sizeof(new_code));
		code_table[*id] -> sym = root -> sym;
		code_table[*id] -> code = (char*)calloc(n , sizeof(char));
		for(int i = 0 ; i < n+1 ; i++)
			code_table[*id] -> code[i] = code[i];
		if (root -> parent == NULL)
			code_table[*id] -> lenght = 1;
		else
			code_table[*id] -> lenght = lenght;
		printf("n %d   ", n );
		printf("%c . ", code_table[*id] -> sym);
		printf("%d . ", code_table[*id] -> lenght );
		for(int i = 0; i < n+1 ; i++)
			printf("%d ", code_table[*id] -> code[i] );
		printf("\n");
		*id += 1;
		}
	if (root -> left != NULL){
		tree_detour(root -> left, code_table, id, code, 0, lenght+1 , code_size);
	}
	if (root -> right != NULL){
		tree_detour(root -> right, code_table, id, code, 1, lenght+1 , code_size);
	}
	if((lenght-1) % 8 == 0)
		code[n] = 0;
	else
		code[n] = ((code[n] & 0xFFFFFFFF) >> 1);
	free(root);
}

int seek_short( new_code **code_table, char s , int table_size)
{
	for(int i = 0; i < table_size ; i++){
		if (code_table[i] -> sym == s){
			return(i);
		}
	}
	return(-1);
}

int rewrite_c (FILE* out , char *readed , new_code **code_table, unsigned char *wr ,int size , int count , int code_size , int table_size )
{
	for (int i = 0 ; i < size ; i++){
		char s = readed[i];
		int id = seek_short(code_table, s , table_size);
		if (id == -1){
			printf("invalid input\n");
			_Exit(1);
		}
		unsigned char *code = code_table[id] -> code;
		int lenght = code_table[id] -> lenght;
		if (count + lenght <= 8){
			count += lenght;
			*wr = ((*wr & 0xFFFFFFFF) << lenght) | code[0];
		}
		else{
			int n = 0;
			if( lenght > 8 ){
				while( lenght > (n+1) * 8){
					fprintf(out, "%c", code[n]);
					n++;
				}
				lenght -= 8 * n;
			}
			if(count + lenght > 8)
			*wr = ((*wr & 0xFFFFFFFF) << (8 - count)) | (code[n] >> (count + lenght - 8));
			fprintf(out, "%c", *wr );
			count = count + lenght - 8;
			*wr = (code[n] << (8 - count));
			*wr = (*wr >> (8 - count));
		}
		if (count == 8){
			fprintf(out, "%c", *wr );
			*wr = 0;
			count = 0;
		}
	}
	return(count);
}

void coding (FILE* input, FILE *out , new_code **code_table, int size , int code_size)
{
	int count = 0;
	unsigned char wr = 0;
	char *readed = (char*)calloc(READ_SIZE, sizeof(char));
	while(1){
		int s = fread ( readed , 1 , READ_SIZE , input);
		if ( s == READ_SIZE && !feof(input))
			count = rewrite_c(out , readed , code_table , &wr , s , count , code_size , size);
		else{
			count = rewrite_c(out , readed , code_table , &wr , s , count , code_size , size);
			if (count > 0){
				wr = (wr & 0xFFFFFFFF) << (8 - count);
				fprintf(out, "%c", wr );
				fseek(out , 0 , SEEK_SET);
				fprintf(out, "%d ", 8 - count );
			}
			else{
				fseek(out , 0 , SEEK_SET);
				fprintf(out, "%d ", 0 );
			}
			break;
		}
	}
	free(readed);
}

new_code **code_read(FILE *input, int size , int *code_size)
{
	sum_info **symbols = (sum_info**)calloc(size , sizeof(sum_info*));
	fseek(input , 1 , SEEK_CUR);
	for(int i = 0 ; i < size ; i++){
		symbols[i] = (sum_info*)malloc(sizeof(sum_info));
		symbols[i] -> parent = NULL;
		symbols[i] -> left = NULL;
		symbols[i] -> right = NULL;
		symbols[i] -> sym = fgetc(input);
		fseek(input , 1 , SEEK_CUR);
		fscanf(input, "%d", &(symbols[i] -> count));
		fseek(input , 1 , SEEK_CUR);
	}
	// for(int i = 0 ; i < size ; i++){
	// 	printf("%c  ", symbols[i] -> sym);
	// 	printf("%d\n", symbols[i] -> count );
	// }
	sum_info *root = build_tree(symbols, size-1);
	new_code **code_table = (new_code**)calloc(size, sizeof(new_code*));
	int id = 0;
	unsigned char *code = (char*)calloc(2 , sizeof(char));
	tree_detour(root, code_table , &id , code , 0 , 0 , code_size);
	free(code);
	free(symbols);
	return(code_table);
}

int seek_code( new_code **code_table, unsigned char *wr, int l , int table_size)
{
	for(int i = 0; i < table_size ; i++){
		if( code_table[i] -> lenght == l){
			int n = 1;
			int s = 0;
			while( l > n * 8)
				n++;
			for(int k = 0 ; k < n ; k++)
				if(code_table[i] -> code[k] == wr[k])
					s++;
			if(s == n)
				return(i);
		}
	}
	return(-1);
}

int rewrite_d(FILE *out, char *readed , new_code **code_table , int size , int count , int table_size , unsigned char *wr , int code_size)
{
	char *write = (char*)calloc(READ_SIZE, sizeof(char));
	long int index = 0;
	int max_index = 1;
	int n = 0;
	for(int k = 0; k < size ; k++){
		n = 0;
		while( count > ( n + 1 ) * 8 ){
			n++;
		}
		unsigned char symbol = readed[k];
		for(int i = 0 ; i < 8; i++){
			if(count > 0 && count % 8 == 0){
				n++;
			}
			unsigned char w = symbol << i;
			wr[n] = (wr[n] << 1 | w >> 7);
			count++;
			// printf("symbol %d\n", symbol );
			// printf("count %d\n", count );
			// for(int h = 0 ; h < n+1 ; h++)
			// 	printf("%d . ", wr[h] );
			// printf("\n\n");
			int id = seek_code(code_table, wr , count , table_size);
			if ( id > -1 ){
				write[index] = code_table[id] -> sym;
				printf("wr %c\n", write[index] );
				index++;
				if( index == READ_SIZE * max_index){
					max_index++;
					write = realloc(write, sizeof(char) * READ_SIZE * max_index);
				}
				count = 0;
				for(int b = 0 ; b < n+1 ; b++)
					wr[b] = 0;
			}
		}
	}
	fwrite( write , 1 , index , out);
	printf("%s", write );
	free(write);
	return(count);
}

void decoding(FILE *input, FILE *out, new_code **code_table , int null_bits , int table_size , int code_size)
{
	int count = 0;
	unsigned char *wr = (char*)calloc(code_size , sizeof(char));
	unsigned char *readed = (char*)calloc(READ_SIZE, sizeof(char));
	unsigned char symbol = 0;
	while(1){
		int s = fread ( readed , 1 , READ_SIZE , input);
		if(feof(input)){
			if (null_bits != 0){
				count = rewrite_d(out , readed , code_table , s-1 , count , table_size , wr, code_size);
				symbol = readed[s-1];
				int n = 0;
				while( count > ( n + 1 ) * 8 )
					n++;
				for(int i = 0 ; i < 8 - null_bits; i++){
					if(count > 0 && count % 8 == 0)
						n++;
					unsigned char w = symbol << i;
					wr[n] = (wr[n] << 1 | w >> 7);
					count++;
					int id = seek_code(code_table, wr , count , table_size);
					if ( id > -1 ){
						fprintf(out, "%c", code_table[id] -> sym );
						count = 0;
					}
				}
			}
			else
				count = rewrite_d(out , readed , code_table , s , count , table_size , wr, code_size);
			break;
		}
		count = rewrite_d(out , readed , code_table , s , count , table_size , wr, code_size);
	}
	free(readed);
}

int main (int argc , char *argv[])
{
	FILE *input = fopen(argv[1], "rb");
	printf("%s\n","press 1 to coding" );
	printf("%s\n", "press 2 to decoding" );
	int n = 0;
	scanf ("%d",&n);
	if (n == 1){
		int records = 0;
		sum_info **symbols = stream_read(input, &records);
		FILE *out = fopen("out.txt", "wb+");
		printf("records %d\n", records );
		fprintf(out, "  %d ", records);
		for(int i = 0 ; i < records ; i++){
			fprintf(out, "%c ", symbols[i] -> sym );
			fprintf(out, "%d ", symbols[i] -> count);
		}
		sum_info *root = build_tree(symbols, records-1);
		new_code **code_table = (new_code**)calloc(records, sizeof(new_code*));
		int id = 0;
		int code_size = 2;
		unsigned char *code = (char*)calloc(2 , sizeof(char));
		tree_detour(root, code_table , &id , code , 0 , 0 , &code_size);
		free(code);
		free(symbols);
		rewind(input);
		printf("coding\n\n");
		coding(input, out , code_table , records , code_size);
		for(int i = 0 ; i < records; i++)
			free(code_table[i]);
		fclose(out);
		fclose(input);
	}
	else if (n == 2){
		int size = 0;
		int null_bits = 0;
		fscanf(input, "%d", &null_bits);
		fscanf(input, "%d", &size);
		printf("size %d\n", size );
		int code_size = 2;
		new_code **code_table = code_read(input , size , &code_size);
		FILE *out = fopen("decoded.txt", "wb+");
		printf("decoding\n\n");
		decoding(input, out , code_table, null_bits , size , code_size);
		for(int i = 0 ; i < size; i++)
			free(code_table[i]);
		fclose(input);
		fclose(out);
	}
	else
		printf("error operation key\n");
	return(0);
}