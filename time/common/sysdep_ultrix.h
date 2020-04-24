/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sysdep_ultrix.h,v $
 * Revision 1.1.6.2  1996/02/18  23:11:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:36:46  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:07:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:58  root]
 * 
 * Revision 1.1.4.1  1994/05/12  15:17:22  rhw
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:37  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  20:39:54  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:08:33  htf]
 * 
 * Revision 1.1  1992/01/19  15:31:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * Copyright (c) 1989 by
 * Digital Equipment Corporation, Maynard, Mass.
 * 
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:	System dependent header file for ULTRIX
 *
 * ENVIRONMENT:	Ultrix 3.0 and above
 *
 * AUTHORS:
 *		Cesar Cortes
 *		Distributed Systems Services (DSS) 
 * 
 * DATE:	August 18, 1989
 *
 */

#ifndef SYSDEP_ULTRIX_H
#define SYSDEP_ULTRIX_H

#include <sys/types.h>

#define K_MAX_USERDATA  (255)

/* socket descriptor */
typedef struct socket_dsc {
    int	  socket_id;               /* -1 if no socket assigned   */     
    char  listener;                /* true if this socket is a listener */
    struct socket_dsc *listening_p;/* non-zero if a listening socket,   */
                                   /* and there are accepted sockets.   */
    char  sockname[K_MAX_SOCKNAME];/* socket name, /tmp/xxx      */
    char  userdata[K_MAX_USERDATA];
    int   userdataLen;
    int   ipcconst;
    struct socket_dsc *next_p;
    } socket_dsc;


#include <dce/dce.h>
/* functional prototypes */
/* processes exception IPC events */
char GetExceptEvent _DCE_PROTOTYPE_((fd_set *));
/* processes connect IPC events */
char GetConnectEvent _DCE_PROTOTYPE_((fd_set *));
/* processes read IPC events */
char GetReadEvent _DCE_PROTOTYPE_((fd_set *, char **, int *));
/* processes write IPC events */
char GetWriteEvent _DCE_PROTOTYPE_((fd_set *, char **, int *));
 

/* 
 * Macros which operate on 64 bit file descriptor masks.  These
 * macros break the fd set types.h interface but are used for
 * efficientcy reasons.
 */
/*
 * resultfdset = op1fdset & op2fdset
 */
#define AndFDset(resultfdset,op1fdset,op2fdset)    \
    ((unsigned int *)resultfdset)[0] =             \
        ((unsigned int *)op1fdset)[0] & ((unsigned int *)op2fdset)[0]; \
    ((unsigned int *)resultfdset)[1] =                                 \
        ((unsigned int *)op1fdset)[1] & ((unsigned int *)op2fdset)[1]; 
/*
 * testfdmask == 0 ? 1 : 0
 */
#define IsFDsetZero(testfdmask)                    \
    ( (((unsigned int *)testfdmask)[0]==0)&&       \
     (((unsigned int *)testfdmask)[1]==0) )


#endif  /* SYSDEP_ULTRIX_H */
