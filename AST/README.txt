Construction de l'Abstract Syntax Tree via des structures C.
Lors de l'analyse syntaxique yacc, on construit les noeuds de l'AST et les relie entre eux.
Ensuite, on appelle une fonction build sur l'ast construit(après yyparse() donc) qui génère le code assembleur dans un fichier assembly.asm

COMPILATION : 

Sous linux : "make"
Sous windows : "mingw32-make CONFIG=win" (ou toute autre version de make for windows)

ASSEMBLEUR :

Pour l'instant, l'assembleur est généré en "clair" avec les noms des opérations. Les opérandes vides(par exemple pour la commande COP) sont représentées par -1.

TODO-LIST :
/!\ Certains de ces points nécessiteront de modifier légèrement le fonctionement des name_list(plus d'infos que juste le nom/index) /!\ => FAIT le 26/03
0 :
	-nombres "en dur" (pas les variables) => FAIT le 24/03
	-const (vérifier leur instanciation/non-modif : un namespace réservé/des infos en plus dans les name_list?) => FAIT le 26/03
1 :
	-if/else : créer un type de noeud condition/comparaison, et un noeud de type if à 3 opérandes (condition,sequence,sequence)
	/!\ va demander de gérer les jumps et les numéros de ligne assembleurs /!\ => FAIT le 31/03 + while
	-gérer les scopes (i.e "niveau","altitude" d'un nom de variable) exemple : ne pas pouvoir référencer une variable déclarée dans un if dans le else qui suit => structures C codées, reste à intégrer à l'ast(après implémentation du if-else probablement) => FAIT le 18/04
2:
	-pointeurs : demande, pour le déréférencement, une instruction CPA @A @@B _ qui copie à l'addresse A la valeur contenue à l'addresse contenue à l'addresse B => Fait le 14/05
	-fonctions : élargir les possibilités de expr, créer un noeud fonction à n arguments => Requiert une nouvelle instru assembleur si on ne veut pas recopier "bêtement" les mêmes instrus pour émuler une fonction. Par exemple l'ajout d'un e fonction "JMA @operande" qui sauterait à l'addresse de la valeur contenu à l'addresse indiquée en argument permettrat de faire fonctionner une pile d'addresse de retour pour intégrer le mécanisme de fonction.
3:
	-gérer finement les erreurs => partiellement géré
4:
	-ajouter des types => un champ "size" est disponible dans les infos sur les variables pour un typage futur
5:
	-optimisation soyons fou

