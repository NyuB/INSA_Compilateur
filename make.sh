lex lexer.l
yacc -d -v --verbose --debug tokens.y 
gcc -Wall y.tab.c lex.yy.c -o comp
./comp < test.c