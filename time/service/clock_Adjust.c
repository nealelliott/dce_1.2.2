/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: clock_Adjust.c,v $
 * Revision 1.1.2.2  1996/02/17  23:35:17  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:31  marty]
 *
 * 	promoting previous changes to source tree
 * 	[1992/06/18  21:10:10  smythe]
 *
 * 	disable include of sys/limits.h for hpux,
 * 	it doesn't exist.
 * 	[1992/06/05  15:50:23  hopkins]
 *
 * 	Fixed Big/Little-endian problem. Cleaned up indentation of conditionals.
 * 	[1992/05/01  20:46:32  comuzzi]
 *
 * Revision 1.1.2.1  1995/12/08  18:13:14  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/07/25  19:26 UTC  pare
 * 	Merge files
 * 
 * 	HP revision /main/HPDCE02/pare_MOTHRA/1  1995/07/25  12:32 UTC  pare
 * 	Remove 'ultrix' from source file string -- CHFts15829
 * 
 * 	HP revision /main/HPDCE02/2  1994/09/26  19:10 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:25:27  root]
 * 
 * 	    Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:34:18  osfrcs]
 * 
 * Revision 1.1.8.7  1994/09/23  20:00:12  tom
 * 	Back out change of unsigned/signed64 to typedef hyper
 * 	[1994/09/23  19:26:57  tom]
 * 
 * Revision 1.1.8.5  1994/08/16  18:12:59  cbrooks
 * 	CR 11494 - 64 bit changes
 * 	[1994/08/09  19:11:12  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  18:09 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.8.2  1993/07/08  20:05:23  root
 * 	Initial King Kong branch
 * 	[1993/07/08  20:04:51  root]
 * 
 * Revision 1.1.6.2  1993/04/05  15:59:32  truitt
 * 	Recreate GAMERA branch.
 * 	[1993/04/05  15:58:38  truitt]
 * 
 * Revision 1.1.6.2  1993/03/19  15:06:43  truitt
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/19  15:06:06  truitt]
 * 
 * Revision 1.1.8.4  1994/05/16  18:51:44  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:17  rhw]
 * 
 * Revision 1.1.8.3  1994/04/28  21:08:18  cbrooks
 * 	DTS Code cleanup
 * 	[1994/04/27  19:41:55  cbrooks]
 * 
 * Revision 1.1.8.2  1994/04/21  22:05:57  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  19:59:41  cbrooks]
 * 
 * Revision 1.1.8.1  1994/01/25  17:16:35  rsalz
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/25  17:15:37  rsalz]
 * 
 * Revision 1.1.6.3  1992/12/30  16:37:46  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:20:10  htf]
 * 
 * Revision 1.1.6.2  1992/09/29  20:43:22  devsrc
 * 	[OT 5373]  SNI/SVR4 Merge.
 * 	[1992/09/14  20:05:56  sekhar]
 * 
 * Revision 1.1.2.3  1992/06/24  16:15:42  grober
 * 	HP/UX Porting Changes
 * 	[1992/06/22  15:54:42  grober]
 * 
 * Revision 1.1.2.2  1992/05/05  21:13:56  comuzzi
 * 	Fixed Big/Little-endian problem. Cleaned up indentation of conditionals.
 * 	[1992/05/01  20:46:32  comuzzi]
 * 
 * Revision 1.1  1992/01/19  15:34:01  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *	Module: CLOCK_ADJUST_ULTRIX.C
 */
/*
 ****************************************************************************
 *									    *
 *  COPYRIGHT (c) 1988,1989,1990 BY					    *
 *  DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASSACHUSETTS.		    *
 *  ALL RIGHTS RESERVED.						    *
 * 									    *
 *  THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND COPIED   *
 *  ONLY IN  ACCORDANCE WITH  THE  TERMS  OF  SUCH  LICENSE  AND WITH THE   *
 *  INCLUSION OF THE ABOVE COPYRIGHT NOTICE. THIS SOFTWARE OR  ANY  OTHER   *
 *  COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY   *
 *  OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE IS  HEREBY   *
 *  TRANSFERRED.							    *
 * 									    *
 *  THE INFORMATION IN THIS SOFTWARE IS  SUBJECT TO CHANGE WITHOUT NOTICE   *
 *  AND  SHOULD  NOT  BE  CONSTRUED AS  A COMMITMENT BY DIGITAL EQUIPMENT   *
 *  CORPORATION.							    *
 * 									    *
 *  DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE  OR  RELIABILITY OF ITS   *
 *  SOFTWARE ON EQUIPMENT WHICH IS NOT SUPPLIED BY DIGITAL.		    *
 * 									    *
 *									    *
 ****************************************************************************
 */

/*
 *(c) Copyright 1991,1992 Siemens-Nixdorf Information Systems,Burlington,MA,USA.
 * All Rights Reserved
 */

/*
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:
 *
 *	This module contains the interfaces to the routines which manipulate
 *	the system's hardware clock.
 *
 * ENVIRONMENT: 	Ultrix 3.0 and above
 *
 * AUTHOR:		Carol Frampton
 *
 * CREATION DATE: 	February 1989
 *
 * MODIFIED BY:         Cesar Cortes for Ultrix implementation
 */

#include <dtss_config.h>

#include <sys/time.h>
#include <limits.h>		/* for max. size of an integer */

#define _DNA_UTIL_H "Don't expand this include file!"

#include <dts.h>
#include <dtss_includes.h>
#include <dtss_service.h>
#include <utctime.h>

extern int utc_adjtime( enum adjmode, union adjunion *);
/*
 * given two UTC values, this macro returns the greater of the two
 */

# define MaxUTC(a,b) (IsUTClt(a, b) ? (a) : (b))

extern ShmBuf *shmPtr;		/* pointer to shared memory which is
				   assumed to be mapped and locked
				   appropriately */
/*
 * Null utc value passed as an argument to routines where it will
 * have no effect.
 */
static UTCValue utcNULL = UTCinit(0x0, 0x0);

/*
 *	Statics
 */
static ExtInacc infiniteInacc = UTCinit ( K_INFINITE_INACC_LO,
						K_INFINITE_INACC_HI );


/*
 * AdjustClock() -
 *
 * Description:
 *
 *	This routine initiates the adjustment of the hardware clock.
 *	It also calculates all variables needed for future inaccuracy
 *	calculations.
 *
 *	The routine assumes the existence of a system service to modify
 *	the clock's ticklength for some specified number of ticks; the
 *	system is expected to return the ticklength to it's nominal value
 *	after the specified number of ticks have elapsed.  For VAX hardware,
 *	the nominal value for the clock's ticklength is 10ms.
 *
 *	Tsync must be <= the current time of the hardware clock for this
 *	algorithm to work correctly.
 *
 *	NOTE:
 *	    THIS ROUTINE ASSUMES SHMEM IS MAPPED AND LOCKED FOR WRITE ACCESS.
 *	    The tdf will be read and the base times will be updated.
 *
 * Inputs:
 *
 *	TsyncPtr - pointer to the UTC time at the synchronization point
 *	IsyncPtr - pointer to the inaccuracy at the synchronization point which
 *		   may be infinite
 *	TcompPtr - pointer to the computed UTC time at the synchronization point
 *	IcompPtr - pointer to the computed inaccuracy at the synchronization
 *		   point which may be infinite
 *
 * Outputs:
 *
 *	none
 *
 * Return Value:
 *
 *	DTSS_SUCESS - clock was adjusted
 *	DTSS_ERROR - clock not adjusted because Tsync > current time -
 *		     probably means a user did a $ set time
 *
 */

StatusBlk AdjustClock (
   UTCValue 	*TsyncPtr,
   ExtInacc 	*IsyncPtr,
   UTCValue 	*TcompPtr,
   ExtInacc 	*IcompPtr
)
{

    union adjunion Adj;
    UTCValue       utcTemp;
    utc_t	   utc, utc2;
    timespec_t	   tdftime;
    long		tdf;
    ExtInacc	        Ibase;
    UTCValue 		elapsedTime, nextLS;
    UTCValue		*nextLSptr, Adjtime;
    int			status;
    StatusBlk		statusBlk;

    /*
     * Assume Success
     */
    statusBlk.status = DTSS_SUCCESS;

    /*
     * Calculate the new base inaccuracy.  For UNIX, the new inaccuracy at the
     * synch point plus the drift between the computed time and the synch point.
     *
     *     Ibase = Icomp + ABS(Tcomp-Tsync);
     *
     * Arithmetic is done starting at the last term, and adding each
     * preceeding term.
     */

    UTCsub (TcompPtr, TsyncPtr, &Adjtime);	/* Tcomp - Tsync */
    UTCabsVal (&Adjtime, &elapsedTime);

    if (! IsInfiniteInacc (IcompPtr))
    {
	UTCadd (IcompPtr, &elapsedTime, &Ibase);
	/*
	 * Check if a leap second maybe occured between Tsync and Tcomp
	 * If it did, increase the inaccuracy by 1 second.
	 */
	nextLSptr = &shmPtr->baseTimes.nextLS;
	if ( ! IsGreaterThanOrEqualNextLS (TsyncPtr, IsyncPtr, nextLSptr) &&
	       IsGreaterThanOrEqualNextLS (TsyncPtr, &Ibase, nextLSptr))
	    AddLSToInacc (&Ibase);

	/*
	 * If the calculated Ibase is greater than 48 bits then
	 * it's infinite by definition.
	 */
	if (IsInfiniteInacc (&Ibase))
	    Ibase = infiniteInacc;
    }

    /*
     * Since Icomp in infinite, Ibase is infinite.
     */
    else
    {
	Ibase = infiniteInacc;
    }

    /*
     * Get the time of the next leap second.  If the current one has
     * expired, calculate a new one.
     */
    CalcNextLS (TsyncPtr, &Ibase, &nextLS);

    /*
     * Convert UTC times into POSIX times (secs and nanosecs)
     */
    utc_mkcomptime(&utc, &Adjtime, &utcNULL, 0L);
    utc_binreltime(&Adj.adjargs.a_adjustment, (timespec_t *)NULL, &utc);

    /*
     * Check the amount of adjustment to be made.  If it exceeds the
     * maximum value (+/- 2000 seconds), set the adjustment to +/- 2000 secs.
     * that will keep it busy for aprox. 27 hours.  When inaccuracies are too
     * large, the clock should be set instead of adjusted, but this strategy
     * prevents the kernel from refusing clock adjustments when (Tcomp - Tsync)
     * is too large.
     */

    if (Adj.adjargs.a_adjustment.tv_sec > 2000)
	Adj.adjargs.a_adjustment.tv_sec = 2000;
    if (Adj.adjargs.a_adjustment.tv_sec < -2000)
	Adj.adjargs.a_adjustment.tv_sec = -2000;

    utc_mkcomptime(&utc, TsyncPtr, &utcNULL, 0L);
    utc_bintime(&Adj.adjargs.a_comptime, (timespec_t *)NULL, (long *)NULL,
		&utc);

    Adj.adjargs.a_adjrate = 100;

#if LONG_BIT >= 64
    Adj.adjargs.a_baseinacc.lo = Ibase;
#else	/* LONG_BIT */
    Adj.adjargs.a_baseinacc.lo = Ibase.lo;
    Adj.adjargs.a_baseinacc.hi = Ibase.hi;
#endif	/* LONG_BIT */

    if (IsInfiniteInacc(&Ibase))
	Adj.adjargs.a_comptime.tv_sec = 0;

#if LONG_BIT >= 64
    Adj.adjargs.a_leaptime.lo = nextLS;
#else   /* LONG_BIT */
    Adj.adjargs.a_leaptime.lo = nextLS.lo;
    Adj.adjargs.a_leaptime.hi = nextLS.hi;
#endif  /* LONG_BIT */


    /*
     * Get the current and next TDF info
     */

    (void) utc_gettime(&utc);
    if (utc_getnextzone(&utc2, &utc))
	BugCheck();
    if (utc_comptime(&utcTemp, (ExtInacc *)NULL, &tdf, &utc))
        BugCheck();
    Adj.adjargs.a_curtdf = tdf / 60;
    if (utc_bintime(&tdftime, (timespec_t *)NULL, &tdf, &utc2))
        BugCheck();
    Adj.adjargs.a_nextdf = tdf / 60;
    Adj.adjargs.a_tdftime = tdftime.tv_sec;

    /*
     * Enter a critical section in the Kernel, to adjust the clock by
     * modifying its tick length.  This value is calculated in the kernel code.
     */
    status = utc_adjtime (adjusttime, &Adj);

    if (status) {
	statusBlk.status = K_SYS_ERROR;
	statusBlk.sysStatus = errno;
    }

    UpdateBaseTimes(TcompPtr,
		    IcompPtr,
		    &nextLS);

    return( statusBlk );
}

/*
 * AdjustClkEnd() -
 *
 * Description:
 *
 *	This routine is used to prematurely terminates the adjustment phase
 *	so that future inaccuracy calculations remain correct.
 *
 * Inputs:
 *
 *	none
 *
 * Outputs:
 *
 *	none
 *
 * Return Value:
 *
 *	none
 *
 */

StatusBlk AdjustClkEnd (void)

{
    UTCValue		Tbase;
    ExtInacc 		Ibase;
    UTCValue 		nextLS;
    StatusBlk           statusBlk;

    /*
     * Assume Success
     */
    statusBlk.status = DTSS_SUCCESS;

    /*
     * End clock adjust by making a call to the kernel code
     */
    if (utc_adjtime (endadjust, (union adjunion *)NULL)) {
	statusBlk.status = K_SYS_ERROR;
	statusBlk.sysStatus = errno;
    }

    /*
     * Get the current time and inaccuracy to compute the time of the next
     * leap second.  If the current one has expired, calculate a new one.
     */

    (void) utc_comptime(&Tbase, &Ibase, (long *)NULL, (utc_t *)NULL);

    CalcNextLS (&Tbase, &Ibase, &nextLS);

    /*
     * Save the base times
     */
    UpdateBaseTimes (
		&Tbase, 		/* (IN) base time, Tbase */
		&Ibase, 		/* (IN) base inaccuracy, Ibase */
		&nextLS);		/* (IN) time of the next leap second */

    return( statusBlk );
}



/*
 * SetClock() -
 *
 * Description:
 *
 *	Set the clock so as to keep future inaccuracy calculations correct.
 *
 *	**** The clock can not be adjusting between the time Tcomp is ****
 *	**** computed and the time the clock is set.  Preceed this    ****
 *      **** with a call to AdjustClkEnd() if the clock is not        ****
 *      **** being set as a result of a sync (for ex., set epoch).    ****
 *
 *
 * Inputs:
 *
 *	TcompPtr - pointer to the new UTC time
 *	IcompPtr - pointer to the new inaccuracy at time, Tcomp, which may be
 *		   infinite
 *	TsyncPtr - pointer to the value of the clock at the point which Tcomp
 *		   applies
 *
 * Outputs:
 *
 *	none
 *
 * Return Value:
 *
 *	none
 *
 */

StatusBlk SetClock (
    UTCValue 	*TcompPtr,
    ExtInacc 	*IcompPtr,
    UTCValue 	*TsyncPtr,
    int		*newTDFptr		/* used by vms only */
)
{

    utc_t	utc, utc2;
    timespec_t  tdftime;
    long	tdf;
    UTCValue 	nextLS, *TbasePtr, AdjTime;
    UTCValue    utcTemp;
    union	adjunion Adj;
    StatusBlk   statusBlk;

    /*
     * Assume Success
     */
    statusBlk.status = DTSS_SUCCESS;

    TbasePtr = TcompPtr;
    UTCsub (TcompPtr, TsyncPtr, &AdjTime);	/* Tcomp - Tsync */

    /*
     * Get the time of the next leap second.  If the current one has
     * expired, calculate a new one.
     */
    CalcNextLS (TbasePtr, IcompPtr, &nextLS);

    /*
     * Convert UTC times into POSIX times (secs and nanosecs)
     */
    utc_mkcomptime(&utc, &AdjTime, &utcNULL, 0L);
    utc_binreltime(&Adj.adjargs.a_adjustment, (timespec_t *)NULL, &utc);

    utc_mkcomptime(&utc, TcompPtr, &utcNULL, 0L);
    utc_bintime(&Adj.adjargs.a_comptime, (timespec_t *)NULL, (long *)NULL,
	        &utc);

    Adj.adjargs.a_adjrate = 100;

#if LONG_BIT >= 64
    Adj.adjargs.a_baseinacc.lo = *IcompPtr;
#else   /* LONG_BIT */
    Adj.adjargs.a_baseinacc.lo = IcompPtr->lo;
    Adj.adjargs.a_baseinacc.hi = IcompPtr->hi;
#endif  /* LONG_BIT */


    if (IsInfiniteInacc(IcompPtr))
	Adj.adjargs.a_comptime.tv_sec = 0;

#if LONG_BIT >= 64
    Adj.adjargs.a_leaptime.lo = nextLS;
#else   /* LONG_BIT */
    Adj.adjargs.a_leaptime.lo = nextLS.lo;
    Adj.adjargs.a_leaptime.hi = nextLS.hi;
#endif  /* LONG_BIT */


    /*
     * Get the current and next TDF info
     */

    (void) utc_gettime(&utc);
    if (utc_getnextzone(&utc2, &utc))
	BugCheck();
    if (utc_comptime(&utcTemp, (ExtInacc *)NULL, &tdf, &utc))
        BugCheck();
    Adj.adjargs.a_curtdf = tdf / 60;
    if (utc_bintime(&tdftime, (timespec_t *)NULL, &tdf, &utc2))
        BugCheck();
    Adj.adjargs.a_nextdf = tdf / 60;
    Adj.adjargs.a_tdftime = tdftime.tv_sec;

    /*
     * Enter a critical section in the Kernel, to adjust the clock by setting
     * it.
     */
    if (utc_adjtime (settime, &Adj)) {
	statusBlk.status = K_SYS_ERROR;
	statusBlk.sysStatus = errno;
    }

    /*
     * If the computed inaccuracy is infinite, make that only the
     * least significant 48 bits are set.
     */
    if (IsInfiniteInacc (IcompPtr)){
	 *IcompPtr = infiniteInacc;
     }

   /*
    * Update the absolute timers maintained in the state engine
    * by the amount we have just altered the system time.
    */
   UpdateStateTimers(&AdjTime);

   /*
    * Update the base times
    */
    UpdateBaseTimes (
		TbasePtr, 		/* (IN) base time, Tbase */
		IcompPtr, 		/* (IN) base inaccuracy (Computed Inacc.)*/
		&nextLS);		/* (IN) time of the next leap second */

    return (statusBlk);
}

/*
 * ReadHWclkParams -
 *
 * Description:
 *
 *     Read the hardward clock parameters drift and resolution.
 *
 * Inputs:
 *
 *	none
 *
 * Outputs:
 *
 *     clockResolution -  the resolution of the clock in nano seconds
 *     clockDrift - the maximum clock drift in parts per billion (10**9).
 *
 * Return Value:
 *
 *	none
 *
 */
StatusBlk ReadHWclkParams(
    unsigned long *clockResolution,
    unsigned long *clockDrift
)
{
    union adjunion Adj;
    StatusBlk statusBlk;

    if (utc_adjtime (getresolution, &Adj)) {
	statusBlk.status = K_SYS_ERROR;
	statusBlk.sysStatus = errno;
	return( statusBlk );
    }

    *clockResolution = Adj.resolution;           /* in NS */

    if (utc_adjtime (getdrift, &Adj)) {
	statusBlk.status = K_SYS_ERROR;
	statusBlk.sysStatus = errno;
	return( statusBlk );
    }

    *clockDrift = K_ONE_BILLION / (unsigned long int)Adj.maxdrift;      /* in PPB */

    ReturnDTSSstatus(DTSS_SUCCESS, statusBlk);
}
