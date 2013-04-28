#pragma once

#include <stdint.h>

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
    uint32_t flags;
    s_EFLAGS s;
} u_EFLAGS;

void flags_to_str (uint32_t flags, strbuf *out);
void dump_flags (fds *s, uint32_t flags);
void DR7_to_str (uint32_t DR7, strbuf* out);
void dump_DR7 (fds* s, uint32_t DR7);
void MXCSR_to_str (uint32_t a, strbuf *out);
void FCW_to_str (uint16_t a, strbuf *out);
void FSW_to_str (uint16_t a, strbuf *out);

// renamed from dump_XMM
void XMM_to_strbuf (uint8_t* p, strbuf *sb);

bool sse_supported();
bool sse2_supported();
