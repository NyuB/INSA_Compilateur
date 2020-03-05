%{
#include <stdio.h>
%}

%token T_MAIN T_PRINTF T_CONST T_VAR T_ADD T_SUB T_MUL T_DIV T_EQ T_POPEN T_PCLOSE T_AOPEN T_ACLOSE T_COPEN T_CCLOSE T_INT T_NAME T_SEP

%%
start : T_MAIN T_POPEN T_PCLOSE BODY;
BODY : T_AOPEN BLOC T_ACLOSE;
BLOC : 
       |CMD
       |CMD T_SEP BLOC ;
CMD : assignement;
assignement : T_VAR T_NAME T_EQ T_INT;
%%
int yyerror(char *msg){
    fprintf(stderr, "err: %s\n", msg);
    return 1;
}
int main(void){
    yyparse();
    return 0;
}
