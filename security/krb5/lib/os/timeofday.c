/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: timeofday.c,v $
 * Revision 1.1.6.2  1996/02/18  00:15:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:06  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:44:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:42  root]
 * 
 * Revision 1.1.4.1  1993/10/14  17:45:41  mccann
 * 	CR8651 64bit port
 * 	[1993/10/14  17:44:34  mccann]
 * 
 * Revision 1.1.2.2  1992/12/29  16:31:27  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:19:00  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  timeofday.c V=3 10/24/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/timeofday.c,v $
 * $Author: marty $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
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
 * libos: krb5_timeofday function for BSD 4.3 
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_timeofday_c[] =
"$Id: timeofday.c,v 1.1.6.2 1996/02/18 00:15:51 marty Exp $";
#endif	/* lint || saber */

#include <time.h>
#include <stdio.h>			/* needed for libos-proto.h */

#include <krb5/config.h>
#include <krb5/krb5.h>
#include <krb5/los-proto.h>

#ifdef POSIX_TYPES
#define timetype time_t
#else
#define timetype long
#endif

extern int errno;

krb5_error_code
krb5_timeofday(timeret)
register krb5_int32 *timeret;
{
    timetype tval;

    tval = time(NULL);
    if (tval == (timetype) -1)
	return (krb5_error_code) errno;
    *timeret = tval;
    return 0;
}
