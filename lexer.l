%{
    #include "y.tab.h"
%}
LETTER [a-zA-Z]
DIGIT [0-9]
INT {DIGIT}+
PRINTF printf
NAME {LETTER}({LETTER}|_|{DIGIT})*
SPACE [ \t\b]+
RET [\n]
SEP ;
CONST const
VAR int
ADD \+
SUB \-
MUL \*
DIV \/
EQ =
POPEN [(]
PCLOSE [)]
AOPEN \{
ACLOSE \}
COPEN \[
CCLOSE \]
COMMA ,

MAIN main


%%

{MAIN} {printf("t_main");return T_MAIN;}
{PRINTF} {printf("t_printf");return T_PRINTF;}
{CONST} {printf("t_const");return T_CONST;}
{VAR} {printf("t_var");return T_VAR;}
{ADD} {printf(" t_add ");return T_ADD;}
{SUB} {printf(" t_sub ");return T_SUB;}
{MUL} {printf(" t_mul ");return T_MUL;}
{DIV} {printf(" t_div ");return T_DIV;}
{EQ} {printf(" t_eq ");return T_EQ;}
{POPEN} {printf(" t_popen ");return T_POPEN;}
{PCLOSE} {printf(" t_pclose ");return T_PCLOSE;}
{AOPEN} {printf(" t_aopen ");return T_AOPEN;}
{ACLOSE} {printf(" t_aclose ");return T_ACLOSE;}
{COPEN} {printf(" t_copen");return T_COPEN;}
{CCLOSE} {printf(" t_cclose ");return T_CCLOSE;}
{INT} {printf("t_int");yylval.s=strdup(yytext);yylval.s = strdup(yytext);return T_INT;}
{NAME} {printf("t_name");yylval.s=strdup(yytext);yylval.s = strdup(yytext);return T_NAME;}
{SPACE} {printf(" ");}
{SEP} {printf(" t_sep ");return T_SEP;}
{COMMA} {printf(" T_comma ");return T_COMMA;}

%%
int yywrap() {}

/*void main(void) {
    yylex();
}*/

