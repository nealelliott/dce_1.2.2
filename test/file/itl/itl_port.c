/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 */
/*
 * ID: $Id: itl_port.c,v 12.3 1995/10/19 21:00:44 blake Exp $
 *
 * COMPONENT_NAME: Encina Development Infrastructure
 *
 * The following functions list may not be complete.
 * Functions defined by/via macros may not be included.
 *
 * FUNCTIONS:
 *    EXPORT   itlPort_OpenServerPort
 *    
 *    static   GetMuxSockName
 *    
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1991
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */
/*
 * HISTORY
 */

/*
 * itl_port.c -- 
 *    Unix port routines -- deal with unix sockets, etc.
 *  
 *  Original grabbed from Larry Jacobs.
 */

#include <utils/itl/itl_private.h>
#include <stdio.h>
#include <sys/param.h>   /* included for u_long, etc. types */
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

/* errno data types */
#include <errno.h>

#ifdef __NeXT__
#include <libc.h>
#endif

#include <sys/socket.h>

/* use UNIX domain sockets */
#include <sys/un.h>

#ifdef __STDC__
static int GetMuxSockName(char *prefix, char *nameP);
#else
static int GetMuxSockName();
#endif /* ansi c */


/* itlPort_OpenServerPort() -- creates a UNIX-domain stream-based port for 
 * listening and accepting connections.
 */
EXPORT int itlPort_OpenServerPort(portPrefix)
  IN char *portPrefix;
{
    int sock;   /* socket */
    struct sockaddr_un sockAddr;
    char sockName[100];

    if (GetMuxSockName(portPrefix, sockName)) {
	return -1;
    }
    
    /* remove the socket if its there*/
    unlink(sockName);

    /* now open a stream socket */
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock == -1) {
	itl_Error("OpenPort: socket command failed.\n");
	return -1;
    }

    sockAddr.sun_family = AF_UNIX;
    strcpy(sockAddr.sun_path, sockName);

    if (bind(sock, (struct sockaddr *)&sockAddr,
	     sizeof(sockAddr) ) == -1) {
	itl_Error("OpenPort: bind failed.\n");
	close(sock);
	return -1;
    }

    itl_Trace(ITL_SERVER_TRACE, "Listening on socket %s\n", sockName);
    /* listen for client connections on this port */
    listen(sock,3);
    return sock;
}

static int GetMuxSockName(prefix, nameP)
  char *prefix;
  char *nameP;
{
    /*XXX should know the length of nameP */
    struct passwd *pwEntryP, *getpwuid();
    uid_t getuid();
    char *userNameP, *getlogin();

    /* build socket name */
    strcpy(nameP, prefix);

    /* get user name */
    userNameP = getlogin();
    if (!userNameP) {
	/* oh well, lets get it from /etc/passwd */
	pwEntryP = getpwuid(getuid());
	if (!pwEntryP) {
	    fprintf(stderr, "OpenPort: cannot find userId\n");
	    return -1;
	}
	userNameP = pwEntryP->pw_name;
    }
    strcat(nameP, userNameP);
    return 0; /* success */
}
