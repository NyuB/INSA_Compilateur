#include <stdlib.h>
#include <stdio.h>
#include "name_list.h"
#include "ast.h"
#include "ast_repr.h"

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

	printf("Test ast\n");
	ast_node * a_dcl = ast_declare("a");
	ast_node * b_dcl = ast_declare("b");
	ast_node * a = ast_var("a");
	ast_node * b = ast_var("b");
	ast_node * plus = ast_math(1,a,b);
	ast_node * a_bis = ast_var("a");
	ast_node * a_asn = ast_affect(a_bis,plus);
	ast_node_list * ast_list = ast_node_list_empty();
	ast_node_list_append(ast_list,a_dcl);
	ast_node_list_append(ast_list,b_dcl);
	ast_node_list_append(ast_list,a_asn);
	ast * tree = ast_new(ast_node_seq(ast_list));
	ast_display(tree);
	ast * tree_bis = ast_new(plus);
	ast_build(tree_bis,"assembly.asm");

	/*
	*/
	return 0;
}