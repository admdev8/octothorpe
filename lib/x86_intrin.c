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

#ifdef O_BITS32

void intrin_SHL (IN tetrabyte value, IN uint8_t shift_value, OUT tetrabyte* result, IN OUT tetrabyte* flags)
{
	tetrabyte tmp;
	
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

void intrin_SHR (IN tetrabyte value, IN uint8_t shift_value, OUT tetrabyte* result, IN OUT tetrabyte* flags)
{
	tetrabyte tmp;
	
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

void intrin_SAR (IN tetrabyte value, IN uint8_t shift_value, OUT tetrabyte* result, IN OUT tetrabyte* flags)
{
	tetrabyte tmp;
	
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

void intrin_ADD (IN tetrabyte op1, IN tetrabyte op2, OUT tetrabyte* result, IN OUT tetrabyte* flags)
{
	tetrabyte tmp;
	
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

void intrin_ADC (IN tetrabyte op1, IN tetrabyte op2, OUT tetrabyte* result, IN OUT tetrabyte* flags)
{
	tetrabyte tmp;
	
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

void intrin_SUB (IN tetrabyte op1, IN tetrabyte op2, OUT tetrabyte* result, IN OUT tetrabyte* flags)
{
	tetrabyte tmp;
	
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

void intrin_SBB (IN tetrabyte op1, IN tetrabyte op2, OUT tetrabyte* result, IN OUT tetrabyte* flags)
{
	tetrabyte tmp;
	
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

void intrin_XOR (IN tetrabyte op1, IN tetrabyte op2, OUT tetrabyte* result, IN OUT tetrabyte* flags)
{
	tetrabyte tmp;
	
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

void intrin_XOR_addr (IN tetrabyte *address_of_op1, IN tetrabyte op2, OUT tetrabyte* result, IN OUT tetrabyte* flags)
{
	tetrabyte tmp;
	
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

void intrin_OR (IN tetrabyte op1, IN tetrabyte op2, OUT tetrabyte* result, IN OUT tetrabyte* flags)
{
	tetrabyte tmp;
	
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

void intrin_AND (IN tetrabyte op1, IN tetrabyte op2, OUT tetrabyte* result, IN OUT tetrabyte* flags)
{
	tetrabyte tmp;
	
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

void intrin_NOT (IN tetrabyte op1, OUT tetrabyte* result, IN OUT tetrabyte* flags)
{
	tetrabyte tmp;
	
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

void intrin_NEG (IN tetrabyte op1, OUT tetrabyte* result, IN OUT tetrabyte* flags)
{
	tetrabyte tmp;
	
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

tetrabyte rotr32(tetrabyte x, byte r)
{
	__asm("rorl %1,%0" : "+r" (x) : "c" (r));
	return x;
}

tetrabyte rotl32(tetrabyte x, byte r)
{
	__asm("roll %1,%0" : "+r" (x) : "c" (r));
	return x;
}

#endif // O_BITS32

#ifdef O_BITS64

octabyte rotr64(octabyte x, byte r)
{
	__asm("rorq %1,%0" : "+r" (x) : "c" (r));
	return x;
}

octabyte rotl64(octabyte x, byte r)
{
	__asm("rolq %1,%0" : "+r" (x) : "c" (r));
	return x;
}

#endif // O_BITS64

