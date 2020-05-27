%{
    #include "ast.h"
    #include "name_list.h"
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
INF <
SUP >
EQEQ ==
EQ =
REF &
POPEN [(]
PCLOSE [)]
AOPEN \{
ACLOSE \}
COPEN \[
CCLOSE \]
COMMA ,

MAIN main
IF if
ELSE else
WHILE while

%%

{MAIN} {printf("t_main");return T_MAIN;}
{IF} {printf("t_if");return T_IF;}
{ELSE} {printf("t_else");return T_ELSE;}
{WHILE} {printf("t_while");return T_WHILE;}
{PRINTF} {printf("t_printf");return T_PRINTF;}
{CONST} {printf("t_const");return T_CONST;}
{VAR} {printf("t_var");return T_VAR;}
{ADD} {printf(" t_add ");return T_ADD;}
{SUB} {printf(" t_sub ");return T_SUB;}
{MUL} {printf(" t_mul ");return T_MUL;}
{DIV} {printf(" t_div ");return T_DIV;}
{SUP} {printf(" t_sup ");return T_SUP;}
{INF} {printf(" t_inf ");return T_INF;}
{EQEQ} {printf(" t_eq ");return T_EQEQ;}
{EQ} {printf(" t_eq ");return T_EQ;}
{REF} {printf("t_ref");return T_REF;}
{POPEN} {printf(" t_popen ");return T_POPEN;}
{PCLOSE} {printf(" t_pclose ");return T_PCLOSE;}
{AOPEN} {printf(" t_aopen ");return T_AOPEN;}
{ACLOSE} {printf(" t_aclose ");return T_ACLOSE;}
{COPEN} {printf(" t_copen");return T_COPEN;}
{CCLOSE} {printf(" t_cclose ");return T_CCLOSE;}
{INT} {printf("t_int");yylval.i=atoi(yytext);return T_INT;}
{NAME} {printf("t_name");yylval.s = strdup(yytext);return T_NAME;}
{SPACE} {printf(" ");}
{SEP} {printf(" t_sep ");return T_SEP;}
{COMMA} {printf(" T_comma ");return T_COMMA;}

%%
int yywrap() {}

/*void main(void) {
    yylex();
}*/

