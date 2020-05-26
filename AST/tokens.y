%{
#include <stdio.h>
#include "name_list.h"
#include "ast.h"
#include "ast_repr.h"
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

%token T_MAIN T_PRINTF T_CONST T_VAR T_ADD T_SUB T_MUL T_DIV T_EQ T_REF T_POPEN T_PCLOSE T_AOPEN T_ACLOSE T_COPEN T_CCLOSE T_INT T_NAME T_SEP T_COMMA T_WHILE T_IF T_ELSE


%union {ast_node* n;ast_node_list * noli; name_list * nli; int i; char * s;}
%type <s> T_NAME 
%type <n> T_MAIN T_PRINTF T_CONST T_VAR T_EQ T_POPEN T_PCLOSE T_AOPEN T_ACLOSE T_COPEN T_CCLOSE T_SEP T_COMMA EXPR REF UNREF CMD PRINT BLOC IF WHILE assignement declaration declare_assignement  
%type <i>  T_ADD T_SUB T_MUL T_DIV T_INT T_IF T_ELSE T_WHILE STARS
%type <noli> BODY NAMELIST 
%%

start : T_MAIN T_POPEN T_PCLOSE BLOC {printf("MAIN\n");tree = ast_new($4);}
	;


BLOC : T_AOPEN BODY T_ACLOSE {printf("{BODY}");$$=ast_node_seq($2);}
	;

BODY : {$$ = ast_node_list_empty();}
    |CMD {ast_node_list * l = ast_node_list_empty();ast_node_list_append(l,$1);$$=l;}
    |CMD T_SEP BODY {ast_node_list * l = $3; ast_node_list_prepend(l,$1);$$=l;}
    |T_SEP BODY {$$ = $2;}
    ;

IF : T_IF T_POPEN EXPR T_PCLOSE BLOC { $$ = ast_node_if($3,$5,NULL);}
	|T_IF T_POPEN EXPR T_PCLOSE BLOC T_ELSE BLOC {$$ = ast_node_if($3,$5,$7);} 
	;
WHILE : T_WHILE T_POPEN EXPR T_PCLOSE BLOC {$$ = ast_node_while($3,$5);}
	;

CMD : declare_assignement
    |declaration
    |assignement
    |PRINT
    |IF
    |WHILE
    ;
REF : T_REF T_NAME {printf("REF\n");$$ = ast_ref($2);}
	;

UNREF : STARS T_NAME {printf("UNREF\n");$$=ast_unref($2,$1);}
	;
STARS : T_MUL {$$=1;}
	|T_MUL STARS {$$=1+$2;}
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
    |REF {$$ = $1;}
    |UNREF {$$ = $1;}
    ;


PRINT : T_PRINTF T_POPEN EXPR T_PCLOSE {$$=ast_node_print($3);}
      ;

NAMELIST : T_NAME {ast_node_list * l = ast_node_list_empty(); ast_node_list_append(l, ast_declare($1));$$ = l; }
           |T_NAME T_COMMA NAMELIST {ast_node_list * l = $3; ast_node_list_prepend(l, ast_declare($1));$$ = l; }
           |T_NAME T_EQ EXPR {
           		ast_node * dcl = ast_declare($1);
				ast_node * var = ast_var($1);
				ast_node * aff = ast_affect(var,$3);
				ast_node_list * li = ast_node_list_empty();
				ast_node_list_append(li,dcl);
				ast_node_list_append(li,aff);
				$$ = li;

           	}
           |T_NAME T_EQ EXPR T_COMMA NAMELIST {
             	ast_node * dcl = ast_declare($1);
				ast_node * var = ast_var($1);
				ast_node * aff = ast_affect(var,$3);
				ast_node_list * li = ast_node_list_empty();
				ast_node_list_append(li,dcl);
				ast_node_list_append(li,aff);
				ast_node_list_prepend($5,ast_node_seq(li));
				$$ = $5;
           	}
           ;

declare_assignement : T_CONST T_VAR T_NAME T_EQ EXPR {$$ = ast_declare_aff_const($3,$5);}
		;
declaration : T_VAR NAMELIST {printf("DECLARATION"); $$ = ast_node_seq($2);}
			|T_CONST T_VAR T_NAME {printf("CONST DECLARATION");$$= ast_declare_const_void($3);}
	;
assignement : T_NAME T_EQ EXPR {printf("ASSIGN_NAME");$$ = ast_affect(ast_var($1),$3); }


%%
int yyerror(char *msg){
    fprintf(stderr, "err: %s\n", msg);
    return 1;
}

int main(int argc, char ** argv){
    const char * filename = "./assembly.asm";
    int size = (argc>1)?atoi(argv[1]):64;
    yyparse();
    ast_build(tree,filename,size);
    ast_display(tree);
    return 0;
}
