#pragma once

typedef unsigned int Elf32_Addr;
typedef unsigned int Elf32_Off;
typedef unsigned short Elf32_Half;
typedef unsigned int Elf32_Word;
typedef int Elf32_Sword;

#define EI_NIDENT 16

#pragma pack(push, 1)

typedef struct {
	unsigned char   e_ident[EI_NIDENT];
	Elf32_Half      e_type;
	Elf32_Half      e_machine;
	Elf32_Word      e_version;
	Elf32_Addr      e_entry;
	Elf32_Off       e_phoff;
	Elf32_Off       e_shoff;
	Elf32_Word      e_flags;
	Elf32_Half      e_ehsize;
	Elf32_Half      e_phentsize;
	Elf32_Half      e_phnum;
	Elf32_Half      e_shentsize;
	Elf32_Half      e_shnum;
	Elf32_Half      e_shstrndx;
} Elf32_Ehdr;

typedef struct {
	Elf32_Word	sh_name;
	Elf32_Word	sh_type;
	Elf32_Word	sh_flags;
	Elf32_Addr	sh_addr;
	Elf32_Off	sh_offset;
	Elf32_Word	sh_size;
	Elf32_Word	sh_link;
	Elf32_Word	sh_info;
	Elf32_Word	sh_addralign;
	Elf32_Word	sh_entsize;
} Elf32_Shdr;

typedef struct {
	Elf32_Word	st_name;
	Elf32_Addr	st_value; // address of ...
	Elf32_Word	st_size;
	unsigned char	st_info;
	unsigned char	st_other;
	Elf32_Half	st_shndx; // section of ...
} Elf32_Sym;

typedef struct {
	Elf32_Addr	r_offset;
	Elf32_Word	r_info;
} Elf32_Rel;

#pragma pack(pop)

// The valid values found in Ehdr e_ident[EI_CLASS].

enum
{
	ELFCLASSNONE = 0,
	ELFCLASS32 = 1,
	ELFCLASS64 = 2
};

// The valid values found in Ehdr e_ident[EI_DATA].

enum
{
	ELFDATANONE = 0,
	ELFDATA2LSB = 1,
	ELFDATA2MSB = 2
};

// The valid values found in Ehdr e_ident[EI_VERSION] and e_version.

enum
{
	EV_NONE = 0,
	EV_CURRENT = 1
};

// The valid values found in Ehdr e_ident[EI_OSABI].

enum ELFOSABI
{
	ELFOSABI_NONE = 0,
	ELFOSABI_HPUX = 1,
	ELFOSABI_NETBSD = 2,
	// ELFOSABI_LINUX is not listed in the ELF standard.
	ELFOSABI_LINUX = 3,
	// ELFOSABI_HURD is not listed in the ELF standard.
	ELFOSABI_HURD = 4,
	ELFOSABI_SOLARIS = 6,
	ELFOSABI_AIX = 7,
	ELFOSABI_IRIX = 8,
	ELFOSABI_FREEBSD = 9,
	ELFOSABI_TRU64 = 10,
	ELFOSABI_MODESTO = 11,
	ELFOSABI_OPENBSD = 12,
	ELFOSABI_OPENVMS = 13,
	ELFOSABI_NSK = 14,
	ELFOSABI_AROS = 15,
	// A GNU extension for the ARM.
	ELFOSABI_ARM = 97,
	// A GNU extension for the MSP.
	ELFOSABI_STANDALONE = 255
};

// The valid values found in the Ehdr e_type field.

enum ET
{
	ET_NONE = 0,
	ET_REL = 1,
	ET_EXEC = 2,
	ET_DYN = 3,
	ET_CORE = 4,
	ET_LOOS = 0xfe00,
	ET_HIOS = 0xfeff,
	ET_LOPROC = 0xff00,
	ET_HIPROC = 0xffff
};

// The valid values found in the Ehdr e_machine field.

enum EM
{
	EM_NONE = 0,
	EM_M32 = 1,
	EM_SPARC = 2,
	EM_386 = 3,
	EM_68K = 4,
	EM_88K = 5,
	// 6 used to be EM_486
	EM_860 = 7,
	EM_MIPS = 8,
	EM_S370 = 9,
	EM_MIPS_RS3_LE = 10,
	// 11 was the old Sparc V9 ABI.
	// 12 through 14 are reserved.
	EM_PARISC = 15,
	// 16 is reserved.
	// Some old PowerPC object files use 17.
	EM_VPP500 = 17,
	EM_SPARC32PLUS = 18,
	EM_960 = 19,
	EM_PPC = 20,
	EM_PPC64 = 21,
	EM_S390 = 22,
	// 23 through 35 are served.
	EM_V800 = 36,
	EM_FR20 = 37,
	EM_RH32 = 38,
	EM_RCE = 39,
	EM_ARM = 40,
	EM_ALPHA = 41,
	EM_SH = 42,
	EM_SPARCV9 = 43,
	EM_TRICORE = 44,
	EM_ARC = 45,
	EM_H8_300 = 46,
	EM_H8_300H = 47,
	EM_H8S = 48,
	EM_H8_500 = 49,
	EM_IA_64 = 50,
	EM_MIPS_X = 51,
	EM_COLDFIRE = 52,
	EM_68HC12 = 53,
	EM_MMA = 54,
	EM_PCP = 55,
	EM_NCPU = 56,
	EM_NDR1 = 57,
	EM_STARCORE = 58,
	EM_ME16 = 59,
	EM_ST100 = 60,
	EM_TINYJ = 61,
	EM_X86_64 = 62,
	EM_PDSP = 63,
	EM_PDP10 = 64,
	EM_PDP11 = 65,
	EM_FX66 = 66,
	EM_ST9PLUS = 67,
	EM_ST7 = 68,
	EM_68HC16 = 69,
	EM_68HC11 = 70,
	EM_68HC08 = 71,
	EM_68HC05 = 72,
	EM_SVX = 73,
	EM_ST19 = 74,
	EM_VAX = 75,
	EM_CRIS = 76,
	EM_JAVELIN = 77,
	EM_FIREPATH = 78,
	EM_ZSP = 79,
	EM_MMIX = 80,
	EM_HUANY = 81,
	EM_PRISM = 82,
	EM_AVR = 83,
	EM_FR30 = 84,
	EM_D10V = 85,
	EM_D30V = 86,
	EM_V850 = 87,
	EM_M32R = 88,
	EM_MN10300 = 89,
	EM_MN10200 = 90,
	EM_PJ = 91,
	EM_OPENRISC = 92,
	EM_ARC_A5 = 93,
	EM_XTENSA = 94,
	EM_VIDEOCORE = 95,
	EM_TMM_GPP = 96,
	EM_NS32K = 97,
	EM_TPC = 98,
	// Some old picoJava object files use 99 (EM_PJ is correct).
	EM_SNP1K = 99,
	EM_ST200 = 100,
	EM_IP2K = 101,
	EM_MAX = 102,
	EM_CR = 103,
	EM_F2MC16 = 104,
	EM_MSP430 = 105,
	EM_BLACKFIN = 106,
	EM_SE_C33 = 107,
	EM_SEP = 108,
	EM_ARCA = 109,
	EM_UNICORE = 110,
	EM_ALTERA_NIOS2 = 113,
	EM_CRX = 114,
	// The Morph MT.
	EM_MT = 0x2530,
	// DLX.
	EM_DLX = 0x5aa5,
	// FRV.
	EM_FRV = 0x5441,
	// Infineon Technologies 16-bit microcontroller with C166-V2 core.
	EM_X16X = 0x4688,
	// Xstorym16
	EM_XSTORMY16 = 0xad45,
	// Renesas M32C
	EM_M32C = 0xfeb0,
	// Vitesse IQ2000
	EM_IQ2000 = 0xfeba,
	// NIOS
	EM_NIOS32 = 0xfebb
	// Old AVR objects used 0x1057 (EM_AVR is correct).
	// Old MSP430 objects used 0x1059 (EM_MSP430 is correct).
	// Old FR30 objects used 0x3330 (EM_FR30 is correct).
	// Old OpenRISC objects used 0x3426 and 0x8472 (EM_OPENRISC is correct).
	// Old D10V objects used 0x7650 (EM_D10V is correct).
	// Old D30V objects used 0x7676 (EM_D30V is correct).
	// Old IP2X objects used 0x8217 (EM_IP2K is correct).
	// Old PowerPC objects used 0x9025 (EM_PPC is correct).
	// Old Alpha objects used 0x9026 (EM_ALPHA is correct).
	// Old M32R objects used 0x9041 (EM_M32R is correct).
	// Old V850 objects used 0x9080 (EM_V850 is correct).
	// Old S/390 objects used 0xa390 (EM_S390 is correct).
	// Old Xtensa objects used 0xabc7 (EM_XTENSA is correct).
	// Old MN10300 objects used 0xbeef (EM_MN10300 is correct).
	// Old MN10200 objects used 0xdead (EM_MN10200 is correct).
};

// Special section indices.

enum
{
	SHN_UNDEF = 0,
	SHN_LORESERVE = 0xff00,
	SHN_LOPROC = 0xff00,
	SHN_HIPROC = 0xff1f,
	SHN_LOOS = 0xff20,
	SHN_HIOS = 0xff3f,
	SHN_ABS = 0xfff1,
	SHN_COMMON = 0xfff2,
	SHN_XINDEX = 0xffff,
	SHN_HIRESERVE = 0xffff,

	// Provide for initial and final section ordering in conjunction
	// with the SHF_LINK_ORDER and SHF_ORDERED section flags.
	SHN_BEFORE = 0xff00,
	SHN_AFTER = 0xff01,

	// x86_64 specific large common symbol.
	SHN_X86_64_LCOMMON = 0xff02
};

enum SHT
{
	SHT_NULL = 0,
	SHT_PROGBITS = 1,
	SHT_SYMTAB = 2,
	SHT_STRTAB = 3,
	SHT_RELA = 4,
	SHT_HASH = 5,
	SHT_DYNAMIC = 6,
	SHT_NOTE = 7,
	SHT_NOBITS = 8,
	SHT_REL = 9,
	SHT_SHLIB = 10,
	SHT_DYNSYM = 11,
	SHT_INIT_ARRAY = 14,
	SHT_FINI_ARRAY = 15,
	SHT_PREINIT_ARRAY = 16,
	SHT_GROUP = 17,
	SHT_SYMTAB_SHNDX = 18,
	SHT_LOOS = 0x60000000,
	SHT_HIOS = 0x6fffffff,
	SHT_LOPROC = 0x70000000,
	SHT_HIPROC = 0x7fffffff,
	SHT_LOUSER = 0x80000000,
	SHT_HIUSER = 0xffffffff,
	// The remaining values are not in the standard.
	// Incremental build data.
	SHT_GNU_INCREMENTAL_INPUTS = 0x6fff4700,
	// Object attributes.
	SHT_GNU_ATTRIBUTES = 0x6ffffff5,
	// GNU style dynamic hash table.
	SHT_GNU_HASH = 0x6ffffff6,
	// List of prelink dependencies.
	SHT_GNU_LIBLIST = 0x6ffffff7,
	// Versions defined by file.
	SHT_SUNW_verdef = 0x6ffffffd,
	SHT_GNU_verdef = 0x6ffffffd,
	// Versions needed by file.
	SHT_SUNW_verneed = 0x6ffffffe,
	SHT_GNU_verneed = 0x6ffffffe,
	// Symbol versions,
	SHT_SUNW_versym = 0x6fffffff,
	SHT_GNU_versym = 0x6fffffff,

	SHT_SPARC_GOTDATA = 0x70000000,

	// ARM-specific section types.
	// Exception Index table.
	SHT_ARM_EXIDX = 0x70000001,
	// BPABI DLL dynamic linking pre-emption map.
	SHT_ARM_PREEMPTMAP = 0x70000002,
	// Object file compatibility attributes.
	SHT_ARM_ATTRIBUTES = 0x70000003,
	// Support for debugging overlaid programs.
	SHT_ARM_DEBUGOVERLAY = 0x70000004,
	SHT_ARM_OVERLAYSECTION = 0x70000005,

	// x86_64 unwind information.
	SHT_X86_64_UNWIND = 0x70000001,

	// Link editor is to sort the entries in this section based on the
	// address specified in the associated symbol table entry.
	SHT_ORDERED = 0x7fffffff,
};

enum SHF
{
	SHF_WRITE = 0x1,
	SHF_ALLOC = 0x2,
	SHF_EXECINSTR = 0x4,
	SHF_MERGE = 0x10,
	SHF_STRINGS = 0x20,
	SHF_INFO_LINK = 0x40,
	SHF_LINK_ORDER = 0x80,
	SHF_OS_NONCONFORMING = 0x100,
	SHF_GROUP = 0x200,
	SHF_TLS = 0x400,
	SHF_MASKOS = 0x0ff00000,
	SHF_MASKPROC = 0xf0000000,

	// Indicates this section requires ordering in relation to
	// other sections of the same type.  Ordered sections are
	// combined within the section pointed to by the sh_link entry.
	// The sh_info values SHN_BEFORE and SHN_AFTER imply that the
	// sorted section is to precede or follow, respectively, all
	// other sections in the set being ordered.
	SHF_ORDERED = 0x40000000,
	// This section is excluded from input to the link-edit of an
	// executable or shared object.  This flag is ignored if SHF_ALLOC
	// is also set, or if relocations exist against the section.
	SHF_EXCLUDE = 0x80000000,

	// x86_64 specific large section.
	SHF_X86_64_LARGE = 0x10000000
};

// Symbol binding from Sym st_info field.

enum STB
{
	STB_LOCAL = 0,
	STB_GLOBAL = 1,
	STB_WEAK = 2,
	STB_LOOS = 10,
	//  STB_GNU_UNIQUE = 10,
	STB_HIOS = 12,
	STB_LOPROC = 13,
	STB_HIPROC = 15
};

// Symbol types from Sym st_info field.

enum STT
{
	STT_NOTYPE = 0,
	STT_OBJECT = 1,
	STT_FUNC = 2,
	STT_SECTION = 3,
	STT_FILE = 4,
	STT_COMMON = 5,
	STT_TLS = 6,
	STT_LOOS = 10,
	//  STT_GNU_IFUNC = 10,
	STT_HIOS = 12,
	STT_LOPROC = 13,
	STT_HIPROC = 15,

	// The section type that must be used for register symbols on
	// Sparc.  These symbols initialize a global register.
	//STT_SPARC_REGISTER = 13,

	// ARM: a THUMB function.  This is not defined in ARM ELF Specification but
	// used by the GNU tool-chain.
	//STT_ARM_TFUNC = 13,
};

#define ELF32_ST_BIND(i)   ((i)>>4)
#define ELF32_ST_TYPE(i)   ((i)&0xf)
#define ELF32_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

#define ELF32_R_SYM(i)	((i)>>8)
#define ELF32_R_TYPE(i)   ((unsigned char)(i))

// http://tomoyo.sourceforge.jp/cgi-bin/lxr/source/arch/um/sys-i386/asm/elf.h?a=ppc#L12
// http://tomoyo.sourceforge.jp/cgi-bin/lxr/source/arch/x86/kernel/module.c?a=ppc#L94

#define R_386_NONE	0
#define R_386_32	1
#define R_386_PC32	2
#define R_386_GOT32	3
#define R_386_PLT32	4
#define R_386_COPY	5
#define R_386_GLOB_DAT	6
#define R_386_JMP_SLOT	7
#define R_386_RELATIVE	8
#define R_386_GOTOFF	9
#define R_386_GOTPC	10
#define R_386_NUM	11

/* special section indexes */
#define SHN_UNDEF	0
#define SHN_LORESERVE	0xff00
#define SHN_LOPROC	0xff00
#define SHN_HIPROC	0xff1f
#define SHN_ABS		0xfff1
#define SHN_COMMON	0xfff2
#define SHN_HIRESERVE	0xffff
