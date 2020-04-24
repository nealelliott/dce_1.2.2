/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cfgexport.c,v $
 * Revision 1.1.74.1  1996/10/02  17:45:29  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:36:40  damon]
 *
 * Revision 1.1.66.3  1994/07/13  22:27:38  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:29:54  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:58:25  mbs]
 * 
 * Revision 1.1.66.2  1994/06/09  14:06:29  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:21:53  annie]
 * 
 * Revision 1.1.66.1  1994/02/04  20:17:36  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:12:31  devsrc]
 * 
 * Revision 1.1.64.1  1993/12/07  17:23:20  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:13:40  jaffe]
 * 
 * Revision 1.1.5.2  1993/01/21  19:32:40  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  19:46:24  zeliff]
 * 
 * Revision 1.1.3.2  1992/04/21  13:03:05  mason
 * 	Transarc delta: cburnett-ot2545-fix_cfgexport.c_malloc_bug 1.1
 * 	  Files modified:
 * 	    export/RIOS: cfgexport.c
 * 	  Selected comments:
 * 	    [ there are accounting problems regarding the number of bytes
 * 	      malloced by realloc in cfgexport.c]
 * 	    [ fixed accounting problems ]
 * 	[1992/04/20  22:10:06  mason]
 * 
 * Revision 1.1  1992/01/19  02:41:07  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * $Header: /u0/rcs_trees/dce/rcs/file/export/RIOS/cfgexport.c,v 1.1.74.1 1996/10/02 17:45:29 damon Exp $
 * $Locker:  $
 *
 * Copyright (c) 1990, Transarc Corporation.  All rights reserved.
 *
 * cfgexport -	load/configure the EXPORT kernel extension
 */
#include <dcedfs/osi.h>
#include <dcedfs/osi_device.h>
#include <fcntl.h>
#include <sys/device.h>
#include <sys/sysconfig.h>
#include <sys/uio.h>
#include <sys/xcoff.h>
#include <sys/ldr.h>
#include <setjmp.h>

#include <export.h>
#include <sym.h>

extern int	sysconfig(int cmd, void *arg, int len);

int debug;
char *syms = "/unix";

main(argc, argv)
char **argv; {
	register add, opts;
	register c;
	char *file;
	mid_t kmid;
	struct cfg_load cload;
	struct cfg_kmod cmod;
	struct k_conf conf;

	osi_setlocale(LC_ALL, "");
	add = 0;

	while ((c = getopt(argc, argv, "a:s:Z")) != EOF) {
		switch (c) {
		    case 'Z':		/* Zdebug option	*/
			++debug;
			break;

		    case 'a':
			add  = 1;
			file = optarg;
			if (!file)
				usage();
			break;

		    case 's':
			syms = optarg;
			break;

		    default:
			usage();
			break;
		}
	}

	if (!add)
		usage();

	if (add) {
	    char *buf[1024];

	    /* Determine if extension is already loaded */
	    if (dfs_check_load("export_kmid"))
		exit(0);	/* already loaded */
	    
	    buf[0] = "execerror";
	    buf[1] = "cfgexport";
	    get_syms(&conf, syms);

	    cload.path = file;
	    if (sysconfig(SYS_KLOAD, &cload, sizeof(cload)) == -1) {
		loadquery(L_GETMESSAGES, &buf[2], sizeof buf - 8);
		execvp("/etc/execerror", buf);
		exit(1);
	    }
	    cmod.kmid   = cload.kmid;
	    cmod.cmd    = CFG_INIT;
	    cmod.mdiptr = (caddr_t) &conf;
	    cmod.mdilen = sizeof (conf);

	    if (sysconfig(SYS_CFGKMOD, &cmod, sizeof(cmod)) == -1) {
		perror("SYS_CFGKMOD");
		cload.kmid = cload.kmid;
		sysconfig(SYS_KULOAD, &cload, sizeof(cload));
		exit(1);
	    }
	    exit(0);
	}
}

usage() {

	error("usage: cfgexport [-a mod_file -s symbols]\n");
}

int xsym_compar();

/*
 * get_syms -	get kernel symbol table info.
 *
 * Input:
 *	conf	-	^ to EXPORT extension configuration struct
 *	syms	-	^ to name of file containing XCOFF symbols
 */
get_syms(conf, syms)
struct k_conf *conf; 
char *syms;
{
	register sym_t *k_symtab, *ksp;
	register struct syment *x_symtab, *xsp, *xsq;
	register char *xstrings;
	char *kstrings;
	struct xcoffhdr hdr;		/* XCOFF header from symbol file*/
	sym_t k_sym;			/* export version of symbol	*/
	struct syment xcoff_sym;	/* xcoff version of symbol	*/
	register i, nsyms, nksyms, nxsyms;
	long xstr_size, kstr_size;
	FILE *fp;

	fp = fopen(syms, "r");
	if (fp == NULL)
		sys_error(syms);

	if (fread(&hdr, sizeof (hdr), 1, fp) != 1)
		sys_error(syms);

	if (hdr.filehdr.f_nsyms == 0)
		error("%s: no symbols", syms);

	switch (hdr.filehdr.f_magic) {
	    case U802WRMAGIC:
	    case U802ROMAGIC:
	    case U802TOCMAGIC:
	    case U800WRMAGIC:
	    case U800ROMAGIC:
	    case U800TOCMAGIC:
		break;

	    default:
		error("%s: funny magic number 0%o"
		      , syms, hdr.filehdr.f_magic);
	}

	nsyms = hdr.filehdr.f_nsyms;
	if (debug)
		printf("nsyms = %d\n", nsyms);

	x_symtab = (struct syment *) malloc(nsyms * SYMESZ);
	if (!x_symtab)
		error("no memory for symbol table");

	/*
	 * try to snarf the string table: should be just past the
	 * symbol table: first 4 bytes is length of rest.
	 */
	if (fseek(fp, hdr.filehdr.f_symptr + nsyms * SYMESZ, 0) < 0)
		sys_error("%s: seek to strtab", syms);

	if (fread(&xstr_size, sizeof (xstr_size), 1, fp) != 1)
		error("%s: reading string table size", syms);

	xstrings = malloc(xstr_size + sizeof (xstr_size));
	if (!xstrings)
		error("no memory for string table");

	/*
	 * seek back to the start of the strings
	 */
	if (fseek(fp, hdr.filehdr.f_symptr + nsyms * SYMESZ, 0) < 0)
		sys_error("%s: seek to strtab", syms);

	if (fread(xstrings, sizeof (*xstrings), xstr_size, fp) != xstr_size)
		error("%s: reading string table");

	/*
	 * now seek back to the start of the symbol table, and read it
	 * all in.
	 */
	if (fseek(fp, hdr.filehdr.f_symptr, 0) < 0)
		sys_error("%s: seek to symtab", syms);

	xsp = &x_symtab[0];

	for (i = nxsyms = 0; i < nsyms; ++i) {
		char name[16], *p;

		if (fread(&xcoff_sym, SYMESZ, 1, fp) != 1)
			error("%s: reading symbol entry", syms);

		if (xcoff_sym.n_zeroes == 0) {
			/*
			 * Need to relocate string table offset
			 */
			p = xcoff_sym.n_nptr = xstrings + xcoff_sym.n_offset;
		} else {
			strncpy(name, xcoff_sym.n_name, 8);
			
			p = name, p[8] = 0;
		}

		if (debug > 2)
			dump_xsym(&xcoff_sym);

		switch (xcoff_sym.n_sclass) {
		    case C_EXT:		/* external			*/
		    case C_HIDEXT:	/* hidden external (sic)	*/
			/*
			 * filtre out the ones with the strange names
			 */
			if (strchr(p, '@') || strchr(p, '$') || p[0] == 0)
				break;

			*xsp++ = xcoff_sym;
			++nxsyms;

			if (debug > 1)
				dump_xsym(&xcoff_sym);

			break;
		}

		if (xcoff_sym.n_numaux) {
			fseek(fp, xcoff_sym.n_numaux * AUXESZ, 1);
			i += xcoff_sym.n_numaux;
		}
	}

	fclose(fp);

	/*
	 * sort the symbol table
	 */
	qsort((char *) x_symtab, nxsyms, sizeof (*x_symtab), xsym_compar);
 
	/*
	 * we will need no more than `nxsyms' symbols.
	 */
	k_symtab = (sym_t *) malloc(nxsyms * sizeof (sym_t));
	if (!k_symtab)
		error("no memory for EXPORT symbol table");

	/*
	 * uniquify it, and xlate to funny EXPORT format
	 */
	xsp = xsq = x_symtab;
	ksp       = k_symtab;
	kstrings  = 0;
	kstr_size = 0;
	nksyms    = 0;

	bzero(xsq = &xcoff_sym, sizeof (*xsq));

	for (i = 1; i < nxsyms; ++i, xsq = xsp++) {
		if (xsp->n_zeroes != xsq->n_zeroes
		    || xsp->n_offset != xsq->n_offset
		    || xsp->n_value  != xsq->n_value) {
			xlate_xtok(xsp, ksp++, &kstrings, &kstr_size);
			++nksyms;
		}
	}

	/*
	 * place the symbol table info into the `conf' data structure
	 *
	 * XXXXX: for today only, leave the string table the same.
	 */
	conf->nsyms   = nksyms;
	conf->symt_sz = nksyms * sizeof (sym_t);
	conf->str_sz  = kstr_size;
	conf->symtab  = (caddr_t) k_symtab;
	conf->strtab  = kstrings;
}

/*
 * xlate_xtok	-	xlate XCOFF to EXPORT format
 *
 * Input:
 *	xp	-	^ to XCOFF symbol
 *	kp	-	^ to EXPORT  symbol save area
 *	strp	-	^ to ^ to EXPORT string table
 *	szp	-	^ to EXPORT string table size
 */
xlate_xtok(xp, kp, strp, szp)
register struct syment *xp;
register sym_t *kp;
char **strp;
ulong *szp; {
	register len;
	static char *export_strings, *prev = "";
	static left, offset, sz;

	if (!export_strings) {
		export_strings = malloc(sz=1024);
		if (!export_strings)
			error("no memory for EXPORT string table");

		*strp = export_strings;
		*szp  = offset = sizeof (ulong);
		left  = 1024 - offset;

		export_strings += offset;

		*(ulong *) export_strings = 0;	/* initial 4 bytes	*/
	}
			
	if (kp->n_zeroes = xp->n_zeroes) {	/* sic	*/
		kp->n_zeroes = xp->n_zeroes;
		kp->n_offset = xp->n_offset;
	} else if (strcmp(prev, xp->n_nptr) == 0) {
		/*
		 * same name as previous entry: just use previous
		 */
		kp->n_offset = offset - strlen(xp->n_nptr) - 1;
	} else if (find_suffix(xp->n_nptr, *strp, offset, &kp->n_offset)) {
		/*
		 * found a string that we are a suffix of
		 */
		;
	} else {
		/*
		 * need to add to our string table
		 */
		len = strlen(xp->n_nptr) + 1;
		while (len >= left) {
			export_strings = (char *)realloc(*strp, sz += 1024);
			if (!export_strings)
				error("no memory for EXPORT string table");
			*strp = export_strings;
			left += 1024;
			prev  = "";	/* lazy	*/
		}

		strcpy(prev = *strp + offset, xp->n_nptr);

		kp->n_offset = offset;
		offset += len;
		left   -= len;
		*szp   += len;
	}

	kp->n_value  = xp->n_value;

	if (debug)
		dump_ksym(kp, *strp);
}

/*
 * find_suffix	-	look for a string that arg string is suffix of
 *
 * Input:
 *	p	-	^ to string we hope is a suffix of another
 *	strings	-	^ to string table
 *	max	-	max offset of valid string in strings
 *	offp	-	^ to place to store offset, if containing string found
 *
 * Returns:
 *	 0	-	no containing string found
 *	!0	-	string found of which `p' is a suffix
 *
 * NOTE:
 *	This is rather inefficient.
 */
find_suffix(p, strings, max, offp)
register char *p, *strings;
ulong *offp; {
	register char *q, *e;
	register len = strlen(p) - 1;

	strings += sizeof (ulong);
	max     -= sizeof (ulong);

	for (e = strings + max; e > strings; ) {
		/*
		 * adjust `e' to point at last non-blank
		 */
		if (*e == 0) {
			--e;
			continue;
		}

		for (q = p + len; q > p && *q == *e; )
			--q, --e;

		if (*q == *e) {
			if (debug)
				printf("found_suffix: %s\n", p);
			return *offp = e - strings + sizeof (ulong);
		}

		if (*e)
			while (*e && e > strings)
				--e;
	}

	return 0;
}

/*
 * xsym_compar -	compare two XCOFF symbol table entries
 *
 * If the names are the same, sort by descending storage class, so that
 * C_EXT < C_HIDEXT;
 */
int xsym_compar(vxp, vxq)
	void *vxp, *vxq;
{
	struct syment *xp = vxp, *xq = vxq;
	register char *p, *q;
	register compar;

	p = (xp->n_zeroes ? xp->n_name : xp->n_nptr);
	q = (xq->n_zeroes ? xq->n_name : xq->n_nptr);

	if (xp->n_zeroes || xq->n_zeroes)
		compar = strncmp(p, q, 8);
	else
		compar = strcmp(p, q);

	if (compar == 0)
		compar = xp->n_sclass - xq->n_sclass;

	return compar;
}

/*
 * dump_xsym -	print to XCOFF symbol
 */
dump_xsym(xsp)
struct syment *xsp; {

	if (xsp->n_zeroes)
		printf(
"nptr <%-8.8s  %8.8s> val %8.8x sc# %4.4x type %4.4x sclass %2.2x naux %2.2x\n"
		       , xsp->n_name
		       , ""
		       , xsp->n_value
		       , xsp->n_scnum & 0xffff
		       , xsp->n_type

		       , xsp->n_sclass
		       , xsp->n_numaux);
	else
		printf(
"nptr <%-17.17s> val %8.8x sc# %4.4x type %4.4x sclass %2.2x naux %2.2x\n"
		       , xsp->n_nptr
		       , xsp->n_value
		       , xsp->n_scnum & 0xffff
		       , xsp->n_type
		       , xsp->n_sclass
		       , xsp->n_numaux);
}

dump_ksym(ksp, strings)
sym_t *ksp;
char *strings; {

	if (ksp->n_zeroes)
		printf("%8.8x %-8.8s\n", ksp->n_value, ksp->n_name);
	else
		printf("%8.8x %s\n", ksp->n_value, ksp->n_offset + strings);
}

error(p, a, b, c, d, e)
char *p; {

	fprintf(stderr, p, a, b, c, d, e);
	fprintf(stderr, "\n");
	exit(1);
}

sys_error(p, a, b, c, d, e) 
char *p;
{

	fprintf(stderr, p, a, b, c, d, e);
	perror(": ");
	exit(1);
}

warn(p, a, b, c, d, e) 
char *p;
{

	fprintf(stderr, p, a, b, c, d, e);
	fprintf(stderr, "\n");
}
