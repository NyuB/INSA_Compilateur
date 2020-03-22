#ifndef H_NAME_LIST
#define H_NAME_LIST
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NOT_FOUND -3

typedef struct name_list name_list;

name_list * nli_empty(void);

int nli_contains(name_list * list, char * name);//Retourne l'index du nom dans la liste si présent, NOT_FOUND sinon

void nli_append(name_list * list, char * name);//Ajoute le nom en fin de liste. Attention pas de vérification d'unicité

void nli_prepend(name_list * list, char * name);//Ajoute le nom en début de liste. Attention pas de vérfication d'unicité

void nli_display(name_list * list);//Printf la liste sous un format lisible dans la console

void myncpy(char * dest,char * src,int n);//idem à strcpy pour les char * non constant

#endif