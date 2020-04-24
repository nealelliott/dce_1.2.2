/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rss_util.h,v $
 * Revision 1.1.4.2  1996/02/18  23:00:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:08  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:28:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:03  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:26:59  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:15:43  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rss_util.h V=9 05/29/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server Surrogate - utilities
 * 
 */

#ifndef rss_util__included
#define rss_util__included

#include <dce/rgynbase.h>


extern  boolean32 rs_verbose;

void rs_util_apply_full_timestamp ( 
#ifdef __STDC__
    sec_timeval_t   *ts
#endif
);

void rs_util_apply_timestamp ( 
#ifdef __STDC__
    sec_timeval_sec_t  *ts
#endif
);

short rs_util_cmp_clock (
#ifdef __STDC__
    sec_timeval_t   clock1,
    sec_timeval_t   clock2
#endif
);

#endif  /* rss_util__included */

