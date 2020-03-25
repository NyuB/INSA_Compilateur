#ifndef H_NAME_LIST
#define H_NAME_LIST
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NOT_FOUND NULL

typedef struct name_list name_list;

typedef enum name_status{
	VS_CONSTANT,
	VS_MUTABLE,
	VS_ASSIGNED,
	VS_FUNCTION,
	VS_NOTFOUND
}var_status;

typedef struct name_info {
	char * name;
	int size;
	int addr;
	var_status status;
}name_info;



name_info * new_name_info(char * name, int size, int addr,var_status status);

name_list * nli_empty(void);

name_info * nli_contains(name_list * list, char * name);//Retourne l'index du nom dans la liste si présent, NOT_FOUND sinon

void nli_append(name_list * list, char * name, int size, int addr, var_status status);//Ajoute le nom en fin de liste. Attention pas de vérification d'unicité

void nli_prepend(name_list * list, char * name, int size, int addr, var_status status);//Ajoute le nom en début de liste. Attention pas de vérification d'unicité

void nli_display(name_list * list);//Printf la liste sous un format lisible dans la console

void myncpy(char * dest,char * src,int n);//idem à strcpy pour les char * non constant

#endif