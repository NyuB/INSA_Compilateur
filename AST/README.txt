Version alternative en construisant l'Abract Syntax Tree via des structures C.
Lors de l'analyse syntaxique yacc, on construit les noeuds de l'AST et les relie entre eux.
Ensuite, on appelle une fonction build sur l'ast construit(après yyparse() donc) qui génère le code assembleur dans un fichier assembly.asm

COMPILATION : 

Sous linux : "make"
Sous windows : "mingw32-make CONFIG=win" (ou toute autre version de make for windows)

ASSEMBLEUR :

Pour l'instant, l'assembleur est généré en "clair" avec les noms des opérations.

TODO :
/!\ Certains de ces points nécessiteront de modifier légèrement le fonctionement des name_list(plus d'infos que juste le nom/index) /!\
0 :
	-nombres "en dur" (pas les variables)
	-const (vérifier leur instanciation/non-modif : un namespace réservé/des infos en plus dans les name_list?)
1 :
	-if/else
	-gérer les scopes (i.e "niveau","altitude" d'un nom de variable) exemple : ne pas pouvoir référence une variable déclarée dans un if dans le else qui suit
2:
	-pointeurs
	-fonctions
3:
	-optimisation soyons fou

