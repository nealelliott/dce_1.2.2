/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_user_mach.h,v $
 * Revision 1.1.729.1  1996/10/02  17:58:00  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:40  damon]
 *
 * Revision 1.1.724.2  1994/06/09  14:15:24  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:27:55  annie]
 * 
 * Revision 1.1.724.1  1994/02/04  20:24:29  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:15:36  devsrc]
 * 
 * Revision 1.1.722.2  1994/01/20  18:43:30  annie
 * 	added copyright header
 * 	[1994/01/20  18:39:45  annie]
 * 
 * Revision 1.1.722.1  1993/12/07  17:29:43  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:00:32  jaffe]
 * 
 * Revision 1.1.6.2  1993/07/19  19:33:29  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:25:54  zeliff]
 * 
 * Revision 1.1.4.3  1993/07/16  20:19:22  kissel
 * 	*** empty log message ***
 * 	[1993/06/21  14:34:52  kissel]
 * 
 * 	Initial GAMERA branch
 * 	[1993/04/02  11:57:02  mgm]
 * 
 * Revision 1.1.2.2  1993/06/04  15:11:57  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  21:44:12  kissel]
 * 
 * Revision 1.1.2.3  1993/01/14  19:22:08  toml
 * 	Make proper #define for osi_getufilelimit for HPUX.
 * 	[1993/01/14  19:20:58  toml]
 * 
 * Revision 1.1.2.2  1992/10/15  21:01:45  toml
 * 	Initial revision.
 * 	[1992/10/14  16:57:09  toml]
 * 
 * $EndLog$
 */

#ifndef TRANSARC_OSI_USER_MACH_H
#define	TRANSARC_OSI_USER_MACH_H

/*
 * machine specific definitions for function to obtain user resource limits
 * Note that resource limits in HPUX are in terms of 512-byte blocks.
 */
#include <sys/user.h>
#define osi_getufilelimit()	(u.u_rlimit[RLIMIT_FSIZE].rlim_cur << 9)	 

#endif /* TRANSARC_OSI_USER_MACH_H */
