/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 *(c) Copyright 1991,1992 Siemens-Nixdorf Information Systems,Burlington,MA,USA.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: sysdep.h,v $
 * Revision 1.1.9.2  1996/02/18  23:11:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:36:44  marty]
 *
 * Revision 1.1.9.1  1995/12/08  18:07:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:57  root]
 * 
 * Revision 1.1.6.5  1994/08/23  20:31:58  cbrooks
 * 	Code Cleanup - ANSI switches
 * 	[1994/08/23  15:26:17  cbrooks]
 * 
 * 	Code Cleanup Checkpoint
 * 	[1994/08/23  13:56:47  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:52:30  cbrooks]
 * 
 * Revision 1.1.6.4  1994/05/12  15:17:21  rhw
 * 	continued code cleanup & merging
 * 	[1994/05/09  18:46:29  rhw]
 * 
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:36  rhw]
 * 
 * Revision 1.1.6.3  1994/04/22  21:12:14  rousseau
 * 	Include sysdep_ultrix.h with dce/ prefix (CR 10446).
 * 	[1994/04/22  21:11:57  rousseau]
 * 
 * Revision 1.1.6.2  1994/04/21  22:05:15  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  20:05:20  cbrooks]
 * 
 * Revision 1.1.6.1  1994/04/07  10:22:36  kevins
 * 	CR 10304 Export some header files for cross component dependency issues
 * 	[1994/04/07  10:18:22  kevins]
 * 
 * Revision 1.1.3.3  1992/12/30  20:39:48  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:08:29  htf]
 * 
 * Revision 1.1.3.2  1992/09/29  20:44:48  devsrc
 * 	[OT 5373]  SNI/SVR4 Merge.
 * 	[1992/09/14  20:03:02  sekhar]
 * 
 * Revision 1.1  1992/01/19  15:31:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * Copyright (c) 1989, 1990, 1991 by
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
 * ABSTRACT:	System dependent header file
 *
 * ENVIRONMENT:	VAX/VMS V5.0
 *
 * AUTHORS:
 *		Carol Frampton
 *		Distributed Systems Services (DSS) 
 * 
 * DATE:	December 15, 1988
 *
 */


#ifndef _SYSDEP_H
#define _SYSDEP_H 1

#ifndef SOCK_STREAM
#include <sys/socket.h>
#include <sys/un.h>
#endif /* SOCK_STREAM */

#ifndef SNI_SVR4
#include <sys/domain.h>
#endif /* SNI_SVR4 */

#include <net/if.h>
#include <netinet/if_ether.h>

#include <dce/sysdep_ultrix.h>



/*
 * Doubly linked list of port descriptors.  These describe the DLI or 
 * WAN ports available to the service.
 */
typedef struct PortDsc
{

/* Operating System independent part */

    Que			que;	        /* for the que.h macros */
    NodeAddr            nodeAddr;       /* my node address on this port */
    unsigned long       lastSolSynchId;	/* synch id when last solic recvd */
    unsigned short	socketType;	/* one of K_SOCKET_xxx */
    unsigned short      owner_id;       /* owner of this socket - a set */
    char	        broadcast;      /* flag: TRUE if port allows */
    					/* broadcast/multicast */
    char	        datagram; 	/* flag: TRUE if port provides */
    					/* unreliable delivery */
    char		devName[K_MAX_DEVNAME+1];

    int                socket_id;     /* socket id */
    fd_set     iocFDs;	      /* mask of open connectionful fds */
    fd_set     activeFDs;     /* active connection, connect()   */
    fd_set     passiveFDs;    /* passive FDs, fd = accept(....) */
    union {
	struct sockaddr_in in;        /* Internet socket address */
    } saddr;
    int                slen;          /* length of saddr */

    struct EventBlk    *eblkptr;      /* associated eventblock */
    struct eblk_list_t *eblkFreeHdr;  /* list of free event blocks */
    struct eblk_list_t *eblkBusyHdr;  /* list of busy event blocks */

} PortDsc;



/*
 * Buffer for handing I/O information to the IPC (channel/pipe), DLI 802.3
 * and DECnet routines.
 */
typedef struct IOdsc
{
    /* 
     * The first part is used on all operating systems
     */
    unsigned char *ptr;		    /* pointer to buffer */
    int bufLen;			    /* buffer size */
    int outLen;			    /* length of data in read buffer */
    NodeAddr nodeAddr;		    /* network address */
    char *nodeName;		    /* node name if arrived over WAN */
    PortDsc *portPtr;		    /* ptr to port Port Descriptor */
    int linkId;			    /* WAN logical link */

    unsigned char *bufP;	    /* ptr to real buffer for dli io descriptors */
    socket_dsc *IPCHandle;	    /* our ipc handle */
} IOdsc;



/*
 * Shared memory flags
 */
#define M_SHMFLAG_READ_ONLY	(1 << 0)
#define M_SHMFLAG_READ_WRITE	(1 << 1)
#define M_SHMFLAG_CREATE	(1 << 2)

/*
 * Channel/Socket constants
 */
#define K_CHANNEL_CPTS	     	0       /* CP to TS messages */
#define K_CHANNEL_TSCP	 	1       /* TS to CP messages */
#define K_CHANNEL_TPTS	 	2       /* TP to TS messages */
#define K_CHANNEL_TSTP	 	3       /* TS to TP messages */
#define K_SOCKET_DTSS_CML       4       /* Dtss daemon to CML (for NCL mgmt) */

#define K_NUM_CHANNELS	 	5       /* Total # of communication channels */
#define K_NUM_CONN_CHANNELS     4       /* Total # of connection-based */
					/* sockets */ 
                                        /* CML uses connectionless datagram */
					/* sockets */
/*
 * Channel operations
 */
#define K_CHANNEL_CREATE  	0 
#define K_CHANNEL_ATTACH  	1 
#define K_CHANNEL_DELETE  	2 
#define K_CHANNEL_DETACH  	3 

/* Events */

#define    EVENTEMPTY  0	/* dummy (0 isn't a useful value;
				 * this is really a set) */

#define    EVENTREAD   0x1	/* wait for read events on this
				 * file descriptor */

#define    EVENTWRITE  0x10	/* write */

#define    EVENTEXCEPT 0x100	/* exception */


/* the following K_SOCKET... values are stored in the PortDsc structure */

#define K_SOCKET_DTS_8023   1	/* send on this port with SendDLImsg */
#define K_SOCKET_DTS_DECNET 2	/* send on this port with SendDECnetMsg */
#define K_SOCKET_DTS_TCP    3	/* send on this port with SendTCPmsg */
#define K_SOCKET_DTS_RPC    5   /* communicate on this port with RPC calls */


/*
 * The following K_PORT_... values are for the VMS port state in the PortDsc
 * structure.
 */
#define K_PORT_STOPPED 0	/* port is stoppped */
#define K_PORT_RUNNING 1	/* port is running */
#define K_PORT_ERROR   2        /* detected a transient error on this port */



/*
 * 			PROTOTYPES
 *
 */

/*
 * Thread-safe versions of IPC routines
 */
#if CONFIG_THREADS

StatusBlk CreateThreadIPC _DCE_PROTOTYPE_ (
	   (int ipcconst,
	    char create_flag,
	    char *ipcname,
	    char *userdata,
	    int userdataLen, 
	    char **opaqueIPCDesc));

StatusBlk GetThreadIPCConnect _DCE_PROTOTYPE_ (
	   (char *listeningIPCdesc,
	    char **connectIPCdesc));

StatusBlk ReadThreadIPC _DCE_PROTOTYPE_ (
	   (char *opaqueIPCDesc,
	    char *buf,
	    int lenExpected,
	    int *amountRead));

StatusBlk TerminateThreadIPC _DCE_PROTOTYPE_ (
	   (char *opaqueIPCDesc,
	    unsigned long flags));

#endif /* CONFIG_THREADS */

/* 
 * These two routines should only be called for NCL mgmt support 
 */

StatusBlk CreateCML_IPC _DCE_PROTOTYPE_ (
	    (int IpcEntClass,
	     int EntityClass,
	     char **IPCDesc));

StatusBlk ReadCML_IPC _DCE_PROTOTYPE_ (
	    (char *IPCDesc,
	     char *buffer,
	     int  buffsize,
	     int  *amountread));


/* dtss should pass zero in userdata length */
StatusBlk CreateIPC _DCE_PROTOTYPE_ (
	   (int ipcconst,
	    int create_flag,
	    char *ipcname,
	    char *userdata,
	    int userdataLen, 
	    char **opaqueIPCDesc));

StatusBlk ReadIPC _DCE_PROTOTYPE_ (
	   (char *opaqueIPCDesc,
	    char *buf,
	    int lenExpected,
	    int *amountRead));

StatusBlk WriteIPC _DCE_PROTOTYPE_ (
	   (char *opaqueIPCDesc,
	    pointer_t buf,
	    int bufLen));

StatusBlk TerminateIPC _DCE_PROTOTYPE_ (
	   (char *opaqueIPCDesc,
	    unsigned long flags));

StatusBlk AddEvent _DCE_PROTOTYPE_ (
	   (char *opaqueIPCDesc,
	    int eventType,
	    char *opaqueSysInfo));

StatusBlk DeleteEvent _DCE_PROTOTYPE_ (
	   (char *opaqueIPCDesc,
	    int eventType));

StatusBlk CheckPrivs _DCE_PROTOTYPE_ (
	   (unsigned long));

StatusBlk GetOsMsg _DCE_PROTOTYPE_ (
	   (unsigned long error,
	    char *buffer_p,
	    int blen));

void PrintOSmsg _DCE_PROTOTYPE_ (
	   (unsigned long OSstatus));


StatusBlk PrintMsgToOperator _DCE_PROTOTYPE_ (
	   ( char *message ));

void PrintOSmsgToOperator _DCE_PROTOTYPE_ (
	   (unsigned long OsStatus));

StatusBlk InitTransport _DCE_PROTOTYPE_ (
	   (void) );

void ReleaseTPTSfds _DCE_PROTOTYPE_ ((IOdsc *iodsc) );

/* O B S O L E T E !! !! !! !! !! !! !! !! !! !! */
/* that means: don't use any of the calls below! */
/* O B S O L E T E !! !! !! !! !! !! !! !! !! !! */

StatusBlk CreateChannel _DCE_PROTOTYPE_ ((int c_index, unsigned long func,
                                     char *chanName));
StatusBlk PostReadToChannel _DCE_PROTOTYPE_ ((int c_index, IOdsc *ioDscPtr));

StatusBlk ReplyOnChannel _DCE_PROTOTYPE_ ((IOdsc *ioDscPtr, pointer_t msgPtr,
                                      int msgLen));
StatusBlk WriteChannel _DCE_PROTOTYPE_ ((int c_index, pointer_t buf, int buflen));
StatusBlk TerminateChannel _DCE_PROTOTYPE_ ((int c_index, unsigned long flags));

#endif  /* #ifndef _SYSDEP_H */

