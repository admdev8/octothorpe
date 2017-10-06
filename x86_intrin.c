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

#include "x86_intrin.h"
#include "x86.h"

#if __WORDSIZE==32

void intrin_SHL (IN tetra value, IN uint8_t shift_value, OUT tetra* result, IN OUT tetra* flags)
{
	tetra tmp;
	
	__asm__("pushfl;"
		"popl %%ebx;"
		"andl $0xfffff72a, %%ebx;" // ~FLAG_PSAZOC
		"orl %%edx, %%ebx;"
		"pushl %%ebx;"
		"popfl;"
		"shll %%cl, %%eax;"
		"pushfl;"
		"popl %%edx;"
				: "=a" (*result), "=d" (tmp)
				: "a" (value), "c" (shift_value), "d" (*flags)
				: "%ebx", "cc"
		);
	*flags=(tmp & FLAG_PSAZOC);
};

void intrin_SHR (IN tetra value, IN uint8_t shift_value, OUT tetra* result, IN OUT tetra* flags)
{
	tetra tmp;
	
	__asm__("pushfl;"
		"popl %%ebx;"
		"andl $0xfffff72a, %%ebx;" // ~FLAG_PSAZOC
		"orl %%edx, %%ebx;"
		"pushl %%ebx;"
		"popfl;"
		"shrl %%cl, %%eax;"
		"pushfl;"
		"popl %%edx;"
				: "=a" (*result), "=d" (tmp)
				: "a" (value), "c" (shift_value), "d" (*flags)
				: "%ebx", "cc"
		);
	*flags=(tmp & FLAG_PSAZOC);
};

void intrin_SAR (IN tetra value, IN uint8_t shift_value, OUT tetra* result, IN OUT tetra* flags)
{
	tetra tmp;
	
	__asm__("pushfl;"
		"popl %%ebx;"
		"andl $0xfffff72a, %%ebx;" // ~FLAG_PSAZOC
		"orl %%edx, %%ebx;"
		"pushl %%ebx;"
		"popfl;"
		"sarl %%cl, %%eax;"
		"pushfl;"
		"popl %%edx;"
				: "=a" (*result), "=d" (tmp)
				: "a" (value), "c" (shift_value), "d" (*flags)
				: "%ebx", "cc"
		);
	*flags=(tmp & FLAG_PSAZOC);
};

void intrin_ADD (IN tetra op1, IN tetra op2, OUT tetra* result, IN OUT tetra* flags)
{
	tetra tmp;
	
	__asm__("pushfl;"
		"popl %%ebx;"
		"andl $0xfffff72a, %%ebx;" // ~FLAG_PSAZOC
		"orl %%edx, %%ebx;"
		"pushl %%ebx;"
		"popfl;"
		"addl %%ecx, %%eax;"
		"pushfl;"
		"popl %%edx;"
				: "=a" (*result), "=d" (tmp)
				: "a" (op1), "c" (op2), "d" (*flags)
				: "%ebx", "cc"
		);
	*flags=(tmp & FLAG_PSAZOC);
};

void intrin_ADC (IN tetra op1, IN tetra op2, OUT tetra* result, IN OUT tetra* flags)
{
	tetra tmp;
	
	__asm__("pushfl;"
		"popl %%ebx;"
		"andl $0xfffff72a, %%ebx;" // ~FLAG_PSAZOC
		"orl %%edx, %%ebx;"
		"pushl %%ebx;"
		"popfl;"
		"adcl %%ecx, %%eax;"
		"pushfl;"
		"popl %%edx;"
				: "=a" (*result), "=d" (tmp)
				: "a" (op1), "c" (op2), "d" (*flags)
				: "%ebx", "cc"
		);
	*flags=(tmp & FLAG_PSAZOC);
};

void intrin_SUB (IN tetra op1, IN tetra op2, OUT tetra* result, IN OUT tetra* flags)
{
	tetra tmp;
	
	__asm__("pushfl;"
		"popl %%ebx;"
		"andl $0xfffff72a, %%ebx;" // ~FLAG_PSAZOC
		"orl %%edx, %%ebx;"
		"pushl %%ebx;"
		"popfl;"
		"subl %%ecx, %%eax;"
		"pushfl;"
		"popl %%edx;"
				: "=a" (*result), "=d" (tmp)
				: "a" (op1), "c" (op2), "d" (*flags)
				: "%ebx", "cc"
		);
	*flags=(tmp & FLAG_PSAZOC);
};

void intrin_SBB (IN tetra op1, IN tetra op2, OUT tetra* result, IN OUT tetra* flags)
{
	tetra tmp;
	
	__asm__("pushfl;"
		"popl %%ebx;"
		"andl $0xfffff72a, %%ebx;" // ~FLAG_PSAZOC
		"orl %%edx, %%ebx;"
		"pushl %%ebx;"
		"popfl;"
		"sbbl %%ecx, %%eax;"
		"pushfl;"
		"popl %%edx;"
				: "=a" (*result), "=d" (tmp)
				: "a" (op1), "c" (op2), "d" (*flags)
				: "%ebx", "cc"
		);
	*flags=(tmp & FLAG_PSAZOC);
};

void intrin_XOR (IN tetra op1, IN tetra op2, OUT tetra* result, IN OUT tetra* flags)
{
	tetra tmp;
	
	__asm__("pushfl;"
		"popl %%ebx;"
		"andl $0xfffff72a, %%ebx;" // ~FLAG_PSAZOC
		"orl %%edx, %%ebx;"
		"pushl %%ebx;"
		"popfl;"
		"xorl %%ecx, %%eax;"
		"pushfl;"
		"popl %%edx;"
				: "=a" (*result), "=d" (tmp)
				: "a" (op1), "c" (op2), "d" (*flags)
				: "%ebx", "cc"
		);
	*flags=(tmp & FLAG_PSAZOC);
};

void intrin_XOR_addr (IN tetra *address_of_op1, IN tetra op2, OUT tetra* result, IN OUT tetra* flags)
{
	tetra tmp;
	
	__asm__("pushfl;"
		"popl %%esi;"
		"andl $0xfffff72a, %%esi;" // ~FLAG_PSAZOC
		"orl %%edx, %%esi;"
		"pushl %%esi;"
		"popfl;"
		"xorl %%ecx, (%%eax);"
		"pushfl;"
		"popl %%edx;"
				: "=a" (*result), "=d" (tmp)
				: "a" (address_of_op1), "c" (op2), "d" (*flags)
				: "%esi", "cc"
		);
	*flags=(tmp & FLAG_PSAZOC);
};

void intrin_OR (IN tetra op1, IN tetra op2, OUT tetra* result, IN OUT tetra* flags)
{
	tetra tmp;
	
	__asm__("pushfl;"
		"popl %%ebx;"
		"andl $0xfffff72a, %%ebx;" // ~FLAG_PSAZOC
		"orl %%edx, %%ebx;"
		"pushl %%ebx;"
		"popfl;"
		"orl %%ecx, %%eax;"
		"pushfl;"
		"popl %%edx;"
				: "=a" (*result), "=d" (tmp)
				: "a" (op1), "c" (op2), "d" (*flags)
				: "%ebx", "cc"
		);
	*flags=(tmp & FLAG_PSAZOC);
};

void intrin_AND (IN tetra op1, IN tetra op2, OUT tetra* result, IN OUT tetra* flags)
{
	tetra tmp;
	
	__asm__("pushfl;"
		"popl %%ebx;"
		"andl $0xfffff72a, %%ebx;" // ~FLAG_PSAZOC
		"orl %%edx, %%ebx;"
		"pushl %%ebx;"
		"popfl;"
		"andl %%ecx, %%eax;"
		"pushfl;"
		"popl %%edx;"
				: "=a" (*result), "=d" (tmp)
				: "a" (op1), "c" (op2), "d" (*flags)
				: "%ebx", "cc"
		);
	*flags=(tmp & FLAG_PSAZOC);
};

void intrin_NOT (IN tetra op1, OUT tetra* result, IN OUT tetra* flags)
{
	tetra tmp;
	
	__asm__("pushfl;"
		"popl %%ebx;"
		"andl $0xfffff72a, %%ebx;" // ~FLAG_PSAZOC
		"orl %%edx, %%ebx;"
		"pushl %%ebx;"
		"popfl;"
		"notl %%eax;"
		"pushfl;"
		"popl %%edx;"
				: "=a" (*result), "=d" (tmp)
				: "a" (op1), "d" (*flags)
				: "%ebx", "cc"
		);
	*flags=(tmp & FLAG_PSAZOC);
};

void intrin_NEG (IN tetra op1, OUT tetra* result, IN OUT tetra* flags)
{
	tetra tmp;
	
	__asm__("pushfl;"
		"popl %%ebx;"
		"andl $0xfffff72a, %%ebx;" // ~FLAG_PSAZOC
		"orl %%edx, %%ebx;"
		"pushl %%ebx;"
		"popfl;"
		"negl %%eax;"
		"pushfl;"
		"popl %%edx;"
				: "=a" (*result), "=d" (tmp)
				: "a" (op1), "d" (*flags)
				: "%ebx", "cc"
		);
	*flags=(tmp & FLAG_PSAZOC);
};

#endif

byte rotr8(byte x, byte r)
{
	__asm("rorb %1,%0" : "+r" (x) : "c" (r));
	return x;
}

byte rotl8(byte x, byte r)
{
	__asm("rolb %1,%0" : "+r" (x) : "c" (r));
	return x;
}

wyde rotr16(wyde x, byte r)
{
	__asm("rorw %1,%0" : "+r" (x) : "c" (r));
	return x;
}

wyde rotl16(wyde x, byte r)
{
	__asm("rolw %1,%0" : "+r" (x) : "c" (r));
	return x;
}

tetra rotr32(tetra x, byte r)
{
	__asm("rorl %1,%0" : "+r" (x) : "c" (r));
	return x;
}

tetra rotl32(tetra x, byte r)
{
	__asm("roll %1,%0" : "+r" (x) : "c" (r));
	return x;
}

#if __WORDSIZE==64

octa rotr64(octa x, byte r)
{
	__asm("rorq %1,%0" : "+r" (x) : "c" (r));
	return x;
}

octa rotl64(octa x, byte r)
{
	__asm("rolq %1,%0" : "+r" (x) : "c" (r));
	return x;
}

#elif __WORDSIZE==32

octa rotr64 ( octa x, byte r )
{
	return (x >> r) | (x << (64 - r));
}

octa rotl64 ( octa x, byte r )
{
	return (x << r) | (x >> (64 - r));
}

#else

#error "__WORDSIZE is undefined"

#endif

