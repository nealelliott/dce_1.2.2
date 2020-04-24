/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sysincludes.h,v $
 * Revision 1.1.624.1  1996/10/02  18:10:06  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:44:43  damon]
 *
 * $EndLog$
*/
/* $Header: /u0/rcs_trees/dce/rcs/file/osi/RIOS/sysincludes.h,v 1.1.624.1 1996/10/02 18:10:06 damon Exp $ */

/* Copyright (C) 1994, 1989 Transarc Corporation - All rights reserved */

#ifndef	TRANSARC_OSI_SYSINCLUDES_H
#define	TRANSARC_OSI_SYSINCLUDES_H 1

/* only defined these symbols, if they haven't been defined already */
#if !defined(_ALL_SOURCE)
#define _ALL_SOURCE
#endif /* !defined(ALL_SOURCE) */

#if !defined(_POSIX_SOURCE)
#define _POSIX_SOURCE
#endif /* !defined(_POSIX_SOURCE) */


/*
 * Basic kernel include files
 */
#include <sys/types.h>
#include <sys/time.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <sys/statfs.h>
#include <sys/vattr.h>
#include <sys/vnode.h>
#include <sys/vfs.h>
#include <fcntl.h>
#ifdef KERNEL
#include <sys/errno.h>
#include <sys/syspest.h>
#include <sys/limits.h>
#else /* KERNEL */
#include <errno.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <strings.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <locale.h>
#endif /* KERNEL */

#endif /* TRANSARC_OSI_SYSINCLUDES_H */
