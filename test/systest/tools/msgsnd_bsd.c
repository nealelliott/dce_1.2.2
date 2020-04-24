/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: msgsnd_bsd.c,v $
 * Revision 1.1.9.2  1996/02/17  23:29:21  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:08  marty]
 *
 * Revision 1.1.9.1  1995/12/11  22:51:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:23:44  root]
 * 
 * Revision 1.1.4.2  1993/02/05  15:29:01  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:49:48  cjd]
 * 
 * Revision 1.1.2.2  1992/03/18  21:16:46  eperkins
 * 	Corrected includes
 * 	[1992/03/18  20:53:10  eperkins]
 * 
 * Revision 1.1  1992/01/19  14:34:27  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *
 * FILE_NAME: msgsnd_bsd.c
 *
 * COMPONENT_NAME: dce.systest
 *
 * FUNCTIONS:	msgsnd_bsd()
 *
 * ORIGINS: 27
 *
 *
 * (C) COPYRIGHT International Business Machines Corp. 1991
 * All Rights Reserved
 *
 *
 * DESCRIPTION:
 *      The API to send messages to a message queue server.
 *
 * HISTORY: 
 * 06/24/91	Wayne Mock	Initial design/code
 * 08/30/91	Wayne Mock	Updated the documentation.
 */


#include <msgq.h>

/*****************************************************************************
 * msgsnd_bsd()
 *
 * DESCRIPTION
 *	Sends a message of a give type to a specified message queue server.
 *
 * ASSUMPTIONS
 *	none.
 *
 * INPUT PARAMETERS
 *	char* name		The server's socket name.
 *	struct message* msg	The message to send.
 *	int usize		The size of the message in bytes.
 *	int flags		Not yet implemented.
 *
 * OUTPUT PARAMETERS
 *	none.
 *
 * RETURN VALUE
 *	If successful, returns the number of bytes sent, otherwise -1.
 *
 * SIDE EFFECTS
 *	none.
 *
 ****************************************************************************/
int
msgsnd_bsd( name, msg, usize, flags )
char* name;
struct message* msg;
int usize;
int flags;
{	int sockfd, svLen, rc, cnt;
	struct sockaddr_un svAddr;
	struct imessage imsgbuf;

	sockfd = socket( AF_UNIX, SOCK_STREAM, 0 );
	if( sockfd < 0 )
	{	return sockfd;	
	}

	svAddr.sun_family = AF_UNIX;
	strcpy( svAddr.sun_path, name );
	svLen = strlen( svAddr.sun_path ) + sizeof( svAddr.sun_family );

	for( cnt = 1;
	     ((rc = connect( sockfd, (struct sockaddr*)&svAddr, svLen )) < 0 );
	     cnt++ )
	{	if( cnt == MSGQ_TIMEOUT )
		{	return rc;
		}
	}

	imsgbuf.action = MSGSND;
	imsgbuf.usize = usize; 
	imsgbuf.flags = flags;
	bcopy( msg, &imsgbuf.umsg, usize );

	rc = send( sockfd, (char*)&imsgbuf, IMSGSIZLESSBUF + usize, 0 );

	close( sockfd );

	return ( rc == IMSGSIZLESSBUF + usize ) ? usize : -1;

} /* end msgsnd_bsd() */
