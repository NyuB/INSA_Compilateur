#include "ast.h"
//Fonctions d'allocation

ast_node * ast_new_node(int code, void * content,int nb_childs,ast_node_list * childs){
	ast_node * res = (ast_node *)malloc(1*sizeof(ast_node));
	res->code=code;
	res->content = content;
	res->nb_childs = nb_childs;
	res->childs = childs;
	return res;
}


ast_node_list * ast_node_list_empty(){
	ast_node_list * res = (ast_node_list *)malloc(1*sizeof(ast_node_list));
	res->start = NULL;
	res->end = NULL;
	return res;
}

//Fonctions de manipulation de listes
void ast_node_list_append(ast_node_list * list,ast_node * node){
	ast_node_cell * new = (ast_node_cell *)malloc(1*sizeof(ast_node_cell));
	new->node = node;
	new->suiv=NULL;
	ast_node_cell * aux = list->end;
	if(aux==NULL){
		list->start = new;
	}
	else{
		list->end->suiv = new;
	}
	list->end = new;
}
void ast_node_list_prepend(ast_node_list * list, ast_node * node){
	ast_node_cell * new = (ast_node_cell *)malloc(1*sizeof(ast_node_cell));
	new->node = node;
	new->suiv=list->start;
	list->start=new;	
}

int ast_node_list_length(ast_node_list * list){
	int res=0;
	ast_node_cell * aux = list->start;
	while(aux!=NULL){
		res++;
		aux=aux->suiv;
	}
	return res;
}


//Fonctions de création de noeud spécifique

//Retourne un noeud de code AST_CODE_VAR ayant pour contenu une copie du nom en argument et ne possédant aucun fils
ast_node * ast_var(char * name){ 
	int l = strlen(name)+1;
	char * content = (char *)malloc(l*sizeof(char));
	myncpy(content,name,l);
	return ast_new_node(AST_CODE_VAR,(void *)content,0,NULL);
}

//Retourne un noeud de code AST_CODE_DCL ayant pour contenu une copie du nom en argument et ne possédant aucun fils
ast_node * ast_declare(char * name){
	int l = strlen(name)+1;
	char * content = (char *)malloc(l*sizeof(char));
	myncpy(content,name,l);
	return ast_new_node(AST_CODE_DCL,content,0,NULL);
}
//Retourne un noeud de code AST_CODE_CON ayant pour contenu une copie de la string argument et possédant un noeud fils expression
ast_node * ast_declare_const(char * name, ast_node * expr){
	int l = strlen(name)+1;
	char * content = (char *)malloc(l*sizeof(char));
	myncpy(content,name,l);
	ast_node_list * child = ast_node_list_empty();
	ast_node_list_append(child, expr);
	return ast_new_node(AST_CODE_CON,content,1,child);
}

//Retourne un noeud de code op avec pour fils les deux noeuds en argument.
ast_node * ast_math(int op, ast_node * left,ast_node * right){
	ast_node_list * list = ast_node_list_empty();
	ast_node_list_append(list,left);
	ast_node_list_append(list,right);
	printf("Building math op : %d\n",op);
	return ast_new_node(op,NULL,2,list);
}

//Retourne un noeud de code AST_CODE_AFF avec pour fils les deux noeuds en arguments. Aucune vérification n'est faite sur la nature des noeuds
ast_node * ast_affect(ast_node * left,ast_node * right){
	ast_node_list * list = ast_node_list_empty();
	ast_node_list_append(list,left);
	ast_node_list_append(list,right);
	return ast_new_node(AST_CODE_AFF,NULL,2,list);
}

//Retourne un noeud de code AST_NODE_SEQ ayant pour fils chacun des noeuds de la liste
ast_node * ast_node_seq(ast_node_list * list){
	return ast_new_node(AST_CODE_SEQ,NULL,ast_node_list_length(list),list);
}

ast_node * ast_int(int integer){
	int * content = (int *)malloc(sizeof(int));
	*(content) = integer;
	return ast_new_node(AST_CODE_INT, content,0,NULL);
}

//Fonctions d'écriture 
void ast_write(FILE * file,const char * op,int destAddr,int leftAddr,int rightAddr){
    fprintf(file,"%s %d %d %d\n",op,destAddr,leftAddr,rightAddr);
}

//Fonctions de construction de l'AST
ast * ast_new(ast_node * root){
	ast * tree = (ast *)malloc(1*sizeof(ast));
	tree->root = root;
	return tree;
}

//Fonctions d'interprétation de l'AST

typedef struct build_data{
	name_list * var_list;
	int * left_addr_min;
	int * right_addr_max;
	int * line;
	FILE * file;
}build_data;

//Fonction récursive principale d'interprétation des noeuds, voire plus bas
void ast_node_build(ast_node * node, build_data * datas);

//Assume que le noeud contient une string (noeud de référence à un nom) , et vérifie qu'il est bien présent dans le namespace donné en argument ,et retourne son adresse
name_info * name_resolve(name_list * var_list, ast_node * node){
	name_info * info = nli_contains(var_list,(char *)node->content);//On récupère les données variable associées dans la liste des noms déclarés
	if(info == NOT_FOUND){//Vérifier que ce nom est bien déclaré dans la liste
		printf("Semantic error : variable [ %s ] referenced before declaration\n",(char *)(node->content));
		//TODO : raise an error ?
		return NULL;
	}
	else{
		if(info->status == NS_MUTABLE){
			printf("Warning : variable [ %s ] may be used before initialization\n",(char *)(node->content));
		}
		return info;
	}
}

int addr_resolve(ast_node * node, build_data * datas,int * stack_shift){
	int addr;
	if(node->code == AST_CODE_VAR){
		addr = name_resolve(datas->var_list, node)->addr;
	}
	else{
		ast_node_build(node, datas);
		addr = *(datas->right_addr_max);
		(*stack_shift)++;
	}
	return addr;
}

//Declaration et affectation d'une constante
void ast_const_build(ast_node * node, build_data * datas){
	name_info * info = nli_contains(datas->var_list,(char *)(node->content));
	int rightAddr;
	int stack_shift=0;
	if(info != NOT_FOUND){//Vérifier que ce nom n'est pas déjà déclaré
		printf("Semantic error : variable [ %s ] is already declared\n",(char *)(node->content));
		//TODO lever une erreur et quitter?
	}
	else{
		ast_node * right = node->childs->start->node;
		rightAddr = addr_resolve(right, datas, &stack_shift);
		nli_append(datas->var_list, (char *)(node->content), AST_TYPESIZE_INT, *(datas->left_addr_min), NS_CONSTANT);
		ast_write(datas->file,"COP", *(datas->left_addr_min), rightAddr, -1);
		(*(datas->left_addr_min)) ++;
		*(datas->right_addr_max) += stack_shift;
	}

}

//Fonction d'interprétation d'une expression à deux opérandes. Vérifie la nature des opérandes pour ajuster les opérations sur la stack
void ast_math_build(const char * op,ast_node * node, build_data * datas){
	ast_node_cell * left = node->childs->start;
	ast_node_cell * right = left->suiv;
	int leftAddr;
	int rightAddr;
	int stack_shift = 0;
	rightAddr = addr_resolve(right->node, datas, &stack_shift);
	leftAddr = addr_resolve(left->node, datas, &stack_shift);
	int addr = *(datas->right_addr_max)+stack_shift-1;//Le résultat est "placé" dans la pile après "consommation" des deux opérandes
	ast_write(datas->file,op, addr, leftAddr, rightAddr);//écriture de l'opération dans le fichier assembleur
	*(datas->right_addr_max) = addr;
}

void ast_int_build(ast_node * node, build_data * datas){
	(*(datas->right_addr_max))--;
	ast_write(datas->file, "AFC", *(datas->right_addr_max),*((int*)(node->content)),-1);
}



void ast_aff_build(ast_node * node, build_data * datas){
	ast_node_cell * left = node->childs->start;
	ast_node_cell * right = left->suiv;
	int rightAddr;
	int stack_shift = 0;
	int leftAddr;
	name_info * info = nli_contains(datas->var_list,(char*)(left->node->content));//On récupère l'index de la variable dans la liste des noms déclarés
	if(info == NOT_FOUND){
		printf("Semantic error : variable [ %s ] referenced before declaration\n",(char *)(left->node->content));
	}
	else if(info->status == NS_CONSTANT){
		printf("Semantic error : constant [ %s ] can't be affected after declaration\n",(char *)(left->node->content));
	}
	else{
		leftAddr = info->addr;
	}
	info -> status = NS_ASSIGNED;
	rightAddr = addr_resolve(right->node, datas, &stack_shift);
	*(datas->right_addr_max)+=stack_shift;
	ast_write(datas->file,"COP",leftAddr,rightAddr,-1);
	
}
ast_node * ast_node_if(ast_node * condition, ast_node * true, ast_node * false);//noeud if (bloc else dans le noeud false)

/*Fonction récursive principale d'interprétation de l'AST
paramètre (valeur initiale) => description

node (root) => le noeud à explorer
var_list (liste vide) => le namespace construit lors de l'exploration
left_addr_min (0) => prochaine addresse dispo en écriture de l'espace de variable
right_addr_max (taille_memoire) => dernière addresse écrite de la stack
file (constant) => fichier assembleur où écrire les instructions au cours de l'exploration

*/
void ast_node_build(ast_node * node, build_data * datas){
	if(*(datas->left_addr_min) > *(datas->right_addr_max)){//On vérifie que l'espace des variables ne dépasse pas dans la pile
		printf("Specification error : expression evaluation will require too much stack\n");
		printf("Allow more stack or split/condense nested expressions\n");
	}
	switch(node->code){
		case AST_CODE_ADD:
			ast_math_build("ADD",node,datas);
			printf("[DEBUG]MATHOP ADD\n");	
			break;
		case AST_CODE_MUL:
			ast_math_build("MUL",node,datas);
			printf("[DEBUG]MATHOP MUL\n");	
			break;
		case AST_CODE_SUB:
			ast_math_build("SUB",node,datas);
			printf("[DEBUG]MATHOP SUB\n");
			break;
		case AST_CODE_DIV:
			ast_math_build("DIV",node,datas);
			printf("[DEBUG]MATHOP DIV\n");	
			break;
		case AST_CODE_AFF:
			ast_aff_build(node,datas);
			printf("[DEBUG]AFFECT\n");	
			break;
		case AST_CODE_DCL:
			if(nli_contains(datas->var_list,(char*)(node->content)) != NOT_FOUND){//On vérifie que ce nom n'est pas déjà déclaré
				printf("Semantic error : [ %s ] is already declared\n",(char*)(node->content));
				//TODO lever une erreur et quitter?
			}
			nli_append(datas->var_list,(char*)(node->content), AST_TYPESIZE_INT, *(datas->left_addr_min), NS_MUTABLE);//On ajoute ce nom à la liste des noms déclarés
			(*(datas->left_addr_min))++;//On décale l'index "d'écriture" de l'espace des variables déclarées
			printf("[DEBUG]DECLARE %s %d\n",(char *)(node->content),*(datas->left_addr_min));
			break;
		case AST_CODE_CON:
			ast_const_build(node,datas);
			printf("[DEBUG]CONST %s %d\n",(char *)(node->content),*(datas->left_addr_min));
			break;
		case AST_CODE_SEQ:
			for(ast_node_cell * cursor = node->childs->start;cursor!=NULL;cursor=cursor->suiv){
				ast_node_build(cursor->node,datas);
			}
			printf("[DEBUG]SEQOVER\n");
			break;
		/*
		case AST_CODE_IF:
			ast_build_if(node, datas);
			break;
		*/
		case AST_CODE_INT:
			ast_int_build(node, datas);
			break;
		case AST_CODE_VAR:
			printf("[DEBUG]Un noeud AST_CODE_VAR(référence à une variable) ne devrait pas être traité dans le switch...\n");
			//N'est pas supposé arrivé
			break;
		default:
			break;
	}
	
	if(*(datas->left_addr_min) > *(datas->right_addr_max)){//On vérifie à nouveau qu'il n'y a pas eu collision entre espace variable et stack
		printf("Specifications error : expression evaluation will require too much stack\n");
		printf("Allow more stack or split/condense nested expressions\n");
	}
}

//Ouvre un fichier assembleur et y écrit les opérations associées à l'interprétation sémantique de l'AST
void ast_build(ast * tree,const char * filename,int mem_size){
	FILE * file = fopen(filename,"w");
	name_list * vars = nli_empty();
	int minMem = 0;
	int maxMem = mem_size;
	int line = 0;
	build_data * data = (build_data *)malloc(1*sizeof(build_data));
	data->left_addr_min = &minMem;
	data->right_addr_max = &maxMem;
	data->var_list = vars;
	data->file = file;
	data->line = &line;
	ast_node_build(tree->root,data);//Lancement de la récursion
	fclose(file);
}





