#include "x86.h"

#include "bitfields.h"

void flags_to_str (uint32_t flags, strbuf *out)
{
    u_EFLAGS uf;
    uf.flags=flags;

    if (uf.s.CF) strbuf_addstr (out, "CF ");

    if (uf.s.PF) strbuf_addstr (out, "PF ");
    if (uf.s.AF) strbuf_addstr (out, "AF ");
    if (uf.s.ZF) strbuf_addstr (out, "ZF ");
    if (uf.s.SF) strbuf_addstr (out, "SF ");
    if (uf.s.TF) strbuf_addstr (out, "TP "); // aka TF
    if (uf.s.IF) strbuf_addstr (out, "IF ");
    if (uf.s.DF) strbuf_addstr (out, "DF ");
    if (uf.s.OF) strbuf_addstr (out, "OF ");
    //FIXME:
    //if (IS_SET (flags, 1<<12)) { if (r.size()) r+=" "; r+="IOPL_L"; };
    //if (IS_SET (flags, 1<<13)) { if (r.size()) r+=" "; r+="IOPL_H"; };
    if (uf.s.NT) strbuf_addstr (out, "NT ");
    if (uf.s.RF) strbuf_addstr (out, "RF ");
    if (uf.s.VM) strbuf_addstr (out, "VM ");
    if (uf.s.AC) strbuf_addstr (out, "AC ");
    if (uf.s.VIF) strbuf_addstr (out, "VIF ");
    if (uf.s.VIP) strbuf_addstr (out, "VIP ");
    if (uf.s.ID) strbuf_addstr (out, "ID ");

    // TODO trim right part
};

void dump_flags (fds *s, uint32_t flags)
{
    strbuf sb = STRBUF_INIT;
    flags_to_str(flags, &sb);

    L_fds (s, sb.buf);

    strbuf_deinit (&sb);
};

void DR7_to_str (uint32_t DR7, strbuf* out)
{
    strbuf_addf(out, "%s%s%s%s%s%s%s%s%s%s%sR/W0=%d%d LEN0=%d%d R/W1=%d%d LEN1=%d%d R/W2=%d%d LEN2=%d%d R/W3=%d%d LEN3=%d%d", 
            IS_SET (DR7, 1<<0) ? "L0 " : "   ",
            IS_SET (DR7, 1<<1) ? "G0 " : "   ",
            IS_SET (DR7, 1<<2) ? "L1 " : "   ",
            IS_SET (DR7, 1<<3) ? "G1 " : "   ",
            IS_SET (DR7, 1<<4) ? "L2 " : "   ",
            IS_SET (DR7, 1<<5) ? "G2 " : "   ",
            IS_SET (DR7, 1<<6) ? "L3 " : "   ",
            IS_SET (DR7, 1<<7) ? "G3 " : "   ",
            IS_SET (DR7, 1<<8) ? "LE " : "   ",
            IS_SET (DR7, 1<<9) ? "GE " : "   ",
            IS_SET (DR7, 1<<13) ? "GD " : "   ",

            IS_SET (DR7, 1<<16) ? 1 : 0, IS_SET (DR7, 1<<17) ? 1 : 0,  // R/W0
            IS_SET (DR7, 1<<18) ? 1 : 0, IS_SET (DR7, 1<<19) ? 1 : 0,  // LEN0
            IS_SET (DR7, 1<<20) ? 1 : 0, IS_SET (DR7, 1<<21) ? 1 : 0,  // R/W1
            IS_SET (DR7, 1<<22) ? 1 : 0, IS_SET (DR7, 1<<23) ? 1 : 0,  // LEN1
            IS_SET (DR7, 1<<24) ? 1 : 0, IS_SET (DR7, 1<<25) ? 1 : 0,  // R/W2
            IS_SET (DR7, 1<<26) ? 1 : 0, IS_SET (DR7, 1<<27) ? 1 : 0,  // LEN2
            IS_SET (DR7, 1<<28) ? 1 : 0, IS_SET (DR7, 1<<29) ? 1 : 0,  // R/W3
            IS_SET (DR7, 1<<30) ? 1 : 0, IS_SET (DR7, 1<<31) ? 1 : 0); // LEN3
};

void dump_DR7 (fds* s, uint32_t DR7)
{
    strbuf sb=STRBUF_INIT;
    DR7_to_str(DR7, &sb);
    L_fds (s, sb.buf);
    strbuf_deinit(&sb);
};

void MXCSR_to_str (uint32_t a, strbuf *out)
{
    if (IS_SET (a, 1<<15)) strbuf_adds(out, "FZ ");

    if (IS_SET (a, 1<<14)==0 && IS_SET (a, 1<<13)==0) strbuf_adds(out, "RN ");
    else
        if (IS_SET (a, 1<<14)==1 && IS_SET (a, 1<<13)==1) strbuf_adds(out, "RZ ");
        else
            if (IS_SET (a, 1<<14)) strbuf_adds(out, "R+ ");
            else
                if (IS_SET (a, 1<<13)) strbuf_adds(out, "R- ");

    if (IS_SET (a, 1<<12)) strbuf_adds(out, "PM ");
    if (IS_SET (a, 1<<11)) strbuf_adds(out, "UM ");
    if (IS_SET (a, 1<<10)) strbuf_adds(out, "OM ");
    if (IS_SET (a, 1<<9)) strbuf_adds(out, "ZM ");
    if (IS_SET (a, 1<<8)) strbuf_adds(out, "DM ");
    if (IS_SET (a, 1<<7)) strbuf_adds(out, "IM ");
    if (IS_SET (a, 1<<6)) strbuf_adds(out, "DAZ ");
    if (IS_SET (a, 1<<5)) strbuf_adds(out, "PE ");
    if (IS_SET (a, 1<<4)) strbuf_adds(out, "UE ");
    if (IS_SET (a, 1<<3)) strbuf_adds(out, "OE ");
    if (IS_SET (a, 1<<2)) strbuf_adds(out, "ZE ");
    if (IS_SET (a, 1<<1)) strbuf_adds(out, "DE ");
    if (IS_SET (a, 1<<0)) strbuf_adds(out, "IE ");

    // TODO trim right part
};

void FCW_to_str (uint16_t a, strbuf *out)
{
    if (IS_SET (a, 1<<12)) strbuf_adds(out, "IC ");
    switch ((a>>10)&3)
    {
        case 0: strbuf_adds(out, "RC=NEAR "); break;
        case 1: strbuf_adds(out, "RC=DOWN "); break;
        case 2: strbuf_adds(out, "RC=UP "); break;
        case 3: strbuf_adds(out, "RC=ZERO "); break;
        default: assert (0);
    };

    switch ((a>>8)&3)
    {
        case 0: strbuf_adds(out, "PC=24bits "); break;
        case 1: strbuf_adds(out, "PC=?? "); break;
        case 2: strbuf_adds(out, "PC=53bits "); break;
        case 3: strbuf_adds(out, "PC=64bits "); break;
        default: assert (0);
    };

    if (IS_SET (a, 1<<7)) strbuf_adds(out, "IEM ");
    if (IS_SET (a, 1<<5)) strbuf_adds(out, "PM ");
    if (IS_SET (a, 1<<4)) strbuf_adds(out, "UM ");
    if (IS_SET (a, 1<<3)) strbuf_adds(out, "OM ");
    if (IS_SET (a, 1<<2)) strbuf_adds(out, "ZM ");
    if (IS_SET (a, 1<<1)) strbuf_adds(out, "DM ");
    if (IS_SET (a, 1<<0)) strbuf_adds(out, "IM ");

    // TODO trim right part
};

void FSW_to_str (uint16_t a, strbuf *out)
{
    if (IS_SET (a, 1<<15)) strbuf_adds(out, "B ");
    if (IS_SET (a, 1<<14)) strbuf_adds(out, "C3 ");
    //L ("TOP=%d ", (ctx->FloatSave.StatusWord>>11)&7);
    if (IS_SET (a, 1<<10)) strbuf_adds(out, "C2 ");
    if (IS_SET (a, 1<<9)) strbuf_adds(out, "C1 ");
    if (IS_SET (a, 1<<8)) strbuf_adds(out, "C0 ");
    if (IS_SET (a, 1<<7)) strbuf_adds(out, "IR ");
    if (IS_SET (a, 1<<6)) strbuf_adds(out, "SF ");
    if (IS_SET (a, 1<<5)) strbuf_adds(out, "P ");
    if (IS_SET (a, 1<<4)) strbuf_adds(out, "U ");
    if (IS_SET (a, 1<<3)) strbuf_adds(out, "O ");
    if (IS_SET (a, 1<<2)) strbuf_adds(out, "Z ");
    if (IS_SET (a, 1<<1)) strbuf_adds(out, "D ");
    if (IS_SET (a, 1<<0)) strbuf_adds(out, "I ");

    // TODO trim right part
};

// old dump_XMM
void XMM_to_strbuf (uint8_t* p, strbuf *sb)
{
    int i;
    BYTE a;

    strbuf_adds(sb, "0x");
    //	double d;
    //	float f;

    for (i=0; i<16; i++)
        strbuf_addf (sb, "%02X", *(p+(15-i)));
    // ASCII
    strbuf_adds(sb, "   \"");
    for (i=0; i<16; i++)
    {
        a=*(p+i); // string is in reverse order
        if (isprint (a))
            strbuf_addf (sb, "%c", a);
        else
            strbuf_addc (sb, '.');
    };

    strbuf_addc (sb, '\"');

    //	memcpy (&d, p, sizeof (d)); // low part of XMM register
    //	memcpy (&f, p, sizeof (f)); // low part of XMM register

    //	if (_isnan(d)==0)
    //		rt+=strfmt ("lowpart=%lf", d);
};
