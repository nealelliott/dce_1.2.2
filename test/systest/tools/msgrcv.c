/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: msgrcv.c,v $
 * Revision 1.1.8.2  1996/02/17  23:29:18  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:05  marty]
 *
 * Revision 1.1.8.1  1995/12/11  22:51:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:23:42  root]
 * 
 * Revision 1.1.4.2  1993/02/05  15:28:42  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:49:34  cjd]
 * 
 * Revision 1.1.2.2  1992/03/18  21:14:51  eperkins
 * 	Corrected includes
 * 	[1992/03/18  20:52:56  eperkins]
 * 
 * Revision 1.1  1992/01/19  14:34:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *
 * FILE_NAME: msgrcv.c
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
 *	msgrcv <msgqname> <type>
 *
 *	msgqname	The pathname of the server's socket.
 *	type		The type of message to receive.  (numeric)
 *
 *
 * DESCRIPTION:
 *      Receives a message from the specified server of the specified type.
 *	The process calling this program blocks until the message is
 *	available.
 *
 *	This progam is useful for receiveing messages within a shell script 
 *	program.
 *
 * HISTORY: 
 * 06/24/91	Wayne Mock	Initial design/code
 * 08/30/91	Wayne Mock	Updated the documentation.
 * 11/06/91	Wayne Mock	Added the -n, no wait, option.
 */

#include <msgq.h>

main( argc, argv )
int argc;
char* argv[];
{	long type;
	int rc;
	STRMSG msg;
	int flag;
	char* name;

	if( argc == 3 )
	{	flag = ~IPC_NOWAIT;
		name = argv[1];
		type = atol( argv[2] );
	}
	else
	if( argc == 4 && strcmp( argv[1], "-n" ) == 0 )
	{	flag = IPC_NOWAIT;
		name = argv[2];
		type = atol( argv[3] );
	}
	else
	{	msgfError( "usage: msgrcv [-n] <msgqname> <type>", 0 );
	}


	rc = msgrcv_bsd( name, (struct message*)&msg, 
					sizeof( msg ), type, flag);
	if( rc != sizeof( msg ) )
	{	if( errno == ENOMSG )
		{	fprintf( stderr, "msgrcv: No message to receive.\n" );
			exit( 1 );
		}
		else
		{	msgfError( "msgrcv: msgrcv_bsd", errno );
		}
	}

	printf( "%s\n", msg.buf );

	exit( 0 );
}

