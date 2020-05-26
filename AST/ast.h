#ifndef H_AST
#define H_AST

#include <stdio.h>
#include <stdlib.h>
#include<string.h>

//#include "name_list.h"
#include "scope.h"

#define AST_CODE_ADD 1
#define AST_CODE_MUL 2
#define AST_CODE_SUB 3
#define AST_CODE_DIV 4
#define AST_CODE_INF 9
#define AST_CODE_SUP 10
#define AST_CODE_EQ 11
#define AST_CODE_AFF 5
#define AST_CODE_DCL 6
#define AST_CODE_VAR 7
#define AST_CODE_SEQ 8
#define AST_CODE_CON 17
#define AST_CODE_INT 18
#define AST_CODE_IF 19
#define AST_CODE_WHILE 12
#define AST_CODE_PRINT 13
#define AST_CODE_REF 14
#define AST_CODE_UNREF 15
#define AST_CODE_CONST_VOID 16

#define AST_SEMANTIC_ERROR -2
#define AST_STACK_REQ_ERROR -3
#define AST_TYPESIZE_INT 1


typedef struct ast_node{
	int code;//type du noeud pour l'interprétation, à affecter parmi AST_CODE_XXX
	void * content;//certains noeuds auront besoin de stocker une valeur
	int nb_childs;
	struct ast_node_list * childs;
}ast_node;

typedef struct ast ast;
struct ast{
	ast_node * root;
};

//Structure de liste chaînée pour les noeuds
typedef struct ast_node_cell{
	ast_node * node;
	struct ast_node_cell * suiv;
}ast_node_cell;

typedef struct ast_node_list{
	ast_node_cell * start;
	ast_node_cell * end;
}ast_node_list;

//Fonctions de génération des noeuds (à exploiter dans le yacc pour construire l'AST avant interprétation)
ast_node * ast_new_node(int code, void * content,int nb_childs,ast_node_list * childs); //fonction générique de création de noeud, a priori ne devrait pas être utilisée telle quelle
ast_node * ast_declare(char * name);// Déclaration de variable, exemple : 'int a'
ast_node * ast_declare_const_void(char * name);// Déclaration de constante sans initialisation, exemple : 'const int a'
ast_node * ast_declare_aff_const(char * name, ast_node * expr);//declaration et affectation de constante
ast_node * ast_math(int op, ast_node * left,ast_node * right);//expression à deux opérandes
ast_node * ast_affect(ast_node * a,ast_node * b);// name = expr
ast_node * ast_var(char * name);//réference à une variable nommée, exemple a
ast_node * ast_int(int integer);//entier écrit "en clair"
ast_node * ast_ref(char * name);//référence addresse d'une variable nommée, exemple &a
ast_node * ast_unref(char * name, int level);//déréférencement d'une variable nommée, exemple *a. Le paramètre level donne le nombre de niveau de déréférencement(nombre d'éoiles)
ast_node * ast_node_seq(ast_node_list * list);//sequence d'instructions, exemple : int a; int b; a=b+2;
ast_node * ast_node_if(ast_node * condition, ast_node * true_body, ast_node * false_body);//noeud if à trois enfants /!\ bloc else dans le noeud false_body, mais sera écrit avant le true_body en assembleur
ast_node * ast_node_while(ast_node * condition, ast_node * body);//noeud while à deux enfants
ast_node * ast_node_print(ast_node * expression);//noeud printf avec un enfant

//Fonctions de manipulation des listes de noeuds
ast_node_list * ast_node_list_empty(void);//génère une liste vite
void ast_node_list_append(ast_node_list * list, ast_node * node);//ajoute un noeud au début de la liste
void ast_node_list_prepend(ast_node_list * list, ast_node * node);//ajoute un noeud en fin de liste

//Fonctions sur l'AST global
ast * ast_new(ast_node * root);//crée un ast ayant l'argument pour racine, a utiliser avant yyparse() pour initialiser l'AST à construire
void ast_build(ast * tree,const char * filename,int mem_size); //interprète l'AST et écrit le fichier ASM. A appeler après yyparse() Cette fonction appelle sur la racine des fonctions ast_[node]_build qui vont traiter récursivement chaque noeud(voir ast.c pour le détail de ces fonctions)

#endif