#include <stdio.h>
#include <stdlib.h>

typedef struct graph {
	int number;
	int neigh_num;
	struct graph **neighbors;
	float *lenght;
} graph;

typedef struct paths {
	int ver_1;
	int ver_2;
	float lenght;
} path;

path **read(FILE *input , int *size)
{
	path **paths = (path**)calloc(*size , sizeof(path*));
	int count = 0;
	while(!feof(input)){
		paths[count] = (path*)calloc(2 , sizeof(path));
		fscanf(input, "%d ", &paths[count] -> ver_1);
		if(feof(input)){
			paths[count] -> ver_2 = paths[count] -> ver_1;
			count++;
			break;
		}
		fscanf(input, "%d", &paths[count] -> ver_2);
		if(feof(input))
			break;
		fscanf(input, "%f", &paths[count] -> lenght);
		count++;
		if (count == *size ){
			*size = *size * 2;
			paths = realloc(paths , sizeof(int*)*(*size));
		}
	}
	*size = count;
	return (paths);
}

int seek_vertex_id(graph **vertices , int n , int ver_num)
{
	for(int i = 0 ; i < ver_num ; i++)
		if( vertices[i] -> number == n)
			return(i);
	return(-1);
}

graph *vertex_create(int id , int size)
{
	graph *vertex = (graph*)malloc(sizeof(graph));
	vertex -> number = id;
	vertex -> neigh_num = 0;
	vertex -> neighbors = (graph**)calloc(size , sizeof(graph*));
	for(int i = 0; i < size ; i++)
		vertex -> neighbors[i] = NULL;
	vertex -> lenght = (float*)calloc(size , sizeof(float));
	return(vertex);
}

graph **build_graph( FILE *input , int *ver_num)
{
	int size = 4;
	path **paths = read(input , &size);
	fclose(input);
	int ver_max = 4;
	graph **vertices = (graph**)calloc( ver_max , sizeof(graph*));
	int n = paths[0] -> ver_1;
	int m = paths[0] -> ver_2;
	vertices[0] = vertex_create(n , size);
	vertices[1] = vertex_create(m , size);
	vertices[0] -> neigh_num = 1;
	vertices[0] -> neighbors[0] = vertices[1];
	vertices[0] -> lenght[0] = paths[0] -> lenght;
	*ver_num = 2;
	for(int i = 1 ; i < size ; i++){
		n = paths[i] -> ver_1;
		int id_1 = seek_vertex_id(vertices , n , *ver_num);
		if (id_1 == -1){
			vertices[*ver_num] = vertex_create(n , size);
			id_1 = *ver_num;
			*ver_num += 1;
		}
		if(*ver_num == ver_max){
			ver_max = ver_max * 2;
			vertices = realloc(vertices , sizeof(graph*) * ver_max );
		}
		m = paths[i] -> ver_2;
		int id_2 = seek_vertex_id(vertices , m , *ver_num);
		if ((id_2 == -1) && (n != m)){
			vertices[*ver_num] = vertex_create(m , size);
			id_2 = *ver_num;
			*ver_num += 1;
		}
		if (n != m){
			for(int k = 0; k < size ; k++)
				if (vertices[id_1] -> neighbors[k] == NULL){
					vertices[id_1] -> neighbors[k] = vertices[id_2];
					vertices[id_1] -> lenght[k] = paths[i] -> lenght;
					vertices[id_1] -> neigh_num += 1;
					break;
				}
		}
	}
	for(int i = 0 ; i < size ; i++)
		free(paths[i]);
	free(paths);
	return(vertices);
}