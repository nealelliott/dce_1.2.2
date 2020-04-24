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
 * $Log: cuvb_nbi_006.c,v $
 * Revision 1.1.15.2  1996/02/17  23:30:59  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:33  marty]
 *
 * Revision 1.1.15.1  1995/12/11  23:01:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:51  root]
 * 
 * Revision 1.1.13.1  1994/04/01  20:10:45  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:16:47  jd]
 * 
 * Revision 1.1.11.1  1993/10/12  19:36:28  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:30  rhw]
 * 
 * Revision 1.1.4.5  1993/02/01  21:46:01  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:07:09  hinman]
 * 
 * Revision 1.1.4.4  1992/12/30  16:51:12  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:09:29  zeliff]
 * 	Revision 1.1.8.2  1993/01/11  16:16:51  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 1.1.4.3  1992/10/14  20:44:57  marty
 * 	OSC1.1.1 upgrade. Changes are compatible with osc1.0.4.
 * 	[1992/10/14  20:03:43  marty]
 * 
 * Revision 1.1.5.2  1992/10/08  16:24:28  garyf
 * 	use POSIX definition SEEK_SET
 * 	[1992/09/29  20:28:21  garyf]
 * 
 * Revision 1.1.4.2  1992/09/29  20:39:00  devsrc
 * 	[OT 5373] SNI/SVR4 Merge UPDATE: Rearranged header files to eliminate compilation
 * 	          errors on AIX3.2 .
 * 	[1992/09/24  20:06:42  sekhar]
 * 
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:33:02  sekhar]
 * 
 * Revision 1.1  1992/01/19  03:21:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990, 1991 by
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
 *	CUVB_NBI_006
 *
 *  TEST TOPICS:
 *
 *	NBI  -  Non-Blocking (Unix) I/O
 *
 *  ABSTRACT:
 *
 *	This test will varify that U*ix calls such as open and
 *	close really, truly do work.  
 *      (This differs from the previous test in that each thread
 *      acquires it's very own descriptor when reading data from
 *      the created file.)
 *
 *  AUTHORS:
 *
 *	CURTIN
 *
 *  CREATION DATE:
 *
 *	30 March 1990
 *
 *  MODIFICATION HISTORY:
 *	001	Paul Curtin	5 July 1990
 *		Added and ifdef proto to routine.
 *	002	Paul Curtin	17 July 1990
 *		Added an include file
 *	003	Webb Scales	4 September 1990
 *		Changed CMA calls to pass structures by reference
 *	004	Dave Butenhof	10 September 1990
 *		Fix order of including cma_ux.h, so it doesn't screw up
 *		files.h.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#if (_CMA_UNIX_TYPE == _CMA__SVR4)
#include <sys/types.h>/* bring in definition for ushort needed by sys/file.h */
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>		/* bring in definition for SEEK_SET */
#endif
#include <sys/file.h>
#include <limits.h>
#include <dce/cts_test.h>

#define BUF_SIZE     80
#define NUM_THREADS  5
#define NUM_ROWS     10

#if _CMA_OS_ == _CMA__UNIX
# define delete_file(path)   unlink (path)
#endif
#if _CMA_OS_ == _CMA__VMS
# define delete_file(path)   delete (path)
#endif

char          copy[NUM_ROWS][BUF_SIZE];     /* Holds copy of what is in file */
cma_t_mutex   position_mutex;               /* Protection for file_position  */
int           file_position;                /* Tracks what has been read     */

cma_t_address
threads_routine	(
	cma_t_address arg)
    {
    char buf2[BUF_SIZE];
    int  c_index;
    int  status;
    int  flags;
    int  mode;
    int  file_descr;
    int  position;
    char *path;
    int  i;


    path = "cma_junk";
#if _CMA_PLATFORM_ == _CMA__SINIX_MX300I
/*
*	FIX-ME:
*	This is necessary because the SINIX operating system does not, as is
*	specified in the man page, ignore the O_CREAT flag on an open if the
*	file exists.  What happens is that the file is truncated, which
*	causes this test to fail.
*/
	flags = O_RDWR;
#else
    flags = O_CREAT | O_RDWR;
#endif
    mode = 0400 | 0200 | 0100 | 0070 | 0007;   /* Give everyone access */


    /* Open the file */

    status = open (path, flags, mode);
    if (status == -1) {
	perror ("Open failed");
	return;
	}
    else
	file_descr = status;


    for (i = 1; i <= 2; i++) {	

	/* - lock the position counter on the file */
	cma_mutex_lock (&position_mutex);
	position = file_position * BUF_SIZE;
	file_position++;
	if ((lseek (file_descr, position, SEEK_SET)) == -1) {
	    perror ("Lseek failed");
	    return;
	    }
	cma_mutex_unlock (&position_mutex);


	if ((read (file_descr, buf2, BUF_SIZE)) == -1) {	  
	    perror ("Read failed");
	    return;
	    }	

	c_index = buf2[0] - 97;     /* a to j */

	status = strcmp (&copy[c_index][0], &buf2[0]);
	if (status != 0) {
	    perror ("Data Corruption");
	    return;
	    }
	else
	    copy[c_index][0] = 'z';      /* For parent to see later */
	}

    /* Close the file */

    if ((close (file_descr)) == -1) {
	perror ("Close failed");
	return;
	}
    }



main ()
    {
    cma_t_thread      thread_handles[NUM_THREADS];
    int  status;
    int  flags;
    int  mode;
    int  letter;
    int  file_descr;
    char buf1[BUF_SIZE];
    int  check_off[9];
    char *path;
    int  i,a,b,c,x,y;


    cts_test ("CUVB_NBI_006", "This tests U*ix file I/O calls.");  

    cma_init ();

    file_position = 0;

    cma_mutex_create (&position_mutex, &cma_c_null);

    /* Do some set up */

    path = "cma_junk";
    flags = O_CREAT | O_RDWR;
    mode = 0400 | 0200 | 0100 | 0070 | 0007;   /* Give everyone access */


    /* Open the file */

    status = open (path, flags, mode);
    if (status == -1) {
	perror ("Open failed");
	return;
	}
    else
	file_descr = status;


    /* Fill the file, and the copy */

    for (letter = 'a'; letter <= 'j'; letter ++) {	
	for (i = 0; i < BUF_SIZE-2; i++)
	    buf1[i] = copy[letter-97][i] = letter;

	buf1[BUF_SIZE-2] = copy[letter-97][BUF_SIZE-2] = '\n';
	buf1[BUF_SIZE-1] = copy[letter-97][BUF_SIZE-1] = '\0';

	if ((write (file_descr, buf1, BUF_SIZE)) == -1) {	  
	    perror ("Write failed");
	    return;
	    }
	}


    /* Return the descriptor to the beginning of the file */

    if ((lseek (file_descr, 0, SEEK_SET)) == -1) {
	perror ("Lseek failed");
	return;
	}


    /* Create threads */

    for (x = 0; x < NUM_THREADS; x++)
	cma_thread_create (
		&thread_handles[x],
		&cma_c_null,
		threads_routine,
		cma_c_null_ptr);


    /* Join the threads */

    for (x = 0; x < NUM_THREADS; x++)
	cma_thread_join (
		&thread_handles[x],
		cma_c_null_ptr,
		cma_c_null_ptr);


    /* Give others time to finish */

    cma_delay (3.0);


    /* Check that the threads got all the data */

    for (i = 0; i <= 9; i++)
	if (copy[i][0] != 'z') {
	    perror ("Data Check Error");
	    return;
	    }


    /* Close the file */

    if ((close (file_descr)) == -1) {
	perror ("Close failed");
	return;
	}


    /* Remove the file from the directory */

    if ((delete_file (path)) == -1) {
	perror ("delete_file failed");
	return;
	}

    cts_result ();  
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_NBI_006.C */
/*  *11    2-JUN-1991 16:40:27 BUTENHOF "Fix race in data check" */
/*  *10    1-MAR-1991 13:40:08 BUTENHOF "Fix header ordering" */
/*  *9    10-SEP-1990 13:34:38 BUTENHOF "Fix more junk" */
/*  *8    10-SEP-1990 13:16:19 BUTENHOF "Fix order of include for cma_ux.h" */
/*  *7     4-SEP-1990 16:53:30 SCALES "Convert to new CMA interfaces" */
/*  *6    17-JUL-1990 14:19:30 CURTIN "Added an include file" */
/*  *5     5-JUL-1990 16:07:55 CURTIN "Added an ifdef proto" */
/*  *4     3-JUL-1990 12:07:17 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:35:51 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:40:39 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:59:35 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_NBI_006.C */
