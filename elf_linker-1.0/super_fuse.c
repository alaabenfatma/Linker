#include "brain.h"

FusedHeader header;
FusedSection section;
Elf32_Sym symb;
Elf32_Rela rela;
Elf32_Rel rel;

int main(int argc, char * argv[]) {
  if (argc != 4) {
    printf("\nNombre d'arguments incorrect : elffile1 elffile2 elffileout\n");
    exit(1);
  }
  FILE * elffile1 = fopen(argv[1], "rb");
  FILE * elffile2 = fopen(argv[2], "rb");
  FILE * elffileout = fopen(argv[3], "wb");
  if (elffile1 == NULL || elffile2 == NULL) {
    printf("Erreur lors de l'ouverture du fichier");
    exit(1);
  }
  //le header a ecrire dans le fichier se fait a la toute fin
  fread( & header.header1, 1, sizeof(Elf32_Ehdr), elffile1);
  fread( & header.header2, 1, sizeof(Elf32_Ehdr), elffile2);
  
  int ancient_offset = header.header1.e_shoff;
  header.header1.e_shoff+=sizeof(header.header2);
  fwrite(&header,sizeof(Elf32_Ehdr),1,elffileout);
   for (int i = 0; i < 12; i++) { //on parcours les sections du fichier 1 & fichier 2
    fseek(elffile1, ancient_offset + i* sizeof(Elf32_Shdr), SEEK_SET);
    fread(&section.section1, 1, sizeof(section), elffile1);
    fseek(elffile2, header.header2.e_shoff + i* sizeof(Elf32_Shdr), SEEK_SET);
    fread(&section.section2, 1, sizeof(section), elffile2);
     //fwrite(&section,sizeof(FusedSection),1,elffileout);
    
   if(section.section1.sh_type == 0x1 
    &&  section.section2.sh_type == 0x1){
    if (section.section1.sh_name == section.section2.sh_name) { //faut faire un test ici pour comparer les noms (pas les types) des sections
      printf("I was at:%d\n",ftell(elffileout));
      //fseek(elffileout,header.header1.e_shoff+ i* sizeof(Elf32_Shdr),SEEK_SET);
     
      fwrite(&section.section1,sizeof(Elf32_Shdr),1,elffileout);
      fwrite(&section.section2,sizeof(Elf32_Shdr),1,elffileout);
     printf("I am now at:%d\n",ftell(elffileout));
      printf("We are writing the section");
    }}
    

  }
}