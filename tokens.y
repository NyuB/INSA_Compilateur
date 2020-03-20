%{
#include <stdio.h>
#include "name_list.h"
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
    FILE * file;
    int min =0;
    int max = MAX;
    name_list * namel;
%}



%token T_MAIN T_PRINTF T_CONST T_VAR T_ADD T_SUB T_MUL T_DIV T_EQ T_POPEN T_PCLOSE T_AOPEN T_ACLOSE T_COPEN T_CCLOSE T_INT T_NAME T_SEP T_COMMA
%left T_MUL T_DIV
%left T_ADD T_SUB
%union : {int i; char * s};

%%
start : T_MAIN T_POPEN T_PCLOSE BLOC {printf("MAIN\n");};


BLOC : T_AOPEN BODY T_ACLOSE {printf("{BODY}");};

BODY :
    |CMD
    |CMD T_SEP BODY ;

CMD : declare_assignement
    |declaration
    |assignement
    |PRINT;

OP: T_ADD {$$=ASM_ADD;}
    |T_SUB {$$=ASM_SUB;}
    |T_MUL {$$=ASM_MUL;}
    |T_DIV {$$=ASM_DIV;}
    ;

EXPR : T_NAME {printf("NAME-EXPR\n"); fprintf(file,"TOK %d\n",yylval);int index = nli_contains(namel, "NONE"); int cp = max; printf("Text ici");ASM_instruction(ASM_COP, max, $1, ASM_NULL); max--; $$ = cp;}//On check l'appartenance au namespace puis copie la valeur de la variable dans la pile
    |T_POPEN EXPR T_PCLOSE {$$=$2;}
    |T_INT {printf("INT-EXPR\n"); int index = max; max--; $$=index;}//Idem on copie dans la pile
    |EXPR {$$ = $1;} OP EXPR {printf("EXPR\n"); int a = $2;int op = $3; int b = $4; max += 1;int index = max + 1; ASM_instruction(op, index, a, b); $$ = index;}/*On pop 2 elements de la pile et on en push 1 */
    |T_SUB EXPR {printf("MIN-EXPR\n");ASM_instruction(ASM_SUB, max, $1, $1); ASM_instruction(ASM_SUB, $2, max, $2); $$ = $2;}//On place 0 au sommet de la pile, on lui soustrait la valeur de expr qui remplace ensuite expr
    |T_ADD EXPR {printf("PLUS-EXPR\n");$$ = $2;}
    ;

PRINT : T_PRINTF T_POPEN EXPR T_PCLOSE {ASM_write(T_PRINTF);};

T_NAMELIST : T_NAME
           |T_NAME T_COMMA T_NAMELIST;

declare_assignement : T_VAR T_NAME T_EQ EXPR{printf("DCLR-ASSIGN\n");ASM_instruction(ASM_COP,$2,$4ASM_NULL);};
declaration : T_VAR T_NAMELIST{printf("DECLARATION"); min++; nli_append(namel,"NONE");};
assignement : T_NAME T_EQ EXPR {printf("ASSIGN_NAME"); max++; ASM_instruction(ASM_COP, $1, $3, ASM_NULL);}




%%
int yyerror(char *msg){
    fprintf(stderr, "err: %s\n", msg);
    return 1;
}





void ASM_write(int instruction){
    fprintf(file,"%d ",instruction);
}
void ASM_endline(){
    fprintf(file,"\n");
}
void ASM_instruction(int code, int dest,int left,int right){
	fprintf(file,"%d %d %d %d\n",code,dest,left,right);
}

int main(void){
    const char * filename = "./assembly.asm";
    namel = nli_empty();
    file = fopen(filename,"w");
    yyparse();
    fclose(file);

    return 0;
}
