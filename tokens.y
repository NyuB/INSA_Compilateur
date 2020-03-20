%{
#include <stdio.h>
#include "name_list.h"
#define MAX 63
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

OP: T_ADD {$$=1;}
    |T_SUB {$$=3;}
    |T_MUL {$$=2;}
    |T_DIV {$$=4;};

EXPR : T_NAME {printf("NAME-EXPR\n");int index=nli_contains(namel,"NONE");printf("Text ici");$$=index;}
    |T_POPEN EXPR T_PCLOSE {$$=$2;}
    |T_INT {printf("INT-EXPR\n");int index=nli_contains(namel,"NONE");$$=index;}
    |EXPR OP EXPR {printf("EXPR\n");int a=$1; int op=$2; int b=$3; int index=max;ASM_write(op);ASM_write(max);ASM_write(a);ASM_write(b);ASM_endline();max--;$$=index;}
    |T_SUB EXPR {printf("MIN-EXPR\n");int index=max;int a=0; int op=$1; int b=$2;ASM_write(op);ASM_write(max);ASM_write(a);ASM_write(b);ASM_endline();max--;$$=index; }
    |T_ADD EXPR {printf("PLUS-EXPR\n");int index=max;int a=0; int op=$1; int b=$2;ASM_write(op);ASM_write(max);ASM_write(a);ASM_write(b);ASM_endline();max--;$$=index;};

PRINT : T_PRINTF T_POPEN EXPR T_PCLOSE {ASM_write(T_PRINTF);};

T_NAMELIST : T_NAME
           |T_NAME T_COMMA T_NAMELIST;

declare_assignement : T_VAR T_NAME T_EQ EXPR{printf("DCLR-ASSIGN\n");};
declaration : T_VAR T_NAMELIST{printf("DECLARATION");min++;nli_append(namel,"NONE");};
assignement : T_NAME T_EQ EXPR {printf("ASSIGN_NAME");ASM_write(5),ASM_write($1),ASM_write($3);ASM_endline();}
            




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

int main(void){
    const char * filename = "./assembly.asm";
    namel = nli_empty();
    file = fopen(filename,"w");
    yyparse();
    fclose(file);

    return 0;
}
