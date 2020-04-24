/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: symtab.c,v $
 * Revision 1.1.13.1  1996/10/02  17:45:39  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:36:46  damon]
 *
 * Revision 1.1.7.2  1994/07/13  22:27:40  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:29:56  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:58:27  mbs]
 * 
 * Revision 1.1.7.1  1994/06/09  14:06:42  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:21:58  annie]
 * 
 * Revision 1.1.3.2  1993/01/21  19:33:12  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  19:46:46  zeliff]
 * 
 * Revision 1.1  1992/01/19  02:41:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * $Header: /u0/rcs_trees/dce/rcs/file/export/RIOS/symtab.c,v 1.1.13.1 1996/10/02 17:45:39 damon Exp $
 * $Locker:  $
 *
 * symtab -	symbol table routines
 *
 * Copyright (c) 1986, 1987, Damon Permezel
 * Copyright (c) 1988, IBM
 */
#include <sys/types.h>
#include <sym.h>

/*
 * using the toc_syment structure, that we fabricate:
 *	sym->n_offset is the string pointer
 */
#define	sym_off(sym)	((sym)->n_offset)
#define	sym_str(sym)	\
	((sym)->n_zeroes == 0 ? (char *) sym_off(sym) : (sym)->n_name)

sym_t *
sym_lookup(name, value)
char *name; {
	static sym_t *symsrch(), *search();
	char buf[64];
	register sym_t *sym;

	if (name) {
		/*
		 * Heuristic:
		 * first, try just the name. if that fails, try with a
		 * prefix '.', and failing that, a prefix '_'.
		 */
		if (sym = symsrch(name))
			return sym;
		bcopy(name, buf+1, sizeof (buf) - 2);
		buf[0] = '.';

		if (sym = symsrch(buf))
			return sym;

		buf[0] = '_';

		return symsrch(buf);
	} else
		return search(value);
}

static sym_t *
search(addr)
unsigned addr; {
	register sym_t *sp;
	register sym_t *save;
	unsigned value;

	value = 0;
	save  = 0;

	for (sp = toc_syms; sp < &toc_syms[toc_nsyms]; ++sp) {
		if (sp->n_value <= addr && sp->n_value >= value) {
			value = sp->n_value;
			save  = sp;
			if (sp->n_value == addr)
				break;
		}
	}
	return save ? sym_flex(save) : 0;
}

static sym_t *
symsrch(s)
register char *s; {
	register sym_t *sp;
	register sym_t *found;
	register len;
	register char *p;

	/*
	 * determine length of symbol
	 */
	for (len = 0, p = s; *p; ++p)
		++len;

	found = 0;
	for (sp = toc_syms; sp < &toc_syms[toc_nsyms]; ++sp) {
		/*
		 * exact matches preferred
		 */
		if (strcmp(sym_str(sp), s) == 0) {
			found = sp;
			break;
		}
		/*
		 * otherwise, prefices might interest us.
		 */
		if (!found && (strncmp(sym_str(sp), s, len) == 0)) {
			found = sp;
			continue;
		}
	}

	return found ? sym_flex(found) : 0;
}

/*
 * sym_flex -	convert a symbol so that there is no distinction between
 *		flex-string and non flex-string format.
 *
 * Input:
 *	sym	-	^ to symbol table entry
 *
 * Returns:
 *	^ to static location containing modified symbol.
 */
sym_t *
sym_flex(sym)
register sym_t *sym; {
	static sym_t symbol;
	static char name[48];

	strncpy(name, sym_str(sym), sizeof (name) - 1);

	if (sym->n_zeroes != 0)
		name[8] = 0;	/* make sure that we truncate correctly	*/

	symbol          = *sym;
	symbol.n_zeroes = 0;
	symbol.n_nptr   = name;

	return &symbol;
}
