#include "brain.h"
#include <string.h>

void displayHelp(){
   printf("-h \t Display the ELF file header \n");
   printf("-S \t Display the sections' header \n");
   printf("-s \t Display the symbol table \n");
   printf("-x \t Dump the contents of section <number> as bytes \n");
   printf("-r \t Displays the reimplementation table \n");
}

//retourne 1 si il y a un argument invalide
int hasBadArguments(int argc, char* argv[]){
   if(argc<3){
      return 1;
   }
   int i=1;
   while(i<argc-1){
      if((strcmp(argv[i],"-h")!=0) && (strcmp(argv[i],"-s")!=0) && (strcmp(argv[i],"-r")!=0) && (strcmp(argv[i],"-S")!=0) && (strcmp(argv[i],"-x")!=0)){
         return 1;
      }
      i++;
   }
}

//retourne 1 si arg est present en argument (sinon 0)
int isPresent(char* arg, int argc, char* argv[]){
   int i=1;
   while((i<argc-1) && (strcmp(argv[i],arg)!=0)){
      i++;
   }
   if(i<argc-1){
      return 1;
   }
   else{
      return 0;
   }
}

int main(int argc, char *argv[])
{
   if(hasBadArguments(argc,argv)==1){
      displayHelp();
      return 0;
   }
   FILE * file = fopen(argv[argc-1], "rb");
   if (file == NULL) {
      printf("Erreur lors de l'ouverture du fichier");
      return 1;
   }
   if(isPresent("-h",argc,argv)){
      etape1(file);
   }
   if(isPresent("-S",argc,argv)){
      etape2(file);
   }
   if(isPresent("-x",argc,argv)){
      printf("Entrer le numero de section: ");
      int x = 0;
      scanf("%d", &x);
      printf("Section number : ");
      etape3(file,x);
   }
   if(isPresent("-s",argc,argv)){
      etape4(file);
   }
   if(isPresent("-r",argc,argv)){
      etape5(file);
   }
}
