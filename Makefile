CC = gcc
CFLAGS = -Wall
LEX = lex
YACC = yacc 
YACCFLAGS = -d -v --verbose --debug

all : comp

comp : lex.yy.c y.tab.c
	${CC} ${CFLAGS} name_list.c y.tab.c lex.yy.c -o comp

lex.yy.c : lexer.l
	${LEX} lexer.l

y.tab.c : tokens.y
	${YACC} ${YACCFLAGS} tokens.y
name_list.o : name_list.c name_list.h
	${CC} ${CFLAGS} -c name_list.c



