/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: export.h,v $
 * Revision 1.1.10.1  1996/10/02  17:45:33  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:36:42  damon]
 *
 * Revision 1.1.5.1  1994/06/09  14:06:33  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:21:55  annie]
 * 
 * Revision 1.1.3.2  1993/01/21  19:32:49  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  19:46:32  zeliff]
 * 
 * Revision 1.1  1992/01/19  02:41:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * $Header: /u0/rcs_trees/dce/rcs/file/export/RIOS/export.h,v 1.1.10.1 1996/10/02 17:45:33 damon Exp $
 * $Locker:  $
 *
 * export.h -	definitions for the EXPORT kernel extension
 */

/*
 * EXPORT kernel extension configuration parameters
 */
struct k_conf {
	ulong	nsyms;			/* # of symbols			*/
	ulong	symt_sz;		/* size of symbol table		*/
	ulong	str_sz;			/* size of string table		*/
	caddr_t	symtab;			/* user address of symtab	*/
	caddr_t	strtab;			/* user address of string table	*/
};

/*
 * kernel function import
 */
struct k_func {
	void	*(**fpp)();	/* ^ to ^ to function we import	*/
	char	*name;		/* ^ to symbol name		*/
	ulong	fdesc[3];	/* function descriptor storage	*/
};

/*
 * kernel variable import
 */
struct k_var {
	void	*varp;		/* ^ to surrogate variable	*/
	char	*name;		/* ^ to symbol name		*/
};
