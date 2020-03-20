%{
#include <stdio.h>
#define MAX 63
#define NOT_FOUND -1;
typedef struct name_cell {
	char * name;
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
%}


%{
    int yydebug=1;
    FILE * file;
    int min =0;
    int max = MAX;
    name_list * namel;
%}



%token T_MAIN T_PRINTF T_CONST T_VAR T_ADD T_SUB T_MUL T_DIV T_EQ T_POPEN T_PCLOSE T_AOPEN T_ACLOSE T_COPEN T_CCLOSE T_INT T_NAME T_SEP T_COMMA
%left T_MUL T_DIV
%left T_ADD T_SUB


%%
start : T_MAIN T_POPEN T_PCLOSE BLOC {printf("MAIN\n");};

BLOC : T_AOPEN BODY T_ACLOSE {printf("{BODY}");};

BODY :
    |CMD
    |CMD T_SEP BODY ;

CMD : declare_assignement
    |declaration
    |assignement
    |PRINT;

OP: T_ADD {$$=1;}
    |T_SUB {$$=3;}
    |T_MUL {$$=2;}
    |T_DIV {$$=4;};

EXPR : T_NAME {printf("NAME-EXPR\n");int index=nli_contains(namel,"NONE");printf("Text ici");$$=index;}
    |T_POPEN EXPR T_PCLOSE {$$=$2;}
    |T_INT {printf("INT-EXPR\n");$$=T_INT;}
    |EXPR OP EXPR {printf("EXPR\n");ASM_write($2);ASM_write(-1);ASM_write($1);ASM_write($3);ASM_endline();$$=-1;}
    |T_SUB EXPR {printf("MIN-EXPR\n");}
    |T_ADD EXPR {printf("PLUS-EXPR\n");};

PRINT : T_PRINTF T_POPEN EXPR T_PCLOSE {ASM_write(T_PRINTF);};

T_NAMELIST : T_NAME
           |T_NAME T_COMMA T_NAMELIST;

declare_assignement : T_VAR T_NAME T_EQ EXPR{printf("DCLR-ASSIGN\n");};
declaration : T_VAR T_NAMELIST{printf("DECLARATION");min++;nli_append(namel,"NONE");};
assignement : T_NAME T_EQ T_NAME{printf("ASSIGN_NAME");ASM_write(5),ASM_write($1),ASM_write($3);ASM_endline();}
            




%%
int yyerror(char *msg){
    fprintf(stderr, "err: %s\n", msg);
    return 1;
}





void ASM_write(int instruction){
    fprintf(file,"%d ",instruction);
}
void ASM_endline(){
    fprintf(file,"\n");
}

int main(void){
    const char * filename = "./assembly.asm";
    namel = nli_empty();
    file = fopen(filename,"w");
    yyparse();
    fclose(file);

    return 0;
}
