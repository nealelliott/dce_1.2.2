/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: kreltime.c,v $
 * Revision 1.1.426.1  1996/10/02  21:13:18  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:51:06  damon]
 *
 * Revision 1.1.421.3  1994/07/13  22:30:31  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  12:02:32  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:04:34  mbs]
 * 
 * Revision 1.1.421.2  1994/06/09  14:24:54  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:36:58  annie]
 * 
 * Revision 1.1.421.1  1994/02/04  20:35:38  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:20:21  devsrc]
 * 
 * Revision 1.1.419.1  1993/12/07  17:37:49  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  18:09:06  jaffe]
 * 
 * Revision 1.1.3.4  1993/01/21  16:31:28  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:25:07  cjd]
 * 
 * Revision 1.1.3.3  1993/01/13  19:51:10  shl
 * 	Transarc delta: jaffe-cleanup-2.3-compiler-warnings 1.8
 * 	  Selected comments:
 * 	    Fix compiler warnings found when moving to OSF/1 1.1.1.
 * 	    add casts for calls to bzero.
 * 	    Remove the function dacl_SetOwnerControl.  It is not used.
 * 	[1993/01/12  22:32:30  shl]
 * 
 * Revision 1.1.3.2  1992/11/24  20:42:48  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:38:30  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:45:39  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* Copyright (C) 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <sys/types.h>
#include <ktime.h>
#include <time.h>
#include <ctype.h>
#include <pthread.h>

#if defined(AFS_HPUX_ENV) || defined(AFS_SVR4_ENV)
#include <string.h>
#else
#include <strings.h>
#endif /* SVR4 */

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/util/kreltime.c,v 1.1.426.1 1996/10/02 21:13:18 damon Exp $")

/* maximum values for relative dates */

#define	MAX_YEAR_VALUE	0
#define	MAX_MONTH_VALUE 11
#define	MAX_DAY_VALUE	30

/* for parsing relative expiration dates */
static struct parseseqS
{
    long ps_field;
    char ps_keychar;
    long ps_maxValue;
} parseseq[] = {
    KTIMEDATE_YEAR, 'y', MAX_YEAR_VALUE,	/* no max. value */
    KTIMEDATE_MONTH, 'm', MAX_MONTH_VALUE,	/* months max. 12 */
    KTIMEDATE_DAY, 'd',	MAX_DAY_VALUE,		/* days max. 31 */
    0, 0, 0,
};

/* Encodings to and from relative dates. The caller is responsible for
 * enforcing appropriate use of these routines
 */


/* ktimeRelDate_ToLong
 *	converts a relative ktime date into a long.
 * exit:
 *	long value of encoded date.
 */

long
ktimeRelDate_ToLong(kdptr)
     struct ktime_date *kdptr;
{
    long retval;

    retval = (((kdptr->year*(MAX_MONTH_VALUE+1)) +
	       kdptr->month)*(MAX_DAY_VALUE+1)) + kdptr->day;
    return(retval);
}

/* LongTo_ktimeRelDate
 *	Convert a relative date encoded in a long - back into a ktime_date
 *	structure
 */

LongTo_ktimeRelDate(longDate, kdptr)
     long longDate;
     struct ktime_date *kdptr;
{
    bzero((char *)kdptr, sizeof(*kdptr));
    
    kdptr->day = longDate % (MAX_DAY_VALUE + 1);
    if ( kdptr->day != 0 )
    	kdptr->mask |= KTIMEDATE_DAY;

    longDate = longDate/(MAX_DAY_VALUE + 1);
    
    kdptr->month = longDate % (MAX_MONTH_VALUE + 1);
    if ( kdptr->month != 0 )
    	kdptr->mask |= KTIMEDATE_MONTH;

    longDate = longDate/(MAX_MONTH_VALUE + 1);
    
    kdptr->year = longDate;
    if ( kdptr->year != 0 )
    	kdptr->mask |= KTIMEDATE_YEAR;

    return(0);
}

/* ktimeDate_FromLong
 *	Converts a time in seconds, to a time (in ktime_date format).
 *	Result is a conventional ktime_date structure.
 *	placed in the supplied structure
 * entry:
 *	timeSecs - time in seconds
 *	ktimePtr - ptr to struct for the return value
 */

ktimeDate_FromLong(timeSecs, ktimePtr)
     long timeSecs;
     struct ktime_date *ktimePtr;
{
    struct tm *timePtr;

    timePtr = localtime(&timeSecs);

    /* copy the relevant fields */
    ktimePtr->sec = timePtr->tm_sec;
    ktimePtr->min = timePtr->tm_min;
    ktimePtr->hour = timePtr->tm_hour;
    ktimePtr->day = timePtr->tm_mday;
    ktimePtr->month = timePtr->tm_mon+1;
    ktimePtr->year = timePtr->tm_year;

    ktimePtr->mask = KTIMEDATE_YEAR | KTIMEDATE_MONTH | KTIMEDATE_DAY |
    	KTIMEDATE_HOUR | KTIMEDATE_MIN | KTIMEDATE_SEC;

    return(0);
}

#define	RD_DIGIT_LIMIT	4			/* max. no. digits permitted */

/* ParseRelDate
 *	Parses a relative date of the form  <n>y<n>m<n>d representing years
 *	months and days. <n> is limited to RD_DIGIT_LIMIT digits in length
 *	and is further restricted by the maximums specified at the head
 *	of the file.
 * entry:
 *	dateStr - ptr to string to parse. Leading white space ingnored.
 * exit:
 *	returns a ptr to a static ktime_date structure containing
 *		appropriately set fields. The mask field is unused.
 *	0 - error in date specification
 */

long
ParseRelDate(dateStr, relDatePtr)
     char *dateStr;
     struct ktime_date *relDatePtr;
{
    struct parseseqS *psPtr;
    long value, digit_limit;
    long type_index;

    bzero((char *)relDatePtr, sizeof(*relDatePtr));
    type_index = 0;

    while ( 1 )
    { /*w*/

	while ( isspace(*dateStr) )                /* skip leading whitespace */
                dateStr++;

	if ( isdigit(*dateStr) == 0 )
    		goto error;

	digit_limit = RD_DIGIT_LIMIT;
	value = 0;
	while ( isdigit(*dateStr) )
	{
	    value = value*10 + *dateStr - '0';
	    dateStr++;
	    if ( digit_limit-- == 0 )
	    	goto error;
	}

	psPtr = &parseseq[type_index];
	/* determine the units. Search for a matching type character */
	while ( (psPtr->ps_keychar != *dateStr)
	&&  	(psPtr->ps_keychar != 0)
	      )
	{
	    type_index++;
	    psPtr = &parseseq[type_index];
	}

	/* no matching type found */
	if ( psPtr->ps_keychar == 0 )
 		goto error;

	/* check the bounds on the maximum value. Can't be negative
	 * and if a maximum value is specified, check against it
	 */
	if ( (value < 0)
	||   ((psPtr->ps_maxValue > 0) && (value > psPtr->ps_maxValue))
	   )
	    	goto error;

	/* save computed value in the relevant type field */
	switch ( psPtr->ps_field )
	{
	  case KTIMEDATE_YEAR:
	    relDatePtr->year = value;
	    relDatePtr->mask |= KTIMEDATE_YEAR;
	    break;

	  case KTIMEDATE_MONTH:
	    if ( value > MAX_MONTH_VALUE )
	    	goto error;
	    relDatePtr->month = value;
	    relDatePtr->mask |= KTIMEDATE_MONTH;
	    break;

	  case KTIMEDATE_DAY:
	    if ( value > MAX_DAY_VALUE )
	    	goto error;

	    relDatePtr->mask |= KTIMEDATE_DAY;
	    relDatePtr->day = value;
	    break;

	  default:
	    goto error;
	}
	dateStr++;				/* next digit */

	if ( *dateStr == 0 )
	{
	    /* no more chars to process, return the result */
	    return(0);
	}
    } /*w*/

error:
    return(1);
}

/* RelDatetoString
 *	returns a static string representing the relative date. This is in
 *	a format acceptable to the relative date parser.
 * entry:
 *	datePtr - relative date to be converted.
 * exit:
 *	ptr to static string
 */

char *
RelDatetoString(datePtr)
     struct ktime_date *datePtr;
{
    static char dateString[64];
    char tempstring[64], *sptr;

    dateString[0] = 0;
    sptr = &dateString[0];

    if ( datePtr->mask & KTIMEDATE_YEAR )
    {
	sprintf(tempstring, "%-dy", datePtr->year);
	strcat(sptr, tempstring);
    }

    if ( datePtr->mask & KTIMEDATE_MONTH )
    {
	strcat(sptr, " ");
	sprintf(tempstring, "%-dm", datePtr->month);
	strcat(sptr, tempstring);
    }

    if ( datePtr->mask & KTIMEDATE_DAY )
    {
	strcat(sptr, " ");
	sprintf(tempstring, "%-dd", datePtr->day);
	strcat(sptr, tempstring);
    }
    return(sptr);
}

/* Add_RelDate_to_Time
 *	Returns current time with a relative time added. Note that the 
 *	computation adds in most significant fields first, i.e. year, month
 *	day etc. Addition of least significant fields would produce different
 *	results (depending on the data).
 * entry:
 *	relDatePtr - a ktime_date containing a relative time specification
 * exit:
 *	returns specified time with relative time added.
 */

long
Add_RelDate_to_Time(relDatePtr, atime)
     struct ktime_date *relDatePtr;
     long atime;
{
    long moreYears;
    static struct ktime_date absDate;
    long remtime = 0;

    ktimeDate_FromLong(atime, &absDate);	/* convert to ktime */

    /* add in years */
    if ( relDatePtr->mask & KTIMEDATE_YEAR )
	    absDate.year += relDatePtr->year;

    /* add in months */
    if ( relDatePtr->mask & KTIMEDATE_MONTH )
    	absDate.month += relDatePtr->month;

    if ( absDate.month > 12 )
    {
	moreYears = absDate.month / 12;
	absDate.month = absDate.month % 12;
	absDate.year += moreYears;
    }
    
    /* day computations depend upon month size, so do these in seconds */
    atime = ktime_InterpretDate(&absDate);

    /* We now have atime which would be <= 0x7fffffff. Before adding the
     * seconds for the remaining days, we must make sure this does not
     * overflow.
     */
    if ( relDatePtr->mask & KTIMEDATE_DAY )
	    remtime = relDatePtr->day*24*60*60;

    if ( relDatePtr->mask & KTIMEDATE_HOUR )
	    remtime += relDatePtr->hour*60*60;

    if ( relDatePtr->mask & KTIMEDATE_MIN )
	    remtime += relDatePtr->min*60;

    if ( relDatePtr->mask & KTIMEDATE_SEC )
	    remtime += relDatePtr->sec;

    /* Guard against overflow of a signed quantity */
    if ((0x7fffffff - atime) >= remtime)
      atime += remtime;
    else
      atime = 0x7fffffff;		/* set to the limit */

    return(atime);
}	
