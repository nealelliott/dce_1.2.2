/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: common.h,v $
 * Revision 1.1.149.1  1996/10/17  18:29:05  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:36  damon]
 *
 * Revision 1.1.144.1  1994/02/04  20:46:41  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:10  devsrc]
 * 
 * Revision 1.1.142.1  1993/12/07  17:45:45  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:34:39  jaffe]
 * 
 * Revision 1.1.13.2  1993/07/19  19:43:14  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:32:36  zeliff]
 * 
 * Revision 1.1.11.3  1993/07/16  22:03:18  kissel
 * 	Put HP RP work on the 1.0.3i branch.
 * 	[1993/06/21  15:07:52  kissel]
 * 
 * Revision 1.1.9.2  1993/06/04  18:35:17  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  22:46:07  kissel]
 * 
 * Revision 1.1.4.5  1993/03/29  21:35:04  maunsell_c
 * 	 03/16/93	maunsell_c	sorry, use __hpux not HP ifdef
 * 	[1993/03/29  21:09:20  maunsell_c]
 * 
 * Revision 1.1.4.4  1993/02/05  21:17:53  maunsell_c
 * 	02/01/93	maunsell_c	use HP ifdef not __hpux (sorry)
 * 	[1993/02/05  21:17:33  maunsell_c]
 * 
 * Revision 1.1.4.3  1993/01/28  23:00:55  kissel
 * 	Merged in maunsell_c change for __hpux to Dce1_0_2_Test930128 version.
 * 	[1993/01/28  22:59:09  kissel]
 * 
 * Revision 1.1.4.2  1992/11/17  15:12:13  maunsell_c
 * 	11/17/92         maunsell_c     Changed HP to __hpux
 * 
 * Revision 1.1.7.2  1993/01/29  19:20:30  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:14:01  htf]
 * 
 * Revision 1.1.2.2  1992/01/24  04:15:13  devsrc
 * 	Fixed logs
 * 	[1992/01/23  22:26:00  devsrc]
 * 
 * $EndLog$
*/
/*
 * OSF DCE Version 1.0, UPDATE 1.0.1
*/

#ifdef _AIX
#define RWA3              unsigned int
#define N_LG_SEEK         15000
#endif

#ifdef __hpux
#define RWA3              unsigned int
#define N_LG_SEEK         15000
#define getwd(path)       (getcwd((path),sizeof(path)-1))
#endif

#ifdef __OSF__
#define RWA3              unsigned int
#define N_LG_SEEK         15000
#endif


#if defined(MIPS)
#define RWA3              int
#define N_LG_SEEK         15000
#endif

#ifdef NEXT
#define RWA3              int
#define getcwd(path,sz)   (getwd(path))
#endif

#ifdef RT
#define RWA3              int
#define getcwd(path,sz)   (getwd(path))
#endif

#if defined(SPARC) || defined(sparc)
#define RWA3              int
#define N_LG_SEEK         15000
#endif

#ifdef SUN3
#define RWA3              int
#endif

#ifdef VAX
#define RWA3              int
#endif

#define EQ(s,t)       (strcmp(s,t) == 0)

#ifndef TRUE
#define TRUE          1
#define FALSE         0
#endif
