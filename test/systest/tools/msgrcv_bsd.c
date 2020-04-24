/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: msgrcv_bsd.c,v $
 * Revision 1.1.9.2  1996/02/17  23:29:19  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:06  marty]
 *
 * Revision 1.1.9.1  1995/12/11  22:51:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:23:43  root]
 * 
 * Revision 1.1.4.2  1993/02/05  15:28:50  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:49:38  cjd]
 * 
 * Revision 1.1.2.2  1992/03/18  21:14:57  eperkins
 * 	Corrected includes
 * 	[1992/03/18  20:53:00  eperkins]
 * 
 * Revision 1.1  1992/01/19  14:34:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *
 * FILE_NAME: msgrcv_bsd.c
 *
 * COMPONENT_NAME: dce.systest
 *
 * FUNCTIONS:	msgrcv_bsd()
 *
 * ORIGINS: 27
 *
 *
 * (C) COPYRIGHT International Business Machines Corp. 1991
 * All Rights Reserved
 *
 *
 * DESCRIPTION:
 *      The API to receive messages from a message queue server.
 *
 * HISTORY: 
 * 06/24/91	Wayne Mock	Initial design/code
 * 08/30/91	Wayne Mock	Updated the documentation.
 * 11/06/91	Wayne Mock	Added the non-blocking functionality.
 */


#include <msgq.h>

/*****************************************************************************
 * msgrcv_bsd()
 *
 * DESCRIPTION
 *	Receievs a message of the given type and places the results in the
 *	given buffer.
 *
 * ASSUMPTIONS
 *	A server is running at the given name.
 *
 * INPUT PARAMETERS
 *	char* name	Name of the server's socket.
 *	int usize	The size of message to receive.
 *	long type	The type of message to receive.
 *	int flags	Not yet implemented.
 *
 * OUTPUT PARAMETERS
 *	struct message* msg	The buffer containing the received message.
 *
 * RETURN VALUE
 *	If successful, it returns the number of types read; otherwise, it
 *	returns -1.
 *
 * SIDE EFFECTS
 *	none.
 *
 ****************************************************************************/
int
msgrcv_bsd( name, msg, usize, type, flags )
char* name;
struct message* msg;
int usize;
long type;
int flags;
{	int sockfd, svLen, rc, cnt;
	struct sockaddr_un svAddr;
 	struct imessage imsgbuf;
/*
 * open a socket of communication.
 */
	sockfd = socket( AF_UNIX, SOCK_STREAM, 0 );
	if( sockfd < 0 )
	{	return sockfd;	
	}

/*
 * attempt to connect to the socket MSGQ_TIMEOUT times.
 */
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

/*
 * convert the user's messge to the internal representation
 */
	imsgbuf.action = MSGRCV;
	imsgbuf.usize = usize;
	imsgbuf.flags = flags;
	imsgbuf.umsg.type = type;

/*
 * write the request to the server, asking it to reveice a message.
 */
	rc = send( sockfd, (char*)&imsgbuf, IMSGSIZLESSBUF, 0 );
	if( rc != IMSGSIZLESSBUF )
	{	return rc;
	}
/*
 * wait for the server to respond.
 */
	rc = recv( sockfd, (char*)&imsgbuf, IMSGSIZLESSBUF + usize, 0 );
	close( sockfd );

/*
 * If the server set the imsgbuf.usize to -1 then the user requested
 * IPC_NOWAIT and there was no message to get.  Force errno = ENOMSG and
 * return with a -1.
 */
	if( imsgbuf.usize == -1 )
	{	errno = ENOMSG;
		return -1;
	}
	else	/* should have receive a message */
	{	bcopy( &imsgbuf.umsg, msg, usize );
		return ( rc == IMSGSIZLESSBUF + usize ) ? usize : -1;
	}

} /* end msgrcv() */



