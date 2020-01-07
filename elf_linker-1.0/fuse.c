#include "crawler.c"

int main(int argc,char *argv[]){
    crawl(argv[1],"tmp0");
    crawl(argv[2],"tmp1");
    FILE *f = fopen("tmp0","rb");
    fread(&section,1,sizeof(Elf32_Shdr),f);
    etape2(f);
    return 0;
}