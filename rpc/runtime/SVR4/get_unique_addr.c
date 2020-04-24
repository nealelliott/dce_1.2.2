/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * (c) Copyright 1991, 1992 
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
 */
/*
 * HISTORY
 * $Log: get_unique_addr.c,v $
 * Revision 1.1.71.2  1996/02/18  00:01:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:54:11  marty]
 *
 * Revision 1.1.71.1  1995/12/08  00:16:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:08  root]
 * 
 * Revision 1.1.69.1  1994/01/21  22:33:35  cbrooks
 * 	RPC Code Cleanup - Initial Submission
 * 	[1994/01/21  20:59:50  cbrooks]
 * 
 * Revision 1.1.5.2  1993/06/24  20:16:51  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:38:36  hinman]
 * 
 * Revision 1.1.3.4  1993/01/03  22:57:28  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:57:38  bbelch]
 * 
 * Revision 1.1.3.3  1992/12/23  20:11:20  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:29:10  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  20:42:09  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:36:51  weisman]
 * 
 * $EndLog$
 */

#ifdef SNI_DCOSX

/*
 * We implement this system call (and others from time) using
 * sysmips as the initial entry point.
 */

#include <sys/sysmips.h>
#include <sys/syscall.h>

#ifndef PMIPS_DCECALL
#define PMIPS_DCECALL 121
#endif

get_unique_addr(addrptr,stat)
void *addrptr;
void *stat;
{
	int uap[6];
	uap[0] = (int)addrptr;
	uap[1] = (int)stat;
	uap[2] = (int)"/dev/ip";
	uap[3] = (int)"/etc/ieee_802_addr";
	uap[4] = 0;
	uap[5] = 0;
	return(syscall(SYS_sysmips,PMIPS_DCECALL,0,(int)uap,0));
}

#else	/* SNI_DCOSX */

#include <stdlib.h> 

typedef struct {
	char eaddr[6];
} *uuid_address_p_t;

typedef unsigned long unsigned32;

int 
get_unique_addr ( uuid_address_p_t addrptr, unsigned32 *stat  )
{
	const char *new_kernel;

	if ((new_kernel = getenv ("CLOCAL_PRESENT")) &&
	    (strcmp (new_kernel, "false") == 0))
		return ( syscall (73, addrptr, stat, 0 ,0) ) ;
	else
		return ( clocal ( 4, addrptr, stat, 0 ,0) ) ;
}

#endif	/* SNI_DCOSX */
