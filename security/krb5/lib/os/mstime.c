/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mstime.c,v $
 * Revision 1.1.5.2  1996/02/18  00:15:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:53  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:43:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:33  root]
 * 
 * Revision 1.1.3.3  1992/12/29  16:30:38  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:18:13  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  20:42:56  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:35:09  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:50:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  mstime.c V=2 10/02/90 //littl/prgy/krb5/lib/os
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
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/mstime.c,v $
 * $Author: marty $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <krb5/copyright.h>.
 *
 * krb5_mstimeofday for BSD 4.3
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_mstime_c[] =
"$Id: mstime.c,v 1.1.5.2 1996/02/18 00:15:33 marty Exp $";
#endif	/* lint || SABER */

#include <krb5/copyright.h>

#include <krb5/config.h>
#include <krb5/krb5.h>
#include <krb5/libos-proto.h>
#include <krb5/sysincl.h>

extern int errno;

krb5_error_code
krb5_ms_timeofday(seconds, milliseconds)
register krb5_int32 *seconds;
register krb5_ui_2 *milliseconds;
{
    struct timeval tv;

#ifdef SNI_SVR4
    if (gettimeofday(&tv) == -1) {
#else
    if (gettimeofday(&tv, (struct timezone *)0) == -1) {
#endif /* SNI_SVR4 */

	/* failed, return errno */
	return (krb5_error_code) errno;
    }
    *seconds = tv.tv_sec;
    *milliseconds = tv.tv_usec / 1000;
    return 0;
}
