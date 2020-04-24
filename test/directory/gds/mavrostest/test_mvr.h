/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: test_mvr.h,v $
 * Revision 1.1.10.2  1996/02/18  23:03:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:32  marty]
 *
 * Revision 1.1.10.1  1995/12/08  22:12:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:00  root]
 * 
 * Revision 1.1.8.1  1994/02/22  17:45:27  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  11:55:56  marrek]
 * 
 * Revision 1.1.6.2  1993/08/12  11:32:14  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:15:07  marrek]
 * 
 * Revision 1.1.4.2  1993/01/04  17:02:57  htf
 * 	Embedded copyright notice
 * 	[1993/01/04  16:15:53  htf]
 * 
 * Revision 1.1.2.2  1992/06/11  19:52:30  melman
 * 	 gds drop
 * 	[1992/06/11  19:18:30  melman]
 * 
 * $EndLog$
 */
#ifndef _TEST_MVR_H
#define _TEST_MVR_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char test_mvr_rcsid[] = "@(#)$RCSfile: test_mvr.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:03:54 $";
#endif

typedef struct _field {
	int l;
	asn1 v;} field;

typedef struct _seq1 {
	char b;
	int i;
	field o; } seq1;

typedef union {
	int i;
	char * s;
	field any;} choice1_opt;

typedef struct _choice1 {
	int x;
	choice1_opt o;} choice1;

typedef struct _set1 {
	int i1;
	int nc;
	choice1 * c1, c2, c3;} set1;

typedef struct _seq2 {
	seq1 a;
	set1 b;
	set1 c;} seq2;

typedef struct _seq3 {
	field oid;
	field any; } seq3;

typedef struct _set4 {
	seq3 a;
	set1 b;
} set4;

typedef struct _entry {
	int i;
	union {
	seq2 s2;
	set4 s4;
	field b;
	int f;
	double r;
	choice1 va;} o;} entry;


typedef entry * table;

typedef unsigned char UC;

/******************************************************\
* 						       *
* Definition of the procedures for ``any defined by''  *
* 						       *
\******************************************************/
struct asn1_proctable {
	asn1_decfunc	dec;
	asn1_errfunc	err;
	asn1_freefunc	free;
	asn1_codfunc	cod;
	asn1_lenfunc	len;
	asn1_lwdfunc	lwd;
	asn1_lwcfunc	lwc;
	asn1_infunc	in;
	asn1_outfunc	out;
	asn1_olenfunc	olen;
	asn1_cpyfunc	cpy;};

asn1_decfunc	variable_article_dec();
asn1_errfunc	variable_article_err();
asn1_freefunc	variable_article_free();
asn1_codfunc	variable_article_cod();
asn1_lenfunc	variable_article_len();
asn1_lwdfunc	variable_article_lwd();
asn1_lwcfunc	variable_article_lwc();
asn1_infunc	variable_article_in();
asn1_outfunc	variable_article_out();
asn1_olenfunc	variable_article_olen();
asn1_cpyfunc	variable_article_cpy();

#endif /* _TEST_MVR_H */
