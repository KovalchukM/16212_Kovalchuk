#include <stdio.h>
#include <stdlib.h>

int step_count(int x , int y , int n , int **arr)
{
	int s = 0;
	//step 1
	if ( (-1 < x - 1) && (x - 1 < n) && (-1 < y + 2) && ( y + 2 < n))
		if(arr[x - 1][y + 2] == 0)
			s++;
	//step 2
	if ( (-1 < x + 1) && (x + 1 < n) && (-1 < y + 2) && ( y + 2 < n))
		if(arr[x + 1][y + 2] == 0)
			s++;
	//step 3
	if ( (-1 < x + 2) && (x + 2 < n) && (-1 < y + 1) && ( y + 1 < n))
		if(arr[x + 2][y + 1] == 0)
			s++;
	//step 4
	if ( (-1 < x + 2) && (x + 2 < n) && (-1 < y - 1) && (y - 1 < n))
		if(arr[x + 2][y - 1] == 0)
			s++;
	//step 5
	if ( (-1 < x + 1) && (x + 1 < n) && (-1 < y - 2) && (y - 2 < n))
		if(arr[x + 1][y - 2] == 0)
			s++;
	//step 6
	if ( (-1 < x - 1) && (x - 1 < n) && (-1 < y - 2) && (y - 2 < n))
		if(arr[x - 1][y - 2] == 0)
			s++;
	//step 7
	if ( (-1 < x - 2) && (x - 2 < n) && (-1 < y - 1) && (y - 1 < n))
		if(arr[x - 2][y - 1] == 0)
			s++;
	//step 8
	if ( (-1 < x - 2) && (x - 2 < n) && (-1 < y + 1) && ( y + 1 < n))
		if(arr[x - 2][y + 1] == 0)
			s++;
	return(s);
}

int *choise(int **arr, int x , int y, int n)
{
	int var[n];
	for(int i = 0; i < 9 ; i++)
		var[i] = -1;
	//step 1
	if ( (-1 < x - 1) && (x - 1 < n) && (-1 < y + 2) && ( y + 2 < n))
		if(arr[x - 1][y + 2] == 0)
			var[0] = step_count(x - 1 , y + 2, n , arr );
	//step 2
	if ( (-1 < x + 1) && (x + 1 < n) && (-1 < y + 2) && ( y + 2 < n))
		if(arr[x + 1][y + 2] == 0)
			var[1] = step_count(x + 1 , y + 2, n , arr );
	//step 3
	if ( (-1 < x + 2) && (x + 2 < n) && (-1 < y + 1) && ( y + 1 < n))
		if(arr[x + 2][y + 1] == 0)
			var[2] = step_count(x + 2 , y + 1, n , arr );
	//step 4
	if ( (-1 < x + 2) && (x + 2 < n) && (-1 < y - 1) && (y - 1 < n))
		if(arr[x + 2][y - 1] == 0)
			var[3] = step_count(x + 2 , y - 1, n , arr );
	//step 5
	if ( (-1 < x + 1) && (x + 1 < n) && (-1 < y - 2) && (y - 2 < n))
		if(arr[x + 1][y - 2] == 0)
			var[4] = step_count(x + 1 , y - 2, n , arr );
	//step 6
	if ( (-1 < x - 1) && (x - 1 < n) && (-1 < y - 2) && (y - 2 < n))
		if(arr[x - 1][y - 2] == 0)
			var[5] = step_count(x - 1 , y - 2, n , arr );
	//step 7
	if ( (-1 < x - 2) && (x - 2 < n) && (-1 < y - 1) && (y - 1 < n))
		if(arr[x - 2][y - 1] == 0)
			var[6] = step_count(x - 2 , y - 1, n , arr );
	//step 8
	if ( (-1 < x - 2) && (x - 2 < n) && (-1 < y + 1) && ( y + 1 < n))
		if(arr[x - 2][y + 1] == 0)
			var[7] = step_count(x - 2 , y + 1, n , arr );

	int *step_priority = (int*)calloc( 9 , sizeof(int));
	int c = 0;
	for(int i = 1 ; i < 9 ; i++)
		for(int k = 0; k < 8; k++){
			if(var[k] == i){
				step_priority[c] = k+1;
				c++;
			}
		}
	return(step_priority);
}

int step(int **arr, int x, int y, int n, int step_count)
{
	/*printf("\n");
	printf("step = %d\n", step_count );
	getch();
	arr[x][y] = step_count;
	for (int i = 0; i < n; i++){
		for (int k = 0; k < n; k++){
			printf("%3d ", arr[k][i] );
		}
		printf("\n");
	}*/
	if (step_count == n*n){
		printf("%s\n","right way" );
		arr[x][y] = step_count;
		for (int i = 0; i < n; i++){
			for (int k = 0; k < n; k++){
				printf("%3d ", arr[k][i] );
			}
			printf("\n");
		}
		printf("\n");
		_Exit(1);
	}
	arr[x][y] = 0;
	int *step_priority = choise(arr, x , y , n );
	/*printf("step_priority =  ");
	for(int i = 0; i < 8 ; i++)
		printf("%2d", step_priority[i] );
	printf("\n");*/
	for(int c = 0; c < 8 ; c++ ){
		//step 1
		if ( step_priority[c] == 1 ){
			arr[x][y] = step_count;
			step(arr, x - 1 , y + 2, n , step_count + 1);
			arr[x][y] = 0;
		}
		// step 2
		if ( step_priority[c] == 2 ){
			arr[x][y] = step_count;
			step(arr, x + 1 , y + 2, n , step_count + 1);
			arr[x][y] = 0;
		}
		// step 3
		if ( step_priority[c] == 3 ){
			arr[x][y] = step_count;
			step(arr, x + 2 , y + 1, n , step_count + 1);
			arr[x][y] = 0;
		}
		//step 4
		if ( step_priority[c] == 4 ){
			arr[x][y] = step_count;
			step(arr, x + 2 , y - 1, n , step_count + 1);
			arr[x][y] = 0;
		}
		//step 5
		if ( step_priority[c] == 5){
			arr[x][y] = step_count;
			step(arr, x + 1 , y - 2, n , step_count + 1);
			arr[x][y] = 0;
		}
		//step 6
		if ( step_priority[c] == 6 ){
			arr[x][y] = step_count;
			step(arr, x - 1 , y - 2, n , step_count + 1);
			arr[x][y] = 0;
		}
		//step 7
		if ( step_priority[c] == 7){
			arr[x][y] = step_count;
			step(arr, x - 2 , y - 1, n , step_count + 1);
			arr[x][y] = 0;
		}
		//step 8
		if ( step_priority[c] == 8 ){
			arr[x][y] = step_count;
			step(arr, x - 2 , y + 1, n , step_count + 1);
			arr[x][y] = 0;
		}
	}
	free(step_priority);
}

int main()
{
	int x = 0;
	int y = 0;
	int n = 0;
	printf("enter x \n");
	scanf("%d", &x);
	printf("enter y\n");
	scanf("%d", &y);
	printf("enter field size\n");
	scanf("%d", &n);
	int **arr = (int**)calloc(n + 1, sizeof(int*));
	for (int i = 0; i < n + 1; i++){
		arr[i] = (int*)calloc(n + 1, sizeof(int));
	}
	for (int i = 0; i < n; i++){
		for (int k = 0; k < n; k++){
			arr[i][k] = 0;
		}
	}
	int step_count = 1;
	step(arr, x, y, n, step_count);
	printf("no ways");
}