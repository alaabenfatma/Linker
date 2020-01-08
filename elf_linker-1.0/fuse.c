#include "crawler.c"
Elf32_Shdr current_sec;
Elf32_Ehdr current_header;
FILE *final;
char progbits[1000];
int n = 0;
int symb_n = 0;
void append_progbits(char hex[])
{
  strcat(progbits, hex);
}
void printbin(unsigned char *buff, int len)
{
  for (size_t j = 0; j <= len; j++)
  {
    fputc(buff[j], final);
  }
}
void printb(unsigned char *buff, int len)
{
  for (size_t j = 0; j <= len; j++)
  {
    if (buff[j] >= ' ' && buff[j] <= '~')
      printf("%c", buff[j]);
    else
    {
      printf(".");
    }
  }
}
void surf_sections1()
{
  int i = 0;
  for (i = 0; i <= sec_1_count; i++)
  {
    printf("\nSection (%d): %s\n", i, sec_names + Sections1[i].section.sh_name);

    unsigned char *buff = malloc(sizeof(unsigned char) * Sections1[i].section.sh_size);
    fseek(file1, Sections1[i].section.sh_offset, SEEK_SET);
    fread(Sections1[i].buff, Sections1[i].section.sh_size, 1, file1);
    printb(Sections1[i].buff, Sections1[i].section.sh_size);
  }
}
void surf_sections2()
{
  int i = 0;
  for (i = 0; i <= sec_2_count; i++)
  {
    printf("Section (%d): %s\n", i, sec_names + Sections2[i].section.sh_name);

    unsigned char *buff = malloc(sizeof(unsigned char) * Sections2[i].section.sh_size);
    fseek(file2, Sections2[i].section.sh_offset, SEEK_SET);
    fread(Sections2[i].buff, Sections2[i].section.sh_size, 1, file2);
    printb(Sections2[i].buff, Sections2[i].section.sh_size);

    printf("\n");
  }
}
void fuse_sections()
{
  int limit = 0;
  unsigned char *buff = malloc(sizeof(unsigned char));
  unsigned char *buff0;
  unsigned char *buff1;
  for (size_t i = 0; i < sec_1_count; i++)
  {

    for (size_t j = 0; j < sec_2_count; j++)
    {

      if (Sections1[i].section.sh_type == SHT_PROGBITS && Sections2[j].section.sh_type == SHT_PROGBITS)
      {
        printf("comarping names %s ? %s\n", sec_names + Sections1[i].section.sh_name, sec_names2 + Sections2[j].section.sh_name);
        if (0 == strcmp((char *)(sec_names + Sections1[i].section.sh_name), (char *)(sec_names2 + Sections2[j].section.sh_name)))
        {
          printf("Writing section : %s\n", sec_names + Sections1[i].section.sh_name);
          Pure_Sections[n].section = Sections1[i].section;
          printbin(Sections1[i].buff, Sections1[i].section.sh_size);
          printbin(Sections2[j].buff, Sections2[j].section.sh_size);
          //printb(Sections2[j].buff,Sections1[i].section.sh_size+Sections2[j].section.sh_size);
          //printbin(Pure_Sections[n].buff,Sections1[i].section.sh_size+Sections2[j].section.sh_size);
          // fwrite(&Pure_Sections[n].section, sizeof(Pure_Sections[n].section), 1, final);

          n++;
          limit++;
        }
        else if (Sections2[j].section.sh_size == 0)
        {
          printf("Writing section 2: %s\n", sec_names + Sections1[i].section.sh_name);
          Pure_Sections[n].section = Sections1[i].section;
          printbin(Sections1[i].buff, Sections1[i].section.sh_size);
          n++;
          limit++;
        }
        else if (Sections1[i].section.sh_size == 0)
        {
          printf("Writing section 3: %s\n", sec_names2 + Sections2[j].section.sh_name);
          Pure_Sections[n].section = Sections2[j].section;
          printbin(Sections2[j].buff, Sections2[j].section.sh_size);
          n++;
          limit++;
        }
      }
      else
      {
        if (limit >= i)
          continue;
        if (Sections1[i].section.sh_type == SHT_PROGBITS)
          continue;
        limit++;
        printf("Writing section 2: %s\n", sec_names + Sections1[i].section.sh_name);
        Pure_Sections[n].section = Sections1[i].section;
        printbin(Sections1[i].buff, Sections1[i].section.sh_size);
        n++;
      }
    }
  }
}
void append_sections_to_file()
{
  for (size_t i = 0; i < n; i++)
  {
    fwrite(&Pure_Sections[i].section, sizeof(Pure_Sections[i].section), 1, final);
  }
}
Elf32_Sym *symbols;
Elf32_Shdr section1, section2;
void append_symbol(Elf32_Sym s)
{
  symbols[symb_n] = s;
  symb_n++;
}
void fuse_symbols()
{
  int indice_tab_symb1 = get_tab_symb(file1, 0);
  if (indice_tab_symb1 == -1)
  {
    printf("Pas de table des symboles dans le fichier1");
    return;
  }
  int indice_tab_symb2 = get_tab_symb(file2, 0);
  if (indice_tab_symb2 == -1)
  {
    printf("Pas de table des symboles dans le fichier2");
    return;
  }

  fseek(file1, header.e_shoff + indice_tab_symb1 * sizeof(section1), SEEK_SET); //on va a la section de la table des symboles
  fread(&section1, 1, sizeof(section1), file1);
  section_to_little_endian(&section1);
  int count1 = section1.sh_size / section1.sh_entsize; //nombre d'entree dans la table des symboles

  fseek(file2, header.e_shoff + indice_tab_symb2 * sizeof(section2), SEEK_SET); //on va a la section de la table des symboles
  fread(&section2, 1, sizeof(section2), file2);
  section_to_little_endian(&section2);
  int count2 = section2.sh_size / section2.sh_entsize; //nombre d'entree dans la table des symboles
  symbols = malloc(sizeof(Elf32_Sym) * max(count1, count2));
  fseek(file1, section1.sh_offset, SEEK_SET);
  fseek(file2, section2.sh_offset, SEEK_SET);

  for (int i = 0; i < count1; i++)
  {
    for (int i = 0; i < count2; i++)
    {
      Elf32_Sym symb1, symb2;
      fread(&symb1, 1, sizeof(symb1), file1);
      symbol_to_little_endian();
      fread(&symb2, 1, sizeof(symb2), file2);
      symbol_to_little_endian();
      switch (ELF32_ST_BIND(symb1.st_info))
      {
      case STB_LOCAL:
        //ecrire dans la table des symboles resultat si le symbole ne s'y trouve pas deja
        switch (ELF32_ST_BIND(symb2.st_info))
        {
        case STB_LOCAL:
          if (symb1.st_name == symb2.st_name)
          {
            //alors on ecrit que le symb1 dans la table resultat s'il n'y est pas deja
            append_symbol(symb1);
          }
          else
          {
            // on ecrit le symb1 et le symb2 dans la table resultat s'il n'y sont pas deja
            append_symbol(symb1);
            append_symbol(symb2);
          }
          break;
        case STB_GLOBAL:
          //on ecrit le symb1 dans la table resultat s'il n'y est pas deja
          append_symbol(symb1);
          break;
        }
        break;
      case STB_GLOBAL:
        //On regarde si deux symboles on le meme nom (symb1._st_name == symb2._st_name) entre les deux fichiers d'entrée, si oui alors l'edition de lien echoue
        //Si un meme symbole est defini dans un fichier et indefini (UND) dans l'autre, on ecrit que la definition dans le fichier resultat
        //Si le meme symbole est UND dans les deux fichiers, on ne le met qu'une fois dans le fichier resultat
        //si un symbole n'apparait que dans un des deux fichiers on l'ecrit dans la table du fichier resultat
        switch (ELF32_ST_BIND(symb2.st_info))
        {
        case STB_LOCAL:
          //on ecrit le symb1 dans la table resultat s'il n'y est pas deja
          append_symbol(symb1);
          break;
        case STB_GLOBAL:
          if (symb1.st_name == symb2.st_name)
          {
            if (symb1.st_shndx == symb2.st_shndx && symb2.st_shndx != SHN_UNDEF)
            {
              //l'edition de lien echoue completement
              printf("ERROR SAME NAME.");
              exit(1);
            }
            if (symb1.st_shndx == SHN_UNDEF && symb2.st_shndx != SHN_UNDEF)
            {
              //on ecrit le symb2 qui porte la definition
              append_symbol(symb2);
            }
            if (symb2.st_shndx == SHN_UNDEF && symb1.st_shndx != SHN_UNDEF)
            {
              //on ecrit le symb1 qui porte la definition
              append_symbol(symb1);
            }
            if (symb2.st_shndx == SHN_UNDEF && symb1.st_shndx == SHN_UNDEF)
            {
              //on ecrit l'un des deux symboles au choix
              append_symbol(symb1);
            }
          }
          else
          {
            //on ecrit le symb1
            append_symbol(symb1);
          }
          break;
        }
        break;
      }
      /*Pour regarder si le symbole est defini :
        switch(symb1.st_shndx){
              case SHN_UNDEF:		printf("Ndx: UND |");break;
              case SHN_BEFORE:		printf("Ndx: BEFORE |");break;
              case SHN_AFTER:		printf("Ndx: AFTER |");break;
              case SHN_HIPROC:		printf("Ndx: HIPROC |");break;
              case SHN_LOOS:		printf("Ndx: LOOS |");break;
              case SHN_HIOS:    	printf("Ndx: HIOS |");break;
              case SHN_ABS:		printf("Ndx: ABS |");break;
              case SHN_COMMON:		printf("Ndx: COMMON |");break;
              case SHN_HIRESERVE:	printf("Ndx: HIRESERVE |");break;
              default : printf("Ndx: %d |", symb1.st_shndx); break;
        }
        */
    }
  }
  fwrite(&symbols, sizeof(symbols), 1, final);
}

int main(int argc, char *argv[])
{
  Pure_Sections = malloc(sizeof(Pure_Section) * max(sec_1_count, sec_2_count));
  ndx = 1;
  crawl(argv[1], "tmp0");
  final = fopen("final.o", "wb");
  ndx = 2;
  crawl(argv[2], "tmp1");
  file1 = fopen(argv[1], "rb");
  file2 = fopen(argv[2], "rb");
  fclose(file1);
  fclose(file2);
  file1 = fopen(argv[1], "rb");
  file2 = fopen(argv[2], "rb");
  surf_sections1();
  printf("\n--\n");
  surf_sections2();

  printf("\n--\n");
  fuse_sections();
  fuse_symbols();
  append_sections_to_file();

  fclose(file1);
  fclose(file2);
  fclose(final);
  return 0;
}
