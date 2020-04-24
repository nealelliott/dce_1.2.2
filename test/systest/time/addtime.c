/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: addtime.c,v $
 * Revision 1.1.7.2  1996/03/09  20:52:39  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:30:12  marty]
 *
 * Revision 1.1.7.1  1995/12/13  21:56:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:38  root]
 * 
 * Revision 1.1.7.2  1993/07/09  15:26:38  root
 * 	Initial King Kong branch
 * 	[1993/07/09  15:26:11  root]
 * 
 * Revision 1.1.5.2  1993/03/26  18:39:08  truitt
 * 	OSF says to defunct this program, but it is part of dcetmudt,
 * 	which we use for system testing.  So creating GAMERA branch.
 * 	[1993/03/26  18:37:44  truitt]
 * 
 * Revision 1.1.3.2  1992/06/11  20:04:45  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:11:13  mcfarland]
 * 
 * 	Fix utc_mkreltime parms
 * 	[1992/03/18  00:05:10  rps]
 * 
 * Revision 1.1.1.2  1992/05/28  15:18:10  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:37:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *   FILE_NAME: addtime.c
 *
 *   COMPONENT_NAME: dce.systest
 *
 *   FUNCTIONS: print_utc 
 *              
 *   ORIGINS: 27
 *
 *   (C) COPYRIGHT International Business Machines Corp. 1991
 *   All Rights Reserved
 *
 *   FOR_TEST: dcetmutc
 *
 *   USAGE: addtime
 *
 *   DESCRIPTION:
 *	This program will return current time plus one minute
 *      
 *   HISTORY: 12/7/90     Initial code
 */

#include <sys/time.h>
#include <time.h>
#include <dce/utc.h>
#include <errno.h>

print_utc(u)
struct utc	*u;
{
	struct tm	time, inacc;
	long		tns, ins, temp;

	if ( utc_localtime(&time, &tns, &inacc, &ins, u) < 0 )
	{
		xx_error("addtime: utc_localtime returned error");
		exit(1);
	}
	printf("%d-%02d-%02d-%02d:%02d:%02d\n",
		time.tm_year+1900, time.tm_mon+1, time.tm_mday,
		time.tm_hour, time.tm_min, time.tm_sec, tns);
}

main()
{
	struct utc	u1, u2, u3;
	struct tm	minute, im;
	long		tns, ins;
	
	cma_init();
	
	/* Define Jan. 1, 1970, 1minute  00:01:00 */
	minute.tm_sec = 0;
	minute.tm_min = 1;
	minute.tm_hour = 0;
	minute.tm_mday = 1;
	minute.tm_mon = 0;
	minute.tm_year = 70;
	minute.tm_wday = 0;
	minute.tm_yday = 0;	

	/* Convert one minute into binary utc form */
 	if ( utc_mkreltime(&u1, &minute, tns, &im, ins) < 0 )
	{
		xx_error("addtime: utc_mkreltime returned error");
		exit(1);
	}

	/* Get the current time */
	if ( utc_gettime(&u2) < 0 )
	{
		xx_error("addtime: utc_gettime returned error");
		exit(1);
	}

	/* Add current time plus 1 minute */
	if ( utc_addtime(&u3, &u1, &u2) < 0 )	
	{
		xx_error("addtime: utc_addtime returned error");
		exit(1);
	}

	/* Print the result to return the data to the shell program */
	print_utc(&u3);
	return(0);
}
