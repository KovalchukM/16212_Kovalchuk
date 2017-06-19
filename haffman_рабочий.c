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
	int code;
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
	long int i = 0;
	while(1){
		char s = fgetc(input);
		i++;
		if( feof(input))
			break;
		int check = sum_check(symbols, s , *records);
		if (check > -1)
			symbols[check] -> count += 1;
		if(check == -1){
			symbols[*records] = (sum_info*)malloc(sizeof(sum_info));
			symbols[*records] -> sym = s;
			symbols[*records] -> count = 1;
			symbols[*records] -> left = symbols[*records] -> right = NULL;
			*records+= 1;
			if(*records == size){
				size += 50;
				symbols = realloc(symbols, sizeof(sum_info*) * size);
			}
		}
	}
	return(symbols);
}

void tree_detour(sum_info *root, new_code **code_table, int *id , int code , int bit, int lenght)
{
	if ( root -> parent != NULL){
		code = ((code << 1) | bit);
	}
	if ((root -> left == NULL) && (root -> right == NULL)){
		code_table[*id] = (new_code*)malloc(sizeof(new_code));
		code_table[*id] -> sym = root -> sym;
		code_table[*id] -> code = code;
		if (root -> parent == NULL)
			code_table[*id] -> lenght = 1;
		else
			code_table[*id] -> lenght = lenght;
		printf("%c . ", code_table[*id] -> sym);
		printf("%d . ", code_table[*id] -> lenght );
		printf("%d ", code_table[*id] -> code );
		printf("\n");
		*id += 1;
		}
	if (root -> left != NULL){
		tree_detour(root -> left, code_table, id, code, 0, lenght+1);
	}
	if (root -> right != NULL){
		tree_detour(root -> right, code_table, id, code, 1, lenght+1);
	}
}

int seek_short( new_code **code_table, char s , int size)
{
	for(int i = 0; i < size ; i++){
		if (code_table[i] -> sym == s){
			return(i);
		}
	}
	return(-1);
}

int rewrite_c (FILE* out , char *readed , new_code **code_table, int *wr ,int size , int count )
{
	for (int i = 0 ; i < size ; i++){
		char s = readed[i];
		int id = seek_short(code_table, s , size);
		if (id == -1){
			printf("invalid input\n");
			_Exit(1);
		}
		int code = code_table[id] -> code;
		int lenght = code_table[id] -> lenght;
		if (count + lenght <= 8){
			count += lenght;
			*wr = ((*wr & 0xFFFFFFFF) << lenght) | code;
		}
		else if(count + lenght > 8){
			*wr = ((*wr & 0xFFFFFFFF) << (8 - count)) | (code >> (count + lenght - 8));
			fprintf(out, "%c", *wr );
			count = count + lenght - 8;
			*wr = (code << (8 - count));
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

void coding (FILE* input, FILE *out , new_code **code_table, int size)
{
	int count = 0;
	int wr = 0;
	char *readed = (char*)calloc(READ_SIZE, sizeof(char));
	//printf("size %d\n", size );
	fprintf(out, "  %d ", size );
	for(int i = 0; i < size ; i++){
		fprintf(out, "%c ", code_table[i] -> sym );
		fprintf(out, "%d ", code_table[i] -> lenght );
		fprintf(out, "%d ", code_table[i] -> code);
	}
	while(1){
		int s = fread ( readed , 1 , READ_SIZE , input);
		if ( s == READ_SIZE)
			count = rewrite_c(out , readed , code_table , &wr , READ_SIZE , count);
		else{
			count = rewrite_c(out , readed , code_table , &wr , s , count);
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

new_code **code_read(FILE *input, int size)
{
	new_code **code_table = (new_code**)calloc(size, sizeof(new_code*));
	for(int i = 0; i < size ; i++){
		code_table[i] = (new_code*)malloc(sizeof(new_code));
		code_table[i] -> sym = fgetc(input);
		fseek(input , 1 , SEEK_CUR);
		fscanf(input, "%d", &(code_table[i] -> lenght));
		fseek(input , 1 , SEEK_CUR);
		fscanf(input, "%d", &code_table[i] -> code);
		fseek(input , 1 , SEEK_CUR);
	}
	return(code_table);
}

int seek_code( new_code **code_table, int s , int l , int size)
{
	for(int i = 0; i < size ; i++){
		if ((code_table[i] -> code == s) && (code_table[i] -> lenght == l ))
			return(i);
	}
	return(-1);
}

int rewrite_d(FILE* out, char *readed , new_code **code_table , int size , int count , int table_size, int *rem , int *rem_lenght)
{
	char *write = (char*)calloc(READ_SIZE, sizeof(char));
	long int index = 0;
	int wr = 0;
	int m = 1;
	for(int k = 0; k < size ; k++){
		unsigned char symbol = readed[k];
		for(int i = 0 ; i < 8; i++){
			wr = ((symbol >> (8 - i)) | (*rem << i));
			int id = seek_code(code_table, wr , count + *rem_lenght, table_size);
			if ( id > -1 ){
				write[index] = code_table[id] -> sym;
				index++;
				if( index == READ_SIZE * m){
					m++;
					write = realloc(write, sizeof(char) * READ_SIZE * m);
				}
				*rem = 0;
				*rem_lenght = 0;
				count = 1;
				symbol = (symbol << i);
				symbol = (symbol >> i);
			}
			else
				count++;
		}
		*rem = ((*rem << count) | symbol);
		*rem_lenght += count;
		count = 0;
	}
	fwrite( write , 1 , index , out);
	free(write);
	return(count);
}

void decoding(FILE *input, FILE *out, new_code **code_table , int null_bits , int table_size)
{
	int count = 0;
	int rem = 0;
	int rem_lenght = 0;
	int wr = 0;
	unsigned char *readed = (char*)calloc(READ_SIZE, sizeof(char));
	unsigned char symbol = 0;
	while(1){
		int s = fread ( readed , 1 , READ_SIZE , input);
		if(feof(input)){
			if (null_bits != 0){
				count = rewrite_d(out , readed , code_table , s-1 , count , table_size ,&rem , &rem_lenght);
				symbol = readed[s-1];
				for(int i = 0 ; i < 8 - null_bits+1; i++){
				wr = ((symbol >> (8 - i)) | (rem << i));
				int id = seek_code(code_table, wr , count + rem_lenght, table_size);
				if ( id > -1 ){
					fprintf(out, "%c", code_table[id] -> sym );
					rem = 0;
					rem_lenght = 0;
					count = 1;
					symbol = (symbol << i);
					symbol = (symbol >> i);
				}
				else
					count++;
				}
			}
			else
				count = rewrite_d(out , readed , code_table , s , count , table_size , &rem , &rem_lenght);
			break;
		}
		count = rewrite_d(out , readed , code_table , s , count , table_size , &rem , &rem_lenght);
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
		sum_info *root = build_tree(symbols, records-1);
		new_code **code_table = (new_code**)calloc(records, sizeof(new_code*));
		int id = 0;
		tree_detour(root, code_table , &id , 0 , 0 , 0);
		for(int i; i < records ; i++)
			free(symbols[i]);
		rewind(input);
		FILE *out = fopen("out.txt", "wb+");
		printf("coding\n\n");
		coding(input, out , code_table , records);
		for(int i = 0 ; i < records; i++)
			free(code_table[i]);
		fclose(out);
		fclose(input);
	}
	else if (n == 2){
		int size = 0;
		int null_bits = 0;
		fscanf(input, "%d", &null_bits);
		fseek(input , 1 , SEEK_CUR);
		fscanf(input, "%d", &size);
		fseek(input , 1 , SEEK_CUR);
		new_code **code_table = code_read(input , size);
		FILE *out = fopen("decoded.txt", "wb+");
		printf("decoding\n\n");
		decoding(input, out , code_table, null_bits , size);
		fclose(input);
		fclose(out);
	}
	else
		printf("error operation key\n");
	return(0);
}