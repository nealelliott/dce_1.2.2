/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 * (c) Copyright 1990, 1991, 1992
 */
/*
 * HISTORY
 * $Log: cuvb_nbi_008.c,v $
 * Revision 1.1.79.1  1996/06/04  22:14:09  arvind
 * 	Fix for HP
 * 	[1996/03/08  22:19 UTC  bissen  /main/DCE_1.2/1]
 *
 * Revision 1.1.77.2  1996/02/17  23:31:01  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:35  marty]
 * 
 * Revision 1.1.77.1  1995/12/11  23:01:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:53  root]
 * 
 * Revision 1.1.75.1  1994/04/01  20:10:49  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:16:56  jd]
 * 
 * Revision 1.1.73.1  1993/10/12  19:36:32  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:31  rhw]
 * 
 * Revision 1.1.5.3  1992/12/30  16:51:18  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:09:37  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  20:39:18  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:33:19  sekhar]
 * 
 * Revision 1.1.2.2  1992/05/11  17:07:17  bolinger
 * 	Fix OT defect 2161:  provide/correct missing/bad cts_test() banners.
 * 	[1992/05/11  13:45:16  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:21:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1991 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 *  This software is furnished under a license and may be used and copied
 *  only in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any other
 *  copies thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without notice
 *  and should  not  be  construed  as  a commitment by DIGITAL Equipment
 *  Corporation.
 *
 *  DIGITAL assumes no responsibility for the use or  reliability  of its
 *  software on equipment which is not supplied by DIGITAL.
 */

/*
 *  FACILITY:
 *
 *	CMA Test Programs
 *
 *  FILENAME:
 *
 *	CUVB_NBI_008
 *
 *  TEST TOPICS:
 *
 *	NBI  -  Non-Blocking (Unix) I/O
 *
 *  ABSTRACT:
 *
 *	Stress open and close from multiple threads.  The
 *	threads all try to open and close the same file.
 *
 *  AUTHORS:
 *
 *	Paul Curtin
 *
 *  CREATION DATE:
 *
 *	20 June 1991
 *
 *  MODIFICATION HISTORY:
 *
 */


/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>
#include <limits.h>
#if (_CMA_UNIX_TYPE == _CMA__SVR4) && (!defined(SNI_BSD_CMP))
#include <sys/types.h>/* bring in definition for ushort needed by sys/file.h */
#include <sys/stat.h>
#include <sys/fcntl.h>
#endif
#if _CMA_OS_ != _CMA__VMS
# include <sys/file.h>
#else
# include <file.h>
#endif

#define	NUM_THREADS	4
#if (_CMA_UNIX_TYPE == _CMA__SVR4) || (_CMA_VENDOR_ == _CMA__HP)
#define NUM_OPENS	10000
#else
#define NUM_OPENS	1000000
#endif

#if _CMA_OS_ == _CMA__UNIX
# define delete_file(path)   unlink (path)
#endif
#if _CMA_OS_ == _CMA__VMS
# define delete_file(path)   delete (path)
#endif

int  file_descr;
char *path = "cma_junk";


cma_t_address
threads_routine	(
	cma_t_address arg)
    {
    int  fd;
    int  flags;
    int  mode;
    int	 i;


    /* File open arguments */

    flags = O_RDONLY;
    mode = 0400 | 0200 | 0100 | 0070 | 0007;   /* Give everyone access */

    for (i = 0; i < NUM_OPENS; i++) {
        fd = open (
		path,
		flags, 
		mode
#if _CMA_OS_ == _CMA__VMS
		, "shr = get"
#endif
		);		/* Open the file */

        if (fd == -1) {
	    cts_failed ("Open failed");
	    perror ("Reason - ");
	    }
	else					/* Close the file */
	    if ((close(fd)) == -1) {
		cts_failed ("Close failed");
		perror ("Reason - ");
		}

	}
    }

main ()
    {
    cma_t_thread      thread_handles[NUM_THREADS];
    cma_t_exit_status exit_status;
    cma_t_address     result;
    int  status;
    int  flags;
    int  mode;
    int	 x;
    int  y;


    cts_test ("CUVB_NBI_008", "Stress test open() close()");

    cma_init ();

    /* Do some set up */

    flags = O_CREAT | O_RDONLY;
    mode = 0400 | 0200 | 0100 | 0070 | 0007;   /* Give everyone access */

    /* Open the file - and CREATE */

    status = open (
		path,
		flags, 
		mode
#if _CMA_OS_ == _CMA__VMS
		, "shr = get"
#endif
		);		/* Open the file */

    if (status == -1) {
        cts_failed ("Open (file create) failed");
	perror ("");
	}
    else {
	file_descr = status;

	for (x = 0; x < NUM_THREADS; x++)	/* Create threads */
	    cma_thread_create (
		    &thread_handles[x],
		    &cma_c_null,
		    threads_routine,
		    cma_c_null_ptr);

	for (y = 0; y < NUM_THREADS; y++) {	/* Join threads */
	    cma_thread_join (
		    &thread_handles[y],
		    &exit_status,
		    &result);

	    if (exit_status != cma_c_term_normal)
		cts_failed ("A thread has exited improperly");
	    }

	if ((close(file_descr)) == -1) {	/* Close the file */
	    cts_failed ("Close failed");
	    perror ("Reason - ");
	    }

	if ((delete_file (path)) == -1) perror ("Reason - ");
	}

    cts_result ();  

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_NBI_008.C */
/*  *1    20-JUN-1991 15:52:20 CURTIN "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_NBI_008.C */
