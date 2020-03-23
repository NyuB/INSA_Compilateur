#include <stdlib.h>
#include <stdio.h>
#include "name_list.h"
#include "ast.h"
#include "ast_repr.h"
#include "scope.h"
int main(int argc, char ** argv)
{
	printf("Test AST build\n");

	printf("Test name_list\n");
	name_list * list = nli_empty();
	nli_append(list,"a");
	nli_append(list,"abcd");
	nli_display(list);
	if(nli_contains(list,"abcd")!=1){
		printf("List should contain 'abcd' at index 1\n");
	}
	if(nli_contains(list,"b")!=NOT_FOUND){
		printf("List should not contain 'b'\n");
	}

	printf("Test scope\n");
	scope * root_00 = scp_empty_root();
	scp_add(root_00,"root");
	scope * child_10 = scp_empty_contained(root_00);
	scp_add(child_10,"a");
	scp_add(child_10,"b");
	scope * child_11 = scp_empty_contained(root_00);
	scp_add(child_11,"b");
	scope * child_20 = scp_empty_contained(child_10);
	scp_add(child_20,"c");
	printf("CH 20\n");
	scp_display(child_20);
	printf("CH 10\n");
	scp_display(child_10);
	printf("CH 11\n");
	scp_display(child_11);
	printf("ROOT\n");
	scp_display(root_00);
	printf("Test ast\n");
	
	//Construction d'un arbre équivalent au programme : {int a; int b; a = b * (a+b)}
	ast_node * a_dcl = ast_declare("a");
	ast_node * b_dcl = ast_declare("b");
	ast_node * a = ast_var("a");
	ast_node * b = ast_var("b");
	ast_node * a_bis = ast_var("a");
	ast_node * b_bis = ast_var("b");
	ast_node * plus = ast_math(1,a,b);
	ast_node * mul = ast_math(2,plus,b_bis);//b*(a+b)
	ast_node * a_asn = ast_affect(a_bis,mul);

	//Construction de la séquence d'instruction (le body) et de l'arbre
	ast_node_list * ast_list = ast_node_list_empty();
	ast_node_list_append(ast_list,a_dcl);
	ast_node_list_append(ast_list,b_dcl);
	ast_node_list_append(ast_list,a_asn);
	ast * tree = ast_new(ast_node_seq(ast_list));

	//Affichage et test compilation
	ast_display(tree);
	ast_build(tree,"test.asm",200);
	return 0;
}