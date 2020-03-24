#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#define ADD 1
#define MUL 2
#define SOU 3
#define DIV 4
#define COP 5
#define AFC 6
#define JMP 7
#define JMF 8
#define INF 9
#define SUP 10
#define EQU 11
#define PRI 12

#define MAX_INSTRUCTIONS_SIZE 256
#define MAX_MEMORY_SIZE 256

void asm_init();
void asm_add_3(char ins, int arg1, int arg2, int arg3);
void asm_add_2(char ins, int arg1, int arg2);
void asm_add_1(char ins, int arg1);
void asm_run();

#endif // #ifndef __INSTRUCTIONS_H__
