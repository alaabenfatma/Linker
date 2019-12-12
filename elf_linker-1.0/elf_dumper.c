//Voir declarations elf.h
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
typedef uint8_t HEAD[16]; 
 
int main(int argc, char *argv[]){
    HEAD header;
    //Il faut essayer avec le fichier foo.o
    FILE* file = fopen(argv[1], "rb");
  if(file) {
    fread(&header, 1, sizeof(header), file);
    if(header[0] == 0x7f &&
       header[1] == 'E' &&
       header[2] == 'L' &&
       header[3] == 'F') {
       printf("Magique: ");
       for(int i = 0;i<16;i++){
           printf("%02x ",header[i]);
       }
       printf("\n");
     }
     else{
         printf("Not a valid ELF format.\n");
         return 1;
     }

    fclose(file);
  }
  else{
      printf("Erreur d'ouverture du fichier '%s'\n",argv[1]);
  }
    return 0;
}