#include "elf.h"

#include "stuff.h"
#include "bitfields.h"

// Offsets within the Ehdr e_ident field.

#define EI_MAG0 0
#define EI_MAG1 1
#define EI_MAG2 2
#define EI_MAG3 3
#define EI_CLASS 4
#define EI_DATA 5
#define EI_VERSION 6
#define EI_OSABI 7
#define EI_ABIVERSION 8
#define EI_PAD 9

// The valid values found in Ehdr e_ident[EI_MAG0 through EI_MAG3].

#define ELFMAG0 0x7f
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'

bool elf_chk_header(byte *buf)
{
    Elf32_Ehdr *hdr=elf_get_ptr_to_hdr(buf);

    if (hdr->e_ident[EI_MAG0]!=ELFMAG0 || hdr->e_ident[EI_MAG1]!=ELFMAG1 || hdr->e_ident[EI_MAG2]!=ELFMAG2 || hdr->e_ident[EI_MAG3]!=ELFMAG3)
    {
        printf ("This file is not ELF\n");
        return false;
    };

    if (hdr->e_ident[EI_DATA]!=ELFDATA2LSB)
    {
        printf ("Not a LSB ELF\n");
        return false;
    };
    assert (hdr->e_ident[EI_VERSION]==EV_CURRENT);

    if (hdr->e_ident[EI_OSABI]!=ELFOSABI_LINUX && hdr->e_ident[EI_OSABI]!=ELFOSABI_NONE)
    {
        printf ("Not a Linux ELF (hdr->e_ident[EI_OSABI]=%d)\n", hdr->e_ident[EI_OSABI]);
        return false;
    };

    if (hdr->e_type!=ET_REL)
    {
        printf ("Not a relocatable file\n");
        return false;
    };

    if (hdr->e_machine!=EM_386)
    {
        printf ("Not a 80386 ELF\n");
        return false;
    };

    assert (hdr->e_version==EV_CURRENT);

    assert (hdr->e_shentsize == sizeof (Elf32_Shdr));

    return true;
};

void elf_dump_hdr (byte *buf)
{
    Elf32_Ehdr *hdr=elf_get_ptr_to_hdr(buf);

    printf ("hdr->e_entry=%d\n",     hdr->e_entry);
    printf ("hdr->e_phoff=0x%x\n",   hdr->e_phoff);
    printf ("hdr->e_shoff=0x%x\n",   hdr->e_shoff); // byte offset from the beginning of the file to the section header table
    printf ("hdr->e_flags=0x%x\n",   hdr->e_flags);
    printf ("hdr->e_ehsize=%d\n",    hdr->e_ehsize);
    printf ("hdr->e_phentsize=%d\n", hdr->e_phentsize);
    printf ("hdr->e_phnum=%d\n",     hdr->e_phnum);
    printf ("hdr->e_shentsize=%d\n", hdr->e_shentsize); // size in bytes of each entry.
    printf ("hdr->e_shnum=%d\n",     hdr->e_shnum); // how many entries the section header table contains
    printf ("hdr->e_shstrndx=%d\n",  hdr->e_shstrndx);
};

byte* elf_get_ptr_to_section_start(byte* buf, int n)
{
    assert (n < elf_get_sections_total(buf));
    return buf + elf_get_ptr_to_section_struc (buf, n)->sh_offset;
};

char *elf_get_str_from_strtab(byte* buf,int sect_n,int idx)
{
    return (char*)(elf_get_ptr_to_section_start(buf,sect_n) + idx);
};

Elf32_Half elf_find_symtab_section (byte *buf)
{
    Elf32_Half i;

    // enumerate sections to find symbol table

    for (i=0; i<elf_get_sections_total (buf); i++) // generalize?
    {
        Elf32_Shdr *sect=elf_get_ptr_to_section_struc (buf, i); // generalize?

        if (sect->sh_type==SHT_SYMTAB)
        {
            assert (sect->sh_entsize == sizeof (Elf32_Sym));
            return i;
        };
    };

    die ("symbol table is not found!\n");
    return 0; // make compiler happy
};

char* elf_get_str_from_shstr(byte* buf, int idx)
{
    return elf_get_str_from_strtab (buf, elf_get_ptr_to_hdr(buf)->e_shstrndx, idx);
};

void elf_dump_section (byte* buf, Elf32_Shdr *sect)
{
    printf ("sect->sh_name=%s\n", elf_get_str_from_shstr (buf, sect->sh_name));
    printf ("sect->sh_type=");
    switch (sect->sh_type)
    {
        case SHT_NULL:        
            printf ("SHT_NULL\n");        
            break;
        case SHT_PROGBITS:    
            printf ("SHT_PROGBITS\n");    
            break;
        case SHT_SYMTAB:    
            printf ("SHT_SYMTAB\n");        
            break;
        case SHT_STRTAB:    
            printf ("SHT_STRTAB\n");        
            break;
        case SHT_NOTE:        
            printf ("SHT_NOTE\n");        
            break;
        case SHT_REL:        
            printf ("SHT_REL\n");        
            break;
        case SHT_NOBITS:    
            printf ("SHT_NOBITS\n");        
            break;
        case SHT_GROUP:        
            printf ("SHT_GROUP\n");        
            break;
        default:    
            printf ("unknown sh_type=%d\n", sect->sh_type); // not yet listed HERE
            assert(0);
            break;
    };
    printf ("sect->sh_flags=");
    if (IS_SET (sect->sh_flags, SHF_WRITE))             printf ("SHF_WRITE ");
    if (IS_SET (sect->sh_flags, SHF_ALLOC))             printf ("SHF_ALLOC ");
    if (IS_SET (sect->sh_flags, SHF_EXECINSTR))         printf ("SHF_EXECINSTR ");
    if (IS_SET (sect->sh_flags, SHF_MERGE))             printf ("SHF_MERGE ");
    if (IS_SET (sect->sh_flags, SHF_STRINGS))           printf ("SHF_STRINGS ");
    if (IS_SET (sect->sh_flags, SHF_INFO_LINK))         printf ("SHF_INFO_LINK ");
    if (IS_SET (sect->sh_flags, SHF_LINK_ORDER))        printf ("SHF_LINK_ORDER ");
    if (IS_SET (sect->sh_flags, SHF_OS_NONCONFORMING))  printf ("SHF_OS_NONCONFORMING ");
    if (IS_SET (sect->sh_flags, SHF_GROUP))             printf ("SHF_GROUP ");
    if (IS_SET (sect->sh_flags, SHF_TLS))               printf ("SHF_TLS ");
    if (IS_SET (sect->sh_flags, SHF_MASKOS))            printf ("SHF_MASKOS ");
    if (IS_SET (sect->sh_flags, SHF_MASKPROC))          printf ("SHF_MASKPROC ");
    printf ("\n");

    printf ("sect->sh_addr=%d\n", sect->sh_addr);
    printf ("sect->sh_offset=0x%x\n", sect->sh_offset);
    printf ("sect->sh_size=%d\n", sect->sh_size);
    printf ("sect->sh_link=%d\n", sect->sh_link);
    printf ("sect->sh_info=%d\n", sect->sh_info);
    printf ("sect->sh_addralign=%d\n", sect->sh_addralign);
    printf ("sect->sh_entsize=%d\n", sect->sh_entsize);
};

Elf32_Sym *elf_get_n_symbol(byte* buf, int n)
{
    assert (n < elf_get_symbols_total(buf));
    return (Elf32_Sym *)elf_get_first_symbol(buf) + n;
};

void elf_dump_sym (byte* buf, Elf32_Sym *sym)
{
    char *bnd_s;
    char *typ_s;

    switch (ELF32_ST_BIND (sym->st_info))
    {
        case STB_LOCAL:        
            bnd_s="STB_LOCAL"; 
            break;
        case STB_GLOBAL:    
            bnd_s="STB_GLOBAL"; 
            break;
        case STB_WEAK:        
            bnd_s="STB_WEAK"; 
            break;
        case STB_LOOS:        
            bnd_s="STB_LOOS"; 
            break;
            //case STB_GNU_UNIQUE: 
            //bnd_s="STB_GNU_UNIQUE"; 
            //break;
        case STB_HIOS:        
            bnd_s="STB_HIOS"; 
            break;
        case STB_LOPROC:    
            bnd_s="STB_LOPROC"; 
            break;
        case STB_HIPROC:    
            bnd_s="STB_HIPROC"; 
            break;
        default: 
            assert(0);
    };

    switch (ELF32_ST_TYPE (sym->st_info))
    {
        case STT_NOTYPE:    
            typ_s="STT_NOTYPE"; 
            break;
        case STT_OBJECT:    
            typ_s="STT_OBJECT"; 
            break;
        case STT_FUNC:        
            typ_s="STT_FUNC"; 
            break;
        case STT_SECTION:    
            typ_s="STT_SECTION"; 
            break;
        case STT_FILE:        
            typ_s="STT_FILE"; 
            break;
        case STT_COMMON:    
            typ_s="STT_COMMON"; 
            break;
        case STT_TLS:        
            typ_s="STT_TLS"; 
            break;
        case STT_LOOS:        
            typ_s="STT_LOOS"; 
            break;
            //case STT_GNU_IFUNC: 
            //typ_s="STT_GNU_IFUNC"; 
            //break;
        case STT_HIOS:        
            typ_s="STT_HIOS"; 
            break;
        case STT_LOPROC:    
            typ_s="STT_LOPROC"; 
            break;
        case STT_HIPROC:    
            typ_s="STT_HIPROC"; 
            break;
            //case STT_SPARC_REGISTER: 
            //typ_s="STT_SPARC_REGISTER"; 
            //break;
            //case STT_ARM_TFUNC: 
            //typ_s="STT_ARM_TFUNC"; 
            //break;
        default:
            assert(0);
    };

    printf ("st_name=%-31s st_value=0x%08x st_size=%5d st_info=%-10s %-11s st_other=%d st_shndx=%d\n", 
            elf_get_symbol_name(buf, sym),
            sym->st_value, sym->st_size,
            bnd_s, typ_s, sym->st_other, sym->st_shndx);
};

Elf32_Sym *elf_find_symbol_by_name (byte* buf, const char *name)
{
    int i;
    Elf32_Sym * s;

    for (i=0,s=elf_get_first_symbol(buf); i<elf_get_symbols_total(buf); i++, s++)
        if (!strcmp (elf_get_symbol_name(buf, s), name))
            return s;
    
    // not found
    return NULL;
};

Elf32_Sym *elf_find_symbol_by_sect_and_offset (byte* buf, int sect_n, byte* point)
{
    int i;
    Elf32_Sym * s;
    Elf32_Addr value_should_be=point - elf_get_ptr_to_section_start(buf, sect_n); 
    unsigned symbols_total=elf_get_symbols_total(buf);
    unsigned sections_total=elf_get_sections_total(buf);
#if 0
    printf ("%s() value_should_be=0x%x\n", __func__, value_should_be);
    printf ("point=0x%x, buf=0x%x, elf_get_ptr_to_section_start()=0x%x sect_n=%d\n",
            point, buf, elf_get_ptr_to_section_start(buf, sect_n), sect_n);
#endif
    for (i=0,s=elf_get_first_symbol(buf); i<symbols_total; i++, s++)
        if ((s->st_shndx < sections_total) &&
                (s->st_shndx==sect_n) &&
                (s->st_value==value_should_be))
            return s;

    // not found
    return NULL;
};

void elf_dump_all_sections(byte *buf)
{
    int i;
    Elf32_Shdr *s;

    for (i=0,s=elf_get_first_section(buf); i<elf_get_sections_total(buf); i++, s++)
    {
        elf_dump_section (buf, s);
        printf ("\n");
    };
};

Elf32_Shdr* elf_find_rel_section_for_section(byte *buf, int sect_n)
{
    int i;
    Elf32_Shdr *s;

    for (i=0,s=elf_get_first_section(buf); i<elf_get_sections_total(buf); i++, s++)
    {
        // sh_info - section of which relocs are applied
        if (s->sh_type==SHT_REL && s->sh_info==sect_n)
        {
            assert (s->sh_entsize==sizeof(Elf32_Rel));
            return s;
        };
    };
    return NULL; // rel section not found
};

void elf_dump_all_symbols (byte *buf)
{
    int i;
    Elf32_Sym * s;
    
    printf ("symbols total: %d\n", elf_get_symbols_total(buf));
    for (i=0,s=elf_get_first_symbol(buf); i<elf_get_symbols_total(buf); i++, s++)
        elf_dump_sym(buf, s);
};

byte *elf_get_ptr_to_symbol_start(byte* buf, Elf32_Sym *s)
{
    return elf_get_ptr_to_section_start(buf, s->st_shndx) + s->st_value;
};

byte *elf_get_ptr_to_symbol_start_by_name(byte* buf, const char *name)
{
    Elf32_Sym *s;

    s=elf_find_symbol_by_name(buf, name);
    if (s==NULL)
        return NULL; // symbol not found

    //elf_dump_sym(buf, s);

    return elf_get_ptr_to_symbol_start (buf, s);
};

tetrabyte elf_get_uint32_from_symbol_or_die(byte* buf, const char *name)
{
    byte *s;

    s=elf_get_ptr_to_symbol_start_by_name(buf, name);
    if (s==0)
        die ("Can't find symbol %s\n", name);

    return *(tetrabyte*)s;
};

Elf32_Rel* elf_find_reloc_for_sect_and_ofs (byte* buf, int sect_n, Elf32_Addr offset)
{
    Elf32_Shdr *rel_section;
    int relocs_total, i;
    Elf32_Rel *first_reloc, *r;

    rel_section=elf_find_rel_section_for_section(buf, sect_n);
    assert (rel_section);

    // enum all relocs
    
    relocs_total=rel_section->sh_size / rel_section->sh_entsize;
    first_reloc=(Elf32_Rel *)(buf + rel_section->sh_offset);

    for (i=0, r=first_reloc; i<relocs_total; i++, r++)
        if (r->r_offset==offset)
            return r;
    
    return NULL;
};

Elf32_Rel *elf_find_reloc_for_sect_and_ofs_in_buf (byte* buf, int sect_n, byte *ofs_in_buf, Elf32_Sym **outsym)
{
    byte *sect_start;
    Elf32_Addr ofs;
    Elf32_Rel *rt;

    sect_start=elf_get_ptr_to_section_start(buf, sect_n);

    ofs=ofs_in_buf - sect_start;
    
    rt=elf_find_reloc_for_sect_and_ofs (buf, sect_n, ofs);

    if (rt && outsym)
        *outsym=elf_get_n_symbol (buf, ELF32_R_SYM(rt->r_info));
    return rt;
};

byte *elf_can_this_tetrabyte_be_ptr_to (byte *buf, int this_sect_n, tetrabyte* point)
{
    Elf32_Rel *r;
    Elf32_Sym *sym;

    r=elf_find_reloc_for_sect_and_ofs_in_buf (buf, this_sect_n, (byte*)point, &sym); 
    if (r==NULL) // no reloc here
        return NULL; // yet

    assert (ELF32_R_TYPE(r->r_info)==R_386_32);

    return elf_get_ptr_to_symbol_start (buf, sym) + *point;
};

// used for qsort()
int elf_cmp_sizes_asc(const void *_p1, const void *_p2)
{
    const Elf32_Sym *p1=(const Elf32_Sym*)_p1;
    const Elf32_Sym *p2=(const Elf32_Sym*)_p2;

    if (p1->st_size>p2->st_size) return 1;
    if (p1->st_size<p2->st_size) return -1;
    return 0;
};

int elf_cmp_sizes_desc(const void *_p1, const void *_p2)
{
    return elf_cmp_sizes_asc(_p2, _p1);
};

static const char *rel_type[]=
{
    "R_386_NONE",
    "R_386_32",
    "R_386_PC32",
    "R_386_GOT32",
    "R_386_PLT32",
    "R_386_COPY",
    "R_386_GLOB_DAT",
    "R_386_JMP_SLOT",
    "R_386_RELATIVE",
    "R_386_GOTOFF",
    "R_386_GOTPC",
    "R_386_NUM"
};
const char *elf_rel_type_to_string(int t)
{
    if (t>R_386_NUM)
        return "?";
    return rel_type[t];
};
