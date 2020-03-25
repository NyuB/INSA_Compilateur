%{
#include <stdio.h>
#include "name_list.h"
#include "ast.h"
#define MAX 63
#define ASM_ADD 1
#define ASM_MUL 2
#define ASM_SUB 3
#define ASM_DIV 4
#define ASM_COP 5
#define ASM_NULL -1
%}


%{
    int yydebug=1;
    ast * tree;
%}

%left T_ADD
%left T_SUB
%left T_MUL T_DIV

%token T_MAIN T_PRINTF T_CONST T_VAR T_ADD T_SUB T_MUL T_DIV T_EQ T_POPEN T_PCLOSE T_AOPEN T_ACLOSE T_COPEN T_CCLOSE T_INT T_NAME T_SEP T_COMMA


%union {ast_node* n;ast_node_list * noli; name_list * nli; int i; char * s}
%type <s> T_NAME 
%type <n> T_MAIN T_PRINTF T_CONST T_VAR T_EQ T_POPEN T_PCLOSE T_AOPEN T_ACLOSE T_COPEN T_CCLOSE T_SEP T_COMMA EXPR CMD PRINT BLOC assignement declaration declare_assignement  
%type <i>  T_ADD T_SUB T_MUL T_DIV T_INT
%type <noli> BODY T_NAMELIST
%%

start : T_MAIN T_POPEN T_PCLOSE BLOC {printf("MAIN\n");tree = ast_new($4);}
	;


BLOC : T_AOPEN BODY T_ACLOSE {printf("{BODY}");$$=ast_node_seq($2);}
	;

BODY :
    |CMD {ast_node_list * l = ast_node_list_empty();ast_node_list_append(l,$1);$$=l;}
    |CMD T_SEP BODY {ast_node_list * l = $3; ast_node_list_prepend(l,$1);$$=l;}
    ;

CMD : declare_assignement
    |declaration
    |assignement
    |PRINT
    ;

EXPR : T_NAME {printf("NAME-EXPR\n"); $$ = ast_var($1); }//Noeud feuille variable
    |T_POPEN EXPR T_PCLOSE {$$ = $2;} //Propagation du noeud
    |T_INT {printf("INT-EXPR\n"); $$ = ast_int($1);}//Noeud feuille constante
    |EXPR T_ADD EXPR {printf("EXPR\n"); $$ = ast_math(ASM_ADD, $1, $3);}//Noeud opération
    |EXPR T_SUB EXPR {printf("EXPR\n"); $$ = ast_math(ASM_SUB, $1, $3);}//Noeud opération
    |EXPR T_MUL EXPR {printf("EXPR\n"); $$ = ast_math(ASM_MUL, $1, $3);}//Noeud opération
    |EXPR T_DIV EXPR {printf("EXPR\n"); $$ = ast_math(ASM_DIV, $1, $3);}//Noeud opération
    |T_SUB EXPR {printf("MIN-EXPR\n");$$ = ast_math(ASM_SUB,ast_int(0),$2);}//Construction d'une négation par 0-expr
    |T_ADD EXPR {printf("PLUS-EXPR\n");$$ = $2;}
    ;

PRINT : T_PRINTF T_POPEN EXPR T_PCLOSE
      ;

T_NAMELIST : T_NAME {ast_node_list * l = ast_node_list_empty(); ast_node_list_append(l, ast_declare($1));$$ = l; }
           |T_NAME T_COMMA T_NAMELIST {ast_node_list * l = $3; ast_node_list_prepend(l, ast_declare($1));$$ = l; }
           ;

declare_assignement : T_VAR T_NAME T_EQ EXPR{
		printf("DCLR-ASSIGN\n");
		ast_node * dcl = ast_declare($2);
		ast_node * var = ast_var($2);
		ast_node * aff = ast_affect(var,$4);
		ast_node_list * li = ast_node_list_empty();
		ast_node_list_append(li,dcl);
		ast_node_list_append(li,aff);
		$$ = ast_node_seq(li);
		}
		| T_CONST T_NAME T_EQ EXPR {$$ = ast_declare_const($2,$4);}
		;
declaration : T_VAR T_NAMELIST
			{
			printf("DECLARATION"); $$ = ast_node_seq($2);
			}
	;
assignement : T_NAME T_EQ EXPR {printf("ASSIGN_NAME");$$ = ast_affect(ast_var($1),$3); }


%%
int yyerror(char *msg){
    fprintf(stderr, "err: %s\n", msg);
    return 1;
}

int main(void){
    const char * filename = "./assembly.asm";
    yyparse();
    ast_build(tree,filename,64);
    return 0;
}
