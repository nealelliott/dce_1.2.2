/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sym.h,v $
 * Revision 1.1.10.1  1996/10/02  17:45:37  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:36:45  damon]
 *
 * Revision 1.1.5.1  1994/06/09  14:06:39  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:21:58  annie]
 * 
 * Revision 1.1.3.2  1993/01/21  19:33:06  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  19:46:42  zeliff]
 * 
 * Revision 1.1  1992/01/19  02:41:09  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * $Header: /u0/rcs_trees/dce/rcs/file/export/RIOS/sym.h,v 1.1.10.1 1996/10/02 17:45:37 damon Exp $
 * $Locker:  $
 *
 * sym	-	symbol table definitions, VRMIX kernel pseudo-TOC crap
 */

struct toc_syment {
	union {
		char		_n_name[8];	/* old COFF version */
		struct {
			long	_n_zeroes;	/* new == 0 */
			long	_n_offset;	/* offset into string table */
		} _n_n;
		char		*_n_nptr[2];	/* allows for overlaying */
	} _n;
	long			n_value;	/* value of symbol */
};
#define n_name		_n._n_name
#define n_nptr		_n._n_nptr[1]
#define n_zeroes	_n._n_n._n_zeroes
#define n_offset	_n._n_n._n_offset

typedef struct toc_syment sym_t;

extern struct toc_syment *toc_syms;	/* symbol table		*/
extern caddr_t toc_strs;		/* string table		*/
extern toc_nsyms;			/* # symbols		*/
extern sym_t *sym_flex();
extern sym_t *sym_lookup();
