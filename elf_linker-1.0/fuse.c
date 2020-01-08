#include "crawler.c"
Elf32_Shdr current_sec;
Elf32_Ehdr current_header;
FILE *final;
typedef struct
{
    unsigned char *buff;
    Elf32_Shdr section;
} Pure_Section;
Pure_Section *Pure_Sections;
int n = 0;
void printbin(unsigned char *buff)
{
    for (size_t j = 0; j <= strlen(buff); j++)
    {
        fputc(buff[j], final);
    }
}
void printb(unsigned char *buff)
{
    for (size_t j = 0; j <= strlen(buff) + 2; j++)
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
        if (Sections1[i].sh_type != 1)
            continue;
        printf("Section (%d): %s\n", i, sec_names + Sections1[i].sh_name);

        unsigned char *buff = malloc(sizeof(unsigned char) * Sections1[i].sh_size);
        fseek(file1, Sections1[i].sh_offset, SEEK_SET);
        fread(buff, Sections1[i].sh_size, 1, file1);
        printb(buff);

        printf("\n");
    }
}
void surf_sections2()
{
    int i = 0;
    for (i = 0; i <= sec_2_count; i++)
    {
        if (Sections2[i].sh_type != 1)
            continue;
        printf("Section (%d): %s\n", i, sec_names + Sections2[i].sh_name);

        unsigned char *buff = malloc(sizeof(unsigned char) * Sections2[i].sh_size);
        fseek(file2, Sections2[i].sh_offset, SEEK_SET);
        fread(buff, Sections2[i].sh_size, 1, file2);
        printb(buff);

        printf("\n");
    }
}
void fuse_sections()
{

    for (size_t i = 0; i < sec_1_count; i++)
    {
        for (size_t j = 0; j < sec_2_count; j++)
        {
            if (Sections1[i].sh_type == Sections2[j].sh_type && Sections2[j].sh_type == 1)
            {
                if (!strcmp(sec_names + Sections1[i].sh_name, sec_names + Sections2[j].sh_name))
                {
                    unsigned long size = sizeof(unsigned char) * Sections1[i].sh_size;
                    unsigned char *buff0 = malloc(sizeof(unsigned char) * Sections1[i].sh_size);
                    unsigned char *buff1 = malloc(sizeof(unsigned char) * Sections2[j].sh_size);

                    fseek(file1, Sections1[i].sh_offset, SEEK_SET);
                    fseek(file2, Sections2[j].sh_offset, SEEK_SET);

                    fread(buff0, Sections1[i].sh_size+1, 1, file1);
                    fread(buff1, Sections2[j].sh_size+1, 1, file2);
                    Pure_Sections[n].buff = malloc(size);
                    Pure_Sections[n].buff = strcat(buff0, buff1);
                    printb(Pure_Sections[n].buff);
                    printbin(Pure_Sections[n].buff);
                }
            }
        }
    }
}


Elf32_Sym symb1, symb2;
Elf32_Shdr section1, section2;
void fuse_symbols()
{
  int indice_tab_symb1 = get_tab_symb(file1, 0);
  if(indice_tab_symb1 == -1){
    printf("Pas de table des symboles dans le fichier1");
    return;
  }
  int indice_tab_symb2 = get_tab_symb(file1, 0);
  if(indice_tab_symb2 == -1){
    printf("Pas de table des symboles dans le fichier2");
    return;
  }

  fseek(file1, header.e_shoff + indice_tab_symb1* sizeof(section1), SEEK_SET);	//on va a la section de la table des symboles
  fread(&section1, 1, sizeof(section1), file1);
  section_to_little_endian(&section1);
  int count1 = section1.sh_size / section1.sh_entsize;	//nombre d'entree dans la table des symboles

  fseek(file2, header.e_shoff + indice_tab_symb2* sizeof(section2), SEEK_SET);	//on va a la section de la table des symboles
  fread(&section2, 1, sizeof(section2), file2);
  section_to_little_endian(&section2);
  int count2 = section2.sh_size / section2.sh_entsize;	//nombre d'entree dans la table des symboles

  fseek(file1, section1.sh_offset, SEEK_SET);
  fseek(file2, section2.sh_offset, SEEK_SET);
  for (int i = 0; i < count1; i++)
  {
     fread(&symb1, 1, sizeof(symb1), file1);
     symbol_to_little_endian();
     switch (ELF32_ST_BIND(symb1.st_info)){
           case STB_LOCAL:
           //ecrire dans la table des symboles resultat si le symbole ne s'y trouve pas deja
           break;
           case STB_GLOBAL:
           //On regarde si deux symboles on le meme nom (symb1._st_name == symb2._st_name) entre les deux fichiers d'entrée, si oui alors l'edition de lien echoue
           //Si un meme symbole est defini dans un fichier et indefini (UND) dans l'autre, on ecrit que la definition dans le fichier resultat
           //Si le meme symbole est UND dans les deux fichiers, on ne le met qu'une fois dans le fichier resultat
           //si un symbole n'apparait que dans un des deux fichiers on l'ecrit dans la table du fichier resultat
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




  for (int i = 0; i < count2; i++)
  {
     fread(&symb2, 1, sizeof(symb2), file2);
     symbol_to_little_endian();
     switch (ELF32_ST_BIND(symb1.st_info)){
           case STB_LOCAL:
           //ecrire dans la table des symboles resultat si le symbole ne s'y trouve pas deja
           break;
           case STB_GLOBAL:
           //On regarde si deux symboles on le meme nom (symb1._st_name == symb2._st_name) entre les deux fichiers d'entrée, si oui alors l'edition de lien echoue
           //Si un meme symbole est defini dans un fichier et indefini (UND) dans l'autre, on ecrit que la definition dans le fichier resultat
           //Si le meme symbole est UND dans les deux fichiers, on ne le met qu'une fois dans le fichier resultat
           //si un symbole n'apparait que dans un des deux fichiers on l'ecrit dans la table du fichier resultat
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
    fclose(file1);
    fclose(file2);
    fclose(final);
    return 0;
}
