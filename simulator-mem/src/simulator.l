%{
    #include "y.tab.h"
%}

vSEP    [ \t\r\n]

%%

ADD {return tADD;}
MUL {return tMUL;}
SOU {return tSOU;}
DIV {return tDIV;}
COP {return tCOP;}
AFC {return tAFC;}
JMP {return tJMP;}
JMF {return tJMF;}
INF {return tINF;}
SUP {return tSUP;}
EQU {return tEQU;}
PRI {return tPRI;}

-?[0-9]+  {
        yylval.nb = atoi(yytext);
        return tNB;
        }


{vSEP}  {}

.       {
        fprintf(stderr, "ERROR lex : Unknown pattern %s", yytext);
        exit(1);
        }

%%

int yywrap(void) { return 1; }
//int main(int argc, char *argv[]) { while (yylex()!=0) ; return 0; }
