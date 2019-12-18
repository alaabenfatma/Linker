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

void dump_sections(FILE *file, int i);
void load_data(FILE *file);
void get_section_names(FILE *file);
void etape1(FILE *f);
void etape2(FILE *f);
int main(int argc, char *argv[]);
void get_magic();
void get_class();
void get_donnees();
void get_e_version();
void get_os();
void get_ABI();
void type_fichier();
void get_machine();
void get_e_version2();
void get_e_entry();
void get_e_phoff();
void get_e_shoff();
void get_e_flags();
void get_e_ehsize();
void get_e_phentsize();
void get_e_phnum();
void get_e_shentsize();
void get_e_shnum();
void get_e_shstrndx();
void get_sh_name();
void get_sh_type();
void surf_sections();
