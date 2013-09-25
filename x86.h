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
#define FLAG_AF (1<<4) // 0x10
#define FLAG_ZF (1<<6) // 0x40
#define FLAG_SF (1<<7) // 0x80
#define FLAG_TF (1<<8) // 0x100
#define FLAG_DF (1<<10) // 0x400
#define FLAG_OF (1<<11) // 0x800
#define FLAG_RF (1<<16) // 0x10000

// 0x8d5
#define FLAG_PSAZOC (FLAG_PF | FLAG_SF | FLAG_AF | FLAG_ZF | FLAG_OF | FLAG_CF)

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

#define FPU_TAG_NON_ZERO 0
#define FPU_TAG_ZERO 1
#define FPU_TAG_NAN 2
#define FPU_TAG_EMPTY 3

#define FPU_TOP(r) (((r)>>11)&7)
void FCW_to_str (uint16_t a, strbuf *out);
void FSW_to_str (uint16_t a, strbuf *out);

void XMM_to_strbuf (byte* p, strbuf *sb);

bool sse_supported();
bool sse2_supported();

#ifdef _WIN64
#define AX_REGISTER_NAME "RAX"
#define BX_REGISTER_NAME "RBX"
#define CX_REGISTER_NAME "RCX"
#define DX_REGISTER_NAME "RDX"
#define SI_REGISTER_NAME "RSI"
#define BP_REGISTER_NAME "RBP"
#else
#define AX_REGISTER_NAME "EAX"
#define BX_REGISTER_NAME "EBX"
#define CX_REGISTER_NAME "ECX"
#define DX_REGISTER_NAME "EDX"
#define SI_REGISTER_NAME "ESI"
#define BP_REGISTER_NAME "EBP"
#endif

// used for testing in tracer/cc and bolt/x86 emulator
#define X86_INC_EAX "\x40"
#define X86_INC_EAX_LEN 1
#define X64_INC_RAX "\x48\xFF\xC0"
#define X64_INC_RAX_LEN 3
#ifdef _WIN64
#define X86_OR_X64_INC_AX       X64_INC_RAX
#define X86_OR_X64_INC_AX_LEN   X64_INC_RAX_LEN
#else
#define X86_OR_X64_INC_AX       X86_INC_EAX
#define X86_OR_X64_INC_AX_LEN   X86_INC_EAX_LEN
#endif

#define X86_MOV_EAX_ESI "\x89\xF0"
#define X86_MOV_EAX_ESI_LEN 2
#define X64_MOV_RAX_RSI "\x48\x89\xF0"
#define X64_MOV_RAX_RSI_LEN 3
#ifdef _WIN64
#define X86_OR_X64_MOV_AX_SI        X64_MOV_RAX_RSI
#define X86_OR_X64_MOV_AX_SI_LEN    X64_MOV_RAX_RSI_LEN
#else
#define X86_OR_X64_MOV_AX_SI        X86_MOV_EAX_ESI
#define X86_OR_X64_MOV_AX_SI_LEN    X86_MOV_EAX_ESI_LEN
#endif

#define X86_FSTP_ESI "\xD9\x1E"
#define X86_FSTP_ESI_LEN 2
#define X64_FSTP_RSI "\x48\xDD\x1E"
#define X64_FSTP_RSI_LEN 3
#ifdef _WIN64
#define X86_OR_X64_FSTP_SI      X64_FSTP_RSI
#define X86_OR_X64_FSTP_SI_LEN  X64_FSTP_RSI_LEN
#else
#define X86_OR_X64_FSTP_SI      X86_FSTP_ESI
#define X86_OR_X64_FSTP_SI_LEN  X86_FSTP_ESI_LEN
#endif

#define X86_OR_X64_JA_NEXT "\x77\x00"
#define X86_OR_X64_JA_NEXT_LEN 2

#define X86_OR_X64_CALL_xAX "\xFF\xD0"
#define X86_OR_X64_CALL_xAX_LEN 2

#define X86_CMP_EAX_EBX "\x39\xD8"
#define X86_CMP_EAX_EBX_LEN 2
#define X64_CMP_RAX_RBX "\x48\x39\xD8"
#define X64_CMP_RAX_RBX_LEN 3
#ifdef _WIN64
#define X86_OR_X64_CMP_xAX_xBX      X64_CMP_RAX_RBX 
#define X86_OR_X64_CMP_xAX_xBX_LEN  X64_CMP_RAX_RBX_LEN
#else
#define X86_OR_X64_CMP_xAX_xBX      X86_CMP_EAX_EBX 
#define X86_OR_X64_CMP_xAX_xBX_LEN  X86_CMP_EAX_EBX_LEN
#endif

#define X86_SHR_OP_ESI_CP_CL "\xD3\x2E"
#define X86_SHR_OP_ESI_CP_CL_LEN 2
#define X86_SHL_OP_ESI_CP_CL "\xD3\x26"
#define X86_SHL_OP_ESI_CP_CL_LEN 2
#define X86_SAR_OP_ESI_CP_CL "\xD3\x3E"
#define X86_SAR_OP_ESI_CP_CL_LEN 2
#define X86_ADD_EAX_EBX "\x01\xD8"
#define X86_ADC_EAX_EBX "\x11\xD8"
#define X86_SUB_EAX_EBX "\x29\xD8"
#define X86_SBB_EAX_EBX "\x19\xD8"
#define X86_XOR_EAX_EBX "\x31\xD8"
#define X86_OR_EAX_EBX "\x09\xD8"
#define X86_AND_EAX_EBX "\x21\xD8"
#define X86_MOVZX_EAX_AL "\x0F\xB6\xC0"
#define X86_MOVZX_EAX_AX "\x0F\xB7\xC0"
#define X86_MOVSX_EAX_AL "\x0F\xBE\xC0"
#define X86_MOVSX_EAX_AX "\x0F\xBF\xC0"
#define X86_MOVZX_AX_AL "\x66\x0F\xB6\xC0"
#define X86_MOVSX_AX_AL "\x66\x0F\xBE\xC0"
#define X86_NOT_EAX "\xF7\xD0"
#define X86_NEG_EAX "\xF7\xD8"

/* vim: set expandtab ts=4 sw=4 : */
