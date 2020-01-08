#ifndef BRAIN_H_   
#define BRAIN_H_


#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elf.h"
#include "util.h"

#define HEXA 16
#define DWORD 4
#define WORD 2
#define HWORD 1
//#undef htonl
//#undef ntohl
#define bswap_16(x) ((unsigned int)__builtin_bswap16(x))
#define bswap_32(x) ((unsigned int)__builtin_bswap32(x))
#define bswap_64(x) ((unsigned int)__builtin_bswap64(x))

//Etape 1 interfaces
void dump_sections(FILE *file, int i);
void etape1(FILE *f);
void etape2(FILE *f);
void etape3(FILE *f, int x);
void etape4(FILE *file);
void etape5(FILE *file);
void get_ABI();
void get_class();
void get_donnees();
void get_e_ehsize();
void get_e_entry();
void get_e_flags();
void get_e_phentsize();
void get_e_phnum();
void get_e_phoff();
void get_e_shentsize();
void get_e_shnum();
void get_e_shoff();
void get_e_shstrndx();
void get_e_version2();
void get_e_version();
void get_machine();
void get_magic();
void get_os();
void get_section_names(FILE *file);
void get_sh_name();
void get_sh_type();
int get_tab_symb(FILE *file, int i);
void header_to_little_endian();
void load_data(FILE *file);
int main(int argc, char *argv[]);
void rel_to_little_endian();
void rela_to_little_endian();
void section_to_little_endian();
void surf_sections();
void symbol_to_little_endian();
void type_fichier();
int has_multiple_declarations(FILE* f1, FILE* f2);
//Etape 2 interfaces
typedef struct{
    Elf32_Ehdr header1;
    Elf32_Ehdr header2;
}FusedHeader;
typedef struct{
    Elf32_Shdr section1;
    Elf32_Shdr section2;
}FusedSection;


#endif // BRAIN_H_
