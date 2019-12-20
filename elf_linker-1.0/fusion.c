#include "brain.h"

Elf32_Ehdr header;
Elf32_Shdr section;
Elf32_Sym symb;
Elf32_Rela rela;
Elf32_Rel rel;

int main(int argc, char *argv[])
{
   if (argc != 4){
     printf("\nNombre d'arguments incorrect : elffile1 elffile2 elffileout\n");
     exit(1);
   }
   FILE *elffile1 = fopen(argv[1], "rb");
   FILE *elffile2 = fopen(argv[2], "rb");
   FILE *elffileout = fopen(argv[3], "w");
   if (elffile1 == NULL || elffile2 == NULL){
     printf("Erreur lors de l'ouverture du fichier");
     exit(1);
   }
   Elf32_Ehdr headerfile1;
   Elf32_Ehdr headerfile2;
   Elf32_Ehdr headerfileout;
   Elf32_Shdr sectionfile1;
   Elf32_Shdr sectionfile2;
   Elf32_Shdr sectionfileout;

   fread(&headerfile1,1, sizeof(header), elffile1);
   fread(&headerfile2,1, sizeof(header), elffile2);
   for (int i = 0; i < headerfile1.e_shnum; i++){
     fseek(elffile1, headerfile1.e_shoff + i* sizeof(section), SEEK_SET);
     fread(&sectionfile1, 1 , sizeof (section), elffile1);
     for (int j = 0; j < headerfile2.e_shnum; j++){
       fseek(elffile2, headerfile2.e_shoff + i* sizeof(section), SEEK_SET);
       fread(&sectionfile2, 1 , sizeof (section), elffile2);
       if (sectionfile1.sh_type == sectionfile2.sh_type ){ //faut faire un test ici pour comparer les noms (pas les types) des sections
         fseek(elffileout,headerfileout.e_shoff + i* sizeof(section),SEEK_SET );
         fwrite(&sectionfile1,1, sizeof(section), elffileout);
         fseek(elffileout,headerfileout.e_shoff + (i+1)* sizeof(section),SEEK_SET );
         fwrite(&sectionfile2,1, sizeof(section), elffileout);
       }
     }
   }
}
