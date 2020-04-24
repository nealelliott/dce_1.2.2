/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rantest_api.c,v $
 * Revision 1.1.8.3  1996/02/17  23:32:25  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:46  marty]
 *
 * Revision 1.1.8.2  1995/12/13  21:19:09  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	Revision /main/HPDCE02/jrr_1.2_mothra/1  1995/12/07  20:35 UTC  dat
 * 	Change ifdef to get same behavior on AIX as on HPUX, to eliminate huge results file
 * 
 * 	Revision /main/HPDCE02/3  1995/06/15  20:07 UTC  pare
 * 	Merge changes
 * 
 * 	Revision /main/HPDCE02/pare_mothra/1  1995/06/15  20:05 UTC  pare
 * 	Change iteration count from 50000 to 500
 * 
 * 	Revision /main/HPDCE02/2  1994/07/25  15:18 UTC  pare
 * 	Change tet/tet_api.h to tet_api.h
 * 
 * 	Revision /main/HPDCE02/1  1994/06/30  18:23 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	Revision /main/HPDCE01/3  1994/03/29  21:30 UTC  pare
 * 
 * 	Revision /main/HPDCE01/pare_rodan/2  1994/03/29  21:25 UTC  pare
 * 	clean up merge problems
 * 
 * 	Revision /main/HPDCE01/pare_rodan/1  1994/03/28  20:18 UTC  pare
 * 	tet api is now in external tree
 * 
 * 	Revision /main/HPDCE01/1  1994/01/31  20:00  pare
 * 	merge kk and hpdce01
 * 	[1995/12/13  21:18:20  root]
 * 
 * Revision 1.1.7.2  1993/07/09  17:13:59  root
 * 	Initial King Kong branch
 * 	[1993/07/09  17:13:28  root]
 * 
 * Revision 1.1.5.5  1993/05/19  17:49:42  truitt
 * 	Fixed problem with the output generation.
 * 	[1993/05/19  17:49:23  truitt]
 * 
 * Revision 1.1.5.4  1993/04/16  17:42:32  truitt
 * 	Put under TET.
 * 	[1993/04/16  17:42:13  truitt]
 * 
 * Revision 1.1.5.3  1993/04/12  14:36:47  truitt
 * 	Enhanced error reporting.  Changed runtime output so it is easier to
 * 	track execution of test.  Added setenv("DTS_TZ",...) to beginning of
 * 	test so internal dce_tzset() function would interpret random dates
 * 	correctly.
 * 	[1993/04/12  14:36:23  truitt]
 * 
 * Revision 1.1.5.2  1993/03/17  20:15:56  truitt
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/17  20:14:39  truitt]
 * 
 * Revision 1.1.1.7  1993/03/11  22:23:21  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.3.3  1993/01/04  18:24:10  smythe
 * 	fixed bug in month and year number test for 1974 hpux check
 * 	added count output in hpux path for error reporting
 * 	[1993/01/04  18:14:42  smythe]
 * 
 * Revision 1.1.2.2  1992/12/31  20:35:14  htf
 * 	Embedded copyright notice
 * 	[1992/12/31  20:28:28  htf]
 * 
 * Revision 1.1.3.2  1992/12/10  18:29:59  smythe
 * 	changed to only run 50000 iterations under hpux
 * 	changed to minimize output under hpux
 * 	paralleled AIX fix to avoid hpux problem with mktime changing its argument
 * 	added test to avoid failure due to wrong hpux 1974 dst end date
 * 	[1992/12/10  18:28:52  smythe]
 * 
 * Revision 1.1.1.6  1992/10/01  21:34:15  osfrcs
 * 	Loading drop Dce1_0
 * 
 * Revision 1.1.1.6  1992/09/21  18:48:14  osfrcs
 * 	    Loading drop Dce1_0_1fnl
 * 
 * Revision 1.1  1992/01/19  14:38:13  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Copyright (c) 1990, 1991 by
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
 * ABSTRACT:    This is a test program for the portable API for DECdts
 *
 * ENVIRONMENT:	VAX/VMS and Ultrix
 *
 * AUTHORS:
 *		Joe Comuzzi
 *		Distributed Systems Services (DSS) 
 * 
 * USAGE:
 *		Compile this file and link it with the API. This program
 *		will generate random test cases and test the API. The program
 *		will stop, displaying information if a failure is found.
 *		Experience indicated most bugs (that will be discovered by
 *		this technique) show up in the first few thousand test cases.
 */

#include <dce/utc.h>
#include <stdlib.h>
#include <math.h>
#include <tet_api.h>

void (*tet_startup)() = NULL;
void (*tet_cleanup)() = NULL;
void rantest();

struct tet_testlist tet_testlist[] =
   {{rantest, 1},
    {NULL, NULL}};

#ifdef ultrix
#ifdef _TIME_H_
#define HAS_GMTOFF
#else
typedef long time_t;
#endif
#endif

#ifdef vms
#pragma nostandard       /* VMS only -  non port */
noshare                  /* make this storage class noshr for tzname */
#pragma standard         /* Continue checking for portability */

extern char *tzname[];
#endif

#ifdef NOV1974_BUG
/*
 * Define this macro if your OS has the 'traditional' ctime bug
 * dealing with the last sunday in October, 1974.
 */
#endif

#define TM_YEAR_BASE	(1900)

char tmpstr[100];

/*
 * This routine returns a 32-bit random number
 */
unsigned long int random32()

{
    return(((rand() & 0x7fff) << 17) +
	   ((rand() & 0x7fff) << 2) + (rand() & 0x3) );
}

/*
 * This routine compares the time field from two utc_t's and print an
 * error if they are different
 *
 * Input:
 *	utc1	1st utc_t
 *	utc2	2nd utc_t
 *	tag	string indicating which test failed
 *
 */
int compare_time( utc1, utc2, tag )
utc_t *utc1;
utc_t *utc2;
char  *tag;
{
    timespec_t t1, t2;

    if (utc_bintime(&t1, (timespec_t *)0, (long *)0, utc1) < 0) {
	sprintf(tmpstr,"1st utc_bintime returned error in %s time compare\n", tag);
	tet_infoline(tmpstr);
	return(0);
	}

    if (utc_bintime(&t2, (timespec_t *)0, (long *)0, utc2) < 0) {
	sprintf(tmpstr,"2nd utc_bintime returned error in %s time compare\n", tag);
	tet_infoline(tmpstr);
	return(0);
	}

    if (t1.tv_sec != t2.tv_sec || t1.tv_nsec != t2.tv_nsec) {
	sprintf(tmpstr,"time was not equal in %s compare\n", tag);
	tet_infoline(tmpstr);
	return(0);
	}

    return(1);

}

/*
 * This routine compares the inaccuracy field from two utc_t's and print an
 * error if they are different
 *
 * Input:
 *	utc1	1st utc_t
 *	utc2	2nd utc_t
 *	tag	string indicating which test failed
 *
 */
int compare_inaccuracy( utc1, utc2, tag )
utc_t *utc1;
utc_t *utc2;
char  *tag;
{
    timespec_t i1, i2;

    if (utc_bintime((timespec_t *)0, &i1, (long *)0, utc1) < 0) {
	sprintf(tmpstr,"1st utc_bintime returned error in %s inacc compare\n", tag);
	tet_infoline(tmpstr);
	return(0);
	}

    if (utc_bintime((timespec_t *)0, &i2, (long *)0, utc2) < 0) {
	sprintf(tmpstr,"2nd utc_bintime returned error in %s inacc compare\n", tag);
	tet_infoline(tmpstr);
	return(0);
	}

    if (i1.tv_sec != i2.tv_sec ||
	(i1.tv_nsec != i2.tv_nsec && i1.tv_nsec != i2.tv_nsec + 100 &&
	 i1.tv_nsec != i2.tv_nsec - 100)) {
	sprintf(tmpstr,"inaccuarcy was not equal in %s compare\n", tag);
	tet_infoline(tmpstr);
	return(0);
	}

    return(1);

}

/*
 * This routine compares the TDF field from two utc_t's and print an
 * error if they are different
 *
 * Input:
 *	utc1	1st utc_t
 *	utc2	2nd utc_t
 *	tag	string indicating which test failed
 *
 */
int compare_tdf( utc1, utc2, tag )
utc_t *utc1;
utc_t *utc2;
char  *tag;
{
    long tdf1, tdf2;

    if (utc_bintime((timespec_t *)0, (timespec_t *)0, &tdf1, utc1) < 0) {
	sprintf(tmpstr,"1st utc_bintime returned error in %s TDF compare\n", tag);
	tet_infoline(tmpstr);
	return(0);
	}

    if (utc_bintime((timespec_t *)0, (timespec_t *)0, &tdf2, utc2) < 0) {
	sprintf(tmpstr,"2nd utc_bintime returned error in %s TDF compare\n", tag);
	tet_infoline(tmpstr);
	return(0);
	}

    if (tdf1 != tdf2) {
	sprintf(tmpstr,"TDF was not equal in %s compare\n", tag);
	tet_infoline(tmpstr);
	return(0);
	}

    return(1);

}

/*
 * This routine compares the time field from two relative utc_t's and print
 * an error if they are different
 *
 * Input:
 *	utc1	1st relative utc_t
 *	utc2	2nd relative utc_t
 *	tag	string indicating which test failed
 *
 */
int compare_rel_time( utc1, utc2, tag )
utc_t *utc1;
utc_t *utc2;
char  *tag;
{
    reltimespec_t t1, t2;

    if (utc_binreltime(&t1, (timespec_t *)0, utc1) < 0) {
	sprintf(tmpstr,"1st utc_binreltime returned error in %s time compare\n", tag);
	tet_infoline(tmpstr);
	return(0);
	}

    if (utc_binreltime(&t2, (timespec_t *)0, utc2) < 0) {
	sprintf(tmpstr,"2nd utc_binreltime returned error in %s time compare\n", tag);
	tet_infoline(tmpstr);
	return(0);
	}

    if (t1.tv_sec != t2.tv_sec || t1.tv_nsec != t2.tv_nsec) {
	sprintf(tmpstr,"time was not equal in %s compare\n", tag);
	tet_infoline(tmpstr);
	return(0);
	}

    return(1);

}

/*
 * This routine compares the inaccuracy field from two relative utc_t's and
 * print an error if they are different
 *
 * Input:
 *	utc1	1st relative utc_t
 *	utc2	2nd relative utc_t
 *	tag	string indicating which test failed
 *
 */
int compare_rel_inaccuracy( utc1, utc2, tag )
utc_t *utc1;
utc_t *utc2;
char  *tag;
{
    timespec_t i1, i2;

    if (utc_binreltime((reltimespec_t *)0, &i1, utc1) < 0) {
	sprintf(tmpstr,"1st utc_binreltime returned error in %s inacc compare\n", tag);
	tet_infoline(tmpstr);
	return(0);
	}

    if (utc_binreltime((reltimespec_t *)0, &i2, utc2) < 0) {
	sprintf(tmpstr,"2nd utc_binreltime returned error in %s inacc compare\n", tag);
	tet_infoline(tmpstr);
	return(0);
	}

    if (i1.tv_sec != i2.tv_sec ||
	(i1.tv_nsec != i2.tv_nsec && i1.tv_nsec != i2.tv_nsec + 100 &&
	 i1.tv_nsec != i2.tv_nsec - 100)) {
	sprintf(tmpstr,"inaccuarcy was not equal in %s compare\n", tag);
	tet_infoline(tmpstr);
	return(0);
	}

    return(1);

}

/*
 * This routine rounds a time and inaccuracy to milliseonds
 */
int round_to_msec( utc )
utc_t *utc;
{
    timespec_t t, i;
    long tdf;

    if (utc_bintime(&t, &i, &tdf, utc) < 0) {
	sprintf(tmpstr,"utc_bintime returned error (rounding to msec)\n");
	tet_infoline(tmpstr);
	return(0);
    }

    t.tv_nsec = 1000000 * (t.tv_nsec/1000000);
    i.tv_nsec = 1000000 * (i.tv_nsec/1000000);

    if (utc_mkbintime(utc, &t, &i, tdf) < 0) {
	sprintf(tmpstr,"utc_mkbintime returned error (rounding to msec)\n");
	tet_infoline(tmpstr);
	return(0);
    }

    return(1);
}

/*
 * This routine rounds a relative time and inaccuracy to milliseonds
 */
int round_rel_to_msec( utc )
utc_t *utc;
{
    reltimespec_t t;
    timespec_t i;

    if (utc_binreltime(&t, &i, utc) < 0) {
	sprintf(tmpstr,"utc_binreltime returned error (rounding to msec)\n");
	tet_infoline(tmpstr);
	return(0);
    }

    t.tv_nsec = 1000000 * (t.tv_nsec/1000000);
    i.tv_nsec = 1000000 * (i.tv_nsec/1000000);

    if (utc_mkbinreltime(utc, &t, &i) < 0) {
	sprintf(tmpstr,"utc_mkbinreltime returned error (rounding to msec)\n");
	tet_infoline(tmpstr);
	return(0);
    }

    return(1);
}

void rantest()
{
    timespec_t  t, i, t2, i2, i3, ari;
    reltimespec_t rt, rt2, art;
    struct tm	im, lcl, ulcl, gmt, ugmt, uany, urel;
#if defined(_AIX) || defined(__hpux)
    struct tm	ulcl1;	/* don't use ulcl for mktime() call  */ 
#endif /* #if defined(_AIX) || defined(__hpux) */
    utc_t	utc, utc2, relutc, relutc2, relutc3;
    utc_t	utch, utcl, utcm, utclm, utcmh;
    enum utc_cmptype relation;
    char	*ulcltz, *lcltz, *ugmttz, *gmttz, *uanytz;
    char	ustr[100], urelstr[100];
    char	tmptzname[100];
    long	tdf, tdf2;
    long	tns, ins;
    long	x;
    int		count, ok, maxcount;
    int		isdst;
    unsigned char *p;

    count = 0;
    ok = 1;

    /*
     * utc_gmtzone only returns constants, so no need to check it in the
     * loop
     */

    if (utc_gmtzone(tmptzname, (size_t)100, &tdf, &isdst, (utc_t *)0) < 0) {
	tet_infoline("utc_gmtzone returned error\n");
	ok = 0;
    }
    
    if (tdf != 0) {
	tet_infoline("utc_gmtzone returned TDF not equal to zero!\n");
	ok = 0;
    }

    if (isdst != 0) {
	tet_infoline("utc_gmtzone returned DST flag not equal to zero!\n");
	ok = 0;
    }

    if (strcmp(tmptzname, "GMT") != 0 &&
	strcmp(tmptzname, "UTC") != 0 &&
	strcmp(tmptzname, "UTC0") != 0 &&
	strcmp(tmptzname, "Z") != 0) {
	tet_infoline("utc_gmtzone returned incorrect timezone name!\n");
	ok = 0;
    }

#if defined(_AIX) || defined(__hpux)
    setenv("DTS_TZ","EST5EDT",1);
    while (ok && count < 500) {
#else /* #if defined(_AIX) || defined(__hpux) */
    while (ok) {
#endif /* #if defined(_AIX) || defined(__hpux) */

	t.tv_sec = random32() / 2;
	t.tv_nsec = 100 * (random32() / 500);
	rt.tv_sec = (long int)random32();
	rt.tv_nsec = 100 * (random32() / 500);
	i.tv_sec = random32() / 8192;
	i.tv_nsec = 100 * (random32() / 500);
	tdf = (random32() / (65536*60)) * 60;  /* -32768 < tdf < 32767 */

	if (utc_mkbintime(&utc, &t, &i, tdf) < 0) {
	    tet_infoline("utc_mkbintime returned error\n");
	    ok = 0;
	}

	p = (unsigned char *) &utc;
	if (((p[15] >> 4) & 0x7) != 1) {
	    tet_infoline("utc timestamp version is not 1\n");
	    ok = 0;
        }

	if (utc_bintime(&t2, &i2, &tdf2, &utc) < 0) {
	    tet_infoline("utc_bintime returned error\n");
	    ok = 0;
	}

	if (t.tv_sec != t2.tv_sec || t.tv_nsec != t2.tv_nsec ||
	    i.tv_sec != i2.tv_sec || i.tv_nsec != i2.tv_nsec ||
	    tdf != tdf2) {
	    tet_infoline("utc_mkbintime - utc_bintime aren't inverses\n");
	    sprintf(tmpstr," input  %d.%09d (%x %08x) +/- %d.%09d (%x %08x) %d (%x)\n",
		            t.tv_sec, t.tv_nsec, t.tv_sec, t.tv_nsec,
		            i.tv_sec, i.tv_nsec, i.tv_sec, i.tv_nsec,
		            tdf, tdf);
	    tet_infoline(tmpstr);
	    sprintf(tmpstr," output %d.%09d (%x %08x) +/- %d.%09d (%x %08x) %d (%x)\n",
		            t2.tv_sec, t2.tv_nsec, t2.tv_sec, t2.tv_nsec,
		            i2.tv_sec, i2.tv_nsec, i2.tv_sec, i2.tv_nsec,
		            tdf2, tdf2);
	    tet_infoline(tmpstr);
	    ok = 0;
	}

	if (utc_localtime(&ulcl, &tns, &im, &ins, &utc) < 0) {
	    tet_infoline("utc_localtime returned error\n");
	    ok = 0;
	}
    
#if !defined(_AIX) && !defined(__hpux)
	sprintf(tmpstr,"ulcl = %d/%d/%d %d:%02d:%02d %s %d %d %d\n",
	        ulcl.tm_year + TM_YEAR_BASE, ulcl.tm_mon+1, ulcl.tm_mday,
                ulcl.tm_hour, ulcl.tm_min, ulcl.tm_sec, tzname[ulcl.tm_isdst],
                ulcl.tm_yday, ulcl.tm_wday, ulcl.tm_isdst);
	tet_infoline(tmpstr);
#endif /* !defined(_AIX) && !defined(__hpux) */

	ulcltz = (ulcl.tm_isdst >= 0) ? tzname[ulcl.tm_isdst] : "???" ;

#ifdef SYSTEM_FIVE
	lcl = *localtime((time_t *)&t.tv_sec);
#if defined(_AIX) || defined(__hpux)
        ulcl1 = ulcl; /* don't pass ulcl to mktime() */
#endif /* #if defined(_AIX) || defined(__hpux) */
#ifdef NOV1974_BUG
        /* adjust daylight rule to conform to utc api rule  
         * if bug, daylight rule ended on last sunday in November, 74 instead
         * last sunday of October as utc rule
         */
        if ((lcl.tm_year == 74)&&(lcl.tm_yday <= 333)&&(lcl.tm_yday > 298)) { 
        lcl.tm_isdst = ulcl.tm_isdst;
	lcl.tm_mon   = ulcl.tm_mon;
        lcl.tm_mday  = ulcl.tm_mday;
        lcl.tm_hour  = ulcl.tm_hour;
        lcl.tm_wday  = ulcl.tm_wday;
        lcl.tm_yday  = ulcl.tm_yday;
        } 
#endif   /* NOV1974_BUG */
        if (ulcl.tm_sec != lcl.tm_sec ||
            ulcl.tm_min != lcl.tm_min ||
            ulcl.tm_hour != lcl.tm_hour ||
	    ulcl.tm_mday != lcl.tm_mday ||
	    ulcl.tm_mon != lcl.tm_mon ||
	    ulcl.tm_year != lcl.tm_year ||
	    ulcl.tm_yday != lcl.tm_yday ||
	    ulcl.tm_wday != lcl.tm_wday ||
#ifdef HAS_GMTOFF
	    ulcl.tm_gmtoff != lcl.tm_gmtoff ||
#endif
	    ulcl.tm_isdst != lcl.tm_isdst) {
#ifdef __hpux
            /* fix up wrong hpux 1974 dst end date */
            if (ulcl.tm_sec != lcl.tm_sec ||
                ulcl.tm_min != lcl.tm_min ||
                (ulcl.tm_hour + 1 != lcl.tm_hour &&
                (ulcl.tm_hour != 23 || lcl.tm_hour != 0)) ||
	        (ulcl.tm_mday != lcl.tm_mday &&
                (ulcl.tm_hour != 23 ||
	        (ulcl.tm_mday + 1 != lcl.tm_mday &&
	        (ulcl.tm_mday != 31 || lcl.tm_mday != 1 )))) ||
	        (ulcl.tm_mon != lcl.tm_mon &&
	        (ulcl.tm_mon != 9 || lcl.tm_mon != 10)) ||
	        (lcl.tm_mon != 9 && lcl.tm_mon != 10) ||
	        ulcl.tm_year != lcl.tm_year ||
	        ulcl.tm_year != 74 ||
	        (ulcl.tm_yday != lcl.tm_yday &&
	        ulcl.tm_yday + 1 != lcl.tm_yday) ||
	        (ulcl.tm_wday != lcl.tm_wday &&
	        ulcl.tm_wday + 1 != lcl.tm_wday &&
	        (ulcl.tm_wday != 6 || lcl.tm_wday != 0)) ||
	        ulcl.tm_isdst + 1 != lcl.tm_isdst || 
                lcl.tm_isdst != 1 ) {
	        tet_infoline("utc_localtime differs from localtime\n");
	        ok = 0;
	    }
	}
#else /* #ifdef __hpux */
	    tet_infoline("utc_localtime differs from localtime\n");
	    ok = 0;
	}
#endif /* #ifdef __hpux */

	lcltz = (lcl.tm_isdst >= 0) ? tzname[lcl.tm_isdst] : "???" ;

#if defined(_AIX) || defined(__hpux)
        t2.tv_sec = mktime(&ulcl1);
#else /* #if defined(_AIX) || defined(__hpux) */
        t2.tv_sec = mktime(&ulcl);
#endif /* #if defined(_AIX) || defined(__hpux) */
        if (t2.tv_sec == (time_t)-1) {
	    tet_infoline("mktime returned error\n");
	    ok = 0;
	}
	if (t2.tv_sec != t.tv_sec) {
	    tet_infoline("mktime returned different time from seed!\n");
	    ok = 0;
	}
#endif

	if (utc_mklocaltime(&utc2, &ulcl, tns, &im, ins) < 0) {
	    tet_infoline("utc_mklocaltime returned error\n");
	    ok = 0;
	}

	if (compare_time(&utc, &utc2, "utc_localtime, utc_mklocaltime") == 0)
	    ok = 0;

	if (compare_inaccuracy(&utc, &utc2, "utc_localtime, utc_mklocaltime") == 0)
	    ok = 0;

	if (utc_gmtime(&ugmt, &tns, &im, &ins, &utc) < 0) {
	    tet_infoline("utc_gmtime returned error\n");
	    ok = 0;
	}
    
	ugmttz = (ugmt.tm_isdst >= 0) ? tzname[ugmt.tm_isdst] : "???" ;

#ifdef unix
	gmt = *gmtime((time_t *)&t.tv_sec);
	if (ugmt.tm_sec != gmt.tm_sec ||
            ugmt.tm_min != gmt.tm_min ||
            ugmt.tm_hour != gmt.tm_hour ||
	    ugmt.tm_mday != gmt.tm_mday ||
	    ugmt.tm_mon != gmt.tm_mon ||
	    ugmt.tm_year != gmt.tm_year ||
	    ugmt.tm_yday != gmt.tm_yday ||
	    ugmt.tm_wday != gmt.tm_wday ||
#ifdef HAS_GMTOFF
	    ugmt.tm_gmtoff != gmt.tm_gmtoff ||
#endif
	    ugmt.tm_isdst != gmt.tm_isdst) {
	    tet_infoline("utc_gmtime differs from gmtime\n");
	    ok = 0;
	}

	gmttz = (gmt.tm_isdst >= 0) ? tzname[gmt.tm_isdst] : "???" ;
#endif

	if (utc_mkgmtime(&utc2, &ugmt, tns, &im, ins) < 0) {
	    tet_infoline("utc_mkgmtime returned error\n");
	    ok = 0;
	}

	if (compare_time(&utc, &utc2, "utc_gmtime, utc_mkgmtime") == 0)
	    ok = 0;

	if (compare_inaccuracy(&utc, &utc2, "utc_gmtime, utc_mkgmtime") == 0)
	    ok = 0;

	if (utc_anytime(&uany, &tns, &im, &ins, &tdf2, &utc) < 0) {
	    tet_infoline("utc_anytime returned error\n");
	    ok = 0;
	}

	uanytz = (uany.tm_isdst >= 0) ? tzname[uany.tm_isdst] : "???" ;

	if (utc_mkanytime(&utc2, &uany, tns, &im, ins, tdf2) < 0) {
	    tet_infoline("utc_mkanytime returned error\n");
	    ok = 0;
	}

	if (compare_time(&utc, &utc2, "utc_anytime, utc_mkanytime") == 0)
	    ok = 0;

	if (compare_inaccuracy(&utc, &utc2, "utc_anytime, utc_mkanytime") == 0)
	    ok = 0;

	if (compare_tdf(&utc, &utc2, "utc_anytime, utc_mkanytime") == 0)
	    ok = 0;

	if (utc_pointtime(&utcl, &utcm, &utch, &utc) < 0) {
	    tet_infoline("utc_pointtime returned error\n");
	    ok = 0;
	}

	if (compare_time(&utc, &utcm, "utc_pointtime - midpoint check") == 0)
	    ok = 0;

	if (utc_boundtime(&utc2, &utcl, &utch) < 0) {
	    tet_infoline("utc_boundtime returned error\n");
	    ok = 0;
	}

	if (compare_time(&utc, &utc2, "utc_pointtime, utc_boundtime") == 0)
	    ok = 0;

	if (compare_inaccuracy(&utc, &utc2, "utc_pointtime, utc_boundtime") == 0)
	    ok = 0;

	if (utc_spantime(&utc2, &utcl, &utch) < 0) {
	    tet_infoline("utc_spantime returned error\n");
	    ok = 0;
	}

	if (compare_time(&utc, &utc2, "utc_pointtime, utc_spantime") == 0)
	    ok = 0;

	if (compare_inaccuracy(&utc, &utc2, "utc_pointtime, utc_spantime") == 0)
	    ok = 0;

	/*
	 * utc_cmpmidtime TESTS
	 */

	if (utc_cmpmidtime(&relation, &utc, &utc) < 0) {
	    tet_infoline("utc_cmpmidtime returned error\n");
	    ok = 0;
	}

	if (relation != utc_equalTo) {
	    tet_infoline("utc_cmpmidtime did not return utc_equalTo, utc - utc\n");
	    ok = 0;
	}

	if (utc_cmpmidtime(&relation, &utc, &utcm) < 0) {
	    tet_infoline("utc_cmpmidtime returned error\n");
	    ok = 0;
	}

	if (relation != utc_equalTo) {
	    tet_infoline("utc_cmpmidtime did not return utc_equalTo, utc - utcm\n");
	    ok = 0;
	}

	if (utc_cmpmidtime(&relation, &utcm, &utc) < 0) {
	    tet_infoline("utc_cmpmidtime returned error\n");
	    ok = 0;
	}

	if (relation != utc_equalTo) {
	    tet_infoline("utc_cmpmidtime did not return utc_equalTo, utcm - utc\n");
	    ok = 0;
	}

	if (utc_cmpmidtime(&relation, &utc, &utcl) < 0) {
	    tet_infoline("utc_cmpmidtime returned error\n");
	    ok = 0;
	}

	if (relation != utc_greaterThan) {
	    tet_infoline("utc_cmpmidtime did not return utc_greaterThan, utc - utcl\n");
	    ok = 0;
	}

	if (utc_cmpmidtime(&relation, &utcl, &utc) < 0) {
	    tet_infoline("utc_cmpmidtime returned error\n");
	    ok = 0;
	}

	if (relation != utc_lessThan) {
	    tet_infoline("utc_cmpmidtime did not return utc_lessThan, utcl - utc\n");
	    ok = 0;
	}

	if (utc_cmpmidtime(&relation, &utc, &utch) < 0) {
	    tet_infoline("utc_cmpmidtime returned error\n");
	    ok = 0;
	}

	if (relation != utc_lessThan) {
	    tet_infoline("utc_cmpmidtime did not return utc_lessThan, utc - utch\n");
	    ok = 0;
	}

	if (utc_cmpmidtime(&relation, &utch, &utc) < 0) {
	    tet_infoline("utc_cmpmidtime returned error\n");
	    ok = 0;
	}

	if (relation != utc_greaterThan) {
	    tet_infoline("utc_cmpmidtime did not return utc_greaterThan, utch - utc\n");
	    ok = 0;
	}

	/*
	 * utc_cmpintervaltime TESTS
	 */

	if (utc_cmpintervaltime(&relation, &utcm, &utcm) < 0) {
	    tet_infoline("utc_cmpintervaltime returned error\n");
	    ok = 0;
	}

	if (relation != utc_equalTo) {
	    tet_infoline("utc_cmpintervaltime did not return utc_equalTo, utcm - utcm\n");
	    ok = 0;
	}

	if (utc_cmpintervaltime(&relation, &utcm, &utc) < 0) {
	    tet_infoline("utc_cmpintervaltime returned error\n");
	    ok = 0;
	}

	if (relation != utc_indeterminate) {
	    tet_infoline("utc_cmpintervaltime did not return utc_indeterminate, utcm - utc\n");
	    ok = 0;
	}

	if (utc_cmpintervaltime(&relation, &utc, &utcm) < 0) {
	    tet_infoline("utc_cmpintervaltime returned error\n");
	    ok = 0;
	}

	if (relation != utc_indeterminate) {
	    tet_infoline("utc_cmpintervaltime did not return utc_indeterminate, utc - utcm\n");
	    ok = 0;
	}

	if (utc_cmpintervaltime(&relation, &utcm, &utch) < 0) {
	    tet_infoline("utc_cmpintervaltime returned error\n");
	    ok = 0;
	}

	if (relation != utc_lessThan) {
	    tet_infoline("utc_cmpintervaltime did not return utc_lessThan, utcm - utch\n");
	    ok = 0;
	}

	if (utc_cmpintervaltime(&relation, &utch, &utcm) < 0) {
	    tet_infoline("utc_cmpintervaltime returned error\n");
	    ok = 0;
	}

	if (relation != utc_greaterThan) {
	    tet_infoline("utc_cmpintervaltime did not return utc_greaterThan, utch - utcm\n");
	    ok = 0;
	}

	if (utc_cmpintervaltime(&relation, &utcm, &utcl) < 0) {
	    tet_infoline("utc_cmpintervaltime returned error\n");
	    ok = 0;
	}

	if (relation != utc_greaterThan) {
	    tet_infoline("utc_cmpintervaltime did not return utc_greaterThan, utcm - utcl\n");
	    ok = 0;
	}

	if (utc_cmpintervaltime(&relation, &utcl, &utcm) < 0) {
	    tet_infoline("utc_cmpintervaltime returned error\n");
	    ok = 0;
	}

	if (relation != utc_lessThan) {
	    tet_infoline("utc_cmpintervaltime did not return utc_lessThan, utcl - utcm\n");
	    ok = 0;
	}

	if (utc_boundtime(&utclm, &utcl, &utcm) < 0) {
	    tet_infoline("utc_boundtime returned error\n");
	    ok = 0;
	}

	if (utc_cmpintervaltime(&relation, &utclm, &utch) < 0) {
	    tet_infoline("utc_cmpintervaltime returned error\n");
	    ok = 0;
	}

	if (relation != utc_lessThan) {
	    tet_infoline("utc_cmpintervaltime did not return utc_lessThan, utclm - utch\n");
	    ok = 0;
	}

	if (utc_cmpintervaltime(&relation, &utch, &utclm) < 0) {
	    tet_infoline("utc_cmpintervaltime returned error\n");
	    ok = 0;
	}

	if (relation != utc_greaterThan) {
	    tet_infoline("utc_cmpintervaltime did not return utc_greaterThan, utch - utclm\n");
	    ok = 0;
	}

	if (utc_boundtime(&utcmh, &utcm, &utch) < 0) {
	    tet_infoline("utc_boundtime returned error\n");
	    ok = 0;
	}

	if (utc_cmpintervaltime(&relation, &utclm, &utcmh) < 0) {
	    tet_infoline("utc_cmpintervaltime returned error\n");
	    ok = 0;
	}

	if (relation != utc_indeterminate) {
	    tet_infoline("utc_cmpintervaltime did not return utc_indeterminate, utclm - utcmh\n");
	    ok = 0;
	}

	if (utc_cmpintervaltime(&relation, &utcmh, &utclm) < 0) {
	    tet_infoline("utc_cmpintervaltime returned error\n");
	    ok = 0;
	}

	if (relation != utc_indeterminate) {
	    tet_infoline("utc_cmpintervaltime did not return utc_indeterminate, utcmh - utclm\n");
	    ok = 0;
	}

	/*
	 * relative TESTS
	 */

	if (utc_mkbinreltime(&relutc, &rt, &i) < 0) {
	    tet_infoline("utc_mkbinreltime returned error\n");
	    ok = 0;
	}

	if (utc_binreltime(&rt2, &i2, &relutc) < 0) {
	    tet_infoline("utc_binreltime returned error\n");
	    ok = 0;
	}

	if (rt.tv_sec != rt2.tv_sec || rt.tv_nsec != rt2.tv_nsec ||
	    i.tv_sec != i2.tv_sec || i.tv_nsec != i2.tv_nsec) {
	    tet_infoline("utc_mkbinreltime - utc_binreltime aren't inverses\n");
	    ok = 0;
	}

	if (utc_reltime(&urel, &tns, &im, &ins, &relutc) < 0) {
	    tet_infoline("utc_reltime returned error\n");
	    ok = 0;
	}

	if (utc_mkreltime(&relutc2, &urel, tns, &im, ins) < 0) {
	    tet_infoline("utc_mkreltime returned error'n");
	    ok = 0;
	}

	if (compare_rel_time(&relutc,
			     &relutc2,
			     "utc_reltime, utc_mkreltime") == 0)
	    ok = 0;

	if (compare_rel_inaccuracy(&relutc,
				   &relutc2,
				   "utc_reltime, utc_mkreltime") == 0)
	    ok = 0;

	/*
	 * Arithmatic TESTS
	 */

	if (utc_addtime(&relutc2, &relutc, &relutc) < 0) {
	    tet_infoline("utc_addtime returned error\n");
	    ok = 0;
	}

	if (utc_mulftime(&relutc2, &relutc2, (double)0.5) < 0 ) {
	    tet_infoline("utc_mulftime returned error\n");
	    ok = 0;
	}

	if (compare_rel_time(&relutc,
			     &relutc2,
			     "utc_addtime, utc_mulftime") == 0)
	    ok = 0;

	if (compare_rel_inaccuracy(&relutc,
				   &relutc2,
				   "utc_addtime, utc_mulftime") == 0)
	    ok = 0;

	if (utc_multime(&relutc3, &relutc, 3) < 0 ) {
	    tet_infoline("utc_multime returned error\n");
	    ok = 0;
	}

	if (utc_subtime(&relutc3, &relutc3, &relutc) < 0) {
	    tet_infoline("utc_subtime returned error\n");
	    ok = 0;
	}

	if (utc_subtime(&relutc3, &relutc3, &relutc) < 0) {
	    tet_infoline("utc_subtime returned error\n");
	    ok = 0;
	}

	if (compare_rel_time(&relutc,
			     &relutc3,
			     "utc_multime, utc_subtime") == 0)
	    ok = 0;

	if (utc_binreltime((reltimespec_t *)0, &i3, &relutc3) < 0) {
	    tet_infoline("utc_binreltime returned error\n");
	    ok = 0;
	}

	i2.tv_sec *= 5;
	x = i2.tv_nsec;
	i2.tv_nsec += x;
	if (i2.tv_nsec > 1000000000) {
	    i2.tv_sec++;
	    i2.tv_nsec -= 1000000000;
	}
	i2.tv_nsec += x;
	if (i2.tv_nsec > 1000000000) {
	    i2.tv_sec++;
	    i2.tv_nsec -= 1000000000;
	}
	i2.tv_nsec += x;
	if (i2.tv_nsec > 1000000000) {
	    i2.tv_sec++;
	    i2.tv_nsec -= 1000000000;
	}
	i2.tv_nsec += x;
	if (i2.tv_nsec > 1000000000) {
	    i2.tv_sec++;
	    i2.tv_nsec -= 1000000000;
	}
	    
	if (i2.tv_sec != i3.tv_sec || i2.tv_nsec != i3.tv_nsec) {
	    tet_infoline("Inaccuracy of 3 * X - X - X != 5 * Inaccuracy of X\n");
	    ok = 0;
	}

	/*
	 * Absolute value test
	 */

	if (utc_abstime(&relutc2, &relutc) < 0) {
	    tet_infoline("utc_abstime returned error\n");
	    ok = 0;
	}

	if (utc_binreltime(&art, &ari, &relutc) < 0) {
	    tet_infoline("utc_binreltime returned error\n");
	    ok = 0;
	}

	if (art.tv_sec < 0) {
	   art.tv_sec++;
	   art.tv_sec = labs(art.tv_sec);
	   art.tv_nsec = labs(art.tv_nsec - 1000000000);
	}

	if (utc_mkbinreltime(&relutc3, &art, &ari) < 0) {
	    tet_infoline("utc_mkbinreltime returned error\n");
	    ok = 0;
	}

	if (compare_rel_time(&relutc2,
			     &relutc3,
			     "utc_abstime") == 0)
	    ok = 0;

	if (compare_rel_inaccuracy(&relutc2,
				   &relutc3,
				   "utc_abstime") == 0)
	    ok = 0;

	/*
	 * Test below this point are rounded to a msec, since that's all
	 * the accuracy that the ASCII routines preserve.
	 */

	if (round_to_msec(&utc) == 0) {
	    ok = 0;
	}

	/*
	 * utc_ascgmtime TEST
	 */

	if (utc_ascgmtime(ustr, (size_t)100, &utc) < 0) {
	    tet_infoline("utc_ascgmtime returned error\n");
	    ok = 0;
	}

	if (utc_mkasctime(&utc2, ustr)) {
	    tet_infoline("utc_mkasctime returned error\n");
	    ok = 0;
	}

	if (compare_time(&utc, &utc2, "utc_ascgmtime, utc_mkasctime") == 0)
	    ok = 0;

	if (compare_inaccuracy(&utc, &utc2, "utc_ascgmtime, utc_mkasctime") == 0)
	    ok = 0;

	/*
	 * utc_asclocaltime TEST
	 */

	if (utc_asclocaltime(ustr, (size_t)100, &utc) < 0) {
	    tet_infoline("utc_asclocaltime returned error\n");
	    ok = 0;
	}

	if (utc_mkasctime(&utc2, ustr)) {
	    tet_infoline("utc_mkasctime returned error\n");
	    ok = 0;
	}

	if (compare_time(&utc, &utc2, "utc_asclocaltime, utc_mkasctime") == 0)
	    ok = 0;

	if (compare_inaccuracy(&utc, &utc2, "utc_asclocaltime, utc_mkasctime") == 0)
	    ok = 0;

	/*
	 * utc_ascanytime TEST
	 */

	if (utc_ascanytime(ustr, (size_t)100, &utc) < 0) {
	    tet_infoline("utc_ascanytime returned error\n");
	    ok = 0;
	}

	if (utc_mkasctime(&utc2, ustr)) {
	    tet_infoline("utc_mkasctime returned error\n");
	    ok = 0;
	}

	if (compare_time(&utc, &utc2, "utc_ascanytime, utc_mkasctime") == 0)
	    ok = 0;

	if (compare_inaccuracy(&utc, &utc2, "utc_ascanytime, utc_mkasctime") == 0)
	    ok = 0;

	if (compare_tdf(&utc, &utc2, "utc_ascanytime, utc_mkasctime") == 0)
	    ok = 0;

	if (round_rel_to_msec(&relutc) == 0) {
	    ok = 0;
	}

	if (utc_ascreltime(urelstr, (size_t)100, &relutc) < 0) {
	    tet_infoline("utc_ascreltime returned error\n");
	    ok = 0;
	}

	if (utc_mkascreltime(&relutc2, urelstr)) {
	    tet_infoline("utc_mkascreltime returned error\n");
	    ok = 0;
	}

	if (compare_rel_time(&relutc,
			     &relutc2,
			     "utc_ascreltime, utc_mkascreltime") == 0)
	    ok = 0;

	if (compare_rel_inaccuracy(&relutc,
				   &relutc2,
				   "utc_ascreltime, utc_mkascreltime") == 0)
	    ok = 0;

#if defined(_AIX) || defined(__hpux)
        ++count;
	if (count % 1000 == 0) {
	    sprintf(tmpstr,"completed %d iterations with no errors\n",count);
	    tet_infoline(tmpstr);
	}
#else /* #if defined(_AIX) || defined(__hpux) */
	sprintf(tmpstr,"count = %d\n", ++count);
	tet_infoline(tmpstr);
#endif /* #if defined(_AIX) || defined(__hpux) */


    }

#ifdef __hpux
    if (ok) {
        printf("PASSED, Test 1 'RANTEST_API' : test ran successfully\n");
	tet_result(TET_PASS);
    }
    else {
        printf("FAILED, Test 1 'RANTEST_API' : test ran unsuccessfully\n");
	tet_result(TET_FAIL);

#endif /* #ifdef __hpux */

	sprintf(tmpstr,"seed = %d = %x\n", t.tv_sec, t.tv_sec);
	tet_infoline(tmpstr);

	sprintf(tmpstr,"ulcl = %d/%d/%d %d:%02d:%02d %s %d %d %d\n",
		ulcl.tm_year + TM_YEAR_BASE, ulcl.tm_mon+1, ulcl.tm_mday,
		ulcl.tm_hour, ulcl.tm_min, ulcl.tm_sec, ulcltz,
		ulcl.tm_yday, ulcl.tm_wday, ulcl.tm_isdst);
	tet_infoline(tmpstr);

#ifdef unix
	sprintf(tmpstr,"lcl  = %d/%d/%d %d:%02d:%02d %s %d %d %d\n",
		lcl.tm_year + TM_YEAR_BASE, lcl.tm_mon+1, lcl.tm_mday,
		lcl.tm_hour, lcl.tm_min, lcl.tm_sec, lcltz,
		lcl.tm_yday, lcl.tm_wday, lcl.tm_isdst);
	tet_infoline(tmpstr);
#endif

	sprintf(tmpstr,"ugmt = %d/%d/%d %d:%02d:%02d %s %d %d %d\n",
		ugmt.tm_year + TM_YEAR_BASE, ugmt.tm_mon+1, ugmt.tm_mday,
		ugmt.tm_hour, ugmt.tm_min, ugmt.tm_sec, ugmttz,
		ugmt.tm_yday, ugmt.tm_wday, ugmt.tm_isdst);
	tet_infoline(tmpstr);

#ifdef unix
	sprintf(tmpstr,"gmt  = %d/%d/%d %d:%02d:%02d %s %d %d %d\n",
		gmt.tm_year + TM_YEAR_BASE, gmt.tm_mon+1, gmt.tm_mday,
		gmt.tm_hour, gmt.tm_min, gmt.tm_sec, gmttz,
		gmt.tm_yday, gmt.tm_wday, gmt.tm_isdst);
	tet_infoline(tmpstr);
#endif

	sprintf(tmpstr,"uany = %d/%d/%d %d:%02d:%02d %s %d %d %d\n",
		uany.tm_year + TM_YEAR_BASE, uany.tm_mon+1, uany.tm_mday,
		uany.tm_hour, uany.tm_min, uany.tm_sec, uanytz,
		uany.tm_yday, uany.tm_wday, uany.tm_isdst);
	tet_infoline(tmpstr);

#ifdef __hpux
    }
#endif /* #ifdef __hpux */

}
