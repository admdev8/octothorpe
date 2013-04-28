#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <stdarg.h>

#include "elf_structures.h"

#define elf_get_ptr_to_hdr(buf) ((Elf32_Ehdr *)buf)
#define elf_get_sections_total(buf) (elf_get_ptr_to_hdr(buf)->e_shnum)
#define elf_get_first_section(buf) ((Elf32_Shdr *)((buf) + elf_get_ptr_to_hdr(buf)->e_shoff))
#define elf_get_ptr_to_section_struc(buf,n) (elf_get_first_section (buf) + n)
#define elf_get_symtab_section(buf) (elf_get_ptr_to_section_struc ((buf), elf_find_symtab_section (buf)))
#define elf_get_symbols_total(buf) (elf_get_symtab_section(buf)->sh_size / elf_get_symtab_section(buf)->sh_entsize)
#define elf_get_first_symbol(buf) ((Elf32_Sym *)((buf) + elf_get_symtab_section(buf)->sh_offset))

// The section header index of the associated string table.
#define elf_get_strtable_of_symtab(buf) (elf_get_symtab_section(buf)->sh_link)

#define elf_get_symbol_name(buf, s) (elf_get_str_from_strtab ((buf), elf_get_strtable_of_symtab (buf), (s)->st_name))

#ifdef  __cplusplus
extern "C" {
#endif

bool elf_chk_header(uint8_t *buf);
void elf_dump_hdr (uint8_t *buf);
uint8_t* elf_get_ptr_to_section_start(uint8_t* buf, int n);
char *elf_get_str_from_strtab(uint8_t* buf,int sect_n,int idx);
Elf32_Half elf_find_symtab_section (uint8_t *buf);
char* elf_get_str_from_shstr(uint8_t* buf, int idx);
void elf_dump_section (uint8_t* buf, Elf32_Shdr *sect);
Elf32_Sym *elf_get_n_symbol(uint8_t* buf, int n);
void elf_dump_sym (uint8_t* buf, Elf32_Sym *sym);
Elf32_Sym *elf_find_symbol_by_name (uint8_t* buf, const char *name);
void elf_dump_all_sections(uint8_t *buf);
Elf32_Shdr* elf_find_rel_section_for_section(uint8_t *buf, int sect_n);
void elf_dump_all_symbols (uint8_t *buf);
uint8_t *elf_get_ptr_to_symbol_start(uint8_t* buf, Elf32_Sym *s);
uint8_t *elf_get_ptr_to_symbol_start_by_name(uint8_t* buf, const char *name);
uint32_t elf_get_uint32_from_symbol_or_die(uint8_t* buf, const char *name);
Elf32_Rel* elf_find_reloc_for_sect_and_ofs (uint8_t* buf, int sect_n, Elf32_Addr offset);
Elf32_Rel *elf_find_reloc_for_sect_and_ofs_in_buf (uint8_t* buf, int sect_n, uint8_t *ofs_in_buf, Elf32_Sym **outsym);
char *elf_can_this_tetrabyte_be_ptr_to (uint8_t *buf, int this_sect_n, uint8_t* point);

#ifdef  __cplusplus
}
#endif
