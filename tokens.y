%{
#include <stdio.h>
%}

%{
    int yydebug=1;
    FILE * file;
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

EXPR : T_NAME {printf("NAME-EXPR\n");$$=T_NAME;}
    |T_POPEN EXPR T_PCLOSE {$$=$2;}
    |T_INT {printf("INT-EXPR\n");$$=T_INT;}
    |EXPR OP EXPR {printf("EXPR\n");ASM_write($2);ASM_write(-1);ASM_write($1);ASM_write($3);ASM_endline();$$=-1;}
    |T_SUB EXPR {printf("MIN-EXPR\n");}
    |T_ADD EXPR {printf("PLUS-EXPR\n");};

PRINT : T_PRINTF T_POPEN EXPR T_PCLOSE {ASM_write(T_PRINTF);};

T_NAMELIST : T_NAME
           |T_NAME T_COMMA T_NAMELIST;

declare_assignement : T_VAR T_NAME T_EQ T_NAME {ASM_write(5); ASM_write($2); ASM_write($4);ASM_endline();}
                    |T_VAR T_NAME T_EQ EXPR{printf("DCLR-ASSIGN\n");ASM_write(5); ASM_write($2); ASM_write($4); ASM_endline();}
assignement : T_NAME T_EQ T_NAME{printf("ASSIGN_NAME");ASM_write(5),ASM_write($1),ASM_write($3);ASM_endline();}
            | T_NAME T_EQ EXPR{printf("ASSIGN\n");ASM_write(5),ASM_write($1),ASM_write($3);ASM_endline();};




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
    file = fopen(filename,"w");
    yyparse();
    fclose(file);

    return 0;
}
