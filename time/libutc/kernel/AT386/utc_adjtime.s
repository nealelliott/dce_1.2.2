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
 * Revision 1.1.8.2  1996/03/09  23:31:28  marty
 * 	Update OSF copyright year
 * 	[1996/03/09  22:46:38  marty]
 *
 * Revision 1.1.8.1  1995/12/08  18:11:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:24:53  root]
 * 
 * Revision 1.1.6.2  1993/05/24  20:51:43  cjd
 * 	Submitting 102-dme port to 103i
 * 	[1993/05/24  20:20:28  cjd]
 * 
 * Revision 1.1.4.2  1993/05/12  17:43:27  jd
 * 	Initial 486 port.
 * 	[1993/05/12  17:42:22  jd]
 * 
 * $EndLog$
 */

#include	<machine/asm.h>
#include	<syscall.h>

SYSCALL(utc_adjtime)
	ret
