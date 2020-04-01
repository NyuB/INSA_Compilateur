SRCC:= ./src/*.c
CONFIG = lin
ifeq ($(CONFIG),win)
	LEX = flex
	YACC = bison 
	YACCFLAGS = -dy -v --verbose
else
	LEX = lex
	YACC = yacc 
	YACCFLAGS = -d -v --verbose
endif

all: simulator

simulator: ./src/simulator.y ./src/simulator.l ./src/instructions.c
	${YACC} ${YACCFLAGS} ./src/simulator.y
	${LEX} ./src/simulator.l
	gcc lex.yy.c y.tab.c ./src/instructions.c -Isrc -o simulator

run: simulator
	./simulator < input.txt

clean:
	rm -f lex.yy.c simulator y.tab.h y.tab.c *.o
