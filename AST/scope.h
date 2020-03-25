#ifndef H_SCOPE
#define H_SCOPE

#include <stdlib.h>
#include "name_list.h"

typedef struct scope{
	name_list * namespace;
	struct scope * container;
}scope;

scope * scp_empty_root();//Retourne un scope vide sans père

scope * scp_empty_contained(scope * container);//Retourne un scope vide ayant pour père le scope container

name_info * scp_contains(scope * scp, char * name);//Retourne l'addresse de la variable si contenue dans le scope, NOT_FOUND sinon

void scp_add(scope * scp, char * name, int size,int addr, var_status status);//Ajoute ce nom de variable au scope (TODO gérer l'adresse)

void scp_display(scope * scp);//Affiche le scope puis ses contenants récursivement
#endif