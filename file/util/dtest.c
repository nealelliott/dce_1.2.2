/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtest.c,v $
 * Revision 1.1.11.1  1996/10/02  21:05:27  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:51:00  damon]
 *
 * Revision 1.1.6.2  1994/07/13  22:30:29  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  12:02:31  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:04:32  mbs]
 * 
 * Revision 1.1.6.1  1994/06/09  14:24:45  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:36:52  annie]
 * 
 * Revision 1.1.2.2  1993/01/21  16:31:15  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:24:52  cjd]
 * 
 * Revision 1.1  1992/01/19  02:45:36  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <ktime.h>

main(argc, argv)
int argc;
char **argv; {
    long code, temp;

    if (argc <= 1) {
	printf("dtest: usage is 'dtest <time to interpret>'\n");
	exit(0);
    }

    code = ktime_DateToLong(argv[1], &temp);
    if (code) {
	printf("date parse failed with code %d.\n", code);
    }
    else {
	printf("returned %d, which, run through ctime, yields %s", temp, ctime(&temp));
    }
    exit(0);
}
