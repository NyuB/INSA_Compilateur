#ifndef H_NAME_LIST
#define H_NAME_LIST
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NOT_FOUND -1

typedef struct name_list name_list;

name_list * nli_empty(void);

int nli_contains(name_list * list, char * name);

void nli_append(name_list * list, char * name);
void nli_prepend(name_list * list, char * name);

void nli_display(name_list * list);

void myncpy(char * dest,char * src,int n);

#endif