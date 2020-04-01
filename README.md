# Dépôt/Archive du projet compilateur de 4IR INSA Toulouse
# Groupe Dimitrios Doganis/Brice Decaestecker

# Contenu :

- Dossier proto : les premières implémentations de lexer/compilateur exécutant l'écriture assembleur dans le fichier yacc
- Dossier AST : le projet après bifurcation "moins de yacc plus de c", on utilise une structure d'arbre(voir le fichier ast.h/c) construite dans le yacc puis une fonction d'interprétation qui écrit le fichier assembleur une fois l'analyse syntaxique terminée.
- simulator-mem : l'interpréteur assembleur fourni
