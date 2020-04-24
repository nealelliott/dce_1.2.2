/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_client.c,v $
 * Revision 1.1.416.1  1996/10/17  18:27:02  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:34  damon]
 *
 * Revision 1.1.411.1  1994/02/04  20:45:05  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:24  devsrc]
 * 
 * Revision 1.1.409.1  1993/12/07  17:44:24  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:25:25  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/11  18:30:30  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:09:04  htf]
 * 
 * Revision 1.1.2.2  1992/09/25  17:14:26  jaffe
 * 	Transarc delta: fred-checkin-itl 1.1
 * 	  Selected comments:
 * 	    Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	    Initial checkin.
 * 	[1992/09/23  21:05:41  jaffe]
 * 
 * $EndLog$
 */
/*
 * ID: $Id: itl_client.c,v 1.1.416.1 1996/10/17 18:27:02 damon Exp $
 *
 * COMPONENT_NAME: Encina Development Infrastructure
 *
 * The following functions list may not be complete.
 * Functions defined by/via macros may not be included.
 *
 * FUNCTIONS:
 *    extern   OpenClientPort, main
 *    
 *    static   GetMuxSockName, ProcessTtyData, ProcessSockData
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
 * $TALog: itl_client.c,v $
 * Revision 1.5  1994/11/01  22:29:11  cfe
 * Bring over the changes that the OSF made in going from their DCE 1.0.3
 * release to their DCE 1.1 release.
 * [from r1.4 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 *
 * Revision 1.4  1993/01/14  17:37:58  jaffe
 * revdrop of latest OSF copyright changes to these files
 * [from r1.3 by delta osf-revdrop-01-12-92, r1.1]
 *
 * Revision 1.1  1992/08/10  20:10:33  fred
 * Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * Initial checkin.
 *
 * Initial checkin.
 * [added by delta fred-checkin-itl, revision 1.1]
 *
 * Revision 1.4  1992/03/26  12:53:29  jaffe
 * Prologue Changes for 1.0.1
 *
 *
 * [from revision 1.3 by delta jaffe-ttools-prologue, revision 1.1]
 *
 * Revision 1.3  1991/10/09  13:05:06  oz
 * 	Renamed a debug flag to a more polite name.
 *
 * Revision 1.2  91/09/27  14:12:15  oz
 * 	Added an aditional third parameter: optional user name.
 * 
 * Revision 1.1  91/08/22  17:19:44  mss
 * 	Modified REC tester to make separate library
 * 
 * Revision 1.1  91/07/10  16:11:45  oz
 * 	Initial checkin - second try.
 */

/*
 * rt_client.c -- 
 *    A small client program that can talk to the rec_test
 *    server module -- it receives its own thread from the 
 *    server and starts running.
 */

#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/socket.h>

/* use UNIX domain sockets */
#include <sys/un.h>

#ifdef __NeXT__
#include <libc.h>
#endif /* next */

#ifdef _AIX
#include <sys/select.h>
#endif  /* _AIX */

#include <utils/itl/itl_private.h>


#define DEBUG_SLAVE   0
#define DEBUG_REWR    0


#ifdef __STDC__
static int ProcessTtyData(int out);
static int ProcessSockData(int sock);
#else
static int ProcessTtyData();
static int ProcessSockData();
#endif /* ansi c */

#define RT_SERVER_PREFIX "/tmp/rEC_tESTER_"

/* OpenClientPort() -- opens a UNIX-domain stream-based socket to
 * communicate with a Mux server.
 */



static int GetMuxSockName(prefix, nameP, userName)
  char *prefix;
  char *nameP;
  char *userName;
{
    /*XXX should know the length of nameP */
    struct passwd *pwEntryP, *getpwuid();
    char *userNameP, *getlogin();

    /* build socket name */
    strcpy(nameP, prefix);

    /* get user name */
    if (userName)
	userNameP = userName;
    else {
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
    }
    strcat(nameP, userNameP);
    return 0; /* success */
}


int OpenClientPort(prefix, userName)
  char *prefix;
  char *userName;
{
    int sock;   /* socket */
    struct sockaddr_un sockAddr;
    char sockName[100];

    if (GetMuxSockName(prefix, sockName, userName)) {
	return -1;
    }
    
    /* now open a stream socket */
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock == -1) {
	perror("OpenPort: socket");
	return -1;
    }

    sockAddr.sun_family = AF_UNIX;
    strcpy(sockAddr.sun_path, sockName);

#if DEBUG_STUFF
    printf("Connecting to socket %s\n", sockName);
#endif
    if (connect(sock, (struct sockaddr *)&sockAddr,
	     sizeof(sockAddr) ) == -1) {
	perror("OpenPort: connect");
	close(sock);
	return -1;
    }

    return sock;
}

int main(argc, argv)
  int argc;
  char *argv[];
{
    int sock, w;
    unsigned char winSize[2];
    char * tempPortPrefix;

    fd_set readFds, writeFds, exceptFds;

    if (argc < 2) {
	fprintf(stderr, "usage: %s <server prefix> [user]\n", argv[0]);
	exit(1);
    }
	
    tempPortPrefix = (char *) malloc(strlen(ITL_SERVER_PREFIX) +
				     strlen(argv[1]) + 1);
    sprintf(tempPortPrefix, ITL_SERVER_PREFIX_SPRINTF, argv[1]);


    if (argc == 2)
	sock = OpenClientPort(tempPortPrefix, NULL);
    else if (argc == 3)
	sock = OpenClientPort(tempPortPrefix, argv[2]);
    else {
	fprintf(stderr, "usage: %s <server prefix> [user]\n", argv[0]);
	exit(1);
    }
	
    if (sock < 0) {
	exit(2);
    }

    free(tempPortPrefix);


    /* place the tty in raw mode */
    /* PtySetRaw(0);  */

    /* clear the write and exception masks */
    FD_ZERO(&writeFds);  FD_ZERO(&exceptFds);

    while (1) {
	int ret;
	FD_ZERO(&readFds);
	FD_SET(sock, &readFds);
	FD_SET(0, &readFds);

#if DEBUG_REWR
	printf("Work: about to select\n");
#endif
	ret = select((sock+1), &readFds, &writeFds, &exceptFds, NULL);
	if (ret == -1) {
	    perror("work: select");
	    return -1;
	}

#if DEBUG_REWR
	printf("Work: select returned\n");
#endif
	if (FD_ISSET(sock, &readFds)){
	    ret = ProcessSockData(sock);
	    if (ret == -1) exit(4);
	}
	if (FD_ISSET(0, &readFds)) {
	    ret = ProcessTtyData(sock);
	    if (ret == -1) exit(5);
	}
    } /* while */
}	


static int ProcessTtyData(out)
  int out;
{
    int r,w;
    char buffer[1024];

    r = read(0, buffer, sizeof(buffer));
    if (r < 1) {
	fprintf(stderr," ProcessTtyData read returned: %d\n", r);
	perror("read");
	exit(3);
    }

#if DEBUG_REWR
    buffer[r] = '\0';
    fprintf(stderr,"ProcessTtyData: read %d bytes \"%s\"\n",r, buffer);
#endif
    
    w = write(out, buffer, r);
    if (w != r) {
	fprintf(stderr," ProcessTtyData write returned: %d after reading %d\n",
		w, r);
	perror("write");
	exit(3);
    }

    return 0;
}
    

static int ProcessSockData(sock)
  int sock;
{
    int r,w;
    char buffer[1024];

    r = read(sock, buffer, sizeof(buffer));
    if (r < 1) {
	if (r == 0) {
	    fprintf(stderr,"Connection closed.  Bye!\n");
	    return -1;
	}

	fprintf(stderr," ProcessSockData read returned: %d\n", r);
	perror("read");
	exit(3);
    }

#if DEBUG_REWR
    buffer[r] = '\0';
    fprintf(stderr,"ProcessSockData: read %d bytes \"%s\"\n",r, buffer);
#endif
    
    w = write(1, buffer, r);
    if (w != r) {
	fprintf(stderr,
		" ProcessSockData write returned: %d after reading %d\n",
		w, r);
	perror("write");
	exit(3);
    }

    return 0;
}
    
