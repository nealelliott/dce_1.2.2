/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ustime.c,v $
 * Revision 1.1.6.2  1996/02/18  00:15:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:09  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:44:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:44  root]
 * 
 * Revision 1.1.3.3  1992/12/29  16:31:34  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:19:06  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  20:43:11  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:35:34  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:50:36  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  ustime.c V=2 10/24/91 //littl/prgy/krb5/lib/os
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
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/ustime.c,v $
 * $Author: marty $
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 *
 * krb5_mstimeofday for BSD 4.3
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_mstime_c[] =
"$Id: ustime.c,v 1.1.6.2 1996/02/18 00:15:54 marty Exp $";
#endif	/* lint || SABER */

#include <krb5/config.h>
#include <krb5/krb5.h>
#include <krb5/los-proto.h>
#include <krb5/sysincl.h>

extern int errno;

static struct timeval last_tv = {0, 0};

krb5_error_code
krb5_us_timeofday(seconds, microseconds)
register krb5_int32 *seconds, *microseconds;
{
    struct timeval tv;

#ifdef SNI_SVR4_POSIX
    if (gettimeofday(&tv) == -1) {
#else
    if (gettimeofday(&tv, (struct timezone *)0) == -1) {
#endif /* SNI_SVR4_POSIX */
	/* failed, return errno */
	return (krb5_error_code) errno;
    }
    if ((tv.tv_sec == last_tv.tv_sec) && (tv.tv_usec == last_tv.tv_usec)) {
	    if (++last_tv.tv_usec >= 1000000) {
		    last_tv.tv_usec = 0;
		    last_tv.tv_sec++;
	    }
	    tv = last_tv;
    } else 
	    last_tv = tv;
	    
    *seconds = tv.tv_sec;
    *microseconds = tv.tv_usec;
    return 0;
}
