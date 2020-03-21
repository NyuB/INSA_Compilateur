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



%token T_MAIN T_PRINTF T_CONST T_VAR T_ADD T_SUB T_MUL T_DIV T_EQ T_POPEN T_PCLOSE T_AOPEN T_ACLOSE T_COPEN T_CCLOSE T_INT T_NAME T_SEP T_COMMA
%left T_MUL T_DIV
%left T_ADD T_SUB
%union {ast_node* n;ast_node_list * noli; name_list * nli; int i; char * s}
%type <s> T_NAME T_INT
%type <n> T_MAIN T_PRINTF T_CONST T_VAR T_EQ T_POPEN T_PCLOSE T_AOPEN T_ACLOSE T_COPEN T_CCLOSE T_SEP T_COMMA EXPR CMD PRINT BLOC assignement declaration declare_assignement  
%type <i> OP T_ADD T_SUB T_MUL T_DIV
%type <noli> BODY T_NAMELIST
%%

start : T_MAIN T_POPEN T_PCLOSE BLOC {printf("MAIN\n");tree = ast_new($4);}
	;


BLOC : T_AOPEN BODY T_ACLOSE {printf("{BODY}");$$=ast_node_seq($2);}
	;

BODY :
    |CMD {ast_node_list * l = ast_node_list_empty();ast_node_list_append(l,$1);$$=l;}
    |CMD T_SEP BODY {ast_node_list * l = $3; ast_node_list_append(l,$1);$$=l;}
    ;

CMD : declare_assignement
    |declaration
    |assignement
    |PRINT
    ;

OP: T_ADD {$$=ASM_ADD;}
    |T_SUB {$$=ASM_SUB;}
    |T_MUL {$$=ASM_MUL;}
    |T_DIV {$$=ASM_DIV;}
    ;

EXPR : T_NAME {printf("NAME-EXPR\n"); $$ = ast_var($1);}//Noeud feuille variable
    |T_POPEN EXPR T_PCLOSE {$$=$2;} //Propagation du noeud
    |T_INT {printf("INT-EXPR\n");$$=ast_var($1);}//Noeud feuille constante
    |EXPR OP EXPR {printf("EXPR\n"); $$ = ast_math($2,$1,$3);}//Noeud opération
    |T_SUB EXPR {printf("MIN-EXPR\n");}
    |T_ADD EXPR {printf("PLUS-EXPR\n");$$ = $2;}
    ;

PRINT : T_PRINTF T_POPEN EXPR T_PCLOSE
      ;

T_NAMELIST : T_NAME {ast_node_list * l = ast_node_list_empty();ast_node_list_append(l,ast_declare($1));$$=l;}
           |T_NAME T_COMMA T_NAMELIST {ast_node_list * l = $3; ast_node_list_append(l,ast_declare($1));$$=l;}
           ;

declare_assignement : T_VAR T_NAME T_EQ EXPR{printf("DCLR-ASSIGN\n");$$ = ast_affect(ast_declare($2),$3);};
declaration : T_VAR T_NAMELIST
			{
			printf("DECLARATION"); $$ = ast_node_seq($2);
			}
	;st * ast_new(ast_node * root);
assignement : T_NAME T_EQ EXPR {printf("ASSIGN_NAME");ast_affect(ast_var($1),$3); }


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