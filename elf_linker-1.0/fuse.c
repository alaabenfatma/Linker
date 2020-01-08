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