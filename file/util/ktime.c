/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ktime.c,v $
 * Revision 1.1.327.1  1996/10/02  21:13:21  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:51:07  damon]
 *
 * Revision 1.1.322.3  1994/07/13  22:30:33  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  12:02:34  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:04:37  mbs]
 * 
 * Revision 1.1.322.2  1994/06/09  14:24:57  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:37:00  annie]
 * 
 * Revision 1.1.322.1  1994/02/04  20:35:40  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:20:22  devsrc]
 * 
 * Revision 1.1.320.1  1993/12/07  17:37:50  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  18:09:29  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/21  16:31:39  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:25:18  cjd]
 * 
 * Revision 1.1.2.2  1992/09/25  19:47:37  jaffe
 * 	Transarc delta: bab-ot5175-util-ktime-next-skip 1.1
 * 	  Selected comments:
 * 	    The ktime_next calculation would miss the next possible time to return
 * 	    if the minutes and seconds of the time at which the computation was
 * 	    started are larger than the minutes and seconds of the target time.  Once
 * 	    it fell into the mode of only incrementing the candidate time by one second
 * 	    at a time, it would wrap the hour, then start incrementing by minutes again.
 * 	    ot 5175
 * 	    Picked up the AFS/3 version of ktime, since they have fixed this bug
 * 	    since we first picked up the code.
 * 	[1992/09/23  20:06:54  jaffe]
 * 
 * Revision 1.1  1992/01/19  02:45:45  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* Copyright (C) 1990 Transarc Corporation - All rights reserved */
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#include <sys/types.h>
#include <time.h>
#include <ctype.h>
#include <ktime.h>
#include <pthread.h>

/* some forward reference dcls */
long ktime_ParseDate();
long ktime_InterpretDate();

/* some date parsing routines */

struct token {
    struct token *next;
    char *key;
};

static char *day[] = {
    "sun",
    "mon",
    "tue",
    "wed",
    "thu",
    "fri",
    "sat"
};

/* free token list returned by parseLine */
static LocalFreeTokens(alist)
    register struct token *alist; {
    register struct token *nlist;
    for(; alist; alist = nlist) {
	nlist = alist->next;
	free(alist->key);
	free(alist);
    }
    return 0;
}

static space(x)
int x; {
    if (x == 0 || x == ' ' || x == '\t' || x== '\n') return 1;
    else return 0;
}

static LocalParseLine(aline, alist)
    char *aline;
    struct token **alist; {
    char tbuffer[256];
    register char *tptr;
    int inToken;
    struct token *first, *last;
    register struct token *ttok;
    register int tc;
    
    inToken = 0;	/* not copying token chars at start */
    first = (struct token *) 0;
    last = (struct token *) 0;
    while (1) {
	tc = *aline++;
	if (tc == 0 || space(tc)) {    /* terminating null gets us in here, too */
	    if (inToken) {
		inToken	= 0;	/* end of this token */
		*tptr++ = 0;
		ttok = (struct token *) malloc(sizeof(struct token));
		ttok->next = (struct token *) 0;
		ttok->key = (char *) malloc(strlen(tbuffer)+1);
		strcpy(ttok->key, tbuffer);
		if (last) {
		    last->next = ttok;
		    last = ttok;
		}
		else last = ttok;
		if (!first) first = ttok;
	    }
	}
	else {
	    /* an alpha character */
	    if (!inToken) {
		tptr = tbuffer;
		inToken = 1;
	    }
	    if (tptr - tbuffer >= sizeof(tbuffer)) return -1;   /* token too long */
	    *tptr++ = tc;
	}
	if (tc == 0) {
	    /* last token flushed 'cause space(0) --> true */
	    if (last) last->next = (struct token *) 0;
	    *alist = first;
	    return 0;
	}
    }
}

/* keyword database for periodic date parsing */
static struct ptemp {
    char *key;
    long value;
} ptkeys [] = {
    "sun", 0x10000,
    "mon", 0x10001,
    "tue", 0x10002,
    "wed", 0x10003,
    "thu", 0x10004,
    "fri", 0x10005,
    "sat", 0x10006,
    "sunday", 0x10000,
    "monday", 0x10001,
    "tuesday", 0x10002,
    "wednesday", 0x10003,
    "thursday", 0x10004,
    "thur", 0x10004,
    "friday", 0x10005,
    "saturday", 0x10006,
    "am", 0x20000,
    "pm", 0x20001,
    "a.m.", 0x20000,
    "p.m.", 0x20001,
    0, 0,
};

/* ktime_DateOf
 * entry:
 *	atime - time in seconds (Unix std)
 * exit:
 *	return value - ptr to time in text form. Ptr is to a static string.
 */

char *ktime_DateOf(atime)
long atime; {
    static char tbuffer[30];
    register char *tp;
    tp=ctime(&atime);
    if (tp) {
	strcpy(tbuffer, tp);
	tbuffer[24] = 0;    /* get rid of new line */
    }
    else
	strcpy(tbuffer, "BAD TIME");
    return tbuffer;
}

/* ParseTime
 *	parse 12:33:12 or 12:33 or 12 into ktime structure
 * entry:
 *	astr - ptr to string to be parsed
 *	ak - ptr to struct for return value.
 * exit:
 *	0 - ak holds parsed time.
 *	-1 - error in format
 */

static ParseTime(ak, astr)
register char *astr;
register struct ktime *ak; {
    int field;
    long temp;
    register char *tp;
    register int tc;

    field = 0;	/* 0=hour, 1=min, 2=sec */
    temp = 0;
    ak->mask |= (KTIME_HOUR | KTIME_MIN | KTIME_SEC);
    for(tp=astr;;) {
	tc = *tp++;
	if (tc == 0 || tc == ':') {
	    if (field == 0)
		ak->hour = temp;
	    else if (field == 1)
		ak->min = temp;
	    else if (field == 2)
		ak->sec = temp;
	    temp = 0;
	    field++;
	    if (tc == 0) break;
	    continue;
	}
	else if	(!isdigit(tc)) return -1;   /* syntax error */
	else {
	    /* digit */
	    temp *= 10;
	    temp += tc - '0';
	}
    }
    if (ak->hour >= 24 || ak->min >= 60 || ak->sec >= 60) return -1;
    return 0;
}

/* ktime_ParsePeriodic
 *	Parses periodic date of form
 *		now | never | at [time spec] | every [time spec]
 *	where [time spec] is a ktime string.
 * entry:
 *	adate - string to be parsed
 *	ak - ptr to structure for returned ktime
 * exit:
 *	0 - parsed ktime in ak
 *	-1 - specification error
 */

/* -1 means error, 0 means now, otherwise returns time of next event */
int ktime_ParsePeriodic(adate, ak)
register struct ktime *ak;
char *adate; {
    struct token *tt;
    register long code;
    struct ptemp *tp;
    
    bzero(ak, sizeof(*ak));
    code = LocalParseLine(adate, &tt);
    if (code) return -1;
    for(;tt;tt=tt->next) {
	/* look at each token */
	if (strcmp(tt->key, "now") == 0) {
	    ak->mask |= KTIME_NOW;
	    return 0;
	}
	if (strcmp(tt->key, "never") == 0) {
	    ak->mask |= KTIME_NEVER;
	    return 0;
	}
	if (strcmp(tt->key, "at") == 0) continue;
	if (strcmp(tt->key, "every") == 0) continue;
	if (isdigit(tt->key[0])) {
	    /* parse a time */
	    code = ParseTime(ak, tt->key);
	    if (code) return -1;
	    continue;
	}
	/* otherwise use keyword table */
	for(tp = ptkeys;; tp++) {
	    if (tp->key == (char *) 0) {
		return -1;
	    }
	    if (strcmp(tp->key, tt->key) == 0) break;
	}
	/* now look at tp->value to see what we've got */
	if ((tp->value>>16) == 1) {
	    /* a day */
	    ak->mask |= KTIME_DAY;
	    ak->day = tp->value & 0xff;
	}
	if ((tp->value >> 16) == 2) {
	    /* am or pm token */
	    if ((tp->value & 0xff) == 1) {
		/* pm */
		if (!(ak->mask & KTIME_HOUR)) return -1;
		if (ak->hour < 12) ak->hour += 12;
		/* 12 is 12 PM */
		else if (ak->hour != 12) return -1;
	    }
	    else {
		/* am is almost a noop, except that we map 12:01 am to 0:01 */
		if (ak->hour > 12) return -1;
		if (ak->hour == 12) ak->hour = 0;
	    }
	}
    }
    return 0;
}

/* ktime_DisplayString
 *	Display ktime structure as English that could go into the ktime	parser
 * entry:
 *	aparm - ktime to be converted to string
 *	astring - ptr to string, for the result
 * exit:
 *	0 - astring contains ktime string.
 */
ktime_DisplayString(aparm, astring)
register char *astring;
struct ktime *aparm; {
    char tempString[50];

    if (aparm->mask & KTIME_NEVER) {
	strcpy(astring, "never");
	return 0;
    }
    else if (aparm->mask & KTIME_NOW) {
	strcpy(astring, "now");
	return 0;
    }
    else {
	strcpy(astring, "at");
	if (aparm->mask & KTIME_DAY) {
	    strcat(astring, " ");
	    strcat(astring, day[aparm->day]);
	}
	if (aparm->mask & KTIME_HOUR) {
	    if (aparm->hour > 12)
		sprintf(tempString, " %d", aparm->hour-12);
	    else if (aparm->hour == 0)
		strcpy(tempString, " 12");
	    else
		sprintf(tempString, " %d", aparm->hour);
	    strcat(astring, tempString);
	}
	if (aparm->mask & KTIME_MIN) {
	    sprintf(tempString, ":%02d", aparm->min);
	    strcat(astring, tempString);
	}
	if ((aparm->mask & KTIME_SEC) && aparm->sec != 0) {
	    sprintf(tempString, ":%02d", aparm->sec);
	    strcat(astring, tempString);
	}
	if (aparm->mask & KTIME_HOUR) {
	    if (aparm->hour >= 12)
		strcat(astring, " pm");
	    else
		strcat(astring, " am");
	}
    }
    return 0;
}

/* get next time that matches ktime structure after time afrom.  0 means now,
   -1 means error */
long ktime_next(aktime, afrom)
long afrom;
struct ktime *aktime; {
    /* try by guessing from now */
    struct tm *tsp;
    long start;	/* time to start looking */
    long probe;	/* time we're investigating */
    long tmask;
    struct ktime_date tdate;

    start = afrom + time(0); /* time to start search */
    tmask = aktime->mask;

    /* handle some special cases */
    if (tmask & KTIME_NEVER) return 0x7fffffff;
    if (tmask & KTIME_NOW) return 0;

    tsp = localtime(&start);	/* find out what time it is */

    tdate.year = tsp->tm_year;
    tdate.month = tsp->tm_mon+1;
    tdate.day = tsp->tm_mday;
    tdate.mask = KTIMEDATE_YEAR | KTIMEDATE_MONTH | KTIMEDATE_DAY |
	KTIMEDATE_HOUR | KTIMEDATE_MIN | KTIMEDATE_SEC;
    tdate.hour = aktime->hour;	/* edit in our changes */
    tdate.min = aktime->min;
    tdate.sec = aktime->sec;
    probe = ktime_InterpretDate(&tdate);
    /* probe is now the right time, but perhaps the wrong day.  Iterate, using
       localtime, until we're happy */
    for (;;probe += (24*3600)) {	/* skip to same time, but tomorrow */
	if (probe < start) continue;
	if ((tmask & KTIME_DAY) == 0)	/* don't care about day, we're done */
	    break;
	tsp = localtime(&probe);
	if (tsp->tm_wday == aktime->day) break;	/* day matches, we're done */
    }
    return probe;
}


/* compare date in both formats, and return as in strcmp */
static KTimeCmp(aktime, atm)
register struct ktime *aktime;
register struct tm *atm; {
    register long tmask;

    /* don't compare day of the week, since we can't tell the
       order in a cyclical set.  Caller must check for equality, if
       she cares */
    tmask = aktime->mask;
    if (tmask & KTIME_HOUR) {
	if (aktime->hour > atm->tm_hour) return 1;
	if (aktime->hour < atm->tm_hour) return -1;
    }
    if (tmask & KTIME_MIN) {
	if (aktime->min > atm->tm_min) return 1;
	if (aktime->min < atm->tm_min) return -1;
    }
    if (tmask & KTIME_SEC) {
	if (aktime->sec > atm->tm_sec) return 1;
	if (aktime->sec < atm->tm_sec) return -1;
    }
    return 0;
}

/* compare date in both formats, and return as in strcmp */
static KDateCmp(akdate, atm)
register struct ktime_date *akdate;
register struct tm *atm; {
    if (akdate->year > atm->tm_year) return 1;
    if (akdate->year < atm->tm_year) return -1;
    if (akdate->month > atm->tm_mon) return 1;
    if (akdate->month < atm->tm_mon) return -1;
    if (akdate->day > atm->tm_mday) return 1;
    if (akdate->day < atm->tm_mday) return -1;
    if (akdate->mask & KTIMEDATE_HOUR) {
	if (akdate->hour > atm->tm_hour) return 1;
	if (akdate->hour < atm->tm_hour) return -1;
    }
    if (akdate->mask & KTIMEDATE_MIN) {
	if (akdate->min > atm->tm_min) return 1;
	if (akdate->min < atm->tm_min) return -1;
    }
    if (akdate->mask & KTIMEDATE_SEC) {
	if (akdate->sec > atm->tm_sec) return 1;
	if (akdate->sec < atm->tm_sec) return -1;
    }
    return 0;
}

/* ktime_DateToLong
 *	Converts a ktime date string into a long
 * entry:
 *	adate - ktime date string
 *	along - ptr to long
 * exit:
 *	0 - along contains converted date.
 */

long ktime_DateToLong(adate, along)
long *along;
char *adate; {
    struct ktime_date tdate;
    register long code;

    /* parse the date into a ktime_date structure */
    code = ktime_ParseDate(adate, &tdate);
    if (code) return code;	/* failed to parse */

    code = ktime_InterpretDate(&tdate);	/* interpret date as seconds since 1970 */
    *along = code;	/* return it */
    return 0;		/* and declare no errors */
}

/* ktime_ParseDate
 * 	parse date string into ktime_date structure
 * entry:
 *	adate - string to be parsed
 *	akdate - ptr to ktime_date for result
 * exit:
 *	0 - akdate contains converted date
 *	-1 - parsing failure
 */

long ktime_ParseDate(adate, akdate)
char *adate;
struct ktime_date *akdate; {
    int code;
    long month, day, year, hour, min;
    char never[7];

    lcstring (never, adate, sizeof(never));
    if (strcmp (never, "never") == 0) akdate->mask = KTIMEDATE_NEVER;
    else if (strcmp (never, "now") == 0) akdate->mask = KTIMEDATE_NOW;
    else akdate->mask = 0;
    if (akdate->mask) return 0;

    code = sscanf(adate, "%d/%d/%d %d:%d", &month, &day, &year, &hour, &min);
    akdate->mask = KTIMEDATE_YEAR | KTIMEDATE_MONTH | KTIMEDATE_DAY
	| KTIMEDATE_HOUR | KTIMEDATE_MIN | KTIMEDATE_SEC;
    if ((year < 0) || (month < 1) || (month > 12) ||
	(day < 1) || (day > 31) /* more or less */) return -2;
    if (year < 69) year += 100;		/* make 1/1/1 => Jan 1, 2001 */
    else if (year >= 1900) year -= 1900;/* allow 1/1/2001 to work */
    else if (year > 99) return -2;	/* only allow 2 or 4 digit years */
    if (code == 3) {
	akdate->year = year;
	akdate->month = month;
	akdate->day = day;
	akdate->hour = 0;
	akdate->min = 0;
	akdate->sec = 0;
    }
    else if (code == 5) {
	if ((hour < 0) || (hour > 23) || (min < 0) || (min > 59)) return -2;
	akdate->year = year;
	akdate->month = month;
	akdate->day = day;
	akdate->hour = hour;
	akdate->min = min;
	akdate->sec = 0;
    }
    else return -1;

    /* done successfully */
    return 0;
}

/* get useful error message to print about date input format */
char *ktime_GetDateUsage() {
    return "date format is 'mm/dd/yy [hh:mm]', using a 24 hour clock";
}


/* ktime_InterpretDate
 *	Converts ktime_date to a long
 * entry:
 *	akdate - date to be converted/interpreted
 * exit:
 *	returns KTIMEDATE_NEVERDATE - if never flag was set, or
 *	date converted to long.
 */

long ktime_InterpretDate(akdate)
  struct ktime_date *akdate;
{
    register unsigned long tresult;
    register unsigned long tbit;
    unsigned long temp;
    register struct tm *tsp;

    if (akdate->mask & KTIMEDATE_NOW) return time(0);
    if (akdate->mask & KTIMEDATE_NEVER) return KTIMEDATE_NEVERDATE;

    tbit = 1<<30;		/* start off at max signed result */
    tresult = 0;		/* result to return */
    while(tbit > 0) {
	temp = tresult + tbit;	/* see if adding this bit keeps us < akdate */
	tsp = localtime((time_t *)&temp);
	tsp->tm_mon++;
#ifdef notdef
	if (tsp->tm_mon == 0) {
	    tsp->tm_mon = 12;
	    tsp->tm_year--;
	}
#endif
	if (KDateCmp(akdate, tsp) >= 0) {
	    /* if temp still represents earlier than date than we're searching
             * for, add in bit as increment, otherwise use old value and look
             * for smaller increment */
	    tresult = temp;
	}
	tbit = tbit >> 1;	/* try next bit */
    }

    return tresult;
}
