/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: msgq.h,v $
 * Revision 1.1.12.2  1996/02/18  23:08:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:31  marty]
 *
 * Revision 1.1.12.1  1995/12/11  22:50:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:23:39  root]
 * 
 * Revision 1.1.10.2  1993/07/28  19:39:46  cmckeen
 * 	HP's TET'ized version of the system tests
 * 	[1993/07/27  13:55:24  cmckeen]
 * 
 * Revision 1.1.6.3  1993/05/27  19:01:46  eheller
 * 	*** empty log message ***
 * 
 * Revision 1.1.6.2  1993/05/27  14:19:11  eheller
 * 	*** empty log message ***
 * 	[1993/05/26  18:43:45  eheller]
 * 
 * Revision 1.1.8.2  1993/04/23  14:43:51  sekhar
 * 	Merged Hp_dce1_0_1 changes
 * 	[1993/04/16  15:00:01  sekhar]
 * 
 * Revision 1.1.1.7  1993/03/11  22:16:13  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.3.2  1992/06/11  20:07:35  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:14:08  mcfarland]
 * 
 * Revision 1.1.4.2  1993/02/05  15:28:21  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:49:18  cjd]
 * 
 * Revision 1.1.2.3  1992/02/11  18:24:43  eperkins
 * 	*** empty log message ***
 * 	[1992/02/11  18:24:19  eperkins]
 * 
 * Revision 1.1  1992/01/19  14:34:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *
 * FILE_NAME: msgq.h
 *
 * COMPONENT_NAME: dce.systest
 *
 * FUNCTIONS: none.
 *
 * ORIGINS: 27
 *
 *
 * (C) COPYRIGHT International Business Machines Corp. 1991
 * All Rights Reserved
 *
 *
 * DESCRIPTION:
 *      This include file defines all the necessary macros, enums and
 *	structures common to all the message queue modules.
 *
 * HISTORY: 
 * 06/24/91	Wayne Mock	Initial design and code.
 * 08/30/91	Wayne Mock	Updated the documentation.
 * 11/06/91	Wayne Mock	Implemented the non-blocking functionality
 *				for message receive requests.
 */

#ifndef msgq_h
#define msgq_h
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
/*
 * MSGQ_THREADS must be defined if you want the msgq modules to be thread
 * safe.
 */
#ifdef MSGQ_THREADS
#include <dce/cma.h>
#include <dce/cma_ux.h>
#include <dce/cmalib_crtlx.h>
#include <pthread.h>
#endif

#include <systest.h>

char* progName;		/* argv[0] */

#define MAXMSGSIZ	1020	     /* the largest message that can be sent */
#define MSGQ_TIMEOUT	1000	     /* number of connect attempts */
#define IMSGSIZLESSBUF	(sizeof( struct imessage ) - MAXMSGSIZ)

/*
 * Things for the flags field in the imsgbuf structure defined below.
 */
#define IPC_NOWAIT	0004000		/* same as System V */


enum msgerr_t { MSG_FATAL_ERROR, MSG_NON_FATAL_ERROR };
#define msgfError(m,e)	MSGprintError( MSG_FATAL_ERROR, m, e ) 
#define msgnError(m,e)	MSGprintError( MSG_NON_FATAL_ERROR, m, e ) 


/*
 * the client includes these as part of the message.  It tells the
 * server what it should do, either send or receive a message.
 */
enum imsg_act { MSGSND, MSGRCV };


/*
 * This is the generic message structure.  All client messages are seen this
 * way by the message queue server.
 */
struct message
{	unsigned32 type;
	char buf[MAXMSGSIZ];
};

/*
 * Used by the msgsnd.c and msgrcv.c programs.  Recommended for use when
 * the user wants to pass string messages in a C program also.
 */
typedef struct message STRMSG;


/*
 * This is the message exchanged by the client stub and the server.
 */
struct imessage
{	enum imsg_act action;	/* either send or receive */
	int usize;		/* size of struct message */
	int flags;		/* not yet implemented */
	struct message umsg;	/* the actual messages sent by the client */
};

/*
 * utility function prototypes
 */
int msgrcv_bsd( char*, struct message*, int, long, int );
int msgsnd_bsd( char*, struct message*, int, int );
#endif
