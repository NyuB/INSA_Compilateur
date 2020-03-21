CC = gcc
CFLAGS = -Wall
YACC = yacc 
YACCFLAGS = -d -v --verbose --debug

all : comp

comp : lex.yy.c y.tab.c ast_all
	${CC} ${CFLAGS} name_list.c y.tab.c lex.yy.c -o comp

lex.yy.c : lexer.l ast_all
	${LEX} lexer.l

y.tab.c : tokens.y ast_all
	${YACC} ${YACCFLAGS} tokens.y

ast_all : ast_obj
	${CC} ${CFLAGS} ast.o ast_repr.o name_list.o  ast_test.o -o ast_test

ast_obj : name_list.o ast.o ast_test.o ast_repr.o 

ast_test.o : ast_test.c
	${CC} ${CFLAGS} -c ast_test.c

ast.o : ast.c ast.h
	${CC} ${CFLAGS} -c ast.c

ast_repr.o : ast_repr.c ast_repr.h
	${CC} ${CFLAGS} -c ast_repr.c

name_list.o : name_list.c name_list.h
	${CC} ${CFLAGS} -c name_list.c