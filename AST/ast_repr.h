#ifndef H_AST_REPR
#define H_AST_REPR

#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

void ast_display(ast * tree);//Affiche l'ast, un niveau de profondeur par ligne. Chaque noeud est représenté sous la forme [id|code|id_du_père]

#endif