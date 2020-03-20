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

y.tab.c : name_list.o AST/name_list.h tokens.y
	${YACC} ${YACCFLAGS} tokens.y

name_list.o : AST/name_list.c AST/name_list.h
	${CC} ${CCFLAGS} -c AST/name_list.c



