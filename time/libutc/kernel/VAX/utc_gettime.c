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
 * Revision 1.1.4.2  1996/02/17  23:35:08  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:18  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:12:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:25:11  root]
 * 
 * Revision 1.1.2.2  1992/12/30  16:36:57  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:19:27  htf]
 * 
 * Revision 1.1  1992/01/19  15:34:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* utc_gettime */

#include "SYS.h"

#ifndef SYS_utc_gettime
#define SYS_utc_gettime	190
#endif

SYSCALL(utc_gettime)
	ret
