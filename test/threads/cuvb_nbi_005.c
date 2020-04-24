/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cuvb_nbi_005.c,v $
 * Revision 1.1.15.2  1996/02/17  23:30:58  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:32  marty]
 *
 * Revision 1.1.15.1  1995/12/11  23:00:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:50  root]
 * 
 * Revision 1.1.13.1  1994/04/01  20:10:44  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:16:43  jd]
 * 
 * Revision 1.1.11.1  1993/10/12  19:36:26  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:29  rhw]
 * 
 * Revision 1.1.6.5  1993/02/04  00:13:36  jd
 * 	Moved the include of cma.h to before the check of
 * 	CMA_UNIX_TYPE == CMA__SVR4.
 * 	[1993/02/04  00:05:37  jd]
 * 
 * Revision 1.1.6.4  1993/02/01  21:34:14  hinman
 * 	[hinman] - SVR4 changes
 * 	[1993/02/01  21:06:18  hinman]
 * 
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:07:01  hinman]
 * 
 * Revision 1.1.6.3  1992/12/30  16:51:09  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:09:23  zeliff]
 * 	Revision 1.1.9.2  1993/01/11  16:16:41  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 1.1.6.2  1992/10/14  20:44:52  marty
 * 	OSC1.1.1 upgrade. Changes are compatible with osc1.0.4.
 * 	[1992/10/14  20:03:35  marty]
 * 
 * Revision 1.1.4.2  1992/10/08  16:23:22  garyf
 * 	use POSIX definition SEEK_SET
 * 	[1992/09/29  20:28:15  garyf]
 * 
 * Revision 9.5.3.2  92/12/17  13:00:33  raj
 * 	[raj] New naming + parallelized reentrant library changes.
 * 
 * Revision 9.5.1.4  92/10/20  16:37:22  raj
 * 	Merge in SVR4 changes
 * 
 * Revision 9.5.2.2  92/10/20  16:35:02  raj
 * 	Merge in SVR4 changes
 * 
 * Revision 9.5.1.3  92/08/05  13:54:10  hinman
 * 	[hinman] - Fold backward from DCE1TOP
 * 
 * Revision 9.5.2.2  92/08/05  11:24:57  blurie
 * 	Merge our changes into OSF files.
 * 
 * Revision 9.5.1.1  92/07/24  18:57:09  hinman
 * 	Create merge branch
 * 
 * Revision 9.5  92/07/24  18:57:07  hinman
 * 	Update checkin
 * 
 * Revision 1.1.2.2  1992/05/15  16:58:02  bolinger
 * 	Fix OT defect 3464:  Add some rudimentary testing of fcntl() to this
 * 	testcase, due to recent changes to cma_fcntl() wrapper (to make it
 * 	POSIX-compliant -- see OT defect 1711).
 * 	[1992/05/14  13:10:37  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:21:29  devrcs
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
 *	CUVB_NBI_005
 *
 *  TEST TOPICS:
 *
 *	NBI  -  Non-Blocking (Unix) I/O
 *
 *  ABSTRACT:
 *
 *	This test will varify that U*ix calls such as open and
 *	close really, truly do work.  
 *
 *  AUTHORS:
 *
 *	Paul Curtin
 *
 *  CREATION DATE:
 *
 *	30 March 1990
 *
 *  MODIFICATION HISTORY:
 *	001	Paul Curtin	5 July 1990
 *		Added an ifdef proto statement to a routine.
 *	002	Paul Curtin	17 July 1990
 *		Added an include file
 *	003	Webb Scales	4 September 1990
 *		Changed CMA calls to pass structures by reference.
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
#include <unistd.h>		/* bring in definition for SEEK_SET */
#endif
#include <sys/file.h>
#include <fcntl.h>
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

char copy[NUM_ROWS][BUF_SIZE];
int  file_descr;


cma_t_address
threads_routine	(
	cma_t_address arg)
    {
    char buf2[BUF_SIZE];
    int  c_index;
    int  status;
    int  i;


    for (i = 1; i <= 2; i++) {	
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
    }



main ()
    {
    cma_t_thread      thread_handles[NUM_THREADS];
    cma_t_exit_status exit_status;
    cma_t_address     result;
    int  status;
    int  flags;
    int  mode;
    int  letter;
    char buf1[BUF_SIZE];
    int  check_off[9];
    char *path;
    int  i,a,b,c,x,y;
	struct flock	fls;


    cts_test ("CUVB_NBI_005", "This tests U*ix file I/O calls.");  

    cma_init ();


    /* Do some set up */

#if _CMA_UNIX_TYPE == _CMA__SVR4
	/*
	*	SVR4 does not communicate very well with lock daemons
	*	on other machines.  Use a file name on a local file 
	*	system.
	*/
	if ( (path = tempnam( "/tmp", "cma" )) == (char *)NULL )
		{
		perror("tempnam");
		cts_failed("tempnam failed");
		}

	/*
	*	Just make sure that there is no other file there.
	*/

	status = unlink(path);
	if ( (status == -1) && (errno != ENOENT) )
		{
		perror("unlink");
		cts_failed("unlink failed.");
		}
#else
    path = "cma_junk";
#endif
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

	/*
	 * Now exercise fcntl() by setting/getting close-on-exec flag,
	 * setting/clearing a lock.
	 */
	if (fcntl(file_descr, F_SETFD, FD_CLOEXEC) == -1) {
		perror ("F_SETFD to set failed");
		return;
	}
	if (fcntl(file_descr, F_GETFD) != FD_CLOEXEC) {
		perror ("F_GETFD failed");
		return;
	}
	if (fcntl(file_descr, F_SETFD, 0) == -1) {
		perror ("F_SETFD to clear failed");
		return;
	}

	fls.l_type = F_WRLCK;
	fls.l_whence = SEEK_SET;
	fls.l_start = 0;
	fls.l_len = 32;
	fls.l_pid = getpid();

	if (fcntl(file_descr, F_SETLK, &fls) == -1) {
		perror ("F_SETLK failed");
		return;
	}
	fls.l_type = F_UNLCK;
	if (fcntl(file_descr, F_SETLK, &fls) == -1) {
		perror ("Unlock failed");
		return;
	}

    /* Create threads */

    for (x = 0; x < NUM_THREADS; x++)
	cma_thread_create (
		&thread_handles[x],
		&cma_c_null,
		threads_routine,
		cma_c_null_ptr);


    /* Join the last thread created */

    cma_thread_join (
	    &thread_handles[NUM_THREADS-1],
	    &exit_status,
	    &result);


    /* Give others time to finish */

    cma_delay (3.0);


    /* Check that the threads got all the data */

    for (i = 0; i <= 9; i++)
	if (copy[i][0] != 'z') {
	    perror ("Data Check Error");
	    return;
	    }


    /* Close the file */

    if ((close(file_descr)) == -1) {
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


/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_NBI_005.C */
/*  *10    1-MAR-1991 13:12:14 BUTENHOF "Fix order of includes" */
/*  *9    10-SEP-1990 13:34:32 BUTENHOF "Fix more junk" */
/*  *8    10-SEP-1990 13:16:14 BUTENHOF "Fix order of include for cma_ux.h" */
/*  *7     4-SEP-1990 16:53:16 SCALES "Convert to new CMA interfaces" */
/*  *6    17-JUL-1990 14:18:08 CURTIN "Added an include file" */
/*  *5     5-JUL-1990 16:07:45 CURTIN "Added an ifdef proto" */
/*  *4     3-JUL-1990 12:07:11 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:35:37 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:40:30 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:59:30 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_NBI_005.C */
