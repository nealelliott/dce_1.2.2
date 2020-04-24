/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: kern_utctime.c,v $
 * Revision 1.1.6.2  1996/02/17  23:34:58  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:09  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:10:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:24:39  root]
 * 
 * Revision 1.1.4.3  1993/01/22  19:35:06  tom
 * 	Bug 6862 - Validate the value of tod.tv_nsec *before* calling ksettimer.
 * 	[1993/01/22  19:34:22  tom]
 * 
 * Revision 1.1.4.2  1992/12/30  16:36:07  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:15:27  htf]
 * 
 * Revision 1.1.2.3  1992/05/06  21:54:48  comuzzi
 * 	Fix defects 2295, 3143 and 3256.
 * 	Fix big-endian constant and return EPERM if no priv.
 * 	[1992/05/06  21:29:31  comuzzi]
 * 
 * Revision 1.1.2.2  1992/04/27  18:44:48  bmw
 * 	removed all program markings "jel 06-21-91"
 * 	[1992/04/27  18:41:34  bmw]
 * 
 * Revision 1.1  1992/01/19  15:33:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *
 * Copyright (c) 1989,1990,1991 by
 * Digital Equipment Corporation, Maynard, Mass.
 *
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * This software is derived from software received  from the  University
 * of California,  Berkeley.  Use, duplication, or disclosure is subject 
 * to  restrictions   under  license   agreements  with   University  of
 * California.
 *
 * Redistribution and use in source and binary forms are permitted provided
 * that: (1) source distributions retain this entire copyright notice and
 * comment, and (2) distributions including binaries display the following
 * acknowledgement:  ``This product includes software developed by the
 * University of California, Berkeley and its contributors'' in the
 * documentation or other materials provided with the distribution and in
 * all advertising materials mentioning features or use of this software.
 * Neither the name of the University nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 */

/* ---------------------------------------------------------------------
 * Modification History 
 *
 * ---------------------------------------------------------------------
 *
 * 18-Jan-90   Joe Comuzzi
 *     Added functions to return maximum drift rate and resolution.
 *
 * 30-Mar-90   Joe Comuzzi
 *     Moved 64 bit macros to include file, and changed external interface
 *     to nanoseconds, eliminated type-puns in external interface.
 *
 *  6-Apr-90   Joe Comuzzi
 *     Fixed Daylight time bug.
 *
 * 13-Aug-90   Joe Comuzzi
 *     Fixed race in setting maxdriftrecip
 *
 *  4-Apr-91   Brian Bailey
 *     Adapted for Reno.
 */


#include <sys/param.h>
#include <sys/time.h>
#include <sys/user.h>


#include <sys/machine.h>
#include <sys/types.h>
#include <sys/intr.h>
#include <rtc.h>
#include <sys/errno.h>
#include <sys/syspest.h>
#include <utctime.h>

/* 
 * Get UTC Time of day and adjust clock routines.
 *
 * These routines provide the kernel entry points to get and set
 * utc timestamps, used by DECdts - the time syncronization service.
 */

/*
 * MAXDRIFTRECIP
 *
 * This constant is the reciprocal of the maximum drift rate of the hardware
 * clock.  We use the reciprocal, rather than the maximum drift rate itself,
 * because it allows for integer calculations without overflow.
 *
 * Change this constant as needed for your cpu clock's drift rate.  Below
 * are values for some processors:
 *
 *	CPU type		MAXDRIFTRECIP value
 *	--------		-------------------
 *
 *	vax			10000
 *	mips			20000
 *
 */
#define	MAXDRIFTRECIP	20000		/* for a RS/6000 clock */

/*
 * Variables shared with other kernel modules
 */

int inaccinfinite = 1;			/* Set in kern_time.c when old
                                           interface is used */


/*
 * Variables defined and used by other kernel modules
 */

int tickdelta;			/* Amount to change tick by each 
					   tick - used by kern_clock.c */
long timedelta;			/* Amount of time remaining to be
					   adjusted - used by kern_clock.c */
static struct int64 diff_base_times = {  
    0x01b21dd2, 0x13814000		
};

/*
 * Variables used in inaccuracy calculation
 */

static struct int64 inacc;		/* Base inaccuracy */
static long int tickadjcount;		/* Number of ticks for which
					   adjustment will occur */
static struct timeval driftbase;	/* Time at which last base
					   inaccuracy was calculated */
static struct timeval adjbase;		/* Time at which last adjustment
					   was started */
static struct int64 leaptime;		/* Time of next leap second
					   after driftbase + inacc */
static long int currentmdr = 0;		/* Current maxdriftrecip to use */
static long int nextmdr = 0;		/* maxdriftrecip to use after next
					   clock adjustment */

int tick = 1000000 / HZ;                /* # of uS per tick */

/*
 * Variables which have the current and next TDF info
 */

#define NO_TDF	8192			/* Used as out-of-band value to
                                           indicate not yet initialized */
static int curtdf = NO_TDF;		/* Current time differential factor */
static int nextdf = NO_TDF;		/* Next time differential factor */
static int tdftime = 0x7fffffff;	/* Time of next TDF change */


/*
 * Routine to compute the number of ticks which have occured since
 * the last time an adjustment was started.
 */

static long int baseticks(curtime, basetime)

struct timeval *curtime;	/* Current time */
struct timeval *basetime;	/* Time last adjustment was started */

{
	struct timeval elapsed;	/* Elapsed time since last adjustment was
				   started */

	elapsed.tv_sec = curtime->tv_sec - basetime->tv_sec;
	elapsed.tv_usec = curtime->tv_usec - basetime->tv_usec;

	/* Calculate the number of ticks, if we must approximate
	   round down so we'll overestimate the inaccuracy */

	/* If it won't overflow, compute using micro-seconds */
	if (elapsed.tv_sec < 4000)
		return(((unsigned)elapsed.tv_sec * K_US_PER_SEC +
			elapsed.tv_usec) / (tick + tickdelta));
/* Change the previous lines if kernel is nano-second based.
 *		return(((unsigned)elapsed.tv_sec * (K_NS_PER_SEC / 1000) +
 *			elapsed.tv_nsec / 1000) /
 *		       ((tick + tickdelta + 999) / 1000));
 */
	/* If it won't overflow, compute using milli-seconds */

	if (elapsed.tv_sec < 4000000) {
 		return(((unsigned)elapsed.tv_sec * (K_US_PER_SEC / 1000) +
			elapsed.tv_usec / 1000) /
		       ((tick + tickdelta + 999) / 1000));
/* Change the previous lines if kernel is nano-second based
 *		return(((unsigned)elapsed.tv_sec * (K_NS_PER_SEC / 1000000) +
 *			elapsed.tv_nsec / 1000000) /
 *		       ((tick + tickdelta + 999999) / 1000000));
 */
	} else {
		/* Compute using seconds */

		struct int64 temp;

		UTC_MUL32P(K_US_PER_SEC / (tick + tickdelta),
/* Change the previous line if kernel is nano-second based
 *		UTC_MUL32P(K_NS_PER_SEC / (tick + tickdelta), */
		       elapsed.tv_sec, &temp);

		if (temp.hi == 0 && (long)temp.lo > 0)
			return(temp.lo);
	}

	return(0x7fffffff);
}

/*
 * Routine to calculate the drift and resolution contribution
 * to inaccuracy.
 *
 *	Return drift in 100ns units, if infinite (or just too large) return
 *	negative.
 */

static long int driftfactor(curtv, driftbase, mdr)

struct timeval *curtv;		/* Current time */
struct timeval *driftbase;	/* Last time base inaccuracy was
				   calculated */
long int mdr;			/* Value of maxdriftrecip to use */

{
	struct timeval	elapsed;	/* elapsed time since last time
					   base inaccuracy was
					   calculated */
	register long int drift;	/* Contribution to inaccuracy of
					   drift and resolution */

	/* Compute the amount of inaccuracy since the last base time by
	   computing the elapsed time, and scaling by the max drift rate */

	elapsed.tv_sec = curtv->tv_sec - driftbase->tv_sec;
	elapsed.tv_usec = curtv->tv_usec - driftbase->tv_usec +
			  tick;

	/* If drift computation won't overflow, compute using micro-seconds */

	if (elapsed.tv_sec < 2000) {
		drift = (elapsed.tv_sec * K_US_PER_SEC +
			 elapsed.tv_usec +
			 mdr - 1) /
			mdr * (K_100NS_PER_SEC / K_US_PER_SEC);
/* Change the previous lines if kernel is nano-second based
 *		drift = (elapsed.tv_sec * (K_NS_PER_SEC / 1000) +
 *			 (elapsed.tv_nsec + 999) / 1000 +
 *			 mdr - 1) /
 *			mdr * (K_100NS_PER_SEC / (K_NS_PER_SEC / 1000));
 */
	} else {
		/* If drift computation won't overflow compute using
		   milli-seconds */

		if (elapsed.tv_sec < 2000000) {
			drift = ((elapsed.tv_sec * (K_US_PER_SEC / 1000) +
				  (elapsed.tv_usec + 999) / 1000 +
				  mdr - 1) / mdr) *
				 (1000 * (K_100NS_PER_SEC / K_US_PER_SEC));
/* Change the previous lines if kernel is nano-second based
 *			drift = ((elapsed.tv_sec * (K_NS_PER_SEC / 1000000) +
 *			  	  (elapsed.tv_nsec + 999999) / 1000000 +
 *				  mdr - 1) / mdr) *
 *				 (1000 * (K_100NS_PER_SEC / (K_NS_PER_SEC / 1000)));
 */
		} else {
			return(-1);
		}
	}

	/* Add in resolution */

	drift += tick * (K_100NS_PER_SEC/K_US_PER_SEC);
/* Change the previous line if kernel is nano-second based
 *	drift += (tick + (K_NS_PER_SEC/K_100NS_PER_SEC) - 1)/
 *               (K_NS_PER_SEC/K_100NS_PER_SEC);
 */

	return(drift);
}

/*
 * Kernel mode get utc timestamp routine
 */

kutc_gettime(register struct utc *user_utc)
{
	struct int64	atime;		/* Current time */
	struct int64	saveinacc;	/* Last base inaccuracy */
	struct int64	saveleaptime;	/* Time of next potential leap
					   second correction */
	struct timeval	savedriftbase;	/* Time of last base inaccuracy */
	struct timeval	atv;		/* Current time as a timeval */
        int             savetdf;	/* Current time differential factor */
	struct int64	hightime;	/* high end of current time interval */
	struct int64	correc;		/* amount to reduce inaccuracy
					   due to current adjustment */
	int		saveinfinite;	/* Inaccuracy is currently
					   infinite */
	long int	savetickadjcnt;	/* Current number of ticks for
					   which adjustment will occur */
	long int	saveticks;	/* Count of ticks since last
					   adjust started */
	long int	savemdr;	/* Current maxdriftrecip to use
					   for drift calculation */
	long int	drift;		/* Drift contribution to
					   inaccuracy */
	int		s;		/* Temp for saving IPL */
        int             dummy;          /* space for unwanted return parms */

	struct utc utc;
	int status;

        s = i_disable(INTMAX);
        atv.tv_sec = tod.tv_sec;
        atv.tv_usec = tod.tv_nsec / 1000;
        savetdf = (atv.tv_sec > tdftime) ? nextdf : curtdf;
        savedriftbase = driftbase;
        saveinacc = inacc;
        kgettickd(&dummy, &dummy, &inaccinfinite);
        saveinfinite = inaccinfinite;
        saveleaptime = leaptime;
        savetickadjcnt = tickadjcount;
        saveticks = baseticks(&atv, &adjbase);
        savemdr = currentmdr;
        i_enable(s);

	/* Convert the time in seconds and micro-seconds to a 64-bit integer
	   number of 100 nanoseconds. */

	UTC_MUL32P(atv.tv_sec, K_100NS_PER_SEC, &atime);
	UTC_ADD32P(atv.tv_usec * (K_100NS_PER_SEC / K_US_PER_SEC), &atime);
/* Change the previous line if kernel is nano-second based
 *	UTC_ADD32P(atv.tv_nsec / (K_NS_PER_SEC / K_100NS_PER_SEC), &atime);
 */

	/* Compensate for different base times */

	UTC_ADD64(&diff_base_times, &atime);

	/* Compute drift and resolution contribution to inaccuracy */

	drift = driftfactor(&atv, &savedriftbase,
			    (savemdr ? savemdr : MAXDRIFTRECIP));

	/* If not infinite, add in the drift contribution and reduce
	   the inaccuracy by the adjustment amount. Finally check for
	   leap seconds */

	if (drift >= 0 && !saveinfinite) {

		UTC_ADD32P(drift, &saveinacc);

		/* Calculate the amount to reduce inaccuracy from an
		   adjustment */

		if (savetickadjcnt != 0) {
			UTC_MUL32P((saveticks < savetickadjcnt) ?
				    saveticks : savetickadjcnt,
			           tickdelta * (K_100NS_PER_SEC / K_US_PER_SEC),
/* Change the previous line if kernel is nano-second based
 *			           tickdelta / (K_NS_PER_SEC / K_100NS_PER_SEC),
 */
			       &correc);

			UTC_SUB64(&correc, &saveinacc);
		}

		/* Compute upper end of interval */

		hightime = atime;
		UTC_ADD64(&saveinacc, &hightime);

		/* If upper end of interval is past a potential leap
		   second, add an extra second of inaccuracy */

		if ((hightime.hi > saveleaptime.hi) ||
		    (hightime.hi == saveleaptime.hi &&
		     hightime.lo >= saveleaptime.lo))
			UTC_ADD32P(K_100NS_PER_SEC, &saveinacc);

	}

	/* If inaccuracy has become infinite, just return
	   max value */

	if (drift < 0 || saveinfinite || saveinacc.hi & 0xffff0000) {
		saveinacc.lo = 0xffffffff;
		saveinacc.hi = 0xffff;
	}

        if (savetdf == NO_TDF)
            savetdf = 0;           /* -tz.tz_minuteswest; */
        utc.time.hi = atime.hi;
        utc.time.lo = atime.lo;
        utc.inacchi = saveinacc.hi;
        utc.inaccmid = saveinacc.lo >> 16;
        utc.inacclo = saveinacc.lo;
        utc.tdfhi = (savetdf & TDFHI_MASK) >> TDFHI_SHIFT;
        utc.flags = (((savetdf & TDFLO_MASK) >> TDFLO_SHIFT) <<
                                UTC_TDFLO_POS) |
                     (K_UTC_VERSION << UTC_VERSION_POS) |
                     (1 << UTC_ENDIAN_POS);

status = copyout((caddr_t)&utc, (caddr_t)(user_utc), sizeof(utc));
return(status);
}

/*
 * User mode get utc timestamp routine
 *
 *	call the kernel routine and copy the results out.
 */


/* set/adjust utc timestamp routine */
kutc_adjtime(register enum adjmode modeflag, register union adjunion *argblk)

/*
		                        	  Set/adjust flag:
						    0 - Set time
						    1 - Adjust time
						    2 - End adjustment
						    3 = Get clock resolution
						    4 = Get clock max drift
						    5 = Trim clock frequency
						    6 = Get trimmed frequency 
		                       		  Pointer to argument block
						  for sub-function:
						    0 - *adjargs union member
						    1 - *adjargs union member
						    2 - IGNORED
						    3 - *resolution
						    4 - *maxdriftrecip
						    5 - *trimargs union member
						    6 - *frequency 
*/
							
{
	struct adjargs aargs;			/* Copy of adj arguments */
	struct trimargs targs;			/* Copy of trim arguments */
	/*
	 * For function 0 and 1:
	 *	struct timespec	a_adjustment; 	Amount to adjust or change
	 *	struct timespec	a_comptime;	Time which corresponds to base
	 *					inaccuracy
	 *	struct int64	a_baseinacc;	Base inaccuracy
	 *	struct int64	a_leaptime;	Time of next potential leap
	 *					second
	 *	long int	a_adjrate;	Rate at which to adjust
	 *					    1000 = .1% (.0768% on PMAX)
	 *					    100 = 1% (.999% on PMAX)
	 *					    10 = 10%, etc.
	 *					    Ignored for set time
	 *	long int	a_curtdf;	Current timezone
	 *	long int	a_nextdf;	Next timezone (Daylight time)
	 *	long int	a_tdftime;	Time of next timezone change
	 * For function 3:
	 *	long int	resolution;	Resolution of clock in nanosecs
	 * For function 4:
	 *	long int	maxdrift;	Maximun drift rate of clock
	 * For function 5:
	 *	long int	t_frequency;	New frequency trim of clock
	 *	long int	t_maxdrift;	New maximun drift rate of clock
	 * For function 6:
	 *	long int	frequency;	Current frequency trim of clock
	 */

        struct timestruc_t atmp;        /* Temporary time value */

	struct timeval atv;		/* Temporary time value */
	struct timezone atz;		/* New timezone */
	long resolution;                /* Resolution of clock */
	long frequency;			/* frequency trim of clock */
	long ndelta;			/* New delta value */
	int s;				/* Temp for saving IPL */
	int newinfinite = 0;		/* Flag for new inaccinfinite */
	long int n;			/* Random temp */
	int error;

	/*
	 * Validate arguments
	 */

	switch (modeflag) {

	    case settime:
	    case adjusttime:

		if (error = copyin((caddr_t)(argblk),
				   (caddr_t)&aargs, sizeof (aargs)))
			return (error);
		if (aargs.a_adjrate == 0)
			aargs.a_adjrate = 100;	/* Default to 1% */
		if (aargs.a_adjrate < 0 ||
		    aargs.a_adjrate > tick/2 ) {
			return (EINVAL);
		}
		if (aargs.a_adjustment.tv_nsec > K_NS_PER_SEC ||
		    aargs.a_adjustment.tv_nsec < -K_NS_PER_SEC) {
			return (EINVAL);
		}
		if (aargs.a_adjustment.tv_nsec < 0) {
			aargs.a_adjustment.tv_nsec += K_NS_PER_SEC;
			aargs.a_adjustment.tv_sec--;
		}
		if (modeflag == adjusttime &&
				     (aargs.a_adjustment.tv_sec > 2000 ||
				      aargs.a_adjustment.tv_sec < -2000)) {
			return (EINVAL);
		}
		if (aargs.a_comptime.tv_sec == 0) {
			newinfinite = 1;
		}
		break;

	    case endadjust:

		break;

	    case getresolution:

		resolution = tick * (K_NS_PER_SEC/K_US_PER_SEC);
/* Change the previous line if kernel is nano-second based
 *              resolution = tick;
 */
		error = copyout((caddr_t)&resolution, 
				(caddr_t)(argblk),
				sizeof (long));
		return (error);

	    case getdrift:

		if (nextmdr == 0)
			nextmdr = MAXDRIFTRECIP;
		error = copyout((caddr_t)&nextmdr,
				(caddr_t)(argblk),
				sizeof (long));
		return (error);

	    case setfreq:

		if (error = copyin((caddr_t)(argblk),
				   (caddr_t)&targs, sizeof (targs)))
			return (error);
		/* Maximum drift rate must be less than 1%! */
		if (targs.t_maxdrift <= 100) {
			return (EINVAL);
		}
		break;

	    case getfreq:

		frequency = tick * hz *
			    (K_NS_PER_SEC/K_US_PER_SEC);
/* Change the previous lines if kernel is nano-second based
 *              frequency = tick * hz;
 */

		error = copyout((caddr_t)&frequency,
				(caddr_t)(argblk),
				sizeof (long));
		return (error);

	    default:

		return (EINVAL);

	}

	/*
	 * Validate privleges
	 */

        if (privcheck(SYS_CONFIG) == EPERM)
		return (EPERM);

	/*
         * The following code must all be locked against clock ticks. This
         * is achieved by raising IPL to the clock IPL. If timekeeping changes
	 * in some grand way, this strategy will need to be changed!
         */

        s = i_disable(INTMAX);

	switch (modeflag) {
	    
	    case settime:	/* Set time */

                tod.tv_sec += aargs.a_adjustment.tv_sec;
                tod.tv_nsec += aargs.a_adjustment.tv_nsec;
		if (tod.tv_nsec < 0) {
			tod.tv_nsec = 0;
		}
                if (tod.tv_nsec >= K_NS_PER_SEC) {
			tod.tv_sec += tod.tv_nsec / K_NS_PER_SEC;
			tod.tv_nsec = tod.tv_nsec % K_NS_PER_SEC;
		}
                atmp.tv_sec = tod.tv_sec;
                atmp.tv_nsec = tod.tv_nsec;
                ksettimer(&atmp); /* set time of day */
		tickdelta = 0;
		timedelta = 0;
                ksettickd(&timedelta, &tickdelta, (int *) NULL);
                adjbase.tv_sec = tod.tv_sec;
                adjbase.tv_usec = tod.tv_nsec / 1000;

		driftbase.tv_sec = aargs.a_comptime.tv_sec;
		driftbase.tv_usec = aargs.a_comptime.tv_nsec /
                                    ( K_NS_PER_SEC / K_US_PER_SEC );
		inacc = aargs.a_baseinacc;
		leaptime = aargs.a_leaptime;
		tickadjcount = 0;
		inaccinfinite = newinfinite;

                ksettickd((int *) NULL, (int *) NULL, &inaccinfinite);

		curtdf = aargs.a_curtdf;
		nextdf = aargs.a_nextdf;
		tdftime = aargs.a_tdftime;
		currentmdr = nextmdr;
		break;
		
	    case adjusttime:	/* Adjust time */

/* If kernel is nano-second based, the variables timedelta and tickdelta
 * will probably be changed. A simple change of units will not suffice, since
 * then the range will be too small. This code will need to be reworked.
 * (As will all the code that supports adjtime) */

		ndelta = aargs.a_adjustment.tv_sec * K_US_PER_SEC +
			 aargs.a_adjustment.tv_nsec /
                         ( K_NS_PER_SEC / K_US_PER_SEC );

		tickdelta = tick / aargs.a_adjrate;
		if (ndelta % tickdelta)
			ndelta = ndelta / tickdelta * tickdelta;
		timedelta = ndelta;

                ksettickd(&timedelta, &tickdelta, (int *) NULL);

                adjbase.tv_sec = tod.tv_sec;
                adjbase.tv_usec = tod.tv_nsec / 1000;

		driftbase.tv_sec = aargs.a_comptime.tv_sec;
		driftbase.tv_usec = aargs.a_comptime.tv_nsec /
                                    ( K_NS_PER_SEC / K_US_PER_SEC );
		inacc = aargs.a_baseinacc;
		leaptime = aargs.a_leaptime;
		tickadjcount = ((ndelta >= 0) ? ndelta : -ndelta) / tickdelta;
		inaccinfinite = newinfinite;

                ksettickd((int *) NULL, (int *) NULL, &inaccinfinite);

		curtdf = aargs.a_curtdf;
		nextdf = aargs.a_nextdf;
		tdftime = aargs.a_tdftime;
		currentmdr = nextmdr;
		break;

	    case endadjust:	/* End adjustment */

		timedelta = 0;

                ksettickd(&timedelta, &tickdelta, (int *) NULL);

                atv.tv_sec = tod.tv_sec;
                atv.tv_usec = tod.tv_nsec / 1000;
                n = baseticks(&atv, &adjbase);

		if (n < tickadjcount)
			tickadjcount = n;
		break;

	    case setfreq:	/* Trim frequency of clock */

		/*
		 * NOTE:  The following code is not currently used by DTS.  It
		 * is here as a placeholder for when DTS supports frequency
		 * synchronization.  When DTS does support it, this code will
		 * need to be extended because the integer calculations below
		 * only allow the frequency to be adjusted by 1 part in 10000
		 * for a 100 hz clock and by an even greater amount for faster
		 * clocks.  This value doesn't provide fine enough granularity
		 * to properly synchronize the frequencies.
		 */


		/* The units of frequency are nanoseconds/second, we compute
                   a new tick so that after hz ticks, the
                   clock has advanced targs.t_frequency nanoseconds! */
		/* For micro-second version we compute using nanoseconds and
                   round to get as close as we can */

		tick = (targs.t_frequency + hz*(K_NS_PER_SEC/K_US_PER_SEC/2))
		        / hz / (K_NS_PER_SEC/K_US_PER_SEC);
/* If kernel is nanosecond-based, replace preceeding two statements
 * with:
 *		tick = (targs.t_frequency + (hz/2)) / hz;
 */
		/* Set the new current and next maxdriftrecip. The nextmdr
		   is set to the new value, unless zero is specified - then
		   it is reverted to the hardware's default. currentmdr is
		   set to the minimum of its current value and the new
		   value. Remember you divide by mdr to get the drift, so
		   we overestimate the drift by using the minimum. */
		if ((nextmdr = targs.t_maxdrift) == 0)
			nextmdr = MAXDRIFTRECIP;
		if (currentmdr == 0)
			currentmdr = MAXDRIFTRECIP;
		if (currentmdr > nextmdr)
			currentmdr = nextmdr;

		
	}

        i_enable(s);

return (0);
}

/* config(), unconfig(), and config_dts_kern() are invoked by config_kern_ext
 * (via sysconfig) to pin or unpin the appropriate text and data used by the
 * dts kernel extensions 
*/

/* CONFIGURE dts kernel extension */

static void config()
{
int  interrupt_level,
     dummy;

/* pin data memory */

if (
    pin(&inaccinfinite, sizeof(int)) |
    pin(&tickdelta, sizeof(int)) |
    pin(&timedelta, sizeof(int)) |
    pin(&diff_base_times, sizeof(struct int64)) |
    pin(&inacc, sizeof(struct int64)) |
    pin(&tickadjcount, sizeof(long int)) |
    pin(&driftbase, sizeof(struct timeval)) |
    pin(&adjbase, sizeof(struct timeval)) |
    pin(&leaptime, sizeof(struct int64)) |
    pin(&currentmdr, sizeof(int)) |
    pin(&nextmdr, sizeof(int)) |
    pin(&tick, sizeof(int)) |
    pin(&curtdf, sizeof(int)) |
    pin(&nextdf, sizeof(int)) |
    pin(&tdftime, sizeof(int)) 
   ) 
  panic("panic:  failed pinning DTS kernel extension data memory");

/* pin text memory */

if (
    pincode(baseticks) |
    pincode(driftfactor) |
    pincode(kutc_gettime) |
    pincode(kutc_adjtime)
   )
  panic("panic:  failed pinning DTS kernel extension text memory");

interrupt_level = i_disable(INTMAX);
timedelta = tickdelta = 0;
ksettickd(&timedelta, &tickdelta, NULL);
i_enable(interrupt_level); 
}


/* UNCONFIGURE dts kernel extension */

static void unconfig()
{
/* unpin data memory */

if (
    unpin(&inaccinfinite, sizeof(int)) |
    unpin(&tickdelta, sizeof(int)) |
    unpin(&timedelta, sizeof(int)) |
    unpin(&diff_base_times, sizeof(struct int64)) |
    unpin(&inacc, sizeof(struct int64)) |
    unpin(&tickadjcount, sizeof(long int)) |
    unpin(&driftbase, sizeof(struct timeval)) |
    unpin(&adjbase, sizeof(struct timeval)) |
    unpin(&leaptime, sizeof(struct int64)) |
    unpin(&currentmdr, sizeof(int)) |
    unpin(&nextmdr, sizeof(int)) |
    unpin(&tick, sizeof(int)) |
    unpin(&curtdf, sizeof(int)) |
    unpin(&nextdf, sizeof(int)) |
    unpin(&tdftime, sizeof(int))
   )
  panic("panic:  failed unpinning DTS kernel extension data memory");

/* unpin text memory */

if (
    unpincode(baseticks) |
    unpincode(driftfactor) |
    unpincode(kutc_gettime) |
    unpincode(kutc_adjtime)
   )
  panic("panic:  failed unpinning DTS kernel extension text memory");
}

/*
This module is invoked by "sysconfig" to configure the kernel extension 
when it is loaded and to unconfigure it when it is unloaded.
*/
#define CONFIG 0
#define UNCONFIG 1

int config_dts_kern(cmd, uiop)
int cmd;                         /* CONFIG or UNCONFIG */
struct uio *uiop;                /* Pointer to other junk(Not used)   */
{
int status;

/* validate uiop parameter */
if (uiop)                        /* expecting no uio parm */
  return(-1);                    /* invalid SYSCMFKMOD call from sysconfig */  

status = 0;
  /* select command */
  switch (cmd)
    {
    case CONFIG:                   
      config();                   /* configure kernel extension   */
      break;   
    case UNCONFIG:
      unconfig();                 /* unconfigure kernel extension */
      break; 
    default:     
      status = -2;                /* invalid cmd */
      break;
    }
return(status);
}

