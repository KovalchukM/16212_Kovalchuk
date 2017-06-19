#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

enum LexType {
NUMBER, OP, BRACKET
};

enum Operation {
PLUS, MINUS, DIV, MUL
};

enum Bracket {
BR_OPEN, BR_CLOSE
};

union Value {
int num;
enum Operation op;
enum Bracket br;
};

typedef struct lexem {
enum LexType type;
union Value value;
} LEXEM;

typedef struct list {
LEXEM lex;
struct list *next;
} NODE;

NODE *node_create(char s)
{
	NODE *head = (NODE*)malloc(sizeof(NODE));
	switch (s){
		case '+':
		head->lex.type = OP;
		head->lex.value.op = PLUS;
		return (head);
		case '-':
		head->lex.type = OP;
		head->lex.value.op = MINUS;
		return (head);
		case '*':
		head->lex.type = OP;
		head->lex.value.op = MUL;
		return (head);
		case '/':
		head->lex.type = OP;
		head->lex.value.op = DIV;
		return (head);
		case '(':
		head->lex.type = BRACKET;
		head->lex.value.br = BR_OPEN;
		return (head);
		case ')':
		head->lex.type = BRACKET;
		head->lex.value.br = BR_CLOSE;
		return (head);
		}
}

int is_nymeric(char s)
{
	char *nymeric = "0123456789";
	for(int i = 0; i < 10 ; i++){
		if ( s == nymeric[i])
			return(1);
	}
	return(0);
}

NODE *list_create(FILE *input)
{
	NODE *head = NULL;
	int s = fgetc(input);
	int n = 0;
	if ( is_nymeric(s) == 1){
		rewind(input);
		fscanf(input, "%d", &n);
		head = (NODE*)malloc(sizeof(NODE));
		head->lex.type = NUMBER;
		head->lex.value.num = n;
	}
	else
		head = node_create(s);
	NODE *tail = head;
	NODE *temp = NULL;
	while(1){
		s = fgetc(input);
		if ( s = ' ')
			s = fgetc(input);
		if (s == EOF)
			break;
		if ( is_nymeric(s) == 1){
			fseek(input , -1 , SEEK_CUR);
			fscanf(input, "%d", &n);
			temp = (NODE*)malloc(sizeof(NODE));
			temp->lex.type = NUMBER;
			temp->lex.value.num = n;
		}
		else{
			temp = node_create(s);
		}
		tail -> next = temp;
		temp -> next = NULL;
		tail = temp;
	}
	if ((tail->lex.type == OP) || ((tail->lex.value.br == BR_OPEN))){
		printf("invalid input\n");
		_Exit(1);
	}
	return(head);
}

void list_print(NODE *head)
{
	NODE *temp = head;
	while (temp != NULL){
		if( temp -> lex.type == NUMBER)
			printf("%d ", temp -> lex.value.num );
		if( temp -> lex.type == OP){
			switch(temp -> lex.value.op){
				case 0:
				printf("+ ");
				break;
				case 1:
				printf("- ");
				break;
				case 2:
				printf("/ ");
				break;
				case 3:
				printf("* ");
				break;
			}
		}
		if( temp -> lex.type == BRACKET){
			if (temp->lex.value.br == BR_CLOSE)
				printf(")");
			if (temp->lex.value.br == BR_OPEN)
				printf("(");
		}
		temp = temp -> next;
	}
	printf("\n");
}

NODE *rewrite(NODE *head)
{
	if( (head->lex.value.br == BR_CLOSE) || (head->lex.type == OP)){
		printf("invalid input\n");
		_Exit(1);
	}
	NODE *steak_head = NULL;
	NODE *temp = NULL;
	while(head->lex.value.br == BR_OPEN){
		if( steak_head == NULL){
			steak_head = head;
			head = head -> next;
			steak_head -> next = NULL;
		}
		else{
			temp = head;
			head = head -> next;
			temp -> next = steak_head;
			steak_head = temp;
		}
	}
	if( (head->lex.value.br == BR_CLOSE) || (head->lex.type == OP)){
		printf("invalid input\n");
		_Exit(1);
	}
	NODE *new_note_head = head;
	head = head -> next;
	new_note_head -> next = NULL;
	NODE *new_note_tail = new_note_head;
	while((head != NULL) || (steak_head != NULL)){
		/*printf("head   ");
		list_print(head);
		printf("steak  ");
		list_print(steak_head);
		printf("pol  ");
		list_print(new_note_head);
		printf("\n");*/
		if((head != NULL) && (head->lex.value.num == 0) && (steak_head != NULL) && (steak_head ->lex.value.op = DIV)){
			printf("invalid input\n");
			_Exit(1);
		}
		if ((head != NULL) && (head->lex.type == OP) && (steak_head != NULL) && (steak_head->lex.type == OP)){
			printf("invalid input\n");
			_Exit(1);
		}
		if (head == NULL){
			while(steak_head != NULL){
				temp = steak_head;
				steak_head = steak_head -> next;
				new_note_tail -> next = temp;
				new_note_tail = temp;
				new_note_tail -> next = NULL;
			}
			break;
		}
		if( (head -> lex.type == NUMBER)){
			temp = head;
			head = head -> next;
			new_note_tail -> next = temp;
			new_note_tail = temp;
			new_note_tail -> next = NULL;
		}
		if( steak_head == NULL){
			if((head->lex.type == BRACKET) && (head->lex.value.br == BR_CLOSE)){
				printf("invalid input\n");
				_Exit(1);
			}
			steak_head = head;
			if (head -> next != NULL)
				head = head -> next;
			steak_head -> next = NULL;
		}
		if((head != NULL) && (head->lex.type == OP )){
			if(steak_head -> lex.type == OP){
				while(((steak_head->lex.value.op > 1) && (head->lex.value.op < 2)) || 
				((steak_head->lex.value.op > 1) && (head->lex.value.op > 1)) || 
				((steak_head->lex.value.op < 2) && (head->lex.value.op < 2))){
					temp = steak_head;
					steak_head = steak_head -> next;
					temp -> next = NULL;
					new_note_tail -> next = temp;
					new_note_tail = temp;
					if ((steak_head == NULL) || (steak_head -> lex.type != OP))
						break;
					}
				if((steak_head != NULL ) && (steak_head->lex.value.op < 2) && (head->lex.value.op > 1)){
					temp = head;
					head = head -> next;
					temp -> next = steak_head;
					steak_head = temp;
				}
			}
			if((steak_head != NULL) && (steak_head -> lex.type == BRACKET)){
				temp = head;
				head = head -> next;
				temp -> next = steak_head;
				steak_head = temp;
			}
		}
		if((head != NULL) && (head->lex.type == BRACKET)){
			if(head->lex.value.br == BR_CLOSE){
				temp = head;
				head = head -> next;
				free(temp);
				while (steak_head -> lex.type != BRACKET){
					if( steak_head -> next == NULL){
						printf("invalid input\n");
						_Exit(1);
					}
					temp = steak_head;
					steak_head = steak_head -> next;
					new_note_tail -> next = temp;
					new_note_tail = temp;
					new_note_tail -> next = NULL;
				}
				temp = steak_head;
				if(steak_head -> next != NULL)
					steak_head = steak_head -> next;
				else
					steak_head = NULL;
				free(temp);
			}
			else{
				temp = head;
				head = head -> next;
				temp -> next = steak_head;
				steak_head = temp;
			}
		}
	}
	if (new_note_tail->lex.type == BRACKET){
		printf("invalid input\n");
		_Exit(1);
	}
	return(new_note_head);
}

void translation(NODE *head)
{
	list_print(head);
	printf("\n");
	head = rewrite(head);
	list_print(head);
}

int main (int argc , char *argv[])
{
	FILE *input = fopen(argv[1], "r");
	NODE *head = list_create(input);
	fclose(input);
	translation(head);
	return(0);
}

//   825*+132*+4-/
//(8+2*5)/(1+3*2-4)