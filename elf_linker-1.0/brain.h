#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <byteswap.h>
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
int ENDIAN = 0; //var globale => 0 = little | 1 = bug
//Etape 1 interfaces
void dump_section(FILE *f,Elf32_Ehdr header,Elf32_Shdr section);
void dump_sections(FILE *file,Elf32_Ehdr header,Elf32_Shdr section,Elf32_Word special_ndx,Elf32_Shdr *secs);
void etape1(FILE *f,Elf32_Ehdr *header);
void etape2(FILE *f,Elf32_Ehdr *header);
void etape3(FILE *f, int x,Elf32_Shdr section,Elf32_Ehdr header);
void etape4(FILE *file,Elf32_Shdr  section,Elf32_Ehdr header);
void etape5(FILE *file,Elf32_Shdr section ,Elf32_Ehdr header);
void get_ABI(Elf32_Ehdr *header);
void get_class(Elf32_Ehdr *header);
void get_donnees(Elf32_Ehdr *header);
void get_e_ehsize(Elf32_Ehdr *header);
void get_e_entry(Elf32_Ehdr *header);
void get_e_flags(Elf32_Ehdr *header);
void get_e_phentsize(Elf32_Ehdr *header);
void get_e_phnum(Elf32_Ehdr *header);
void get_e_phoff(Elf32_Ehdr *header);
void get_e_shentsize(Elf32_Ehdr *header);
void get_e_shnum(Elf32_Ehdr *header);
void get_e_shoff(Elf32_Ehdr *header);
void get_e_shstrndx(Elf32_Ehdr *header);
void get_e_version2(Elf32_Ehdr *header);
void get_e_version(Elf32_Ehdr *header);
void get_machine(Elf32_Ehdr *header);
void get_magic(Elf32_Ehdr *header);
void get_os(Elf32_Ehdr *header);
void get_section_names(FILE *file,Elf32_Shdr section);
void get_sh_name(Elf32_Shdr *section);
void get_sh_type(Elf32_Shdr *section);
int get_tab_symb(FILE *file, Elf32_Shdr section,Elf32_Ehdr header);
void header_to_little_endian(Elf32_Ehdr *header);
Elf32_Ehdr  load_data(FILE *file, Elf32_Shdr *section);
int main(int argc, char *argv[]);
void rel_to_little_endian();
void rela_to_little_endian();
void section_to_little_endian(Elf32_Shdr *section);
void surf_sections(Elf32_Shdr *section);
void symbol_to_little_endian();
void type_fichier(Elf32_Ehdr *header);
//Etape 2 interfaces
typedef struct{
    Elf32_Ehdr header1;
    Elf32_Ehdr header2;
}FusedHeader;
typedef struct{
    Elf32_Shdr section1;
    Elf32_Shdr section2;
}FusedSection;