/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tzset.c,v $
 * Revision 1.1.13.2  1996/02/17  23:34:36  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:43  marty]
 *
 * Revision 1.1.13.1  1995/12/08  18:08:14  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1994/09/26  13:23 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:24:03  root]
 * 
 * Revision 1.1.11.5  1994/08/23  20:32:01  cbrooks
 * 	Code Cleanup
 * 	[1994/08/19  17:52:34  cbrooks]
 * 	HP revision /main/HPDCE02/2  1994/08/18  14:40 UTC  pare
 * 	Merge changes from Hpdce02
 * 
 * Revision 1.1.11.4  1994/08/16  18:12:50  cbrooks
 * 	CR 11494
 * 	[1994/08/09  18:59:59  cbrooks]
 * 
 * Revision 1.1.11.3  1994/07/29  18:51:05  cbrooks
 * 	CR9380 - RIOS strdup() doesn't like const char *
 * 	[1994/07/29  18:49:14  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  20:24 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.8.5  1993/11/03  18:30:15  pare
 * 	The function settzname() is not called so don't instrument it --
 * 	wrap it with #pragma covcc !instr/instr.
 * 	[1993/11/03  18:29:29  pare]
 * 	Revision 1.1.11.2  1994/05/25  20:47:54  cbrooks
 * 	CR 10767 - dcecp printing in GMT
 * 	[1994/05/25  20:47:38  cbrooks]
 * 
 * Revision 1.1.8.4  1993/09/23  17:24:39  pare
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/23  17:12:10  pare]
 * 
 * Revision 1.1.9.3  1993/09/15  17:46:45  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.8.3  1993/07/15  13:11:35  truitt
 * 	Added missing default case to switch statement.  This
 * 	default case will simply break, with no bug status.
 * 	[1993/07/15  13:11:16  truitt]
 * 
 * Revision 1.1.8.2  1993/07/08  19:41:33  root
 * 	Initial King Kong branch
 * 	[1993/07/08  19:41:04  root]
 * 
 * Revision 1.1.6.3  1993/04/07  15:25:23  truitt
 * 	Ensure a large size for timezone name string.
 * 	[1993/04/07  15:24:56  truitt]
 * 
 * Revision 1.1.6.2  1993/04/05  17:42:00  truitt
 * 	Recreate GAMERA branch.
 * 	[1993/04/05  17:41:08  truitt]
 * 
 * Revision 1.1.6.2  1993/03/19  14:34:52  truitt
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/19  14:34:18  truitt]
 * 
 * Revision 1.1.1.8  1993/03/11  22:31:49  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.3.3  1992/11/10  15:33:36  smythe
 * 	made getenv(DTS_TZ) conditional on __hpux
 * 	for later submission back to OSF source
 * 	[1992/11/10  15:32:37  smythe]
 * 
 * Revision 1.1.3.2  1992/09/10  17:46:20  hanfei
 * 	change dts TZ environment variable to DTS_TZ so it won't
 * 	conflict with HP-UX TZ.
 * 	[1992/09/10  17:41:24  hanfei]
 * 
 * Revision 1.1.6.4  1993/01/28  15:26:26  tom
 * 	Do not touch the global variable tzname[], since the OS ctime
 * 	routines should be taking care of it.  This way the OS routines
 * 	don't have pointers to our data to mess it up.
 * 	[1993/01/28  15:24:36  tom]
 * 
 * Revision 1.1.11.1  1994/05/12  15:17:33  rhw
 * 	continued code cleanup & merging
 * 	[1994/05/09  18:46:31  rhw]
 * 
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:47  rhw]
 * 
 * Revision 1.1.6.4  1993/01/28  15:26:26  tom
 * 	Do not touch the global variable tzname[], since the OS ctime
 * 	routines should be taking care of it.  This way the OS routines
 * 	don't have pointers to our data to mess it up.
 * 	[1993/01/28  15:24:36  tom]
 * 
 * Revision 1.1.6.3  1993/01/21  19:37:50  tom
 * 	Bug 5917 -  Rename tzset to dce_tzset and make static.
 * 	Add a check to make sure we don't repeat work if dce_tzset called
 * 	with the same TZ variable.
 * 	[1993/01/21  19:37:03  tom]
 * 
 * Revision 1.1.6.2  1992/12/30  16:34:30  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:10:54  htf]
 * 
 * Revision 1.1.2.3  1992/05/14  18:27:55  ko
 * 	fix ot 2116 - uninitialized variable warnings.
 * 	[1992/05/14  16:53:09  ko]
 * 
 * Revision 1.1.2.2  1992/04/14  21:39:03  melman
 * 	Removed the following from the definition of tzset():
 * 	    #ifndef vms
 * 	    static
 * 	    #endif
 * 	It should never be static.
 * 	[1992/04/14  21:38:38  melman]
 * 
 * Revision 1.1  1992/01/19  15:32:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** POSIX-style TZ environment variable handling from Guy Harris
** (guy@auspex.com).
*/

/*LINTLIBRARY*/

/* Decide if we should use a private localtime (tzset, etc.) or should
   we just use the OS supplied one. If this is VMS or if this is prior to
   V4.0 of Ultrix (where the symbol _TIME_H_ appears) use the private
   localtime. Otherwise just use the Ultrix one, which is based on this
   same public domain code. If we're going to use our own, define
   USE_OWN_LOCALTIME, otherwise leave it undefined. */

/* Basically, if your OS supports zoneinfo, you don't need this code */


#define USE_OWN_LOCALTIME

#include <tzfile.h>
#include <limits.h>
#include <string.h>

/*
 * N.B. This could be file system specific 
 */

#ifndef PATH_MAX 
#define PATH_MAX 1024
#endif

#include <fcntl.h>

#ifdef USE_OWN_LOCALTIME

#define ACCESS_MODE	O_RDONLY

#ifdef O_BINARY
#define OPEN_MODE	(O_RDONLY | O_BINARY)
#else /* !defined O_BINARY */
#define OPEN_MODE	O_RDONLY
#endif /* !defined O_BINARY */

struct ttinfo {				/* time type information */
	long		tt_gmtoff;	/* GMT offset in seconds */
	int		tt_isdst;	/* used to set tm_isdst */
	int		tt_abbrind;	/* abbreviation list index */
	int		tt_ttisstd;	/* TRUE if transition is std time */
};

struct state {
	int		timecnt;
	int		typecnt;
	int		charcnt;
	time_t		ats[TZ_MAX_TIMES];
	unsigned char	types[TZ_MAX_TIMES];
	struct ttinfo	ttis[TZ_MAX_TYPES];
	char		chars[(TZ_MAX_CHARS + 1 > sizeof GMT) ?
				TZ_MAX_CHARS + 1 : sizeof GMT];
};

struct rule {
	int		r_type;		/* type of rule--see below */
	int		r_day;		/* day number of rule */
	int		r_week;		/* week number of rule */
	int		r_mon;		/* month number of rule */
	long		r_time;		/* transition time of rule */
};

#define	JULIAN_DAY		0	/* Jn - Julian day */
#define	DAY_OF_YEAR		1	/* n - day of year */
#define	MONTH_NTH_DAY_OF_WEEK	2	/* Mm.n.d - month, week, day of week */

/*
** Prototypes for static functions.
*/

static long		detzcode _DCE_PROTOTYPE_((const char * const codep));
static const char *	getzname _DCE_PROTOTYPE_((const char * strp));
static const char *	getnum _DCE_PROTOTYPE_((const char * strp, 
				int * const nump, const int minnum, 
				const int maxnum));
static const char *	getsecs _DCE_PROTOTYPE_((const char * strp, 
				long * const secsp));
static const char *	getoffset _DCE_PROTOTYPE_((const char * strp, 
				long * const offsetp));
static const char *	getrule _DCE_PROTOTYPE_((const char * strp, struct rule * const rulep));
static void		gmtload _DCE_PROTOTYPE_((void));
static time_t		transtime _DCE_PROTOTYPE_((const time_t janfirst, 
				const int year, const struct rule * const rulep,
				const long offset));
static int		tzload _DCE_PROTOTYPE_((const char * name));
static int		tzparse _DCE_PROTOTYPE_((const char * name, const int lastditch));

static struct state	lclstate;
static int		lcl_is_set;

/* char *tzname[2];		we should never touch tzname[] */

#else

#include <tzs.h>
extern struct state	_tzs;
#define lclstate	_tzs
extern int		_tz_is_set;
#define lcl_is_set	_tz_is_set

/*
 * Use the OS tzset since it supports the zoneinfo structures.  
 * Since the app should call tzset if it changes TZ,
 * we can check lcl_is_set.
 */
#define dce_tzset() { 		\
	if (!lcl_is_set)	\
		tzset();	\
	}

#endif

#ifdef USE_OWN_LOCALTIME
#ifdef USG_COMPAT
time_t			timezone = 0;
int			daylight = 0;
#endif /* defined USG_COMPAT */

#ifdef ALTZONE
time_t			altzone = 0;
#endif /* defined ALTZONE */

static const int	mon_lengths[2][MONS_PER_YEAR] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
	31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static const int	year_lengths[2] = {
	DAYS_PER_NYEAR, DAYS_PER_LYEAR
};

static long
detzcode(codep)
const char * const	codep;
{
	register long	result;
	register int	i;

	result = 0;
	for (i = 0; i < 4; ++i)
		result = (result << 8) | (codep[i] & 0xff);
	return result;
}

static int
tzload(name)
const char *		name;
{
	register const char *	p;
	register int		i;
	register int		fid;

	if (name == NULL && (name = TZDEFAULT) == NULL)
		return -1;
	{
		register int 	doaccess;
		char		fullname[PATH_MAX + 1];

		if (name[0] == ':')
			++name;
		doaccess = name[0] == '/';
		if (!doaccess) {
			if ((p = TZDIR) == NULL)
				return -1;
			if ((strlen(p) + strlen(name) + 1) >= sizeof fullname)
				return -1;
			(void) strcpy(fullname, p);
   			(void) strcat(fullname, "/");
			(void) strcat(fullname, name);
			/*
			** Set doaccess if '.' (as in "../") shows up in name.
			*/
			if (strchr(name, '.') != NULL)
				doaccess = TRUE;
			name = fullname;
		}
		if (doaccess && access(name, ACCESS_MODE) != 0)
			return -1;
		if ((fid = open((char *)name, OPEN_MODE)) == -1)
			return -1;
	}
	{
		register const struct tzhead *	tzhp;
		char				buf[sizeof lclstate + sizeof *tzhp];
		int				ttisstdcnt;

		i = read(fid, buf, sizeof buf);
		if (close(fid) != 0 || i < sizeof *tzhp)
			return -1;
		tzhp = (struct tzhead *) buf;
		ttisstdcnt = (int) detzcode(tzhp->tzh_ttisstdcnt);
		lclstate.timecnt = (int) detzcode(tzhp->tzh_timecnt);
		lclstate.typecnt = (int) detzcode(tzhp->tzh_typecnt);
		lclstate.charcnt = (int) detzcode(tzhp->tzh_charcnt);
		if (lclstate.typecnt <= 0 || lclstate.typecnt > TZ_MAX_TYPES ||
			lclstate.timecnt < 0 || lclstate.timecnt > TZ_MAX_TIMES ||
			lclstate.charcnt < 0 || lclstate.charcnt > TZ_MAX_CHARS ||
			(ttisstdcnt != lclstate.typecnt && ttisstdcnt != 0))
				return -1;
		if (i < sizeof *tzhp +
			lclstate.timecnt * (4 + sizeof (char)) +
			lclstate.typecnt * (4 + 2 * sizeof (char)) +
			lclstate.charcnt * sizeof (char) +
			ttisstdcnt * sizeof (char))
				return -1;
		p = buf + sizeof *tzhp;
		for (i = 0; i < lclstate.timecnt; ++i) {
			lclstate.ats[i] = detzcode(p);
			p += 4;
		}
		for (i = 0; i < lclstate.timecnt; ++i) {
			lclstate.types[i] = (unsigned char) *p++;
			if (lclstate.types[i] >= lclstate.typecnt)
				return -1;
		}
		for (i = 0; i < lclstate.typecnt; ++i) {
			register struct ttinfo *	ttisp;

			ttisp = &lclstate.ttis[i];
			ttisp->tt_gmtoff = detzcode(p);
			p += 4;
			ttisp->tt_isdst = (unsigned char) *p++;
			if (ttisp->tt_isdst != 0 && ttisp->tt_isdst != 1)
				return -1;
			ttisp->tt_abbrind = (unsigned char) *p++;
			if (ttisp->tt_abbrind < 0 ||
				ttisp->tt_abbrind > lclstate.charcnt)
					return -1;
		}
		for (i = 0; i < lclstate.charcnt; ++i)
			lclstate.chars[i] = *p++;
		lclstate.chars[i] = '\0';	/* ensure '\0' at end */
		for (i = 0; i < lclstate.typecnt; ++i) {
			register struct ttinfo *	ttisp;

			ttisp = &lclstate.ttis[i];
			if (ttisstdcnt == 0)
				ttisp->tt_ttisstd = FALSE;
			else {
				ttisp->tt_ttisstd = *p++;
				if (ttisp->tt_ttisstd != TRUE &&
					ttisp->tt_ttisstd != FALSE)
						return -1;
			}
		}
	}
	return 0;
}

/*
** Given a pointer into a time zone string, scan until a character that is not
** a valid character in a zone name is found.  Return a pointer to that
** character.
*/

static const char *
getzname(strp)
register const char *	strp;
{
	register char	c;

	while ((c = *strp) != '\0' && !isdigit(c) && c != ',' && c != '-' &&
		c != '+')
			++strp;
	return strp;
}

/*
** Given a pointer into a time zone string, extract a number from that string.
** Check that the number is within a specified range; if it is not, return
** NULL.
** Otherwise, return a pointer to the first character not part of the number.
*/

static const char *
getnum(strp, nump, minnum, maxnum)
register const char *	strp;
int * const		nump;
const int		minnum;
const int		maxnum;
{
	register char	c;
	register int	num;

	if (strp == NULL || !isdigit(*strp))
		return NULL;
	num = 0;
	while ((c = *strp) != '\0' && isdigit(c)) {
		num = num * 10 + (c - '0');
		if (num > maxnum)
			return NULL;	/* illegal value */
		++strp;
	}
	if (num < minnum)
		return NULL;		/* illegal value */
	*nump = num;
	return strp;
}

/*
** Given a pointer into a time zone string, extract a number of seconds,
** in hh[:mm[:ss]] form, from the string.
** If any error occurs, return NULL.
** Otherwise, return a pointer to the first character not part of the number
** of seconds.
*/

static const char *
getsecs(strp, secsp)
register const char *	strp;
long * const		secsp;
{
	int	num;

	strp = getnum(strp, &num, 0, HOURS_PER_DAY);
	if (strp == NULL)
		return NULL;
	*secsp = num * SECS_PER_HOUR;
	if (*strp == ':') {
		++strp;
		strp = getnum(strp, &num, 0, MINS_PER_HOUR - 1);
		if (strp == NULL)
			return NULL;
		*secsp += num * SECS_PER_MIN;
		if (*strp == ':') {
			++strp;
			strp = getnum(strp, &num, 0, SECS_PER_MIN - 1);
			if (strp == NULL)
				return NULL;
			*secsp += num;
		}
	}
	return strp;
}

/*
** Given a pointer into a time zone string, extract an offset, in
** [+-]hh[:mm[:ss]] form, from the string.
** If any error occurs, return NULL.
** Otherwise, return a pointer to the first character not part of the time.
*/

static const char *
getoffset(strp, offsetp)
register const char *	strp;
long * const		offsetp;
{
	register int	neg;

	if (*strp == '-') {
		neg = 1;
		++strp;
	} else if (isdigit(*strp) || *strp++ == '+')
		neg = 0;
	else	return NULL;		/* illegal offset */
	strp = getsecs(strp, offsetp);
	if (strp == NULL)
		return NULL;		/* illegal time */
	if (neg)
		*offsetp = -*offsetp;
	return strp;
}

/*
** Given a pointer into a time zone string, extract a rule in the form
** date[/time].  See POSIX section 8 for the format of "date" and "time".
** If a valid rule is not found, return NULL.
** Otherwise, return a pointer to the first character not part of the rule.
*/

static const char *
getrule(strp, rulep)
const char *			strp;
register struct rule * const	rulep;
{
	if (*strp == 'J') {
		/*
		** Julian day.
		*/
		rulep->r_type = JULIAN_DAY;
		++strp;
		strp = getnum(strp, &rulep->r_day, 1, DAYS_PER_NYEAR);
	} else if (*strp == 'M') {
		/*
		** Month, week, day.
		*/
		rulep->r_type = MONTH_NTH_DAY_OF_WEEK;
		++strp;
		strp = getnum(strp, &rulep->r_mon, 1, MONS_PER_YEAR);
		if (strp == NULL)
			return NULL;
		if (*strp++ != '.')
			return NULL;
		strp = getnum(strp, &rulep->r_week, 1, 5);
		if (strp == NULL)
			return NULL;
		if (*strp++ != '.')
			return NULL;
		strp = getnum(strp, &rulep->r_day, 0, DAYS_PER_WEEK - 1);
	} else if (isdigit(*strp)) {
		/*
		** Day of year.
		*/
		rulep->r_type = DAY_OF_YEAR;
		strp = getnum(strp, &rulep->r_day, 0, DAYS_PER_LYEAR - 1);
	} else	return NULL;		/* invalid format */
	if (strp == NULL)
		return NULL;
	if (*strp == '/') {
		/*
		** Time specified.
		*/
		++strp;
		strp = getsecs(strp, &rulep->r_time);
	} else	rulep->r_time = 2 * SECS_PER_HOUR;	/* default = 2:00:00 */
	return strp;
}

/*
** Given the Epoch-relative time of January 1, 00:00:00 GMT, in a year, the
** year, a rule, and the offset from GMT at the time that rule takes effect,
** calculate the Epoch-relative time that rule takes effect.
*/

static time_t
transtime(janfirst, year, rulep, offset)
const time_t				janfirst;
const int				year;
register const struct rule * const	rulep;
const long				offset;
{
	register int	leapyear;
	register time_t	value = 0;
	register int	i;
	int		d, m1, yy0, yy1, yy2, dow;

	leapyear = isleap(year);

	switch (rulep->r_type) {

	case JULIAN_DAY:
		/*
		** Jn - Julian day, 1 == January 1, 60 == March 1 even in leap
		** years.
		** In non-leap years, or if the day number is 59 or less, just
		** add SECS_PER_DAY times the day number-1 to the time of
		** January 1, midnight, to get the day.
		*/
		value = janfirst + (rulep->r_day - 1) * SECS_PER_DAY;
		if (leapyear && rulep->r_day >= 60)
			value += SECS_PER_DAY;
		break;

	case DAY_OF_YEAR:
		/*
		** n - day of year.
		** Just add SECS_PER_DAY times the day number to the time of
		** January 1, midnight, to get the day.
		*/
		value = janfirst + rulep->r_day * SECS_PER_DAY;
		break;

	case MONTH_NTH_DAY_OF_WEEK:
		/*
		** Mm.n.d - nth "dth day" of month m.
		*/
		value = janfirst;
		for (i = 0; i < rulep->r_mon - 1; ++i)
			value += mon_lengths[leapyear][i] * SECS_PER_DAY;

		/*
		** Use Zeller's Congruence to get day-of-week of first day of
		** month.
		*/
		m1 = (rulep->r_mon + 9) % 12 + 1;
		yy0 = (rulep->r_mon <= 2) ? (year - 1) : year;
		yy1 = yy0 / 100;
		yy2 = yy0 % 100;
		dow = ((26 * m1 - 2) / 10 +
			1 + yy2 + yy2 / 4 + yy1 / 4 - 2 * yy1) % 7;
		if (dow < 0)
			dow += DAYS_PER_WEEK;

		/*
		** "dow" is the day-of-week of the first day of the month.  Get
		** the day-of-month (zero-origin) of the first "dow" day of the
		** month.
		*/
		d = rulep->r_day - dow;
		if (d < 0)
			d += DAYS_PER_WEEK;
		for (i = 1; i < rulep->r_week; ++i) {
			if (d + DAYS_PER_WEEK >=
				mon_lengths[leapyear][rulep->r_mon - 1])
					break;
			d += DAYS_PER_WEEK;
		}

		/*
		** "d" is the day-of-month (zero-origin) of the day we want.
		*/
		value += d * SECS_PER_DAY;
		break;

	default:
		break;
	}

	/*
	** "value" is the Epoch-relative time of 00:00:00 GMT on the day in
	** question.  To get the Epoch-relative time of the specified local
	** time on that day, add the transition time and the current offset
	** from GMT.
	*/
	return value + rulep->r_time + offset;
}

/*
** Given a POSIX section 8-style TZ string, fill in the rule tables as
** appropriate.
*/

static int
tzparse(name, lastditch)
const char *			name;
const int			lastditch;
{
	const char *			stdname;
	const char *			dstname;
	size_t				stdlen;
	size_t				dstlen;
	long				stdoffset;
	long				dstoffset;
	register time_t *		atp;
	register unsigned char *	typep;
	register char *			cp;
	register int			load_result;

        dstlen  = 0;
        dstname = NULL;
	stdname = name;
	if (lastditch) {
		stdlen = strlen(name);	/* length of standard zone name */
		name += stdlen;
		if (stdlen >= sizeof lclstate.chars)
			stdlen = (sizeof lclstate.chars) - 1;
	} else {
		name = getzname(name);
		stdlen = name - stdname;
		if (stdlen < 3)
			return -1;
	}
	if (*name == '\0')
		stdoffset = 0;
	else {
		name = getoffset(name, &stdoffset);
		if (name == NULL)
			return -1;
	}
	load_result = tzload(TZDEFRULES);
	if (*name != '\0') {
		dstname = name;
		name = getzname(name);
		dstlen = name - dstname;	/* length of DST zone name */
		if (dstlen < 3)
			return -1;
		if (*name != '\0' && *name != ',' && *name != ';') {
			name = getoffset(name, &dstoffset);
			if (name == NULL)
				return -1;
		} else	dstoffset = stdoffset - SECS_PER_HOUR;
		if (*name == ',' || *name == ';') {
			struct rule	start;
			struct rule	end;
			register int	year;
			register time_t	janfirst;
			time_t		starttime;
			time_t		endtime;

			++name;
			if ((name = getrule(name, &start)) == NULL)
				return -1;
			if (*name++ != ',')
				return -1;
			if ((name = getrule(name, &end)) == NULL)
				return -1;
			if (*name != '\0')
				return -1;
			lclstate.typecnt = 2;	/* standard time and DST */
			/*
			** Two transitions per year, from EPOCH_YEAR to 2037.
			*/
			lclstate.timecnt = 2 * (2037 - EPOCH_YEAR + 1);
			if (lclstate.timecnt > TZ_MAX_TIMES)
				return -1;
			lclstate.ttis[0].tt_gmtoff = -dstoffset;
			lclstate.ttis[0].tt_isdst = 1;
			lclstate.ttis[0].tt_abbrind = stdlen + 1;
			lclstate.ttis[1].tt_gmtoff = -stdoffset;
			lclstate.ttis[1].tt_isdst = 0;
			lclstate.ttis[1].tt_abbrind = 0;
			atp = lclstate.ats;
			typep = lclstate.types;
			janfirst = 0;
			for (year = EPOCH_YEAR; year <= 2037; ++year) {
				starttime = transtime(janfirst, year, &start,
					stdoffset);
				endtime = transtime(janfirst, year, &end,
					dstoffset);
				if (starttime > endtime) {
					*atp++ = endtime;
					*typep++ = 1;	/* DST ends */
					*atp++ = starttime;
					*typep++ = 0;	/* DST begins */
				} else {
					*atp++ = starttime;
					*typep++ = 0;	/* DST begins */
					*atp++ = endtime;
					*typep++ = 1;	/* DST ends */
				}
				janfirst +=
					year_lengths[isleap(year)] * SECS_PER_DAY;
			}
		} else {
			int		sawstd;
			int		sawdst;
			long		stdfix;
			long		dstfix;
			long		oldfix;
			int		isdst;
			register int	i;

			if (*name != '\0')
				return -1;
			if (load_result != 0)
				return -1;
			/*
			** Compute the difference between the real and
			** prototype standard and summer time offsets
			** from GMT, and put the real standard and summer
			** time offsets into the rules in place of the
			** prototype offsets.
			*/
			sawstd = FALSE;
			sawdst = FALSE;
			stdfix = 0;
			dstfix = 0;
			for (i = 0; i < lclstate.typecnt; ++i) {
				if (lclstate.ttis[i].tt_isdst) {
					oldfix = dstfix;
					dstfix =
					    lclstate.ttis[i].tt_gmtoff + dstoffset;
					if (sawdst && (oldfix != dstfix))
						return -1;
					lclstate.ttis[i].tt_gmtoff = -dstoffset;
					lclstate.ttis[i].tt_abbrind = stdlen + 1;
					sawdst = TRUE;
				} else {
					oldfix = stdfix;
					stdfix =
					    lclstate.ttis[i].tt_gmtoff + stdoffset;
					if (sawstd && (oldfix != stdfix))
						return -1;
					lclstate.ttis[i].tt_gmtoff = -stdoffset;
					lclstate.ttis[i].tt_abbrind = 0;
					sawstd = TRUE;
				}
			}
			/*
			** Make sure we have both standard and summer time.
			*/
			if (!sawdst || !sawstd)
				return -1;
			/*
			** Now correct the transition times by shifting
			** them by the difference between the real and
			** prototype offsets.  Note that this difference
			** can be different in standard and summer time;
			** the prototype probably has a 1-hour difference
			** between standard and summer time, but a different
			** difference can be specified in TZ.
			*/
			isdst = FALSE;	/* we start in standard time */
			for (i = 0; i < lclstate.timecnt; ++i) {
				register const struct ttinfo *	ttisp;

				/*
				** If summer time is in effect, and the
				** transition time was not specified as
				** standard time, add the summer time
				** offset to the transition time;
				** otherwise, add the standard time offset
				** to the transition time.
				*/
				ttisp = &lclstate.ttis[lclstate.types[i]];
				lclstate.ats[i] +=
					(isdst && !ttisp->tt_ttisstd) ?
						dstfix : stdfix;
				isdst = ttisp->tt_isdst;
			}
		}
	} else {
		dstlen = 0;
		lclstate.typecnt = 1;		/* only standard time */
		lclstate.timecnt = 0;
		lclstate.ttis[0].tt_gmtoff = -stdoffset;
		lclstate.ttis[0].tt_isdst = 0;
		lclstate.ttis[0].tt_abbrind = 0;
		lclstate.types[0] = 0;
	}
	lclstate.charcnt = stdlen + 1;
	if (dstlen != 0)
		lclstate.charcnt += dstlen + 1;
	if (lclstate.charcnt > sizeof lclstate.chars)
		return -1;
	cp = lclstate.chars;
	(void) strncpy(cp, stdname, stdlen);
	cp += stdlen;
	*cp++ = '\0';
	if (dstlen != 0) {
		(void) strncpy(cp, dstname, dstlen);
		*(cp + dstlen) = '\0';
	}
	return 0;
}

static void
gmtload()
{
#ifdef ultrix
	{
	    struct timeval t;
            struct timezone tz;

	    if (!gettimeofday(&t, &tz)) {
		lclstate.typecnt = 1;
		lclstate.timecnt = 0;
		lclstate.ttis[0].tt_gmtoff = -tz.tz_minuteswest*SECSPERMIN;
		lclstate.ttis[0].tt_isdst = 0;
		lclstate.ttis[0].tt_abbrind = 0;
		lclstate.types[0] = 0;
		lclstate.charcnt = sizeof GMT;
		(void) strcpy( lclstate.chars, GMT );
		return;
	    }
	}
#endif
	if (tzload(GMT) != 0)
		(void) tzparse(GMT, TRUE);
	
}

#ifndef STD_INSPIRED
static
#endif /* !defined STD_INSPIRED */
void tzsetwall(void)
{
	lcl_is_set = TRUE;
	if (tzload((char *) NULL) != 0)
		gmtload();
}

static
void dce_tzset(void)
{
	const char *	name;
	static char *	prevTZ=NULL;

#ifdef __hpux
	name = getenv("DTS_TZ");
#else /* #ifdef __hpux */
	name = getenv("TZ");
#endif /* #ifdef __hpux */
	if (name == NULL) {
		/*
		 * Check to make sure we haven't already done tzsetwall()
		 */
		if (prevTZ == NULL) {
			if (lcl_is_set)
				return;		/* same TZ, already done */
		} else {
			free(prevTZ);		/* new TZ */
			prevTZ = NULL;
		}
		tzsetwall();
		return;
	}
	/*
	 * Since this is called unconditionally from utc functions, 
	 * try and optimize by not repeating the same work.
	 */
	if (prevTZ && (strcmp(name, prevTZ) == 0))
		return;		/* we've already been here with this TZ */
	else if (prevTZ)
		free(prevTZ);
	prevTZ = strdup((char*)name);
	lcl_is_set = TRUE;
	if (*name == '\0') {
		/*
		** User wants it fast rather than right.
		*/
		lclstate.timecnt = 0;
		lclstate.ttis[0].tt_gmtoff = 0;
		lclstate.ttis[0].tt_abbrind = 0;
		(void) strcpy(lclstate.chars, GMT);
	} else if (tzload(name) != 0)
		if (name[0] == ':' || tzparse(name, FALSE) != 0)
			(void) tzparse(name, TRUE);
}
#endif /* define USE_OWN_LOCALTIME */

