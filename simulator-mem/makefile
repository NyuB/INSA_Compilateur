SRCC:= ./src/*.c

all: simulator

simulator: ./src/simulator.y ./src/simulator.l ./src/instructions.c
	yacc -d ./src/simulator.y
	lex ./src/simulator.l
	gcc lex.yy.c y.tab.c ./src/instructions.c -Isrc -o simulator

run: simulator
	./simulator < input.txt

clean:
	rm -f lex.yy.c simulator y.tab.h y.tab.c *.o
