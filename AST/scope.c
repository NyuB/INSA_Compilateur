#include "scope.h"

scope * scp_empty_contained(scope * container){
	scope * res = (scope *)malloc(1*sizeof(scope));
	res->namespace = nli_empty();
	res->container = container;
	return res;
}

scope * scp_empty_root(){
	return scp_empty_contained(NULL);
}

name_info * scp_contains(scope * scp, char * name){
	scope * cursor = scp;
	name_info * var;
	while(cursor != NULL){
		var = nli_contains(cursor->namespace,name); 
		if(var != NOT_FOUND){
			return var;
		}
		cursor = cursor -> container;
	}
	return NOT_FOUND;
}

void scp_add(scope * scp, char * name, int size, int addr, var_status status){
	nli_append(scp->namespace, name, size, addr, status);
}

void scp_display(scope * scp){
	scope * cursor = scp;
	int index = 0;
	while(cursor != NULL){
		printf("level -%d ",index);
		nli_display(cursor->namespace);
		cursor = cursor->container;
		index ++;
	}
}

