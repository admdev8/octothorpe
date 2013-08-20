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

#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <stdarg.h>

#include "datatypes.h"

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

char *elf_get_symbol_name(byte *buf, Elf32_Sym *s);

#ifdef  __cplusplus
extern "C" {
#endif

bool elf_chk_header(byte *buf);
void elf_dump_hdr (byte *buf);
byte* elf_get_ptr_to_section_start(byte* buf, int sect_n);
Elf32_Half elf_find_symtab_section (byte *buf);
char* elf_get_str_from_shstr(byte* buf, int idx);
void elf_dump_section (byte* buf, Elf32_Shdr *sect);
Elf32_Sym *elf_get_n_symbol(byte* buf, int n);
void elf_dump_sym (byte* buf, Elf32_Sym *sym);
Elf32_Sym *elf_find_symbol_by_name (byte* buf, const char *name);
Elf32_Sym *elf_find_symbol_by_sect_and_offset (byte* buf, int sect_n, byte* point);
void elf_dump_all_sections(byte *buf);
Elf32_Shdr* elf_find_rel_section_for_section(byte *buf, int sect_n);
void elf_dump_all_symbols (byte *buf);
byte *elf_get_ptr_to_symbol_start(byte* buf, Elf32_Sym *s);
byte *elf_get_ptr_to_symbol_start_by_name(byte* buf, const char *name);
tetrabyte elf_get_tetrabyte_from_symbol_by_name_or_die(byte* buf, const char *name);
Elf32_Rel* elf_find_reloc_for_sect_and_ofs (byte* buf, int sect_n, Elf32_Addr offset);
Elf32_Rel *elf_find_reloc_for_ofs_in_buf (byte* buf, byte *ofs_in_buf, Elf32_Sym **outsym);
byte *elf_dereference_tetrabyte_in_buf (byte *buf, tetrabyte* point);

// used for qsort()
int elf_cmp_sizes_asc(const void *_p1, const void *_p2);
int elf_cmp_sizes_desc(const void *_p1, const void *_p2);

const char *elf_rel_type_to_string(int t);
void elf_dump_biggest_data_objects(byte *buf, int n);
Elf32_Shdr *elf_find_section_for_point_in_buf (byte *buf, byte *point, Elf32_Half *out_sect_n);
Elf32_Sym *elf_get_symbol_of_tetrabyte_in_buf (byte* buf, tetrabyte* point);
char *elf_get_symbol_name_of_tetrabyte_in_buf_or_NULL(byte *buf, tetrabyte* point);

#ifdef  __cplusplus
}
#endif

/* vim: set expandtab ts=4 sw=4 : */
