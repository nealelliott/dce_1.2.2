/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: unix_time.c,v $
 * Revision 1.1.10.2  1996/02/18  19:35:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:21  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:27:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:43  root]
 * 
 * Revision 1.1.7.6  1994/08/03  19:03:21  mccann
 * 	includes file cleanup
 * 	[1994/07/19  18:10:58  mccann]
 * 
 * Revision 1.1.7.5  1994/06/30  19:18:01  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:05:10  mccann]
 * 
 * Revision 1.1.7.4  1994/06/09  18:42:36  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:51  devsrc]
 * 
 * Revision 1.1.7.3  1994/04/29  15:55:59  peckham
 * 	Remove unnecessary conditionals.
 * 	Change gettimeofday() call to utc_gettime()/utc_bintime().
 * 	Use dcecdsmac.h macros for dce_svc_printf().
 * 	[1994/04/29  14:35:51  peckham]
 * 
 * Revision 1.1.7.1  1994/03/12  22:09:17  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:36:22  peckham]
 * 
 * Revision 1.1.5.4  1993/01/26  14:08:21  mfox
 * 	Fix ANSI C compiler warnings.
 * 	[1993/01/26  14:03:06  mfox]
 * 
 * Revision 1.1.5.3  1992/12/30  14:17:51  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:18:05  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  18:54:22  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:16:39  weisman]
 * 
 * Revision 1.1.2.2  1992/04/14  20:51:27  mfox
 * 	Fixes to allow CDS to compile cleanly with RIOS ANSI C compiler
 * 	[1992/04/14  19:42:39  mfox]
 * 
 * Revision 1.1  1992/01/19  15:19:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: unix_time.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 *
 */
#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <dce/sys_time.h>

#include <dce/assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <dce/utc.h>
#include <dce/dnsmessage.h>
#include <dns_malloc.h>

/* (M)milli (U)micro (N)nano */
#if !defined(NSEC_PER_SEC)			/* if defined in sys/time.h */
# define NSEC_PER_SEC	(1000*1000*1000L)	/* from POSIX 1003.4 */
#endif
#define	USEC_PER_SEC	(1000*1000L)
#define MSEC_PER_SEC	(1000L)
#define	SECS_PER_MIN	60L
#define	MINS_PER_HOUR	60L
#define	HOURS_PER_DAY	24L

/* DNS clock ticks every 100 nanoseconds */
#define NSEC_PER_TICK	100L
#define TICKS_PER_SEC	(10L*1000*1000)
#define TICKS_PER_MSEC	(10L*1000)
#define	TICKS_PER_USEC	(10L)

static int ticks_per_sec = TICKS_PER_SEC;
#if (LONG_BIT == 64)
# define UNIX_BASE_QUAD 0x7C95674BEB4000L
static time_quad_t unix_base_quad = UNIX_BASE_QUAD;
#else
static time_quad_t unix_base_quad = {1273708544, 8164711};
#endif

#define TV_NSEC_BITS sizeof(((timespec_t *)0)->tv_nsec)*8

#define TL_INTERVAL (1UL << (TV_NSEC_BITS - 1))

/*
 * sys_timezone_to_utc
 *	Get local timezone in UTC format.
 *	The value is obtained from the logical DNS$TIMEZONE
 *
 * outputs:
 *	utc_p = optional address of structure to receive the timezone
 *
 * This routine modifies module globals dns_timezone and previous_tdf.
 * previous_tdf is assumed to be initialized to some value that will
 * trip it the first time through.
 */
void 
sys_timezone_to_utc (time_quad_t *utc_p)
{
    utc_t utc;			/* Binary UTC from DECdts */
    struct tm utc_component;	/* UTC in tm format - not used */
    long tns;			/* Time in nanoseconds - not used */
    struct tm tm_inacc;		/* Inacc. in tm format - not used */
    long ins;			/* Inacc. in nanoseconds - not used */
    long tdf;			/* TDF (in seconds) */
    int int_tdf;
    int status;


    status = utc_gettime(&utc);
    dce_assert(cds__svc_handle, status >= 0);

    status = utc_anytime(&utc_component, &tns, &tm_inacc, &ins, &tdf, &utc);
    dce_assert(cds__svc_handle, status >= 0);

    int_tdf = tdf;
    dce_assert(cds__svc_handle, int_tdf == tdf);
    dns_emul(&int_tdf, &ticks_per_sec, utc_p);
}

/*
 * sys_time_from_interval
 *	This routine produces an internal time interval
 *
 * Inputs:
 *	local_p = pointer to block to receive timer interval
 *	hours = hours of time interval
 *	minutes = minutes of time interval
 *	seconds = seconds of time interval
 */
void 
sys_time_from_interval (time_local_t  *local_p,
                        int           hours,
                        int           minutes,
                        int           seconds)
{
    local_p->tv_sec = (((hours * MINS_PER_HOUR) + minutes) * SECS_PER_MIN)
		      + seconds;
    local_p->tv_nsec = TL_INTERVAL;
}

/*
 * sys_time_from_utc
 *	Convert utc time value to local system time
 *
 * Inputs:
 *	local_p = pointer to block to receive local time
 *	utc_p = optional pointer to utc value.
 *		if zero, current time is used.
 */
void 
sys_time_from_utc (time_local_t  *local_p,
                   time_quad_t   *utc_p)
{
    if (utc_p) {
	if (!ZERO_time_quad(utc_p)) {
	    time_quad_t	temp;

	    dns_qsub(utc_p, &unix_base_quad, &temp);
	    DIV_time_quad(&ticks_per_sec, &temp,
			&local_p->tv_sec, &local_p->tv_nsec);

	    local_p->tv_nsec *= NSEC_PER_TICK;
	} else {
	    local_p->tv_sec = 0;
	    local_p->tv_nsec = 0;
	}
    } else {
	struct utc	utc;		/* Binary UTC from DECdts */
	struct timespec	inacc;		/* throw-away inaccuracy */
	long		tdf;		/* throw-away timezone */
	int status;

	status = utc_gettime(&utc);
	dce_assert(cds__svc_handle, status >= 0);

	status = utc_bintime(local_p, &inacc, &tdf, &utc);
	dce_assert(cds__svc_handle, status >= 0);
    }
}

/*
 * sys_time_to_utc
 *	Convert local system time to utc time
 *
 * Inputs:
 *	local_p = optional pointer to local time
 *		if zero, current time is used.
 *	utc_p = pointer to receive utc value.
 */
void 
sys_time_to_utc (time_local_t  *local_p,
                 time_quad_t   *utc_p)
{
    if (local_p) {
	time_quad_t temp;

# if (LONG_BIT == 64)
	if (temp = (local_p->tv_nsec & ~TL_INTERVAL)) {
	    temp /= NSEC_PER_TICK;
	}
	temp += local_p->tv_sec * TICKS_PER_SEC;
	if (!(local_p->tv_nsec & TL_INTERVAL))
	    temp += UNIX_BASE_QUAD;
	*utc_p = temp;
# else
	int secs = local_p->tv_sec;

	dns_emul(&ticks_per_sec, &secs, utc_p);
	if (temp.tq_lo = (local_p->tv_nsec & ~TL_INTERVAL)) {
	    temp.tq_lo /= NSEC_PER_TICK;
	    temp.tq_hi = 0;
	    dns_qadd(&temp, utc_p, utc_p);
	}
	if (!(local_p->tv_nsec & TL_INTERVAL))
	    dns_qadd(utc_p, &unix_base_quad, utc_p);
# endif
    } else {
	time_quad_t temp;
	utc_t now_utc;
	timespec_t time, inacc;
	long tdf;
	int secs;

	if (utc_gettime(&now_utc) < 0) abort();
	if (utc_bintime(&time, &inacc, &tdf, &now_utc) < 0) abort();

# if (LONG_BIT == 64)
	*utc_p = UNIX_BASE_QUAD
		 + time.tv_sec * TICKS_PER_SEC
		 + time.tv_nsec / NSEC_PER_TICK;
# else
	secs = time.tv_sec;
	dns_emul(&ticks_per_sec, &secs, utc_p);
	temp.tq_lo = time.tv_nsec / NSEC_PER_TICK;
	temp.tq_hi = 0;
	dns_qadd(&temp, utc_p, utc_p);
	dns_qadd(utc_p, &unix_base_quad, utc_p);
# endif
    }
}

/*
 * sys_time_to_absolute
 *	Make sure given time is absolute, not interval
 *
 * Inputs:
 *	reltime_p = pointer to interval
 *	abstime_p = receives absolute time
 */
void 
sys_time_to_absolute (time_local_t  *reltime_p,
                      time_local_t  *abstime_p)
{
    if (reltime_p->tv_nsec & TL_INTERVAL) {
	time_local_t now;

	sys_time_from_utc(&now, NULL);
	sys_time_add(abstime_p, &now, reltime_p);
    } else
	*abstime_p = *reltime_p;
}

/*
 * sys_time_add
 *	Add two local time values together.
 *
 * Inputs:
 *	result_p = receives sum
 *	abstime_p = pointer to absolute time or interval
 *	reltime_p = pointer to interval
 */
void 
sys_time_add (time_local_t  *result_p,
              time_local_t  *abstime_p,
              time_local_t  *reltime_p)
{
    /* do a hard add */
    result_p->tv_sec = abstime_p->tv_sec + reltime_p->tv_sec;
    result_p->tv_nsec = (abstime_p->tv_nsec & ~TL_INTERVAL)
			+ (reltime_p->tv_nsec & ~TL_INTERVAL);

    /* adjust seconds if nanoseconds overflows */
    if (NSEC_PER_SEC <= result_p->tv_nsec) {
	result_p->tv_nsec -= NSEC_PER_SEC;
	result_p->tv_sec++;
    }

    /* if both are intervals, the result is an interval */
    if ((abstime_p->tv_nsec & TL_INTERVAL) &&
		(reltime_p->tv_nsec & TL_INTERVAL))
	result_p->tv_nsec |= TL_INTERVAL;
}

/*
 * sys_time_to_timespec
 *	Convert local system time to timespec
 *
 * Inputs:
 *	local_p = optional pointer to local time
 *		if zero, current time is used.
 *	utc_p = pointer to receive timespec value.
 */
void 
sys_time_to_timespec (time_local_t     *local_p,
                      struct timespec  *time_p)
{
    struct utc		utc;		/* Binary UTC from DECdts */
    struct timespec	inacc;		/* throw-away inaccuracy */
    long tdf;		/* throw-away timezone */
    int status;

    if (local_p && !(local_p->tv_nsec & TL_INTERVAL)) {
	*time_p = *local_p;
	return;
    }

    status = utc_gettime(&utc);
    dce_assert(cds__svc_handle, status >= 0);

    status = utc_bintime(time_p, &inacc, &tdf, &utc);
    dce_assert(cds__svc_handle, status >= 0);

    if (local_p && (local_p->tv_nsec & TL_INTERVAL)) {
	time_p->tv_sec += local_p->tv_sec;
	time_p->tv_nsec += local_p->tv_nsec & ~TL_INTERVAL;
	if (NSEC_PER_SEC <= time_p->tv_nsec) {
	    time_p->tv_nsec -= NSEC_PER_SEC;
	    time_p->tv_sec++;
	}
    }
}

/*
 * sys_time_to_string
 *	Convert local time value to external string
 *
 * Inputs:
 *	local_p = pointer to local time
 *	buf_p = pointer to buffer to receive null-terminated string
 *	buflen = length of buffer
 */
void 
sys_time_to_string (time_local_t  *local_p,
                    char          *buf_p,
                    int           buflen)
{
    if (local_p->tv_nsec & TL_INTERVAL) {
	long days, hours, minutes;
	double secs;
	char s[20];

	minutes = local_p->tv_sec/SECS_PER_MIN;
	hours = minutes/MINS_PER_HOUR;
	days = hours/HOURS_PER_DAY;
	secs = ((local_p->tv_nsec & ~TL_INTERVAL) / (1.0*NSEC_PER_SEC))
		+ (local_p->tv_sec % SECS_PER_MIN);
	/* GET AROUND DOUBLEWORD ALIGNMENT BUG ON MIPS */
	sprintf(buf_p, "%d", days);
	buf_p = strchr(buf_p, '\0');
	dtos(secs, s);
	sprintf(buf_p, " %02d:%02d:%s",
		hours%HOURS_PER_DAY, minutes%MINS_PER_HOUR, s);
    } else if ((local_p->tv_sec != 0) || (local_p->tv_nsec != 0)) {
	static struct timespec no_inacc = {0};
	static char *themonth[12] = {
            "jan", "feb", "mar", "apr", "may", "jun",
            "jul", "aug", "sep", "oct", "nov", "dec"
	};
	struct utc utc;
	struct tm time;
	long tns, ins;
	struct tm tm_inacc;
	double secs;
	char s[20];

	utc_mkbintime(&utc, local_p, &no_inacc, 0);
	utc_localtime(&time, &tns, &tm_inacc, &ins, &utc);
	secs = (local_p->tv_nsec / (1.0*NSEC_PER_SEC)) + time.tm_sec;
	/* GET AROUND DOUBLEWORD ALIGNMENT BUG ON MIPS */
	sprintf(buf_p, "%02d-%s-%04d",
	    time.tm_mday, themonth[time.tm_mon], time.tm_year + 1900);
	buf_p = strchr(buf_p, '\0');
	dtos(secs, s);
	sprintf(buf_p, " %02d:%02d:%s", time.tm_hour, time.tm_min, s);
    } else {
	*buf_p++ = '0';
	*buf_p = 0;
    }
}

/*
 * sys_utc_to_string
 *	Convert utc value to external string
 *
 * Inputs:
 *	utc_p = pointer to time quad
 *	buf_p = pointer to buffer to receive null-terminated string
 *	buflen = length of buffer
 */
void 
sys_utc_to_string (time_quad_t  *utc_p,
                   char         *buf_p,
                   int          buflen)
{
    static char *themonth[12] = {
	"jan", "feb", "mar", "apr", "may", "jun",
	"jul", "aug", "sep", "oct", "nov", "dec"
	};


    if (!ZERO_time_quad(utc_p)) 
    {
	struct tm *tm_p;
	time_quad_t temp;
	double secs;
	int seconds, ticks;
	char s[20];

	dns_qsub(utc_p, &unix_base_quad, &temp);
	dns_ediv(&ticks_per_sec, &temp, &seconds, &ticks);
	tm_p = gmtime((time_t *)&seconds);
	secs = (ticks / (1.0*TICKS_PER_SEC)) + tm_p->tm_sec;
	/* GET AROUND DOUBLEWORD ALIGNMENT BUG ON MIPS */
	(void)sprintf(buf_p, "%02d-%s-%04d",
	    tm_p->tm_mday, themonth[tm_p->tm_mon], tm_p->tm_year + 1900);
	buf_p = strchr(buf_p, '\0');
	dtos(secs, s);
	(void)sprintf(buf_p, " %02d:%02d:%s", tm_p->tm_hour, tm_p->tm_min, s);
    } 
    else 
    {
	*buf_p++ = '0';
	*buf_p = 0;
    }
}

/*
 * dtos
 *	Convert a double to a char string - equivalent to
 *
 *		sprintf(s, "%010.7f", d);
 *
 * 	(sprintf on mips barfs on doubles)
 *
 * Inputs:
 *	d = double precision float
 *
 * Outputs:
 *	s = null terminated character string
 *
 * Returns:
 *	nothing
 */
void 
dtos (double  d,
      char    *s)
{
	long frac, floor;

	floor = d;			/* C knows floor() */
	frac = (d - floor) * 1e7;	/* want 7 digits in frac. part */
	(void)sprintf(s, "%02d.%07d", floor, frac);
}
