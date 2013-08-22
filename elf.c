/*
 *             _        _   _                           
 *            | |      | | | |                          
 *   ___   ___| |_ ___ | |_| |__   ___  _ __ _ __   ___ 
 *  / _ \ / __| __/ _ \| __| '_ \ / _ \| '__| '_ \ / _ \
 * | (_) | (__| || (_) | |_| | | | (_) | |  | |_) |  __/
 *  \___/ \___|\__\___/ \__|_| |_|\___/|_|  | .__/ \___|
 *                                          | |         
 *                                          |_|
 *
 * Written by Dennis Yurichev <dennis(a)yurichev.com>, 2013
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/3.0/.
 *
 */

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

byte* elf_get_ptr_to_section_start(byte* buf, int sect_n)
{
    assert (sect_n < elf_get_sections_total(buf));
    return buf + elf_get_ptr_to_section_struc (buf, sect_n)->sh_offset;
};

Elf32_Half elf_find_symtab_section (byte *buf)
{
    Elf32_Half i;
    Elf32_Shdr *s;

    // enumerate sections to find symbol table
    for (i=0,s=elf_get_first_section(buf); i<elf_get_sections_total (buf); i++, s++)
        if (s->sh_type==SHT_SYMTAB)
        {
            assert (s->sh_entsize == sizeof (Elf32_Sym)); // just to be sure 
            return i;
        };

    die ("symbol table is not found!\n");
};

char* elf_get_str_from_shstr(byte* buf, int idx)
{
    return (char*)elf_get_ptr_to_section_start (buf, elf_get_ptr_to_hdr(buf)->e_shstrndx) + idx;
};

char *elf_get_symbol_name(byte *buf, Elf32_Sym *s)
{
    return (char*)elf_get_ptr_to_section_start ((buf), elf_get_strtable_of_symtab (buf)) + (s)->st_name;
};

const char *SHT_x[]=
{
    "SHT_NULL", // 0
    "SHT_PROGBITS", // 1
    "SHT_SYMTAB", // 2
    "SHT_STRTAB", // 3
    "SHT_RELA", // 4
    "SHT_HASH", // 5
    "SHT_DYNAMIC", // 6
    "SHT_NOTE", // 7
    "SHT_NOBITS", // 8
    "SHT_REL", // 9
    "SHT_SHLIB", // 10
    "SHT_DYNSYM", // 11
    "?",
    "?",
    "SHT_INIT_ARRAY", // 14
    "SHT_FINI_ARRAY", // 15
    "SHT_PREINIT_ARRAY", // 16
    "SHT_GROUP", // 17
    "SHT_SYMTAB_SHNDX" // 18
};

void elf_dump_section (byte* buf, Elf32_Shdr *sect)
{
    printf ("sect->sh_name=%s\n", elf_get_str_from_shstr (buf, sect->sh_name));
    printf ("sect->sh_type=");
    if (sect->sh_type>SHT_SYMTAB_SHNDX)
        printf ("?\n");
    else
        printf ("%s\n", SHT_x[sect->sh_type]);

    printf ("sect->sh_flags=");
    if (IS_SET (sect->sh_flags, SHF_WRITE))             
        printf ("SHF_WRITE ");
    if (IS_SET (sect->sh_flags, SHF_ALLOC))             
        printf ("SHF_ALLOC ");
    if (IS_SET (sect->sh_flags, SHF_EXECINSTR))         
        printf ("SHF_EXECINSTR ");
    if (IS_SET (sect->sh_flags, SHF_MERGE))             
        printf ("SHF_MERGE ");
    if (IS_SET (sect->sh_flags, SHF_STRINGS))           
        printf ("SHF_STRINGS ");
    if (IS_SET (sect->sh_flags, SHF_INFO_LINK))         
        printf ("SHF_INFO_LINK ");
    if (IS_SET (sect->sh_flags, SHF_LINK_ORDER))        
        printf ("SHF_LINK_ORDER ");
    if (IS_SET (sect->sh_flags, SHF_OS_NONCONFORMING))  
        printf ("SHF_OS_NONCONFORMING ");
    if (IS_SET (sect->sh_flags, SHF_GROUP))             
        printf ("SHF_GROUP ");
    if (IS_SET (sect->sh_flags, SHF_TLS))               
        printf ("SHF_TLS ");
    if (IS_SET (sect->sh_flags, SHF_MASKOS))            
        printf ("SHF_MASKOS ");
    if (IS_SET (sect->sh_flags, SHF_MASKPROC))          
        printf ("SHF_MASKPROC ");
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

static const char* STB_x[]=
{
    "STB_LOCAL", // 0
    "STB_GLOBAL", // 1
    "STB_WEAK", // 2
    "?", // 3
    "?", // 4
    "?", // 5
    "?", // 6
    "?", // 7
    "?", // 8
    "?", // 9
    "STB_LOOS", // 10
    "?", // 11
    "STB_HIOS", // 12
    "STB_LOPROC", // 13
    "?", // 14
    "STB_HIPROC" // 15
};

const char *STT_x[]=
{
    "STT_NOTYPE", // 0
    "STT_OBJECT", // 1
    "STT_FUNC", // 2
    "STT_SECTION", // 3
    "STT_FILE", // 4
    "STT_COMMON", // 5
    "STT_TLS", // 6
    "?", // 7
    "?", // 8
    "?", // 9
    "STT_LOOS", // 10
    "?", // 11
    "STT_HIOS", // 12
    "STT_LOPROC", // 13
    "?", // 14
    "STT_HIPROC" // 15
};

void elf_dump_sym (byte* buf, Elf32_Sym *sym)
{
    const char *bnd_s;
    const char *typ_s;

    if (ELF32_ST_BIND (sym->st_info)>STB_HIPROC)
        bnd_s="?";
    else
        bnd_s=STB_x[ELF32_ST_BIND (sym->st_info)];

    if (ELF32_ST_TYPE (sym->st_info)>STT_HIPROC)
        typ_s="?";
    else
        typ_s=STT_x[ELF32_ST_TYPE (sym->st_info)];

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

    return elf_get_ptr_to_symbol_start (buf, s);
};

tetrabyte elf_get_tetrabyte_from_symbol_by_name_or_die(byte* buf, const char *name)
{
    byte *s;

    s=elf_get_ptr_to_symbol_start_by_name(buf, name);
    if (s==0)
        die ("Can't find symbol %s\n", name);

    return *(tetrabyte*)s;
};

// FIXME: slow
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

Elf32_Shdr *elf_find_section_for_point_in_buf (byte *buf, byte *point, Elf32_Half *out_sect_n)
{
    Elf32_Half i;
    Elf32_Shdr *s;
    Elf32_Off ofs=point-buf;

    // enumerate sections
    for (i=0,s=elf_get_first_section(buf); i<elf_get_sections_total (buf); i++, s++)
        if (ofs >= s->sh_offset && ofs < (s->sh_offset + s->sh_size))
        {
            if (out_sect_n)
                *out_sect_n=i;
            return s;
        };
    return NULL;
};

Elf32_Rel *elf_find_reloc_for_ofs_in_buf (byte* buf, byte *ofs_in_buf, Elf32_Sym **outsym)
{
    byte *sect_start;
    Elf32_Addr ofs_in_section;
    Elf32_Rel *rt;
    Elf32_Shdr *sect;
    Elf32_Half sect_n;

    sect=elf_find_section_for_point_in_buf(buf, ofs_in_buf, &sect_n);

    sect_start = buf + sect->sh_offset;

    ofs_in_section=ofs_in_buf - sect_start;

    rt=elf_find_reloc_for_sect_and_ofs (buf, sect_n, ofs_in_section);

    if (rt && outsym)
        *outsym=elf_get_n_symbol (buf, ELF32_R_SYM(rt->r_info));
    return rt;
};

byte *elf_dereference_tetrabyte_in_buf (byte *buf, tetrabyte* point)
{
    Elf32_Rel *r;
    Elf32_Sym *sym;

    r=elf_find_reloc_for_ofs_in_buf (buf, (byte*)point, &sym); 
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

void elf_dump_biggest_data_objects(byte *buf, int n)
{
    int i, dumped=0, symbols_total=elf_get_symbols_total(buf);
    Elf32_Sym *first_sym=elf_get_first_symbol(buf), *copy, *s;

    copy=(Elf32_Sym*)memdup(first_sym, symbols_total*sizeof(Elf32_Sym));

    qsort (copy, symbols_total, sizeof(Elf32_Sym), &elf_cmp_sizes_desc);

    for (i=0,s=copy; i<symbols_total && dumped<n; i++, s++)
        if (ELF32_ST_TYPE(s->st_info)==STT_OBJECT)
        {
            elf_dump_sym(buf, s);
            dumped++;
        };

    free(copy);
};

Elf32_Sym *elf_get_symbol_of_tetrabyte_in_buf (byte* buf, tetrabyte* point)
{
    Elf32_Sym *s, *rt;
    Elf32_Rel *rel=elf_find_reloc_for_ofs_in_buf (buf, (byte*)point, &s);

    if (rel==NULL)
        return NULL;

    if (*point==0)
        return s; // already found

    // add tetrabyte contents and find symbol at this place...
    rt=elf_find_symbol_by_sect_and_offset (buf, s->st_shndx, 
            elf_get_ptr_to_symbol_start (buf, s) + *point);
    return rt;
};

char *elf_get_symbol_name_of_tetrabyte_in_buf_or_NULL(byte *buf, tetrabyte* point)
{
    Elf32_Sym *s;
   
    // no reloc at this point and value is zero?
    if (elf_find_reloc_for_ofs_in_buf (buf, (byte*)point, NULL)==NULL && *point==0)
        return "NULL";

    s=elf_get_symbol_of_tetrabyte_in_buf(buf, point);
    if (s)
        return elf_get_symbol_name(buf, s);
    else
        return "<symbol not found>";
};

/* vim: set expandtab ts=4 sw=4 : */
