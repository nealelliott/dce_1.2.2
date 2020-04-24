/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tzfile.h,v $
 * Revision 1.1.6.2  1996/02/18  23:11:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:36:51  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:08:09  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/09/26  13:13 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:24:02  root]
 * 
 * Revision 1.1.4.2  1994/08/23  20:32:00  cbrooks
 * 	Code Cleanup
 * 	[1994/08/19  17:52:33  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  20:14 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	HP revision /main/HPDCE01/1  1994/02/17  20:37  pare
 * 	create HPDCE01 branch
 * 
 * 	HP revision /main/pare_rodan/1  1994/02/17  20:28  pare
 * 	conditionalize zoneinfo directory for hpux1000p
 * 
 * Revision 1.1.4.1  1994/05/12  15:17:30  rhw
 * 	continued code cleanup & merging
 * 	[1994/05/09  18:46:30  rhw]
 * 
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:46  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  20:40:04  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:08:41  htf]
 * 
 * Revision 1.1  1992/01/19  15:32:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#ifndef TZFILE_H

#define TZFILE_H

/*
** This header is for use ONLY with the time conversion code.
** There is no guarantee that it will remain unchanged,
** or that it will remain at all.
** Do NOT copy it to any system include directory.
** Thank you!
*/

/*
** Information about time zone files.
*/

#if __hpux1000p
#define TZDIR	"/opt/dce/lib/zoneinfo" /* Time zone object file directory */
#else
#define TZDIR	"/etc/zoneinfo" /* Time zone object file directory */
#endif

#define TZDEFAULT	"localtime"

#define TZDEFRULES	"posixrules"

/*
** Each file begins with. . .
*/

struct tzhead {
	char	tzh_reserved[24];	/* reserved for future use */
	char	tzh_ttisstdcnt[4];	/* coded number of trans. time flags */
	char	tzh_leapcnt[4];		/* coded number of leap seconds */
	char	tzh_timecnt[4];		/* coded number of transition times */
	char	tzh_typecnt[4];		/* coded number of local time types */
	char	tzh_charcnt[4];		/* coded number of abbr. chars */
};

/*
** . . .followed by. . .
**
**	tzh_timecnt (char [4])s		coded transition times a la time(2)
**	tzh_timecnt (unsigned char)s	types of local time starting at above
**	tzh_typecnt repetitions of
**		one (char [4])		coded GMT offset in seconds
**		one (unsigned char)	used to set tm_isdst
**		one (unsigned char)	that's an abbreviation list index
**	tzh_charcnt (char)s		'\0'-terminated zone abbreviations
**	tzh_leapcnt repetitions of
**		one (char [4])		coded leap second transition times
**		one (char [4])		total correction after above
**	tzh_ttisstdcnt (char)s		indexed by type; if TRUE, transition
**					time is standard time, if FALSE,
**					transition time is wall clock time
**					if absent, transition times are
**					assumed to be wall clock time
*/

/*
** In the current implementation, "tzset()" refuses to deal with files that
** exceed any of the limits below.
*/

/*
** The TZ_MAX_TIMES value below is enough to handle a bit more than a
** year's worth of solar time (corrected daily to the nearest second) or
** 138 years of Pacific Presidential Election time
** (where there are three time zone transitions every fourth year).
*/
#define TZ_MAX_TIMES	370

#define NOSOLAR		/* No support for SOLAR time */

#ifndef NOSOLAR
#define TZ_MAX_TYPES	256	/* Limited by what (unsigned char)'s can hold */
#else /* !defined NOSOLAR */
#define TZ_MAX_TYPES	10	/* Maximum number of local time types */
#endif /* !defined NOSOLAR */

#define TZ_MAX_CHARS	100	/* Maximum number of abbreviation characters */

#define	TZ_MAX_LEAPS	50	/* Maximum number of leap second corrections */

#define SECSPERMIN	60
#define MINSPERHOUR	60
#define HOURSPERDAY	24
#define DAYSPERWEEK	7
#define DAYSPERNYEAR	365
#define DAYSPERLYEAR	366
#define SECSPERHOUR	(SECSPERMIN * MINSPERHOUR)
#define SECSPERDAY	((long) SECSPERHOUR * HOURSPERDAY)
#define MONSPERYEAR	12

#define TM_SUNDAY	0
#define TM_MONDAY	1
#define TM_TUESDAY	2
#define TM_WEDNESDAY	3
#define TM_THURSDAY	4
#define TM_FRIDAY	5
#define TM_SATURDAY	6

#define TM_JANUARY	0
#define TM_FEBRUARY	1
#define TM_MARCH	2
#define TM_APRIL	3
#define TM_MAY		4
#define TM_JUNE		5
#define TM_JULY		6
#define TM_AUGUST	7
#define TM_SEPTEMBER	8
#define TM_OCTOBER	9
#define TM_NOVEMBER	10
#define TM_DECEMBER	11

#define TM_YEAR_BASE	1900

#define EPOCH_YEAR	1970
#define EPOCH_WDAY	TM_THURSDAY

/*
** Accurate only for the past couple of centuries;
** that will probably do.
*/

#define isleap(y) (((y) % 4) == 0 && ((y) % 100) != 0 || ((y) % 400) == 0)

#ifndef USG

/*
** Use of the underscored variants may cause problems if you move your code to
** certain System-V-based systems; for maximum portability, use the
** underscore-free variants.  The underscored variants are provided for
** backward compatibility only; they may disappear from future versions of
** this file.
*/

#define SECS_PER_MIN	SECSPERMIN
#define MINS_PER_HOUR	MINSPERHOUR
#define HOURS_PER_DAY	HOURSPERDAY
#define DAYS_PER_WEEK	DAYSPERWEEK
#define DAYS_PER_NYEAR	DAYSPERNYEAR
#define DAYS_PER_LYEAR	DAYSPERLYEAR
#define SECS_PER_HOUR	SECSPERHOUR
#define SECS_PER_DAY	SECSPERDAY
#define MONS_PER_YEAR	MONSPERYEAR

#endif /* !defined USG */

#endif /* !defined TZFILE_H */
