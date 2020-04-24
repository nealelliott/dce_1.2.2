/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
*/
/*
 * HISTORY
 * $Log: calcLogSize.c,v $
 * Revision 1.1.45.1  1996/10/02  17:16:54  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:26:49  damon]
 *
 * Revision 1.1.39.2  1994/06/09  13:57:11  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:32:08  annie]
 * 
 * Revision 1.1.39.1  1994/02/04  20:10:56  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:08:56  devsrc]
 * 
 * Revision 1.1.37.1  1993/12/07  17:16:45  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:38:19  jaffe]
 * 
 * Revision 1.1.2.4  1993/01/18  22:02:22  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  21:41:53  cjd]
 * 
 * Revision 1.1.2.3  1992/11/24  16:19:28  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:17:19  bolinger]
 * 
 * Revision 1.1.2.2  1992/11/18  18:58:06  jaffe
 * 	Transarc delta: rajesh-ot5699-trans-concurrency-to-required-logsize 1.4
 * 	  Selected comments:
 * 	    Create a tool to give log size needed for a certain  transaction
 * 	    concurrency level
 * 	    See above.
 * 	    Shots at finding out compilation problems. To be "defuncted"
 * 	    Encountered compilation problems when compiling on pmax. So included more
 * 	    header files to match those in newaggr.c. But this turned out be bogus.
 * 	    Actually, libs logbuf,async,tools were referencing symbols defined in
 * 	    lib lwpdummy which was appeared on the command line earlier in sequence
 * 	    and hence the symbols were not found. A fix appears to be to change the
 * 	    sequence of libs on the command line. Need to check with integration
 * 	    about this.
 * 	    Checkout rcs revision 1.1 of the calcLogSize.c and check it back in.
 * 	    To get around the cml defunct of version 1.2 of this delta.
 * 	    See above.
 * 	    Fix link-time errors while building calcLogSize.
 * 	[1992/11/17  20:13:23  jaffe]
 * 
 * Revision 1.1  92/10/19  21:51:36  rajesh
 * 	Create a tool to give log size needed for a certain  transaction
 * 	concurrency level
 * 
 * 	See above.
 * 	[added by delta rajesh-ot5699-trans-concurrency-to-required-logsize, r1.1]
 * 
 * $EndLog$
 */
/* Copyright (C) 1992 Transarc Corporation - All rights reserved. */

/*
 * calcLogSize - calculates the log size needed to allow a certain
 * number of concurrent transactions given the aggregate block size.
 */

#include <dcedfs/stds.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/episode/async.h>
#include <dcedfs/episode/logbuf.h>

/* user space I/O simulation */
#include <dcedfs/episode/us.h>

#define IS_POWER_OF_TWO(x) (((x) & ((x)-1)) == 0)

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

char *programName;

void usage () 
{
    fprintf(stderr,"Usage: %s <block size> <# concurrent transactions> \n", programName); 
    fprintf(stderr,"%s: calculates log size needed for specified transaction concurrency \n", programName);

}

main(argc, argv) 
int argc;
char **argv;
{
    struct async_device *dev;
    long BlkSize;
    unsigned NumConcurrentTrans;
    unsigned LogSize;
    long code;

    osi_setlocale(LC_ALL, "");
    programName =argv[0];

    if (argc < 3 ) {
	fprintf(stderr,"%s: insufficient arguments \n",programName);
	usage();
	exit(1);
    }

    BlkSize=strtol(argv[1], NULL, 0);
    NumConcurrentTrans=strtol(argv[2], NULL, 0);

    if (!IS_POWER_OF_TWO(BlkSize)) {
	fprintf(stderr,
		"Episode block size must be a power of two, not %ld.\n",
		BlkSize);
	exit(1);
    }

    code = asevent_InitDevice(US_MAJOR_DEV, 1, BlkSize, &dev);
    if (code) {
	fprintf(stderr,"%s: internal error %d from asevent_InitDevice\n",
		programName,code); 
	exit(1);
    } else {
	LogSize = elbb_ComputeMinLogSize (dev, NumConcurrentTrans);
/* 	printf("Log size of %u blocks needed for %u concurrent transactions and %ld byte block size \n",
	LogSize, NumConcurrentTrans, BlkSize);
*/
	printf("%u \n",LogSize);
	
	asevent_CloseDevice (dev);
    }
}
