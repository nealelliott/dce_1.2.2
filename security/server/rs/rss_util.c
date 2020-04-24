/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rss_util.c,v $
 * Revision 1.1.6.2  1996/02/18  00:20:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:33  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:57:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:21:48  root]
 * 
 * Revision 1.1.3.3  1992/12/29  16:37:53  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:27:21  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  20:13:16  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:38:23  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:43:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rss_util.c V=11 05/29/91 //littl/prgy/src/server/rs
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
 *     Registry Server Surrogate - utilities
 * 
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rss_util.c	11 - 05/29/91";
#endif


#include <un_time.h>
#include <un_io.h> 

#include <rss_util.h>
#include <rs_base.h> 
#include <rsdb_pvt.h>
#include <rsdb_util.h>
#include <rsdb_dir.h>


PUBLIC boolean32 rs_verbose = false;


PUBLIC void rs_util_apply_full_timestamp
#ifndef __STDC__
    ( ts ) 
    sec_timeval_t   *ts;
#else
  (
    sec_timeval_t   *ts
  )
#endif
{
    struct timezone tz;

#ifdef SNI_SVR4_POSIX
    gettimeofday( (struct timeval *) ts);
#else
    gettimeofday( (struct timeval *) ts, &tz);
#endif
}

PUBLIC void rs_util_apply_timestamp
#ifndef __STDC__
    ( ts ) 
    sec_timeval_sec_t  *ts;
#else
  (
    sec_timeval_sec_t  *ts
  )
#endif
{
    sec_timeval_t   rts;

    rs_util_apply_full_timestamp(&rts);
    *ts = rts.sec;
}

/*
  if clock1 > clock2 return 1
  if clock1 = clock2 return 0
  if clock1 < clock2 return -1
*/
PUBLIC short rs_util_cmp_clock
#ifndef __STDC__
    (clock1, clock2)
    sec_timeval_t   clock1;
    sec_timeval_t   clock2;
#else
  (
    sec_timeval_t   clock1,
    sec_timeval_t   clock2
  )
#endif
{
    short cmp_clock;

    if (timercmp((struct timeval *) &clock1, (struct timeval *) &clock2, <)) {
         cmp_clock = -1;
    } else if (timercmp((struct timeval *) &clock1,
			(struct timeval *) &clock2, ==)) {
         cmp_clock = 0;
    } else if (timercmp((struct timeval *) &clock1, 
			(struct timeval *) &clock2, >)) {
         cmp_clock = 1;
    }

    return(cmp_clock);
}



