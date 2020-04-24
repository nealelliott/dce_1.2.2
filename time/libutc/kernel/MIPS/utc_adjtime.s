/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: utc_adjtime.s,v $
 * Revision 1.1.4.2  1996/03/09  23:31:31  marty
 * 	Update OSF copyright year
 * 	[1996/03/09  22:46:41  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:11:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:24:57  root]
 * 
 * Revision 1.1.2.2  1992/12/30  20:41:25  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:19:00  htf]
 * 
 * Revision 1.1  1992/01/19  18:36:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*	@(#)utc_adjtime.s	2.1	(ULTRIX)	4/12/89 */

#include <mips/regdef.h>
#include <mips/asm.h>
#include <syscall.h>

#ifndef SYS_utc_adjtime
#define SYS_utc_adjtime	(220)
#endif

SYSCALL(utc_adjtime)
	RET
.end utc_adjtime
