/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: utc_gettime_user.c,v $
 * Revision 1.1.11.2  1996/02/17  23:35:13  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:24  marty]
 *
 * Revision 1.1.11.1  1995/12/08  18:13:01  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/08  17:27 UTC  sommerfeld
 * 	Back off gracefully if shm isn't there.
 * 	Only lock shm once every 10 seconds.
 * 	[1995/08/31  19:22 UTC  sommerfeld  /main/sommerfeld_dfsperf/1]
 * 
 * Revision 1.1.8.4  1994/09/23  20:00:10  tom
 * 	Back out change of unsigned/signed64 to typedef hyper
 * 	[1994/09/23  19:26:53  tom]
 * 
 * Revision 1.1.8.2  1994/08/23  20:32:16  cbrooks
 * 	Code Cleanup
 * 	[1994/08/19  17:52:53  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:30:20  cbrooks]
 * 
 * Revision 1.1.8.1  1994/05/12  15:17:48  rhw
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:11:05  rhw]
 * 
 * Revision 1.1.5.3  1992/12/30  16:37:21  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:19:49  htf]
 * 
 * Revision 1.1.5.2  1992/09/29  21:28:23  devsrc
 * 	[OT 5373]  SNI/SVR4 Merge.
 * 	[1992/09/14  20:05:42  sekhar]
 * 
 * Revision 1.1.2.2  1992/05/20  21:06:28  comuzzi
 * 	Fixed endian problems in user mode API. OT defect 3676.
 * 	[1992/05/20  15:17:07  comuzzi]
 * 
 * Revision 1.1  1992/01/19  15:35:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 	Module utc_gettime_user.c
 *	Version X0.6
 */
/*
 * Copyright (c) 1990 by
 * Digital Equipment Corporation, Maynard, Mass.
 * 
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:	This module implements utc_gettime using shared memory
 *		primatives for the no-kernel-mods version of DTS.
 *
 * ENVIRONMENT:	Ultrix V4.0
 *
 * AUTHORS:	Joe Comuzzi
 *		Distributed Systems Services (DSS) 
 * 
 * DATE:	August 27, 1990
 *
 */

/*
 *(c) Copyright 1991,1992 Siemens-Nixdorf Information Systems,Burlington,MA,USA.
 * All Rights Reserved
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <utctime.h>

#define _UTC_H	"Don't expand this include file!"
#define _DNA_UTIL_H "Don't expand this include file!"

#include <dtss_config.h>
#include <utc_shared.h>
#include <utc_arith.h>

#include <assert.h>

/*
 * Include arith.c
 */

#define INCLUDED 1
#include <arith.c>
#include <utc.h>

#define KERNEL_PARAMS_DATAFILE	"/var/adm/time/dtss_kernel.conf"

extern SharedState *utc_sharedptr;

int utc_gettime( utc_t *);
int utc_mkbintime(utc_t *, timespec_t *,  timespec_t *, long );


int utc_gettime(utc_t *utc)
{
    struct timeval tv;
    struct timespec ts, its;
    static struct timeval last_map_tv;
    static int backoff = 1;
    static SharedState state;
    Bits64 temp, drift;
    UTCValue tn;
    long waste;

    if (utc_sharedptr == (SharedState *)NULL)
    {
	dce_gettimeofday(&tv);
	if (abs(tv.tv_sec - last_map_tv.tv_sec) > backoff)
	{
	    last_map_tv = tv;
	    if (utc_sharedmap(SHM_RDONLY) != 0) 
	    {
		if (backoff < 1024)
		    backoff <<= 1;
	    }
	}
    }
    if (utc_sharedptr == (SharedState *)NULL) {
	/* still not there.. */
	ts.tv_sec = tv.tv_sec;
	ts.tv_nsec = tv.tv_usec * 1000;
	return(utc_mkbintime(utc, &ts, (struct timespec *)NULL, 0));
    }
    
    /*
     * at this point, we believe the utc_sharedptr is valid 
     * since we go indirect thru this field
     * we use a real assert here instead of a dce_assert() because 
     * the dce_assert calls gettime(), and we don't want no stinking
     * recursion goin' on around here
     */

#ifdef DCE_DEBUG 
    assert(utc_sharedptr != 0) ;
#endif    

    dce_gettimeofday(&tv);
    if (abs(tv.tv_sec - last_map_tv.tv_sec) > 10) 
    {
	utc_readlock();
	dce_gettimeofday(&tv);
	last_map_tv = tv;
	state = *utc_sharedptr;
	utc_readunlock();
    }
    /* Fill in the time */

    ts.tv_sec = tv.tv_sec;
    ts.tv_nsec = tv.tv_usec * 1000;
    emul(tv.tv_sec, K_100NS_PER_SEC, &tn);
    UTCiAdd(&tn, tv.tv_usec * 10, &tn);

    its.tv_sec = -1;

    if (state.inaccinfinite == 0) {
	/* Compute drift and resolution part of inaccuracy */

        UTCsub(&tn, &state.driftbase, &temp);
	UTCiDiv(&temp, state.currentmdr, &drift, &waste);
	UTCiAdd(&drift, state.resolution + 1, &drift);

	/* Compute amount to reduce inaccuracy */

	if (IsUTCgt(&tn, &state.adjbase)) {
	    if (IsUTCgt(&tn, &state.adjend)) {
		UTCsub(&state.adjend, &state.adjbase, &temp);
	    } else {
		UTCsub(&tn, &state.adjbase, &temp);
	    }
	    UTCiDiv(&temp, state.adjrate, &temp, &waste);
	    UTCsub(&drift, &temp, &drift);
	}

	/* Add in leap second if appropriate */

	if (IsUTCgt(&tn, &state.leaptime))
	    UTCiAdd(&drift, K_100NS_PER_SEC, &drift);

	UTCadd(&state.inacc, &drift, &state.inacc);
	UTCiDiv(&state.inacc, K_100NS_PER_SEC, &temp, &its.tv_nsec);

	its.tv_nsec *= 100;
	its.tv_sec = temp.lo;
    }

    if (tv.tv_sec >= state.tdftime)
	state.curtdf = state.nextdf;

    return(utc_mkbintime(utc, &ts, &its, state.curtdf*60));
}
