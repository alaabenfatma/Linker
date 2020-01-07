#include "godbolt.c"

int main(int argc,char *argv[]){
    godbolt(argv[1],"tmp0");
    godbolt(argv[2],"tmp1");
    FILE *f = fopen("tmp0","rb");
    fread(&section,1,sizeof(Elf32_Shdr),f);
    etape2(f);
    return 0;
}