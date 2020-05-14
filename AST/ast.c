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

typedef struct asm_cell{
	char * instru;
	int destAddr;
	int leftAddr;
	int rightAddr;
	struct asm_cell * next;
}asm_cell;

typedef struct asm_instru_list{
	asm_cell * start;
}asm_instru_list;

asm_instru_list * asm_instru_list_empty(){
	asm_instru_list * res = (asm_instru_list *)malloc(1*sizeof(asm_instru_list));
	res->start = NULL;
	return res;
}

asm_cell * asm_cell_new(char * s,int destAddr,int leftAddr,int rightAddr){
	asm_cell * new = (asm_cell *)malloc(1*sizeof(asm_cell));
	int len = strlen(s)+1;
	new->instru = (char *)malloc(len*sizeof(char));
	new->destAddr = destAddr;
	new->leftAddr = leftAddr;
	new->rightAddr = rightAddr;
	myncpy(new->instru, s, len);
	new->next = NULL;
	return new;
}

void asm_instru_list_append(asm_instru_list * list,char * s,int destAddr,int leftAddr,int rightAddr){
	asm_cell * new = asm_cell_new(s, destAddr, leftAddr, rightAddr);
	if(list->start==NULL){
		list->start = new;
	}
	else{
		asm_cell * cursor = list->start;
		while(cursor->next != NULL){
			cursor = cursor->next;
		}
		cursor->next = new;
	}
}

void asm_instru_list_insert(asm_instru_list * list,char * s, int destAddr, int leftAddr, int rightAddr, int index){
	asm_cell * new = asm_cell_new(s, destAddr, leftAddr, rightAddr);
	if(index == 0){
		new->next = list->start;
		list->start=new;

	}
	else{
		asm_cell * cursor = list->start;
		for(int i=1;i<index;i++){
			cursor=cursor->next;
		}
		new->next = cursor->next;
		cursor->next=new;
	}
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

ast_node * ast_node_if(ast_node * condition, ast_node * true_body, ast_node * false_body){
	ast_node_list * three = ast_node_list_empty();
	ast_node_list_prepend(three,false_body);
	ast_node_list_prepend(three,true_body);
	ast_node_list_prepend(three,condition);
	return ast_new_node(AST_CODE_IF,NULL,3,three);
}

ast_node * ast_node_while(ast_node * condition, ast_node * body){
	ast_node_list * three = ast_node_list_empty();
	ast_node_list_prepend(three,body);
	ast_node_list_prepend(three,condition);
	return ast_new_node(AST_CODE_WHILE,NULL,2,three);
}

ast_node * ast_node_print(ast_node * expression){
	ast_node_list * child = ast_node_list_empty();
	ast_node_list_append(child,expression);
	return ast_new_node(AST_CODE_PRINT, NULL, 1,child);
}

ast_node * ast_int(int integer){
	int * content = (int *)malloc(sizeof(int));
	*(content) = integer;
	return ast_new_node(AST_CODE_INT, content,0,NULL);
}

ast_node * ast_ref(char * name){//référence addresse d'une variable nommée, exemple &a
	int l = strlen(name)+1;
	char * content = (char *)malloc(l*sizeof(char));
	myncpy(content,name,l);
	return ast_new_node(AST_CODE_REF,content,0,NULL);
}

typedef struct int_str{
	int i;
	char * s;
}int_str;

ast_node * ast_unref(char * name, int level){
	int_str * content = (int_str *)malloc(1*sizeof(int_str));
	content->i = level;
	int l = strlen(name)+1;
	content->s = (char *)malloc(l*sizeof(char));
	myncpy(content->s,name,l);
	return ast_new_node(AST_CODE_UNREF,content,0,NULL);
}

//Fonctions de construction de l'AST
ast * ast_new(ast_node * root){
	ast * tree = (ast *)malloc(1*sizeof(ast));
	tree->root = root;
	return tree;
}

//Fonctions d'interprétation de l'AST

/*Structure d'interprétation de l'AST (génération es instructions, affectation mémoire, namespace, etc)
var_list (liste vide) => le namespace construit lors de l'exploration
left_addr_min (0) => prochaine addresse dispo en écriture de l'espace de variable
right_addr_max (taille_memoire) => dernière addresse écrite de la stack
line (0) => numéro de la prochaine instruction assembleur
instructions (liste vide) instructions générées
file (constant) => fichier assembleur où écrire les instructions au cours de l'exploration
*/
typedef struct build_data{
	scope * var_list;
	int left_addr_min;
	int right_addr_max;
	int line;
	asm_instru_list * instructions;
	FILE * file;
}build_data;

//Fonctions d'écriture 
void ast_write(char * op,int destAddr,int leftAddr,int rightAddr,build_data * datas){
	asm_instru_list_append(datas->instructions,op,destAddr,leftAddr,rightAddr);
    datas->line++;

}
void ast_write_at(char * op,int destAddr,int leftAddr,int rightAddr,build_data * datas,int line){
	asm_instru_list_insert(datas->instructions, op, destAddr, leftAddr,rightAddr, line);
}
void asm_write_all(build_data * datas){
	asm_cell * cursor = datas->instructions->start;
	while(cursor!=NULL){
		fprintf(datas->file,"%s %d %d %d\n",cursor->instru,cursor->destAddr,cursor->leftAddr,cursor->rightAddr);
		cursor = cursor->next;
	}
}

//Fonction récursive principale d'interprétation des noeuds, voire plus bas
void ast_node_build(ast_node * node, build_data * datas);

//Assume que le noeud contient une string (noeud de référence à un nom) , et vérifie qu'il est bien présent dans le namespace donné en argument ,et retourne son adresse
name_info * name_resolve(scope * var_list, ast_node * node){
	name_info * info = scp_contains(var_list,(char *)node->content);//On récupère les données variable associées dans la liste des noms déclarés
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
		addr = datas->right_addr_max;
		(*stack_shift)++;
	}
	return addr;
}

//Bloc if qui construit chaque noeud avant d'insérer les instructions JMF/JMP pour savoir où sauter
void ast_if_build(ast_node * node,build_data * datas){
	ast_node_cell * cursor = node->childs->start;

	ast_node * condition = cursor->node;
	cursor = cursor->suiv;

	ast_node * true_body = cursor->node;
	cursor = cursor->suiv;

	ast_node * false_body = (cursor!=NULL)?cursor->node:NULL;

	int stack_shift = 0;
	int addr = addr_resolve(condition, datas, &stack_shift); //Résolution de l'expression conditionnelle
	int jmf_line = datas->line; //On conserve le numéro d'instruction où écrire le JMF en attendant de savoir où sauter
	datas->line++;//On 'saute' une instruction(le JMF)
	datas->right_addr_max += stack_shift;

	//Gestion du scope des noms de variable, on crée un scope fils pour le noeud true, détruit après sa construction
	scope * child = scp_empty_contained(datas->var_list);
	datas->var_list=child;
	ast_node_build(true_body,datas);
	datas->var_list = child->container;
	free(child);

	if(false_body !=NULL){
		int incond = datas->line;//On conserve le numéro d'instruction où écrire le JMP en attendant de savoir où sauter
		datas->line++;//On 'saute' une instruction(le JMP)

		//Gestion du scope pour le bloc false, qui ne s'intersecte donc pas avec celui du bloc true
		child = scp_empty_contained(datas->var_list);
		datas->var_list = child;
		ast_node_build(false_body,datas);
		datas->var_list = child->container;
		scp_free(child);

		
		//On connaît maintenant la taille du bloc true et false, on insère les instructions JMP et JFM
		ast_write_at("JMF", addr, incond + 1, -1, datas, jmf_line);
		ast_write_at("JMP", datas->line, -1, -1, datas, incond);
	}
	else{
		ast_write_at("JMF", addr, datas->line, -1, datas, jmf_line);//On connaît maintenant la taille du bloc, on insère l'instruction JMF avec le numéro d'instruction courant en argument
	}
}

//Cf bloc if, logique similaire
void ast_while_build(ast_node * node, build_data * datas){
	ast_node_cell * cursor = node->childs->start;

	ast_node * condition = cursor->node;
	cursor = cursor->suiv;

	ast_node * body = cursor->node;
	cursor = cursor->suiv;

	int stack_shift = 0;
	int expr_line = datas->line;
	int addr = addr_resolve(condition, datas, &stack_shift);
	int jmf_line = datas->line;
	datas->line++;
	datas->right_addr_max += stack_shift;

	scope * child = scp_empty_contained(datas->var_list);
	datas->var_list = child;
	ast_node_build(body,datas);
	datas->var_list = child->container;
	scp_free(child);

	ast_write("JMP", expr_line, -1, -1, datas);
	ast_write_at("JMF", addr, datas->line, -1, datas, jmf_line);
}

void ast_print_build(ast_node * node, build_data * datas){
	int stack_shift = 0;
	int addr = addr_resolve(node->childs->start->node,datas,&stack_shift);
	datas->right_addr_max++;
	ast_write("PRI", addr, -1, -1, datas);
}

//Declaration et affectation d'une constante
void ast_const_build(ast_node * node, build_data * datas){
	name_info * info = scp_contains_floor(datas->var_list,(char *)(node->content));
	int rightAddr;
	int stack_shift=0;
	if(info != NOT_FOUND){//Vérifier que ce nom n'est pas déjà déclaré dans ce scope
		printf("Semantic error : variable [ %s ] is already declared\n",(char *)(node->content));
		//TODO lever une erreur et quitter?
	}
	else{
		ast_node * right = node->childs->start->node;
		rightAddr = addr_resolve(right, datas, &stack_shift);
		scp_add(datas->var_list, (char *)(node->content), AST_TYPESIZE_INT, datas->left_addr_min, NS_CONSTANT);
		ast_write("COP", datas->left_addr_min, rightAddr, -1,datas);
		datas->left_addr_min ++;
		datas->right_addr_max += stack_shift;
	}

}

//Fonction d'interprétation d'une expression à deux opérandes. Vérifie la nature des opérandes pour ajuster les opérations sur la stack
void ast_math_build(char * op,ast_node * node, build_data * datas){
	ast_node_cell * left = node->childs->start;
	ast_node_cell * right = left->suiv;
	int leftAddr;
	int rightAddr;
	int stack_shift = 0;
	rightAddr = addr_resolve(right->node, datas, &stack_shift);
	leftAddr = addr_resolve(left->node, datas, &stack_shift);
	int addr = datas->right_addr_max+stack_shift-1;//Le résultat est "placé" dans la pile après "consommation" des deux opérandes
	ast_write(op, addr, leftAddr, rightAddr,datas);//écriture de l'opération dans le fichier assembleur
	datas->right_addr_max = addr;
}

void ast_ref_build(ast_node * node, build_data * datas){
	name_info * info = scp_contains(datas->var_list,(char*)(node->content));//On récupère l'index de la variable dans la liste des noms déclarés
	if(info == NOT_FOUND){
		printf("Semantic error : variable [ %s ] referenced before declaration\n",(char *)(node->content));
	}
	else{
		datas->right_addr_max--;
		ast_write("AFC",datas->right_addr_max,info->addr,-1,datas);
	}
}
void ast_unref_build(ast_node * node, build_data * datas){
	int_str * istr = (int_str *)(node->content);
	name_info * info = scp_contains(datas->var_list,istr->s);//On récupère l'index de la variable dans la liste des noms déclarés
	if(info == NOT_FOUND){
		printf("Semantic error : variable [ %s ] referenced before declaration\n",istr->s);
	}
	else{
		datas->right_addr_max--;
		ast_write("CPA",datas->right_addr_max,info->addr,-1,datas);
		for(int i =0;i<istr->i-1;i++){
			ast_write("CPA",datas->right_addr_max,datas->right_addr_max,-1,datas);
		}
	}
}



void ast_int_build(ast_node * node, build_data * datas){
	datas->right_addr_max--;
	ast_write( "AFC", datas->right_addr_max,*((int*)(node->content)),-1,datas);
}

void ast_aff_build(ast_node * node, build_data * datas){
	ast_node_cell * left = node->childs->start;
	ast_node_cell * right = left->suiv;
	int rightAddr;
	int stack_shift = 0;
	int leftAddr;
	name_info * info = scp_contains(datas->var_list,(char*)(left->node->content));//On récupère l'index de la variable dans la liste des noms déclarés
	if(info == NOT_FOUND){
		printf("Semantic error : variable [ %s ] referenced before declaration\n",(char *)(left->node->content));
	}
	else if(info->status == NS_CONSTANT){
		printf("Semantic error : constant [ %s ] can't be affected outside declaration\n",(char *)(left->node->content));
	}
	else{
		leftAddr = info->addr;
	}
	info -> status = NS_ASSIGNED;
	rightAddr = addr_resolve(right->node, datas, &stack_shift);
	datas->right_addr_max+=stack_shift;
	ast_write("COP",leftAddr,rightAddr,-1,datas);
}

/*Fonction récursive principale d'interprétation de l'AST
paramètre (valeur initiale) => description

node (root) => le noeud à explorer

*/
void ast_node_build(ast_node * node, build_data * datas){
	if(datas->left_addr_min > datas->right_addr_max){//On vérifie que l'espace des variables ne dépasse pas dans la pile
		printf("Specifications error : programm compilation will require too much memory space\n");
		printf("Allow more memory, reduce number of variables or split/condense nested expressions\n");
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
			if(scp_contains_floor(datas->var_list,(char*)(node->content)) != NOT_FOUND){//On vérifie que ce nom n'est pas déjà déclaré dans ce scope
				printf("Semantic error : [ %s ] is already declared\n",(char*)(node->content));
				//TODO lever une erreur et quitter?
			}
			scp_add(datas->var_list,(char*)(node->content), AST_TYPESIZE_INT, datas->left_addr_min, NS_MUTABLE);//On ajoute ce nom à la liste des noms déclarés
			datas->left_addr_min++;//On décale l'index "d'écriture" de l'espace des variables déclarées
			printf("[DEBUG]DECLARE %s %d\n",(char *)(node->content),datas->left_addr_min);
			break;
		case AST_CODE_CON:
			ast_const_build(node,datas);
			printf("[DEBUG]CONST %s %d\n",(char *)(node->content),datas->left_addr_min);
			break;
		case AST_CODE_SEQ:
			for(ast_node_cell * cursor = node->childs->start;cursor!=NULL;cursor=cursor->suiv){
				ast_node_build(cursor->node,datas);
			}
			printf("[DEBUG]SEQOVER\n");
			break;
		case AST_CODE_IF:
			ast_if_build(node, datas);
			break;
		case AST_CODE_WHILE:
			ast_while_build(node,datas);
			break;
		case AST_CODE_INT:
			ast_int_build(node, datas);
			break;
		case AST_CODE_PRINT:
			ast_print_build(node,datas);
			break;
		case AST_CODE_UNREF:
			ast_unref_build(node,datas);
			break;
		case AST_CODE_REF:
			ast_ref_build(node,datas);
			break;
		case AST_CODE_VAR:
			printf("[DEBUG]Un noeud AST_CODE_VAR(référence à une variable) ne devrait pas être traité dans le switch...\n");
			//N'est pas supposé arrivé
			break;
		default:
			break;
	}
	
	if(datas->left_addr_min > datas->right_addr_max){//On vérifie à nouveau qu'il n'y a pas eu collision entre espace variable et stack
		printf("Specifications error : programm compilation will require too much memory space\n");
		printf("Allow more memory, reduce number of variables or split/condense nested expressions\n");
	}
}

//Ouvre un fichier assembleur et y écrit les opérations associées à l'interprétation sémantique de l'AST
void ast_build(ast * tree,const char * filename,int mem_size){
	FILE * file = fopen(filename,"w");
	scope * vars = scp_empty_root();
	build_data * data = (build_data *)malloc(1*sizeof(build_data));
	data->left_addr_min = 0;
	data->right_addr_max = mem_size;
	data->var_list = vars;
	data->file = file;
	data->line = 0;
	data->instructions = asm_instru_list_empty();
	ast_node_build(tree->root,data);//Lancement de la récursion
	asm_write_all(data);
	fclose(file);
}





