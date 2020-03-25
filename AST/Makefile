CONFIG ?= lin
CC = gcc
CFLAGS = -Wall

ifeq ($(CONFIG),win)
	LEX = flex
	YACC = bison 
	YACCFLAGS = -dy -v --verbose
else
	LEX = lex
	YACC = yacc 
	YACCFLAGS = -d -v --verbose
endif

exe : comp

test: ast_test

comp : y.tab.c lex.yy.c ast_all
	${CC} ${CFLAGS} name_list.c scope.c ast.c y.tab.c lex.yy.c -o comp

lex.yy.c : lexer.l ast_all
	${LEX} lexer.l

y.tab.c : tokens.y ast_all
	${YACC} ${YACCFLAGS} tokens.y

ast_all : name_list.o ast.o scope.o 

ast.o : ast.c ast.h name_list.h scope.h
	${CC} ${CFLAGS} -c ast.c

scope.o : scope.c scope.h
	${CC} ${CFLAGS} -c scope.c

name_list.o : name_list.c name_list.h
	${CC} ${CFLAGS} -c name_list.c

ast_test: ast_test.o ast_repr.o ast.o name_list.o
	${CC} ${CFLAGS} ast.o ast_repr.o name_list.o scope.o ast_test.o -o ast_test

ast_test.o : ast_test.c ast.h ast_repr.h scope.h
	${CC} ${CFLAGS} -c ast_test.c

ast_repr.o : ast_repr.c ast_repr.h ast.h
	${CC} ${CFLAGS} -c ast_repr.c


