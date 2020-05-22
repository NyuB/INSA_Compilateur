#ifndef H_NAME_LIST
#define H_NAME_LIST
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NOT_FOUND NULL

typedef struct name_list name_list;

typedef enum var_status{//Informations supplémentaire, par exemple si la variable est une constante, s elle a été initialisée, etc
	NS_CONSTANT,
	NS_CONSTANT_ASSIGNED,
	NS_MUTABLE,
	NS_ASSIGNED,
	NS_FUNCTION,
	NS_POINTER,
	NS_NOTFOUND
}var_status;

typedef struct name_info {
	char * name;
	int size;//Pour l'instant inutilisé, on ne gère que des entiers donc 1 seule taille de variable
	int addr;//Addresse dans le tas
	var_status status;
}name_info;



name_info * new_name_info(char * name, int size, int addr,var_status status);

name_list * nli_empty(void);

name_info * nli_contains(name_list * list, char * name);//Retourne l'index du nom dans la liste si présent, NOT_FOUND sinon

void nli_append(name_list * list, char * name, int size, int addr, var_status status);//Ajoute le nom en fin de liste. Attention pas de vérification d'unicité

void nli_prepend(name_list * list, char * name, int size, int addr, var_status status);//Ajoute le nom en début de liste. Attention pas de vérification d'unicité

void nli_display(name_list * list);//Affiche la liste sous un format lisible dans la console

void nli_info_free(name_info * info);//Libère la mémoire allouée à ces informations

void nli_free(name_list * list);//Libère la mémoire allouée à cette liste et ses éléments

void myncpy(char * dest,char * src,int n);//idem à strcpy pour les char * non constant

#endif