#ifndef H_AST
#define H_AST

#include <stdio.h>
#include <stdlib.h>
#include "name_list.h"
#define AST_CODE_ADD 1
#define AST_CODE_MUL 2
#define AST_CODE_SUB 3
#define AST_CODE_DIV 4
#define AST_CODE_AFF 5
#define AST_CODE_DCL 6
#define AST_CODE_VAR 7
#define AST_CODE_SEQ 8


typedef struct ast_node{
	int code;//type du noeud pour l'interprétation
	void * content;//certains noeuds auront besoin de stocker une valeur
	int nb_childs;
	struct ast_node_list * childs;
}ast_node;

typedef struct ast ast;
struct ast{
	ast_node * root;
};

typedef struct ast_node_cell{
	ast_node * node;
	struct ast_node_cell * suiv;
}ast_node_cell;

typedef struct ast_node_list{
	ast_node_cell * start;
	ast_node_cell * end;
}ast_node_list;

ast_node * ast_declare(char * name);// 'int a'
ast_node * ast_math(int op, ast_node * left,ast_node * right);// expr +-/* expr
ast_node * ast_affect(ast_node * a,ast_node * b);// name = expr
ast_node * ast_var(char * name);// name
ast_node * ast_new_node(int code, void * content,int nb_childs,ast_node_list * childs); //function générique de création de noeud, a priori ne devrait pas être utilisé en externe, à placer dans le .c à la fin des tests
ast_node * ast_node_seq(ast_node_list * list);// noeud père de la liste de noeud donnée(sequence d'instructions, typiquement le body)
ast * ast_new(ast_node * root);
//Fonctions de manipulation des listes de noeuds
ast_node_list * ast_node_list_empty(void);
void ast_node_list_append(ast_node_list * list,ast_node * node);

void ast_build(ast * tree,const char * filename); //fonction principale, une fois l'AST construit on l'interprète et écrit le fichier ASM
#endif