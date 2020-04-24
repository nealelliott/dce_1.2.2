/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: test_zonecvt.c,v $
 * Revision 1.1.6.2  1996/02/17  23:32:28  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:49  marty]
 *
 * 	Initial King Kong branch
 * 	[1993/07/09  17:16:48  root]
 *
 * Revision 1.1.6.1  1995/12/11  23:05:40  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/07/25  15:19 UTC  pare
 * 	Change tet/tet_api.h to tet_api.h
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/30  18:32 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	HP revision /main/HPDCE01/2  1994/03/28  20:30 UTC  pare
 * 
 * 	HP revision /main/HPDCE01/pare_rodan/1  1994/03/28  20:28 UTC  pare
 * 	tet api is now in external tree
 * 
 * 	HP revision /main/HPDCE01/1  1994/02/01  14:40  pare
 * 	merge kk and hpdce01
 * 	[1995/12/11  22:37:11  root]
 * 
 * 	Put under TET.
 * 	[1993/04/16  17:40:02  truitt]
 * 
 * 	Modified test output format.
 * 	[1993/04/09  14:03:20  truitt]
 * 
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/18  13:59:11  truitt]
 * 
 * Revision 1.1.1.7  1993/03/11  22:23:24  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.3.2  1992/11/24  15:55:41  smythe
 * 	changed TZ to DTS_TZ within __hpux to avoid collision with hpux TZ
 * 	added continue test to eat # header lines in the data file
 * 	[1992/11/24  15:51:53  smythe]
 * 
 * Revision 1.1.2.3  1993/02/04  22:10:14  tom
 * 	Update output for readability.  Report total number of failures.
 * 	Don't stop on failure.
 * 	[1993/02/04  21:44:01  tom]
 * 
 * Revision 1.1.2.2  1992/12/31  20:35:35  htf
 * 	Embedded copyright notice
 * 	[1992/12/31  20:28:42  htf]
 * 
 * Revision 1.1.1.6  1992/09/21  18:48:17  osfrcs
 * 	    Loading drop Dce1_0_1fnl
 * 
 * Revision 1.1  1992/01/19  14:38:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Copyright (c) 1991 by
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
 *		It tests time conversions in various timezones.
 *
 * ENVIRONMENT:	VAX/VMS and Ultrix
 *
 * AUTHORS:
 *		Joe Comuzzi
 *		Distributed Systems Services (DSS) 
 * 
 * USAGE:
 *		Compile this file and link it with the API. This program
 *		will test several cases. The program will stop, displaying
 *		information if a failure is found.
 */

#include <dce/utc.h>
#include <stdio.h>
#include <tet_api.h>

void (*tet_startup)() = NULL;
void (*tet_cleanup)() = NULL;
void zonecvt();

struct tet_testlist tet_testlist[] =
   {{zonecvt, 1},
    {NULL, NULL}};

#ifdef vms
#pragma nostandard       /* VMS only -  non port */
noshare                  /* make this storage class noshr for tzname */
#pragma standard         /* Continue checking for portability */

extern char *tzname[];
#endif

void zonecvt()
{
    char 	timezonename[100], localzonename[100], anyzonename[100];
    char	timestr[100], localtimestr[100], anytimestr[100];
    char	teststr[300], standard[300], tmpstr[100];
    long	localtdf, anytdf;
    int		localisdst, anyisdst;
    utc_t	utc;
    int		ok, testing, failed = 0, count = 0;

    testing = 1;
    ok = 1;

    /*if (argc > 1) {
	if (strcmp(argv[1], "-b") == 0)
	    testing = 0;
    }*/

    while(ok) {

	if (gets(standard) == NULL) {
	    tet_infoline("****************************************");
	    tet_infoline("****************************************\n");
	    sprintf(tmpstr,"\nNumber of tests which FAILED: %d\n", failed);
	    tet_infoline(tmpstr);
            if (failed == 0) {
               printf("PASSED, Test 1 'TEST_ZONECVT' : test ran successfully\n");
               tet_result(TET_PASS);
            }
            else {
               printf("FAILED, Test 1 'TEST_ZONECVT' : test ran unsuccessfully\n");
               tet_result(TET_FAIL);
            }
	    return;
	}

        if (standard[0] == '#')
            continue;

	if (sscanf(standard, "%s %s", timezonename, timestr) != 2) {
	    tet_infoline("ERROR: Incorrect syntax in test file\n");
	    return;
	}

	tet_infoline("****************************************");
	tet_infoline("****************************************\n\n");

	sprintf(tmpstr,"TESTCASE: %s\n", standard);
	tet_infoline(tmpstr);
	count++;

	if (utc_mkasctime(&utc, timestr) < 0) {
	    tet_infoline("ERROR: utc_mkasctime returned error\n");
	    ok = 0;
	}

	/* Set our timezone to timezonename */
#ifdef __hpux
	setenv("DTS_TZ", timezonename, 1);
#else /* __hpux */
	setenv("TZ", timezonename, 1);
#endif /* __hpux*/

	tzset();

	if (utc_asclocaltime(localtimestr, (size_t)100, &utc) < 0) {
	    tet_infoline("ERROR: utc_asclocaltime returned error\n");
	    ok = 0;
	}

	if (utc_localzone(localzonename, (size_t)100, &localtdf, &localisdst,
                          &utc) < 0) {
	    tet_infoline("ERROR: utc_localzone returned error\n");
	    ok = 0;
	}

	if (localisdst != 0 && localisdst != 1) {
	    tet_infoline("ERROR: utc_localzone returned invalid isdst\n");
	    localisdst = 0;
	    ok = 0;
	}

	sprintf(teststr, "%s (%s) %s", localtimestr, localzonename,
		(localisdst ? "Daylight Savings Time" : "Standard Time") );

	if (testing) {

	    gets(standard);

	    if (strcmp(teststr, standard) != 0) {
		sprintf(tmpstr,"ACTUAL:   %s\n", teststr);
		tet_infoline(tmpstr);
		sprintf(tmpstr,"EXPECTED: %s\n", standard);
		tet_infoline(tmpstr);
		ok = 0;
	    }
	    else {
		sprintf(tmpstr,"ACTUAL:   %s\n", teststr);
		tet_infoline(tmpstr);
		sprintf(tmpstr,"EXPECTED: %s\n", standard);
		tet_infoline(tmpstr);
		tet_infoline("PART ONE PASS\n");
	    }

	}

	if (utc_ascanytime(anytimestr, (size_t)100, &utc) < 0) {
	    tet_infoline("ERROR   : utc_ascanytime returned error\n");
	    ok = 0;
	}

	if (utc_anyzone(anyzonename, (size_t)100, &anytdf, &anyisdst,
                        &utc) < 0) {
	    tet_infoline("ERROR   : utc_anyzone returned error\n");
	    ok = 0;
	}

	if (anyisdst != -1) {
	    tet_infoline("ERROR   : utc_anyzone returned invalid isdst\n");
	    ok = 0;
	}

	sprintf(teststr, "%s (%s)", anytimestr, anyzonename);

	if (testing) {

	    gets(standard);

	    if (strcmp(teststr, standard) != 0) {
		sprintf(tmpstr,"ACTUAL:   %s\n", teststr);
		tet_infoline(tmpstr);
		sprintf(tmpstr,"EXPECTED: %s\n", standard);
		tet_infoline(tmpstr);
		tet_infoline("PART TWO FAIL\n");
		ok = 0;
	    }
	    else {
		sprintf(tmpstr,"ACTUAL:   %s\n", teststr);
		tet_infoline(tmpstr);
		sprintf(tmpstr,"EXPECTED: %s\n", standard);
		tet_infoline(tmpstr);
		tet_infoline("PART TWO PASS\n");
	    }

	}

	if (ok == 1) {
	    sprintf(tmpstr,"TESTCASE %d RESULT: PASSED\n\n",count);
	    tet_infoline(tmpstr);
	}

	if (ok == 0) {
	    sprintf(tmpstr,"TESTCASE %d RESULT: FAILED\n\n",count);
	    tet_infoline(tmpstr);
	    failed++;
	    ok = 1;
	}
    }
    return;
}
