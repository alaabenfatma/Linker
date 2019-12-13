#include <inttypes.h>

#include <stdio.h>

#include <stdlib.h>
#include <elf.h>

//local header
Elf64_Ehdr header;
void read_magic(FILE *file){
  //Il faut essayer avec le fichier foo.o

  if (file) {
    fread( & header, 1, sizeof(header), file);
    if (header.e_ident[0] == 0x7f && header.e_ident[1] == 'E' && header.e_ident[2] == 'L' && header.e_ident[3] == 'F') {
      printf("Magique: ");
      for (int i = 0; i < 16; i++) {
        printf("%02x ", header.e_ident[i]);
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
void get_class(){
    if(header.e_ident[4]==0x0){
        printf("Classe:\tN'est pas valable.\n");
    }
    else if(header.e_ident[4]==0x1){
        printf("Classe:\tELF32\n");
    }
    else if(header.e_ident[4]==0x2){
        printf("Classe:\tELF64\n");
    }
}
void get_donnees(){
    if(header.e_ident[5]==0x0){
        printf("Données: Format de données inconnu.\n");
    }
    else if(header.e_ident[5]==0x1){
        printf("Données: Complément à deux, petit boutiste.\n");
    }
    else if(header.e_ident[5]==0x2){
        printf("Données: Complément à deux, gros boutiste.\n");
    }
}
void get_e_version(){
    if(header.e_ident[6]==0x0){
        printf("e_version: 0 (e_version non valable)\n");
    }
    else if(header.e_ident[6]==0x1){
        printf("e_version: 1 (current)\n");
    }
}
void get_os(){
    switch(header.e_ident[7]){
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
    printf("e_version ABI: %x\n",header.e_ident[8]);
}
void type_fichier(){
    printf("Type: ");
    switch(header.e_type){
        case 0x0: printf("Type inconnu"); break;
        case 0x1: printf("REL"); break;
        case 0x2: printf("EXEC"); break;
        case 0x3: printf("DYN"); break;
        case 0x4: printf("CORE"); break;
    }
    printf("\n");
}

void get_machine(){
        printf("Machine Cible: ");
        switch(header.e_machine){
                case 0x0: printf("Aucune\n");break;
                case 0x2: printf("SPARC\n");break;
                case 0x3: printf("Intel 80386\n");break;
                case 0x4: printf("Motorola 68000\n");break;
                case 0x7: printf("Intel i860\n");break;
                case 0x8: printf("MIPS I\n");break;
                case 0x13: printf("Intel i960\n");break;
                case 0x14: printf("PowerPC\n");break;
                case 0x28: printf("ARM\n");break;
                case 0x32: printf("Intel IA64\n");break;
                case 0x3E: printf("x64\n");break;
                case 0xF3: printf("RISC-V\n");break;
                default: printf("\n");break;
        }
}

void get_e_version2(){
    printf("e_version: 0x%x \n", header.e_version);
}

int main(int argc, char * argv[]) {
    FILE * file = fopen(argv[1], "rb");
    read_magic(file);
    get_class();
    get_donnees();
    get_e_version();
    get_os();
    get_ABI();
    type_fichier();
    get_machine();
    get_e_version2();
  return 0;
}
