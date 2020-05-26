# Dépôt/Archive du projet compilateur de 4IR INSA Toulouse
# Groupe Dimitrios Doganis/Brice Decaestecker

# Structure :

- Dossier proto (_ancienne version_) : les premières implémentations de lexer/compilateur exécutant l'écriture assembleur dans le fichier yacc, conservés pour archive
- Dossier AST (_version courante_) : le projet après bifurcation "moins de yacc plus de c", on utilise une structure d'arbre(voir le fichier ast.h/c) construite dans le yacc puis une fonction d'interprétation qui écrit le fichier assembleur une fois l'analyse syntaxique terminée.
- simulator-mem : l'interpréteur assembleur fourni, légèrement modifié pour notre 

# Build

Les sources du dossier AST génèrent un exécutable comp.exe, celles du dossier simulator-mem un exécutable simulator.exe
Les deux projets se compilent via un makefile
Sous Linux :
'''
make
'''
Sous Windows :
'''
make CONFIG=win
'''

# Utilisation
## Compilateur :

### Appel
Par défaut, le compilateur s'éxécute en considérant uen taille mémoire de 64 bytes. Un argument peut être ajouté lors de l'appel pour faire varier ce paramètre.

Linux
```
./source.c | ./comp
Ou
./source.c | ./comp mem_size
```

Windows
```
more source.c | comp
more source.c | comp mem_size
```

### Affichage
Si l'analyse syntaxique est un succès, le programme affichera en fin d'exécution une représentation concise de l'AST du fichier source, un niveau par ligne.
```
Starting AST display
Format : [ID | Type | FatherID]
   [0|8|-1]
   [1|8|0]   [2|8|0]   [3|13|0]   [4|13|0]
   [5|8|1]   [6|8|1]   [7|6|1]   [8|5|1]   [9|6|2]   [10|5|2]   [11|7|3]   [12|7|4]
   [13|6|5]   [14|5|5]   [15|6|6]   [16|5|6]   [17|7|8]   [18|10|8]   [19|7|10]   [20|10|10]
   [21|7|14]   [22|10|14]   [23|7|16]   [24|10|16]
```
Sur cet affichage un noeud est représenté par trois nombres : son identifiant unique, son type (décrit dans le fichier ast.h), et l'identifiant de son noeud père.

## Simulateur
Appel identique

