/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cuvb_nbi_009.c,v $
 * Revision 1.2.58.2  1996/02/17  23:31:02  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:36  marty]
 *
 * Revision 1.2.58.1  1995/12/11  23:01:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:54  root]
 * 
 * Revision 1.2.56.1  1994/04/01  20:10:51  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:17:01  jd]
 * 
 * Revision 1.2.54.1  1993/10/12  19:36:34  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:32  rhw]
 * 
 * Revision 1.2.4.2  1992/12/30  16:51:23  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:09:42  zeliff]
 * 
 * Revision 1.2.2.3  1992/06/12  19:58:02  bolinger
 * 	Fix OT defect 3776:  properly initialize the fd_set rfd_mask before
 * 	using it.
 * 	[1992/06/12  19:57:36  bolinger]
 * 
 * Revision 1.2.2.2  1992/06/12  19:28:10  bolinger
 * 	Fix CMA defect 4005:  try harder to avoid including sys/select.h
 * 	unnecessarily.  Use absence of FD_SET preprocessor symbol to
 * 	indicate that file needs to be included.
 * 	[1992/06/10  22:06:15  bolinger]
 * 
 * Revision 1.2  1992/01/19  22:14:43  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
*/
/*
 *  OSF DCE Version 1.0 
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
 *	CUVB_NBI_009
 *
 *  TEST TOPICS:
 *
 *	NBI  -  Non-Blocking (Unix) I/O
 *
 *  ABSTRACT:
 *
 *	This test endeavors to show that a blocking call to 
 *	the select wrapper will eventually return the right
 *	results.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	14 November 1991
 *
 *  MODIFICATION HISTORY:
 *
 */


/*
 *  INCLUDE FILES
 */
#include <dce/cma.h>
#include <dce/cts_test.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#ifndef FD_SET
/*
 * Try to figure out if we still need an fd_set definition;
 * if so, get it from here.
 */
# include <sys/select.h>
#endif

#define WRITE_END   1
#define READ_END    0
#define RD_BUFSIZ   81
#define SELECT_TIMEOUT	10
#define WRITE_DELAY	5.0
#define ALERT_DELAY	50.0


extern char	*sys_errlist[];
cma_t_thread	main_thread;
int	pipe_fd[2];
char	message[] = "Hello?";


cma_t_address
pipe_writer	(cma_t_address arg)
    {
    cma_delay (WRITE_DELAY);
    write (pipe_fd[WRITE_END], message, strlen(message));

    cma_delay (WRITE_DELAY);
    cma_thread_alert (&main_thread);

    return cma_c_null_ptr;
    }


int
main	(int argc, char *argv[], char *envp[])
    {
    fd_set  rfd_mask;
    struct timeval timeout;
    int	    retval;
    char    rd_buf[RD_BUFSIZ];
    cma_t_thread    writer_thread;


	FD_ZERO (&rfd_mask);
    cts_test ("CUVB_NBI_009", "Test blocking select wrapper operation");
    TRY {
	cma_init ();

	if (pipe (pipe_fd) == -1) {
	    cts_failed ("Pipe call failed: %s", sys_errlist[errno]);
	    }
	else {
	    cma_thread_get_self (&main_thread);

	    cma_thread_create (
		    &writer_thread,
		    &cma_c_null,
		    pipe_writer,
		    cma_c_null_ptr);

	    FD_SET (pipe_fd[READ_END], &rfd_mask);

	    timeout.tv_sec = SELECT_TIMEOUT;
	    timeout.tv_usec = 0;

	    retval = select (
		    pipe_fd[READ_END] + 1, 
		    &rfd_mask, 
		    (fd_set *)cma_c_null_ptr,
		    (fd_set *)cma_c_null_ptr,
		    &timeout);

	    if (retval != 1) {
		switch (retval) {
		    case -1:
			{
			cts_failed ("select call failed: %s", sys_errlist[errno]);
			break;
			}
		    case 0:
			{
			cts_failed ("select call timed out.");
			break;
			}
		    default:
			{
			cts_failed ("select call returned incorrect value: %d", retval);
			break;
			}
		    }
		}
	    else {
		if (FD_ISSET (pipe_fd[READ_END], &rfd_mask)) {
		    retval = read (pipe_fd[READ_END], rd_buf, RD_BUFSIZ-1);
		    if (retval < 0)
			cts_failed ("read call failed: %s", sys_errlist[errno]);
		    else {
			rd_buf[retval] = '\0';
			if (retval != strlen(message))
			    cts_failed (
				    "wrong message length: %d [%s]",
				    retval, 
				    rd_buf);
			else 
			    printf ("message received: \"%s\"\n", rd_buf);
			}
		    }
		else {
		    cts_failed ("wrong bit set in file descriptor mask.");
		    }
		}
	    }
	}
    CATCH_ALL {
	cts_failed ("Unexpected exception.");
	exc_report (THIS_CATCH);
	}
    ENDTRY

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_NBI_009.C */
/*  *1    14-NOV-1991 18:03:14 SCALES "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_NBI_009.C */
