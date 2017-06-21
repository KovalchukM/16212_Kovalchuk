#include <stdio.h>
#include <stdlib.h>

typedef struct graph {
	int number;
	int neigh_num;
	struct graph **neighbors;
	int sum_flow;
	int *max_flow;
	int *cur_flow;
} graph;

typedef struct list {
	graph *vertex;
	struct list *parent;
	struct list *next;
} list;

void free_vertices(graph **vertices , int ver_num)
{
	for(int i = 0 ; i < ver_num ; i++){
		free(vertices[i] -> neighbors);
		free(vertices[i] -> max_flow);
		free(vertices[i] -> cur_flow);
		free(vertices[i]);
	}
	free(vertices);
}

void peek(list *queue_head)
{
	list *queue_cur = queue_head;
	while(queue_cur != NULL){
		printf("%d  ", queue_cur -> vertex -> number );
		queue_cur = queue_cur -> next;
	}
	printf("\n");
}

list *queue_create(graph **vertices , int source_num , int ver_num)
{
	list *queue_head = (list*)malloc(sizeof(list));
	for(int i = 0 ; i < ver_num ; i++){
		if(vertices[i] -> number == source_num)
			queue_head -> vertex = vertices[i];
	}
	queue_head -> parent = NULL;
	queue_head -> next = NULL;
	return(queue_head);
}

list *push(list *queue_tail , list *queue_cur , int neigh_id)
{
	list *temp = (list*)malloc(sizeof(list));
	temp -> vertex = queue_cur -> vertex -> neighbors[neigh_id];
	temp -> next = NULL;
	temp -> parent = queue_cur;
	queue_tail -> next = temp;
	queue_tail = temp;
	return(queue_tail);
}

list *find_path(list *queue_head , list *queue_tail , int *marked)
{
	list *new_tail = queue_tail;
	list *temp = NULL;
	while(new_tail -> parent != NULL){
		temp = new_tail -> parent;
		temp -> next = new_tail;
		new_tail = temp;
	}
	queue_head -> next = new_tail -> next;
	return(queue_head);
}

list *graph_detour(graph **vertices , int ver_num , int source_num , int sink_num)
{
	int *marked = (int*)calloc(ver_num , sizeof(int));
	int count = 0;
	list *queue_head = queue_create(vertices , source_num , ver_num);
	list *queue_tail = queue_head;
	list *queue_cur = queue_head;
	marked[count] = queue_head -> vertex -> number;
	count++;
	int found = 0;
	while(1){
		for(int i = 0 ; i < queue_cur -> vertex -> neigh_num ; i++){
			for(int k = 0 ; k < count ; k++)
				if(queue_cur -> vertex -> neighbors[i] -> number == marked[k]){
					found = 1;
					break;
				}
			if( found == 0 )
				if( queue_cur -> vertex -> max_flow[i] > queue_cur -> vertex -> cur_flow[i] ){
					queue_tail = push(queue_tail , queue_cur ,i);
					marked[count] = queue_tail -> vertex -> number;
					count++;
					if(queue_tail -> vertex -> number == sink_num)
						break;
				}
			found = 0;
		}
		if(queue_tail -> vertex -> number == sink_num)
			break;
		queue_cur = queue_cur -> next;
		if(queue_cur == NULL){
			return(NULL);
		}
	}
	queue_head = find_path(queue_head , queue_tail , marked);
	free(marked);
	return(queue_head);
}

int minimal_flow( list *queue_head)
{
	list *cur = queue_head;
	int min_flow = 0;
	int flow = 0;
	for(int i = 0 ; i < queue_head -> vertex -> neigh_num ; i++)
		if(queue_head -> vertex -> neighbors[i] == queue_head -> next -> vertex){
			min_flow = (queue_head -> vertex -> max_flow[i]) - (queue_head -> vertex -> cur_flow[i]);
			break;
		}
	cur = cur -> next;
	while(cur -> next != NULL){
		for(int i = 0 ; i < cur -> vertex -> neigh_num ; i++)
			if(cur -> vertex -> neighbors[i] == cur -> next -> vertex){
				flow = (cur -> vertex -> max_flow[i]) - (cur -> vertex -> cur_flow[i]);
				break;
			}
		if(flow < min_flow)
			min_flow = flow;
		cur = cur -> next;
	}
	return(min_flow);
}

void fill_path( list *queue_head)
{
	int min = minimal_flow(queue_head);
	list *cur = queue_head;
	while(cur -> next != NULL){
		for(int i = 0 ; i < cur -> vertex -> neigh_num ; i++)
			if(cur -> vertex -> neighbors[i] == cur -> next -> vertex){
				cur -> vertex -> cur_flow[i] += min;
				cur -> next -> vertex -> sum_flow += min;
				for(int k = 0 ; k < cur -> next -> vertex -> neigh_num ; k++)
					if(cur -> next -> vertex -> neighbors[k] == cur -> vertex){
						cur -> next -> vertex -> cur_flow[k] -= min;
						if(cur -> next -> vertex -> cur_flow[k] < 0)
							cur -> next -> vertex -> cur_flow[k] = 0;
					}
			}
		cur = cur -> next;
	}
}

int spot_flow(graph **vertices , int ver_num , int sink_num , int source_num)
{
	list *queue_head = NULL;
	while(1){
		queue_head = graph_detour( vertices , ver_num , source_num , sink_num);
		if(queue_head == NULL)
			break;
		list *cur = queue_head;
		fill_path(queue_head);
	}
	for(int i = 0 ; i < ver_num ; i++)
		if(vertices[i] -> number == sink_num){
			return(vertices[i] -> sum_flow);
		}
	return(0);
}

int main(int argc , char *argv[])
{
	FILE *input = fopen(argv[1] , "r");
	int ver_num = 0;
	graph **vertices = build_graph(input, &ver_num);
	printf("enter source vertex number\n");
	int source_num = 0;
	scanf("%d", &source_num);
	printf("enter sink vertex number\n");
	int sink_num = 0;
	scanf("%d", &sink_num);
	printf("\n");
	if(source_num == sink_num){
		printf("error\n");
		printf("source = sink\n");
		_Exit(1);
	}
	int flow = spot_flow(vertices , ver_num , sink_num , source_num);
	printf("flow power = %d\n", flow );
	free_vertices(vertices , ver_num);
}
