CC = gcc
CFLAGS = -Wall

all : comp

lex.yy.c : lexer.l
	lex lexer.l

y.tab.c : tokens.y
	yacc -d -v --verbose --debug tokens.y

comp : lex.yy.c y.tab.c
	${CC} ${CFLAGS} y.tab.c lex.yy.c -o comp  