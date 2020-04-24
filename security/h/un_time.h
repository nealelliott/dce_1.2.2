/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: un_time.h,v $
 * Revision 1.1.8.2  1996/02/18  23:01:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:09  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:30:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:38  root]
 * 
 * Revision 1.1.5.3  1992/12/29  13:28:22  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:47:41  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  21:17:57  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:21:45  sekhar]
 * 
 * Revision 1.1.2.2  1992/06/30  21:37:21  burati
 * 	CR4516 HP/UX porting changes
 * 	[1992/06/30  20:31:16  burati]
 * 
 * Revision 1.1  1992/01/19  14:43:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  un_time.h V=5 11/11/91 //nbs/dds_tools/src
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
**      Prototypes for unix time functions
*/

#ifndef unix_time_h__included
#define unix_time_h__included

#include <sys/time.h>
#include <time.h>

#ifndef _INCLUDE_HPUX_SOURCE
int gettimeofday (
#ifdef __STDC__
#ifdef SNI_SVR4
    struct timeval  *tp
#else
    struct timeval  *tp,
    struct timezone *tzp
#endif
#endif
);

int settimeofday (
#ifdef __STDC__
#ifdef SNI_SVR4
    struct timeval  *tp
#else
    struct timeval  *tp,
    struct timezone *tzp
#endif
#endif
);
#endif /* _INCLUDE_HPUX_SOURCE */

#endif /* unix_time_h__included */
