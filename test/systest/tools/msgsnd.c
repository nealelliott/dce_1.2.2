/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: msgsnd.c,v $
 * Revision 1.1.8.2  1996/02/17  23:29:20  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:07  marty]
 *
 * Revision 1.1.8.1  1995/12/11  22:51:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:23:44  root]
 * 
 * Revision 1.1.4.2  1993/02/05  15:28:55  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:49:43  cjd]
 * 
 * Revision 1.1.2.2  1992/03/18  21:16:38  eperkins
 * 	Corrected includes
 * 	[1992/03/18  20:53:05  eperkins]
 * 
 * Revision 1.1  1992/01/19  14:34:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *
 * FILE_NAME: msgsnd.c
 *
 * COMPONENT_NAME: dce.systest
 *
 * FUNCTIONS: main()
 *
 * ORIGINS: 27
 *
 *
 * (C) COPYRIGHT International Business Machines Corp. 1991
 * All Rights Reserved
 *
 *
 * USAGE:
 *	msgsnd <msgqname> <type> <message>
 *
 *	msgqname	The pathname of the server's socket.
 *	type		The type of message to receive.  (numeric)
 *	message		The message to send. (string)
 *
 *
 * DESCRIPTION:
 *      Sends a message to the specified server of the specified type.
 *
 *	This progam is useful for sending messages within a shell script 
 *	program.
 *
 *
 * HISTORY: 
 * 06/24/91	Wayne Mock	Initial design/code
 * 08/30/91	Wayne Mock	Updated the documentation.
 */

#include <msgq.h>

main( argc, argv )
int argc;
char* argv[];
{	int rc;
	STRMSG msg;

	if( argc != 4 )
	{	msgfError( "usage: msgsnd <msgqname> <type> <message>", 0 );
	}

	msg.type = atol( argv[2] );
	strcpy( msg.buf, argv[3] );

	rc = msgsnd_bsd( argv[1], (struct message*)&msg, sizeof( msg ), 0 );
	if( rc != sizeof( msg ) )
	{	msgfError( "msgsnd: msgsnd_bsd", errno );
	}

	exit( 0 );
}

