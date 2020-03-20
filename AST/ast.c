#include "ast.h"




ast_node * ast_new_node(int code, void * content,int nb_childs,ast_node_list * childs){
	ast_node * res = (ast_node *)malloc(1*sizeof(ast_node));
	res->code=code;
	res->nb_childs = nb_childs;
	res->childs = childs;
	return res;
}


ast_node_list * ast_node_list_empty(){
	ast_node_list * res = (ast_node_list *)malloc(1*sizeof(ast_node_list));
	res->start = NULL;
	res->end = NULL;
	return res;
}

void ast_node_list_append(ast_node_list * list,ast_node * node){
	ast_node_cell * new = (ast_node_cell *)malloc(1*sizeof(ast_node_cell));
	new->node = node;
	new->suiv=NULL;
	ast_node_cell * aux = list->end;
	if(aux==NULL){
		list->start = new;
	}
	else{
		list->end->suiv = new;
	}
	list->end = new;
}

ast_node * ast_var(char * name){
	return ast_new_node(AST_CODE_VAR,(void *)name,0,NULL);
}

ast_node * ast_declare(char * name){
	return ast_new_node(AST_CODE_DCL,(void*)name,0,NULL);
}

ast_node * ast_math(int op, ast_node * left,ast_node * right){
	ast_node_list * list = ast_node_list_empty();
	ast_node_list_append(list,left);
	ast_node_list_append(list,right);
	return ast_new_node(op,NULL,2,list);
}

ast_node * ast_affect(ast_node * left,ast_node * right){
	if(left->code!=AST_CODE_VAR){
		printf("Invalid left operand for affectation\n");
		exit(-1);
	}
	ast_node_list * list = ast_node_list_empty();
	ast_node_list_append(list,left);
	ast_node_list_append(list,right);

	return ast_new_node(AST_CODE_AFF,NULL,2,list);
}
int ast_node_list_length(ast_node_list * list){
	int res=0;
	ast_node_cell * aux = list->start;
	while(aux!=NULL){
		res++;
		aux=aux->suiv;
	}
	return res;
}

ast_node * ast_node_seq(ast_node_list * list){
	return ast_new_node(AST_CODE_SEQ,NULL,ast_node_list_length(list),list);
}

void ast_write(FILE * file,const char * op,int destAddr,int leftAddr,int rightAddr){
    fprintf(file,"%s %d %d %d\n",op,destAddr,leftAddr,rightAddr);
}

ast * ast_new(ast_node * root){
	ast * tree = (ast *)malloc(1*sizeof(ast));
	tree->root = root;
	return tree;
}

void ast_node_build(ast_node * node, name_list * var_list,int * left_addr_min,int * right_addr_max,FILE * file);

void ast_math_build(const char * op,ast_node * node, name_list * var_list,int * left_addr_min,int * right_addr_max,FILE * file){
	ast_node_cell * left = node->childs->start;
	ast_node_cell * right = left->suiv;
	int leftAddr;
	int rightAddr;
	if(left->node->code == AST_CODE_VAR){
		leftAddr = nli_contains(var_list,(char *)(left->node->content));
	}

	else{
		ast_node_build(left->node,var_list,left_addr_min,right_addr_max,file);
		leftAddr = *((int*)(left->node->content));
	}

	if(left->node->code == AST_CODE_VAR){
		rightAddr = nli_contains(var_list,(char *)(right->node->content));
	}

	else{
		ast_node_build(right->node,var_list,left_addr_min,right_addr_max,file);
		rightAddr = *((int*)(right->node->content));
	}

	ast_write(file,op,*right_addr_max,leftAddr,rightAddr);
	*right_addr_max--;

}

void ast_node_build(ast_node * node, name_list * var_list,int * left_addr_min,int * right_addr_max,FILE * file){
	switch(node->code){
		case AST_CODE_ADD:
			ast_math_build("ADD",node,var_list,left_addr_min,right_addr_max,file);
			break;
		case AST_CODE_MUL:
			ast_math_build("MUL",node,var_list,left_addr_min,right_addr_max,file);
			break;
		case AST_CODE_SUB:
			ast_math_build("SUB",node,var_list,left_addr_min,right_addr_max,file);
			break;
		case AST_CODE_DIV:
			ast_math_build("DIV",node,var_list,left_addr_min,right_addr_max,file);
			break;
		case AST_CODE_AFF:
			break;
		case AST_CODE_DCL:
			break;

		case AST_CODE_SEQ:
			break;

		case AST_CODE_VAR:
			break;

		default:
			break;
	}
}
void ast_build(ast * tree,const char * filename){
	FILE * file = fopen(filename,"w");
	name_list * vars = nli_empty();
	int minMem =0;
	int maxMem = 200;
	ast_node_build(tree->root,vars,&minMem,&maxMem,file);
	fclose(file);
}





