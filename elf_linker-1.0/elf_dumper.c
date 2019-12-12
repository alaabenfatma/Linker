#include <inttypes.h>

#include <stdio.h>

#include <stdlib.h>

typedef struct{
    uint8_t ID[16];
    uint16_t File_Type;
    uint16_t Machine_Cible;
    uint16_t Version;

}HEAD;
//local header
HEAD header;
void read_magic(FILE *file){
  //Il faut essayer avec le fichier foo.o

  if (file) {
    fread( & header, 1, sizeof(header), file);
    if (header.ID[0] == 0x7f && header.ID[1] == 'E' && header.ID[2] == 'L' && header.ID[3] == 'F') {
      printf("Magique: ");
      for (int i = 0; i < 16; i++) {
        printf("%02x ", header.ID[i]);
      }
        printf("%02x ", header.File_Type);
        printf("%02x ", header.Machine_Cible);
        printf("%02x ", header.Version);
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
void get_class(){
    if(header.ID[4]==0x0){
        printf("Classe:\tN'est pas valable.\n");
    }
    else if(header.ID[4]==0x1){
        printf("Classe:\tELF32\n");
    }
    else if(header.ID[4]==0x2){
        printf("Classe:\tELF64\n");
    }
}
void get_donnees(){
    if(header.ID[5]==0x0){
        printf("Données: Format de données inconnu.\n");
    }
    else if(header.ID[5]==0x1){
        printf("Données: Complément à deux, petit boutiste.\n");
    }
    else if(header.ID[5]==0x2){
        printf("Données: Complément à deux, gros boutiste.\n");
    }
}
void get_version(){
    if(header.ID[6]==0x0){
        printf("Version: 0 (Version non valable)\n");
    }
    else if(header.ID[6]==0x1){
        printf("Version: 1 (current)\n");
    }
}
void get_os(){
    switch(header.ID[7]){
        case 0x0:
            printf("OS/ABI: UNIX System V");
            break;
        case 0x1:
            printf("OS/ABI: HP-UX");
            break;
        case 0x2:
            printf("OS/ABI: NetBSD");
            break;
        case 0x3:
            printf("OS/ABI: Linux");
            break;
        case 0x4:
            printf("OS/ABI: Linux");
            break;
        case 0x5:
            printf("OS/ABI: IRIX");
            break;
        case 0x6:
            printf("OS/ABI: FreeBSD");
            break;
        case 0x7:
            printf("OS/ABI: UNIX TRU64");
            break;
        case 0x8:
            printf("OS/ABI: de l'architecture ARM");
            break;
        case 0x9:
            printf("OS/ABI: autonome (embarqué)");
            break;
        default:
            break;
    }
    printf("\n");
}
void get_ABI(){
    printf("%x\n",header.ID[8]);
}
int main(int argc, char * argv[]) {
    FILE * file = fopen(argv[1], "rb");
    read_magic(file);
    get_class();
    get_donnees();
    get_version();
    get_os();
    get_ABI();
  return 0;
}
