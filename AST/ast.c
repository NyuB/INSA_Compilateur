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

//Fonction récursive principale d'interprétation des noeuds, voire plus bas
void ast_node_build(ast_node * node, name_list * var_list,int * left_addr_min,int * right_addr_max,FILE * file);

//Fonction d'interprétation d'une expression à deux opérandes. Vérifie la nature des opérandes pour ajuster les opérations sur la stack
void ast_math_build(const char * op,ast_node * node, name_list * var_list,int * left_addr_min,int * right_addr_max,FILE * file){
	ast_node_cell * left = node->childs->start;
	ast_node_cell * right = left->suiv;
	int leftAddr;
	int rightAddr;
	int stack_shift = 0;
	if(left->node->code == AST_CODE_VAR){//Si le noeud de gauche est une référence directe à une variable nommée
		leftAddr = nli_contains(var_list,(char *)(left->node->content));//On récupère l'index de la variable dans la liste des noms déclarés
		if(leftAddr == NOT_FOUND){//Vérifier que ce nom est bien déclaré dans la liste
			printf("Semantic error : variable [ %s ] referenced before declaration\n",(char *)(left->node->content));
			//TODO lever une erreur et quitter?
		}
	}
	//TODO traiter le cas des entiers
	else{//Le noeud est lui-même une expression
		ast_node_build(left->node,var_list,left_addr_min,right_addr_max,file);//appel récursif, on résoud ce noeud avant de poursuivre
		leftAddr = *((int*)(left->node->content));//"l'adresse" correspondante à ce noeud a été stockée dans son contenu après résolution
		stack_shift++;//on va "consommer" une variable de la stack, on décale(i.e "pop")
	}

	//Même traitement que pour le noeud de gauche
	if(right->node->code == AST_CODE_VAR){
		rightAddr = nli_contains(var_list,(char *)(right->node->content));
		if(rightAddr == NOT_FOUND){
			printf("Semantic error : variable [ %s ] referenced before declaration\n",(char *)(right->node->content));
		}
	}

	else{
		ast_node_build(right->node,var_list,left_addr_min,right_addr_max,file);
		rightAddr = *((int*)(right->node->content));
		stack_shift++;
	}

	int * addr = (int *)malloc(1*sizeof(int));//addresse de la stack où "placer" le résultat
	*addr = *right_addr_max+stack_shift;//Le résultat est "placé" dans la pile après "consommation" des deux opérandes
	ast_write(file,op,*addr,leftAddr,rightAddr);//écriture de l'opération dans le fichier assembleur
	node->content = addr;//on stocke pour information l'addresse du résultat dans le contenu noeud
	*right_addr_max = *addr-1;

}

void ast_aff_build(ast_node * node, name_list * var_list,int * left_addr_min,int * right_addr_max,FILE * file){
	ast_node_cell * left = node->childs->start;
	ast_node_cell * right = left->suiv;
	int rightAddr;
	int stack_shift = 0;
	int leftAddr = nli_contains(var_list,(char*)(left->node->content));//On récupère l'index de la variable dans la liste des noms déclarés
	if(leftAddr == NOT_FOUND){
		printf("Semantic error : variable [ %s ] referenced before declaration\n",(char *)(left->node->content));
	}
	if(right->node->code==AST_CODE_VAR){
		rightAddr = nli_contains(var_list,(char*)(right->node->content));
		if(rightAddr == NOT_FOUND){//Vérifier que ce nom est bien déclaré dans la liste
			printf("Semantic error : variable [ %s ] referenced before declaration\n",(char *)(right->node->content));
			//TODO lever une erreur et quitter?
		}

	}
	else{
		ast_node_build(right->node,var_list,left_addr_min,right_addr_max,file);//appel récursif, on résoud ce noeud avant de poursuivre
		rightAddr = *((int*)(right->node->content));//"l'adresse" correspondante à ce noeud a été stockée dans son contenu après résolution
		stack_shift++;//on va "consommer" une variable de la stack, on décale(i.e "pop")
	}
	(*right_addr_max)+=stack_shift;
	ast_write(file,"COP",leftAddr,rightAddr,-1);
}

void ast_node_build(ast_node * node, name_list * var_list,int * left_addr_min,int * right_addr_max,FILE * file){
	if(*left_addr_min > *right_addr_max){//On vérifie que l'espace des variables ne dépasse pas dans la pile
		printf("Specifications error : expression evaluation will require too much stack\n");
		printf("Allow more stack or split/condense nested expressions\n");
	}
	switch(node->code){
		case AST_CODE_ADD:
			ast_math_build("ADD",node,var_list,left_addr_min,right_addr_max,file);
			printf("MATHOP ADD\n");	
			break;
		case AST_CODE_MUL:
			ast_math_build("MUL",node,var_list,left_addr_min,right_addr_max,file);
			printf("MATHOP MUL\n");	
			break;
		case AST_CODE_SUB:
			ast_math_build("SUB",node,var_list,left_addr_min,right_addr_max,file);
			printf("MATHOP SUB\n");
			break;
		case AST_CODE_DIV:
			ast_math_build("DIV",node,var_list,left_addr_min,right_addr_max,file);
			printf("MATHOP DIV\n");	
			break;
		case AST_CODE_AFF:
			ast_aff_build(node,var_list,left_addr_min,right_addr_max,file);
			printf("AFFECT\n");	
			break;
		case AST_CODE_DCL:
			if(nli_contains(var_list,(char*)(node->content)) != NOT_FOUND){//On vérfie que ce nom n'est pas déjà déclaré
				printf("Semantic error : [ %s ] is already declared\n",(char*)(node->content));
				//TODO lever une erreur et quitter?
			}
			nli_append(var_list,(char*)(node->content));//On ajoute ce nom à la liste des noms déclarés
			(*left_addr_min)++;//On déclae l'index "d'écriture" de l'espace des variables déclarées
			printf("DECLARE %s %d\n",(char *)(node->content),*left_addr_min);
			break;
		case AST_CODE_SEQ:
			for(ast_node_cell * cursor = node->childs->start;cursor!=NULL;cursor=cursor->suiv){
				ast_node_build(cursor->node,var_list,left_addr_min,right_addr_max,file);
			}
			printf("SEQOVER\n");
			break;
		case AST_CODE_VAR:
			break;
		default:
			break;
	}
	if(*left_addr_min > *right_addr_max){//On vérifie à nouveau qu'il n'y a pas eu collision entre espace variable et stack
		printf("Specifications error : expression evaluation will require too much stack\n");
		printf("Allow more stack or split/condense nested expressions\n");
	}
}

//Ouvre un fichier assembleur et y écrit les opérations associées à l'interprétation sémantique de l'AST
void ast_build(ast * tree,const char * filename,int mem_size){
	FILE * file = fopen(filename,"w");
	name_list * vars = nli_empty();
	int minMem = 0;
	int maxMem = mem_size-1;
	ast_node_build(tree->root,vars,&minMem,&maxMem,file);//Lancement de la récursion
	fclose(file);
}





