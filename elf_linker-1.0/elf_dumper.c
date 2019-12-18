#include "brain.h"

//local header
Elf64_Ehdr header;
Elf64_Shdr section;
Elf64_Sym symb;
Elf64_Rela rela;
int indice_tab_symb = -1;
int table_rela[50];
int indice_tab_rela = 0;
char *sec_names = NULL;
char *symb_names = NULL;

void header_to_little_endian()
{
      if(ENDIAN==1){
      header.e_flags = reverse_4(header.e_flags);
      header.e_ehsize = reverse_2(header.e_ehsize);
      header.e_machine = reverse_2(header.e_machine);
      header.e_phentsize = reverse_2(header.e_phentsize);
      header.e_phnum = reverse_2(header.e_phnum);
      header.e_phoff = reverse_4(header.e_phoff);
      header.e_shentsize = reverse_2(header.e_shentsize);
      header.e_shnum = reverse_2(header.e_shnum);
      header.e_shoff = reverse_4(header.e_shoff);
      header.e_shstrndx = reverse_2(header.e_shstrndx);
      header.e_type = reverse_2(header.e_type);
      header.e_version = reverse_4(header.e_version);
      header.e_entry = reverse_4(header.e_entry);}
} 
void section_to_little_endian(){
   if(ENDIAN==1){
   section.sh_name = reverse_4(section.sh_name);
   section.sh_type = reverse_4(section.sh_type);
   section.sh_flags = reverse_4(section.sh_flags);
   section.sh_addr = reverse_4(section.sh_addr);
   section.sh_offset = reverse_4(section.sh_offset);
   section.sh_size = reverse_4(section.sh_size);
   section.sh_link = reverse_4(section.sh_link);
   section.sh_info = reverse_4(section.sh_info);
   section.sh_addralign = reverse_4(section.sh_addralign);
   section.sh_entsize = reverse_4(section.sh_entsize);}
}
void load_data(FILE *file)
{
   if (file)
   {
      if (!fread(&header, 1, sizeof(header), file))
      {
         printf("Error header");
         exit(1);
      }
      get_magic();
      get_class();
      get_donnees();
      header_to_little_endian();
      
      fseek(file, (header.e_shoff) + (header.e_shstrndx) * sizeof(section), SEEK_SET);
      if (!fread(&section, 1, sizeof(section), file))
      {
         printf("Error section");
         exit(1);
      }
      section_to_little_endian();
      if (header.e_ident[0] == 0x7f && header.e_ident[1] == 'E' && header.e_ident[2] == 'L' && header.e_ident[3] == 'F')
      {
         //ignore
      }
      else
      {
         printf("Not a valid ELF format.\n");
         exit(1);
      }
   }
   else
   {
      printf("Erreur d'ouverture du fichier\n.");
   }
}
void get_magic()
{
   printf("Magique: ");
   for (int i = 0; i < 16; i++)
   {
      printf("%02x ", header.e_ident[i]);
   }
   printf("\n");
}
void get_class()
{
   if (header.e_ident[4] == 0x0)
   {
      printf("Classe:\tN'est pas valable.\n");
   }
   else if (header.e_ident[4] == 0x1)
   {
      printf("Classe:\tELF32\n");
   }
   else if (header.e_ident[4] == 0x2)
   {
      printf("Classe:\tELF64\n");
   }
}
void get_donnees()
{
   if (header.e_ident[5] == 0x0)
   {
      printf("Données: Format de données inconnu.\n");
   }
   else if (header.e_ident[5] == 0x1)
   {
      printf("Données: complément à 2, système à octets de poids faible d'abord (little endian) \n");
   }
   else if (header.e_ident[5] == 0x2)
   {
      printf("Données: Complément à 2, système à octets de poids fort d'abord (big endian)\n");
   }
}
void get_e_version()
{
   if (header.e_ident[6] == 0x0)
   {
      printf("Version: 0 (e_version non valable)\n");
   }
   else if (header.e_ident[6] == 0x1)
   {
      printf("Version: 1 (current)\n");
   }
}
void get_os()
{
   switch (header.e_ident[7])
   {
      case 0x0:
         printf("OS/ABI: UNIX System V");
         break;
      case 0x1:
         printf("OS/ABI: HP-UX");
         break;
      case 0x2:
         printf("OS/ABI: NetBSD");
         break;
      case 0x3:
         printf("OS/ABI: Linux");
         break;
      case 0x4:
         printf("OS/ABI: Linux");
         break;
      case 0x5:
         printf("OS/ABI: IRIX");
         break;
      case 0x6:
         printf("OS/ABI: FreeBSD");
         break;
      case 0x7:
         printf("OS/ABI: UNIX TRU64");
         break;
      case 0x8:
         printf("OS/ABI: de l'architecture ARM");
         break;
      case 0x9:
         printf("OS/ABI: autonome (embarqué)");
         break;
      default:
         break;
   }
   printf("\n");
}
void get_ABI()
{
   printf("Version ABI: %x\n", header.e_ident[8]);
}
void type_fichier()
{
   printf("Type: ");
   switch (header.e_type)
   {
      case 0x0:
         printf("Type inconnu");
         break;
      case 0x1:
         printf("REL");
         break;
      case 0x2:
         printf("EXEC");
         break;
      case 0x3:
         printf("DYN");
         break;
      case 0x4:
         printf("CORE");
         break;
   }
   printf("\n");
}

void get_machine()
{
   printf("Machine : ");
   switch (header.e_machine)
   {
      case 0:
         printf("No machine");
         break;
      case 1:
         printf("AT&T WE 32100");
         break;
      case 2:
         printf("SUN SPARC");
         break;
      case 3:
         printf("Intel 80386");
         break;
      case 4:
         printf("Motorola m68k family ");
         break;
      case 5:
         printf("Motorola m88k family");
         break;
      case 6:
         printf("Intel MCU");
         break;
      case 7:
         printf("Intel 80860");
         break;
      case 8:
         printf("MIPS R3000 big-endian");
         break;
      case 9:
         printf("IBM System/370");
         break;
      case 10:
         printf("MIPS R3000 little-endian");
         break;
      case 15:
         printf("HPPA");
         break;
      case 17:
         printf("Fujitsu VPP500");
         break;
      case 18:
         printf("Sun's v8plus");
         break;
      case 19:
         printf("Intel 80960");
         break;
      case 20:
         printf("PowerPC");
         break;
      case 21:
         printf("PowerPC 64-bit");
         break;
      case 22:
         printf("IBM S390");
         break;
      case 23:
         printf("IBM SPU/SPC");
         break;
      case 36:
         printf("NEC V800 series");
         break;
      case 37:
         printf("Fujitsu FR20");
         break;
      case 38:
         printf("TRW RH-32");
         break;
      case 39:
         printf("Motorola RCE");
         break;
      case 40:
         printf("ARM");
         break;
      case 42:
         printf("Hitachi SH");
         break;
      case 41:
         printf("Digital Alpha");
         break;
      case 43:
         printf("SPARC v9 64-bit");
         break;
      case 44:
         printf("Siemens Tricore");
         break;
      case 45:
         printf("Argonaut RISC Core");
         break;
      case 46:
         printf("Hitachi H8/300");
         break;
      case 47:
         printf("Hitachi H8/300H");
         break;
      case 48:
         printf("Hitachi H8S");
         break;
      case 49:
         printf("Hitachi H8/500");
         break;
      case 50:
         printf("Intel Merced");
         break;
      case 51:
         printf("Stanford MIPS-X");
         break;
      case 52:
         printf("Motorola Coldfire");
         break;
      case 53:
         printf("Motorola M68HC12");
         break;
      case 54:
         printf("Fujitsu MMA Multimedia Accelerator");
         break;
      case 55:
         printf("Siemens PCP");
         break;
      case 56:
         printf("Sony nCPU embeeded RISC");
         break;
      case 57:
         printf("Denso NDR1 microprocessor");
         break;
      case 58:
         printf("Motorola Start*Core processor");
         break;
      case 59:
         printf("Toyota ME16 processor");
         break;
      case 60:
         printf("STMicroelectronic ST100 processor");
         break;
      case 61:
         printf("Advanced Logic Corp. Tinyj emb.fam");
         break;
      case 62:
         printf("AMD x86-64 architecture");
         break;
      case 63:
         printf("Sony DSP Processor");
         break;
      case 64:
         printf("Digital PDP-10");
         break;
      case 65:
         printf("Digital PDP-11");
         break;
      case 66:
         printf("Siemens FX66 microcontroller");
         break;
      case 67:
         printf("STMicroelectronics ST9+ 8/16 mc");
         break;
      case 68:
         printf("STmicroelectronics ST7 8 bit mc");
         break;
      case 69:
         printf("Motorola MC68HC16 microcontroller");
         break;
      case 70:
         printf("Motorola MC68HC11 microcontroller");
         break;
      case 71:
         printf("Motorola MC68HC08 microcontroller");
         break;
      case 72:
         printf("Motorola MC68HC05 microcontroller");
         break;
      case 73:
         printf("Silicon Graphics SVx");
         break;
      case 74:
         printf("STMicroelectronics ST19 8 bit mc");
         break;
      case 75:
         printf("Digital VAX");
         break;
      case 76:
         printf("Axis Communications 32-bit emb.proc");
         break;
      case 77:
         printf("Infineon Technologies 32-bit emb.proc");
         break;
      case 78:
         printf("Element 14 64-bit DSP Processor");
         break;
      case 79:
         printf("LSI Logic 16-bit DSP Processor");
         break;
      case 80:
         printf("Donald Knuth's educational 64-bit proc");
         break;
      case 81:
         printf("Harvard University machine-independent object files");
         break;
      case 82:
         printf("SiTera Prism");
         break;
      case 83:
         printf("Atmel AVR 8-bit microcontroller");
         break;
      case 84:
         printf("Fujitsu FR30");
         break;
      case 85:
         printf("Mitsubishi D10V");
         break;
      case 86:
         printf("Mitsubishi D30V");
         break;
      case 87:
         printf("NEC v850");
         break;
      case 88:
         printf("Mitsubishi M32R");
         break;
      case 89:
         printf("Matsushita MN10300");
         break;
      case 90:
         printf("Matsushita MN10200");
         break;
      case 91:
         printf("picoJava");
         break;
      case 92:
         printf("OpenRISC 32-bit embedded processor");
         break;
      case 93:
         printf("ARC International ARCompact");
         break;
      case 94:
         printf("Tensilica Xtensa Architecture");
         break;
      case 95:
         printf("Alphamosaic VideoCore");
         break;
      case 96:
         printf("Thompson Multimedia General Purpose Proc");
         break;
      case 97:
         printf("National Semi. 32000");
         break;
      case 98:
         printf("Tenor Network TPC");
         break;
      case 99:
         printf("Trebia SNP 1000");
         break;
      case 100:
         printf("STMicroelectronics ST200");
         break;
      case 101:
         printf("Ubicom IP2xxx");
         break;
      case 102:
         printf("MAX processor");
         break;
      case 103:
         printf("National Semi. CompactRISC");
         break;
      case 104:
         printf("Fujitsu F2MC16");
         break;
      case 105:
         printf("Texas Instruments msp430");
         break;
      case 106:
         printf("Analog Devices Blackfin DSP");
         break;
      case 107:
         printf("Seiko Epson S1C33 family");
         break;
      case 108:
         printf("Sharp embedded microprocessor");
         break;
      case 109:
         printf("Arca RISC");
         break;
      case 110:
         printf("PKU-Unity &MPRC Peking Uni. mc series");
         break;
      case 111:
         printf("eXcess configurable cpu");
         break;
      case 112:
         printf("Icera Semi. Deep Execution Processor");
         break;
      case 113:
         printf("Altera Nios II");
         break;
      case 114:
         printf("National Semi. CompactRISC CRX");
         break;
      case 115:
         printf("Motorola XGATE");
         break;
      case 116:
         printf("Infineon C16x/XC16x");
         break;
      case 117:
         printf("Renesas M16C");
         break;
      case 118:
         printf("Microchip Technology dsPIC30F");
         break;
      case 119:
         printf("Freescale Communication Engine RISC");
         break;
      case 120:
         printf("Renesas M32C");
         break;
      case 131:
         printf("Altium TSK3000");
         break;
      case 132:
         printf("Freescale RS08");
         break;
      case 133:
         printf("Analog Devices SHARC family");
         break;
      case 134:
         printf("Cyan Technology eCOG2");
         break;
      case 135:
         printf("Sunplus S+core7 RISC");
         break;
      case 136:
         printf("New Japan Radio (NJR) 24-bit DSP");
         break;
      case 137:
         printf("Broadcom VideoCore III");
         break;
      case 138:
         printf("RISC for Lattice FPGA");
         break;
      case 139:
         printf("Seiko Epson C17");
         break;
      case 140:
         printf("Texas Instruments TMS320C6000 DSP");
         break;
      case 141:
         printf("Texas Instruments TMS320C2000 DSP");
         break;
      case 142:
         printf("Texas Instruments TMS320C55x DSP");
         break;
      case 143:
         printf("Texas Instruments App. Specific RISC");
         break;
      case 144:
         printf("Texas Instruments Prog. Realtime Unit");
         break;
      case 160:
         printf("STMicroelectronics 64bit VLIW DSP");
         break;
      case 161:
         printf("Cypress M8C");
         break;
      case 162:
         printf("Renesas R32C");
         break;
      case 163:
         printf("NXP Semi. TriMedia");
         break;
      case 164:
         printf("QUALCOMM DSP6");
         break;
      case 166:
         printf("STMicroelectronics STxP7x");
         break;
      case 165:
         printf("Intel 8051 and variants");
         break;
      case 167:
         printf("Andes Tech. compact code emb. RISC");
         break;
      case 168:
         printf("Cyan Technology eCOG1X");
         break;
      case 169:
         printf("Dallas Semi. MAXQ30 mc");
         break;
      case 170:
         printf("New Japan Radio (NJR) 16-bit DSP");
         break;
      case 171:
         printf("M2000 Reconfigurable RISC");
         break;
      case 172:
         printf("Cray NV2 vector architecture");
         break;
      case 173:
         printf("Renesas RX");
         break;
      case 174:
         printf("Imagination Tech. META");
         break;
      case 175:
         printf("MCST Elbrus");
         break;
      case 176:
         printf("Cyan Technology eCOG16");
         break;
      case 177:
         printf("National Semi. CompactRISC CR16");
         break;
      case 178:
         printf("Freescale Extended Time Processing Unit");
         break;
      case 179:
         printf("Infineon Tech. SLE9X");
         break;
      case 180:
         printf("Intel L10M");
         break;
      case 181:
         printf("Intel K10M");
         break;
      case 183:
         printf("ARM AARCH64");
         break;
      case 185:
         printf("Amtel 32-bit microprocessor");
         break;
      case 186:
         printf("STMicroelectronics STM8");
         break;
      case 187:
         printf("Tileta TILE64");
         break;
      case 188:
         printf("Tilera TILEPro");
         break;
      case 189:
         printf("Xilinx MicroBlaze");
         break;
      case 190:
         printf("NVIDIA CUDA");
         break;
      case 191:
         printf("Tilera TILE-Gx");
         break;
      case 192:
         printf("CloudShield");
         break;
      case 193:
         printf("KIPO-KAIST Core-A 1st gen.");
         break;
      case 194:
         printf("KIPO-KAIST Core-A 2nd gen.");
         break;
      case 195:
         printf("Synopsys ARCompact V2");
         break;
      case 196:
         printf("Open8 RISC");
         break;
      case 197:
         printf("Renesas RL78");
         break;
      case 198:
         printf("Broadcom VideoCore V");
         break;
      case 199:
         printf("Renesas 78KOR");
         break;
      case 200:
         printf("Freescale 56800EX DSC");
         break;
      case 201:
         printf("Beyond BA1");
         break;
      case 202:
         printf("Beyond BA2");
         break;
      case 203:
         printf("XMOS xCORE");
         break;
      case 204:
         printf("Microchip 8-bit PIC(r)");
         break;
      case 210:
         printf("KM211 KM32");
         break;
      case 211:
         printf("KM211 KMX32");
         break;
      case 212:
         printf("KM211 KMX16");
         break;
      case 213:
         printf("KM211 KMX8");
         break;
      case 214:
         printf("KM211 KVARC");
         break;
      case 215:
         printf("Paneve CDP");
         break;
      case 216:
         printf("Cognitive Smart Memory Processor");
         break;
      case 217:
         printf("Bluechip CoolEngine");
         break;
      case 218:
         printf("Nanoradio Optimized RISC");
         break;
      case 219:
         printf("CSR Kalimba");
         break;
      case 220:
         printf("Zilog Z80");
         break;
      case 221:
         printf("Controls and Data Services VISIUMcore");
         break;
      case 222:
         printf("FTDI Chip FT32");
         break;
      case 223:
         printf("Moxie processor");
         break;
      case 224:
         printf("AMD GPU");
         break;
      case 243:
         printf("RISC-V");
         break;
      case 247:
         printf("Linux BPF -- in-kernel virtual machine");
         break;
   }
}

void get_e_version2()
{
   printf("\nVersion: 0x%x \n", header.e_version);
}

void get_e_entry()
{
   printf("Adresse du point d'entrée: 0x%lx \n", header.e_entry);
}

void get_e_phoff()
{
   printf("Début des en-têtes de programme : %lx (octets dans le fichier)\n", header.e_phoff);
}

void get_e_shoff()
{
   printf("Début des en-têtes de section : %li (octets dans le fichier)\n", header.e_shoff);
}

void get_e_flags()
{
   printf("Fanions : 0x%x\n", header.e_flags);
}

void get_e_ehsize()
{
   printf("Taille de cet en-tête: %i (octets)\n", header.e_ehsize);
}

void get_e_phentsize()
{
   printf("Taille de l'en-tête du programme: %d (octets)\n", header.e_phentsize);
}

void get_e_phnum()
{
   printf("Nombre d'en-tête du programme: %d  \n", header.e_phnum);
}

void get_e_shentsize()
{
   printf("Taille des en-têtes de section:  %d (octets) \n", header.e_shentsize);
}

void get_e_shnum()
{
   printf("Nombre d'en-têtes de section:   %d \n", header.e_shnum);
}

void get_e_shstrndx()
{
   printf("Table d'indexes des chaînes d'en-tête de section: %d \n", header.e_shstrndx);
}

void get_sh_name()
{
   printf("Nom de la section: '%c' \n", section.sh_name);
}

void get_sh_type()
{
   printf("Type de la section: ");
   switch (section.sh_type)
   {
      case 0:
         printf("Section header table entry unused");
         break;
      case 1:
         printf("Program data");
         break;
      case 2:
         printf("Symbol table");
         break;
      case 3:
         printf("String table");
         break;
      case 4:
         printf("Relocation entries with addends");
         break;
      case 5:
         printf("Symbol hash table");
         break;
      case 6:
         printf("Dynamic linking information");
         break;
      case 7:
         printf("Notes");
         break;
      case 8:
         printf("Program space with no data (bss)");
         break;
      case 9:
         printf("Relocation entries, no addends");
         break;
      case 10:
         printf("Reserved");
         break;
      case 11:
         printf("Dynamic linker symbol table");
         break;
      case 14:
         printf("Array of constructors");
         break;
      case 15:
         printf("Array of destructors");
         break;
      case 16:
         printf("Array of pre-constructors");
         break;
      case 17:
         printf("Section group ");
         break;
      case 18:
         printf("Extended section indeces");
         break;
      case 19:
         printf("Number of defined types.");
         break;
      case 0x60000000:
         printf("Start OS-specific.");
         break;
      case 0x6ffffff5:
         printf("Object attributes.");
         break;
      case 0x6ffffff6:
         printf("GNU-style hash table.");
         break;
      case 0x6ffffff7:
         printf("Prelink library list");
         break;
      case 0x6ffffff8:
         printf("Checksum for DSO content.");
         break;
      case 0x6ffffffa:
         printf("Sun-specific low bound.");
         break;
      case 0x6ffffffd:
         printf("Version definition section.");
         break;
      case 0x6ffffffe:
         printf("Version needs section.");
         break;
      case 0x6fffffff:
         printf("Version symbol table.");
         break;
        	//case 0x6fffffff	: printf("Sun-specific high bound."); break;
        	//case 0x6fffffff	: printf("End OS-specific type"); break;
      case 0x70000000:
         printf("Start of processor-specific");
         break;
      case 0x7fffffff:
         printf("End of processor-specific");
         break;
      case 0x80000000:
         printf("Start of application-specific");
         break;
      case 0x8fffffff:
         printf("End of application-specific");
         break;
      default:
         printf("ouin ouin");
         break;
   }
}
void surf_sections()
{
   printf("\n");
   for (int i = 0; i < 4; i++)
   {
      printf("%c", (section.sh_name + i));
   }
}
void get_section_names(FILE *file)
{
  	//Lire les noms of sections
   sec_names = malloc(section.sh_size);
   fseek(file, section.sh_offset, SEEK_SET);
   fread(sec_names, 1, section.sh_size, file);
}
void etape1(FILE *f)
{
   load_data(f);
   get_magic();
   get_class();
   get_donnees();
   get_e_version();
   get_os();
   get_ABI();
   type_fichier();
   get_machine();
   get_e_version2();
   get_e_entry();
   get_e_phoff();
   get_e_shoff();
   get_e_flags();
   get_e_ehsize();
   get_e_phentsize();
   get_e_phnum();
   get_e_shentsize();
   get_e_shnum();
   get_e_shstrndx();
}
//TODO Please change the function name later on.
void dump_sections(FILE *file, int i)
{
   if (i == header.e_shnum)
      return;
   fseek(file, header.e_shoff + i* sizeof(section), SEEK_SET);
   fread(&section, 1, sizeof(section), file);
   printf("\n--- NEW SECTION ---\n");
   printf("Numéro:\t%2u \nNom:\t%s \nTaille:\t%lx\n", i, sec_names + section.sh_name, section.sh_size);
   printf("Type:\t");
   switch (section.sh_type)
   {
      case 0x0:
         printf("NULL");
         break;
      case 0x1:
         printf("PROGBITS");
         break;
      case 0x2:
         printf("SYMTAB");
         indice_tab_symb = i;
         break;
      case 0x3:
         printf("STRTAB");
         break;
      case 0x4:
         printf("RELA");
         table_rela[indice_tab_rela] = i;
         indice_tab_rela = indice_tab_rela + 1;
         break;
      case 0x5:
         printf("HASH");
         break;
      case 0x6:
         printf("DYNAMIC");
         break;
      case 0x7:
         printf("NOTE");
         break;
      case 0x8:
         printf("NOBITS");
         break;
      case 0x9:
         printf("REL");
         break;
      case 0x0A:
         printf("SHLIB");
         break;

      default:
         printf("Type inconnu");
         break;
   }
   printf("\n");
   printf("Properties (flag %lx) : ", section.sh_flags);
  	//Voir https://en.wikipedia.org/wiki/Executable_and_Linkable_Format#Section_header
   if (section.sh_flags &SHF_WRITE)
      printf("W");
   if (section.sh_flags &SHF_ALLOC)
      printf("A");
   if (section.sh_flags &SHF_EXECINSTR)
      printf("X");
   if (section.sh_flags &SHF_MERGE)
      printf("M");
   if (section.sh_flags &SHF_STRINGS)
      printf("S");
   if (section.sh_flags &SHF_INFO_LINK)
      printf("I");
   if (section.sh_flags &SHF_LINK_ORDER)
      printf("L");
   if (section.sh_flags &SHF_OS_NONCONFORMING)
      printf("O");
   if (section.sh_flags &SHF_GROUP)
      printf("G");
   if (section.sh_flags &SHF_TLS)
      printf("T");
   if (section.sh_flags &SHF_EXCLUDE)
      printf("E");
   printf("\n");
   printf("Position of section (en octets) :\t%lo\n", section.sh_offset);
   dump_sections(file, i + 1);
}
void etape2(FILE *f)
{
   get_section_names(f);
   dump_sections(f, 0);
}
void etape3(FILE *f, int x)
{

   fseek(f, header.e_shoff + x* sizeof(section), SEEK_SET);
   fread(&section, 1, sizeof(section), f);
   unsigned char *buff = malloc(sizeof(unsigned char));
   unsigned char ASCII_DUMP[HEXA];
  	//TODO: Show section name:
   printf("\nVidange hexadécimale de la section « %s » :\n", sec_names + section.sh_name);
   int j, k;
   int printed_smth = 0;
   fseek(f, section.sh_offset, SEEK_SET);
  	// Process every byte in the data of the section.
   int i = 0;
   int counter = i;
   while (i < section.sh_size)
   {
      printf("0x%08x ", i / 16);
      counter = 0;
      for (j = 0; j < section.sh_size; j++)
      {
         for (k = 0; k < DWORD; k++)
         {

            if (i / 16 < section.sh_size)
            {

               fread(buff, sizeof(*buff), 1, f);
               printf("%02x", *buff);
               ASCII_DUMP[i / 16 % HEXA] = *buff;
               i += 16;
               counter++;
            }
            else
            {
              	//printf(" ");
            }
         }
         printf(" ");
         if ((counter) > 12)
         {

            counter = 0;
            for (int n = 0; n < HEXA; n++)
            {
               if (ASCII_DUMP[n] >= ' ' && ASCII_DUMP[n] <= '~')
                  printf("%c", ASCII_DUMP[n]);
               else
                  printf(".");
            }
            if (i / 16 < section.sh_size)
            {

               printf("\n");
               printf("0x%08x ", i / 16);
            }
            else {}
         }
      }
      if (counter > 0)
         for (int n = 0; n < strlen(ASCII_DUMP) + 1; n++)
         {
            if (ASCII_DUMP[n] >= ' ' && ASCII_DUMP[n] <= '~')
               printf("%c", ASCII_DUMP[n]);
            else
               printf(".");
         }
   }

   printf("\n");
}

void etape4(FILE *file)
{
   fseek(file, header.e_shoff + indice_tab_symb* sizeof(section), SEEK_SET);	//on va a la section de la table des symboles
   fread(&section, 1, sizeof(section), file);
   printf("Name : %s\n", sec_names + section.sh_name);
   int count = section.sh_size / section.sh_entsize;	//nombre d'entree dans la table des symboles
   fseek(file, section.sh_offset, SEEK_SET);
   for (int i = 0; i < count; i++)
   {
      fread(&symb, 1, sizeof(symb), file);
      printf("Num : %d  \t", i);
      printf("Valeur : %x  \t", symb.st_value);
      printf("Taille : %d  \t", symb.st_size);
      //printf("Type : %x  ", symb.st_info);
      switch (symb.st_info){
            case STT_NOTYPE:  printf("Type : NOTYPE\t");break;
            case STT_OBJECT:	printf("Type : OBJECT\t");break;		/* Symbol is a data object */
            case STT_FUNC:	printf("Type : FUNC\t");break;	/* Symbol is a code object */
            case STT_SECTION:	printf("Type : SECTION\t");break;	/* Symbol associated with a section */
            case STT_FILE:	printf("Type : FILE\t");break;		/* Symbol's name is file name */
            case STT_COMMON:	printf("Type : COMMON\t");break;	/* Symbol is a common data object */
            case STT_TLS:	printf("Type : TLS\t");break;		/* Symbol is thread-local data object*/
            case STT_NUM:	printf("Type : NUM\t");break;		/* Number of defined types.  */
            case STT_LOOS:	printf("Type : LOOS\t");break;	/* Start of OS-specific */
            case STT_HIOS:	printf("Type : HIOS\t");break;	/* End of OS-specific */
            case STT_LOPROC:	printf("Type : LOPROC\t");break;		/* Start of processor-specific */
            case STT_HIPROC:	printf("Type : HIPROC\t");break;		/* End of processor-specific */
            default :  printf("Type : NOTYPE\t");break;  ///problème ici!!!!
            
      }


      //printf("symb.st_info = %i", symb.st_info);
      //printf("Vis : %x  ", symb.st_other);
      
      switch(symb.st_other){
            case STV_DEFAULT :	printf("Vis : DEFAULT\t");break;		/* Default symbol visibility rules */
            case STV_INTERNAL	:     printf("Vis : INTERNAL\t");break;		/* Processor specific hidden class */
            case STV_HIDDEN	: 	printf("Vis : HIDDEN\t");break;	      /* Sym unavailable in other modules */
            case STV_PROTECTED : 	printf("Vis : PROTECTED\t");break;	      /* Not preemptible, not exported */
      }
      //printf("Index : %x  ", symb.st_shndx);
      switch(symb.st_shndx){
            case SHN_UNDEF:		printf("Ndx : UND\t");break;	      /* Undefined section */
            //case SHN_LORESERVE:	printf("Ndx : LORESERVE\t");break;	/* Start of reserved indices */
            //case SHN_LOPROC:      printf("Ndx : LOPROC\t");break;	/* Start of processor-specific */
            case SHN_BEFORE:		printf("Ndx : BEFORE\t");break;	/* Order section before all others (Solaris).  */
            case SHN_AFTER:		printf("Ndx : AFTER\t");break;	/* Order section after all others (Solaris).  */
            case SHN_HIPROC:		printf("Ndx : HIPROC\t");break;	/* End of processor-specific */
            case SHN_LOOS:		printf("Ndx : LOOS\t");break;	      /* Start of OS-specific */
            case SHN_HIOS:    	printf("Ndx : HIOS\t");break;	      /* End of OS-specific */
            case SHN_ABS:		printf("Ndx : ABS\t");break;		/* Associated symbol is absolute */
            case SHN_COMMON:		printf("Ndx : COMMON\t");break;	/* Associated symbol is common */
            //case SHN_XINDEX:	printf("Ndx : XINDEX\t");break;	/* Index is in extra table.  */
            case SHN_HIRESERVE:	printf("Ndx : HIRESERVE\t");break;	/* End of reserved indices */
            default : 
                  if(symb.st_shndx<10){
                        printf("Ndx : %d\t\t", symb.st_shndx); break;
                  } else {
                        printf("Ndx : %d\t", symb.st_shndx); break;}
      }
     // printf("Nom : %x\n", symb.st_name);
      switch (symb.st_name){
            case SHT_NULL:          printf("Nom : \n", symb.st_name);break;   /* Section header table entry unused */
            case SHT_PROGBITS: 	printf("Nom : %x\n", symb.st_name);break;	/* Program data */
            case SHT_SYMTAB: 		printf("Nom : %x\n", symb.st_name);break; /* Symbol table */
            case SHT_STRTAB:	  	printf("Nom : %x\n", symb.st_name);break;	/* String table */
            case SHT_RELA:	  	printf("Nom : %x\n", symb.st_name);break;	/* Relocation entries with addends */
            case SHT_HASH:	  	printf("Nom : %x\n", symb.st_name);break;	/* Symbol hash table */
            case SHT_DYNAMIC:	  	printf("Nom : %x\n", symb.st_name);break; /* Dynamic linking information */
            case SHT_NOTE:	  	printf("Nom : %x\n", symb.st_name);break;	/* Notes */
            case SHT_NOBITS:	  	printf("Nom : %x\n", symb.st_name);break;	/* Program space with no data (bss) */
            case SHT_REL:		printf("Nom : %x\n", symb.st_name);break;	/* Relocation entries, no addends */
            case SHT_SHLIB:	  	printf("Nom : %x\n", symb.st_name);break;	/* Reserved */
            case SHT_DYNSYM:	  	printf("Nom : %x\n", symb.st_name);break;	/* Dynamic linker symbol table */
            case SHT_INIT_ARRAY:	printf("Nom : %x\n", symb.st_name);break; /* Array of constructors */
            case SHT_FINI_ARRAY:	printf("Nom : %x\n", symb.st_name);break;	/* Array of destructors */
            case SHT_PREINIT_ARRAY: printf("Nom : %x\n", symb.st_name);break;	/* Array of pre-constructors */
            case SHT_GROUP:	      printf("Nom : %x\n", symb.st_name);break; /* Section group */
            case SHT_SYMTAB_SHNDX: 	printf("Nom : %x\n", symb.st_name);break;	/* Extended section indeces */
            case SHT_NUM:		printf("Nom : %x\n", symb.st_name);break; /* Number of defined types.  */
            case SHT_LOOS:	  	printf("Nom : %x\n", symb.st_name);break; /* Start OS-specific.  */
            case SHT_GNU_ATTRIBUTES:printf("Nom : %x\n", symb.st_name);break; /* Object attributes.  */
            case SHT_GNU_HASH:	printf("Nom : %x\n", symb.st_name);break; /* GNU-style hash table.  */
            case SHT_GNU_LIBLIST:	printf("Nom : %x\n", symb.st_name);break;	/* Prelink library list */
            case SHT_CHECKSUM:	printf("Nom : %x\n", symb.st_name);break;	/* Checksum for DSO content.  */
            case SHT_LOSUNW:	  	printf("Nom : %x\n", symb.st_name);break; /* Sun-specific low bound.  */
            //case SHT_SUNW_move:	printf("Nom : %x\n", symb.st_name);break;
            case SHT_SUNW_COMDAT:   printf("Nom : %x\n", symb.st_name);break;
            case SHT_SUNW_syminfo:  printf("Nom : %x\n", symb.st_name);break;
            case SHT_GNU_verdef:	printf("Nom : %x\n", symb.st_name);break;	/* Version definition section.  */
            case SHT_GNU_verneed:	printf("Nom : %x\n", symb.st_name);break;	/* Version needs section.  */
            case SHT_GNU_versym:	printf("Nom : %x\n", symb.st_name);break;	/* Version symbol table.  */
            //case SHT_HISUNW:	0x6fffffff	/* Sun-specific high bound.  */
            //case SHT_HIOS:	  	printf("Nom : %x\n", symb.st_name);break; /* End OS-specific type */
            case SHT_LOPROC:	  	printf("Nom : %x\n", symb.st_name);break; /* Start of processor-specific */
            case SHT_HIPROC:	  	printf("Nom : %x\n", symb.st_name);break; /* End of processor-specific */
            case SHT_LOUSER:	  	printf("Nom : %x\n", symb.st_name);break; /* Start of application-specific */
            case SHT_HIUSER:	      printf("Nom : %x\n", symb.st_name);break; /* End of application-specific */

            default : printf("Nom : %x\n", symb.st_name);break;
      }
   }
}

void etape5(FILE *file)
{
   int count = 0;
   for (int i = 0; i < indice_tab_rela; i++)
   {
      fseek(file, header.e_shoff + table_rela[i] *sizeof(section), SEEK_SET);
      fread(&section, 1, sizeof(section), file);
      count = section.sh_size / section.sh_entsize;
      printf("Section de redressage numéro : %d\n", table_rela[i]);
      fseek(file, section.sh_offset, SEEK_SET);
      for (int j = 0; j < count; j++)
      {
         fread(&rela, 1, sizeof(rela), file);
         printf("Decalage : %llx  ", rela.r_offset);
         printf("Type : %x  ", ELF64_R_TYPE(rela.r_info));
         printf("Index : %x\n", ELF64_R_SYM(rela.r_info));
      }
   }
}

int main(int argc, char *argv[])
{
   FILE *file = fopen(argv[1], "rb");
   etape1(file);
   etape2(file);
   //int x = 0;
   //scanf("%d", &x);
   //etape3(file, x);	//TODO afficher "pas de données a dump/vidanger si le ontenu de la section est vide"
   etape4(file);
   //etape5(file); 
   fclose(file);
   return 0;
}
