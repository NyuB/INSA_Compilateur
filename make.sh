lex lexer.l
yacc -d tokens.y 
gcc -Wall y.tab.c lex.yy.c -o comp
./comp < test.c
