#include "ast_repr.h"
#define NEWLINE -1
typedef struct noderepr{
	ast_node * node;
	int id;
	int father_id;
	struct noderepr * suiv;
}noderepr;

typedef struct qrepr{
	noderepr * head;
	noderepr * tail;
}qrepr;

void q_add(qrepr * q,noderepr * new){
	if(q->head==NULL){
		q->head = new;
	}
	else{
		q->tail->suiv = new;
	}
	q->tail = new;
}

void q_display(qrepr * q){
	noderepr * aux = q->head;
	while(aux!=NULL){
		printf("=> (%d %d %d)",aux->id,aux->node->code,aux->father_id);
		aux=aux->suiv;
	}
	printf("\n");
}

ast_node * node_newline(){
	return ast_new_node(NEWLINE,NULL,0,NULL);
}


noderepr * new_noderepr(ast_node * node,int id,int father_id){
	noderepr * new = (noderepr *)malloc(1*sizeof(noderepr));
	new->node = node;
	new->id=id;
	new->father_id=father_id;
	new->suiv=NULL;
	return new;
}

noderepr * repr_newline(){
	return new_noderepr(node_newline(),NEWLINE,NEWLINE);
}



void ast_display(ast * tree){
	printf("%d\n",tree->root->code);
	qrepr * q = (qrepr *)malloc(1*sizeof(qrepr));
	int index=0;
	q->head=NULL;
	q->tail=NULL;
	noderepr * item = new_noderepr(tree->root,0,-1);
	noderepr * newline = repr_newline();
	noderepr * new;
	q_add(q,item);
	q_add(q,newline);
	printf("Starting AST display\n");
	printf("Format : [ID | Type | FatherID]\n");
	while(q->head != NULL){
		//q_display(q);
		item = q->head;
		q->head = q->head->suiv;
		//printf("Popping %d\n",index);
		if(item->node->code==NEWLINE){
			printf("\n");
		}
		
		else{
			printf("   [%d|%d|%d]",item->id,item->node->code,item->father_id);
			if(item->node->childs != NULL){
				ast_node_cell * cursor = item->node->childs->start;
				while(cursor!=NULL){
					index++;
					new = new_noderepr(cursor->node,index,item->id);
					q_add(q,new);
					cursor = cursor->suiv;
				}
			}
			
			if(q->head->node->code == NEWLINE){
				q_add(q,repr_newline());
			}
		}
		free(item);
	}
}