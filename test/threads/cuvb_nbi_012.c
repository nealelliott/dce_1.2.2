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
 * $Log: cuvb_nbi_012.c,v $
 * Revision 1.1.50.2  1996/02/17  23:31:06  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:39  marty]
 *
 * Revision 1.1.50.1  1995/12/11  23:01:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:56  root]
 * 
 * Revision 1.1.48.1  1994/04/01  20:10:58  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:17:14  jd]
 * 
 * Revision 1.1.46.1  1993/10/12  19:36:38  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:34  rhw]
 * 
 * Revision 1.1.3.3  1992/12/30  16:51:33  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:09:53  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  20:39:33  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:33:29  sekhar]
 * 
 * $EndLog$
 */

/*
 *  FACILITY:
 *
 *	CMA Test Programs
 *
 *  FILENAME:
 *
 *	CUVB_NBI_012
 *
 *  TEST TOPICS:
 *
 *	NBI  -  Non-Blocking (Unix) I/O
 *
 *  ABSTRACT:
 *
 *  This program tests that the getsockname(2) call correctly returns
 *  an error when a client tries to use it on a connection which
 *	has been broken by the server.
 *
 *  AUTHORS:
 *		Benjamin Lurie
 *
 *  CREATION DATE:
 *
 *	30 July 1992 - Modified from cuvb_nbi_011.c.
 *
 *  MODIFICATION HISTORY:
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

extern char			*sys_errlist[];

/*
*	Global Data.
*/

cma_t_thread		server_thread, client_thread;
struct sockaddr_in	global_socket;

cma_t_address
client	(
	cma_t_address	p)
{
struct sockaddr		tmp_socket;
int					socketlen;
int					s;
cma_t_exit_status	status;
cma_t_address		result;

    if ( (s = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
		{
		cts_failed ("client:  Error creating socket: %s.", sys_errlist[errno]);
        cma_thread_exit_error ();
        }

	while (1)
		{
		if (connect(s, 
				(struct sockaddr *)&global_socket, 
				sizeof(global_socket)) == -1) 
			{
			if ( errno == ETIMEDOUT )
				{
				printf("client: could not connect to server, retrying...\n");
				continue;
				}
			cts_failed ("client:  Error connecting socket: %s.", 
						sys_errlist[errno]);
			cma_thread_exit_error ();
			}
		else
			{
			break;
			}
		}

	/*
	*	We are connected now.
	*
	*	Join with server thread, which will enable us to make sure
	*	that the connection has been closed.
	*/

    cma_thread_join (&server_thread, &status, &result);
    if (status != cma_c_term_normal && status != cma_c_term_alert)
		{
		cts_failed ("Server thread terminated abnormally."); 
		cma_thread_exit_error();
		}

	cma_delay(10.0);

	/*
	*	Call getsockname(2), which should return an error.
	*/

    socketlen = sizeof(tmp_socket);
    if (getsockname(s, (struct sockaddr *)&tmp_socket, &socketlen) != -1)
		{
		cts_failed("client:  Did not receive error on getsockname(2) call.");
		}

    return cma_c_null_ptr;
}	/* client() */


cma_t_address
server	(
	cma_t_address	p)
{
int				s, ns;
unsigned int	addrlen;

    global_socket.sin_family = AF_INET;
    global_socket.sin_port = 0;
    global_socket.sin_addr.s_addr = INADDR_ANY;

    if ( (s = socket (AF_INET, SOCK_STREAM, 0)) == -1 )
		{
		cts_failed("server:  Error creating socket: %s.", sys_errlist[errno]);
        cma_thread_exit_error ();
        }

    if (bind(s, &global_socket, sizeof(global_socket)) == -1) 
		{
		cts_failed("server:  Error binding socket: %s.", sys_errlist[errno]);
        cma_thread_exit_error ();
        }

    addrlen = sizeof(global_socket);
    if (getsockname(s, (struct sockaddr *)&global_socket, &addrlen) == -1)
		{
		cts_failed("server:  Error getting socket name: %s.", 
					sys_errlist[errno]);
		cma_thread_exit_error();
		}

	/*
	*	At this point the socket is all set up so 
	*	the client can bind to us.
	*	
	*	Start the client now.
	*/

    cma_thread_create (
	    &client_thread, 
	    &cma_c_null, 
	    client,
	    cma_c_null_ptr);

    if (listen (s, 5) == -1) 
		{
		cts_failed("server:  Error in listen: %s.", sys_errlist[errno]);
        cma_thread_exit_error ();
        }

	addrlen = sizeof(global_socket);
	if ( (ns = accept (s, &global_socket, &addrlen)) == -1 )
		{
		cts_failed ("server: Error in accept: %s.", sys_errlist[errno]);
        cma_thread_exit_error ();
		}

	/*
	*	The client has connected now, close the connection.
	*/

	if (close(ns) == -1 )
		{
		cts_failed("server: Error closing socket: %s.", sys_errlist[errno]);
        cma_thread_exit_error ();
		}

    return cma_c_null_ptr;
}	/* server() */

int
main	(int argc, char *argv[], char *envp[])
    {
cma_t_exit_status	status;
cma_t_address		result;


    cts_test ("CUVB_NBI_012", "Test getsockname(2) call");
    cma_init();

    cma_thread_create (
	    &server_thread, 
	    &cma_c_null, 
	    server,
	    cma_c_null_ptr);

    cma_thread_join (&server_thread, &status, &result);
    if (status != cma_c_term_normal && status != cma_c_term_alert)
		{
		cts_failed ("Server thread terminated abnormally."); 
		exit(-1);
		}

    cma_thread_join (&client_thread, &status, &result);
    if (status != cma_c_term_normal && status != cma_c_term_alert)
		{
		cts_failed ("Client thread terminated abnormally."); 
		}
    cts_result ();
}	/* main() */
