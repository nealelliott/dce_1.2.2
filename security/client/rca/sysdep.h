/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sysdep.h,v $
 * Revision 1.1.6.2  1996/02/18  22:57:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:34  marty]
 *
 * Revision 1.1.6.1  1995/12/13  16:24:04  root
 * 	Submit
 * 	[1995/12/11  15:13:55  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:04:44  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:13:49  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:47:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  sysdep.h V=5 04/24/91 //littl/prgy/src/client/rca
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**  System dependent features
** 
*/

#ifndef sysdep_h__included
#define sysdep_h__included

void rgylib_init(
#ifdef __STDC__
    void
#endif
);

short rgy_cmp_clock (
#ifdef __STDC__
    sec_timeval_t   clock1,
    sec_timeval_t   clock2
#endif
); 

void rgy_get_clock (
#ifdef __STDC__
    sec_timeval_t   *clock
#endif
);

#endif /* sysdep_h__included */
