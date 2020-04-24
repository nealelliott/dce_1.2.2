/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_server.c,v $
 * Revision 1.1.418.1  1996/10/17  18:27:39  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:52  damon]
 *
 * Revision 1.1.413.1  1994/02/04  20:45:38  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:40  devsrc]
 * 
 * Revision 1.1.411.1  1993/12/07  17:44:51  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:29:17  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/11  18:31:24  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:10:53  htf]
 * 
 * Revision 1.1.2.2  1992/09/25  17:23:13  jaffe
 * 	Transarc delta: fred-checkin-itl 1.1
 * 	 Selected comments:
 * 	   Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	   Initial checkin.
 * 	[1992/09/23  21:29:19  jaffe]
 * 
 * $EndLog$
 */
/*
 * ID: $Id: itl_server.c,v 1.1.418.1 1996/10/17 18:27:39 damon Exp $
 *
 * COMPONENT_NAME: Encina Development Infrastructure
 *
 * The following functions list may not be complete.
 * Functions defined by/via macros may not be included.
 *
 * FUNCTIONS:
 *    EXPORT   itlServer_Start
 *    
 *    PRIVATE  serverListen
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
 * $TALog: itl_server.c,v $
 * Revision 1.5  1994/11/01  22:29:58  cfe
 * Bring over the changes that the OSF made in going from their DCE 1.0.3
 * release to their DCE 1.1 release.
 * [from r1.4 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 *
 * Revision 1.4  1993/01/14  17:38:37  jaffe
 * revdrop of latest OSF copyright changes to these files
 * [from r1.3 by delta osf-revdrop-01-12-92, r1.1]
 *
 * Revision 1.1  1992/08/10  20:11:26  fred
 * Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * Initial checkin.
 *
 * Initial checkin.
 * [added by delta fred-checkin-itl, revision 1.1]
 *
 * Revision 1.9  1992/03/27  21:15:55  oz
 * Changes which were made before 1.0.1 and were not supposed to be
 * incorporated in 1.0.1 and therefore were not readied and were
 * not moved to CML with the rest of the file.
 *
 * These changes add tranC support for ITL as well as handling
 * transaction aborts (exceptions) in the lower level io modules.
 *
 * See General Comment Above
 * [from revision 1.8 by delta oz-2172-itl-support-tranc, revision 1.1]
 *
 * Revision 1.6  1992/01/20  08:30:54  oz
 * 	 - using:
 * 		itl_Trace	instead of itl_TracePrintf
 * 	 	itl_Error	instead of itl_PrintErrMsg
 * 	[92/01/18  13:12:52  oz]
 *
 * Revision 1.5  92/01/08  15:30:50  oz
 * 	 - If serverListen return an error, do not create a thread.
 * 	   simply continue.
 * 	 - Keep track of the number of errors which occurred while listening
 * 	   on the socket and give up after 10.
 * 	[92/01/06  09:02:54  oz]
 * 
 * Revision 1.4  91/10/02  11:50:51  oz
 * 	Renamed serverRoutine to be itlServer_Start
 * 
 * Revision 1.3  91/09/16  12:00:45  mss
 * 	Renamed
 * 	 tracePrintf =>  itl_TracePrintf
 * 	 currentOutput => itl_CurrentOutput
 * 	 printErrMsg => itl_PrintErrMsg
 * 
 * Revision 1.2  91/09/12  17:59:58  oz
 * 	Renamed startThreadOnSocket to be itl_StartIndependentThread
 * 
 * Revision 1.1  91/08/22  17:23:05  mss
 * 	Modified REC tester to make separate library
 * 
 * Revision 1.1  91/07/10  16:08:16  oz
 * 	Initial checkin - second try.
 */

/*
 * itl_server.c -- 
 *    Server routines for the test program to allow processes
 *    to attach to the test program while it is running and
 *    to talk to it.  In particualr, one can add windows on the
 *    fly.
 *  
 *  Original grabbed from Larry Jacobs.
 */

#include <utils/itl/itl_private.h>
#include <stdio.h>

#ifdef __STDC__
#include <stdlib.h>
#endif /* ansi */
#include <sys/types.h>   /* since the sun socket.h forgot to */
#include <sys/socket.h>

#ifdef __NeXT__
#include <libc.h>
#endif /* next */


PRIVATE int serverListen(socketId)
  int socketId;
{
    int winFd;
    int accept();
    /* XXX in the future get the connecter's addr for
     * authentication.
     */
    winFd = accept(socketId, NULL, NULL);

    itl_Trace(ITL_SERVER_TRACE,
	      "Accepted a connection with socket %d\n",
	      winFd);

    if (winFd < 0) {
	itl_Error("Accept returned an invalid descriptor.\n");
	/* this socket should never give us any trouble */
    }
    
    return(winFd);
} 

/*
 *  Listen on the designated socket and wait for someone
 *  to talk to us.
 *	The portPrefix is passes as a pointer_t so that this function can
 *	be passed as parameter to thread routines.
 */
EXPORT void itlServer_Start(ptr)
  IN pointer_t ptr;
{
    char * portPrefix = (char *)ptr;
    int socketId;
    char * tempPortPrefix;
    int errCount = 0;

    ITL_ALLOC(tempPortPrefix, char *,
	      strlen(ITL_SERVER_PREFIX) + strlen(portPrefix) + 1);
    sprintf(tempPortPrefix, ITL_SERVER_PREFIX_SPRINTF, portPrefix);

    socketId = itlPort_OpenServerPort(tempPortPrefix);

    ITL_FREE(tempPortPrefix);

    while(TRUE) {
	int fdIn, fdOut, fdErr;
	FILE *Fin, *Fout, *Ferr;

	if ((fdIn = serverListen(socketId)) < 0) {
	    if (errCount++ > 10) {
		itl_Error("Too many server errors - giving up\n");
		break;
	    }
	    else {
		continue;
	    }
	}

	fdOut = fdIn;
	fdErr = fdIn;
	/*  */

        Fin  = fdopen(fdIn,  "r");
	Fout = fdopen(fdOut, "w");
	Ferr = fdopen(fdErr, "w");

	itl_Trace(ITL_SERVER_TRACE,
		  "Opened files-- In: %p, Out %p, Err: %p\n",
		  Fin, Fout, Ferr);

	if (!Fin || !Fout || !Ferr) {
	    itl_Error("Server: Error opening file descriptor %d - socket %d\n",
		      fdIn, socketId);
	}
	itl_StartIndependentThread(Fin, Fout, Ferr);
	itl_Trace(ITL_SERVER_TRACE,
		  "Server thread is listening on socket %d\n",
		  socketId);
    }
}
