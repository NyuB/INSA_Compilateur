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

int scp_contains(scope *  scp,char * name){
	scope * cursor = scp;
	int addr = -1;
	while(scp!=NULL){
		addr = nli_contains(cursor->namespace,name); 
		if(addr != NOT_FOUND){
			return addr;
		}
		cursor = cursor -> container;
	}
	return NOT_FOUND;
}

void scp_add(scope * scp, char * name){
	nli_append(scp->namespace,name);
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

