/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: utc_gettime.c,v $
 * Revision 1.1.9.2  1996/02/17  23:35:05  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:16  marty]
 *
 * Revision 1.1.9.1  1995/12/08  18:12:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:25:07  root]
 * 
 * Revision 1.1.7.2  1993/06/24  21:31:27  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:49:18  hinman]
 * 
 * Revision 1.1.3.4  1993/02/01  22:43:20  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:40:36  hinman]
 * 
 * Revision 1.1.3.3  1992/12/30  16:36:45  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:19:17  htf]
 * 	Revision 1.1.5.2  1993/01/11  16:25:06  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 1.1.3.2  1992/09/29  21:28:07  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/18  18:14:06  sekhar]
 * 
 * $EndLog$
 */
/*
 * @SNI_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: utc_gettime.c,v $
 * Revision 1.1.9.2  1996/02/17  23:35:05  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:16  marty]
 *
 * Revision 1.1.10.2  1996/02/17  22:51:16  marty
 * 	Update OSF copyright year
 *
 * Revision 1.1.9.1  1995/12/08  18:12:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:25:07  root]
 *
 * Revision 1.1.8.2  1995/12/08  17:25:07  root
 * 	Submit OSF/DCE 1.2.1
 *
 * Revision 1.1.7.2  1993/06/24  21:31:27  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:49:18  hinman]
 *
 * Revision 1.1.6.2  1993/06/17  14:49:18  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 *
 * Revision 1.1.3.4  1993/02/01  22:43:20  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:40:36  hinman]
 *
 * Revision 1.1.5.3  1993/01/31  17:40:36  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 *
 * Revision 1.1.3.3  1992/12/30  16:36:45  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:19:17  htf]
 *
 * Revision 1.1.4.2  1992/12/28  22:19:17  htf
 * 	Embedding copyright notice
 *
 * Revision 1.1.3.2  1992/09/29  21:28:07  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/18  18:14:06  sekhar]
 *
 * Revision 1.1.2.2  1992/09/18  18:15:45  sekhar
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/18  18:14:06  sekhar]
 *
 * Revision 1.1.1.2  1992/09/18  18:14:06  sekhar
 * 	[OT 5373]    SNI/SVR4 merge.
 *
 * Revision 9.1  92/07/19  20:10:15  hinman
 * Create split point
 * 
 * Revision 1.1.2.4  92/07/15  15:43:49  karr
 * 	Check for null ptr from getenv().
 * 	[92/07/15  15:41:24  karr]
 * 
 * 	Create initial version.
 * 	[92/07/14  17:18:31  karr]
 * 
 * Revision 1.1.2.3  92/07/14  17:25:06  karr
 * 	Create initial version.
 * 	[92/07/14  17:18:31  karr]
 * 
 * Revision 1.1.2.2  92/04/17  11:00:58  tikku
 * 	initial version
 * 	[92/04/17  10:59:08  tikku]
 * 
 * Revision 1.1.2.2  92/04/17  11:00:58  tikku
 * 	initial version
 * 	[92/04/17  10:59:08  tikku]
 * 
 * $EndLog$
 */

#ifdef SNI_DCOSX

/*
 * We implement this system call using
 * sysmips as the initial entry point.
 */

#include <sys/sysmips.h>
#include <sys/syscall.h>

#ifndef PMIPS_DCECALL
#define PMIPS_DCECALL 121
#endif

int
utc_gettime (void *utc)	/* (utc_t *) */
{
	int uap[6];
	uap[0] = (int)utc;
	uap[1] = 0;
	uap[2] = 0;
	uap[3] = 0;
	uap[4] = 0;
	uap[5] = 0;
	return(syscall(SYS_sysmips,PMIPS_DCECALL,1,(int)uap,0));
}

#else	/* SNI_DCOSX */

#include <stdlib.h>
#include <utctime.h>

int
utc_gettime (utc_t *utc)
{
	const char *new_kernel;

	if ((new_kernel = getenv ("CLOCAL_PRESENT")) && 
	    (strcmp (new_kernel, "false") == 0)) 
		return (syscall (70, utc));
	else
		return ( clocal ( 2, utc ) ) ;
}

#endif	/* SNI_DCOSX */
