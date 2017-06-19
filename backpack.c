#include <stdio.h>
#include <stdlib.h>

typedef struct item{
	int weight;
	int value;
} item_info;

typedef struct backpack_info{
	int *items;
	int weight;
	int value;
} backpack_info;

item_info **read(char *name , int *items_num)
{
	int max = 4;
	FILE *input = fopen(name , "r");
	item_info **item = (item_info**)calloc(max , sizeof(item_info*));
	item[0] = (item_info*)malloc(sizeof(item_info));
	item[0] -> weight = 0;
	item[0] -> value = 0;
	while(!feof(input)){
		item[*items_num] = (item_info*)malloc(sizeof(item_info));
		fscanf(input , "%d", &item[*items_num] -> weight);
		printf("%d  ", item[*items_num] -> weight);
		if(feof(input)){
			free(item[*items_num]);
			break;
		}
		fscanf(input , "%d", &item[*items_num] -> value);
		printf("%d\n", item[*items_num] -> value);
		*items_num += 1;
		if( *items_num == max){
			max *= 2;
			item = realloc(item , max * sizeof(item_info*));
		}
	}
	fclose(input);
	item = realloc(item , (*items_num) * sizeof(item_info*));
	return(item);
}

int **build_table(item_info **item , int items_num , int backpack_size)
{
	int **table = (int**)calloc(items_num, sizeof(int*));
	for(int i = 0; i < items_num ; i++){
		table[i] = (int*)calloc(backpack_size+1, sizeof(int));
		for(int k = 0 ; k < backpack_size+1 ; k++)
			table[i][k] = 0;
	}
	for(int i = 1; i < items_num ; i++)
		for(int k = 1 ; k < backpack_size+1 ; k++){
			if (k >= item[i] -> weight){
				if (table[i-1][k] > table[i-1][k - item[i] -> weight] + item[i] -> value)
					table[i][k] = table[i-1][k];
				else
					table[i][k] =  table[i-1][k - item[i] -> weight] + item[i] -> value;
			}
			else
				table[i][k] = table[i-1][k];
		}
	for(int i = 0; i < items_num ; i++){
		for(int k = 1 ; k < backpack_size+1 ; k++)
			printf("%3d  ", table[i][k] );
		printf("\n");
	}
	printf("\n\n");
	return(table);
}

int find_expensive(int **table , item_info **item , int i , int k , backpack_info *backpack , int *b_items )
{
	if( table[i][k] == 0)
		return(0);
	if( table[i-1][k] == table[i][k])
		find_expensive(table , item ,i-1 , k , backpack , b_items);
	else{
		backpack -> items[*b_items] = i;
		backpack -> weight += item[i] -> weight;
		backpack -> value += item[i] -> value;
		*b_items += 1;
		find_expensive(table , item ,i-1 , k - item[i] -> weight , backpack , b_items);
	}
}

void free_table( int** table , int items_num)
{
	for(int i = 0; i < items_num ; i++){
		free(table[i]);
	}
	free(table);
}

backpack_info *fill_backpack(item_info **item , int items_num , int backpack_size, int *b_items)
{
	backpack_info *backpack = (backpack_info*)malloc(sizeof(backpack_info));
	backpack -> items = (int*)calloc(items_num ,sizeof(int));
	backpack -> weight = 0;
	backpack -> value = 0;
	int **table = build_table(item , items_num , backpack_size);
	find_expensive(table , item , items_num-1 , backpack_size , backpack , b_items);
	free_table(table, items_num);
	return (backpack);
}

int main(int argc, char *argv[]){
	int items_num = 1;
	item_info **item = read(argv[1] , &items_num);
	int backpack_size = 0;
	printf("enter backpack size\n");
	scanf("%d" , &backpack_size);
	int b_items = 0;
	backpack_info *backpack = fill_backpack(item , items_num , backpack_size , &b_items);
	printf("picked items : ");
	for(int i = 0 ; i < b_items ; i++)
		printf("%d ", backpack -> items[i] );
	free(backpack -> items);
	free(backpack);
	return (0);
}
