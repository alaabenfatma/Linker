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

### Examples
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

