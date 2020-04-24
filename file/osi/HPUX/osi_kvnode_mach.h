/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_kvnode_mach.h,v $
 * Revision 1.1.810.1  1996/10/02  17:57:39  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:32  damon]
 *
 * Revision 1.1.805.2  1994/06/09  14:15:08  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:27:40  annie]
 * 
 * Revision 1.1.805.1  1994/02/04  20:24:04  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:15:28  devsrc]
 * 
 * Revision 1.1.803.2  1994/01/20  18:43:29  annie
 * 	added copyright header
 * 	[1994/01/20  18:39:45  annie]
 * 
 * Revision 1.1.803.1  1993/12/07  17:29:32  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:58:32  jaffe]
 * 
 * Revision 1.1.7.3  1993/08/13  15:13:27  kinney
 * 	Add definition of vcexcl stuff
 * 	[1993/08/13  12:39:49  kinney]
 * 
 * Revision 1.1.7.2  1993/07/19  19:32:56  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:25:24  zeliff]
 * 
 * Revision 1.1.4.3  1993/07/16  20:18:46  kissel
 * 	*** empty log message ***
 * 	[1993/06/21  14:33:24  kissel]
 * 
 * 	Initial GAMERA branch
 * 	[1993/04/02  11:56:11  mgm]
 * 
 * Revision 1.1.2.2  1993/06/04  15:08:55  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  21:42:33  kissel]
 * 
 * Revision 1.1.2.2  1992/10/15  21:00:53  toml
 * 	Initial revision.
 * 	[1992/10/14  16:55:58  toml]
 * 
 * $EndLog$
*/

/*
 * The purpose of this file is to allow us to compile Episode in a non-kernel
 * environment, by duplicating various declarations that are found under
 * #ifdef _KERNEL in the system include files.
 */

#ifndef _KERNEL

/* from <sys/vnode.h> */

enum vcexcl	{ NONEXCL, EXCL};		/* (non)excl create (create) */

#define IO_UNIT		0x01
#define IO_APPEND	0x02
#define IO_SYNC		0x04

#define VN_RELE(VP)     { \
        vn_rele(VP); \
}

/* from <sys/resource.h> */

#define RLIMIT_FSIZE    1

/* from <sys/buf.h> */

struct  vnode *devtovp();

#endif
