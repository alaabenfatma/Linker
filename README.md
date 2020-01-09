# Linker

## Introduction
Ce projet nous permet de faire l'édition de liens de fichiers .o

## Make
Afin de configurer le _Makefile_, nous devons exécuter cette commande :

```
autoreconf
```
De plus, nous devons ajouter ou modifier cette ligne dans le Makefile lui-même.

```
COMMON=debug.h debug.c util.h util.c brain.h  elf_dumper.c brain.c crawler.c fuse.c
```
### Compilation séparée des différentes parties du projet

```
make ${TARGET}
```

La variable TARGET doit être remplacée par l'une des options suivantes

* elf_dumper
* fuse

Voir le compte rendu pour plus de détails sur chaque TARGET.

### Exemples
Tous les exemples sont contenus dans le dossier *exemples*.

cmd:
```
./elf_dumper exemples/e1
```
out:
```
-h  Display the ELF file header 
-S  Display the sections' header 
-s  Display the symbol table 
-x  Dump the contents of section <number> as bytes 
-r  Displays the reimplementation table 
```
Une cmd pour exécuter tout le programme
```
./elf_dumper -x -s -S -h -r exemples/e1
```

Un exemple sur le fonctionnement de la fusion

```
./fuse examples
```
Afin de vérifier le contenu du fichier et vérifier les résultats, utilisez la commande *xxd*

```
xxd fichier.o
```

# Schéma de la structure du code
![Alt Text](https://github.com/alaabenfatma/Linker/blob/master/Untitled%20Diagram.png)

### LECTURE ELF

Pour ces étapes l'utilisateur rentre dans le terminal la commande qu'il souhaite executer :
./elf_dumper -h fichier.o   affiche le header
./elf_dumper -S fichier.o   affiche les entetes de sections
./elf_dumper -s fichier.o   affiche la table des symboles
./elf_dumper -x fichier.o   affiche le hontenu d'une section
./elf_dumper -r fichier.o   affiche les tables de redressage


Etape 1 :
-Affichage de toutes les informations contenues dans le header.

Etape 2 :
-Affichage dans le terminal de chaque section du fichier elf :
--- NEW SECTION ---
Numéro :
Nom :
.
.
.

Etape 3 : 
-Affiche le contenu de la section donnée par son numéro en entrée dans le terminal

Etape 4 :
-Affichage de la table des symboles et son nombre d'entrées, pour chaque symbole affiche : son numéro, son nom, sa valeur, sa taille ect.

Etape 5 :
-Affiche les tables de redressage et leur contenu.
Ne fonctionne que sur le little endian (probleme avec le big endian pas eu le temps de le corriger)


------------------------------------------------------------FUSION-----------------------------------------------


Etape 6 :
-Fusionne les sections PROGBITS des deux fichiers d'entrée et les fusionne dans un fichier de sortie.

Etape 7:
-Capable de verifier les symboles des sections et de ne recuperer que les symboles a mettre dans la table du fichier de sortie.
Probleme pour les ecrire dans le fichier.

Etape 8, 9 : Non réalisées.
Partie sur les tests:
  Afin de vérifier les bons résultats du programme, nous avons implémenté un programme Bash ( checker.sh )
qui va comparer le résultat avec celui de readelf. Chaque étape peut être vérifiée indépendamment et si 
jamais le résultat de elf_dumper n’est pas correct, le programme nous indique de quelle différence il s’agit.  

----------------------------------------------Journal de bord---------------------------------------------------

Semaine 1 : du 12 au 20 décembre 2019
- Lecture du sujet, prise en main des fichiers fournis.
- Implémentation de la phase 1 : étape 1, 2,3,5 et partie 4 incomplète (fichier elf_dumper).
- Implémentation des tests (checker.sh) de l’étape 1, 2.
- Début de la phase 2(fichiers crawler.c et fuse.c).

Semaine 2 : du 6 au 12 Janvier 2020
 -Continuation de la phase 1 : Ajout du nom des fonctions dans l’affichage d’une section de l’étape 4.
- Correction des bugs de l’étape 5.
- Continuation de la phase 2: Etape 6 et Etape 7.
- Implémentation des tests ( checker.sh ) de l’étape 4. 

