/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: gmt_mktime.c,v $
 * Revision 1.1.2.1  1996/06/05  21:10:22  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:06:44  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/* This code placed in the public domain by Mark W. Eichin */

#include <stdio.h>

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#ifdef TIME_WITH_SYS_TIME
#include <time.h>
#endif
#else
#include <time.h>
#endif

/* take a struct tm, return seconds from GMT epoch */
/* like mktime, this ignores tm_wday and tm_yday. */
/* unlike mktime, this does not set them... it only passes a return value. */

static days_in_month[12] = {
0,				/* jan 31 */
31,				/* feb 28 */
59,				/* mar 31 */
90,				/* apr 30 */
120,				/* may 31 */
151,				/* jun 30 */
181,				/* jul 31 */
212,				/* aug 31 */
243,				/* sep 30 */
273,				/* oct 31 */
304,				/* nov 30 */
334				/* dec 31 */
};

#define hasleapday(year) (year%400?(year%100?(year%4?0:1):0):1)

time_t gmt_mktime(t)
     struct tm* t;
{
  time_t accum;

#define assert_time(cnd) if(!(cnd)) return (time_t) -1

  assert_time(t->tm_year>=70);
  assert_time(t->tm_year<=138);
  assert_time(t->tm_mon>=0);
  assert_time(t->tm_mon<=11);
  assert_time(t->tm_mday>=0);
  assert_time(t->tm_mday<=31);
  assert_time(t->tm_hour>=0);
  assert_time(t->tm_hour<=23);
  assert_time(t->tm_min>=0);
  assert_time(t->tm_min<=59);
  assert_time(t->tm_sec>=0);
  assert_time(t->tm_sec<=62);

#undef assert_time


  accum = t->tm_year - 70;
  accum *= 365;			/* 365 days/normal year */

  /* add in leap day for all previous years */
  accum += (t->tm_year - 69) / 4;
  /* add in leap day for this year */
  if(t->tm_mon >= 2)		/* march or later */
    if(hasleapday(t->tm_year)) accum += 1;

  accum += days_in_month[t->tm_mon];
  accum += t->tm_mday-1;	/* days of month are the only 1-based field */
  accum *= 24;			/* 24 hour/day */
  accum += t->tm_hour;
  accum *= 60;			/* 60 minute/hour */
  accum += t->tm_min;
  accum *= 60;			/* 60 seconds/minute */
  accum += t->tm_sec;

  return accum;
}
