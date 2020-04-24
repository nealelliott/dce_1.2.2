/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sysdep.c,v $
 * Revision 1.1.8.2  1996/02/18  00:07:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:58:54  marty]
 *
 * Revision 1.1.8.1  1995/12/13  16:24:03  root
 * 	Submit
 * 	[1995/12/11  15:13:54  root]
 * 
 * Revision 1.1.3.3  1992/12/29  13:04:39  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:42:14  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  21:16:53  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:20:36  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:47:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  sysdep.c V=7 04/24/91 //littl/prgy/src/client/rca
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
**      Registry Client Agent Interface - System Dependent routines
** 
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)sysdep.c	7 - 04/24/91";
#endif

#include <rs_base.h>
#include <un_time.h>
#include <rgybase.h>
#include <sysdep.h>
#include <bstring.h>

public void rgylib_init()
{
}

public short rgy_cmp_clock(clock1, clock2)
    sec_timeval_t   clock1;
    sec_timeval_t   clock2;
{
    short cmp_clock;

    if (timercmp((struct timeval *) &clock1, (struct timeval *) &clock2, <)) {
         cmp_clock = -1;
    } else if (timercmp((struct timeval *) &clock1, (struct timeval *) &clock2, ==)) {
         cmp_clock = 0;
    } else if (timercmp((struct timeval *) &clock1, (struct timeval *) &clock2, >)) {
         cmp_clock = 1;
    }

    return(cmp_clock);
}

public void rgy_get_clock(clock)
    sec_timeval_t   *clock;
{
    struct timezone tz;

#ifdef SNI_SVR4_POSIX
    gettimeofday((struct timeval *) clock);
#else
    gettimeofday((struct timeval *) clock, &tz);
#endif /* SNI_SVR4_POSIX */
}

