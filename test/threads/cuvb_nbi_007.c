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
 * $Log: cuvb_nbi_007.c,v $
 * Revision 1.1.17.2  1996/02/17  23:31:00  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:34  marty]
 *
 * Revision 1.1.17.1  1995/12/11  23:01:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:52  root]
 * 
 * Revision 1.1.14.1  1994/04/01  20:10:47  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:16:52  jd]
 * 
 * Revision 1.1.12.1  1993/10/12  19:36:30  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:30  rhw]
 * 
 * Revision 1.1.10.2  1993/06/14  20:17:06  sommerfeld
 * 	Removed ifdef of SVR4 around fix that
 * 	sets addrlen before calling accept.
 * 	If __hpux, define random() to be rand().
 * 	[1993/05/06  20:07:21  hopkins]
 * 
 * Revision 1.1.4.3  1992/12/30  16:51:16  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:09:33  zeliff]
 * 
 * Revision 1.1.4.2  1992/09/29  20:39:07  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:33:13  sekhar]
 * 
 * Revision 1.1  1992/01/19  03:21:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990, 1991 by
 *  Digital Equipment Corporation, Maynard, Mass.
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
 *  and should  not  be  construed  as  a commitment by Digital Equipment
 *  Corporation.
 *
 *  Digital assumes no responsibility for the use or  reliability  of its
 *  software on equipment which is not supplied by Digital.
 */

/*
 *  FACILITY:
 *
 *	CMA Test Programs
 *
 *  FILENAME:
 *
 *      CUVB_NBI_007
 *
 *  TEST TOPICS:
 *
 *	NBI  -  Non-Blocking (Unix) I/O
 *
 *  ABSTRACT:
 *
 *	This test program tests U*ix I/O system calls which are
 *	necessary for performing connection-based communications
 *	using sockets.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	02 July 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Webb Scales	4 September 1990
 *		Changed CMA calls to pass structures by reference.
 *	002	Dave Butenhof	11 October 1990
 *		Use cma_errno.h.
 *	003	Dave Butenhof	20 February 1991
 *		Correct alert control.
 *	004	Dave Butenhof	23 May 1991
 *		Fix compilation errors under new MIPS C compiler.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>

#ifdef __hpux
# define random()	((long)rand())
#endif

#define BACKLOG		5	/* Currently max allowed */
#define BUF_SIZ		8192
#define NCLIENTS	5
#define NSERVERS	5
#define TEST_TIME	10.0

extern char	*sys_errlist[];

cma_t_mutex	the_mutex;
cma_t_cond	server_ready, client_ready;
cma_t_thread	master_thread, listener_thread, talker_thread;
cma_t_boolean	done = cma_c_false;
struct sockaddr_in name;

struct a_thread {
    cma_t_integer	id;
    cma_t_integer	mesgs;
    cma_t_boolean	alive;
    int			socket;
    cma_t_thread	handle;
    cma_t_mutex		mutex;
    }	server_thread[NSERVERS], client_thread[NCLIENTS];


cma_t_address
client	(
	cma_t_address	p)
    {
    struct a_thread	*myself = (struct a_thread *)p;
    int 		s;
    int 		i, j, ci, co;
    char 		in_buf[BUF_SIZ], out_buf[BUF_SIZ];
    cma_t_alert_state	prior;


    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) {
	cts_failed (
		"Thread #%d:  Error creating socket: %s.", 
		myself->id,
		sys_errlist[errno]);
	cma_thread_alert (&master_thread);
        cma_thread_exit_error ();
        }

    if (connect(s, (struct sockaddr *)&name, sizeof(name)) == -1) {
	cts_failed (
		"Thread #%d:  Error connecting socket: %s.", 
		myself->id,
		sys_errlist[errno]);
	cma_thread_alert (&master_thread);
        cma_thread_exit_error ();
        }

    cma_mutex_lock (&the_mutex);
#if (_CMA_UNIX_TYPE == _CMA__SVR4)
	{
	struct timeval t;
	/*
	*	FIX-ME:
	*		We should use the SVR4 version of gettimeofday, but
	*		we don't.
		if ( gettimeofday( &t ) == -1 )
	*/
	struct timezone tz;

		if ( gettimeofday( &t ) == -1 )
			{
			cts_failed("gettimeofday: %s", sys_errlist[errno]);
			}
		srand( (unsigned int) t.tv_sec );
		j = rand() % 127;
	}
#else
    j = random() % 127;
#endif
    for (i = 0; (i < j) && (!done); i++) {
	cma_mutex_unlock (&the_mutex);

        cma_mutex_lock (&myself->mutex);
	sprintf (out_buf, "msg #C%d from #C%d", myself->mesgs++, myself->id);
        cma_mutex_unlock (&myself->mutex);
	ci = strlen(out_buf);

        co = send (s, out_buf, ci, 0);
        if (co == -1) {
	    cts_failed (
		    "Thread #%d:  Error in send (client): %s.", 
		    myself->id,
		    sys_errlist[errno]);
	    cma_thread_alert (&master_thread);
            cma_thread_exit_error ();
	    }
	else if (co != ci) {
	    cts_failed (
		    "Thread #%d:  Error in send (client): only %d of %d chars written.",
		    myself->id,
		    co,
		    ci);
	    cma_thread_alert (&master_thread);
            cma_thread_exit_error ();
	    }

        ci = recv(s, in_buf, BUF_SIZ, 0);
        if (ci == -1) {
	    cts_failed (
		    "Thread #%d:  Error in recv (client): %s.", 
		    myself->id,
		    sys_errlist[errno]);
	    cma_thread_alert (&master_thread);
            cma_thread_exit_error ();
	    }
	in_buf[ci] = '\0';

	cma_mutex_lock (&the_mutex);
        }
    cma_mutex_unlock (&the_mutex);

    cma_alert_disable_general (&prior);

    close (s);

    cma_mutex_lock (&myself->mutex);
    sprintf (
	out_buf,
	"Client thread #%d:  sent %d messages.\n",
	myself->id,
	myself->mesgs);
    ci = strlen(out_buf);
    write (1, out_buf, ci);

    cma_thread_detach (&myself->handle);
    myself->socket = -1;
    myself->alive = cma_c_false;
    cma_mutex_unlock (&myself->mutex);
    cma_cond_signal (&client_ready);

    return cma_c_null_ptr;
    }


cma_t_address
server	(
	cma_t_address	p)
    {
    struct a_thread	*myself = (struct a_thread *)p;
    int 		s = ((struct a_thread *)p)->socket;
    int 		ci, co;
    char 		in_buf[BUF_SIZ], out_buf[BUF_SIZ];
    cma_t_alert_state	prior;


    cma_mutex_lock (&the_mutex);
    while (!done) {
	cma_mutex_unlock (&the_mutex);
        ci = recv(s, in_buf, BUF_SIZ, 0);
	
	/* 
	* check for error and disconnect 
	*/
#if _CMA_UNIX_TYPE == _CMA__SVR4
	if ( ci == 0 )	/* client disconnect */
		{
		cma_mutex_lock (&the_mutex);
		break;
		}
	else if ( ci == -1 )
		{
		cts_failed (
			"Thread #%d:  Error in recv (server): %s.", 
			myself->id,
			sys_errlist[errno]);
		cma_thread_alert (&master_thread);
        	cma_thread_exit_error ();
		}
#else
        if (ci == -1) {
	    if (errno == ECONNRESET) {	/* Client disconnect */
		cma_mutex_lock (&the_mutex);
		break;
		}
	    else {
		cts_failed (
			"Thread #%d:  Error in recv (server): %s.", 
			myself->id,
			sys_errlist[errno]);
		cma_thread_alert (&master_thread);
        	cma_thread_exit_error ();
		}
	    }
#endif

	in_buf[ci] = '\0';
	cma_mutex_lock (&myself->mutex);
	sprintf (
	    out_buf, 
	    "/%s/ from #S%d msg #S%d", 
	    in_buf, 
	    myself->id, 
	    myself->mesgs++);
	cma_mutex_unlock (&myself->mutex);
	ci = strlen(out_buf);

        co = send (s, out_buf, ci, 0);
        if (co == -1) {
	    cts_failed (
		    "Thread #%d:  Error in send (server): %s.", 
		    myself->id,
		    sys_errlist[errno]);
	    cma_thread_alert (&master_thread);
            cma_thread_exit_error ();
	    }
	else if (co != ci) {
	    cts_failed (
		    "Thread #%d:  Error in send (server): only %d of %d chars written.",
		    myself->id,
		    co,
		    ci);
	    cma_thread_alert (&master_thread);
            cma_thread_exit_error ();
	    }
	cma_mutex_lock (&the_mutex);
        }
    cma_mutex_unlock (&the_mutex);

    cma_alert_disable_general (&prior);

    close (s);

    cma_mutex_lock (&myself->mutex);
    sprintf (
	out_buf,
	"Server thread #%d: sent %d messages.\n",
	myself->id,
	myself->mesgs);
    ci = strlen(out_buf);
    write (1, out_buf, ci);

    cma_thread_detach (&myself->handle);
    myself->socket = -1;
    myself->alive = cma_c_false;
    cma_mutex_unlock (&myself->mutex);
    cma_cond_signal (&server_ready);

    return cma_c_null_ptr;
    }


cma_t_address
listener	(
	cma_t_address	p)
    {
    int			s, ns;
    struct sockaddr	addr;
    int			addrlen;
    cma_t_integer	i, s_cnt = 1;
    cma_t_boolean	found;
    cma_t_alert_state	prior;
    cma_t_exit_status	status;
    cma_t_address	result;
    char		output[80];


    s = socket (AF_INET, SOCK_STREAM, 0);
    if (s == -1) {
	cts_failed (
		"Listener:  Error creating socket: %s.", 
		sys_errlist[errno]);
	cma_thread_alert (&master_thread);
        cma_thread_exit_error ();
        }

    if (bind(s, &name, sizeof(name)) == -1) {
	cts_failed (
		"Listener:  Error binding socket: %s.", 
		sys_errlist[errno]);
	cma_thread_alert (&master_thread);
        cma_thread_exit_error ();
        }

#if _CMA_UNIX_TYPE == _CMA__SVR4
    addrlen = sizeof(name);
    if (getsockname(s, (struct sockaddr *)&name, &addrlen) == -1)
	{
	perror("getsockname");
	exit(1);
	}
#endif
    if (listen (s, BACKLOG) == -1) {
	cts_failed (
		"Listener:  Error in listen: %s.", 
		sys_errlist[errno]);
	cma_thread_alert (&master_thread);
        cma_thread_exit_error ();
        }

    cma_mutex_lock (&the_mutex);
    while (!done) {
	cma_mutex_unlock (&the_mutex);

	found = cma_c_false;
	for (i = 0; i < NSERVERS; i++) {
	    cma_mutex_lock (&server_thread[i].mutex);
	    found = !server_thread[i].alive;
	    cma_mutex_unlock (&server_thread[i].mutex);

	    if (found)  break;
	    }

	if (found) {
	    TRY {
		addrlen = sizeof(addr);
		ns = accept (s, &addr, &addrlen);
		if (ns == -1) {
		    cts_failed ("Error in accept: %s.", sys_errlist[errno]);
		    cma_thread_alert (&master_thread);
		    cma_mutex_lock (&the_mutex);
		    break;
		    }
		}
	    CATCH (cma_e_alerted) {
		cma_mutex_lock (&the_mutex);
		break;
        	}
	    CATCH_ALL {
        	cts_failed ("Unexpected exception during accept.");
		cma_thread_alert (&master_thread);
		cma_mutex_lock (&the_mutex);
		break;
        	}
	    ENDTRY

	    cma_mutex_lock (&server_thread[i].mutex);
	    server_thread[i].id = s_cnt++;
	    server_thread[i].mesgs = 0;
	    server_thread[i].alive = cma_c_true;
	    server_thread[i].socket = ns;
	    cma_mutex_unlock (&server_thread[i].mutex);

	    cma_thread_create (
		    &server_thread[i].handle, 
		    &cma_c_null, 
		    server, 
		    (cma_t_address)&server_thread[i]);

	    cma_mutex_lock (&the_mutex);		/* To read 'done' */
	    }
	else {
	    cma_mutex_lock (&the_mutex);
	    TRY {
		cma_cond_wait (&server_ready, &the_mutex);
		}
	    CATCH (cma_e_alerted) {
		break;
		}
	    CATCH_ALL {
        	cts_failed ("Unexpected exception during wait for client.");
		cma_thread_alert (&master_thread);
		break;
		}
	    ENDTRY
	    }
        }
    cma_mutex_unlock (&the_mutex);

    cma_alert_disable_general (&prior);

    for (i = 0; i < NSERVERS; i++) {
        cma_mutex_lock (&server_thread[i].mutex);
        if (server_thread[i].alive) {
	    cma_mutex_unlock (&server_thread[i].mutex);

	    cma_thread_alert (&server_thread[i].handle);

	    cma_thread_join (
		    &server_thread[i].handle,
		    &status,
		    &result);

	    cma_mutex_lock (&server_thread[i].mutex);

	    sprintf (
		    output, 
		    "Listener:  Server thread #%d sent %d messages.\n", 
		    server_thread[i].id, 
		    server_thread[i].mesgs);
	    write (1, output, strlen (output));

	    if (status != cma_c_term_normal && status != cma_c_term_alert)
		cts_failed (
			"Server thread #%d terminated abnormally.", 
			server_thread[i].id); 

	    cma_thread_detach (&server_thread[i].handle);
	    }
	cma_mutex_unlock (&server_thread[i].mutex);
        }
    }


cma_t_address
talker	(
	cma_t_address	p)
    {
    int			i, c_cnt = 1;
    cma_t_boolean	found;
    cma_t_alert_state	prior;
    cma_t_exit_status	status;
    cma_t_address	result;
    char		output[80];


    cma_mutex_lock (&the_mutex);
    while (!done) {
	cma_mutex_unlock (&the_mutex);

	found = cma_c_false;
	for (i = 0; i < NCLIENTS; i++) {
	    cma_mutex_lock (&client_thread[i].mutex);
	    found = !client_thread[i].alive;
	    cma_mutex_unlock (&client_thread[i].mutex);

	    if (found)  break;
	    }

	if (found) {
	    cma_mutex_lock (&client_thread[i].mutex);
	    client_thread[i].id = c_cnt++;
	    client_thread[i].mesgs = 0;
	    client_thread[i].alive = cma_c_true;
	    cma_mutex_unlock (&client_thread[i].mutex);

	    cma_thread_create (
		    &client_thread[i].handle, 
		    &cma_c_null, 
		    client, 
		    (cma_t_address)&client_thread[i]);

	    cma_mutex_lock (&the_mutex);		/* To read 'done' */
	    }
	else {
	    cma_mutex_lock (&the_mutex);
	    TRY {
		cma_cond_wait (&client_ready, &the_mutex);
		}
	    CATCH (cma_e_alerted) {
		break;
		}
	    CATCH_ALL {
        	cts_failed ("Unexpected exception during wait for server.");
		cma_thread_alert (&master_thread);
		break;
		}
	    ENDTRY
	    }
        }
    cma_mutex_unlock (&the_mutex);

    cma_alert_disable_general (&prior);

    for (i = 0; i < NCLIENTS; i++) {
        cma_mutex_lock (&client_thread[i].mutex);
        if (client_thread[i].alive) {
	    cma_mutex_unlock (&client_thread[i].mutex);

	    cma_thread_alert (&client_thread[i].handle);

	    cma_thread_join (
		    &client_thread[i].handle,
		    &status,
		    &result);

	    cma_mutex_lock (&client_thread[i].mutex);

	    sprintf (
		    output, 
		    "Talker:  Client thread #%d sent %d messages.\n", 
		    client_thread[i].id, 
		    client_thread[i].mesgs);
	    write (1, output, strlen (output));

	    if (status != cma_c_term_normal && status != cma_c_term_alert)
	    	cts_failed (
			"Client thread #%d terminated abnormally.", 
			client_thread[i].id);

	    cma_thread_detach (&client_thread[i].handle);
	    }
	cma_mutex_unlock (&client_thread[i].mutex);
        }
    }


int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_integer	i; 
    cma_t_alert_state	prior;
    cma_t_exit_status	status;
    cma_t_address	result;


    cts_test ("CUVB_NBI_007", "Test connection-based sockets");

    cma_init();

    cma_thread_get_self (&master_thread);

    cma_mutex_create (&the_mutex,    &cma_c_null);
    cma_cond_create  (&server_ready, &cma_c_null);
    cma_cond_create  (&client_ready, &cma_c_null);

    for (i = 0; i < NSERVERS; i++) {
	server_thread[i].alive = cma_c_false;
	server_thread[i].socket = -1;
        server_thread[i].handle = cma_c_null;
	cma_mutex_create (&server_thread[i].mutex, &cma_c_null);
        }

    for (i = 0; i < NCLIENTS; i++) {
	client_thread[i].alive = cma_c_false;
	client_thread[i].socket = -1;
        client_thread[i].handle = cma_c_null;
	cma_mutex_create (&client_thread[i].mutex, &cma_c_null);
        }

    name.sin_family = AF_INET;
#if _CMA_UNIX_TYPE == _CMA__SVR4
    name.sin_port = 0;
#else
    name.sin_port = htons(7543);
#endif
    name.sin_addr.s_addr = INADDR_ANY;

    cma_thread_create (
	    &listener_thread, 
	    &cma_c_null, 
	    listener,
	    cma_c_null_ptr);

    cma_thread_create (
	    &talker_thread, 
	    &cma_c_null, 
	    talker,
	    cma_c_null_ptr);

    TRY {
	cma_delay (TEST_TIME);
        }
    CATCH (cma_e_alerted) {
        }
    CATCH_ALL {
        cts_failed ("Unexpected exception during delay.");
        }
    ENDTRY

    cma_alert_disable_general (&prior);

    cma_mutex_lock (&the_mutex);
    done = cma_c_true;
    cma_mutex_unlock (&the_mutex);

    cma_thread_alert (&talker_thread);
    cma_thread_alert (&listener_thread);

    cma_thread_join (&talker_thread, &status, &result);
    if (status != cma_c_term_normal && status != cma_c_term_alert)
	cts_failed ("Talker thread terminated abnormally."); 

    cma_thread_join (&listener_thread, &status, &result);
    if (status != cma_c_term_normal && status != cma_c_term_alert)
	cts_failed ("Listener thread terminated abnormally."); 

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_NBI_007.C */
/*  *11   29-MAY-1991 18:03:10 BUTENHOF "Fix compilation problems" */
/*  *10    1-MAR-1991 14:29:23 BUTENHOF "Tweaks" */
/*  *9     1-MAR-1991 13:40:15 BUTENHOF "Fix header ordering" */
/*  *8    20-FEB-1991 23:41:00 BUTENHOF "Fix alert control" */
/*  *7    12-FEB-1991 01:34:30 BUTENHOF "Changes to alert control" */
/*  *6    11-OCT-1990 13:04:03 BUTENHOF "Fix use of errno" */
/*  *5     4-SEP-1990 16:53:43 SCALES "Convert to new CMA interfaces" */
/*  *4    21-AUG-1990 09:47:49 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *3     5-JUL-1990 19:37:30 SCALES "" */
/*  *2     5-JUL-1990 19:34:11 SCALES "" */
/*  *1     5-JUL-1990 19:30:19 SCALES "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_NBI_007.C */
