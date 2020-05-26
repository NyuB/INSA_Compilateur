#include "name_list.h"

name_info * new_name_info(char * name, int size, int addr,var_status status){
	name_info * res = (name_info *)malloc(1*sizeof(name_info));
	int len = strlen(name)+1;
	res->name = (char *)malloc(len*sizeof(char));
	myncpy(res->name,name,len);
	res->size = size;
	res->addr = addr;
	res->status = status;
	return res;
}

typedef struct name_cell {
	name_info * info;
	struct name_cell * suiv;
}name_cell;

typedef struct name_list{
	name_cell * start;
	name_cell * end;
}name_list;

void myncpy(char * dest,char * src,int n){
	for(int i=0;i<n;i++){
		dest[i] = src[i];
	}
}

name_cell * new_name_cell(name_info * info){
	name_cell * res = (name_cell *)malloc(1*sizeof(name_cell));
	res->info = info;
	res->suiv = NULL;
	return res;
}

name_list * nli_empty(void){
	name_list * res = (name_list *)malloc(1*sizeof(name_list));
	res->start = NULL;
	res->end = NULL;
	return res;
}

void nli_append(name_list * list, char * name, int size, int addr, var_status status){
	name_cell * new = new_name_cell(new_name_info(name,size,addr,status));
	if(list->start == NULL){
		list->start = new;
		list->end = list->start;
	}
	else{
		list->end->suiv = new;
		list->end = list->end->suiv;
	}
}

void nli_prepend(name_list * list, char * name, int size, int addr, var_status status){
	name_cell * new = new_name_cell(new_name_info(name,size,addr,status));
	new->suiv = list->start;
	list->start = new;
}

name_info * nli_contains(name_list * list, char * name){
	name_cell * cursor = list->start;
	while(cursor != NULL){
		if(strcmp(cursor->info->name,name) == 0) return cursor->info;
		cursor = cursor->suiv;
	}
	return NOT_FOUND;
}

void nli_display(name_list * list){
	name_cell * aux = list->start;
	while(aux != NULL){
		printf("=> %s %d %d %s ", aux->info->name, aux->info->size, aux->info->addr, (aux->info->status == NS_MUTABLE)?"var":"const");
		aux = aux->suiv;
	}
	printf("\n");
}

void nli_warn(name_list * list){
	name_cell * aux = list->start;
	while(aux!=NULL){
		if(aux->info->status == NS_MUTABLE){
			printf("Warning : [%s] declared but never assigned\n",aux->info->name);
		}
		aux=aux->suiv;
	}
}

void nli_info_free(name_info * info){
	free(info->name);
	free(info);
}

void cell_free(name_cell * cell){
	free(cell->info);
	free(cell);
}



void nli_free(name_list * list){
	name_cell * cursor = list->start;
	name_cell * aux;
	while(cursor != NULL){
		aux = cursor->suiv;
		free(cursor);
		cursor = aux;
	}
	free(list);
}


