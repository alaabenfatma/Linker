#include "brain.h"

//local header
Elf32_Ehdr header;
Elf32_Shdr section;
Elf32_Sym symb;
Elf32_Rela rela;
Elf32_Rel rel;
char *sec_names = NULL;
char *symb_names = NULL;
int ENDIAN = 0; //var globale => 0 = little | 1 = bug
int isLoaded = 0;
int r;

void header_to_little_endian()
{
   if (ENDIAN == 1)
   {
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
      header.e_entry = reverse_4(header.e_entry);
   }
}
void section_to_little_endian()
{
   if (ENDIAN == 1)
   {
      section.sh_name = reverse_4(section.sh_name);
      section.sh_type = reverse_4(section.sh_type);
      section.sh_flags = reverse_4(section.sh_flags);
      section.sh_addr = reverse_4(section.sh_addr);
      section.sh_offset = reverse_4(section.sh_offset);
      section.sh_size = reverse_4(section.sh_size);
      section.sh_link = reverse_4(section.sh_link);
      section.sh_info = reverse_4(section.sh_info);
      section.sh_addralign = reverse_4(section.sh_addralign);
      section.sh_entsize = reverse_4(section.sh_entsize);
   }
}

void section_to_little_endian2(Elf32_Shdr* sec)
{
   if (ENDIAN == 1)
   {
      sec->sh_name = reverse_4(sec->sh_name);
      sec->sh_type = reverse_4(sec->sh_type);
      sec->sh_flags = reverse_4(sec->sh_flags);
      sec->sh_addr = reverse_4(sec->sh_addr);
      sec->sh_offset = reverse_4(sec->sh_offset);
      sec->sh_size = reverse_4(sec->sh_size);
      sec->sh_link = reverse_4(sec->sh_link);
      sec->sh_info = reverse_4(sec->sh_info);
      sec->sh_addralign = reverse_4(sec->sh_addralign);
      sec->sh_entsize = reverse_4(sec->sh_entsize);
   }
}



void symbol_to_little_endian()
{
   if (ENDIAN == 1)
   {
      symb.st_name = reverse_4(symb.st_name);
      symb.st_value = reverse_4(symb.st_value);
      symb.st_size = reverse_4(symb.st_size);
      /*
   These are already in unsigned char, can't be swapped.
   symb.st_info = reverse_4(symb.st_info);
   symb.st_other = reverse_4(symb.st_other);*/
      symb.st_shndx = reverse_2(symb.st_shndx);
   }
}



void rela_to_little_endian()
{
   if (ENDIAN == 1)
   {
      rela.r_offset = reverse_4(rela.r_offset);
      rela.r_info = reverse_4(rela.r_info);
      rela.r_addend = reverse_4(rela.r_addend);
   }
}

void rel_to_little_endian()
{
   if (ENDIAN == 1)
   {
      rel.r_offset = reverse_4(rel.r_offset);
      rel.r_info = reverse_4(rel.r_info);
   }
}

void load_data(FILE *file)
{
   isLoaded = 1;
   if (file)
   {
      r = fread(&header, 1, sizeof(header), file);
      if(r != sizeof(header)){
            //ignore
      }
      //ENDIANESS!!
      if (header.e_ident[5] == 0x2)
         ENDIAN = 1;
      header_to_little_endian();

      fseek(file, (header.e_shoff) + (header.e_shstrndx) * sizeof(section), SEEK_SET);
      r = fread(&section, 1, sizeof(section), file);
      if(r != sizeof(section)){
            //ignore
      }
      section_to_little_endian();
      get_section_names(file);
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

//almost the same as load_data
void load_data2(FILE *file, Elf32_Ehdr *h, Elf32_Shdr *s)
{
   //isLoaded = 1;
   if (file)
   {
      r = fread(h, 1, sizeof(Elf32_Ehdr), file);
      if(r != sizeof(Elf32_Ehdr)){
            //ignore
      }
      //ENDIANESS!!
      if (h->e_ident[5] == 0x2)
         ENDIAN = 1;
      header_to_little_endian();

      fseek(file, (h->e_shoff) + (h->e_shstrndx) * sizeof(s), SEEK_SET);
      r = fread(s, 1, sizeof(Elf32_Shdr), file);
      if(r != sizeof(Elf32_Shdr)){
            //ignore
      }
      section_to_little_endian();
      get_section_names(file);
      if (h->e_ident[0] == 0x7f && h->e_ident[1] == 'E' && h->e_ident[2] == 'L' && h->e_ident[3] == 'F')
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
   printf("Adresse du point d'entrée: 0x%xx \n", header.e_entry);
}

void get_e_phoff()
{
   printf("Début des en-têtes de programme : %x (octets dans le fichier)\n", header.e_phoff);
}

void get_e_shoff()
{
   printf("Début des en-têtes de section : %i (octets dans le fichier)\n", header.e_shoff);
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
   r = fread(sec_names, 1, section.sh_size, file);
   if(r != section.sh_size){
            //ignore
   }
}
void etape1(FILE *f)
{
   if (isLoaded == 0)
   {
      load_data(f);
   }
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
   fseek(file, header.e_shoff + i * sizeof(section), SEEK_SET);
   r = fread(&section, 1, sizeof(section), file);
   if(r != sizeof(section)){
            //ignore
   }
   section_to_little_endian();
   printf("\n--- NEW SECTION ---\n");
   printf("Numéro:\t%2u \nNom:\t%s \nTaille:\t%x\n", i, sec_names + section.sh_name, section.sh_size);
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
      break;
   case 0x3:
      printf("STRTAB");
      break;
   case 0x4:
      printf("RELA");
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
   printf("Properties (flag %x) : ", section.sh_flags);
   //Voir https://en.wikipedia.org/wiki/Executable_and_Linkable_Format#Section_header
   if (section.sh_flags & SHF_WRITE)
      printf("W");
   if (section.sh_flags & SHF_ALLOC)
      printf("A");
   if (section.sh_flags & SHF_EXECINSTR)
      printf("X");
   if (section.sh_flags & SHF_MERGE)
      printf("M");
   if (section.sh_flags & SHF_STRINGS)
      printf("S");
   if (section.sh_flags & SHF_INFO_LINK)
      printf("I");
   if (section.sh_flags & SHF_LINK_ORDER)
      printf("L");
   if (section.sh_flags & SHF_OS_NONCONFORMING)
      printf("O");
   if (section.sh_flags & SHF_GROUP)
      printf("G");
   if (section.sh_flags & SHF_TLS)
      printf("T");
   if (section.sh_flags & SHF_EXCLUDE)
      printf("E");
   printf("\n");
   printf("Position of section (en octets) :\t%o\n", section.sh_offset);
   dump_sections(file, i + 1);
}
void etape2(FILE *f)
{
   if (isLoaded == 0)
   {
      load_data(f);
   }
   get_section_names(f);
   dump_sections(f, 0);
}
void etape3(FILE *f, int x)
{

   if (isLoaded == 0)
   {
      load_data(f);
   }
   fseek(f, header.e_shoff + x * sizeof(section), SEEK_SET);
   r = fread(&section, 1, sizeof(section), f);
   if(r != sizeof(section)){
            //ignore
   }
   section_to_little_endian();
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

               r = fread(buff, sizeof(*buff), 1, f);
               if(r != 1){
                  //ignore
               }
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
            else
            {
            }
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

int get_tab_symb(FILE *file, int i)
{
   if (i == header.e_shnum)
      return 0;
   fseek(file, header.e_shoff + i * sizeof(section), SEEK_SET);
   r = fread(&section, 1, sizeof(section), file);
   if(r != sizeof(section)){
            //ignore
   }
   section_to_little_endian();
   switch (section.sh_type)
   {
   case 0x0:
      break;
   case 0x1:
      break;
   case 0x2:
      return i;
   case 0x3:
      break;
   case 0x4:
      break;
   case 0x5:
      break;
   case 0x6:
      break;
   case 0x7:
      break;
   case 0x8:
      break;
   case 0x9:
      break;
   case 0x0A:
      break;
   default:
      break;
   }
   get_tab_symb(file, i + 1);
}


char *get_func_name(FILE *f)
{
   Elf32_Shdr sec;
   int pos = ftell(f);
   char *name = malloc(10 * sizeof(char));
   fseek(f, header.e_shoff + section.sh_link * header.e_shentsize, SEEK_SET);
   r = fread(&sec, 1, sizeof(sec), f);
   if(r != sizeof(sec)){
            //ignore
   }
   section_to_little_endian2(&sec);
   fseek(f, sec.sh_offset + symb.st_name, SEEK_SET);
   char c = fgetc(f);
   int i = 0;
   while (c != '\0' && i < 256)
   {
      name[i] = c;
      i++;
      c = fgetc(f);
   }
   name[i] = '\0';
   fseek(f, pos, SEEK_SET);
   return name;
}



void etape4(FILE *file)
{
   if (isLoaded == 0)
   {
      load_data(file);
   }

   int indice_tab_symb = get_tab_symb(file, 0);
   if (indice_tab_symb == -1)
   {
      printf("Pas de table des symboles");
      return;
   }
   fseek(file, header.e_shoff + indice_tab_symb * sizeof(section), SEEK_SET); //on va a la section de la table des symboles
   r = fread(&section, 1, sizeof(section), file);
   if(r != sizeof(section)){
            //ignore
   }
   section_to_little_endian();
   int count = section.sh_size / section.sh_entsize; //nombre d'entree dans la table des symboles
   printf("\nTable des symboles (section %d) à %d entrées: \n", indice_tab_symb, count);
   fseek(file, section.sh_offset, SEEK_SET);
   for (int i = 0; i < count; i++)
   {
      r = fread(&symb, 1, sizeof(symb), file);
      if(r != sizeof(symb)){
            //ignore
      }

      symbol_to_little_endian();

      printf("Num: %d |", i);
      printf("Valeur: %08x |", symb.st_value);
      printf("Taille: %d |", symb.st_size);
      //printf("Type : %x  ", symb.st_info);
      switch (symb.st_info)
      {
      case STT_NOTYPE:
         printf("Type: NOTYPE |");
         break;
      case STT_OBJECT:
         printf("Type: OBJECT |");
         break; /* Symbol is a data object */
      case STT_FUNC:
         printf("Type: FUNC |");
         break; /* Symbol is a code object */
      case STT_SECTION:
         printf("Type: SECTION |");
         break; /* Symbol associated with a section */
      case STT_FILE:
         printf("Type: FILE |");
         break; /* Symbol's name is file name */
      case STT_COMMON:
         printf("Type: COMMON |");
         break; /* Symbol is a common data object */
      case STT_TLS:
         printf("Type: TLS |");
         break; /* Symbol is thread-local data object*/
      case STT_NUM:
         printf("Type: NUM |");
         break; /* Number of defined types.  */
      case STT_LOOS:
         printf("Type: LOOS |");
         break; /* Start of OS-specific */
      case STT_HIOS:
         printf("Type: HIOS |");
         break; /* End of OS-specific */
      case STT_LOPROC:
         printf("Type: LOPROC |");
         break; /* Start of processor-specific */
      case STT_HIPROC:
         printf("Type: HIPROC |");
         break; /* End of processor-specific */
      default:
         printf("Type: NOTYPE |");
         break; ///problème ici!!!!
      }

      // printf(" Lien: %x \n", ELF32_ST_BIND(symb.st_info));
      switch (ELF32_ST_BIND(symb.st_info))
      {
      case STB_LOCAL:
         printf("Lien: LOCAL |");
         break;
      case STB_GLOBAL:
         printf("Lien: GLOBAL |");
         break;
      }
      //printf("symb.st_info = %i", symb.st_info);
      //printf("Vis : %x  ", symb.st_other);

      switch (symb.st_other)
      {
      case STV_DEFAULT:
         printf("Vis: DEFAULT |");
         break; /* Default symbol visibility rules */
      case STV_INTERNAL:
         printf("Vis: INTERNAL |");
         break; /* Processor specific hidden class */
      case STV_HIDDEN:
         printf("Vis: HIDDEN |");
         break; /* Sym unavailable in other modules */
      case STV_PROTECTED:
         printf("Vis: PROTECTED |");
         break; /* Not preemptible, not exported */
      }
      //printf("Index: %x |", symb.st_shndx);
      switch (symb.st_shndx)
      {
      case SHN_UNDEF:
         printf("Ndx: UND |");
         break; /* Undefined section */
      //case SHN_LORESERVE:	printf("Ndx : LORESERVE |");break;	/* Start of reserved indices */
      //case SHN_LOPROC:      printf("Ndx : LOPROC |");break;	/* Start of processor-specific */
      case SHN_BEFORE:
         printf("Ndx: BEFORE |");
         break; /* Order section before all others (Solaris).  */
      case SHN_AFTER:
         printf("Ndx: AFTER |");
         break; /* Order section after all others (Solaris).  */
      case SHN_HIPROC:
         printf("Ndx: HIPROC |");
         break; /* End of processor-specific */
      case SHN_LOOS:
         printf("Ndx: LOOS |");
         break; /* Start of OS-specific */
      case SHN_HIOS:
         printf("Ndx: HIOS |");
         break; /* End of OS-specific */
      case SHN_ABS:
         printf("Ndx: ABS |");
         break; /* Associated symbol is absolute */
      case SHN_COMMON:
         printf("Ndx: COMMON |");
         break; /* Associated symbol is common */
      //case SHN_XINDEX:	printf("Ndx : XINDEX |");break;	/* Index is in extra table.  */
      case SHN_HIRESERVE:
         printf("Ndx: HIRESERVE |");
         break; /* End of reserved indices */
      default:
         printf("Ndx: %d |", symb.st_shndx);
         break;
      }
      double st = symb.st_name;
      double sh = section.sh_link;

      printf("Nom: %s \n", get_func_name(file));
   }
}

void etape5(FILE *file)
{
   if(!fread(&header, 1, sizeof(header), file))
   {
      //ignore
   }
   for (int i = 0; i < header.e_shnum; i++)
   {
      fseek(file, header.e_shoff + i * sizeof(section), SEEK_SET);
      if (!fread(&section, 1, sizeof(section), file))
      {
         //ignore
      }
      section_to_little_endian();
      if (section.sh_type == 4 || section.sh_type == 9)
      {
         int count = section.sh_size / section.sh_entsize;
         printf("\nsection de redressage numéro : %d\n", i);
         fseek(file, section.sh_offset, SEEK_SET);
         for (int j = 0; j < count; j++)
         {
            if (section.sh_type == 4)
            {
               if (!fread(&rela, 1, sizeof(rela), file))
               {
                  //ignore
               }
               rela_to_little_endian();
               printf("Decalage: %x |", rela.r_offset);
               printf("Type: %x |", ELF32_R_TYPE(rela.r_info));
               printf("Index: %x\n", ELF32_R_SYM(rela.r_info));
            }
            if (section.sh_type == 9)
            {
               if (!fread(&rel, 1, sizeof(rel), file))
               {
                  //ignore
               }
               rel_to_little_endian();
               printf("Decalage: %x |", rel.r_offset);
               printf("Type: %x\n", ELF32_R_TYPE(rel.r_info));
            }
         }
      }
   }
}

