/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: test_kernel.c,v $
 * Revision 1.1.8.2  1996/02/17  23:32:27  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:48  marty]
 *
 * Revision 1.1.8.1  1995/12/11  23:05:38  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/07/25  15:19 UTC  pare
 * 	Change tet/tet_api.h to tet_api.h
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/30  18:29 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	HP revision /main/HPDCE01/2  1994/03/28  20:24 UTC  pare
 * 
 * 	HP revision /main/HPDCE01/pare_rodan/1  1994/03/28  20:23 UTC  pare
 * 	tet api is now in external tree
 * 
 * 	HP revision /main/HPDCE01/1  1994/02/01  14:38  pare
 * 	merge kk and hpdce01
 * 	[1995/12/11  22:37:09  root]
 * 
 * Revision 1.1.5.2  1993/07/09  17:15:36  root
 * 	Initial King Kong branch
 * 	[1993/07/09  17:14:54  root]
 * 
 * Revision 1.1.3.3  1993/04/16  18:47:38  truitt
 * 	Put under TET.
 * 	[1993/04/16  18:47:19  truitt]
 * 
 * 	Embedded copyright notice
 * 	[1992/12/31  20:28:34  htf]
 * 
 * Revision 1.1.3.2  1993/04/12  19:14:20  truitt
 * 	Enhanced error reporting.  Identified and got rid of an
 * 	abnormal utc_adjtime() failure.  Issues "PASS" or "FAIL"
 * 	message at the end of the test for quick result gathering.
 * 	[1993/04/12  19:09:07  truitt]
 * 
 * 	Fixing bug 2075 regarding test/time/common syntax errors on RIOS.
 * 	The problem had to do with includes of headers to get struct
 * 	timespec, struct timeval, and utc_t all defined correctly for the
 * 	file.  The comment in the file explains all.  Basically on OSF/1
 * 	and AIX just include <sys/time.h>, #define _TIMESPEC_T_, and
 * 	include <utctime.h>.  This might have a doc impact I think it should.
 * 	Had to also include <time.h> to get struct tm on a RIOS.  In OSF/1
 * 	this is included by <sys/time.h> but not for non-kernel code on
 * 	AIX.  I include it explicitly, on OSF/1 it's re-inclusion is idempotent.
 * 	[1992/03/03  23:19:53  melman]
 * 
 * 	Added OSF/1 and generic includes to avoid timespec redefine and to pickup define for timeval
 * 	[1992/02/06  21:36:34  eperkins]
 * 
 * Revision 1.1.1.7  1993/03/11  22:23:22  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1  1992/01/19  14:38:14  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <errno.h>
#include <pthread.h>

/* 
 * Need to get struct timeval and struct timespec defined here.  Can't 
 * just include utc.h since that will redefine utc_t and we want it's 
 * definition from utctime.h (yes they are different).  #define 
 * _TIMESPEC_T_ so that struct timespec is not redefined in utctime.h. 
 * I also include <time.h> so that on AIX struct tm is defined.
 * On OSF/1 <time.h> is included by  <sys/time.h> and is idempotent
 */
#include <sys/time.h>
#include <time.h>
#define _TIMESPEC_T_ /* avoid redefine of timespec */
#include <utctime.h>
#include <tet_api.h>

void (*tet_startup)() = NULL;
void (*tet_cleanup)() = NULL;
void test_kernel();

struct tet_testlist tet_testlist[] =
   {{test_kernel, 1},
    {NULL, NULL}};

extern char *tzname[2];

void print_timeval(t)

struct timeval *t;

{
    struct tm	*px;
    time_t      temp;
    char	tmpstr[100];

    temp = t->tv_sec;
    px = gmtime(&temp);
    (void) sprintf(tmpstr,"%d-%02d-%02d %02d:%02d:%02d.%06d\n", px->tm_year+1900,
           px->tm_mon+1, px->tm_mday, px->tm_hour, px->tm_min, px->tm_sec,
           t->tv_usec);
    tet_infoline(tmpstr);

}

void print_utc(u)

utc_t *u;

{
    struct tm	tmtime, tminacc;
    long        tns, ins;
    int		i;
    int		tdf;
    char 	zonename[80], tmpstr[100];

/*  long temp;

    temp = (unsigned int) u->chars[0] + ((unsigned int) u->chars[1] << 8) +
             ((unsigned int) u->chars[2] << 16) +
	     ((unsigned int) u->chars[3] << 24);
    (void) printf("%08x ", temp);
    temp = (unsigned int) u->chars[4] + ((unsigned int) u->chars[5] << 8) +
           ((unsigned int) u->chars[6] << 16) +
           ((unsigned int) u->chars[7] << 24);
    (void) printf("%08x ", temp);
    temp = (unsigned int) u->chars[8] + ((unsigned int) u->chars[9] << 8) +
           ((unsigned int) u->chars[10] << 16) +
	   ((unsigned int) u->chars[11] << 24);
    (void) printf("%08x ", temp);
    temp = (unsigned int) u->chars[12] + ((unsigned int) u->chars[13] << 8) +
           ((unsigned int) u->chars[14] << 16) +
	   ((unsigned int) u->chars[15] << 24);
    (void) printf("%08x\n", temp); */

    i = utc_gmtime(&tmtime, &tns, &tminacc, &ins, u);
    (void) sprintf(tmpstr,"%d-%02d-%02d %02d:%02d:%02d.%09d +/- ",
		   tmtime.tm_year+1900, tmtime.tm_mon+1, tmtime.tm_mday,
		   tmtime.tm_hour, tmtime.tm_min, tmtime.tm_sec, tns);
    tet_infoline(tmpstr);
    if (tminacc.tm_yday >= 0) {
	(void) sprintf(tmpstr,"%d %02d:%02d:%02d.%09d", tminacc.tm_yday,
		       tminacc.tm_hour, tminacc.tm_min, tminacc.tm_sec, ins);
	tet_infoline(tmpstr);
    }
    else
	tet_infoline("Infinite");

    if (i < 0 || tmtime.tm_isdst < 0)
	tet_infoline(" *Bad UTC struct*\n");
    else {
	(void) sprintf(tmpstr," (%s)\n", tzname[tmtime.tm_isdst]);
	tet_infoline(tmpstr);
    }

    i = utc_anytime(&tmtime, &tns, &tminacc, &ins, &tdf, u);

    if (tdf) {
	(void) sprintf(tmpstr,"%d-%02d-%02d %02d:%02d:%02d.%09d +/- ",
		       tmtime.tm_year+1900, tmtime.tm_mon+1, tmtime.tm_mday,
		       tmtime.tm_hour, tmtime.tm_min, tmtime.tm_sec, tns);
	tet_infoline(tmpstr);
	if (tminacc.tm_yday >= 0) {
	    (void) sprintf(tmpstr,"%d %02d:%02d:%02d.%09d", tminacc.tm_yday,
			   tminacc.tm_hour, tminacc.tm_min, tminacc.tm_sec, ins);
	    tet_infoline(tmpstr);
	}
	else
	    tet_infoline("Infinite");

	if (i >= 0) {
	    if (utc_anyzone(zonename, sizeof(zonename), (int *)NULL,
		            (int *)NULL, u) < 0)
		tet_infoline(" *Bad UTC struct*\n");
	    else {
		(void) sprintf(tmpstr," (%s = %d)\n", zonename, tdf);
		tet_infoline(tmpstr);
	    }
	}
    }

}

void test_kernel()
{
    struct timeval	t0, t1;
    struct timespec	ts0, ts1;
    struct reltimespec  t_one;
    utc_t		u0, u1, u2;
    union adjunion	argblk;
    struct int64	zero, small;
    int                 i, j, failure;
    char		tmpstr[100];

#if _DECTHREADS_
    /*
     * This really shouldn't be necessary, but there is a bug in
     * CMA, that cma_global_lock is not self initializing.
     */
    cma_init();
#endif /* _DECTHREADS_ */

/*    for (i = 0; i < 16; i++)
	u0.chars[i] = u1.chars[i] = (char) 0xAA; */

    failure = 0;

    /* Get and print resolution */

    i = utc_adjtime(getresolution, &argblk);
    if (i) {
        (void) sprintf(tmpstr,"utc_adjtime returned: %d\terrno: %d\t(resolution call)\n", i, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    (void) sprintf(tmpstr,"resolution = %d nanosecs\n", argblk.resolution);
    tet_infoline(tmpstr);

    /* Get and print drift */

    i = utc_adjtime(getdrift, &argblk);
    if (i) {
        (void) sprintf(tmpstr,"utc_adjtime returned: %d\terrno: %d\t(max drift call)\n", i, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    (void) sprintf(tmpstr,"drift = 1/%d\n", argblk.maxdrift);
    tet_infoline(tmpstr);
    
    /* Get and print frequency */

    i = utc_adjtime(getfreq, &argblk);
    if (i) {
        (void) sprintf(tmpstr,"utc_adjtime returned: %d\terrno: %d\t(frequency call)\n", i, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    (void) sprintf(tmpstr,"frequency = %d nanosec / sec\n", argblk.frequency);
    tet_infoline(tmpstr);
    
    /* Get and print time of day */

    (void) gettimeofday(&t0, (struct timezone *)0);
    print_timeval(&t0);

    /* Get UTC time twice and print */

    i = utc_gettime(&u0);
    j = utc_gettime(&u1);
    if (i) {
	(void) sprintf(tmpstr,"utc_gettime returned: %d\terrno: %d\n", i, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    print_utc(&u0);
    if (j) {
	(void) sprintf(tmpstr,"utc_gettime returned: %d\terrno: %d\n", j, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    print_utc(&u1);

    /* Get UTC time with USER tdf */
    i = utc_getusertime(&u0);
    if (i) {
	(void) sprintf(tmpstr,"utc_getusertime returned: %d\terrno: %d\n", i, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    print_utc(&u0);

    t_one.tv_sec = 1;
    t_one.tv_nsec = 0;
    zero.hi = 0;
    zero.lo = 0;
    argblk.adjargs.a_adjustment = t_one;
    argblk.adjargs.a_adjrate = 100;
    argblk.adjargs.a_baseinacc = zero;

    (void) gettimeofday(&t0, (struct timezone *)NULL);

    ts0.tv_sec = t0.tv_sec;
    ts0.tv_nsec = t0.tv_usec * 1000;
    argblk.adjargs.a_comptime = ts0;
    ts1.tv_sec = t0.tv_sec + 5;
    ts1.tv_nsec = t0.tv_usec * 1000;
    if (i = utc_mkbintime(&u0, &ts1, &zero, 0)) {
	(void) sprintf(tmpstr,"utc_mkbintime returned: %d\n", i);
	tet_infoline(tmpstr);
	failure = 1;
    }
    argblk.adjargs.a_leaptime = *( (struct int64 *) &u0);
    argblk.adjargs.a_curtdf = -300;
    argblk.adjargs.a_nextdf = -240;
    argblk.adjargs.a_tdftime = 0;

    if (i = utc_adjtime(settime, &argblk)) {
        (void) sprintf(tmpstr,"utc_adjtime returned: %d\terrno: %d\n", i, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }

    (void) gettimeofday(&t1, (struct timezone *)NULL);

    j = utc_gettime(&u1);
    print_timeval(&t0);
    tet_infoline("Setting time forward 1 second\n");
    tet_infoline("Leap second set to: ");
    print_utc(&u0);
    if (j) {
	(void) sprintf(tmpstr,"utc_gettime returned: %d\terrno: %d\n", j, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    print_timeval(&t1);
    print_utc(&u1);

    (void) sleep(3);

    (void) gettimeofday(&t0, (struct timezone *)NULL);
    i = utc_gettime(&u0);
    print_timeval(&t0);
    if (i) {
	(void) sprintf(tmpstr,"utc_gettime returned: %d\terrno: %d\n", i, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    print_utc(&u0);

    (void) sleep(3);

    (void) gettimeofday(&t0, (struct timezone *)NULL);
    i = utc_gettime(&u0);
    print_timeval(&t0);
    if (i) {
	(void) sprintf(tmpstr,"utc_gettime returned: %d\terrno: %d\n", i, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    print_utc(&u0);

    t_one.tv_sec = 0;
    t_one.tv_nsec = -100000000;
    small.hi = 0;
    small.lo = 1000000;
    argblk.adjargs.a_adjustment = t_one;
    argblk.adjargs.a_adjrate = 100;
    argblk.adjargs.a_baseinacc = small;

    (void) gettimeofday(&t0, (struct timezone *)NULL);

    ts0.tv_sec = t0.tv_sec;
    ts0.tv_nsec = t0.tv_usec * 1000;
    argblk.adjargs.a_comptime = ts0;
    ts1.tv_sec = t0.tv_sec + 5;
    ts1.tv_nsec = t0.tv_usec * 1000;
    if (i = utc_mkbintime(&u0, &ts1, &zero, NULL)) {
	(void) sprintf(tmpstr,"utc_mkbintime returned: %d\n", i);
	tet_infoline(tmpstr);
	failure = 1;
    }
    argblk.adjargs.a_leaptime = *( (struct int64 *) &u0);

    i = utc_adjtime(adjusttime, &argblk);

    (void) gettimeofday(&t1, (struct timezone *)NULL);

    j = utc_gettime(&u1);
    print_timeval(&t0);
    tet_infoline("Adjusting time backwards 0.1 second\n");
    tet_infoline("Leap second set to: ");
    print_utc(&u0);
    if (i) {
        (void) sprintf(tmpstr,"utc_adjtime returned: %d\terrno: %d\n", i, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    if (j) {
	(void) sprintf(tmpstr,"utc_gettime returned: %d\terrno: %d\n", j, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    print_timeval(&t1);
    print_utc(&u1);

    (void) sleep(3);

    (void) gettimeofday(&t0, (struct timezone *)NULL);
    i = utc_gettime(&u0);
    print_timeval(&t0);
    if (i) {
	(void) sprintf(tmpstr,"utc_gettime returned: %d\terrno: %d\n", i, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    print_utc(&u0);

    (void) sleep(3);

    (void) gettimeofday(&t0, (struct timezone *)NULL);
    i = utc_gettime(&u0);
    print_timeval(&t0);
    if (i) {
	(void) sprintf(tmpstr,"utc_gettime returned: %d\terrno: %d\n", i, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    print_utc(&u0);

    tet_infoline("Ending adjustment prematurely\n");
    if (i = utc_adjtime(endadjust, &argblk)) {
        (void) sprintf(tmpstr,"utc_adjtime returned: %d\terrno: %d\n", i, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }

    (void) gettimeofday(&t0, (struct timezone *)NULL);
    i = utc_gettime(&u0);
    print_timeval(&t0);
    if (i) {
	(void) sprintf(tmpstr,"utc_gettime returned: %d\terrno: %d\n", i, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    print_utc(&u0);

    (void) gettimeofday(&t0, (struct timezone *)NULL);
    i = utc_gettime(&u0);
    print_timeval(&t0);
    if (i) {
	(void) sprintf(tmpstr,"utc_gettime returned: %d\terrno: %d\n", i, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    print_utc(&u0);

    t_one.tv_sec = 0;
    t_one.tv_nsec = -100000000;
    small.hi = 0;
    small.lo = 1000000;
    argblk.adjargs.a_adjustment = t_one;
    argblk.adjargs.a_adjrate = 100;
    argblk.adjargs.a_baseinacc = small;

    (void) gettimeofday(&t0, (struct timezone *)NULL);

    ts0.tv_sec = t0.tv_sec;
    ts0.tv_nsec = t0.tv_usec * 1000;
    argblk.adjargs.a_comptime = ts0;
    ts1.tv_sec = t0.tv_sec + 5;
    ts1.tv_nsec = t0.tv_usec * 1000;
    if (i = utc_mkbintime(&u0, &ts1, &zero, 0)) {
	(void) sprintf(tmpstr,"utc_mkbintime returned: %d\n", i);
	tet_infoline(tmpstr);
	failure = 1;
    }
    argblk.adjargs.a_leaptime = *( (struct int64 *) &u0);
    argblk.adjargs.a_curtdf = -300;
    argblk.adjargs.a_nextdf = -240;
    ts1.tv_sec = t0.tv_sec + 10;
    ts1.tv_nsec = 0;
    if (i = utc_mkbintime(&u2, &ts1, &zero, 0)) {
	(void) sprintf(tmpstr,"utc_mkbintime returned: %d\n", i);
	tet_infoline(tmpstr);
	failure = 1;
    }
    argblk.adjargs.a_tdftime = ts1.tv_sec;

    i = utc_adjtime(adjusttime, &argblk);

    (void) gettimeofday(&t1, (struct timezone *)NULL);

    j = utc_gettime(&u1);
    print_timeval(&t0);
    tet_infoline("Adjusting time backwards 0.1 second\n");
    tet_infoline("Leap second set to: ");
    print_utc(&u0);
    tet_infoline("TDF change set to: ");
    print_utc(&u2);
    if (i) {
        (void) sprintf(tmpstr,"utc_adjtime returned: %d\terrno: %d\n", i, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    if (j) {
	(void) sprintf(tmpstr,"utc_gettime returned: %d\terrno: %d\n", j, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    print_timeval(&t1);
    print_utc(&u1);

    (void) sleep(120);

    (void) tet_infoline("Adjustment should have completed.\n");

    (void) gettimeofday(&t0, (struct timezone *)NULL);
    i = utc_gettime(&u0);
    print_timeval(&t0);
    if (i) {
	(void) sprintf(tmpstr,"utc_gettime returned: %d\terrno: %d\n", i, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    print_utc(&u0);

    (void) gettimeofday(&t0, (struct timezone *)NULL);
    i = utc_gettime(&u0);
    print_timeval(&t0);
    if (i) {
	(void) sprintf(tmpstr,"utc_gettime returned: %d\terrno: %d\n", i, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    print_utc(&u0);

    tet_infoline("Ending adjustment again\n");
    if (i = utc_adjtime(endadjust, &argblk)) {
        (void) sprintf(tmpstr,"utc_adjtime returned: %d\terrno: %d\n", i, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }

    (void) gettimeofday(&t0, (struct timezone *)NULL);
    i = utc_gettime(&u0);
    print_timeval(&t0);
    if (i) {
	(void) sprintf(tmpstr,"utc_gettime returned: %d\terrno: %d\n", i, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    print_utc(&u0);

    (void) gettimeofday(&t0, (struct timezone *)NULL);
    i = utc_gettime(&u0);
    print_timeval(&t0);
    if (i) {
	(void) sprintf(tmpstr,"utc_gettime returned: %d\terrno: %d\n", i, errno);
	tet_infoline(tmpstr);
	failure = 1;
    }
    print_utc(&u0);

    if (failure == 0) {
	printf("PASSED, Test 1 'TEST_KERNEL' : test ran successfully\n");
	tet_result(TET_PASS);
    }
    else {
	printf("FAILED, Test 1 'TEST_KERNEL' : test ran unsuccessfully\n");
	tet_result(TET_FAIL);
    }
}
