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

#include "datatypes.h"

#include "strbuf.h"
#include "logging.h"

#define FLAG_CF (1<<0)
#define FLAG_RSRV1 (1<<1)
#define FLAG_PF (1<<2)
#define FLAG_AF (1<<4)
#define FLAG_ZF (1<<6)
#define FLAG_SF (1<<7)
#define FLAG_TF (1<<8)
#define FLAG_DF (1<<10)
#define FLAG_OF (1<<11)
#define FLAG_RF (1<<16)

typedef struct _s_EFLAGS
{
    unsigned CF : 1;
    unsigned reserved1 : 1;
    unsigned PF : 1;
    unsigned reserved2 : 1;
    unsigned AF : 1;
    unsigned reserved3 : 1;
    unsigned ZF : 1;
    unsigned SF : 1;
    unsigned TF : 1;
    unsigned IF : 1;
    unsigned DF : 1;
    unsigned OF : 1;
    unsigned IOPL : 2;
    unsigned NT : 1;
    unsigned reserved4 : 1;
    unsigned RF : 1;
    unsigned VM : 1;
    unsigned AC : 1;
    unsigned VIF : 1;
    unsigned VIP : 1;
    unsigned ID : 1;
} s_EFLAGS;

typedef union _u_EFLAGS
{
    tetrabyte flags;
    s_EFLAGS s;
} u_EFLAGS;

#define FLAG_DR6_B0 1<<0
#define FLAG_DR6_B1 1<<1
#define FLAG_DR6_B2 1<<2
#define FLAG_DR6_B3 1<<3
#define FLAG_DR6_BS 1<<14

#define FLAG_DR7_L0 1<<0
#define FLAG_DR7_G0 1<<1
#define FLAG_DR7_L1 1<<2
#define FLAG_DR7_G1 1<<3
#define FLAG_DR7_L2 1<<4
#define FLAG_DR7_G2 1<<5
#define FLAG_DR7_L3 1<<6
#define FLAG_DR7_G3 1<<7

#define FLAG_DR7_BP0_W 1<<16
#define FLAG_DR7_BP0_RW (1<<16 | 1 << 17)
#define FLAG_DR7_BP0_LEN_WYDE 1<<18
#define FLAG_DR7_BP0_LEN_TETRABYTE (1<<18 | 1<<19)
#define FLAG_DR7_BP0_LEN_OCTABYTE 1<<19

#define FLAG_DR7_BP1_W 1<<20
#define FLAG_DR7_BP1_RW (1<<20 | 1 << 21)
#define FLAG_DR7_BP1_LEN_WYDE 1<<22
#define FLAG_DR7_BP1_LEN_TETRABYTE (1<<22 | 1<<23)
#define FLAG_DR7_BP1_LEN_OCTABYTE 1<<23

#define FLAG_DR7_BP2_W 1<<24
#define FLAG_DR7_BP2_RW (1<<24 | 1 << 25)
#define FLAG_DR7_BP2_LEN_WYDE 1<<26
#define FLAG_DR7_BP2_LEN_TETRABYTE (1<<26 | 1<<27)
#define FLAG_DR7_BP2_LEN_OCTABYTE 1<<27

#define FLAG_DR7_BP3_W 1<<28
#define FLAG_DR7_BP3_RW (1<<28 | 1 << 29)
#define FLAG_DR7_BP3_LEN_WYDE 1<<30
#define FLAG_DR7_BP3_LEN_TETRABYTE (1<<30 | 1<<31)
#define FLAG_DR7_BP3_LEN_OCTABYTE 1<<31

void flags_to_str (tetrabyte flags, strbuf *out);
void dump_flags (fds *s, tetrabyte flags);
void DR7_to_str (tetrabyte DR7, strbuf* out);
void dump_DR6 (fds* s, tetrabyte DR6);
void dump_DR7 (fds* s, tetrabyte DR7);
void MXCSR_to_str (tetrabyte a, strbuf *out);
void FCW_to_str (uint16_t a, strbuf *out);
void FSW_to_str (uint16_t a, strbuf *out);

void XMM_to_strbuf (byte* p, strbuf *sb);

bool sse_supported();
bool sse2_supported();

/* vim: set expandtab ts=4 sw=4 : */
