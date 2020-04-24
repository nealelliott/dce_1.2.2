/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: un_stat.h,v $
 * Revision 1.1.8.2  1996/02/18  23:01:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:05  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:30:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:36  root]
 * 
 * Revision 1.1.5.3  1992/12/29  13:28:18  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:47:33  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  21:17:44  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:21:31  sekhar]
 * 
 * Revision 1.1.2.2  1992/06/30  21:36:49  burati
 * 	CR4516 HP/UX porting changes
 * 	[1992/06/30  20:30:46  burati]
 * 
 * Revision 1.1  1992/01/19  14:43:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  un_stat.h V=3 11/11/91 //nbs/dds_tools/src
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/* 
**      Prototypes for unix stat functions
*/

#ifndef unix_stat_h__included
#define unix_stat_h__included

#include <sys/types.h>
#include <sys/stat.h>

#if !defined(_INCLUDE_POSIX_SOURCE) && !defined(_POSIX_SOURCE) && !defined(SNI_SVR4_POSIX)
int stat (
#ifdef __STDC__
    char        *path,
    struct stat *buf
#endif
);
#endif

#ifdef apollo
int _bky_stat (
#ifdef __STDC__
    char        *path,
    struct stat *buf
#endif
);
#endif /* apollo */

int lstat (
#ifdef __STDC__
    const char  *path,
    struct stat *buf
#endif
);

int fstat (
#ifdef __STDC__
    int         fd,
    struct stat *buf
#endif
);

#endif /* unix_stat_h__included */
