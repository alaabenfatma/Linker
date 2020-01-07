#include "elf_dumper.c"
//headers
Elf32_Ehdr *header1;
Elf32_Ehdr header2;
//PROGBITS
Elf32_Shdr *sec_first;
Elf32_Shdr *sec_seconds;
int main(int argc, char *argv[])
{
   FILE *file1 = fopen(argv[1], "rb");
   FILE *file2 = fopen(argv[2], "rb");
   header1=load_data(file1,sec_first);
   dump_sections(file1,header1,-1,NULL);
   
   fclose(file1);
   fclose(file2);
   return 0;
}   