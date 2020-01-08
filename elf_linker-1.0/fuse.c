#include "crawler.c"
Elf32_Shdr current_sec;
Elf32_Ehdr current_header;

Elf32_Shdr *Pure_Section;
void surf_sections1(){
    int i = 0;
   for(i=0;i<=sec_1_count;i++){
       if(Sections1[i].sh_type!=1)
        continue;
         printf("Section (%d): %s\n",i,sec_names+Sections1[i].sh_name);
    
        unsigned char *buff = malloc(sizeof(unsigned char)*Sections1[i].sh_size);
        fseek(file1,Sections1[i].sh_offset,SEEK_SET);
        fread(buff,Sections1[i].sh_size,1,file1);
        for (size_t j = 0; j < strlen(buff); j++)
        {
             if (buff[j] >= ' ' && buff[j] <= '~')
                printf("%c",buff[j]);
            else
            {
                printf(".");
            }
            
        }
        
         printf("\n");
    }
   

   
}
void surf_sections2(){
    int i = 0;
   for(i=0;i<=sec_2_count;i++){
       if(Sections2[i].sh_type!=1)
        continue;
         printf("Section (%d): %s\n",i,sec_names+Sections2[i].sh_name);
   
        unsigned char *buff = malloc(sizeof(unsigned char)*Sections2[i].sh_size);
        fseek(file2,Sections2[i].sh_offset,SEEK_SET);
        fread(buff,Sections2[i].sh_size,1,file2);
        for (size_t j = 0; j < strlen(buff); j++)
        {
             if (buff[j] >= ' ' && buff[j] <= '~')
                printf("%c",buff[j]);
            else
            {
                printf(".");
            }
            
        }
        
         printf("\n");
    }
   
   
}
int main(int argc, char *argv[])
{ 
    
    ndx = 1;
    crawl(argv[1], "tmp0");
    
    ndx= 2;
    crawl(argv[2], "tmp1");
    file1 = fopen(argv[1],"rb");
    file2 = fopen(argv[2],"rb");
    surf_sections1();
    printf("\n--\n");
    surf_sections2();
    return 0;
}