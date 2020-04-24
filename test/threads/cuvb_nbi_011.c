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
 * $Log: cuvb_nbi_011.c,v $
 * Revision 1.1.9.1  1996/06/04  22:14:21  arvind
 * 	Remove stropts.h
 * 	[1996/03/08  22:22 UTC  bissen  /main/DCE_1.2/1]
 *
 * Revision 1.1.7.2  1996/02/17  23:31:05  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:38  marty]
 * 
 * Revision 1.1.7.1  1995/12/11  23:01:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:56  root]
 * 
 * Revision 1.1.5.1  1994/04/01  20:10:54  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:17:10  jd]
 * 
 * Revision 1.1.3.3  1992/12/30  16:51:31  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:09:49  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  20:39:24  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:33:24  sekhar]
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
 *	CUVB_NBI_011
 *
 *  TEST TOPICS:
 *
 *	NBI  -  Non-Blocking (Unix) I/O
 *
 *  ABSTRACT:
 *
 *  This program tests that the connect wrapper correctly returns
 *  an error when a client tries to connect(2) to a UNIX domain
 *  socket on which no one is listen(2)ing.  The thread should not
 *  block and an error should be returned.
 *
 *  AUTHORS:
 *
 *  CREATION DATE:
 *
 *	20 July 1992 - Modified from cuvb_nbi_010.c.
 *
 *  MODIFICATION HISTORY:
 */


/*
 *  INCLUDE FILES 
 */

#include <cma.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>

main	(int argc, char *argv[], char *envp[])
{
int					status;
struct sockaddr_un	sockaddr;
register int		sd;
char				*pname;


    cma_init ();
    cts_test ("CUVB_NBI_011", "Test connect wrapper");

	/*
	*	Generate socket name
	*/
	if ( (pname = tempnam( "/tmp", "cma" )) == (char *)NULL )
		{
		perror("tempnam");
		cts_failed("tempnam failed");
		exit(-1);
		}

	/*
	*	Just make sure that there is no other file there.
	*/

	status = unlink(pname);
	if ( (status == -1) && (errno != ENOENT) )
		{
		perror("unlink");
		cts_failed("unlink failed.");
		exit(-1);
		}

	/*
	*	Create the socket.
	*/

	memset( (char *)&sockaddr, '\0', sizeof(sockaddr));
  	sockaddr.sun_family = AF_UNIX;
  	strcpy(sockaddr.sun_path, pname );
	if ((sd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1 )
		{
		perror("create: socket");
		cts_failed("socket(2) failed.");
		exit(-1);
		}

	if (bind(sd, &sockaddr, sizeof (sockaddr))== -1)
		{
		perror("bind");
		if (close(sd) == -1 )
			{
			perror("close");
			cts_failed("close(2) failed.");
			}
		cts_failed("bind(2) failed.");
		unlink(pname);
		exit(-1);
		}

	/*
	*	Close the socket now.
	*/
	if (close(sd) == -1 )
		{
		perror("close");
		cts_failed("close(2) failed.");
		unlink(pname);
		exit(-1);
		}

	/*
	*	Try to connect(2) to the socket as a client would.
	*/
	if ((sd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1 ) 
		{
		perror("socket");
		cts_failed("socket(2) call failed");
		unlink(pname);
		exit(-1);
		}

	status = connect(sd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
	if ( status == -1 )
		{
		perror("connect");
		if ( errno != ECONNREFUSED )
			{
			cts_failed("Did not receive errno '%d' from connect.", errno);
			}
		}
	else
		{
		cts_failed("Did not receive error on connect(2).");
		}

	/*
	*	Remove the socket.
	*/

	if ( (status = unlink(pname)) == -1 )
		{
		perror("unlink");
		cts_failed("Error on unlink of socket.");
		}
    cts_result ();
} /* main() */
