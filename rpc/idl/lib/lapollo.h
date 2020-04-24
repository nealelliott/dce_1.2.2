/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: lapollo.h,v $
 * Revision 1.1.10.2  1996/02/18  23:45:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:34  marty]
 *
 * Revision 1.1.10.1  1995/12/07  22:28:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:15:27  root]
 * 
 * Revision 1.1.6.1  1994/01/21  22:30:37  cbrooks
 * 	RPC Code Cleanup - Initial Submission
 * 	[1994/01/21  21:51:21  cbrooks]
 * 
 * Revision 1.1.4.2  1993/07/07  20:08:09  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:37:45  ganni]
 * 
 * $EndLog$
 */
/*
**
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**  Digital Equipment Corporation, Maynard, Mass.
**  All Rights Reserved.  Unpublished rights reserved
**  under the copyright laws of the United States.
**
**  The software contained on this media is proprietary
**  to and embodies the confidential technology of
**  Digital Equipment Corporation.  Possession, use,
**  duplication or dissemination of the software and
**  media is authorized only pursuant to a valid written
**  license from Digital Equipment Corporation.
**
**  RESTRICTED RIGHTS LEGEND   Use, duplication, or
**  disclosure by the U.S. Government is subject to
**  restrictions as set forth in Subparagraph (c)(1)(ii)
**  of DFARS 252.227-7013, or in FAR 52.227-19, as
**  applicable.
**
**  NAME:
**
**      lapollo.h
**
**  FACILITY:
**
**      IDL Stub Runtime Support
**
**  ABSTRACT:
**
**      Apollo system dependencies.
**
**  VERSION: DCE 1.0
*/

#ifndef LAPOLLO_H
#define LAPOLLO_H
    
/*
 * If we're building an apollo shared library, we need to use the shared
 * vesions of the functions in libc.  The following include file will
 * do the right thing.
 */
#ifdef APOLLO_GLOBAL_LIBRARY
#   include <local/shlib.h>
#endif

#include <stdlib.h>         

/*
 * Tell the compiler to place all static data, declared within the scope
 * of a function, in a section named nck_pure_data$.  This section will
 * be loaded as a R/O, shared, initialized data section.  All other data,
 * global or statics at the file scope, will be loaded as R/W, per-process,
 * and zero-filled.
 */
#if __STDC__
#   pragma HP_SECTION( , nck_pure_data$)
#else
#   section( , nck_pure_data$)
#endif

#endif
