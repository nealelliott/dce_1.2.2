/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: internal.h,v $
 * Revision 1.1.2.1  1996/06/05  21:23:54  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:13:38  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * internal include file for com_err package
 */
#include "mit-sipb-copyright.h"

#include <errno.h>

#ifdef NEED_SYS_ERRLIST
extern char const * const sys_errlist[];
extern const int sys_nerr;
#endif

#if defined(__STDC__) && !defined(HDR_HAS_PERROR) && !defined(_WINDOWS)
void perror (const char *);
#endif
