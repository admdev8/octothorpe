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
 * Written by Dennis Yurichev <dennis(a)yurichev.com>, 2013-2017
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/3.0/.
 *
 */

#include "oassert.h"

#include "x86_intrin.h"
#include "base64.h"
#include "octomath.h"
#include "files.h"
#include "datatypes.h"
#include "entropy.h"
#include "memutils.h"
#include "dmalloc.h"
#include "dlist.h"
#include "logging.h"
#include "lisp.h"

void x86_intrin_tests()
{
	oassert(rotr32(1,1)==0x80000000);
	oassert(rotl32(0x80000000,1)==1);
};

void base64_tests()
{
	oassert (likely_base64_string("0123456")==0);
	oassert (likely_base64_string("aaaaaaa")==0);
	oassert (likely_base64_string("aaAA11")==1);
	oassert (likely_base64_string("aaaaAAAA1111")==1);
	oassert (likely_base64_string("aaaaaAAAAA11")==1);
};

void octomath_tests()
{
	oassert (octa_log2(1)==0);
	oassert (octa_log2(2)==1);
	oassert (octa_log2(4)==2);
	oassert (octa_log2(8)==3);
	oassert (octa_log2(65536)==16);
	oassert (octa_log2(0xFFFFFFFFFFFFFFFF)==63);

	oassert(is_prime(131071)==1); // https://oeis.org/A000043
	oassert(is_prime(131071+1)==0);

	// modinv32/64
	bool b;
	tetra tmp1, tmp2;
	b=modinv32(9, &tmp1, &tmp2);
	oassert (b==true);
	oassert (tmp1==954437177);
	oassert (tmp2==2);
	b=modinv32(tmp1, &tmp1, &tmp2);
	oassert (b==true);
	oassert (tmp1==9);
	oassert (tmp2==2);
	b=modinv32(10, &tmp1, &tmp2);
	oassert (b==false); // no solutions

	octa t1, t2;
	b=modinv64(123, &t1, &t2);
	oassert (b==true);
	oassert (t1==3449391168254631603);
	oassert (t2==23);
	b=modinv64(t1, &t1, &t2);
	oassert (b==true);
	oassert (t1==123);
	oassert (t2==23);
	b=modinv64(999999, &t1, &t2);
	oassert (b==true);
	oassert (t1==1672254362768354751);
	oassert (t2==90653);
};

void files_tests()
{
	char t1[128];
	char t2[128];

	split_fname("filename.ext", t1, sizeof(t1), t2, sizeof(t2));
	oassert(strcmp(t1, "filename")==0);
	oassert(strcmp(t2, "ext")==0);
};

void entropy_tests()
{
	byte buf[256];
	bzero(buf, sizeof(buf));
	oassert(entropy (buf, sizeof(buf))==0);
	bytefill(buf, sizeof(buf), 0x11);
	oassert(entropy (buf, sizeof(buf))==0);
	for (int i=0; i<256; i++)
		buf[i]=i;
	oassert(entropy (buf, sizeof(buf))==8);

	// integer entropy
	bzero(buf, sizeof(buf));
	oassert(entropy_int (buf, sizeof(buf))==0);
	bytefill(buf, sizeof(buf), 0x11);
	oassert(entropy_int (buf, sizeof(buf))==0);
	for (int i=0; i<256; i++)
		buf[i]=i;
	oassert(entropy_int (buf, sizeof(buf))>>16==8);
}

void dlist_tests()
{
	dlist *l=dlist_init();

	dlist_insert_at_begin (&l, DSTRDUP("1st", "string"));
	dlist_insert_at_begin (&l, DSTRDUP("2nd", "string"));
	dlist_insert_at_begin (&l, DSTRDUP("3rd", "string"));

	//printf ("first dump:\n");
	//dlist_dump (l);

	oassert(strcmp((char*)dlist_get_first(l), "3rd")==0);

	dlist *i=l;
	oassert(strcmp((char*)i->data, "3rd")==0);
	i=i->next;
	oassert(strcmp((char*)i->data, "2nd")==0);
	i=i->next;
	oassert(strcmp((char*)i->data, "1st")==0);
	i=i->next;

	// remove first
	char *tmp=l->data;
	dlist_unlink (&l, l);
	DFREE(tmp);
	//printf ("dump after unlink:\n");
	//dlist_dump (l);
	dlist_check_consistency (l);

	i=l;
	oassert(strcmp((char*)i->data, "2nd")==0);
	i=i->next;
	oassert(strcmp((char*)i->data, "1st")==0);
	i=i->next;

	dlist_free(l, dfree);
};

void dmalloc_tests()
{
	byte *t1, *t2, *t3, *t4, *t5, *t6, *t7;
	char *s;
	//L_init("tmp");

	s=DSTRDUP ("hahaha", "s");

	t1=DMALLOC(byte, 6, "block123");
	t2=DMALLOC(byte, 124, "block124");
	memcpy (t2, "block124", 8);
	t3=DMALLOC(byte, 12,  "block12");
	memcpy (t3, "block12", 7);
	t4=DMALLOC(byte, 555, "block555");
	memcpy (t4, "block555", 8);
	t5=DMALLOC(byte, 666, "block666");
	t6=DMALLOC(byte, 777, "block777");
	t7=DMALLOC(byte, 0, "empty block");

	t6=DREALLOC(t6, byte, 888, "new size for block777");

	// guards are checking?
	//memcpy (t1, "12345678", 8);
	//memcpy((byte*)t1-1, "a", 1);

	t1=DREALLOC(t1, byte, 100, "new size for block123");

	DFREE(t1);
	DFREE(t5);
	//L_print_buf (t5, 666);
	DFREE(t6);
	DFREE(t7);
	DFREE(s);

	DFREE(t2);
	DFREE(t3);
	DFREE(t4);

	// make compiler happy about unused variables!
	t1=t4; t1=t3; t1=t2;
}

void lisp_tests()
{
	obj *o, *i;

	o=obj_octa(0x0123456789ABCDEF);
	strbuf s1=STRBUF_INIT;
	obj_to_strbuf(&s1, o);
	oassert(strcmp(s1.buf,"0x123456789abcdef")==0);
	obj_free(o);
	strbuf_deinit(&s1);

	o=create_list(obj_tetra(1), obj_tetra(2), obj_tetra(3), obj_tetra(4), NULL);
	strbuf s2=STRBUF_INIT;
	obj_to_strbuf(&s2, o);
	oassert (strcmp (s2.buf, "(0x1 0x2 0x3 0x4)")==0);
	oassert(LENGTH(o)==4);
	obj_free(o);
	strbuf_deinit(&s2);

	o=create_list(
			cons(obj_tetra(1), obj_cstring("hello1")),
			cons(obj_tetra(2), obj_cstring("hello2")),
			cons(obj_tetra(3), obj_cstring("hello3")),
			NULL);

	strbuf s3=STRBUF_INIT;
	obj_to_strbuf(&s3, o);
	oassert(strcmp(s3.buf, "((0x1 \"hello1\") (0x2 \"hello2\") (0x3 \"hello3\"))")==0);
	strbuf_deinit(&s3);

	// enum objects in list
	i=o;
	oassert(obj_get_as_tetra(car(car(i)))==1);
	oassert(strcmp(obj_get_as_cstring(cdr(car(i))),"hello1")==0);
	i=cdr(i);
	oassert(obj_get_as_tetra(car(car(i)))==2);
	oassert(strcmp(obj_get_as_cstring(cdr(car(i))),"hello2")==0);
	i=cdr(i);
	oassert(obj_get_as_tetra(car(car(i)))==3);
	oassert(strcmp(obj_get_as_cstring(cdr(car(i))),"hello3")==0);
	i=cdr(i);

	obj_free(o);

	// test add_to_list()
	o=NULL;
	o=add_to_list(o, obj_tetra(101));
	o=add_to_list(o, obj_tetra(102));
	o=add_to_list(o, obj_tetra(103));
	o=add_to_list(o, obj_tetra(104));
	strbuf s4=STRBUF_INIT;
	obj_to_strbuf(&s4, o);
	oassert(strcmp(s4.buf, "(0x65 0x66 0x67 0x68)")==0);
	strbuf_deinit(&s4);
	obj_free(o);
};


int main()
{
	x86_intrin_tests();
	base64_tests();
	octomath_tests();
	files_tests();
	entropy_tests();
	dlist_tests();
	dmalloc_tests();
	lisp_tests();

	dump_unfreed_blocks();
};

