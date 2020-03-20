CFLAGS = -Wall
testall : ast.o ast_repr.o name_list.o test.o
	gcc ${CFLAGS} ast.o ast_repr.o name_list.o  test.o -o test


obj : name_list.o ast.o

test.o : test.c
	gcc ${CFLAGS} -c test.c

ast.o : ast.c ast.h
	gcc ${CFLAGS} -c ast.c
ast_repr.o : ast_repr.c ast_repr.h
	gcc ${CFLAGS} -c ast_repr.c
name_list.o : name_list.c name_list.h
	gcc ${CFLAGS} -c name_list.c