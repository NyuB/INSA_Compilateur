CC = gcc
CFLAGS = -Wall
LEX = lex
YACC = yacc 
YACCFLAGS = -d -v --verbose --debug

all : comp

comp : lex.yy.c y.tab.c
	${CC} ${CFLAGS} y.tab.c lex.yy.c -o comp

lex.yy.c : lexer.l
	${LEX} lexer.l

y.tab.c : tokens.y
	${YACC} ${YACCFLAGS} tokens.y



