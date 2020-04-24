/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: export.c,v $
 * Revision 1.1.16.1  1996/10/02  17:45:30  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:36:41  damon]
 *
 * Revision 1.1.10.2  1994/07/13  22:27:40  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:29:55  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:58:26  mbs]
 * 
 * Revision 1.1.10.1  1994/06/09  14:06:31  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:21:54  annie]
 * 
 * Revision 1.1.6.4  1993/01/21  19:32:43  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  19:46:28  zeliff]
 * 
 * Revision 1.1.6.3  1992/11/24  17:01:16  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  17:56:04  bolinger]
 * 
 * Revision 1.1.6.2  1992/08/31  19:32:18  jaffe
 * 	Replace missing RCS ids
 * 	[1992/08/31  16:25:42  jaffe]
 * 
 * Revision 1.1.4.4  1992/07/03  01:53:41  mason
 * 	Transarc delta: cburnett-ot3963-export_ext_pinned_mem 1.1
 * 	  Selected comments:
 * 	    The export.ext extension should not used pinned memory to hold the
 * 	    kernel symbol data.  Pageable memory will work fine and leave more
 * 	    memory for other uses.  This saves about 64k of pinned memory.
 * 	    Changed export.c to allocate pageable memory.
 * 	[1992/07/02  18:58:46  mason]
 * 
 * Revision 1.1.4.3  1992/07/02  21:51:48  mason
 * 	Transarc delta: cburnett-ot3963-export_ext_pinned_mem 1.1
 * 	  Selected comments:
 * 	    The export.ext extension should not used pinned memory to hold the
 * 	    kernel symbol data.  Pageable memory will work fine and leave more
 * 	    memory for other uses.  This saves about 64k of pinned memory.
 * 	    Changed export.c to allocate pageable memory.
 * 	[1992/07/02  18:58:46  mason]
 * 
 * Revision 1.1.4.2  1992/05/22  20:39:48  garyf
 * 	use macro to call uiomove
 * 	[1992/05/22  20:36:03  garyf]
 * 
 * Revision 1.1  1992/01/19  02:41:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* 
 * $Header: /u0/rcs_trees/dce/rcs/file/export/RIOS/export.c,v 1.1.16.1 1996/10/02 17:45:30 damon Exp $
 * $Locker:  $
 *
 * Copyright (c) 1990, Transarc Corporation.  All rights reserved.
 *
 * export -	EXPORT kernel extension
 */

#include <sys/types.h>
#include <sys/user.h>
#include <sys/conf.h>
#include <sys/errno.h>
#include <sys/device.h>
#include <sys/uio.h>
#include <sys/pri.h>
#include <sys/malloc.h>
#include <sym.h>
#include <export.h>
#include <dcedfs/osi_uio.h>

#undef RELOC

sym_t  *toc_syms;		/* symbol table			*/
int	toc_nsyms;		/* # of symbols			*/
caddr_t toc_strs;		/* string table			*/
int     toc_size;		/* size of toc_syms		*/

mid_t export_kmid;

/*
 * export	-	entry point to EXPORT kernel extension
 *
 * Input:
 *	cmd	-	add/delete command
 *	uiop	-	uio vector describing any config params
 */
export(cmd, uiop)
struct uio *uiop; {
	int	err, monster;
	static once;

	err     = 0;
	monster = lockl(&kernel_lock, LOCK_SHORT);

	switch (cmd) {
	    case CFG_INIT:			/* add EXPORT		*/
		if (err = config(uiop))
			export_cleanup();
		break;

	    case CFG_TERM:			/* remove EXPORT	*/
		if (err = export_cleanup())
			break;
		break;

	    default:
		err = EINVAL;
		break;
	}

	if (monster != LOCK_NEST)
		unlockl(&kernel_lock);

	return err;
}

/*
 * config -	process configuration data
 */
config(uiop)
register struct uio *uiop; {
	struct k_conf conf;
	register struct export_nl *np;
	register sym_t *sym;
	register err;

	if (err = osi_uiomove((caddr_t)&conf, sizeof (conf), UIO_WRITE, uiop))
		return err;

	toc_nsyms = conf.nsyms;
	toc_size  = conf.symt_sz + conf.str_sz;

	if (toc_nsyms * sizeof (sym_t) != conf.symt_sz
	    || toc_size > (1024 * 1024))
		return EINVAL;

	toc_syms = (sym_t *) xmalloc(toc_size, 2, kernel_heap);

	if (!toc_syms)
		return ENOMEM;

	toc_strs = (char *) &toc_syms[toc_nsyms];

	/*
	 * copy in the symbol table and the string table
	 */
	if (err = copyin(conf.symtab, toc_syms, conf.symt_sz)
	    || (err = copyin(conf.strtab, toc_strs, conf.str_sz))) {
		xmfree(toc_syms, kernel_heap);
		toc_syms = 0;
		return err;
	}

	/*
	 * `TOC' format in kernel has offsets relocated to point directly
	 * into the string table.
	 */
	for (sym = toc_syms; sym < &toc_syms[toc_nsyms]; ++sym)
		if (sym->n_zeroes == 0)
			sym->n_nptr = sym->n_offset + toc_strs;

	return 0;
}

/*
 * export_cleanup -	cleanup EXPORT prior to removing kernel extension
 */
export_cleanup() {

	/*
	 * get rid of the symbol table
	 */
	if (toc_syms) {
		xmfree(toc_syms, kernel_heap);
		toc_syms = 0;
		toc_size = 0;
		toc_strs = 0;
	}

	return 0;
}

/*
 * import_kfunc -	import a kernel function that was mistakenly left
 *			off the exports list
 *
 * NOTE:
 *	We are assuming that the functions we are importing from the
 *	kernel really are within the kernel.  If they are actually
 *	exported from some other kernel extension (but referenced in
 *	the /unix symbol table) we are in trouble.
 */
import_kfunc(struct k_func *kfp) {
	register sym_t *sym;
	register caddr_t *toc;
	register i, pri;
	static ulong *g_toc;

	if (!g_toc) {
		sym = sym_lookup("g_toc", 0);
		if (!sym) {
			printf("\nimport: can't ascertain kernel's TOC\n");
			return EINVAL;
		}
		g_toc = (ulong *) sym->n_value;
	}

	sym = sym_lookup(kfp->name, 0);
	if (!sym) {
		printf("\nimport: function `%s' not found\n", kfp->name);
		return EINVAL;
	}

	kfp->fdesc[0] = sym->n_value;
	kfp->fdesc[1] = *g_toc;
	kfp->fdesc[2] = 0;

	*kfp->fpp = (void * ((*)())) kfp->fdesc;

	return 0;
}

/*
 * import_kvar -	import a kernel variable that was mistakenly left
 *			off the exports list
 */
import_kvar(struct k_var *kvp, ulong *toc) {
	register sym_t *sym;
	register i, pri;
	label_t jmpbuf;

	switch (setjmpx(&jmpbuf)) {
	    case 0:
		break;

	    default:
		return EINVAL;
	}

	sym = sym_lookup(kvp->name, 0);
	if (!sym) {
		printf("\nimport: variable `%s' not found\n", kvp->name);
		longjmpx(EINVAL);
	}

	/*
	 * look through the caller's TOC for the reference to his surrogate
	 * variable.
	 */
	while (*toc != (ulong)kvp->varp)
		++toc;

	printf("import(%s): replacing my TOC at 0x%x: 0x%8x with 0x%8x\n"
	       , kvp->name, toc, *toc, sym->n_value);

	/*
	 * replace reference to surrogate with reference real
	 */
	pri = i_disable(INTMAX);
	*toc = (ulong) sym->n_value;
	i_enable(pri);

	clrjmpx(&jmpbuf);

	return 0;
}


/*
 * Call vanilla syscalls
 */
osetgroups(ngroups, gidset)
    int ngroups;
    gid_t *gidset;
{
    int error;

    error = setgroups(ngroups, gidset);
    return (error);
}


okioctl(fdes, cmd, arg, ext)
    int fdes, cmd, arg;
    caddr_t ext;
{
    int error;
    
    error = kioctl(fdes, cmd, arg, ext);
    return (error);
}


olockf(fdes, func, size)
    int fdes, func;
    long size;
{
    int error;

    error = lockf(fdes, func, size);
    return (error);
}
