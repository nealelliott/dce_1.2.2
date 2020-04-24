/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: gettime.c,v $
 * Revision 1.1.4.2  1996/02/18  19:21:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:06:55  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:37:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:11  root]
 * 
 * Revision 1.1.2.2  1994/10/06  20:25:08  agd
 * 	expand copyright
 * 	[1994/10/06  14:11:50  agd]
 * 
 * 	expand copyright
 * 
 * Revision 1.1.2.1  1994/08/28  14:40:28  rsalz
 * 	Add dce_gettimeofday (OT CR 11931).
 * 	[1994/08/28  14:20:11  rsalz]
 * 
 * $EndLog$
 */
#include <dce/dce.h>
#include <dce/dce_utils.h>
#include <time.h>
#include <pthread.h>


int
dce_gettimeofday(
    struct timeval	*tv
)
{
#ifdef	SNI_SRV4
    return gettimeofday(tv);
#else
    return gettimeofday(tv, NULL);
#endif	/* SNI_SVR4 */
}
