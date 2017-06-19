#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
	int id;
	int num_siblings;
	struct node **siblings;
	int visited;
	int *visited_siblings;
} NODE;

typedef struct list
{
	NODE *vertex;
	struct list* next;
} list;

int seek_sibling(NODE *graph)
{
	int count = 0;
	for(int i = 0 ; i < graph -> num_siblings ; i++){
		for(int k = 0 ; k < graph -> visited ; k++){
			if(graph -> siblings[i] -> id != graph -> visited_siblings[k])
				count++;
		}
		if (count == graph -> visited)
			return(i);
		count = 0;
	}
	if (graph -> num_siblings != graph -> visited){
		int sib[graph -> num_siblings];
		int vis[graph -> visited];
		for(int i = 0; i < graph -> num_siblings ; i++){
			sib[i] = 0;
			for(int k = 0; k < graph -> num_siblings ; k++){
				if(graph -> siblings[i] -> id == graph -> siblings[k] -> id)
					sib[i] += 1;
			}
		}
		for(int i = 0; i < graph -> visited ; i++){
			vis[i] = 0;
			for(int k = 0; k < graph -> visited ; k++){
				if(graph -> visited_siblings[i] == graph -> visited_siblings[k])
					vis[i] += 1;
			}
		}
		for(int i = 0 ; i < graph -> visited ; i++){
			if( sib[i] > vis[i])
				return(i);
		}
	}
	return (-1);
}

int seek_unvisited(list *head)
{
	list *temp = head;
	temp -> vertex -> visited -= 1;
	while(1){
		temp = temp -> next;
		if(temp == NULL)
			return(head -> vertex -> id);
		if(temp -> vertex -> visited != temp -> vertex -> num_siblings)
			return(temp -> vertex -> id);
		temp -> vertex -> visited -= 1;
	}
}

void graph_detour(NODE *graph)
{
	int *path = (int*)calloc( 11 , sizeof(int));
	list *head = (list*)malloc(sizeof(list));
	head -> vertex = graph;
	head -> next = NULL;
	list *tail = head;
	list *temp = NULL;
	path[0] = graph -> id;
	int count = 1;
	int id = 0;
	while(1){
		id = seek_sibling(graph);
		if(id > -1){
			temp = (list*)malloc(sizeof(list));
			temp -> vertex = graph;
			temp -> next = head;
			head = temp;
			graph -> visited_siblings[graph -> visited] = graph -> siblings[id] -> id;
			path[count] = graph -> siblings[id] -> id;
			count++;
			graph -> visited += 1;
			graph = graph -> siblings[id];
			graph -> visited_siblings[graph -> visited] = head -> vertex -> id;
			graph -> visited += 1;
		}
		if(graph -> visited == graph -> num_siblings ){
			id = seek_unvisited(head);
			if (id == head -> vertex -> id){
				break;
			}
			else
				while(head -> vertex -> id != id){
					head -> vertex -> visited -= 1;
					head = head -> next;
					count--;
				}
		}
	}
	for(int i = 0 ; i < count ; i++)
		printf("%d  ", path[i] );
}

int main (){
	NODE **graph = ( NODE** ) calloc ( 11, sizeof ( NODE* ) );
	for ( int i = 0; i < 11; i++ )
		graph [ i ] = ( NODE * ) malloc (sizeof ( NODE ) );

// graph [ 0 ] - Стартовая точка пути, graph [ 7 ] - Конец пути
graph [ 0 ] -> num_siblings = 3;
graph [ 0 ] -> id = 0;
graph [ 0 ] -> siblings = ( NODE ** ) calloc ( graph [ 0 ] -> num_siblings, sizeof ( NODE * ) );
graph [ 0 ] -> siblings [ 0 ] = graph [ 1 ];
graph [ 0 ] -> siblings [ 1 ] = graph [ 3 ];
graph [ 0 ] -> siblings [ 2 ] = graph [ 4 ];
graph [ 0 ] -> visited = 0;
graph [ 0 ] -> visited_siblings = ( int * ) calloc ( graph [ 0 ] -> num_siblings, sizeof ( int ) );

graph [ 1 ] -> num_siblings = 2;
graph [ 1 ] -> id = 1;
graph [ 1 ] -> siblings = ( NODE ** ) calloc ( graph [ 1 ] -> num_siblings, sizeof ( NODE * ) );
graph [ 1 ] -> siblings [ 0 ] = graph [ 0 ];
graph [ 1 ] -> siblings [ 1 ] = graph [ 4 ];
graph [ 1 ] -> visited = 0;
graph [ 1 ] -> visited_siblings = ( int * ) calloc ( graph [ 1 ] -> num_siblings, sizeof ( int ) );

graph [ 2 ] -> num_siblings = 2;
graph [ 2 ] -> id = 2;
graph [ 2 ] -> siblings = ( NODE ** ) calloc ( graph [ 2 ] -> num_siblings, sizeof ( NODE * ) );
graph [ 2 ] -> siblings [ 0 ] = graph [ 4 ];
graph [ 2 ] -> siblings [ 1 ] = graph [ 3 ];
graph [ 2 ] -> visited = 0;
graph [ 2 ] -> visited_siblings = ( int * ) calloc ( graph [ 2 ] -> num_siblings, sizeof ( int ) );

graph [ 3 ] -> num_siblings = 2;
graph [ 3 ] -> id = 3;
graph [ 3 ] -> siblings = ( NODE ** ) calloc ( graph [ 3 ] -> num_siblings, sizeof ( NODE * ) );
graph [ 3 ] -> siblings [ 0 ] = graph [ 0 ];
graph [ 3 ] -> siblings [ 1 ] = graph [ 2 ];
graph [ 3 ] -> visited = 0;
graph [ 3 ] -> visited_siblings = ( int * ) calloc ( graph [ 3 ] -> num_siblings, sizeof ( int ) );

graph [ 4 ] -> num_siblings = 4;
graph [ 4 ] -> id = 4;
graph [ 4 ] -> siblings = ( NODE ** ) calloc ( graph [ 4 ] -> num_siblings, sizeof ( NODE * ) );
graph [ 4 ] -> siblings [ 0 ] = graph [ 0 ];
graph [ 4 ] -> siblings [ 1 ] = graph [ 1 ];
graph [ 4 ] -> siblings [ 2 ] = graph [ 2 ];
graph [ 4 ] -> siblings [ 3 ] = graph [ 5 ];
graph [ 4 ] -> visited = 0;
graph [ 4 ] -> visited_siblings = ( int * ) calloc ( graph [ 4 ] -> num_siblings, sizeof ( int ) );

graph [ 5 ] -> num_siblings = 4;
graph [ 5 ] -> id = 5;
graph [ 5 ] -> siblings = ( NODE ** ) calloc ( graph [ 5 ] -> num_siblings, sizeof ( NODE * ) );
graph [ 5 ] -> siblings [ 0 ] = graph [ 4 ];
graph [ 5 ] -> siblings [ 1 ] = graph [ 6 ];
graph [ 5 ] -> siblings [ 2 ] = graph [ 8 ];
graph [ 5 ] -> siblings [ 3 ] = graph [ 10 ];
graph [ 5 ] -> visited = 0;
graph [ 5 ] -> visited_siblings = ( int * ) calloc ( graph [ 5 ] -> num_siblings, sizeof ( int ) );

graph [ 6 ] -> num_siblings = 2;
graph [ 6 ] -> id = 6;
graph [ 6 ] -> siblings = ( NODE ** ) calloc ( graph [ 6 ] -> num_siblings, sizeof ( NODE * ) );
graph [ 6 ] -> siblings [ 0 ] = graph [ 5 ];
graph [ 6 ] -> siblings [ 1 ] = graph [ 7 ];
graph [ 6 ] -> visited = 0;
graph [ 6 ] -> visited_siblings = ( int * ) calloc ( graph [ 6 ] -> num_siblings, sizeof ( int ) );

graph [ 7 ] -> num_siblings = 3;
graph [ 7 ] -> id = 7;
graph [ 7 ] -> siblings = ( NODE ** ) calloc ( graph [ 7 ] -> num_siblings, sizeof ( NODE * ) );
graph [ 7 ] -> siblings [ 0 ] = graph [ 6 ];
graph [ 7 ] -> siblings [ 1 ] = graph [ 8 ];
graph [ 7 ] -> siblings [ 2 ] = graph [ 8 ];
graph [ 7 ] -> visited = 0;
graph [ 7 ] -> visited_siblings = ( int * ) calloc ( graph [ 7 ] -> num_siblings, sizeof ( int ) );

graph [ 8 ] -> num_siblings = 4;
graph [ 8 ] -> id = 8;
graph [ 8 ] -> siblings = ( NODE ** ) calloc ( graph [ 8 ] -> num_siblings, sizeof ( NODE * ) );
graph [ 8 ] -> siblings [ 0 ] = graph [ 5 ];
graph [ 8 ] -> siblings [ 1 ] = graph [ 7 ];
graph [ 8 ] -> siblings [ 2 ] = graph [ 7 ];
graph [ 8 ] -> siblings [ 3 ] = graph [ 9 ];
graph [ 8 ] -> visited = 0;
graph [ 8 ] -> visited_siblings = ( int * ) calloc ( graph [ 8 ] -> num_siblings, sizeof ( int ) );

graph [ 9 ] -> num_siblings = 2;
graph [ 9 ] -> id = 9;
graph [ 9 ] -> siblings = ( NODE ** ) calloc ( graph [ 9 ] -> num_siblings, sizeof ( NODE * ) );
graph [ 9 ] -> siblings [ 0 ] = graph [ 8 ];
graph [ 9 ] -> siblings [ 1 ] = graph [ 10 ];
graph [ 9 ] -> visited = 0;
graph [ 9 ] -> visited_siblings = ( int * ) calloc ( graph [ 9 ] -> num_siblings, sizeof ( int ) );

graph [ 10 ] -> num_siblings = 2;
graph [ 10 ] -> id = 10;
graph [ 10 ] -> siblings = ( NODE ** ) calloc ( graph [ 10 ] -> num_siblings, sizeof ( NODE *) );
graph [ 10 ] -> siblings [ 0 ] = graph [ 5 ];
graph [ 10 ] -> siblings [ 1 ] = graph [ 9 ];
graph [ 10 ] -> visited = 0;
graph [ 10 ] -> visited_siblings = ( int * ) calloc ( graph [ 10 ] -> num_siblings, sizeof ( int ) );

graph_detour(graph[0]);
}
