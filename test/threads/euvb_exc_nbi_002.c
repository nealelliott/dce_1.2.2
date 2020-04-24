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
 * $Log: euvb_exc_nbi_002.c,v $
 * Revision 1.2.78.2  1996/02/17  23:31:24  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:55  marty]
 *
 * Revision 1.2.78.1  1995/12/11  23:02:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:11  root]
 * 
 * Revision 1.2.76.1  1994/04/01  20:11:22  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:18:09  jd]
 * 
 * Revision 1.2.74.1  1993/10/12  19:37:03  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:46  rhw]
 * 
 * Revision 1.2.6.2  1993/06/24  20:49:49  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:42:26  hinman]
 * 
 * Revision 1.2.3.3  1992/12/30  16:52:49  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:11:03  zeliff]
 * 
 * Revision 1.2.3.2  1992/09/29  20:39:52  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:34:27  sekhar]
 * 
 * Revision 1.2  1992/01/19  22:14:44  devrcs
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
 *	EUVB_EXC_NBI_002
 *
 *  TEST TOPICS:
 *
 *	EXC  -  Exceptions
 *	NBI  -  Non-Blocking (Unix) I/O
 *
 *  ABSTRACT:
 *
 *	User-supplied sample program from QAR 146 in CMA database;
 *	test use of SIGPIPE in detecting write to broken pipe.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	06 November 1991
 *
 *  MODIFICATION HISTORY:
 *
 */

/*
 *  INCLUDE FILES
 */

#include <pthread_exc.h>
#include <dce/cts_test.h>
#include <sys/socket.h>
#include <sys/un.h>

static pthread_addr_t	client  (pthread_addr_t);
static pthread_addr_t	server  (pthread_addr_t);

static pthread_t	server_tid;
static pthread_t	client_tid;

#if (_CMA_UNIX_TYPE == _CMA__SVR4)
static char			*sockpath;
#endif

int
main	(int argc, char *argv[], char *envp[])
    {
    cts_test ("EUVB_EXC_NBI_002", "full test of SIGPIPE and write");

#if (_CMA_UNIX_TYPE == _CMA__SVR4)
	/*
	*	Generate socket name
	*/
	if ( (sockpath = tempnam( "/tmp", "cma" )) == (char *)NULL )
		{
		perror("tempnam(3) failed");
		exit(-1);
		}
#endif

    /*
     * create thread to listen for and accept the connection
     */
    pthread_create (
	    &server_tid,
	    pthread_attr_default,
	    server,
	    (pthread_addr_t)0);

    /*
     * create thread to initiate, establish, and then break the connection
     */
    pthread_create (
	    &client_tid,
	    pthread_attr_default,
	    client,
	    (pthread_addr_t)0);

    /*
     * wait for the client and server threads to finish
     */
    pthread_join (server_tid, (pthread_addr_t *)0);
    pthread_join (client_tid, (pthread_addr_t *)0);

    cts_result ();
    }

static pthread_addr_t
server	(pthread_addr_t	not_used)
    {
    int lsock,sock;
    struct sockaddr_un name;
    int bufr;
    int		epipe = 0;


    /*
     * create a socket for listening
     */
    if ((lsock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
	perror("server: socket(2) failed");
	exit(-1);
	}

    /*
     * setup for bind(2)
     */
    name.sun_family = AF_UNIX;

#if (_CMA_UNIX_TYPE == _CMA__SVR4)
    strcpy(name.sun_path,sockpath);
#else
    strcpy(name.sun_path,"/usr/tmp/test.sock");
#endif

    /*
     * clean-up from the previous invocation, if any
     */
    if ((unlink(name.sun_path) < 0) && (errno != ENOENT)) {
	perror("server: unlink(2) failed");
	exit(-1);
	}

    /*
     * bind name to socket
     */
    if (bind(lsock, &name, sizeof(name)) < 0) {
	perror("server: bind(2) failed");
	exit(-1);
	}

    /*
     * start listening
     */
    if (listen(lsock, 1) < 0) {
	perror("server: listen(2) failed");
	exit(-1);
	}

    /*
     * wait for a connection to come-in
     */
    if ((sock = accept(lsock, 0, 0)) < 0) {
	perror("server: accept(2) failed");
	exit(-1);
	}

    cts_comment ("server: connection accepted (fd=%d).",sock);
	
    /*
     * have a connection; now wait for it to be broken
     */
    switch (recv(sock, &bufr, sizeof(bufr), 0)) {
	case -1:
#if _CMA_UNIX_TYPE == _CMA__SVR4
# if (_CMA_PLATFORM_ == _CMA__DCOSX_MIPS)
 	    if (errno != ECONNABORTED) 
			{
 		    perror("server: recv(2) failed");
 		    cts_failed ("unexpected server error");
 		    exit(-1);
			}
# else
 	    if (errno != ECONNRESET) 
			{
 		    perror("server: recv(2) failed");
 		    cts_failed ("unexpected server error");
 		    exit(-1);
			}
# endif
#else	/* _CMA_UNIX_TYPE == _CMA___SVR4 */
	    perror("server: recv(2) failed");
	    cts_failed ("unexpected server error");
	    exit(-1);
#endif

	case 0:
	    cts_comment ("server: connection terminated.");
	    break;

	default:
	    cts_failed ("server: something is wrong...recv(2) succeeded!");
	    exit(-1);
	}
	
    cts_comment ("server: sending over the broken connection.\n");

    /*
     * send over the broken connection to get the SIGPIPE exception
     */
    TRY {
	send (sock, &bufr, sizeof(bufr), 0);
	}
    CATCH (exc_SIGPIPE_e) {
	epipe = 1;
	cts_comment ("server: SIGPIPE exception caught as expected.");
	}
    CATCH_ALL {
	cts_failed ("server: unexpected exception");
	exit(-1);
	}
    ENDTRY

    if (epipe == 0) {
	cts_failed ("server: SIGPIPE exception was not raised.");
	exit(-1);
	}
	
    /*
     *	Now attempt to close on the broken connection.
     */
    cts_comment ("server: calling close(2) to close the connection.");

    if (close(sock) < 0) {
	cts_failed ("server: close(2) didn't hang, but failed.");
	exit(-1);
	}

    cts_comment ("server: returned successfully from close(2).");
    }

static pthread_addr_t
client	(pthread_addr_t	not_used)
    {
    int			sock;
    struct sockaddr_un	name;
    struct timespec	delay_ts;


    /*
     * create a socket for connecting
     */
    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
	perror("client: socket(2) failed");
	cts_failed ("client socket creation failed");
	exit(-1);
	}

    /*
     * setup for connect(2)
     */
    name.sun_family = AF_UNIX;
#if (_CMA_UNIX_TYPE == _CMA__SVR4)
    strcpy (name.sun_path, sockpath);
#else
    strcpy (name.sun_path, "/usr/tmp/test.sock");
#endif

    /*
     * make the connection
     */
    while (connect(sock, &name, sizeof(name)) < 0) {
	switch (errno) {
	    case ENOENT:
	    case ETIMEDOUT:
	    case ECONNREFUSED:
		continue;
	    default:
		perror("client: connect(2) call failed");
		cts_failed ("client connect failed");
		exit(-1);
	    }

	}
	
    /*
     * connection established; break it then!
     */
    cts_comment ("client: connection initiated (fd=%d).",sock);

    delay_ts.tv_sec = 2;
    delay_ts.tv_nsec = 0;
    pthread_delay_np (&delay_ts);	/* pause for a bit */

    if (close(sock) < 0) {
	perror("client: close(2) failed");
	cts_failed ("client close failed");
    	exit(-1);
	}
    else
	cts_comment ("client closed connection");

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element EUVB_EXC_NBI_002.C */
/*  *1     6-NOV-1991 11:56:34 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element EUVB_EXC_NBI_002.C */
