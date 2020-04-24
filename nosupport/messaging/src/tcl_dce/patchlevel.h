/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: patchlevel.h,v $
 * Revision 1.1  1996/09/27  20:01:41  wwang
 * Initial revision
 *
 * Revision 1.1  1996/09/27  19:59:14  wwang
 * Initial revision
 *
 * Revision 1.1.4.1  1994/06/09  16:04:02  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:49:15  devsrc]
 *
 * Revision 1.1.2.1  1993/12/12  01:49:43  rousseau
 * 	Initial 7.3 version.
 * 	[1993/12/12  01:45:16  rousseau]
 * 
 * $EndLog$
 */
/*
 * patchlevel.h --
 *
 * This file does nothing except define a "patch level" for Tcl.
 * The patch level is an integer that increments with each new
 * release or patch release.  It's used to make sure that Tcl
 * patches are applied in the correct order and only to appropriate
 * sources.
 */

#define TCL_PATCH_LEVEL 106
