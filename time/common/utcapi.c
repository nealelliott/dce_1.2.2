/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 *(c) Copyright 1991,1992 Siemens-Nixdorf Information Systems,Burlington,MA,USA.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: utcapi.c,v $
 * Revision 1.1.17.2  1996/02/17  23:34:39  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:47  marty]
 *
 * Revision 1.1.17.1  1995/12/08  18:08:36  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/9  1995/06/05  15:18 UTC  mullan_s
 * 	Merge CHFts15134
 * 
 * 	HP revision /main/HPDCE02/mullan_mothra_bin_compat5/1  1995/06/01  18:23 UTC  mullan_s
 * 	Merge utc_gettime_noshm.
 * 
 * 	HP revision /main/HPDCE02/pare_mothra/3  1995/05/31  17:24 UTC  pare
 * 	Add function utc_gettime_noshm
 * 
 * 	HP revision /main/HPDCE02/pare_mothra/2  1994/11/23  19:58 UTC  pare
 * 	Specify inaccuracy value of -1 in utc_getTCPtime()
 * 
 * 	HP revision /main/HPDCE02/6  1994/11/15  19:07 UTC  pare
 * 	Merge changes
 * 
 * 	HP revision /main/HPDCE02/pare_mothra/1  1994/11/15  19:04 UTC  pare
 * 	Add utc_getTCPtime() function.
 * 
 * 	HP revision /main/HPDCE02/5  1994/09/27  17:28 UTC  pare
 * 	Fix bl8 merge problem
 * 
 * 	HP revision /main/HPDCE02/4  1994/09/26  13:37 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:24:07  root]
 * 
 * Revision 1.1.15.10  1994/09/23  20:00:05  tom
 * 	Back out change of unsigned/signed64 to typedef hyper
 * 	[1994/09/23  19:26:50  tom]
 * 
 * Revision 1.1.15.8  1994/09/21  22:01:48  tom
 * 	Remove whitespace in from of SWAP for 64 bit.
 * 	[1994/09/21  21:14:28  tom]
 * 
 * Revision 1.1.15.7  1994/08/31  14:23:23  cbrooks
 * 	CR11964 - test_kernel fails
 * 	[1994/08/31  14:22:39  cbrooks]
 * 
 * Revision 1.1.15.6  1994/08/23  20:32:05  cbrooks
 * 	Code Cleanup Checkpoint
 * 	[1994/08/23  13:56:51  cbrooks]
 * 
 * 	CR8692 utc_abstime() returns incorrect time for neg value
 * 	[1994/08/22  19:07:44  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:52:39  cbrooks]
 * 
 * Revision 1.1.15.5  1994/08/16  18:12:53  cbrooks
 * 	CR 11494
 * 	[1994/08/09  19:00:03  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/3  1994/09/02  18:34 UTC  pare
 * 	Merge changes to HPDCE02
 * 
 * 	HP revision /main/HPDCE02/pare_mothra/1  1994/09/02  18:32 UTC  pare
 * 	Remove reference to fabs() and add intrenal routine utc_fabs()
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/18  14:45 UTC  pare
 * 	Merge changes from Hpdce02
 * 
 * Revision 1.1.15.4  1994/08/02  19:05:25  cbrooks
 * 	CR11514 - change assert(...) to dce_assert(...)
 * 	[1994/08/02  14:56:38  cbrooks]
 * 
 * Revision 1.1.15.3  1994/07/29  18:23:47  cbrooks
 * 	Merged with changes from 1.1.15.2
 * 	[1994/07/29  18:23:33  cbrooks]
 * 
 * 	CR9380 - utc_mkcomptime() will return 0
 * 	[1994/07/29  18:20:20  cbrooks]
 * 
 * Revision 1.1.15.2  1994/06/29  21:00:08  ohara
 * 	need tzfile.h for SECS_PER_DAY .. but creates redefs with commondef.h
 * 	which isn't need anyway, so removed it.
 * 	[1994/06/09  17:14:35  ohara]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  20:40 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	HP revision /main/HPDCE01/1  1994/02/01  15:01  pare
 * 	merge kk and hpdce01
 * 
 * Revision 1.1.4.3  1993/09/23  17:32:40  pare
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/23  17:32:09  pare]
 * 
 * Revision 1.1.5.3  1993/09/15  17:46:58  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.4.2  1993/07/15  17:47:33  truitt
 * 	OT7014: utc_getusertime() was not using user-supplied TDF if
 * 	        no daylight savings time was specified in the timezone.
 * 	[1993/07/15  17:45:24  truitt]
 * 
 * Revision 1.1.15.1  1994/05/12  15:17:38  rhw
 * 	continued code cleanup & merging
 * 	[1994/05/09  18:46:37  rhw]
 * 
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:53  rhw]
 * 
 * Revision 1.1.13.3  1993/07/14  20:37:17  tom
 * 	Bug 7014: utc_getusertime wasn't using user tdf if no DST (ie TZ=EST5)
 * 	[1993/07/13  20:34:38  tom]
 * 
 * Revision 1.1.13.2  1993/05/24  20:51:23  cjd
 * 	Submitting 102-dme port to 103i
 * 	[1993/05/24  20:19:29  cjd]
 * 
 * Revision 1.1.11.3  1993/05/17  15:50:29  jd
 * 	Tom sets endian in the makefile now.
 * 	[1993/05/17  15:50:06  jd]
 * 
 * Revision 1.1.1.10  1993/05/04  22:38:18  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.5.8  1993/01/28  17:58:29  tom
 * 	Bug 2848 - Correct fix: check against max and min unix times to we
 * 	can handle huge dates.
 * 	[1993/01/28  17:20:27  tom]
 * 
 * Revision 1.1.5.7  1993/01/28  15:26:14  tom
 * 	Don't update tzname[], let the OS ctime functions take care of that.
 * 	This is so the OS functions won't have pointers in to our data.
 * 	Bug 6919 - Back out fix for 2848, it broke more reasonable times.
 * 	[1993/01/28  15:24:46  tom]
 * 
 * Revision 1.1.5.6  1993/01/21  19:37:55  tom
 * 	Bug 5917 - Rename tzset to dce_tzset, call it unconditionally to
 * 	always check TZ.
 * 	[1993/01/21  19:37:13  tom]
 * 
 * Revision 1.1.5.5  1993/01/19  00:39:15  jwade
 * 	Fix for OT CR 6891.
 * 	[1993/01/19  00:18:39  jwade]
 * 
 * 	If either inaccuracy parameter for utc_adtime or utc_subtime is infinite
 * 	then set the resulting inaccuracy to infinite.
 * 	OT CR 6890
 * 	[1993/01/19  00:08:26  jwade]
 * 
 * 	Fix for OT CR 6893.
 * 	[1993/01/19  00:02:02  jwade]
 * 
 * Revision 1.1.5.4  1992/12/30  16:34:41  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  11:11:08  htf]
 * 
 * Revision 1.1.5.3  1992/12/29  16:06:19  yankes
 * 	This change fixes OT 2848 so that now high values of UTCvalue (ie. in the
 * 	range of the year 9999) work properly in utc_mklocaltime.  May my
 * 	(great^^266th)-grandchildren appreciate this code. :-)
 * 	[1992/12/22  22:12:17  yankes]
 * 
 * 	Fix for OT 5495: remove unprotected write into "tns" so that NULL param
 * 	works correctly.
 * 	[1992/11/03  20:49:03  yankes]
 * 
 * Revision 1.1.5.2  1992/09/29  20:45:05  devsrc
 * 	[OT 5373]  SNI/SVR4 Merge.
 * 	[1992/09/14  20:03:40  sekhar]
 * 
 * Revision 1.1.2.7  1992/06/09  20:20:50  ko
 * 	OT 4165 - bug in interpretation of inacc field running on little endian
 * 	machines.  Big Endian timestamp inaccuracies getting corrupted.
 * 	[1992/06/09  20:06:31  ko]
 * 
 * Revision 1.1.2.6  1992/06/02  17:53:36  ko
 * 	Include symbol __MIPSEL__ along with MIPSEL, since c89 -std only defines
 * 	__MIPSEL__.
 * 	[1992/06/02  15:21:39  ko]
 * 
 * Revision 1.1.2.5  1992/05/14  14:01:19  comuzzi
 * 	Fix sign error in splittime, OT defect 2833.
 * 	[1992/05/13  19:13:31  comuzzi]
 * 
 * Revision 1.1.2.4  1992/05/13  18:47:32  comuzzi
 * 	Fixed logic error in mergetime, OT defect 2834.
 * 	[1992/05/13  17:55:07  comuzzi]
 * 
 * Revision 1.1.2.3  1992/05/12  19:04:20  comuzzi
 * 	Fixed several logic errors in splittime,
 * 	Fixed problem with infinite inacc,
 * 	Changed lower range check.
 * 	Fixed defects: 2830, 2835, 2853, 2855, 2865
 * 	[1992/05/12  18:07:55  comuzzi]
 * 
 * Revision 1.1.2.2  1992/05/11  13:39:36  comuzzi
 * 	Fixed defects 2829, 2863 and 2864. 2829 fixes missing checks for null input
 * 	arguments. 2863 and 2864 fixes two logic errors in utc_mulftime. Also fixed
 * 	some ANSI standard C warnings and changed VMS specific conditionals.
 * 	[1992/05/07  14:04:17  comuzzi]
 * 
 * Revision 1.1  1992/01/19  15:32:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 	Module PORTABLE_API.C
 *	Version X0.1
 */
/*
 * Copyright (c) 1989 by
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
 * ABSTRACT:    This is the portable API for DECdts
 *
 * ENVIRONMENT:	VAX/VMS and Ultrix
 *
 * AUTHORS:
 *		Joe Comuzzi
 *		Distributed Systems Services (DSS) 
 * 
 * DATE:	August 24, 1989
 *
 */

/* LINTLIBRARY */

#include <sys/types.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <dts.h>
#include <dtss_config.h>
#include <dtssdef.h>
#include <utc_arith.h>
#include <utc.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <tzfile.h>

#include <dce/assert.h>
#include <dtssmacros.h>

#define SWAP16(src) (unsigned short)(((unsigned short)src >> 8) +	\
			             ((unsigned short)src << 8))

#define SWAP32(src) (unsigned int)(((unsigned int)src >> 24) +  \
                              (((unsigned int)src & 0xff0000) >> 8) +   \
                              (((unsigned int)src & 0xff00) << 8) +     \
                              ((unsigned int)src << 24))

#if LONG_BIT >= 64
#define SWAP64(src) (unsigned long)(((unsigned long)src >> 56) +      \
                      (((unsigned long)src & 0xff000000000000) >> 40) + \
                      (((unsigned long)src & 0xff0000000000) >> 24) + 	\
                      (((unsigned long)src & 0xff00000000) >> 8) + 	\
                      (((unsigned long)src & 0xff000000) << 8) + 	\
                      (((unsigned long)src & 0xff0000) << 24) + 	\
                      (((unsigned long)src & 0xff00) << 40) + 		\
                      ((unsigned long)src << 56))
#endif	/* LONG_BIT */

/*
 * Define utc structure union. There are two version a little-endian one
 * and an big-endian structure. Integers (except for the tdf field) are
 * stored in the natural order for each architecture. This interface
 * always returns utc structures that are natural for this architecture
 * and interprets utc structures generated by either architecture. (It is
 * assumed that the bytes are copied between systems in a byte order
 * preserving manner.) This means that timestamps generated on little-endian
 * systems (VAXen, IBMPCs, etc) can be interpreted on big-endian systems
 * (MC68000s, SUN SPARCs, etc) and vice-versa. This is particularly useful
 * for timestamps stored in file systems, such timestamps can be accessed
 * by either architecture without difficulty.
 */

struct UTC
{
#if LONG_BIT >= 64
    union {
        struct {
             unsigned long int time;
             unsigned int inacclo;
             unsigned short int inacchi;
             unsigned char tdflo;
             unsigned char flags;
        } little;
        struct {
             unsigned long int time;
             unsigned int inacchi;
             unsigned short int inacclo;
             unsigned char tdfhi;
             unsigned char flags;
        } big;
    } endian;
#else	/* LONG_BIT	*/
    union {
	struct {
	    unsigned long int timelo;
	    unsigned long int timehi;
	    unsigned long int inacclo;
	    unsigned short int inacchi;
	    unsigned char tdflo;
	    unsigned char flags;
	} little;
	struct {
	    unsigned long int timehi;
	    unsigned long int timelo;
	    unsigned short int inacchi;
	    unsigned short int inaccmid;
	    unsigned short int inacclo;
	    unsigned char tdfhi;
	    unsigned char flags;
	} big;
    } endian;
#endif	/* LONG_BIT */

#define ENDIAN_FLAG_MASK	(0x80)
#define VERSION_MASK		(0x70)
#define VERSION_SHIFT		(4)

#define LITTLE_ENDIAN_FLAG	(0 * ENDIAN_FLAG_MASK)
#define BIG_ENDIAN_FLAG		(1 * ENDIAN_FLAG_MASK)
};

/*
 * local prototypes 
 */

int utc_comptime(UTCValue *, UTCValue *, long *, utc_t *);
int utc_mkcomptime( utc_t *, UTCValue *, UTCValue *, long);
int utc_getnextzone( utc_t *, utc_t *) ;
int utc_settdf(utc_t *, utc_t *, long) ;
int tcp_open(char *, char *, int);

/*
 * Constants needed in conversions
 *
 *    Differences in base times in 100ns
 */

static UTCValue diff_UNIX_UTC_ticks = UTCinit(0x13814000UL,0x01b21dd2UL);
static UTCValue diff_UTC_UNIX_ticks = UTCinit(0xec7ec000UL,0xfe4de22dUL);

/*
 *    Differences in base times in seconds
 */

static UTCValue diff_UTC_UNIX_secs = UTCinit(0x27ac6380UL, 0xfffffffdUL);


/*
 *    Minimum and Maximum UNIX times (for range compares)
 */

static UTCValue min_UNIX_time = UTCinit(0x0,0x0);

static UTCValue min_UNIX_sgn_time = UTCinit(0x80000000UL,0xffffffffUL);


static UTCValue max_UNIX_time = UTCinit(0xffffffffUL,0x0);

static UTCValue max_UNIX_sgn_time = UTCinit(0x7fffffffUL,0x0);

static int monthToDays[12] = {
    0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
};

   
#define UTC_WDAY (1)
#define GREGORIAN_OFFSET (1581*365 + 1581/4 - 1581/100 + 1581/400 + 273 + 15)
#define JULIAN_OFFSET (1581*365 + 1581/4 + 273 + 5)

/*
 * Determine if the specified year is (was) a leap year
 */

#ifdef IsLeapYear
#undef IsLeapYear 
#endif

#define IsLeapYear(year)                                                \
    ( ((year) > 1582) ?                                                 \
		( (((year) % 4 == 0) && ((year) % 100 != 0)) ||         \
                  ((year) % 400 == 0) ) :                               \
                ( (year) % 4 == 0 ) )



/*
 * Include system include files
 */

#include <private.h>

#if defined(__OSF__) || defined(__osf__)

# undef TM_ZONE
# undef TM_GMTOFF

#endif /* defined(__OSF__) || defined(__osf__) */

static char GMT[] = "GMT";

#if defined(TM_ZONE)
static char rel[] = "rel";
static char gmt_relative[15];
#endif /* defined(TM_ZONE) */

/*
 * Include tzset.c  -  defines dce_tzset()
 */
/* VISTA -- don't re-instrument .c files */
#pragma covcc !instr
#include <tzset.c>
#pragma covcc instr

/*
 * Include arith.c
 * 6/20/94 CLBrooks - the intent here is that the functions defined
 * in arith.c be included within this file as statically linked;
 * if defined otherwise, should be externally linked
 */
#define INCLUDED 1
/* VISTA -- don't re-instrument .c files */
#pragma covcc !instr
#include <arith.c>
#pragma covcc instr

/* Remove the dependency of libm on libdce.sl.
* fabs() is the only math function called in libdce
* so replace fabs() with utc_fabs described below.
* Note that utc_fabs is lacking range checking found
* in fabs() -- range checking is done after the call  
* to utc_fabs. This change will eliminate the need for
* -lm on the link line when linking applications with
* libdce.a
*/
#if defined(__hpux)
static double utc_fabs(x)
double x;
{
   return (x < 0.0 ? -x : x);
}
#endif



static int find_ttis_given_local ( struct ttinfo **ttispp, UTCValue *secondp, int isdst)

{
    struct ttinfo *ttis;
    UTCValue secTemp;
    int i;

    secTemp = *secondp;

    dce_tzset();
    ttis = &lclstate.ttis[lclstate.types[0]];
    /* 
     * If isdst is -1, caller doesn't know whether or not it's 
     * daylight saving time
     */
    if (isdst < 0) {
	/*
	 * If there there are entries in the table, we're not past the 
	 * end and we're not before the beginning, then look for a 
	 * transition in the table, ignoring the state of tt_isdst
	 */
        if (lclstate.timecnt != 0 &&
            IsUTCge(&secTemp, &min_UNIX_sgn_time) &&
	    IsUTCle(&secTemp, &max_UNIX_sgn_time) &&
            (long)UTClow(&secTemp) >= ((long) lclstate.ats[0] + 
					(int) ttis->tt_gmtoff)) {
	    for (i = 1; i < lclstate.timecnt; i++) {
		ttis = &lclstate.ttis[lclstate.types[i - 1]];
                if ((long)UTClow(&secTemp) < (long) lclstate.ats[i - 1] +
					(int) ttis->tt_gmtoff)
		    return(-1);
                if ((long)UTClow(&secTemp) < (long) lclstate.ats[i] +
					(int) ttis->tt_gmtoff &&
                    IsUTClt(&secTemp, &max_UNIX_sgn_time))
		    break;
	    };
	};
    } else {
	/* 
	 * Caller has asserted either standard time (isdst == 0) or 
	 * daylight savings time (isdst > 0)
	 */
	if (isdst > 0) isdst = 1;
	/*
	 * If there there are entries in the table, we're not past the 
	 * end and we're not before the beginning, then look for a 
	 * transition in the table, checking tt_isdst against 
	 * requested isdst.
	 */
        if (lclstate.timecnt != 0 &&
            IsUTCge(&secTemp, &min_UNIX_sgn_time) &&
	    IsUTCle(&secTemp, &max_UNIX_sgn_time) &&
	    (long)UTClow(&secTemp) >= ((long) lclstate.ats[0] + 
					(int) ttis->tt_gmtoff)) {
	    for (i = 1; i < lclstate.timecnt; i++) {
		ttis = &lclstate.ttis[lclstate.types[i - 1]];

                if ((long)UTClow(&secTemp) < ((long) lclstate.ats[i - 1] +
					 (int)ttis->tt_gmtoff))
		    return(-1);
                if ((long)UTClow(&secTemp) < ((long) lclstate.ats[i] +
                                         (int)ttis->tt_gmtoff) &&
                    IsUTClt(&secTemp, &max_UNIX_sgn_time) &&
                    isdst == ttis->tt_isdst)
		    break;

		ttis = &lclstate.ttis[lclstate.types[i]];
	    };
	} else {
	    ttis = NULL;
	};

	if (ttis == NULL) {
	    /* 
	     * Can't find an appropriate transition in the table,
	     * either there were no transitions or our time was out of
	     * range.  Assume standard time; look for the first rule
	     * that claims standard time.  A user asking for dst in
	     * this case is confused.
	     */
	    i = 0;
	    while (lclstate.ttis[i].tt_isdst)
		if (++i >= lclstate.timecnt) {
		    i = 0;
		    break;
		};
	    ttis = &lclstate.ttis[i];
	}

	if (ttis->tt_isdst != isdst)
	    return(-1);
    };

    /* 
     * Return pointer to the ttis we found
     */
    *ttispp = ttis;

/*
 * The 64 bit long tt_gmtoff is not being
 * sign extended from a 32 bit int.  Extend it...
 */
    (*ttispp)->tt_gmtoff = (int) (*ttispp)->tt_gmtoff;

    return (0);
}
/* end of find_ttis_given_local */



static int find_ttis_given_utc ( struct ttinfo **ttispp, UTCValue *timevaluep)

{
    int i;
    long tns;
    UTCValue seconds;

    /* 
     * Convert utc time to seconds since unix epoch
     */
    UTCadd (timevaluep, &diff_UTC_UNIX_ticks, &seconds);
    UTCiDiv(&seconds, K_100NS_PER_SEC, &seconds, &tns);

    /* 
     * Find matching timezone
     */
    if (lclstate.timecnt == 0 ||
        IsUTClt(&seconds, &min_UNIX_sgn_time) ||
	IsUTCgt(&seconds, &max_UNIX_sgn_time) ||
	(long)UTClow(&seconds) < (long)lclstate.ats[0]) {
	i = 0;
	while (lclstate.ttis[i].tt_isdst)
	    if (++i >= lclstate.timecnt) {
		i = 0;
		break;
	    };
    } else {
	for (i = 1; i < lclstate.timecnt; ++i)
        {
            if ((long) UTClow(&seconds) < (long) lclstate.ats[i] &&
	        IsUTCle(&seconds,
			&max_UNIX_sgn_time) && /* Temp hack - times past  */
					     /* 2037 are always standard */
		IsUTCge(&seconds,
			&min_UNIX_sgn_time)) /* Times before 1902 are std */
		break;
        }
	i = lclstate.types[i-1];
    };

    /* 
     * Return pointer to the ttis we found
     */
    *ttispp = &lclstate.ttis[i];

#if LONG_BIT >= 64
/*
 * The 64 bit long tt_gmtoff is not being
 * sign extended from a 32 bit int.  Extend it...
 */
    (*ttispp)->tt_gmtoff = (int) (*ttispp)->tt_gmtoff;
#endif

    return(0);
}
/* end of find_ttis_given_utc */

/*
 *++
 *  splitinacc()
 *
 *  Functional Description:
 *
 *      Splits an inaccuracy into a tm struct.
 *  
 *  Inputs:
 *
 *      inaccp - pointer to 64-bit inaccuracy 
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *	inacc - pointer to tm struct which contains inaccuracy
 *      ins - pointer to longword which contains nanosec of inacc
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *
 *  Side Effects:
 *
 *
 *--
 */
static void splitinacc( UTCValue *inaccp, struct tm *inacc, long *ins)

{
    UTCValue	utcTemp;
    unsigned long temp;
    
    if (IsInfiniteInacc(inaccp)) {
	if (ins != NULL)
	    *ins = -1;
	if (inacc != NULL) {
	    inacc->tm_isdst = -1;
	    inacc->tm_wday = -1;
	    inacc->tm_sec = -1;
	    inacc->tm_min = -1;
	    inacc->tm_hour = -1;
	    inacc->tm_mday = -1;
	    inacc->tm_mon = -1;
	    inacc->tm_year = -1;
	    inacc->tm_yday = -1;
	}
    } else {
	UTCiDiv(inaccp, K_100NS_PER_SEC, &utcTemp, &temp);
	if (ins != NULL)
	    *ins = temp*100;
	if (inacc != NULL) {
	    inacc->tm_isdst = -1;
	    inacc->tm_wday = -1;
	    inacc->tm_sec = UTClow(&utcTemp) % SECS_PER_MIN;
	    UTClow(&utcTemp) = UTClow(&utcTemp) / SECS_PER_MIN;
	    inacc->tm_min = UTClow(&utcTemp) % MINS_PER_HOUR;
	    UTClow(&utcTemp) = UTClow(&utcTemp) / MINS_PER_HOUR;
	    inacc->tm_hour = UTClow(&utcTemp) % HOURS_PER_DAY;
	    UTClow(&utcTemp) = UTClow(&utcTemp) / HOURS_PER_DAY;
	    inacc->tm_mday = -1;
	    inacc->tm_mon = 0;
	    inacc->tm_year = 0;
	    inacc->tm_yday = UTClow(&utcTemp);
	};
    };
}
/* End of routine splitinacc */

/*
 *++
 *  splittime()
 *
 *  Functional Description:
 *
 *      Splits a utc time into a tm struct.
 *  
 *  Inputs:
 *
 *      timep - pointer to 64-bit utc time value.
 *      offset - timezone offset
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *	tm - pointer to tm struct which contains time
 *      tns - pointer to longword which contains nanosec of time
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *
 *  Side Effects:
 *
 *
 *--
 */
static void splittime( UTCValue *timep, long offset, struct tm *tm, long *tns)

{
    UTCValue utcTemp, lclTime;
    long seconds;
    long temp;
    register long days, year;
    register int i;

    if (tm != NULL)
	    tm->tm_isdst = -1;
    emul(offset, (long) K_100NS_PER_SEC, &lclTime);
    UTCadd(&lclTime, timep, &lclTime);

        if (IsUTCneg(&lclTime)) {
	UTCiDiv(&lclTime, -K_100NS_PER_SEC, &utcTemp, &temp);
	if (temp != 0) {
	    temp = (K_100NS_PER_SEC + temp) * 100;
	    UTCiAdd(&utcTemp, 1, &utcTemp);
	}
        if (tns != NULL)
	    *tns = temp;
	UTCiDiv(&utcTemp, SECS_PER_DAY, &utcTemp, &seconds);
	if (seconds != 0) {
	    seconds = SECS_PER_DAY - seconds;
	    UTClow(&utcTemp) += 1;
	}	    
	days = -(UTClow(&utcTemp)) + JULIAN_OFFSET - 1;
    } else {
	UTCiDiv(&lclTime, K_100NS_PER_SEC, &utcTemp, &temp);
	if (tns != NULL)
	    *tns = temp * 100;
	UTCiDiv(&utcTemp, SECS_PER_DAY, &utcTemp, &seconds);
	days = UTClow(&utcTemp) + GREGORIAN_OFFSET - 1;
    }
    if (tm == NULL)
	return;
    tm->tm_hour = (int) seconds / SECS_PER_HOUR;
    seconds = seconds % SECS_PER_HOUR;
    tm->tm_min = (int) seconds / SECS_PER_MIN;
    tm->tm_sec = (int) seconds % SECS_PER_MIN;    
    tm->tm_wday = (int) ((UTC_WDAY + days) % DAYS_PER_WEEK);
    if (tm->tm_wday < 0)
	tm->tm_wday += DAYS_PER_WEEK;
    year = 1;

    if (!IsUTCneg(&lclTime)) {
	year += (days / 146097) * 400;
	days %= 146097;
	temp = (days / 36524 < 3) ? (days / 36524) : 3;
	year += temp * 100;
	days -= temp * 36524;
    }
    year += (days / 1461) * 4;
    days %= 1461;
    if (days < 0) {
	days += 1461;
	year -= 4;
    }
    temp = (days / 365 < 3) ? (days / 365) : 3;
    year += temp;
    days -= temp * 365;
    tm->tm_yday = days;
    if (days == 59 && IsLeapYear(year)) {
	tm->tm_mon = 2-1;
        tm->tm_mday = 29;
    } else {
	if (days > 59 && IsLeapYear(year))
	    days--;
	for (i = 1; (i < 12) && (days >= monthToDays[i]); i++);
	tm->tm_mon = --i;
	tm->tm_mday = ++days - monthToDays[i];
    };
    tm->tm_year = year - TM_YEAR_BASE;

}
/* End of routine splittime */

/*
 *++
 *  mergeinacc()
 *
 *  Functional Description:
 *
 *      Merges an inaccuracy in a tm struct into a 48-bit inacc
 *  
 *  Inputs:
 *
 *	inacc - pointer to tm struct which contains inaccuracy
 *      ins - longword which contains nanosec of inacc
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      inaccp - pointer to 64-bin inaccuracy
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *
 *  Side Effects:
 *
 *
 *--
 */
static void mergeinacc( struct tm *inacc, long ins, UTCValue *inaccp)

{
    if (inacc == NULL || inacc->tm_yday < 0) {
	UTCassign(inaccp,K_INFINITE_INACC_LO,K_INFINITE_INACC_HI);
    } else {
	uemul((unsigned long) (((inacc->tm_yday * HOURS_PER_DAY +
                                 inacc->tm_hour) * MINS_PER_HOUR +
                                inacc->tm_min) * SECS_PER_MIN +
	                       inacc->tm_sec),
              (unsigned long) K_100NS_PER_SEC, inaccp);

	UTCiAdd(inaccp, (ins + 99)/100, inaccp);
    };

}
/* End of routine mergeinacc */

/*
 *++
 *  mergetime()
 *
 *  Functional Description:
 *
 *      Merges a time in a tm struct into a 64-bit time
 *  
 *  Inputs:
 *
 *	tm - pointer to tm struct which contains time
 *      tns - longword which contains nanosec of time
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      timep - pointer to 64-bit time value
 *      seconds - pointer to 64-bit seconds since UNIX epoch
 *                (used for TDF calculations)
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *
 *  Side Effects:
 *
 *
 *--
 */
static int mergetime( struct tm *tm, long tns, UTCValue *timep, UTCValue *seconds)

{
    long	year, month, days;

    if (tm == NULL)
	return(-1);

    year = tm->tm_year + 1900;
    if (year < -27644 || year > 30809)
	return(-1);

    month = tm->tm_mon + 1;
    days = tm->tm_mday + 365*(year - 1) + (year - 1)/4; 
    if (year > 1582 ||
        (year == 1582 && (month > 10 ||
                          (month == 10 && tm->tm_mday > 14)))) {
	days += - (year - 1)/100 + (year - 1)/400;  
	days -= GREGORIAN_OFFSET;
    } else {
	if (year == 1582 && month == 10 &&
            tm->tm_mday > 4 && tm->tm_mday < 15)
		return (-1);
	days -= JULIAN_OFFSET;
    }

    if (days > 0) {
	if (month > 2 && IsLeapYear(year))
	    days += 1;
    } else {
	if (month < 3 && IsLeapYear(year))
	    days -= 1;
    }

    days += monthToDays[tm->tm_mon];
    emul(days, SECS_PER_DAY, seconds);
    UTCiAdd(seconds, (tm->tm_hour * MINS_PER_HOUR +
                      tm->tm_min) * SECS_PER_MIN +
                      tm->tm_sec, seconds);

    UTCiMul(seconds, K_100NS_PER_SEC, timep);
    UTCiAdd(timep, (tns + 50)/100, timep);
    UTCadd(seconds, &diff_UTC_UNIX_secs, seconds);

    return(0);
}
/* End of routine mergetime */




/*
 *++
 *  utc_comptime()
 *
 *  Functional Description:
 *
 *      Converts a 128-bit UTC time into components.
 *  
 *  Inputs:
 *
 *      utc - pointer to 128-bit time to split into components
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      timevalue - pointer to component to receive time component of utc.
 *      inaccvalue - pointer to component to receive inaccuracy component
 *      tdf  - pointer to int to receive the tdf
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */
int utc_comptime( UTCValue *timevalue, UTCValue *inaccvalue, long *tdf, utc_t *utc)

{
    struct UTC autc;
    UTCValue ainaccvalue;
    register int atdf;
    register int i;

    if (utc == NULL) {
	if ((i = utc_gettime((utc_t *)&autc)) < 0)
	    return(i);
    } else {
	(void) memcpy((pointer_t)&autc, (pointer_t)utc, sizeof(utc_t));
    };

#if (BYTE_ORDER == LITTLE_ENDIAN)
    if (((int)(autc.endian.little.flags & VERSION_MASK) >> VERSION_SHIFT) !=
	K_BINTIME_VERSION)
	return(-1);

    if ((autc.endian.little.flags & ENDIAN_FLAG_MASK) == LITTLE_ENDIAN_FLAG) {
	atdf = ((autc.endian.little.flags & 0xf) << 8) +
	       autc.endian.little.tdflo;
	UTCassign(&ainaccvalue, autc.endian.little.inacclo,
				autc.endian.little.inacchi);

	if (timevalue != NULL)
	{
#if LONG_BIT >= 64
            *timevalue = autc.endian.little.time;
#else	/* LONG_BIT */
	    timevalue->lo = autc.endian.little.timelo;
	    timevalue->hi = autc.endian.little.timehi;
#endif	/* LONG_BIT */
	}
    } else {
	atdf = (autc.endian.big.tdfhi << 4) + (autc.endian.big.flags & 0xf);
#if LONG_BIT >= 64
        ainaccvalue = (SWAP32(autc.endian.big.inacchi) << 16) +
                         SWAP16(autc.endian.big.inacclo);
#else	/* LONG_BIT */
	ainaccvalue.hi = SWAP16(autc.endian.big.inacchi);
	ainaccvalue.lo = (SWAP16(autc.endian.big.inaccmid) << 16) +
			 SWAP16(autc.endian.big.inacclo);
#endif	/* LONG_BIT */
	if (timevalue != NULL)
	{
#if LONG_BIT >= 64
            *timevalue = SWAP64(autc.endian.big.time);
#else	/* LONG_BIT */
	    timevalue->hi = SWAP32(autc.endian.big.timehi);
	    timevalue->lo = SWAP32(autc.endian.big.timelo);
#endif	/* LONG_BIT */
	}
    }
#endif	/* (BYTE_ORDER == LITTLE_ENDIAN)	*/

#if (BYTE_ORDER == BIG_ENDIAN)
    if (((autc.endian.big.flags & VERSION_MASK) >> VERSION_SHIFT) !=
	K_BINTIME_VERSION)
	return(-1);

    if ((autc.endian.big.flags & ENDIAN_FLAG_MASK) == BIG_ENDIAN_FLAG) {
	atdf = (autc.endian.big.tdfhi << 4) + (autc.endian.big.flags & 0xf);

#if LONG_BIT >= 64
        ainaccvalue = autc.endian.big.inacchi;
        ainaccvalue = (ainaccvalue << 32) + autc.endian.big.inacclo;
#else	/* LONG_BIT */
	ainaccvalue.hi = autc.endian.big.inacchi;
	ainaccvalue.lo = (autc.endian.big.inaccmid << 16) +
			 autc.endian.big.inacclo;
#endif	/* LONG_BIT */

	if (timevalue != NULL)
	{
#if LONG_BIT >= 64
            *timevalue = autc.endian.big.time;
#else	/* LONG_BIT */
	    timevalue->hi = autc.endian.big.timehi;
	    timevalue->lo = autc.endian.big.timelo;
#endif	/* LONG_BIT */
	}
    } else {
	atdf = ((autc.endian.little.flags & 0xf) << 8) +
	       autc.endian.little.tdflo;

#if LONG_BIT >= 64
        ainaccvalue = SWAP16(autc.endian.little.inacchi);
        ainaccvalue = (ainaccvalue.lo << 32) +
            SWAP32(autc.endian.little.inacclo);
#else	/* LONG_BIT */
	ainaccvalue.lo = SWAP32(autc.endian.little.inacclo);
	ainaccvalue.hi = SWAP16(autc.endian.little.inacchi);
#endif	/* LONG_BIT */

	if (timevalue != NULL)
	{
#if LONG_BIT >= 64
            *timevalue = SWAP64(autc.endian.little.time);
#else
	    timevalue->lo = SWAP32(autc.endian.little.timelo);
	    timevalue->hi = SWAP32(autc.endian.little.timehi);
#endif
	}
    }
#endif	/* (BYTE_ORDER == BIG_ENDIAN)	*/

    if (tdf != NULL) {
	if (atdf > 0x7ff) atdf -= 0x1000;	/* Sign extend tdf */
	*tdf = atdf * SECS_PER_MIN;
    }

    if (inaccvalue != NULL) {
	if (IsInfiniteInacc(&ainaccvalue)) {
	    UTCassign(inaccvalue, K_INFINITE_INACC_LO, K_INFINITE_INACC_HI);
	} else {
	    *inaccvalue = ainaccvalue;
	}	    
    }

    return(0);

}
/* End of routine utc_comptime */

/*
 *++
 *  utc_mkcomptime()
 *
 *  Functional Description:
 *
 *      Converts components into a 128-bit UTC time.
 *  
 *  Inputs:
 *
 *      timevalue - pointer to timespecs to provide time component of utc.
 *      inaccvalue - pointer to timespecs to provide inaccuracy component
 *                   (if pointer is null, assume infinite iaccuracy)
 *      tdf  - tdf to place in 128-bit UTC time.
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      utc - pointer to 128-bit time resulting from components
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */     
int utc_mkcomptime( utc_t *utc, UTCValue *timevalue, UTCValue *inaccvalue, long tdf)

{
    struct UTC autc;
    UTCValue ainacc;

    dce_assert( (struct dce_svc_handle_s_t *)0, utc != NULL ) ;

	if (inaccvalue == NULL || (IsInfiniteInacc(inaccvalue))) {
	    UTCassign (&ainacc, K_INFINITE_INACC_LO, K_INFINITE_INACC_HI);
	} else {
	    ainacc = *inaccvalue;
	} 

#if (BYTE_ORDER == LITTLE_ENDIAN)
	autc.endian.little.flags = ((tdf / SECS_PER_MIN) >> 8) & 0xf |
				   (K_BINTIME_VERSION << VERSION_SHIFT) |
			           LITTLE_ENDIAN_FLAG;
	autc.endian.little.tdflo = (tdf / SECS_PER_MIN) & 0xff;
#if LONG_BIT >= 64
        autc.endian.little.inacclo = (unsigned int) (ainacc & 0x0ffffffff);
        autc.endian.little.inacchi = (unsigned short int)
                                     ((ainacc & 0x0ffff00000000) >> 32);
        autc.endian.little.time = *timevalue;
#else	/* LONG_BIT */
	autc.endian.little.inacclo = ainacc.lo;
	autc.endian.little.inacchi = ainacc.hi;
	autc.endian.little.timelo = timevalue->lo;
	autc.endian.little.timehi = timevalue->hi;
#endif	/* LONG_BIT */
#endif	/* (BYTE_ORDER == LITTLE_ENDIAN) */

#if (BYTE_ORDER == BIG_ENDIAN)
	autc.endian.big.flags = ((tdf / SECS_PER_MIN) & 0xf) |
				(K_BINTIME_VERSION << VERSION_SHIFT) |
				BIG_ENDIAN_FLAG;
	autc.endian.big.tdfhi = (tdf / SECS_PER_MIN) >> 4;
#if LONG_BIT >= 64
        autc.endian.big.inacchi = (unsigned int) (ainacc & 0x0ffffffff);
        autc.endian.big.inacclo = (unsigned short int)
                                  ((ainacc &0x0ffff00000000) >> 32);
        autc.endian.big.time = *timevalue;
#else	/* LONG_BIT */
	autc.endian.big.inacchi = ainacc.hi;
	autc.endian.big.inaccmid = ainacc.lo >> 16;
	autc.endian.big.inacclo = ainacc.lo;
	autc.endian.big.timehi = timevalue->hi;
	autc.endian.big.timelo = timevalue->lo;
#endif 	/* LONG_BIT */
#endif	/* (BYTE_ORDER == BIG_ENDIAN) */

    (void) memcpy((pointer_t)utc, (pointer_t)&autc, sizeof(utc_t));

    return(0);

}
/* End of routine utc_mkcomptime */

/*
 *++
 *  utc_boundtime()
 *
 *  Functional Description:
 *
 *      Given two UTC times, returns a single UTC whose inaccuracy 
 *      bounds the two input times.  Useful for timestamping events: 
 *      get utc values before and after the event, then call 
 *      utc_boundtime to build a timestamp that must include the 
 *      event.
 *  
 *  Inputs:
 *
 *      utclp - low utc value (the before timestamp)
 *      utchp - high utc value (the after timestamp)
 *
 *  Implicit Inputs:
 *
 *  Outputs:
 *
 *      result - bounding timestamp
 * 
 *  Implicit Outputs:
 *
 *      tdf in output UTC value is copied from the utch input
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *--
 */     
int utc_boundtime( utc_t *result, utc_t *utclp, utc_t *utchp)

{
    UTCValue timevaluel, timevalueh, timevalues;
    UTCValue inaccvaluel, inaccvalueh, inaccvalues;
    long tdfh, lowbits;
    int status;
    
    if (((status = utc_comptime(&timevaluel, &inaccvaluel, (long *)NULL, utclp)) < 0) ||
	((status = utc_comptime(&timevalueh, &inaccvalueh, &tdfh, utchp)) < 0))
	return(status);

    if ((IsInfiniteInacc (&inaccvaluel)) ||
	(IsInfiniteInacc (&inaccvalueh))) {
	/* 
	 * One or both inputs have infinite inaccuracy. Average the 
	 * two midpoints and return infinite inaccuracy
	 * If lower bound is greater than upper bound, return error
	 */
	if (IsUTCgt(&timevaluel, &timevalueh))
	    return(-1);
	lowbits = (UTClow(&timevaluel) & 1) + (UTClow(&timevalueh) & 1);
	UTCdivideBy2(&timevaluel, &timevaluel);
	UTCdivideBy2(&timevalueh, &timevalueh);
	UTCadd(&timevaluel, &timevalueh, &timevalues);
	UTCiAdd(&timevalues, (lowbits/2), &timevalues);
	return(utc_mkcomptime(result, &timevalues, (UTCValue *)NULL, tdfh));
    }
    /* 
     * Calculate the lower and upper bounds of the bounding timestamp
     */
    UTCsub(&timevaluel, &inaccvaluel, &timevaluel);
    UTCadd(&timevalueh, &inaccvalueh, &timevalueh);
    /* 
     * If lower bound is greater than upper bound, return error
     */
    if (IsUTCgt(&timevaluel, &timevalueh))
	return(-1);

    /* 
     * Calculate the midpoint and the inaccuracy (half the span from 
     * low to high
     */
    lowbits = (UTClow(&timevaluel) & 1) + (UTClow(&timevalueh) & 1);
    UTCdivideBy2(&timevaluel, &timevaluel);
    UTCdivideBy2(&timevalueh, &timevalueh);
    UTCadd(&timevaluel, &timevalueh, &timevalues);
    UTCsub(&timevalueh, &timevaluel, &inaccvalues);
    /* 
     * Account for possible roundoff error in the two divides
     */
    UTCiAdd(&timevalues, (lowbits/2), &timevalues);
    UTCiAdd(&inaccvalues, ((lowbits+1)/2), &inaccvalues);

    return(utc_mkcomptime(result, &timevalues, &inaccvalues, tdfh));
}
/* End of routine utc_boundtime */

/*
 *++
 *  utc_spantime()
 *
 *  Functional Description:
 *
 *      Given two UTC times, returns a single UTC whose inaccuracy 
 *      spans the two input times.  Make no assumptions about the
 *      order of the two timestamps.
 *  
 *  Inputs:
 *
 *      utc1 - utc value
 *      utc2 - utc value
 *
 *  Implicit Inputs:
 *
 *  Outputs:
 *
 *      result - spanning timestamp
 * 
 *  Implicit Outputs:
 *
 *      tdf in output UTC value is copied from the utc2 input
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *--
 */     
int utc_spantime( utc_t *result, utc_t *utc1, utc_t *utc2)

{
    UTCValue timevalue1, timevalue2, Temp, TempHi, TempLo, timevalues;
    UTCValue inaccvalue1, inaccvalue2, inaccvalues;
    long tdf, lowbits;
    int status;
    
    if (((status = utc_comptime(&timevalue1, &inaccvalue1, (long *)NULL, utc1)) < 0) ||
	((status = utc_comptime(&timevalue2, &inaccvalue2, &tdf, utc2)) < 0))
	return(status);

    if ((IsInfiniteInacc (&inaccvalue1)) ||
	(IsInfiniteInacc (&inaccvalue2))) {
	/* 
	 * One or both inputs have infinite inaccuracy. Average the 
	 * two midpoints and return infinite inaccuracy
	 */
	lowbits = (UTClow(&timevalue1) & 1) + (UTClow(&timevalue2) & 1);
	UTCdivideBy2(&timevalue1, &timevalue1);
	UTCdivideBy2(&timevalue2, &timevalue2);
	UTCadd(&timevalue1, &timevalue2, &timevalues);
	UTCiAdd(&timevalues, (lowbits/2), &timevalues);
	return(utc_mkcomptime(result, &timevalues, (UTCValue *)NULL, tdf));
    }

    /* 
     * Calculate upper bounds of the spanning timestamp
     */
    UTCadd(&timevalue1, &inaccvalue1, &TempHi);
    UTCadd(&timevalue2, &inaccvalue2, &Temp);
    if (IsUTClt(&TempHi, &Temp))
	TempHi = Temp;
    /*
     * Calculate lower bounds of the spanning timestamp
     */
    UTCsub(&timevalue1, &inaccvalue1, &TempLo);
    UTCsub(&timevalue2, &inaccvalue2, &Temp);
    if (IsUTCgt(&TempLo, &Temp))
	TempLo = Temp;
    /* 
     * Calculate the midpoint and the inaccuracy (half the span from 
     * low to high
     */
    lowbits = (UTClow(&TempLo) & 1) + (UTClow(&TempHi) & 1);
    UTCdivideBy2(&TempLo, &TempLo);
    UTCdivideBy2(&TempHi, &TempHi);
    UTCadd(&TempHi, &TempLo, &timevalues);
    UTCsub(&TempHi, &TempLo, &inaccvalues);
    /* 
     * Account for possible roundoff error in the two divides
     */
    UTCiAdd(&timevalues, (lowbits/2), &timevalues);
    UTCiAdd(&inaccvalues, ((lowbits+1)/2), &inaccvalues);

    return(utc_mkcomptime(result, &timevalues, &inaccvalues, tdf));
}
/* End of routine utc_spantime */

/*
 *++
 *  utc_pointtime();
 *
 *  Functional Description:
 *
 *      Given a UTC times, returns two UTC whose inaccuracies are zero
 *	that bound the given time. That is, this routine returns the
 *	upper and lower limits of the given time.
 *  
 *  Inputs:
 *
 *	utc - input timestamp
 *
 *  Implicit Inputs:
 *
 *  Outputs:
 *
 *      utclp - low utc value (the earliest possible timestamp)
 *	utcmp - mid utc value (with zero inaccuracy)
 *      utchp - high utc value (the latest possible timestamp)
 * 
 *  Implicit Outputs:
 *
 *      tdf in output UTC value is copied from the utc1 input
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *--
 */     
int utc_pointtime( utc_t *utclp, utc_t *utcmp, utc_t *utchp, utc_t *utc)

{
    UTCValue timevaluel, timevalueh, timevalue;
    UTCValue inaccvalue;
    long tdf;
    int status;
    
    if ((status = utc_comptime(&timevalue, &inaccvalue, &tdf, utc)) < 0)
	return(status);

    if (IsInfiniteInacc (&inaccvalue)) {
	status = -1;
    }

    UTCsub(&timevalue, &inaccvalue, &timevaluel);
    UTCadd(&timevalue, &inaccvalue, &timevalueh);
    UTCzero(&inaccvalue);

    (void) utc_mkcomptime(utclp, &timevaluel, &inaccvalue, tdf);
    (void) utc_mkcomptime(utcmp, &timevalue, &inaccvalue, tdf);
    (void) utc_mkcomptime(utchp, &timevalueh, &inaccvalue, tdf);

    return(status);

}
/* End of routine utc_pointtime */

/*
 *++
 *  utc_bintime()
 *
 *  Functional Description:
 *
 *      Converts a 128-bit UTC time into timespecs
 *  
 *  Inputs:
 *
 *      utc - pointer to 128-bit time to split into timespecs
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      timesp - pointer to timespec to receive time component of utc.
 *      inaccsp - pointer to timespec to receive inaccuracy component
 *      tdf  - pointer to int to receive the tdf
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */
int utc_bintime( timespec_t *timesp, timespec_t *inaccsp, long *tdf, utc_t *utc)

{
    UTCValue timevalue, inaccvalue;
    unsigned long temp;
    register int i;

    if ((i = utc_comptime(&timevalue, &inaccvalue, tdf, utc)) < 0)
	return(i);

    if (inaccsp != NULL) {
	if (IsInfiniteInacc(&inaccvalue)) {
	    inaccsp->tv_sec = inaccsp->tv_nsec = -1;
	} else {
	    UTCiDiv(&inaccvalue, K_100NS_PER_SEC, &inaccvalue, &temp);
	    inaccsp->tv_sec = UTClow(&inaccvalue);
	    inaccsp->tv_nsec = temp*100;
	}
    }
    UTCadd(&timevalue, &diff_UTC_UNIX_ticks, &timevalue);
    UTCiDiv(&timevalue, K_100NS_PER_SEC, &timevalue, &temp);
    if ((long) temp < 0) {
	temp += K_100NS_PER_SEC;
	(UTClow(&timevalue))--;	
    }
    if (timesp != NULL)
    {
	timesp->tv_sec = UTClow(&timevalue);
	timesp->tv_nsec = temp*100;
    }
    
    if (IsUTCgt(&timevalue, &max_UNIX_time) ||
        IsUTClt(&timevalue, &min_UNIX_time)) {
        return(-1);
    };

    return(0);

}
/* End of routine utc_bintime */

/*
 *++
 *  utc_mkbintime()
 *
 *  Functional Description:
 *
 *      Converts timespecs into a 128-bit UTC time.
 *  
 *  Inputs:
 *
 *      timesp - pointer to timespecs to provide time component of utc.
 *      inaccsp - pointer to timespecs to provide inaccuracy component
 *      tdf  - tdf, in seconds east of gmt, to place in 128-bit UTC time.
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      utc - pointer to 128-bit time resulting from timespecs.
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */     
int utc_mkbintime( utc_t *utc, timespec_t *timesp, timespec_t *inaccsp, long tdf)

{
    UTCValue timevalue, inaccvalue;

    if (inaccsp == NULL || (long)inaccsp->tv_sec < 0) {
	UTCassign (&inaccvalue, K_INFINITE_INACC_LO, K_INFINITE_INACC_HI);
    } else {
	uemul((unsigned long)inaccsp->tv_sec, K_100NS_PER_SEC,
              &inaccvalue);
	UTCiAdd(&inaccvalue, ((inaccsp->tv_nsec + 99)/100),
		&inaccvalue);
    };

    emul((long)timesp->tv_sec, K_100NS_PER_SEC, &timevalue);
    UTCiAdd(&timevalue, ((timesp->tv_nsec + 50)/100), &timevalue);
    UTCadd(&timevalue, &diff_UNIX_UTC_ticks, &timevalue);

    return(utc_mkcomptime(utc, &timevalue, &inaccvalue, tdf));

}
/* End of routine utc_mkbintime */

/*
 *++
 *  utc_binreltime()
 *
 *  Functional Description:
 *
 *      Converts a relative 128-bit UTC time into timespecs.
 *  
 *  Inputs:
 *
 *      utc - pointer to relative 128-bit time to split into timespecs.
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      timesp - pointer to timespecs to receive time component of utc.
 *      inaccsp - pointer to timespecs to receive inaccuracy component
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */
int utc_binreltime( reltimespec_t *timesp, timespec_t *inaccsp, utc_t *utc)

{
    UTCValue timevalue, inaccvalue;
    unsigned long temp;
    register int i;

    if ((i = utc_comptime(&timevalue, &inaccvalue, (long *)NULL, utc)) < 0)
	return(i);

    if (inaccsp != NULL) {
	if (IsInfiniteInacc(&inaccvalue)) {
	    inaccsp->tv_sec = inaccsp->tv_nsec = 0xffffffffUL;
	} else {
	    UTCiDiv(&inaccvalue, K_100NS_PER_SEC, &inaccvalue, &temp);
	    inaccsp->tv_sec = UTClow(&inaccvalue);
	    inaccsp->tv_nsec = temp*100;
	};
    };

    UTCiDiv(&timevalue, K_100NS_PER_SEC, &timevalue, &temp);
    if ((long) temp < 0) {
	temp += K_100NS_PER_SEC;
#if LONG_BIT >= 64
        timevalue--;
#else
	if ((timevalue.lo-- == 0) &&
	    (timevalue.hi == 0)) timevalue.hi--;
#endif  /* LONG_BIT */
    }
    if (timesp != NULL)
    {
	timesp->tv_sec = UTClow(&timevalue);
	timesp->tv_nsec = temp*100;
    }

    if (IsUTCgt(&timevalue, &max_UNIX_sgn_time) ||
        IsUTClt(&timevalue, &min_UNIX_sgn_time)) {
        return(-1);
    };

    return(0);

}
/* End of routine utc_binreltime */

/*
 *++
 *  utc_mkbinreltime()
 *
 *  Functional Description:
 *
 *      Converts relative timespecs into a 128-bit UTC time.
 *  
 *  Inputs:
 *
 *      timesp - pointer to timespecs to provide time component of utc.
 *      inaccsp - pointer to timespecs to provide inaccuracy component
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      utc - pointer to 128-bit time resulting from timespecs.
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */     
int utc_mkbinreltime( utc_t *utc, reltimespec_t *timesp, timespec_t *inaccsp)

{
    UTCValue timevalue, inaccvalue;

    if (inaccsp == NULL || (long)inaccsp->tv_sec < 0) {
	UTCassign (&inaccvalue,K_INFINITE_INACC_LO,K_INFINITE_INACC_HI);
    } else {
	uemul((unsigned long)inaccsp->tv_sec, K_100NS_PER_SEC,
              &inaccvalue);
	UTCiAdd(&inaccvalue, ((inaccsp->tv_nsec + 99)/100),
                &inaccvalue);
    };

    emul((long)timesp->tv_sec, K_100NS_PER_SEC, &timevalue);
    UTCiAdd(&timevalue, ((timesp->tv_nsec + 50)/100), &timevalue);

    return(utc_mkcomptime(utc, &timevalue, &inaccvalue, 0L));

}
/* End of routine utc_mkbinreltime */

/*
 *++
 *  utc_localtime()
 *
 *  Functional Description:
 *
 *      Converts a 128-bit UTC time into local time tm structs.
 *  
 *  Inputs:
 *
 *      utc - pointer to 128-bit time to split into tm structs
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      timetm -  pointer to tm struct to receive time component of utc.
 *      tns -     pointer to longword to receive nanosecond within second
 *      inacctm - pointer to tm struct to receive inaccuracy component
 *      ins -     pointer to longword to receive inacc nanosecond
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */
int utc_localtime( struct tm *timetm, long *tns, struct tm *inacctm, long *ins, utc_t *utc)

{
    UTCValue	 timevalue, inaccvalue;
    struct ttinfo *ttisp;
    struct tm    atimetm;
    register int i;

    dce_tzset();

    if ((i = utc_comptime(&timevalue, &inaccvalue, (long *)NULL, utc)) < 0)
	return(i);

    splitinacc(&inaccvalue, inacctm, ins);

    if (find_ttis_given_utc (&ttisp, &timevalue))
	return(-1);

    splittime(&timevalue, (long) ttisp->tt_gmtoff, &atimetm, tns);

    atimetm.tm_isdst = ttisp->tt_isdst;
#ifdef TM_ZONE				/* some systems don't have these */
    atimetm.TM_ZONE = &lclstate.chars[ttisp->tt_abbrind];
#endif
#ifdef TM_GMTOFF
    atimetm.TM_GMTOFF = ttisp->tt_gmtoff;
#endif

    /* 
     * Set up the global variable tzname, same as standard localtime does
     * Let the OS take care of this, cause we don't want it mucking with
     * lclstate.chars[].
     * tzname[ttisp->tt_isdst] = &lclstate.chars[ttisp->tt_abbrind];
     */

    /* 
     * Now return those values caller has supplied pointers for
     */
    if (timetm != NULL)  *timetm = atimetm;

    return(0);
}
/* End of routine utc_localtime */


/* 
 *
 *	utc_localzone
 *
 *	Gets timezone label and offset from gmt, given utc
 *
 * Inputs:
 *      tzlen -   length of buffer tznam
 *      utc -    pointer to utc_t with current time
 *
 * Implicit Inputs:
 *      lclstate array
 *
 * Outputs:
 *	tznam -  pointer to char string long enough to hold the name
 *      tdf -  pointer to long with differential in seconds east of GMT
 *      isdst -  pointer to int = 0 (standard time) or 1 (daylight time)
 *
 * Returns:
 *	0  if translation successful
 *	-1 if failure
 *
 * Side Effects:
 *
 */

int utc_localzone ( char *tznam, size_t tzlen, long *tdf, int *isdst, utc_t *utc)

{
    UTCValue timevalue;
    struct ttinfo *ttisp;

    /* 
     * Break up the components of the utc passed in
     */
    if (utc_comptime (&timevalue, (UTCValue *)NULL, (long *)NULL, utc))
	return (-1);
    
    /* 
     * If timevalue is negative, it represents a time long before the
     * beginning of the table; let find_ttis_given_local figure out
     * what to do about it.
     */
    if (IsUTCneg(&timevalue))
	UTCzero(&timevalue);
    
    dce_tzset();

    /* 
     * Find timezone entry for time we've calc'd.
     */
    if (find_ttis_given_utc(&ttisp, &timevalue))
	return(-1);

    if (tdf != NULL)
	*tdf = ttisp->tt_gmtoff;
    
    if ((tznam != NULL) && (tzlen != 0))
	(void) strncpy (tznam, &lclstate.chars[ttisp->tt_abbrind], tzlen);

    if (isdst != NULL)
	*isdst = ttisp->tt_isdst;

    return (0);
}
/* end of utc_localzone */


/* 
 *
 *	utc_anyzone
 *
 *	Gets timezone label and offset from gmt, given utc
 *
 * Inputs:
 *      tzlen -   length of buffer tznam
 *      utc -     pointer to utc_t with current time
 *
 * Outputs:
 *	tznam -  pointer to char string long enough to hold the name
 *      tdf    -  pointer to long with differential in seconds east of GMT
 *      isdst  -  pointer to int = 0 (standard time) or 1 (daylight time)
 *
 * Returns:
 *	0  if translation successful
 *	-1 if failure
 *
 * Side Effects:
 *
 */

int utc_anyzone ( char *tznam, size_t tzlen, long *tdf, int *isdst, utc_t *utc)

{
    char temp_name[80];
    long temp_tdf;
    
    /* 
     * Get the tdf out of the utc struct, convert from utc's minutes 
     * to the interface's seconds.
     */
    if (utc_comptime ((UTCValue *)0, (UTCValue *)0, &temp_tdf, utc))
	return (-1);

    if (tdf != NULL)
	*tdf = temp_tdf;

    if ((tznam != NULL) && (tzlen != 0))
    {
	if (temp_tdf >= 0) {
	    (void) sprintf(temp_name, "GMT+%d:%02d", (temp_tdf / SECS_PER_HOUR),
			   (temp_tdf % SECS_PER_HOUR)/SECS_PER_MIN);
	} else {
	    (void) sprintf(temp_name, "GMT-%d:%02d", (-temp_tdf / SECS_PER_HOUR),
			   (-temp_tdf % SECS_PER_HOUR)/SECS_PER_MIN);
	}
	(void) strncpy (tznam, temp_name, tzlen);
    }

    if (isdst != NULL)
	*isdst = -1;		/* we don't know if dst or not */

    return (0);
}



/* 
 *
 *	utc_gmtzone
 *
 *	Gets timezone label and zero offset from gmt, given utc.
 *      This routine is here only for completeness: its outputs are 
 *      obviously tdf = 0 and tznam = 'GMT'
 *
 * Inputs:
 *      tzlen -   length of buffer tznam
 *      utc -	  pointer to utc_t with current time
 *
 * Outputs:
 *	tznam -  pointer to char string long enough to hold the name
 *      tdf -     pointer to long with differential
 *      isdst -   pointer to int = 0 (standard time) or 1 (daylight time)
 *
 * Returns:
 *	0  always (can't fail)
 *
 * Side Effects:
 *
 */

int utc_gmtzone ( char *tznam, size_t tzlen, long *tdf, int *isdst, utc_t *utc)

{
    if (tdf != NULL)
	*tdf = 0;

    if ((tznam != NULL) && (tzlen != 0))
	(void) strncpy (tznam, "GMT", tzlen);

    if (isdst != NULL)
	*isdst = 0;		/* gmt is always standard time */

    return (0);
}



/*
 *++
 *  utc_mklocaltime()
 *
 *  Functional Description:
 *
 *      Converts a localtime tm structs and nanoseconds into an 128-bit UTC
 *      time.
 *  
 *  Inputs:
 *
 *      timetm - pointer to tm struct with time component
 *      tns - longword with nanosecond within second
 *      inacctm - pointer to tm struct with inaccuracy component
 *      ins - longword with inacc nanosecond
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      utc - pointer to 128-bit UTC structure to receive result.
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */
int utc_mklocaltime( utc_t *utc, struct tm *timetm, long tns, struct tm *inacctm, long ins)

{
    UTCValue timevalue, inaccvalue;
    UTCValue utcTemp2, secTemp;
    struct ttinfo *ttisp;

    mergeinacc(inacctm, ins, &inaccvalue); 

    if (mergetime(timetm, tns, &timevalue, &secTemp) < 0)
	return(-1);

    if (find_ttis_given_local(&ttisp, &secTemp, timetm->tm_isdst))
	return(-1);

    emul((long) ttisp->tt_gmtoff, K_100NS_PER_SEC, &utcTemp2);
    UTCsub(&timevalue, &utcTemp2, &timevalue);

    return (utc_mkcomptime (utc,
			    &timevalue,
			    &inaccvalue,
			    (long) ttisp->tt_gmtoff));
}
/* End of routine utc_mklocaltime */

/*
 *++
 *  utc_gmtime()
 *
 *  Functional Description:
 *
 *      Converts a 128-bit UTC time into gmt time tm structs.
 *  
 *  Inputs:
 *
 *      utc - pointer to 128-bit time to split into tm structs
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      timetm - pointer to tm struct to receive time component of utc.
 *      tns - pointer to longword to receive nanosecond within second
 *      inacctm - pointer to tm struct to receive inaccuracy component
 *      ins - pointer to longword to receive inacc nanosecond
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */
int utc_gmtime( struct tm *timetm, long *tns, struct tm *inacctm, long *ins, utc_t *utc)

{
    UTCValue timevalue, inaccvalue;
    register int i;

    if ((i = utc_comptime(&timevalue, &inaccvalue, (long *)NULL, utc)) < 0)
	return(i);

    splitinacc(&inaccvalue, inacctm, ins);

    splittime(&timevalue, 0L, timetm, tns);

    if (timetm != NULL) {
        timetm->tm_isdst = 0;
#ifdef TM_ZONE				/* some systems don't have these */
        timetm->TM_ZONE = GMT; 
#endif
#ifdef TM_GMTOFF
        timetm->TM_GMTOFF = 0;
#endif
    }

    /*
     * Let the OS take care of this.
     * tzname[0] = GMT;
     */

    return(0);

}
/* End of routine utc_gmtime */

/*
 *++
 *  utc_mkgmtime()
 *
 *  Functional Description:
 *
 *      Converts a GMT tm struct and nanoseconds into an 128-bit UTC time.
 *  
 *  Inputs:
 *
 *      timetm - pointer to tm struct with time component of utc.
 *      tns - longword of nanosecond within second
 *      inacctm - pointer to tm struct with inaccuracy component
 *      ins - longword of inacc nanosecond
 * 
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      utc - pointer to 128-bit time to split into tm structs
 *
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */
int utc_mkgmtime( utc_t *utc, struct tm *timetm, long tns, struct tm *inacctm, long ins)

{
    UTCValue	timevalue, inaccvalue, secTemp;

    mergeinacc(inacctm, ins, &inaccvalue); 

    if (mergetime(timetm, tns, &timevalue, &secTemp) < 0)
	return(-1);

    return(utc_mkcomptime(utc, &timevalue, &inaccvalue, 0L));

}
/* End of routine utc_mkgmtime */

/*
 *++
 *  utc_anytime()
 *
 *  Functional Description:
 *
 *      Converts a 128-bit UTC time into a tm struct using the TDF
 *      contained in the timestamp.
 *  
 *  Inputs:
 *
 *      utc - pointer to 128-bit time to split into tm structs
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      timetm - pointer to tm struct to receive time component of utc.
 *      tns - pointer to longword to receive nanosecond within second
 *      inacctm - pointer to tm struct to receive inaccuracy component
 *      ins - pointer to longword to receive inacc nanosecond
 *	tdf - pointer to integer to receive seconds east of GMT
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */
int utc_anytime( struct tm *timetm, long *tns, struct tm *inacctm, long *ins, long *tdf, utc_t *utc)

{
    UTCValue timevalue, inaccvalue;
    register int i;
    long atdf;

    if ((i = utc_comptime(&timevalue, &inaccvalue, &atdf, utc)) < 0)
	return(i);

    splitinacc(&inaccvalue, inacctm, ins);

    splittime(&timevalue, atdf, timetm, tns);

    if (timetm != NULL) {
        timetm->tm_isdst = -1;
#ifdef TM_ZONE				/* some systems don't have these */
        if (atdf >= 0) {
	    (void) sprintf( gmt_relative, "GMT+%d:%02d",
                            (atdf / SECS_PER_HOUR),
			    (atdf % SECS_PER_HOUR)/SECS_PER_MIN );
        } else {
	    (void) sprintf( gmt_relative, "GMT-%d:%02d",
                            (-atdf / SECS_PER_HOUR),
		            (-atdf % SECS_PER_HOUR)/SECS_PER_MIN );
        }
        timetm->TM_ZONE = gmt_relative;
#endif
#ifdef TM_GMTOFF
        timetm->TM_GMTOFF = atdf;
#endif
    }

    if (tdf != NULL)
	*tdf = atdf;

    return(0);

}
/* End of routine utc_anytime */

/*
 *++
 *  utc_mkanytime()
 *
 *  Functional Description:
 *
 *      Converts tm structs in an arbitrary timezone into an 128-bit UTC time.
 *  
 *  Inputs:
 *
 *      timetm - pointer to tm struct with time component of utc.
 *      tns - longword of nanosecond within second
 *      inacctm - pointer to tm struct with inaccuracy component
 *      ins - longword of inacc nanosecond
 *	tdf - integer seconds east of GMT
 * 
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      utc - pointer to receive 128-bit time struct.
 *
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */
int utc_mkanytime( utc_t *utc, struct tm *timetm, long tns, struct tm *inacctm, long ins, long tdf)

{
    UTCValue	timevalue, inaccvalue, utcTemp;

    mergeinacc(inacctm, ins, &inaccvalue);

    if (mergetime(timetm, tns, &timevalue, &utcTemp) < 0)
	return(-1);

    emul(tdf, (long)K_100NS_PER_SEC, &utcTemp);
    UTCsub(&timevalue, &utcTemp, &timevalue);

    return(utc_mkcomptime(utc, &timevalue, &inaccvalue, tdf));

}
/* End of routine utc_mkanytime */

/*
 *++
 *  utc_reltime()
 *
 *  Functional Description:
 *
 *      Converts a relative 128-bit UTC time into tm structs.
 *  
 *  Inputs:
 *
 *      utc - pointer to relative 128-bit time to split into tm structs
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      timetm - pointer to tm struct to receive time component of utc.
 *      tns - pointer to longword to receive nanosecond within second
 *      inacctm - pointer to tm struct to receive inaccuracy component
 *      ins - pointer to longword to receive inacc nanosecond
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */
int utc_reltime( struct tm *timetm, long *tns, struct tm *inacctm, long *ins, utc_t *utc)

{
    UTCValue timevalue, inaccvalue;
    long temp;
    register int i;

    if (utc == NULL)
	return(-1);

    if ((i = utc_comptime(&timevalue, &inaccvalue, (long *)NULL, utc)) < 0)
	return(i);

    splitinacc(&inaccvalue, inacctm, ins);

    UTCiDiv(&timevalue, K_100NS_PER_SEC, &timevalue, tns);
    if (tns != NULL)
	*tns *= 100;
    if (timetm != NULL) {
        UTCiDiv(&timevalue, SECS_PER_MIN, &timevalue, &temp);
        timetm->tm_sec = temp;
        UTCiDiv(&timevalue, MINS_PER_HOUR, &timevalue, &temp);
        timetm->tm_min = temp;
        timetm->tm_hour = ((long) UTClow(&timevalue)) % HOURS_PER_DAY;
        UTClow(&timevalue) = ((long) UTClow(&timevalue)) / HOURS_PER_DAY;
        timetm->tm_mday = -1;
        timetm->tm_mon = 0;
        timetm->tm_year = 0;
        timetm->tm_yday = (int) UTClow(&timevalue);
        timetm->tm_isdst = 0;
#ifdef TM_ZONE				/* some systems don't have these */
        timetm->TM_ZONE = rel; 
#endif
#ifdef TM_GMTOFF
        timetm->TM_GMTOFF = 0;
#endif
    }
    
    return(0);

}
/* End of routine utc_reltime */

/*
 *++
 *  utc_mkreltime()
 *
 *  Functional Description:
 *
 *      Converts a relative tm struct and nanoseconds into an 128-bit UTC time.
 *  
 *  Inputs:
 *
 *      timetm - pointer to tm struct with time component of utc.
 *      tns - longword of nanosecond within second
 *      inacctm - pointer to tm struct with inaccuracy component
 *      ins - longword of inacc nanosecond
 * 
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      utc - pointer to 128-bit time to split into tm structs
 *
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */
int utc_mkreltime( utc_t *utc, struct tm *timetm, long tns, struct tm *inacctm, long ins)

{
    UTCValue timevalue, inaccvalue;
    long t100ns;

    mergeinacc(inacctm, ins, &inaccvalue); 

    if (timetm == NULL)
	return(-1);

    emul((long) (timetm->tm_yday * HOURS_PER_DAY + timetm->tm_hour),
         (long)MINS_PER_HOUR, &timevalue);
    UTCiAdd(&timevalue, timetm->tm_min, &timevalue);
    UTCiMul(&timevalue, SECS_PER_MIN, &timevalue);
    UTCiAdd(&timevalue, timetm->tm_sec, &timevalue);
    UTCiMul(&timevalue, K_100NS_PER_SEC, &timevalue);
    t100ns = tns/100;
    UTCiAdd(&timevalue, t100ns, &timevalue);

    return(utc_mkcomptime(utc, &timevalue, &inaccvalue, 0L));

}
/* End of routine utc_mkreltime */

/*
 *++
 *  utc_addtime()
 *
 *  Functional Description:
 *
 *      Compute the sum of two relative times or the sum of a 
 *	relative time and an absolute time.
 *  
 *  Inputs:
 *
 *      utc1 - pointer to 128-bit time 
 *      utc2 - pointer to 128-bit time
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      result - pointer to receive the sum.
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */
int utc_addtime( utc_t *result, utc_t *utc1, utc_t *utc2)

{
    UTCValue	time1, inacc1, time2, inacc2, resulttime, resultinacc;
    long        tdf;
    register int i;
    
    if ((i = utc_comptime(&time1, &inacc1, &tdf, utc1)) < 0)
	return(i);
    if ((i = utc_comptime(&time2, &inacc2, (long *)NULL, utc2)) < 0)
	return(i);

    if ((IsInfiniteInacc(&inacc1)) || (IsInfiniteInacc(&inacc2))) {
	UTCassign (&resultinacc,K_INFINITE_INACC_LO,K_INFINITE_INACC_HI);
    } else
        UTCadd(&inacc1, &inacc2, &resultinacc);

    UTCadd(&time1, &time2, &resulttime);

    return(utc_mkcomptime(result, &resulttime, &resultinacc, tdf));

}
/* End of routine utc_addtime */

/*
 *++
 *  utc_subtime()
 *
 *  Functional Description:
 *
 *      Compute the difference of two relative times, the difference of
 *	an absolute time and a relative time or the difference of two
 *      absolute times
 *  
 *  Inputs:
 *
 *      utc1 - pointer to 128-bit time 
 *      utc2 - pointer to 128-bit time
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      result - pointer to receive the sum.
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */
int utc_subtime( utc_t *result, utc_t *utc1, utc_t *utc2)

{
    UTCValue	time1, inacc1, time2, inacc2, resulttime, resultinacc;
    long        tdf;
    register int i;
    
    if ((i = utc_comptime(&time1, &inacc1, &tdf, utc1)) < 0)
	return(i);
    if ((i = utc_comptime(&time2, &inacc2, (long *)NULL, utc2)) < 0)
	return(i);

    if ((IsInfiniteInacc(&inacc1)) || (IsInfiniteInacc(&inacc2))) {
	UTCassign (&resultinacc,K_INFINITE_INACC_LO,K_INFINITE_INACC_HI);
    } else
        UTCadd(&inacc1, &inacc2, &resultinacc);

    UTCsub(&time1, &time2, &resulttime);

    return(utc_mkcomptime(result, &resulttime, &resultinacc, tdf));

}
/* End of routine utc_subtime */

/*
 *++
 *  utc_multime()
 *
 *  Functional Description:
 *
 *      Compute the product of a relative time and an integer.
 *  
 *  Inputs:
 *
 *      utc1 - pointer to 128-bit time 
 *      factor - integer to multiply by
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      result - pointer to receive the sum.
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */
int utc_multime( utc_t *result, utc_t *utc1, long factor)

{
    UTCValue	time1, inacc1, resulttime, resultinacc;
    long        absfactor;
    long	tdf;
    register int i;

    if ((i = utc_comptime(&time1, &inacc1, &tdf, utc1)) < 0)
	return(i);

    absfactor = (factor < 0) ? -factor : factor;
#if LONG_BIT >= 64
    resultinacc = (inacc1 >> 32) * absfactor;
    if ( resultinacc > ((K_INFINITE_INACC_LO<<32)|K_INFINITE_INACC_HI) ){
        resultinacc = 0xffffffffffffffff;
    } else {
        UTCiMul(&inacc1, absfactor, &resultinacc);
    };
#else	/* LONG_BIT */
    emul((long)inacc1.hi, absfactor, &resultinacc);
    if (resultinacc.hi != 0 ||
        resultinacc.lo > 0xffff) {
	resultinacc.hi = resultinacc.lo = 0xffffffffUL;
    } else {
	UTCiMul(&inacc1, absfactor, &resultinacc);
    };
    emul((long)time1.hi, factor, &resulttime);
    if ((long) resulttime.hi < -1 || (long) resulttime.hi > 0 ||
        (resulttime.hi == 0 && (long) resulttime.lo < 0) ||
        ((long) resulttime.hi == -1 && (long) resulttime.lo >= 0)) {
	return(-1);
    }
#endif 	/* LONG_BIT */

    UTCiMul(&time1, factor, &resulttime);
    if (!IsUTCneg(&time1) ^ (factor >= 0) ^ (IsUTCneg(&resulttime)))
	return(-1);

    return(utc_mkcomptime(result, &resulttime, &resultinacc, tdf));

}
/* End of routine utc_multime */

/*
 *++
 *  utc_mulftime()
 *
 *  Functional Description:
 *
 *      Compute the product of a relative time and an integer.
 *  
 *  Inputs:
 *
 *      utc1 -  real to 128-bit time 
 *      factor - integer to multiply by
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      result - pointer to receive the sum.
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */
int utc_mulftime( utc_t *result, utc_t *utc1, double factor)

{
    UTCValue resulttime, resultinacc;
    double abs_factor;
#if !defined(__hpux)
    double fabs();
#endif
    double k_2_64 = 65536.0 * 65536.0 * 65536.0 * 65536.0;
    register int logf;
    int time_neg = 0;
    int time_round = 0;
    Bits128 Inacc128;
    Bits128 Time128;
    Bits64 intf;
    long tdf;
    register int i;

#if defined(__hpux)
    abs_factor = utc_fabs(factor);
#else
    abs_factor = fabs(factor);
#endif

    if ((i = utc_comptime(&resulttime, &resultinacc, &tdf, utc1)) < 0)
	return(i);

    if (abs_factor >= k_2_64) {
	return(-1);
    }
    if (abs_factor * k_2_64 < 1.0) {

	UTCassign (&resultinacc,1,0);
	UTCzero(&resulttime);

	return(utc_mkcomptime(result, &resulttime, &resultinacc, tdf));
    }

    logf = 0;
    while (abs_factor >= 1.0) {
	logf++;
	abs_factor = abs_factor / 2;
    }
    while (abs_factor < 0.5) {
	logf--;
	abs_factor = abs_factor * 2;
    }

#if LONG_BIT >= 64

    abs_factor *= 65536.0 * 65536.0;
    intf = (unsigned long)abs_factor;
    abs_factor -= (double)intf;
    abs_factor *= 65536.0 * 65536.0;
    intf = (intf << 32) + (unsigned long)abs_factor;

    if (time_neg = IsUTCneg(&resulttime))
        UTCnegate(&resulttime, &resulttime);

    ueemul(&intf, &resultinacc, &Inacc128);
    ueemul(&intf, &resulttime, &Time128);

    if (logf == 64) {
        if (Inacc128.hi != 0) {
            resultinacc = 0xffffffff;
        } else {
            resultinacc = Inacc128.lo;
        }
        if (Time128.hi != 0)
            return(-1);
        resulttime = Time128.lo;
    } else if (logf > 0) {
        if (Inacc128.hi >> (64 - logf)) {
            resultinacc = 0xffffffff;
        } else {
            resultinacc = (Inacc128.hi << logf) +
                             (Inacc128.lo >> (64 - logf));
        }
        if (Time128.hi >> (64 - logf))
            return(-1);
        resulttime = (Time128.hi << logf) +
                        (Time128.lo >> (64 - logf));
        time_round = (Time128.lo >> (63 - logf)) & 1;
    } else if (logf == 0) {
        resultinacc = Inacc128.hi;
        resulttime = Time128.hi;
        time_round = (Time128.lo >> 63) & 1;
    } else if (logf > -64) {
        resultinacc = Inacc128.hi >> (-64 - logf);
        resulttime = Time128.hi >> (-64 - logf);
        time_round = (Time128.hi >> (-65 - logf)) & 1;
#else	/* LONG_BIT */

    abs_factor *= 65536.0;
    intf.hi = (unsigned long)abs_factor;
    abs_factor -= (double)intf.hi;
    abs_factor *= 65536.0;    
    intf.hi = (intf.hi << 16) + (unsigned long)abs_factor;
    abs_factor -= (double)((unsigned long)abs_factor);
    abs_factor *= 65536.0;
    intf.lo = (unsigned long)abs_factor;
    abs_factor -= (double)intf.lo;
    abs_factor *= 65536.0;
    intf.lo = (intf.lo << 16) + (unsigned long)abs_factor;    

    if (time_neg = IsUTCneg(&resulttime))
	UTCnegate(&resulttime, &resulttime);

    ueemul(&intf, &resultinacc, &Inacc128);
    ueemul(&intf, &resulttime, &Time128);

    if (logf == 64) {
	if (Inacc128.hihi != 0 || Inacc128.hilo != 0) {
	    resultinacc.hi = K_INFINITE_INACC_HI;
	    resultinacc.lo = K_INFINITE_INACC_LO;
	} else {
	    resultinacc.hi = Inacc128.lohi;
	    resultinacc.lo = Inacc128.lolo;
	}
	if (Time128.hihi != 0 || Time128.hilo != 0)
	    return(-1);
	resulttime.hi = Time128.lohi;
	resulttime.lo = Time128.lolo;
	time_round = 0;
    } else if (logf > 32) {
	if (Inacc128.hihi != 0 || Inacc128.hilo >> (64 - logf)) {
	    resultinacc.hi = K_INFINITE_INACC_HI;
	    resultinacc.lo = K_INFINITE_INACC_LO;
	} else {
	    resultinacc.hi = (Inacc128.hilo << (logf - 32)) +
			     (Inacc128.lohi >> (64 - logf));
	    resultinacc.lo = (Inacc128.lohi << (logf - 32)) +
	    		     (Inacc128.lolo >> (64 - logf));
	}
	if (Time128.hihi != 0 || Time128.hilo >> (64 - logf))
	    return(-1);
	resulttime.hi = (Time128.hilo << (logf - 32)) +
			(Time128.lohi >> (64 - logf));
	resulttime.lo = (Time128.lohi << (logf - 32)) +
			(Time128.lolo >> (64 - logf));
	time_round = (Time128.lolo >> (63 - logf)) & 1;
    } else if (logf == 32) {
	if (Inacc128.hihi != 0) {
	    resultinacc.hi = K_INFINITE_INACC_HI;
	    resultinacc.lo = K_INFINITE_INACC_LO;
	} else {
	    resultinacc.hi = Inacc128.hilo;
	    resultinacc.lo = Inacc128.lohi;
	}
	if (Time128.hihi != 0)
	    return(-1);
	resulttime.hi = Time128.hilo;
	resulttime.lo = Time128.lohi;
	time_round = (Time128.lolo >> 31) & 1;
    } else if (logf > 0) {
	if (Inacc128.hihi >> (32 - logf)) {
	    resultinacc.hi = K_INFINITE_INACC_HI;
	    resultinacc.lo = K_INFINITE_INACC_LO;
	} else {
	    resultinacc.hi = (Inacc128.hihi << logf) +
			     (Inacc128.hilo >> (32 - logf));
	    resultinacc.lo = (Inacc128.hilo << logf) +
			     (Inacc128.lohi >> (32 - logf));
	}
	if (Time128.hihi >> (32 - logf))
	    return(-1);
	resulttime.hi = (Time128.hihi << logf) +
			(Time128.hilo >> (32 - logf));
	resulttime.lo = (Time128.hilo << logf) +
			(Time128.lohi >> (32 - logf));
	time_round = (Time128.lohi >> (31 - logf)) & 1;
    } else if (logf == 0) {
	resultinacc.hi = Inacc128.hihi;
	resultinacc.lo = Inacc128.hilo;
	resulttime.hi = Time128.hihi;
	resulttime.lo = Time128.hilo;
	time_round = (Time128.lohi >> 31) & 1;
    } else if (logf > -32) {
	resultinacc.hi = Inacc128.hihi >> (-logf);
	resultinacc.lo = (Inacc128.hihi << (32 + logf)) +
		         (Inacc128.hilo >> (-logf));
	resulttime.hi = Time128.hihi >> (-logf);
	resulttime.lo = (Time128.hihi << (32 + logf)) +
			(Time128.hilo >> (-logf));
	time_round = (Time128.hilo >> (-1 - logf)) & 1;
    } else if (logf == -32) {
	resultinacc.hi = 0;
	resultinacc.lo = Inacc128.hihi;
	resulttime.hi = 0;
	resulttime.lo = Time128.hihi;
	time_round = (Time128.hilo >> 31) & 1;
    } else if (logf > -64) {
	resultinacc.hi = 0;
	resultinacc.lo = Inacc128.hihi >> (-32 - logf);
	resulttime.hi = 0;
	resulttime.lo = Time128.hihi >> (-32 - logf);
	time_round = (Time128.hihi >> (-33 - logf)) & 1;
#endif	/* LONG_BIT */
    } else {
        UTCzero(&resultinacc);
        UTCzero(&resulttime);
    }	

    UTCiAdd(&resultinacc, 1, &resultinacc);

    if (time_round)
	UTCiAdd(&resulttime, 1, &resulttime);

    if (IsUTCneg(&resulttime))
	return(-1);

    if (time_neg^(factor < 0))
	UTCnegate(&resulttime, &resulttime);

    return(utc_mkcomptime(result, &resulttime, &resultinacc, tdf));

}
/* End of routine utc_mulftime */

/*
 *++
 *  utc_abstime()
 *
 *  Functional Description:
 *
 *      Compute the absolute value of a relative times.
 *  
 *  Inputs:
 *
 *      utc1 - pointer to 128-bit time 
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      result - pointer to receive the absolute value.
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 * 8/22/94 CBrooks - if we recieve a time value that is the largest 
 *                   2's complement negative number we can handle,
 *		     we return an error, since we can't represent 
 *                   this as a positive number
 *
 *--
 */
int utc_abstime( utc_t *result, utc_t *utc1)

{
    UTCValue	resulttime, resultinacc;
    long	tdf;
    register int i;
    UTCValue negTime = UTCinit( 0x80000000UL, 0UL) ;
    
    if ((i = utc_comptime(&resulttime, &resultinacc, &tdf, utc1)) < 0)
	return(i);

    if (IsUTCeq(&resulttime, &negTime))
	return -1 ;
    UTCabsVal(&resulttime, &resulttime);

    return(utc_mkcomptime(result, &resulttime, &resultinacc, tdf));

}
/* End of routine utc_abstime */

/*
 *++
 *  utc_cmpmidtime()
 *
 *  Functional Description:
 *
 *      Compare the midpoints of two absolute or relative times,
 *      ignoring inaccuraccy information.
 *  
 *  Inputs:
 *
 *      utc1 - pointer to 128-bit time 
 *      utc2 - pointer to 128-bit time
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      relation - pointer to reciveve the relation
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */
int utc_cmpmidtime( enum utc_cmptype *relation, utc_t *utc1, utc_t *utc2)

{
    UTCValue time1, time2;
    register int i;

    if ((i = utc_comptime(&time1, (UTCValue *)NULL, (long *)NULL, utc1)) < 0)
	return(i);
    if ((i = utc_comptime(&time2, (UTCValue *)NULL, (long *)NULL, utc2)) < 0)
	return(i);

    if (relation == NULL)
	return(-1);

    if (IsUTClt(&time1, &time2)) {
   	*relation = utc_lessThan;
    } else {
        if (IsUTCgt(&time1, &time2))
	    *relation = utc_greaterThan;
	else
	    *relation = utc_equalTo;    
    }

    return(0);

}
/* End of routine utc_cmpmidtime */

/*
 *++
 *  utc_cmpintervaltime()
 *
 *  Functional Description:
 *
 *      Compare the two absolute or relative times, including
 *      inaccuraccy information.
 *  
 *  Inputs:
 *
 *      utc1 - pointer to 128-bit time 
 *      utc2 - pointer to 128-bit time
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      relation - pointer to reciveve the relation
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */
int utc_cmpintervaltime( enum utc_cmptype *relation, utc_t *utc1, utc_t *utc2)

{
    UTCValue time1, inacc1, time2, inacc2;
    register int i;

    if ((i = utc_comptime(&time1, &inacc1, (long *)NULL, utc1)) < 0)
	return(i);
    if ((i = utc_comptime(&time2, &inacc2, (long *)NULL, utc2)) < 0)
	return(i);

    if (relation == NULL)
	return(-1);

    *relation = utc_indeterminate;

    if (IsUTClt(&time1, &time2)) {
        UTCadd(&time1, &inacc1, &inacc1);
        UTCsub(&time2, &inacc2, &inacc2);
        if (IsUTClt(&inacc1, &inacc2))
	    *relation = utc_lessThan;
    }
    if (IsUTCgt(&time1, &time2)) {
        UTCsub(&time1, &inacc1, &inacc1);
        UTCadd(&time2, &inacc2, &inacc2);
        if (IsUTCgt(&inacc1, &inacc2))
            *relation = utc_greaterThan;
    }
    if (IsUTCeq(&time1, &time2)) {
	if (IsUTCzero(&inacc1) && IsUTCzero(&inacc2))
    	    *relation = utc_equalTo;
    }

    return(0);

}
/* End of routine utc_cmpintervaltime */

/*
 *++
 *  utc_getnextzone()
 *
 *  Functional Description:
 *
 *      Returns 128-bit UTC time of next timezone transition.
 *  
 *  Inputs:
 *
 *	utc - pointer to time to find next transition after
 *
 *  Implicit Inputs:
 *
 *	local time database
 *
 *  Outputs:
 *
 *      next - pointer to 128-bit time of next timezone transition
 *	       after utc.
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */     
int utc_getnextzone( utc_t *next, utc_t *utc)

{
    UTCValue	timevalue, inaccvalue;
    UTCValue	nexttime, nextinacc;
    UTCValue	utcTemp;
    long	temp;
    long	tdf;
    register struct ttinfo *ttisp;
    register int i;

    dce_tzset();

    if ((i = utc_comptime(&timevalue, &inaccvalue, &tdf, utc)) < 0)
	return(i);

    UTCzero(&nextinacc);

    UTCadd(&timevalue, &diff_UTC_UNIX_ticks, &utcTemp);
    UTCiDiv(&utcTemp, K_100NS_PER_SEC, &utcTemp, &temp);

    if (lclstate.timecnt > 1) {
	for (i = 0; i < lclstate.timecnt-1; ++i)
	    if ((long) UTClow(&utcTemp) < (long) lclstate.ats[i]) {
		emul((long)lclstate.ats[i], K_100NS_PER_SEC, &nexttime);
		UTCadd(&nexttime, &diff_UNIX_UTC_ticks, &nexttime);
		ttisp = &lclstate.ttis[lclstate.types[i-1]];
		(void) utc_mkcomptime(utc, &timevalue, &inaccvalue,
				      (long) ttisp->tt_gmtoff);
		ttisp = &lclstate.ttis[lclstate.types[i]];
		return(utc_mkcomptime(next, &nexttime, &nextinacc,
				      (long) ttisp->tt_gmtoff));
	}
    } else {
	ttisp = &lclstate.ttis[lclstate.types[0]];
	(void) utc_mkcomptime(utc, &timevalue, &inaccvalue,
			      (long) ttisp->tt_gmtoff);
    }

    return(utc_mkcomptime(next, &timevalue, &nextinacc, tdf));

}
/* End of routine utc_getnextzone */

/*
 *++
 *  utc_settdf()
 *
 *  Functional Description:
 *
 *      Returns 128-bit UTC that contains the specified time zone (tdf).
 *  
 *  Inputs:
 *
 *	utc1 - pointer to the utc whose tdf is to be changed.
 *	tdf  - the new tdf to include in the output tdf
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      result - the result utc with the new timezone (tdf) and the same
 *		GMT.
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */     
int utc_settdf( utc_t *result, utc_t *utc1, long tdf)

{
    UTCValue timevalue, inaccvalue;
    register int i;

    if ((i = utc_comptime(&timevalue, &inaccvalue, (long *)NULL, utc1)) < 0)
	return(i);

    return(utc_mkcomptime(result, &timevalue, &inaccvalue, tdf));
}

/*
 *++
 *  utc_getusertime()
 *
 *  Functional Description:
 *
 *      Returns the current time in 128-bit UTC format with the process TDF
 *  
 *  Inputs:
 *
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *      utc - pointer to 128-bit UTC time
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 *
 *--
 */
int utc_getusertime( utc_t *utc )
{
    utc_t junk ;

    if (utc_gettime(utc) != 0)
	return(-1);
    /*
     * The following call depends on the fact that utc_getnextzone
     * changes the tdf of the second argument to the current processes
     * tdf.
     */
    return(utc_getnextzone((utc_t *)&junk, utc));
}

/*
 *  utc_getTCPtime()
 *
 *  Functional Description:
 *
 *      Obtains 128-bit UTC time from hostname over TCP socket to 
 *      inetd time service. 
 *
 *  Inputs:
 *  
 *      hostname
 *
 *  Outputs:
 *
 *      utc - pointer to 128-bit utc time.
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 */

int
utc_getTCPtime(char *host, utc_t *utc) 
{
int fd, i;
time_t temptime, timeval;
timespec_t timeval_t, timeval_t_inacc;

    /*
     * Initiate the connection and receive the 32-bit time value.
     * (in network byte order) from the server.
     */
    if ((fd = tcp_open(host, "time", 0)) < 0 )
	return (-1);

    if (i = read(fd, (char *) &temptime, 100) == 0) {
        close(fd);
	return (-1);
    }
    
    timeval = ntohl(temptime);
    /*
     * We've got the UTC time (since 1900).  Subtract 70 yrs of seconds
     * to get the time since the epoc.
     */
    timeval -= 2208988800;
    timeval_t.tv_sec = timeval;
    timeval_t.tv_nsec = 0;

    /* convert timeval_t into 128-bit UTC time */
    timeval_t_inacc.tv_sec = -1;

    if ((utc_mkbintime(utc, &timeval_t, &timeval_t_inacc, 0)) < 0) {
	close(fd);
	return(-1);
    }
    close(fd);
    return (0);
}

/*
 *  tcp_open()
 *
 *  Functional Description:
 *     Open the TCP socket to the specified port.
 *
 *     "host"	        Host name or IP address of target host
 *     "service"	name of service being requested (NULL iff "port" > 0)
 *     "port"   	if == 0, use port# of "service"
 *	        	f < 0, bind a local reserved port
 *	        	if > 0, specifies the port # of server
 *  Inputs:
 *
 *      hostname
 *      service
 *      port #
 *
 *  Implicit Inputs:
 *
 *  Outputs:
 *
 *      file descriptor to open port
 *      
 *  Value Returned:
 *
 *      0 success, -1 failure
 *
 *  Side Effects:
 *
 */     

int
tcp_open(char *host, char *service, int port)
{
int fd, resvport;
unsigned long inaddr;
struct servent *sp;
struct hostent *hp;
struct sockaddr_in tcp_srv_addr; /* server's IP socket addr */

    /*
     * Initialize the server's internet address structure.
     * We'll store the actual 4-byte internet address and the
     * 2-byte port# below.
     */
    bzero((char *) &tcp_srv_addr, sizeof(tcp_srv_addr));
    tcp_srv_addr.sin_family = AF_INET;

    if ((sp = getservbyname(service, "tcp")) == NULL)
	return (-1);

    tcp_srv_addr.sin_port = sp->s_port; /* Service's value */

    /*
     * First try to convert the host name as a dotted-decimal number.
     * Only if that fails, do we call gethostbyname().
     */
    if ((inaddr = inet_addr(host)) != INADDR_NONE)
    {
	bcopy((char *) &inaddr, (char *) &tcp_srv_addr.sin_addr,
	    sizeof(inaddr));
    }
    else
    {
	if ((hp = gethostbyname(host)) == NULL)
	    return (-1);

	bcopy(hp->h_addr, (char *) &tcp_srv_addr.sin_addr, hp->h_length);
    }

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	return (-1);

    /*
     * Connect to the server.
     */
    if (connect(fd, (struct sockaddr *) &tcp_srv_addr,
	sizeof(tcp_srv_addr)) < 0)
    {
	close(fd);
	return (-1);
    }

    return fd;
}

#ifndef HPDCE_FIX_CHFtsXXXXX
/*
 *  utc_gettime_noshm(utc_t *)
 *
 *  Functional Description:
 *      utc_gettime_noshm - Returns the current system time
 *                          as a binary timestamp.
 *  include:
 *      <dce/utc.h>
 *  Input:
 *      None
 *
 *  Output:
 *      utc - pointer to 128-bit binary timestamp.
 *      
 *  Description:
 *      The utc_gettime_noshm() routine returns the current system time
 *      in a binary timestamp. The routine takes the TDF from the operating
 *      system's kernel; the TDF is specified in a system dependent manner.
 *      utc_gettime_noshm() bypasses the shared memory control implemented
 *      by dts which uses semaphores to lock/unlock read/write access to dts'
 *      notion of system time. Unlike utc_gettime, utc_gettime_noshm dose not
 *      adjust for the drift and resolution component of time based on inaccuracy
 *      calculations.
 *      
 *  Values Returned:
 *      0  - Indicates that the routine executed successfully. 
 *      -1 - Generic failure that indicates the time service cannot be accessed.
 */
int utc_gettime_noshm(utc_t *utc)
{
    struct timeval tv;
    struct timezone tzv;
    struct timespec ts;

    gettimeofday(&tv, &tzv);
    ts.tv_sec = tv.tv_sec;
    ts.tv_nsec = tv.tv_usec * 1000;
    return(utc_mkbintime(utc, &ts, (struct timespec *)NULL,
				 -tzv.tz_minuteswest*60));
}
#endif
