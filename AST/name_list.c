#include "name_list.h"

typedef struct name_cell {
	char * name;
	struct name_cell * suiv;
}name_cell;

struct name_list{
	name_cell * start;
	name_cell * end;
};

void myncpy(char * dest,char * src,int n){
	for(int i=0;i<n;i++){
		dest[i] = src[i];
	}
}

name_cell * new_name_cell(char * name){
	name_cell * res = (name_cell *)malloc(1*sizeof(name_cell));
	int len = strlen(name)+1;
	char * str = (char *)malloc(len*sizeof(char));
	myncpy(str,name,len);
	res->name = str;
	res->suiv = NULL;
	return res;
}

name_list * nli_empty(void){
	name_list * res = (name_list *)malloc(1*sizeof(name_list));
	res->start=NULL;
	res->end=NULL;
	return res;
}

void nli_append(name_list * list, char * name){
	if(list->start == NULL){
		list->start = new_name_cell(name);
		list->end = list->start;
	}
	else{
		list->end->suiv = new_name_cell(name);
		list->end = list->end->suiv;
	}
}

void nli_display(name_list * list){
	name_cell * aux = list->start;
	while(aux!=NULL){
		printf("=> %s ",aux->name);
		aux=aux->suiv;
	}
	printf("\n");
}

int nli_contains(name_list * list, char * name){
	name_cell * cursor = list->start;
	int index = 0;
	while(cursor!=NULL){
		if(strcmp(cursor->name,name) == 0)return index;
		index++;
		cursor=cursor->suiv;
	}
	return NOT_FOUND;
}
