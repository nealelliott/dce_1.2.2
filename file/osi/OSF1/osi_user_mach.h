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
 * Revision 1.1.386.1  1996/10/02  17:58:47  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:44:11  damon]
 *
 * Revision 1.1.381.2  1994/06/09  14:15:59  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:28:23  annie]
 * 
 * Revision 1.1.381.1  1994/02/04  20:25:11  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:15:53  devsrc]
 * 
 * Revision 1.1.379.1  1993/12/07  17:30:11  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:03:23  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/21  14:50:54  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  14:53:16  cjd]
 * 
 * Revision 1.1.2.2  1992/12/10  22:27:43  marty
 * 	OSF 1.1.1 port
 * 	[1992/11/30  23:07:46  marty]
 * 
 * $EndLog$
 */

#ifdef _KERNEL
#define osi_getufilelimit()     u.u_rlimit[RLIMIT_FSIZE].rlim_cur
#else
#define osi_getufilelimit()	RLIM_INFINITY
#endif
