#include <stdio.h>
#include <stdlib.h>

typedef struct graph {
	int number;
	int neigh_num;
	struct graph **neighbors;
	float *lenght;
} graph;

typedef struct tree{
	float lenght;
	struct graph* vertex;
	struct tree* parent;
	struct tree* child;
	struct tree* sibling;
} tree;

void free_vertices(graph **vertices , int ver_num)
{
	for(int i = 0 ; i < ver_num ; i++)
		free(vertices[i]);
	free(vertices);
}

graph *seek_vertex(int id , graph **vertices , int ver_num)
{
	for(int i = 0 ; i < ver_num ; i++)
		if (vertices[i] -> number == id)
			return(vertices[i]);
	return(NULL);
}

int is_used( graph *vertex , tree *root)
{
	tree *cur = root;
	while(cur -> parent != NULL){
		if(cur -> vertex == vertex)
			return(1);
		cur = cur -> parent;
	}
	if(cur -> vertex == vertex)
		return(1);
	return(0);
}

tree *make_node_sibling( tree *root , tree *cur , int id )
{
	tree *temp = (tree*)malloc(sizeof(tree));
	temp -> parent = root;
	temp -> vertex = root -> vertex -> neighbors[id];
	temp -> lenght = (root -> lenght) + (root -> vertex -> lenght[id]);
	temp -> sibling = NULL;
	temp -> child = NULL;
	cur -> sibling = temp;
	//printf("sib %f\n", temp -> lenght );
	return(temp);
}

tree *make_node_childs( tree *root , graph *ver_2 )
{
	if(root -> vertex -> neighbors[0] == NULL)
		return(NULL);
	if(root -> vertex == ver_2)
		return(NULL);
	int n = 0;
	while(is_used(root -> vertex -> neighbors[n] , root)){
		n++;
		if(root -> vertex -> neighbors[n] == NULL)
			return(NULL);
	}
	tree *new = (tree*)malloc(sizeof(tree));
	new -> parent = root;
	new -> vertex = root -> vertex -> neighbors[n];
	new -> lenght = (root -> lenght) + (root -> vertex -> lenght[n]);
	new -> sibling = NULL;
	new -> child = NULL;
	tree *cur = new;
	//printf("child %f\n", cur -> lenght );
	for(int i = n ; i < root -> vertex -> neigh_num ; i++){
		if(!is_used(root -> vertex -> neighbors[i] , new))
			cur = make_node_sibling(root , cur , i);
	}
	return(new);
}

tree *build_tree(tree *root , graph *ver_2)
{
	if (root -> vertex == ver_2)
		return(NULL);
	root -> child = make_node_childs(root , ver_2);
	if(root -> sibling != NULL)
		build_tree(root -> sibling , ver_2);
	if(root -> child != NULL)
		build_tree(root -> child , ver_2);
	return(root);
}

tree *make_root(graph **vertices , int ver_num , int id_1 , int id_2)
{
	graph *ver_1 = seek_vertex(id_1 , vertices , ver_num);
	graph *ver_2 = seek_vertex(id_2 , vertices , ver_num);
	if (ver_1 == NULL || ver_2 == NULL){
		printf("wrong vertex\n");
		_Exit(1);
	}
	tree *root = (tree*)malloc(sizeof(tree));
	root -> lenght = 0;
	root -> vertex = ver_1;
	root -> parent = NULL;
	root -> sibling = NULL;
	root -> child = NULL;
	root = build_tree(root , ver_2 );
	free(vertices);
	return(root);
}

float tree_detour(tree *root , int id_2)
{
	float s = -1;
	float c = -1;
	if(root -> vertex -> number == id_2){
		return(root -> lenght);
	}
	if(root -> sibling != NULL){
		s = tree_detour(root -> sibling , id_2);
	}
	if(root -> child != NULL){
		c = tree_detour(root -> child , id_2);
	}
	if( (s > c || s == -1) && c > 0){
		return(c);
	}
	return(s);

}

void free_tree(tree *root)
{
	if(root -> child != NULL)
		free_tree(root -> child);
	if(root -> sibling != NULL)
		free_tree(root -> sibling);
	free(root -> vertex -> neighbors);
	free(root -> vertex -> lenght);
	free(root -> vertex);
	free(root);
}

int main(int argc , char* argv[])
{
	FILE *input = fopen(argv[1] , "r");
	int ver_num = 0;
	graph **vertices = build_graph(input, &ver_num);
	printf("enter first vertex number\n");
	int id_1 = 0;
	scanf("%d", &id_1);
	printf("enter second vertex number\n");
	int id_2 = 0;
	scanf("%d", &id_2);
	printf("\n");
	if (id_1 == id_2){
		printf("lenght = 0");
		free_vertices(vertices , ver_num);
		_Exit(1);
	}
	tree *root = make_root(vertices , ver_num , id_1 , id_2);
	float lenght = tree_detour(root , id_2);
	if (lenght == -1)
		printf("path not found\n");
	else
		printf("lenght = %f", lenght );
	free_tree(root);
	return(0);
}
