/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: subtime.c,v $
 * Revision 1.1.6.2  1996/03/09  20:52:42  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:30:14  marty]
 *
 * 	Initial King Kong branch
 * 	[1993/07/09  15:35:11  root]
 *
 * Revision 1.1.6.1  1995/12/13  21:56:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:40  root]
 * 
 * 	OSF says to defunct this programs, but it is part of the
 * 	dcetmudt system test, which we use.  Creating GAMERA branch.
 * 	[1993/03/26  18:38:32  truitt]
 * 
 * Revision 1.1.3.2  1992/06/11  20:05:35  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:12:02  mcfarland]
 * 
 * 	new systest changes for time
 * 	[1992/05/08  15:30:32  bmw]
 * 
 * 	Fix utc_mkreltime parms
 * 	[1992/03/18  00:05:21  rps]
 * 
 * 	Adding changes from defect_fixes archive.
 * 	[1992/01/22  22:32:45  melman]
 * 
 * Revision 1.1.1.2  1992/05/28  15:18:27  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * $EndLog$
 */
/*
 *   FILE_NAME: subtime.c
 *
 *   COMPONENT_NAME: dce.systest
 *
 *   FUNCTIONS: print_utc
 *
 *   ORIGINS: 27
 *
 *
 *   (C) COPYRIGHT International Business Machines Corp. 1991
 *   All Rights Reserved
 *
 *
 *   FOR_TEST: dcetmutc
 *
 *   DESCRIPTION: This program will subtract start time (input) from 
 *   current time and return the difference
 *
 *   Usage: subtime <year month day hour min sec>
 *
 *   HISTORY: 12/07/90   Leslie Flake     Initial code
 */

#include <sys/time.h>
#include <time.h>
#include <dce/utc.h>
#include <errno.h>

print_utc(u)
struct utc	*u;
{
	struct tm	time, inacc;
	long		tns, ins, tdf;

	/* Convert utc to tm */
	if ( utc_anytime(&time, &tns, &inacc, &ins, &tdf, u) < 0 )
	{
		xx_error("subtime: utc_anytime returned error");
		exit(1);
	}

	/* Print the resulting time difference (only hour, minute and second) */
	printf("%02d:%02d:%02d\n", time.tm_hour, time.tm_min, time.tm_sec);
}

main(argc, argv)
int argc;
char **argv;
{
	/* Pass in the start time */
	struct utc	u1, u2, u3;
	struct tm	starttime, im;
	long		tns, ins;
	
	cma_init();

	/* Get the current time */
	if ( utc_gettime(&u1) < 0 )
	{
		xx_error("subtime: utc_gettime returned error");
		exit(1);
	}

	starttime.tm_year = atoi(argv[1]);		
	starttime.tm_mon = atoi(argv[2]);	
	starttime.tm_mday = atoi(argv[3]);	
	starttime.tm_hour = atoi(argv[4]);	
	starttime.tm_min = atoi(argv[5]);	
	starttime.tm_sec = atoi(argv[6]);	
	starttime.tm_wday = 0;	
	starttime.tm_yday = 0; 

	/* Convert the tm "starttime" to utc "u2" */
	if ( utc_mkreltime(&u2, &starttime, tns, &im, ins) < 0 )
	{
		xx_error("subtime: utc_mkreltime returned error");
		exit(1);
	}

	/* Subtract the starttime from the current time */
	if ( utc_subtime(&u3, &u1, &u2) < 0 )
	{
		xx_error("subtime: utc_subtime returned error");
		exit(1);
	}

	/* Print the difference */ 
	print_utc(&u3);
	return(0);
}
