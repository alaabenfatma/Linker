//Voir declarations elf.h
#include <inttypes.h>

#include <stdio.h>

#include <stdlib.h>

typedef uint8_t HEAD[16];

void read_magic(FILE *file){
HEAD header;
  //Il faut essayer avec le fichier foo.o

  if (file) {
    fread( & header, 1, sizeof(header), file);
    if (header[0] == 0x7f && header[1] == 'E' && header[2] == 'L' && header[3] == 'F') {
      printf("Magique: ");
      for (int i = 0; i < 16; i++) {
        printf("%02x ", header[i]);
      }
      printf("\n");
    } else {
      printf("Not a valid ELF format.\n");
      exit(1);
    }

    fclose(file);
  } else {
    printf("Erreur d'ouverture du fichier\n.");
  }
}
int main(int argc, char * argv[]) {
    FILE * file = fopen(argv[1], "rb");
    read_magic(file);
  return 0;
}