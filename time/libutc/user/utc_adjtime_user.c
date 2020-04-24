/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: utc_adjtime_user.c,v $
 * Revision 1.1.15.2  1996/02/17  23:35:11  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:22  marty]
 *
 * Revision 1.1.15.1  1995/12/08  18:12:59  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/4  1994/10/25  17:45 UTC  pare
 * 	Fix first argument in adjtime function -- change struct timeval * to const struct timeval *
 * 
 * 	HP revision /main/HPDCE02/3  1994/10/25  13:51 UTC  pare
 * 	Remove double define for ADJTIME.
 * 
 * 	HP revision /main/HPDCE02/2  1994/09/26  18:34 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:25:17  root]
 * 
 * Revision 1.1.12.5  1994/09/23  20:00:08  tom
 * 	Back out change of unsigned/signed64 to typedef hyper
 * 	[1994/09/23  19:26:52  tom]
 * 
 * Revision 1.1.12.3  1994/09/21  21:44:35  tom
 * 	Bug 12065 - Make sure we are mapping shared memory read/write
 * 	since utc_gettime maps readonly, and its may get called first.
 * 	[1994/09/21  21:44:22  tom]
 * 
 * Revision 1.1.12.2  1994/08/23  20:32:15  cbrooks
 * 	Code Cleanup
 * 	[1994/08/23  18:59:46  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/30  12:59 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.12.1  1994/05/12  15:17:46  rhw
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:11:04  rhw]
 * 
 * 	HP revision /main/HPDCE01/2  1994/03/28  20:47 UTC  pare
 * 
 * 	HP revision /main/HPDCE01/pare_rodan/1  1994/03/28  20:46 UTC  pare
 * 	replace dtstime with adjtime at HP-UX 10.0
 * 
 * Revision 1.1.7.3  1993/09/24  15:25:12  pare
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/24  15:24:47  pare]
 * 
 * 	   Replace calls to adjtime with calls to dtstime (same parameters).
 * 	   When hpux supports adjtime, calls will be put back.
 * 	   [1993/04/29  18:40:45  truitt]
 * 	[1993/06/08  20:13:49  sommerfeld]
 * 
 * Revision 1.1.8.3  1993/09/15  17:48:02  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.7.2  1993/07/08  19:58:53  root
 * 	Initial King Kong branch
 * 	[1993/07/08  19:58:22  root]
 * 
 * Revision 1.1.5.2  1993/03/18  15:05:53  truitt
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/18  15:05:05  truitt]
 * 
 * Revision 1.1.1.8  1993/03/11  22:34:00  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.3.2  1992/12/07  18:24:13  smythe
 * 	added __hpux dtstime calls to replace renamed adjtime
 * 	[1992/12/07  18:23:24  smythe]
 * 
 * Revision 1.1.5.3  1992/12/30  16:37:17  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:19:44  htf]
 * 
 * Revision 1.1.5.2  1992/09/29  21:28:18  devsrc
 * 	[OT 5373]  SNI/SVR4 Merge.
 * 	[1992/09/14  20:05:33  sekhar]
 * 
 * Revision 1.1.1.7  1992/09/21  18:55:57  osfrcs
 * 	    Loading drop Dce1_0_1fnl
 * 
 * Revision 1.1.2.2  1992/05/20  21:06:18  comuzzi
 * 	Fixed endian profile in user mode library. OT defect 3676.
 * 	[1992/05/20  15:18:11  comuzzi]
 * 
 * Revision 1.1  1992/01/19  15:35:14  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * 	Module utc_adjtime_user.c
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
 * ABSTRACT:	This module implements utc_adjtime using shared memory
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
#include <string.h>

#define _UTC_H	"Don't expand this include file!"
#define _DNA_UTIL_H "Don't expand this include file!"

#include <dtss_config.h>
#include <utc_shared.h>
#include <utc_arith.h>

/*
 * Include arith.c
 */

#define INCLUDED 1

#include <arith.c>

#define KERNEL_PARAMS_DATAFILE	"/var/adm/time/dtss_kernel.conf"

extern SharedState *utc_sharedptr;
static int utc_shared_read_write = 0;

static void InitializeKernelParams(void);

int utc_adjtime(enum adjmode , union adjunion *);


/*
 * we set these to default values that were previously set in the 
 * InitializeKernelParams() call. This is so that if a call is 
 * made to utc_gettime() and the shared memory segment is initialized
 * we don't fail to initialize these values to something sensible
 */
static long resolution = 10000000 / 100 ;
static long maxdrift = 1000 ;
static long frequency = 1000000000 ;
static long adjrate = 100 ;

#if defined(__hpux) && !(__hpux1000p)
extern int dtstime( struct timeval *, struct timeval *) ;
#define ADJTIME_FUNCTION dtstime 
int (*adjtime_func)(struct timeval *, struct timeval *) = ADJTIME_FUNCTION ;
#else 
extern int adjtime( const struct timeval *, struct timeval *) ;
#define ADJTIME_FUNCTION adjtime
int (*adjtime_func)(const struct timeval *, struct timeval *) = ADJTIME_FUNCTION ;
#endif

int utc_adjtime(enum adjmode mode, union adjunion *argblk)
{
    struct timeval tv, tv2;
    Bits64 temp;
    SharedState state;
    static int nextmdr;

    /*
     * Attempt to map shared memory
     * Watch out for utc_gettime mapping it read-only
     */
    if (utc_shared_read_write == 0) {
        utc_sharedunmap();		/* will check for null utc_sharedptr */
        utc_shared_read_write = 1;
    }

    if (utc_sharedptr == (SharedState *)NULL) {
	InitializeKernelParams() ;
	if (utc_sharedcreate()) {
	    return(-1);
	}
    }

    state = *utc_sharedptr;

    switch (mode) {

	case settime:

#ifdef SNI_SVR4
	    gettimeofday(&tv);
#else
	    gettimeofday(&tv, (struct timezone *)NULL);
#endif /* SNI_SVR4 */
	    tv.tv_sec += argblk->adjargs.a_adjustment.tv_sec;
	    tv.tv_usec += argblk->adjargs.a_adjustment.tv_nsec / 1000;
	    if (tv.tv_usec > 1000000) {
		tv.tv_usec -= 1000000;
		tv.tv_sec++;
	    }
	    
	    state.adjbase.hi = 0;
	    state.adjbase.lo = 0;
	    state.adjend.hi = 0;
	    state.adjend.lo = 0;

	    emul((long)argblk->adjargs.a_comptime.tv_sec,
	         K_100NS_PER_SEC, &state.driftbase);

	    UTCiAdd(&state.driftbase,
		    argblk->adjargs.a_comptime.tv_nsec / 100,
		    &state.driftbase);

	    state.inacc.hi = argblk->adjargs.a_baseinacc.hi;
	    state.inacc.lo = argblk->adjargs.a_baseinacc.lo;
	    if ((state.inacc.hi & 0xffff0000UL) == 0)
		state.inaccinfinite = 0;	

	    state.leaptime.hi = argblk->adjargs.a_leaptime.hi;
            state.leaptime.lo = argblk->adjargs.a_leaptime.lo;
	    state.curtdf = argblk->adjargs.a_curtdf;
	    state.nextdf = argblk->adjargs.a_nextdf;
	    state.tdftime = argblk->adjargs.a_tdftime;
	    state.currentmdr = nextmdr;

	    if (state.currentmdr == 0)
		state.currentmdr = maxdrift;

	    state.resolution = resolution;
	    state.adjrate = adjrate;

	    utc_writelock();
	    *utc_sharedptr = state;
#ifdef SNI_SVR4
	    settimeofday(&tv);
#else
	    settimeofday(&tv, (struct timezone *)NULL);
#endif /* SNI_SVR4 */
	    utc_writeunlock();
	    break;


	case adjusttime:

	    emul((long)argblk->adjargs.a_comptime.tv_sec,
		 K_100NS_PER_SEC, &state.driftbase);

	    UTCiAdd(&state.driftbase,
		    argblk->adjargs.a_comptime.tv_nsec / 100,
		    &state.driftbase);

	    emul((long)argblk->adjargs.a_adjustment.tv_sec,
		 K_100NS_PER_SEC, &state.adjend);

	    UTCiAdd(&state.adjend, argblk->adjargs.a_adjustment.tv_nsec / 100,
		    &state.adjend);

	    UTCabsVal(&state.adjend, &state.adjend);
	    UTCiMul((UTCValue *)&state.adjend, 100, &state.adjend);

	    state.inacc.hi = argblk->adjargs.a_baseinacc.hi;
	    state.inacc.lo = argblk->adjargs.a_baseinacc.lo;

	    if ((state.inacc.hi & 0xffff0000UL) == 0)
		state.inaccinfinite = 0;	

	    state.leaptime.hi = argblk->adjargs.a_leaptime.hi;
            state.leaptime.lo = argblk->adjargs.a_leaptime.lo;

	    state.curtdf = argblk->adjargs.a_curtdf;
	    state.nextdf = argblk->adjargs.a_nextdf;

	    state.tdftime = argblk->adjargs.a_tdftime;

	    state.currentmdr = nextmdr;

	    if (state.currentmdr == 0)
		state.currentmdr = maxdrift;

	    state.resolution = resolution;

	    state.adjrate = adjrate;

	    tv2.tv_sec = argblk->adjargs.a_adjustment.tv_sec;
	    tv2.tv_usec = (argblk->adjargs.a_adjustment.tv_nsec + 500) / 1000;

	    utc_writelock();
#ifdef SNI_SVR4
	    gettimeofday(&tv);
#else
	    gettimeofday(&tv, (struct timezone *)NULL);
#endif /* SNI_SVR4 */

	    emul((long)tv.tv_sec, K_100NS_PER_SEC, &state.adjbase);

	    UTCiAdd(&state.adjbase, tv.tv_usec * 10, &state.adjbase);

	    UTCadd(&state.adjbase, &state.adjend, &state.adjend);

	    *utc_sharedptr = state;
	    (*adjtime_func)(&tv2, (struct timeval *)NULL);
	    utc_writeunlock();

	    break;

	case endadjust:

	    state = *utc_sharedptr;
#ifdef SNI_SVR4
	    gettimeofday(&tv);
#else
	    gettimeofday(&tv, (struct timezone *)NULL);
#endif /* SNI_SVR4 */

	    emul((long)tv.tv_sec, K_100NS_PER_SEC, &temp);

	    UTCiAdd(&temp, tv.tv_usec * 10, &temp);

	    if (IsUTCgt(&state.adjend, &temp))
		state.adjend = temp;
	    tv.tv_sec = 0;
	    tv.tv_usec = 0;

	    utc_writelock();
	    *utc_sharedptr = state;
	    (*adjtime_func)(&tv, (struct timeval *)NULL);
	    utc_writeunlock();
	
	    break;

	case getresolution:
	
	    argblk->resolution = resolution * 100;
	    break;
	
	case getdrift:

	    argblk->maxdrift = maxdrift;
	    break;

	case getfreq:

	    argblk->frequency = frequency;
	    break;

	default:
	    return(-1);

    }

    return(0);
}

static void InitializeKernelParams(void)
{
    FILE *fp;
    char text[80];
    char params_file[K_MAX_FILENAME+1];
    long res = 10000000, temp;


    (void) strncpy(params_file, dcelocal_path, K_MAX_FILENAME);
    (void) strncat(params_file, KERNEL_PARAMS_DATAFILE, K_MAX_FILENAME);

    fp = fopen(params_file, "r");
    
    if (fp != NULL) {
	while(fgets(text, 80, fp) != 0) {
	    if (sscanf(text, "resolution=%ld", &temp) == 1)
		res = temp;
	    if (sscanf(text, "maxdrift=%ld", &temp) == 1)
		maxdrift = temp;
	    if (sscanf(text, "frequency=%ld", &temp) == 1)
		frequency = temp;
	    if (sscanf(text, "adjrate=%ld", &temp) == 1)
		adjrate = temp;
	}
    }

    resolution = res / 100;	/* Convert to units of 100ns. */
}
