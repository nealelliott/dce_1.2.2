/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cuvb_nbi_004.c,v $
 * Revision 1.1.98.2  1996/02/17  23:30:57  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:31  marty]
 *
 * Revision 1.1.98.1  1995/12/11  23:00:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:49  root]
 * 
 * Revision 1.1.95.1  1994/04/01  20:10:43  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:16:38  jd]
 * 
 * Revision 1.1.93.1  1993/10/12  19:36:24  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:28  rhw]
 * 
 * Revision 1.1.4.4  1992/12/30  16:51:06  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:09:19  zeliff]
 * 
 * Revision 1.1.4.3  1992/10/14  20:44:45  marty
 * 	OSC1.1.1 upgrade. Changes are compatible with osc1.0.4.
 * 	[1992/10/14  20:03:28  marty]
 * 
 * Revision 1.1.6.2  1992/10/08  16:22:43  garyf
 * 	change name of local procedure to not conflict
 * 	with listen()
 * 	[1992/09/29  20:28:09  garyf]
 * 
 * Revision 1.1.4.2  1992/09/03  14:32:13  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:45:06  bolinger]
 * 
 * Revision 1.1.2.2  1992/08/12  21:11:34  bolinger
 * 	Replace uses of struct timeb/ftime(3) with refs to new timing
 * 	interface specific to these tests (which is designed to hide
 * 	any further changes to the underlying system facilities used).
 * 
 * Revision 1.1  1992/01/19  03:21:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990 by
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
 *	CUVB_NBI_004
 *
 *  TEST TOPICS:
 *
 *	NBI  -  Non-Blocking (Unix) I/O
 *
 *  ABSTRACT:
 *
 *	This test attempts to ensure that the CMA wrapper for the
 *	select U*ix system call works under various situations.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	27 September 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Webb Scales	2 October 1990
 *		Corrected time calculation.
 */


/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cma_ux.h>


#define my_failure(message) {	\
        perror(message);	\
	cts_failed(message);	\
        } 

#define	delay	2.5


cma_t_integer	p_fd[2];
cma_t_mutex	mutex;
cma_t_cond	cond;
cma_t_boolean	ping = cma_c_false;



cma_t_address
echo	(
	cma_t_integer	id)
    {
    char		    buf[80];
    cma_t_integer	    cc;


    cma_lock_global ();
    printf ("Thread #%d: starting test.\n", id);
    cma_unlock_global ();

    /*
     * Phase one, selects on an empty socket.
     */

    /*
     * Hang out until the other thread yells.
     */
    cma_mutex_lock (&mutex);
    while (!ping)
	cma_cond_wait (&cond, &mutex);
    ping = cma_c_false;
    cma_mutex_unlock (&mutex);

    /*
     * Phase one-A, demonstrate that a blocking select with no data waiting
     *		    blocks the thread until data becomes available.
     */

    cma_delay (delay);

    sprintf (buf, "Message from Thread #%d.", id);

    cc = write (p_fd[id], buf, strlen(buf));
    if (cc < 0) {
	my_failure ("Error in 'write'");
	}
    else
	if (cc != strlen(buf))
	    cts_failed (
		    "Only partial buffer sent: %d out of %d characters.",
		    cc,
		    strlen(buf));

    /*
     * Phase two, selects on a socket that has data waiting.
     */

    /*
     * Nothing to do, everything is all set up, so exit.
     */
    return (cma_t_address)cc;
    }


cma_t_address
my_listen	(
	cma_t_integer	id)
    {
    cma_t_integer	    rfds, nfound;
    static struct timeval   poll = {0, 0},
			    timeout = {2, 500000};   /* 2.5 Seconds */
    cts_timebuf_t	    start;
    cts_timebuf_t	    end;


    cma_lock_global ();
    printf ("Thread #%d: starting test.\n", id);
    cma_unlock_global ();

    /*
     * Phase one, doing selects on an empty socket.
     */

    /*
     * Test #1:  Polling select with no waiting data.
     */
    rfds = 1 << p_fd[id];
    nfound = select (p_fd[id]+1, &rfds, 0, 0, &poll);
    if (nfound < 0) my_failure ("Error in select (poll/no data)");
    if (nfound > 0) cts_failed ("Polling select with no data, found data.");

    /*
     * Test #2:  Timed select with no waiting data.
     */
    rfds = 1 << p_fd[id];
    nfound = select (p_fd[id]+1, &rfds, 0, 0, &timeout);
    if (nfound < 0) my_failure ("Error in select (timeout/no data)");
    if (nfound > 0) cts_failed ("Timeout select with no data, found data.");


    /*
     * Phase one-A, demonstrate that a blocking select with no data waiting
     *		    blocks the thread until data becomes available.
     */

    /*
     * Tell other thread to initiate a delayed write to the socket.  It will 
     * begin its delay after we block on the select.
     */
    cts_gettime(&start);

    cma_mutex_lock (&mutex);
    ping = cma_c_true;
    cma_mutex_unlock (&mutex);
    cma_cond_signal (&cond);

    /*
     * Test #3:  "Blocking" select with no waiting data.
     */
    rfds = 1 << p_fd[id];

    nfound = select (p_fd[id]+1, &rfds, 0, 0, 0);
    cts_gettime(&end);

    if (nfound < 0) my_failure ("Error in select (blocking/no data)");
    if (nfound == 0) 
	    cts_failed ("Blocking select with no data, returned spuriously.");

    {
	float	delta = cts_timediff(&start, &end);

	if (delta < delay)
		cts_failed ("Blocking select with no data, blocked for %5.3f rather than %5.3f seconds.", delta, delay);
	}

    /*
     * Phase two, doing selects on a socket that has data waiting.
     */

    /*
     * Test #4:  Polling select with waiting data.
     */
    rfds = 1 << p_fd[id];
    nfound = select (p_fd[id]+1, &rfds, 0, 0, &poll);
    if (nfound < 0) my_failure ("Error in select (poll/data)");
    if (nfound == 0) cts_failed ("Polling select with data, found no data.");

    /*
     * Test #5:  Timed select with waiting data.
     */
    rfds = 1 << p_fd[id];
    nfound = select (p_fd[id]+1, &rfds, 0, 0, &timeout);
    if (nfound < 0) my_failure ("Error in select (timeout/data)");
    if (nfound == 0) cts_failed ("Timeout select with data, found no data.");

    /*
     * Test #6:  "Blocking" select with waiting data.
     */
    rfds = 1 << p_fd[id];
    nfound = select (p_fd[id]+1, &rfds, 0, 0, 0);
    if (nfound < 0) my_failure ("Error in select (blocking/data)");
    if (nfound == 0) cts_failed ("Blocking select with data, found no data.");

    return cma_c_null_ptr;
    }


int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_thread	e_thread;
    cma_t_exit_status	status;
    cma_t_integer	msg;


    cts_test ("CUVB_NBI_004", "Test select system call");
    cma_init ();

    if (pipe(p_fd) == -1)
	my_failure ("Error calling 'pipe'");

    cma_mutex_create (&mutex, &cma_c_null);
    cma_cond_create  (&cond,  &cma_c_null);

    cma_thread_create (&e_thread, &cma_c_null, echo, (cma_t_address)1);
    cma_yield ();

    my_listen (0);

    cma_thread_join (&e_thread, &status, (cma_t_address *)&msg);
    if (status != cma_c_term_normal)
	cts_failed ("Bad status from subordinate thread.");

    printf ("Subordinate sent %d characters.\n", msg);

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_NBI_004.C */
/*  *2     2-OCT-1990 11:29:32 SCALES "Fix timer bug" */
/*  *1    27-SEP-1990 18:33:53 SCALES "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_NBI_004.C */
