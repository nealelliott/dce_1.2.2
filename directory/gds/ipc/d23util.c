/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23util.c,v $
 * Revision 1.1.773.2  1996/02/18  18:19:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:43  marty]
 *
 * Revision 1.1.773.1  1995/12/08  16:03:59  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/06/15  19:23 UTC  smythe
 * 	fix for CHFts15308
 * 
 * 	HP revision /main/HPDCE02/smythe_CHFts15308/1  1995/06/14  20:32 UTC  smythe
 * 	fix for CHFts15308
 * 
 * 	HP revision /main/HPDCE02/2  1995/04/26  20:24 UTC  dtruong
 * 	Added ifdef for __hpux
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/07  17:58 UTC  dtruong
 * 	Undefine msgrcv and msgsnd
 * 	[1995/12/08  15:15:27  root]
 * 
 * Revision 1.1.771.7  1994/09/06  12:25:40  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:16:49  keutel]
 * 
 * Revision 1.1.771.6  1994/08/25  15:34:49  keutel
 * 	OT 11882: logging wrong in d23_msgrcv
 * 	[1994/08/25  14:51:30  keutel]
 * 
 * Revision 1.1.771.5  1994/07/06  15:07:42  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:17:04  marrek]
 * 
 * Revision 1.1.771.4  1994/06/21  14:47:30  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  12:59:47  marrek]
 * 
 * Revision 1.1.771.3  1994/05/10  15:56:45  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:41:11  marrek]
 * 
 * Revision 1.1.771.2  1994/03/23  15:19:02  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:32:11  keutel]
 * 
 * Revision 1.1.771.1  1994/02/22  17:41:07  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:55:01  marrek]
 * 
 * Revision 1.1.769.2  1993/10/14  17:35:56  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:34:02  keutel]
 * 
 * Revision 1.1.769.1  1993/10/13  17:33:15  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:56:03  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  12:27:11  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:28:36  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  20:46:35  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:33:02  bbelch]
 * 
 * Revision 1.1.4.3  1992/11/27  17:32:46  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  14:07:08  marrek]
 * 
 * Revision 1.1.4.2  1992/10/12  21:07:37  sommerfeld
 * 	Change _HPUX_SOURCE to __hpux in #ifdef
 * 	[1992/10/05  16:15:42  sommerfeld]
 * 
 * 	Don't do a local declaration of ftok() on hpux (it clashes with
 * 	defn in system header files).
 * 	[1992/09/30  21:33:37  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/06/01  20:57:28  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:42:01  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23util.c,v $ $Revision: 1.1.773.2 $ $Date: 1996/02/18 18:19:59 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton **********************************************************************/
/*                                                                           */
/*   TYPE:    module                                                         */
/*                                                                           */
/*   NAME:    d23util.c                                                      */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       This module contains all common subroutines which are used by the   */
/*       client- resp. server specific IPC-functions of a directory system.  */
/*       The following subroutines are available:                            */
/*                                                                           */
/*          * d23_lock     -  lock (IPC specific) shared memory              */
/*          * d23_unlock   -  unlock shared memory                           */
/*          * d23_ipcerror -  handled IPC-error occurred                     */
/*          * d23_get_ipc  -  attach to all IPC-resources                    */
/*          * d23_msgsnd   -  send private IPC-message                       */
/*          * d23_msgrcv   -  receive private IPC-message                    */
/*          * d23_attsnd   -  send attention info through named FIFO         */
/*          * d23_getvpid  -  get next virtual process-ID                    */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 17.5.91     */
/*datoff *********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>

#if defined(__hpux)
/* --------------------------------------------------------------------------
   Undefine msgrcv and msgsnd because the non-block wrapper does not provides
   ways to handle signal handling, caused GDS to hang when SIGUSR1 arrive.
   GDS's backup/restore time out while sending distribute command to DSA
   process.
   -------------------------------------------------------------------------*/
#undef msgrcv
#define msgrcv _msgrcv_sys
#undef msgsnd
#define msgsnd _msgsnd_sys

#endif /* __hpux */

/* ------------------ DECLARATION OF GLOBAL DATA --------------------------- */

					/* process specific IPC-information */
D23_iprocinfo	d23_info = { D23_SIS(0) } ;
					/* variable is used to suppress */
					/* standard prolog in serviceability */
signed32	d23_svcprolog = 0 ;


/* ------------------- PROTOTYPING OF EXTERNAL FUNCTIONS ------------------- */
#if defined(__OSF__) || defined(_AIX)
extern key_t ftok (char *path_name, char project_id);
#endif /* __OSF__ || _AIX */

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d23_lock ()                                                    */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function locks the (IPC-specific) shared memory area by using   */
/*       a semaphore, and thus the accesses of different IPC-users (pro-     */
/*       cesses) to this shared memory are synchronized.                     */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       semid       = Semaphore set identifier                              */
/*       semnum      = No. of the semaphore within the semaphore set         */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     None                                                  */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*       0           = The function has been performed successfully.         */
/*       -1          = A system error is occurred during execution of the    */
/*                     function (in this case the system error variable      */
/*                     'errno' contains error information).                  */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 17.5.91     */
/*exoff **********************************************************************/

signed32 d23_lock (signed32 semid, signed32 semnum) {

    struct sembuf s ;
    signed32	  res ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_IPC,
			     svc_c_debug4, GDS_S_CSLOCK_ENTRY, semid, semnum)) ;
    s.sem_num = semnum ;
    s.sem_op = -1 ;
    s.sem_flg = SEM_UNDO ;
    while ((res = (signed32) semop ((int) semid, &s, 1)) == -1 &&
							       errno == EINTR) ;
    if (res == -1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
			    svc_c_sev_fatal, GDS_S_SEMOP_ERR, s.sem_op, errno) ;
	return (res) ;
    }
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					     svc_c_debug4, GDS_S_CSLOCK_EXIT)) ;
    return (res) ;
}

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d23_unlock ()                                                  */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function unlocks the (IPC-specific) shared memory area by       */
/*       using a semaphore, and thus the accesses of different IPC-users     */
/*       (processes) to this shared memory are synchronized. The function    */
/*       corresponds to the 'lock'-function (see above).                     */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       semid       = Semaphore set identifier                              */
/*       semnum      = No. of the semaphore within the semaphore set         */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     None                                                  */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*       0           = The function has been performed successfully.         */
/*       -1          = A system error is occurred during execution of the    */
/*                     function (in this case the system error variable      */
/*                     'errno' contains error information).                  */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 17.5.91     */
/*exoff **********************************************************************/

signed32 d23_unlock (signed32 semid, signed32 semnum) {

    struct sembuf s ;
    signed32	  res ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_IPC,
			   svc_c_debug4, GDS_S_CSUNLOCK_ENTRY, semid, semnum)) ;
    s.sem_num = semnum ;
    s.sem_op = 1 ;
    s.sem_flg = SEM_UNDO ;
    while ((res = (signed32) semop ((int) semid, &s, 1)) == -1 &&
							       errno == EINTR) ;
    if (res == -1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
			    svc_c_sev_fatal, GDS_S_SEMOP_ERR, s.sem_op, errno) ;
	return (res) ;
    }
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_debug4, GDS_S_CSUNLOCK_EXIT)) ;
    return (res) ;
}

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d23_ipcerror ()                                                */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function handles an IPC-error which is occurred by writing      */
/*       error informations to the log-file and to the standard header.      */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       d23_hdinfo  = Reference to the standard header.                     */
/*       error_no    = Error value.                                          */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     Error information within the standard header.         */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*       D23_ERROR   = An internal error is occurred.                        */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 17.5.91     */
/*exoff **********************************************************************/

signed32 d23_ipcerror (D2_pbhead *d23_hdinfo, signed32 error_no) {

    d23_hdinfo->d2_retcode = D2_TEMP_ERR;
    d23_hdinfo->d2_errclass = D2_INT_ERR;
    d23_hdinfo->d2_errvalue = error_no;
    return (D23_ERROR);
}

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d23_get_ipc ()                                                 */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function attaches the directory system client- resp. server     */
/*       process to the used IPC-resources (shared memory, semaphore and     */
/*       message queue). For attaching the IPC-resources are referenced by   */
/*       using a special key. This key is generated with the help of a       */
/*       constant key no. (D23_IPC_KEY) and the path name of an IPC-specific */
/*       file (D23_IPC_KFILE) within the current directory system            */
/*       installation.                                                       */
/*       Note: Some attentiveness should be done to avoid collisions         */
/*       between the keys used within the directory system and keys used by  */
/*       other applications which also handle with IPC-resources.            */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*                     None                                                  */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     None                                                  */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*       D23_SUCES   = The function has been performed successfully          */
/*                     (in this case the global structure d23_ipc contains   */
/*                     information about the allocated resources).           */
/*       D23_ERROR   = A system error is occurred during execution of the    */
/*                     function (in this case the system error variable      */
/*                     'errno' contains error information).                  */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 17.5.91     */
/*exoff **********************************************************************/

signed32 d23_get_ipc () {

    key_t		ipc_key ;
    char		key_path[D27_LFILE_NAME] ;
    struct msqid_ds	msgstate ;

    /* check availability of IPC-resources (message queue) */
    if (D23_GIS(d23_info.d23_istate) == 0 ||
		     msgctl (d23_info.d23_msgid, IPC_STAT, &msgstate) == -1) {
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					  svc_c_debug4, GDS_S_CSGETIPC_ENTRY)) ;

	/* get path name of current installation */
	sprintf (key_path, D23_IPC_KFILE, dcelocal_path, D27_BIN_DIR) ;

	if ((ipc_key = ftok (key_path, D23_IPC_KEY)) == (key_t)-1) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%c%d"), GDS_S_IPC,
				svc_c_sev_fatal | d23_svcprolog, GDS_S_FTOK_ERR,
				key_path, D23_IPC_KEY, errno) ;
	    return (D23_ERROR) ;
	}

	/* get IPC-resources (shared memory) */
	if ((d23_info.d23_shmid = (signed32) shmget (ipc_key, 0,
							  D23_ACCMODE)) == -1) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_IPC,
#ifndef __hpux_CHFts15308
			svc_c_sev_notice, GDS_S_SHMGET_ERR,
#else
			svc_c_sev_fatal | d23_svcprolog, GDS_S_SHMGET_ERR,
#endif
			ipc_key, D23_ACCMODE, errno) ;
	    return (D23_ERROR) ;
	}
	if ((d23_info.d23_genarea = (D23_ipcinfo *) shmat ((int) d23_info.d23_shmid,
				     (void *) NULL, 0)) == (D23_ipcinfo *) -1) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		      svc_c_sev_fatal | d23_svcprolog, GDS_S_SHMAT_ERR, errno) ;
	    return (D23_ERROR) ;
	}

	/* get IPC-resources (semaphores) */
	if ((d23_info.d23_semid = (signed32) semget (ipc_key, D23_MXSEMA,
							  D23_ACCMODE)) == -1) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_IPC,
			svc_c_sev_fatal | d23_svcprolog, GDS_S_SEMGET_ERR,
			ipc_key, D23_ACCMODE, errno) ;
	    return (D23_ERROR) ;
	}

	/* get IPC-resources (message queue) */
	if ((d23_info.d23_msgid = (signed32) msgget (ipc_key,
							  D23_ACCMODE)) == -1) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_IPC,
			svc_c_sev_fatal | d23_svcprolog, GDS_S_MSGGET_ERR,
			ipc_key, D23_ACCMODE, errno) ;
	    return (D23_ERROR) ;
	}

	/* verify version no. of shared memory layout */
	if (d23_info.d23_genarea->d23_versno != D23_SHMVERS) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
			   svc_c_sev_fatal | d23_svcprolog, GDS_S_IPCVERS_ERR) ;
	    return (D23_ERROR) ;
	}

	/* supply SHM-area references */
	d23_info.d23_dstarea = (D23_distcmd *) ((char *) d23_info.d23_genarea +
					d23_info.d23_genarea->d23_darea) ;
	d23_info.d23_regarea = (D23_reginfo *) ((char *) d23_info.d23_genarea +
					d23_info.d23_genarea->d23_rarea) ;
	d23_info.d23_svrarea = (D23_ipcentry *) ((char *) d23_info.d23_genarea +
					d23_info.d23_genarea->d23_sarea) ;
	d23_info.d23_cltarea = (D23_ipcentry *) ((char *) d23_info.d23_genarea +
					d23_info.d23_genarea->d23_carea) ;
	d23_info.d23_bufarea = (byte *) ((char *) d23_info.d23_genarea +
					d23_info.d23_genarea->d23_barea) ;
	d23_info.d23_crdarea = (D23_usrcred *) ((char *) d23_info.d23_genarea +
					d23_info.d23_genarea->d23_crarea) ;
	d23_info.d23_hparea = (D23_hpbufinfo *) ((char *) d23_info.d23_genarea +
					d23_info.d23_genarea->d23_hparea) ;
	d23_info.d23_extarea = (D23_extinfo *) ((char *) d23_info.d23_genarea +
					d23_info.d23_genarea->d23_exarea) ;
	d23_info.d23_ipid.d23_rpid = getpid () ;
	d23_info.d23_istate |= D23_SIS(D23_INIT) ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d"), GDS_S_IPC,
		svc_c_debug4, GDS_S_CSGETIPC_EXIT, d23_info.d23_shmid,
		d23_info.d23_genarea, d23_info.d23_semid, d23_info.d23_msgid)) ;
    }
    return (D23_SUCES) ;
}
 
/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d23_msgsnd ()                                                  */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function sends a IPC-message to an IPC-partner (client resp.    */
/*       server) by using the message queue.                                 */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       msgid       = Identifier of the message queue.                      */
/*       msgptr      = Reference to IPC-message.                             */
/*       msgsz       = Size of the IPC-message.                              */
/*       msgflg      = Message flag (means blocking/nonblocking mode).       */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     None                                                  */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*       D23_SUCES   = The function has been performed successfully.         */
/*       D23_ERROR   = A system error is occurred during execution of the    */
/*                     function (in this case the standard header contain    */
/*                     error information).                                   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 17.5.91     */
/*exoff **********************************************************************/

signed32 d23_msgsnd (signed32 msgid, D23_ipcmsg *msgptr, signed32 msgsz,
							      signed32 msgflg) {
    signed32	mtype, assid, res ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_IPC,
		      svc_c_debug4, GDS_S_CSSMSG_ENTRY, msgid, msgsz, msgflg)) ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s%d%s%d%d%d%d"),
		GDS_S_IPC, svc_c_debug2, GDS_S_IPCMSGP1, "SND",
		D23_GITYPE(msgptr->d23_mtype),
		(D23_GITYPE(msgptr->d23_mtype) == D23_INVMSG) ? "S" : "VP",
		D23_GDSTID(msgptr->d23_mtype), msgptr->d23_invinfo.d23_invid,
		D23_GCALASS(msgptr->d23_iassid),
		D23_GCLDASS(msgptr->d23_iassid))) ;
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d%d%d%d"),
		GDS_S_IPC, svc_c_debug2, GDS_S_IPCMSGP2,
		D23_GIOF(msgptr->d23_opid), msgptr->d23_invinfo.d23_contid,
		msgptr->d23_invinfo.d23_usrid, msgptr->d23_msgsz,
		D23_GIOI(msgptr->d23_opid), msgptr->d23_callpid.d23_rpid,
		msgptr->d23_callpid.d23_vpid)) ;
    if (msgptr->d23_msgsz > 0) {
	mtype = D23_GITYPE(msgptr->d23_mtype) ;
	assid = (mtype == D23_INVMSG || mtype == D23_INVCMSG) ?
             D23_GCALASS(msgptr->d23_iassid) : D23_GCLDASS(msgptr->d23_iassid) ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%b"),
		GDS_S_IPC, svc_c_debug6, GDS_S_IPCMSGP3, msgptr->d23_msgsz,
		d23_info.d23_bufarea + d23_info.d23_genarea->d23_bsize * assid)) ;
    }

    /* send message */
    while ((res = (signed32) msgsnd ((int) msgid, (void *) msgptr,
		       (size_t) msgsz, (int) msgflg)) == -1 && errno == EINTR) ;
    if (res == -1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
				     svc_c_sev_fatal, GDS_S_MSGSND_ERR, errno) ;
	return (D23_ERROR) ;
    }

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					     svc_c_debug4, GDS_S_CSSMSG_EXIT)) ;
    return (D23_SUCES) ;
}

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d23_msgrcv ()                                                  */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function waits resp. checks for an IPC-message from an          */
/*       IPC-partner (client resp. server) by using the message queue.       */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       msgid       = Identifier of the message queue.                      */
/*       msgptr      = Reference to a memory area where the received message */
/*                     should be stored.                                     */
/*       msgsz       = Max. size of the message which can by received.       */
/*       msgtyp      = Type of the message which should be received.         */
/*       msgflg      = Message flag (means blocking/nonblocking mode).       */
/*       pbptr       = Reference to a standard header.                       */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*       *msgptr     = Received message (only of the function has been       */
/*                     performed successfully).                              */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*       >= 0        = Size of the received message.                         */
/*       D23_ERROR   = A system error is occurred during execution of the    */
/*                     function (in this case the standard header contain    */
/*                     error information).                                   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 17.5.91     */
/*exoff **********************************************************************/

signed32 d23_msgrcv (signed32 msgid, D23_ipcmsg *msgptr, signed32 msgsz,
			   signed32 msgtyp, signed32 msgflg, D2_pbhead *pbptr) {

    signed32	assid, mtype, cnt = 0 ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d"), GDS_S_IPC,
	      svc_c_debug4, GDS_S_CSRMSG_ENTRY, msgid, msgsz, msgtyp, msgflg)) ;

    /* check or wait for a message */
    if ((cnt = (signed32) msgrcv ((int) msgid, (void *) msgptr, (size_t) msgsz,
					  (long) msgtyp, (int) msgflg)) == -1) {
	pbptr->d2_errvalue = D23_RMSGERR + errno ;
	if (errno == ENOMSG) {
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
					svc_c_debug4, GDS_S_CSRMSG_EXIT, 0)) ;
	    return (D23_ERROR) ;
	}
	if (errno == EINTR) {
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					    svc_c_debug4, GDS_S_CSRMSG_EXIT1)) ;
	    return (D23_ERROR) ;
	}
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
				     svc_c_sev_fatal, GDS_S_MSGRCV_ERR, errno) ;
	return (D23_ERROR) ;
    }

    /* check size of received message */
    if (cnt != D23_MSGBUFSZ) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
				     svc_c_sev_fatal, GDS_S_IPCMSGSZ_ERR, cnt) ;
	return (D23_ERROR) ;
    }

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s%d%s%d%d%d%d"),
		GDS_S_IPC, svc_c_debug2, GDS_S_IPCMSGP1, "RCV",
		D23_GITYPE(msgptr->d23_mtype),
		(D23_GITYPE(msgptr->d23_mtype) == D23_INVMSG) ? "S" : "VP",
		D23_GDSTID(msgptr->d23_mtype), msgptr->d23_invinfo.d23_invid,
		D23_GCALASS(msgptr->d23_iassid),
		D23_GCLDASS(msgptr->d23_iassid))) ;
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d%d%d%d"),
		GDS_S_IPC, svc_c_debug2, GDS_S_IPCMSGP2,
		D23_GIOF(msgptr->d23_opid), msgptr->d23_invinfo.d23_contid,
		msgptr->d23_invinfo.d23_usrid, msgptr->d23_msgsz,
		D23_GIOI(msgptr->d23_opid), msgptr->d23_callpid.d23_rpid,
		msgptr->d23_callpid.d23_vpid)) ;
    if (msgptr->d23_msgsz > 0) {
	mtype = D23_GITYPE(msgptr->d23_mtype) ;
	assid = (mtype == D23_INVMSG || mtype == D23_INVCMSG) ?
	     D23_GCALASS(msgptr->d23_iassid) : D23_GCLDASS(msgptr->d23_iassid) ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%b"),
		GDS_S_IPC, svc_c_debug6, GDS_S_IPCMSGP3, msgptr->d23_msgsz,
		d23_info.d23_bufarea + d23_info.d23_genarea->d23_bsize * assid)) ;
    }

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
					svc_c_debug4, GDS_S_CSRMSG_EXIT, cnt)) ;
    return (cnt) ;
}

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d23_attsnd ()                                                  */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function sends an attention information to a client- resp.      */
/*       server process by writing a byte to a process-ID resp. server-ID    */
/*       specific named FIFO.                                                */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       dstid       = Destination-ID (process-ID resp. server-ID).          */
/*       dsttype     = Destination-ID type.                                  */
/*       pbhead      = Reference to a standard header.                       */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     None                                                  */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*       D23_SUCES   = The function has been performed successfully.         */
/*       D23_ERROR   = A system error or an internal error is occurred       */
/*                     during execution of the function (in this case the    */
/*                     standard header contains detailed error information). */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 17.5.91     */
/*exoff **********************************************************************/


signed32 d23_attsnd (signed32 dstid, signed32 dsttype, D2_pbhead *pbhead) {

    signed32		i, j ;
    static signed32	sig_pipe_ignored = FALSE ;
    char		fname[D27_LFILE_NAME] ;
    byte		attbyte = 0 ;
    struct sigaction	siginfo ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_IPC,
			     svc_c_debug4, GDS_S_CSATT_ENTRY, dstid, dsttype)) ;

    /* search for the requested named FIFO information */
    for (i = j = 0; i < D23_MXEV; i++) {
	if (d23_info.d23_evinfo[i].d23_evfd != D23_UNUSED &&
	    d23_info.d23_evinfo[i].d23_evid == (D23_SDSTTY(dsttype) |
							     D23_SDSTID(dstid)))
	    break ;
	if (d23_info.d23_evinfo[i].d23_evrc < d23_info.d23_evinfo[j].d23_evrc)
	    j  = i ;
    }

    if (i == D23_MXEV) {
	/* requested named FIFO information not found -> */
	/* FIFO information with lowest reference counter value is discarded */
	if (d23_info.d23_evinfo[j].d23_evfd != D23_UNUSED) {
	    close (d23_info.d23_evinfo[j].d23_evfd) ;
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	     svc_c_debug3, GDS_S_CLOSE_FIFO, d23_info.d23_evinfo[j].d23_evfd)) ;
	}

	sprintf (fname, D23_FIFO_FILE, dcelocal_path, D27_CLIENT_DIR,
				   (dsttype == D23_DSTPID) ? 'p' : 's', dstid) ;

	/* open requested event FIFO */
	if ((d23_info.d23_evinfo[j].d23_evfd = open (fname, O_WRONLY |
							     O_NONBLOCK)) < 0) {
	    if (errno != ENXIO && errno != ENOENT) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d%d"),
			GDS_S_IPC, svc_c_sev_fatal, GDS_S_OPEN_ERR,
					    fname, O_WRONLY|O_NONBLOCK, errno) ;
		return (d23_ipcerror (pbhead, D23_IFFOPN)) ;
	    } else {
		/* no process has the named FIFO open for reading -> */
		/* ignored here, because this state should be detected by the */
		/* IPC-monitoring process */
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					      svc_c_debug4, GDS_S_CSATT_EXIT)) ;
		return (D23_SUCES) ;
	    }
	}
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s%d%d"), GDS_S_IPC,
		svc_c_debug3, GDS_S_OPEN_FIFO, fname,
		d23_info.d23_evinfo[j].d23_evfd, O_WRONLY | O_NONBLOCK)) ;
	d23_info.d23_evinfo[j].d23_evid = D23_SDSTTY(dsttype) | D23_SDSTID(dstid) ;
	d23_info.d23_evinfo[j].d23_evrc = 0 ;
	i = j ;
    }
    /* ignore signal SIGPIPE if not already done */
    if (!sig_pipe_ignored) {
	siginfo.sa_handler = SIG_IGN ;
	siginfo.sa_flags = 0 ;
	sigemptyset (&siginfo.sa_mask) ;
	if (SIGACTION (SIGPIPE, &siginfo, (struct sigaction *) NULL) == -1) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
			    svc_c_sev_fatal, GDS_S_SIGHDL_ERR, SIGPIPE, errno) ;
	    return (d23_ipcerror (pbhead, D23_SIGERR)) ;
	}
	sig_pipe_ignored = TRUE ;
    }

    /* send attention byte through named FIFO */
    if (write (d23_info.d23_evinfo[i].d23_evfd, (void *) &attbyte,
							      (size_t) 1) < 0) {
	if (errno != EPIPE) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
				      svc_c_sev_fatal, GDS_S_WRITE_ERR, errno) ;
	    return (d23_ipcerror (pbhead, D23_IFFWRT)) ;
	}
	/* no process has the named FIFO open for reading -> */
	/* ignored here, because this state should be detected by the */
	/* IPC-monitoring process, but we try it a second time to detect */
	/* an invalid file descriptor (because of a reused destination-id) */
	close (d23_info.d23_evinfo[i].d23_evfd) ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	     svc_c_debug3, GDS_S_CLOSE_FIFO, d23_info.d23_evinfo[i].d23_evfd)) ;
	d23_info.d23_evinfo[i].d23_evfd = D23_UNUSED ;
	d23_info.d23_evinfo[i].d23_evrc = 0 ;
	sprintf (fname, D23_FIFO_FILE, dcelocal_path, D27_CLIENT_DIR,
				   (dsttype == D23_DSTPID) ? 'p' : 's', dstid) ;
	if ((d23_info.d23_evinfo[i].d23_evfd = open (fname, O_WRONLY |
							    O_NONBLOCK)) >= 0) {
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s%d%d"), GDS_S_IPC,
		svc_c_debug3, GDS_S_OPEN_FIFO, fname,
		d23_info.d23_evinfo[i].d23_evfd, O_WRONLY | O_NONBLOCK)) ;

	    if (write (d23_info.d23_evinfo[i].d23_evfd, (void *) &attbyte,
							      (size_t) 1) > 0) {
		d23_info.d23_evinfo[i].d23_evrc++ ;

		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	        svc_c_debug2, GDS_S_IPCWRATT, d23_info.d23_evinfo[i].d23_evfd)) ;
	    }
	}
    } else {
	d23_info.d23_evinfo[i].d23_evrc++ ;

	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	       svc_c_debug2, GDS_S_IPCWRATT, d23_info.d23_evinfo[i].d23_evfd)) ;
    }

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					      svc_c_debug4, GDS_S_CSATT_EXIT)) ;
    return (D23_SUCES) ;
}

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d23_getvpid ()                                                 */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function gets the next virtual process-ID from shared memory.   */
/*       Virtual process-ID's are used in case of the real process-ID's as   */
/*       an unambiguous IPC-message-ID because in some operation systems     */
/*       like AIX the range of real process-ID's exceeds the range of        */
/*       message-ID's acceptable by IPC.                                     */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*                     None                                                  */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     None                                                  */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*       > 0         = Next virtual process-ID                               */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 28.9.93     */
/*exoff **********************************************************************/

signed32 d23_getvpid () {

    signed32		i, j, mxproc ;
    D23_ipcentry	*svrptr, *cltptr ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					    svc_c_debug4, GDS_S_CSVPID_ENTRY)) ;
    mxproc = d23_info.d23_genarea->d23_msno + d23_info.d23_genarea->d23_mcno +
								 D23_MXREGINFO ;
    for (i = 0; i < mxproc; i++) {
	/* get next virtual process-ID */
	d23_info.d23_genarea->d23_lvpid =
			    (d23_info.d23_genarea->d23_lvpid % D23_MXVPID) + 1 ;
	/* verify whether this virtual process-ID is already used */
	svrptr = d23_info.d23_svrarea ;
	for (j = 0; j < d23_info.d23_genarea->d23_msno; j++, svrptr++)
	    if (D23_GES(svrptr->d23_estate) == D23_ATTACH &&
		   svrptr->d23_epid.d23_vpid == d23_info.d23_genarea->d23_lvpid)
		break ;
	if (j < d23_info.d23_genarea->d23_msno)
	    /* virtual process-ID already used */
	    continue ;
	
	cltptr = d23_info.d23_cltarea ;
	for (j = 0; j < d23_info.d23_genarea->d23_mcno; j++, cltptr++)
	    if (D23_GES(cltptr->d23_estate) == D23_ATTACH &&
		   cltptr->d23_epid.d23_vpid == d23_info.d23_genarea->d23_lvpid)
		break ;
	if (j < d23_info.d23_genarea->d23_mcno)
	    /* virtual process-ID already used */
	    continue ;

	if (j == d23_info.d23_genarea->d23_mcno) {
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	    svc_c_debug4, GDS_S_CSVPID_EXIT, d23_info.d23_genarea->d23_lvpid)) ;
	    return (d23_info.d23_genarea->d23_lvpid) ;
	}
    }
    /* no virtual process-ID found - this should never occur */
    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC, svc_c_sev_fatal,
							    GDS_S_IPCVPID_ERR) ;
    return (0) ;
}
