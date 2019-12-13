#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

int main(int argc, char *argv[]);
void etape1(FILE *f);
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
void get_sh_name();
void get_sh_type();
void load_data(FILE *file);
void surf_sections();
void type_fichier();