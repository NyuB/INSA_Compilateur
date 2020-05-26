#ifndef H_SCOPE
#define H_SCOPE

#include <stdlib.h>
#include "name_list.h"

typedef struct scope{
	name_list * namespace;
	struct scope * container;//Le scope parent ou NULL
}scope;

scope * scp_empty_root();//Retourne un scope vide sans père

scope * scp_empty_contained(scope * container);//Retourne un scope vide ayant pour père le scope container

name_info * scp_contains(scope * scp, char * name);//Retourne l'addresse de la variable si contenue dans le scope, NOT_FOUND sinon

name_info * scp_contains_floor(scope * scp, char * name);

void scp_add(scope * scp, char * name, int size,int addr, var_status status);//Ajoute ce nom de variable au scope

void scp_display(scope * scp);//Affiche le scope puis ses contenants récursivement

void scp_warn_and_free(scope * scp);//Analyse un scope avant sa destruction et lève des warnings si nécessaire

void scp_free(scope * scp);//Libère la mémoire allouée à ce scope
#endif