/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23ipcchk.c,v $
 * Revision 1.1.10.2  1996/02/18  18:19:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:25  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:03:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:15:04  root]
 * 
 * Revision 1.1.8.6  1994/09/06  12:25:24  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:15:21  keutel]
 * 
 * Revision 1.1.8.5  1994/07/06  15:07:38  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:16:47  marrek]
 * 
 * Revision 1.1.8.4  1994/06/21  14:47:24  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  12:59:27  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:56:28  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/05  06:42:14  marrek]
 * 
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/02  13:23:30  marrek]
 * 
 * 	Bug fixes.
 * 	[1994/04/29  11:59:13  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:40:08  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:18:53  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:31:43  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:40:34  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:52:34  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:09:29  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:25:58  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  20:44:59  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:32:10  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/17  23:13:26  tom
 * 	Bug 6411 - Add setlocale call for i18n.
 * 	[1992/12/17  23:08:43  tom]
 * 
 * Revision 1.1.4.2  1992/11/27  17:31:56  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  14:06:13  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:54:16  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:40:43  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23ipcchk.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:19:39 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton **********************************************************************/
/*                                                                           */
/*   TYPE:    program                                                        */
/*                                                                           */
/*   NAME:    d23ipcchk                                                      */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The program verifies the IPC-state informations contained in the    */
/*       shared memory area of a directory system installation.              */
/*       The time interval (in seconds), in which the verification should be */
/*       done can be controlled by starting the program with a specific      */
/*       switch. If there is no switch on startup the program uses the       */
/*       default interval time D23_DEFCHKTIME.                               */
/*       Particulary the availability of the client- resp. server components */
/*       (processes) of the directory system which are attached to the IPC-  */
/*       resources is verified. If there is a client- resp. server entry     */
/*       occupied within the shared memory area and the corresponding        */
/*       process doesn't exist, then this entry is removed together with     */
/*       other IPC-resources assigned to this process (e.g. messages within  */
/*       the message queue, buffers within the heap, etc.). Also, a ABORT-   */
/*       -message is send to those IPC-partners (if there are any)           */
/*       which are waiting for a request/response from this process.         */
/*                                                                           */
/*       In a multithreaded environment also a thread is activated which     */
/*       exports an RPC-server interface for serviceability (see also        */
/*       servicability documentation).                                       */
/*                                                                           */
/*                                                                           */
/*   CALLING PARAMETER:                                                      */
/*        -i<nnn>    = time interval (in seconds) in which the verification  */
/*                     of the IPC-state informations should be done -->      */
/*                     <nnn> must be within the range (1 <= <nnn> <=         */
/*                     D23_MCHKTIME)                                         */
/*        -f         = don't fork daemon process (this switch should be      */
/*                     used for running the program under a debugger).       */
/*        -N<name>   = global name of the RPC-server (following the DCE name */
/*                     syntax) in the name service data base.                */
/*                     Note: if such a global name is not specified, then    */
/*                     the server may be contacted by using the binding      */
/*                     information written into the exception file of the    */
/*                     process.                                              */
/*        -P, -v, -w = serviceability switches.                              */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*    Author: R.Horn                                      Date: 17.5.91      */
/*datoff *********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <locale.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <dce/dcesvcmsg.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>

/* --------------------- DECLARATION OF GLOBAL DATA ------------------------ */

extern D23_iprocinfo    d23_info ;      /* process specific IPC-information */

/* --------------------- DECLARATION OF LOCAL DATA ------------------------- */

				/* check interval time */
static signed32	chktime = D23_DEFCHKTIME ;
				/* 'running mode'-flag */
static boolean	foreground = FALSE ;

#if defined THREADSAFE
				/* 'serviceability RPC-server active'-flag */
static boolean  svc_rpc_server_active = FALSE ;
#endif

/* --------------------- PROTOTYPING OF LOCAL FUNCTIONS -------------------- */

					/* send IPC-abort message */
signed32 d23_snd_abort (signed32, signed32, signed32, D23_rvpid *, signed32,
								     signed32) ;
					/* remove named FIFO */
signed32 d23_remove_fifo (char *, signed32, signed32) ;
					/* check existence of process */
signed32 d23_cproc_check (D23_ipcentry *, signed32, signed32, signed32) ;
					/* send attention byte */
signed32 d23_snd_att (char *instname, signed32 fifotype, signed32 fifoid) ;
					/* signal handler 'logging on/off' */
void siglogging (int) ;
					/* signal handler 'terminate process' */
void sigterm (int) ;

int main (signed32 argc, char *argv[]);

/* ---------------------- M  A  I  N --------------------------------------- */

main (signed32 argc, char *argv[]) {

    D23_distcmd		*distptr ;
    D23_ipcentry	*svrptr, *svraptr, *svrhptr ;
    D23_ipcentry	*cltptr, *cltaptr ;
    D23_ipcmsg		rcvmsg ;
    D23_usrcred		*credptr ;
    D23_reginfo		*procptr ;
    D23_regprocpb	regpb ;
    error_status_t	svc_result ;
    struct sigaction	siginfo ;
    D2_pbhead		pbhead ;
    signed32		i, j, mxsvr, mxclt, instno, opt, adsvr ;
    signed32		msgtype, oflg, ostate, errsav, remid ;
    pid_t		result ;
    char		daemon_dir[D27_LFILE_NAME] ;
    char		install_path[D27_LFILE_NAME];
    char		*instptr = install_path ;
#if defined THREADSAFE
    unsigned_char_t	*server_name = (unsigned_char_t *) NULL ;
    struct timespec	delay ;
#endif

    setlocale(LC_ALL, "");

    /* evaluate all calling parameters */
    opterr = 0 ;
    while ((opt = getopt (argc, argv, "fi:N:P:v:w:")) != -1) {
	switch (opt) {
	case 'f':
	    /* run process in the foreground */
	    foreground = TRUE ;
	    break ;
	case 'i':
	    if ((chktime = atoi (optarg)) < 1 || chktime > D23_MCHKTIME) {
		exit (D27_INV_IVTIME) ;
	    }
	    break ;
#if defined THREADSAFE
	case 'N':
	    /* global name of RPC-server */
	    server_name = (unsigned_char_t *) optarg ;
	    break ;
#endif
	case 'P':
	case 'v':
	case 'w':
	    /* serviceability switches */ 
	    break ;
	default:
	    exit (D27_INV_PAR) ;
	}
    }

    /* get # of directory system installation */
    sprintf(instptr, "%s%s", dcelocal_path, D27_CLIENT_DIR);
    instno = 1;

    if (!foreground) {
	/* fork daemon process */
	if ((result = fork()) < 0) {
	    exit(D27_FORK_ERR) ;
	}
	if (result != 0)
	    exit(0) ;

	/* set new process group */
	setpgrp () ;

	/* change actual directory */
	sprintf (daemon_dir, "%s%s", instptr, D27_ADM_DIR) ;
	if (chdir (daemon_dir) == -1) {
	    exit (D27_CHDIR_ERR) ;
	}

	/* close all unused files (inherited from fork ()) */
	d27_011_close_unused_files (TRUE) ;
    }

    /* initialize serviceability */
    d27_001_svc_init (argc, argv, &svc_result) ;
    if (svc_result != svc_s_ok) {
	fprintf (stderr, "%s: Unable to initialize serviceability, status: 0x%lx\n",    
							  argv[0], svc_result) ;
	exit (1) ;
    }

    /* establish distributed command and process termination signal routine */
    siginfo.sa_handler = siglogging ;
    siginfo.sa_flags = 0 ;
    sigemptyset (&siginfo.sa_mask) ;
    if (SIGACTION (SIGUSR1, &siginfo, (struct sigaction *) NULL) == -1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
			    svc_c_sev_fatal, GDS_S_SIGHDL_ERR, SIGUSR1, errno) ;
	exit (1) ;
    }

    siginfo.sa_handler = sigterm ;
    siginfo.sa_flags = 0 ;
    sigemptyset (&siginfo.sa_mask) ;
    if (SIGACTION (SIGTERM, &siginfo, (struct sigaction *) NULL) == - 1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
			    svc_c_sev_fatal, GDS_S_SIGHDL_ERR, SIGTERM, errno) ;
	exit (1) ;
    }

    siginfo.sa_handler = SIG_IGN ;
    siginfo.sa_flags = 0 ;
    sigemptyset (&siginfo.sa_mask) ;
    if (SIGACTION (SIGPIPE, &siginfo, (struct sigaction *) NULL) == -1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
			    svc_c_sev_fatal, GDS_S_SIGHDL_ERR, SIGPIPE, errno) ;
	exit (1) ;
    }

    /* register daemon process to IPC */
    regpb.d23_Ihdinfo.d2_version = D23_V02 ;
    regpb.d23_Iptype = D23_MONPROC ;
    regpb.d23_Idirid = 0 ;
    regpb.d23_Iotype = TRUE ;
    if (d23_reg_process (&regpb) == D23_ERROR) {
	exit (1) ;
    }

#if defined THREADSAFE
    /* initialize and establish serviceability RPC-server */
    if (server_name != (unsigned_char_t *) NULL) {
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s"), GDS_S_GENERAL,
			     svc_c_debug3, GDS_S_IPCMON_RPCNAME, server_name)) ;
    }
    if (d23_init_svcserver (server_name) == 0) {
	svc_rpc_server_active = TRUE ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				      svc_c_sev_notice, GDS_S_ENTRY_RPCSERVER) ;
    }
#endif

    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
					 svc_c_sev_notice, GDS_S_ENTRYMONITOR) ;

    for (;;) {
#if defined THREADSAFE
	delay.tv_sec = chktime ;
	delay.tv_nsec = 0 ;
	if (pthread_delay_np (&delay) == -1) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			      svc_c_sev_warning, GDS_S_RPC_THREADS_ERR, errno) ;
	    exit (1) ;
	}
#else
	if (sleep (chktime) > 0) {
		continue ;
	}
#endif
	/* get/check IPC-resources */
	if (d23_get_ipc () == D23_ERROR) {
	    exit (1) ;
	}

	/* lock shared memory area */
	if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	    exit (1) ;
	}

	/* *** verify IPC-registration area information *** */
	/* ************************************************ */
	for (i = 0, procptr = d23_info.d23_regarea;
			   i < d23_info.d23_genarea->d23_mrno; i++, procptr++) {
	    if (procptr->d23_pregtype != D23_UNUSED &&
			    kill (procptr->d23_procid, 0) < 0) {
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
					svc_c_debug1, GDS_S_IPCMON_REG, i + 1,
					procptr->d23_procid, errno)) ;
		procptr->d23_pregtype = D23_UNUSED ;
		strcpy (procptr->d23_pstate, D23_NOSTATE) ;
		d23_info.d23_genarea->d23_rno-- ;
	    }
	}

	/* *** verify distributed command area information *** */
	/* *************************************************** */
	for (i = 0, distptr = d23_info.d23_dstarea;
			   i < d23_info.d23_genarea->d23_mdno; i++, distptr++) {
	    if (distptr->d23_dcmd != D23_UNUSED &&
				     kill (distptr->d23_dcpid, 0) < 0) {
		distptr->d23_dcmd = D23_UNUSED ;
		if (distptr->d23_dparsize > 0)
		    d23_shmfree (distptr->d23_dpar) ;
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"),
				GDS_S_GENERAL, svc_c_debug1, GDS_S_IPCMON_DIST,
				i + 1, distptr->d23_dcpid)) ;
		d23_info.d23_genarea->d23_dno-- ;
	    }
	}

	/* *** verify server area information *** */
	/* ************************************** */
	svrptr = d23_info.d23_svrarea ;
	mxsvr = d23_info.d23_genarea->d23_msno ;
	cltptr = d23_info.d23_cltarea ;
	mxclt = d23_info.d23_genarea->d23_mcno ;

	for (i = 0, svraptr = svrptr; i < mxsvr; i++, svraptr++) {
	    if (D23_GES(svraptr->d23_estate) == D23_NOSVR)
		continue ;

	    ostate = D23_GEOS(svraptr->d23_estate) ;
	    if (kill (svraptr->d23_epid.d23_rpid, 0) < 0) {
		errsav = errno ;
		/* server process doesn't longer exist -> check existence */
		/* of a corresponding client process */
		for (adsvr = 0, svrhptr = svrptr; adsvr < mxsvr; adsvr++,
								    svrhptr++) {
		    if (D23_GES(svrhptr->d23_estate) == D23_ATTACH &&
			svrhptr->d23_esid == svraptr->d23_esid &&
			svrhptr->d23_epid.d23_rpid != svraptr->d23_epid.d23_rpid &&
				      kill (svrhptr->d23_epid.d23_rpid, 0) == 0)
			/* additional server-ID specific process exists */
			break ;
		}
		if (ostate != D23_ASSOC && d23_cproc_check (cltptr, mxclt,
			    svraptr->d23_esid, svraptr->d23_cpid.d23_rpid) >= 0) {
		    msgtype = (ostate == D23_INVPEND)   ? D23_INVCMSG :
			      (ostate == D23_RBRESPEND) ? D23_RRSBFMSG : 0 ;
		    if (msgtype) {
			/* check for a invoke continuation resp. a return */
			/* result buffer message */
			if (d23_msgrcv (d23_info.d23_msgid, &rcvmsg,
			    D23_MSGBUFSZ, (signed32) (D23_SITYPE(msgtype) |
			    D23_SDSTID(svraptr->d23_epid.d23_vpid)), IPC_NOWAIT,
							&pbhead) == D23_ERROR) {
			    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d"),
				GDS_S_GENERAL, svc_c_debug1, GDS_S_IPCMON_SVR1,
				i + 1, svraptr->d23_epid.d23_rpid,
				svraptr->d23_epid.d23_vpid, errsav)) ;
			    if (pbhead.d2_errvalue != D23_RMSGERR + ENOMSG) {
				DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"),
				GDS_S_GENERAL, svc_c_debug1, GDS_S_IPCMON_SVR2,
								       i + 1)) ;
			    }
			    continue ;
			} 
			oflg = D23_GIOF(rcvmsg.d23_opid) ;
		    }
		    msgtype = (ostate == D23_REQPEND ||
			      (ostate == D23_INVPEND && oflg == D23_END))  ?
							       D23_RESMSG :
			      (ostate == D23_RBINVPEND ||
			      (ostate == D23_INVPEND && oflg == D23_MORE)) ?
							       D23_RRQBFMSG :
			      (ostate == D23_RBRESPEND ||
			       ostate == D23_RESPEND)   ?      D23_RESCMSG : 0 ;
		    if (msgtype) {
			/* send abort message to client process */
			if (d23_snd_abort (d23_info.d23_msgid, msgtype,
			      svraptr->d23_cpid.d23_vpid, &svraptr->d23_epid,
			      i, svraptr->d23_cassid) == D23_ERROR) {
			    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"),
			    GDS_S_GENERAL, svc_c_debug1, GDS_S_IPCMON_SVR3,
								       i + 1)) ;
			}
			if (D23_GPEVM(svraptr->d23_estate) == D23_FASYNC_EM) {
			    /* send attention byte through named FIFO */
			    d23_snd_att (instptr, D23_DSTPID,
						   svraptr->d23_cpid.d23_rpid) ;
			}
		    }
		} else {
		    /* remove process-ID specific unused messages */
		    for (j = 0; j < 2; j++) {
			msgtype = (j == 0) ? D23_INVCMSG : D23_RRSBFMSG ;
			if (d23_msgrcv (d23_info.d23_msgid, &rcvmsg,
			    D23_MSGBUFSZ, (signed32) (D23_SITYPE(msgtype) |
			    D23_SDSTID(svraptr->d23_epid.d23_vpid)), IPC_NOWAIT,
							&pbhead) == D23_ERROR) {
			    if (pbhead.d2_errvalue != D23_RMSGERR + ENOMSG) {
				DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"),
				GDS_S_GENERAL, svc_c_debug1, GDS_S_IPCMON_SVR2,
								       i + 1)) ;
			    }
			} else
			    break ;
		    }
		    /* remove server-ID specific unused messages */
		    if (adsvr == mxsvr) {
			if (d23_msgrcv (d23_info.d23_msgid, &rcvmsg,
			    D23_MSGBUFSZ, (signed32) (D23_SITYPE(D23_INVMSG) |
			    D23_SDSTID(svraptr->d23_esid)), IPC_NOWAIT,
							&pbhead) == D23_ERROR) {
			    if (pbhead.d2_errvalue != D23_RMSGERR + ENOMSG) {
				DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"),
				GDS_S_GENERAL, svc_c_debug1, GDS_S_IPCMON_SVR2,
								       i + 1)) ;
			    }
			}
		    }
		}

		if (D23_GOEVM(svraptr->d23_estate) != D23_SYNC_EM) {
		    /* remove process-ID specific named FIFO */
		    d23_remove_fifo (instptr, D23_DSTPID,
		    				   svraptr->d23_epid.d23_rpid) ;

		    if (adsvr == mxsvr)
		        /* last process which belongs to the server-ID -> */
		        /* remove server-ID specific named FIFO */
		        d23_remove_fifo (instptr, D23_DSTSID,
							    svraptr->d23_esid) ;
		}

		/* release server entry */
		svraptr->d23_estate = D23_SES(D23_NOSVR) ;
		d23_info.d23_genarea->d23_sno-- ;	
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d"),
				GDS_S_GENERAL, svc_c_debug1, GDS_S_IPCMON_SVR,
				i + 1, svraptr->d23_epid.d23_rpid,
				svraptr->d23_epid.d23_vpid, errsav)) ;
	    }
	}

	/* *** verify client area information *** */
	/* ************************************** */
	for (i = 0, cltaptr = cltptr; i < mxclt; i++, cltaptr++) {
	    if (D23_GES(cltaptr->d23_estate) == D23_NOCLT)
		continue ;

	    ostate = D23_GEOS(cltaptr->d23_estate) ;
	    if (kill (cltaptr->d23_epid.d23_rpid, 0) < 0) {
		errsav = errno ;
		/* client process doesn't longer exist -> check existence */
		/* of a corresponding server process */
		if (d23_cproc_check (svrptr, mxsvr, cltaptr->d23_esid,
					     cltaptr->d23_cpid.d23_rpid) >= 0) {
		    msgtype = (ostate == D23_RBINVPEND) ? D23_RRQBFMSG :
			      (ostate == D23_REQPEND)   ? D23_RESMSG :
			      (ostate == D23_RESPEND)   ? D23_RESCMSG : 0 ;
		    if (msgtype) {
			/* check for a return buffer resp. result message */
			if (d23_msgrcv (d23_info.d23_msgid, &rcvmsg,
			    D23_MSGBUFSZ, (signed32) (D23_SITYPE(msgtype) |
			    D23_SDSTID(cltaptr->d23_epid.d23_vpid)), IPC_NOWAIT,
							&pbhead) == D23_ERROR) {
			    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d"),
				GDS_S_GENERAL, svc_c_debug1, GDS_S_IPCMON_CLT1,
				i + 1, cltaptr->d23_epid.d23_rpid,
				cltaptr->d23_epid.d23_vpid, errsav)) ;
			    if (pbhead.d2_errvalue != D23_RMSGERR + ENOMSG) {
				DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"),
				GDS_S_GENERAL, svc_c_debug1, GDS_S_IPCMON_CLT2,
								       i + 1)) ;
			    }
			    continue ;
			} 
			oflg = D23_GIOF(rcvmsg.d23_opid) ;
			cltaptr->d23_cassid = D23_GCALASS(rcvmsg.d23_iassid) ;
			cltaptr->d23_cpid = rcvmsg.d23_callpid ;
		    }
		    msgtype =
			    (ostate == D23_RBINVPEND || ostate == D23_INVPEND) ?
								D23_INVCMSG :
			    (ostate == D23_RBRESPEND ||
			    ((ostate == D23_REQPEND || ostate == D23_RESPEND) &&
							    oflg == D23_MORE)) ?
							      D23_RRSBFMSG : 0 ;
		    if (msgtype) {
		        /* abort pending IPC-operation by sending an */
			/* abort message to the server process */
			if (d23_snd_abort (d23_info.d23_msgid, msgtype,
				cltaptr->d23_cpid.d23_vpid, &cltaptr->d23_epid,
				i, cltaptr->d23_cassid) == D23_ERROR) {
			    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"),
			    GDS_S_GENERAL, svc_c_debug1, GDS_S_IPCMON_CLT3,
								       i + 1)) ;
			    continue ;
			}
			if (D23_GPEVM(cltaptr->d23_estate) == D23_FASYNC_EM) {
			    /* send attention byte through named FIFO */
			    d23_snd_att (instptr, D23_DSTPID,
						   cltaptr->d23_cpid.d23_rpid) ;
			}
		    } else {
		    	if ((cltaptr->d23_esid == D23_CSTSID ||
		 	    (cltaptr->d23_esid >= D23_SSTSID &&
		 	    cltaptr->d23_esid <= D23_SSTSID + D2MAXDIR_ID)) &&
		 	    cltaptr->d23_ucstate[0] == D23_RSTATE &&
		 	    atoi (&cltaptr->d23_ucstate[1]) >= 10) {
		    	    /* abort existing logical association by sending */
			    /* an abort message to the server process(es) */
			    if (d23_snd_abort (d23_info.d23_msgid, D23_INVMSG,
			    cltaptr->d23_esid, &cltaptr->d23_epid, i, 0) ==
								    D23_ERROR) {
				DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"),
				GDS_S_GENERAL, svc_c_debug1, GDS_S_IPCMON_CLT3,
								       i + 1)) ;
				continue ;
			    }

			    if (D23_GPEVM(cltaptr->d23_estate) == D23_FASYNC_EM) {
				/* send attention byte through named FIFO */
				if (D23_GSEST(cltptr->d23_estate) ==
								   D23_MPROCSVR)
				    d23_snd_att (instptr, D23_DSTSID,
							    cltaptr->d23_esid) ;
				else
				    d23_snd_att (instptr, D23_DSTPID,
						   cltaptr->d23_cpid.d23_rpid) ;
			    }
			}
		    }
		}
		/* remove process-ID specific unused messages */
		for (j = 0; j < 3; j++) {
		    msgtype = (j == 0) ? D23_RESMSG : (j == 1) ?
						    D23_RESCMSG : D23_RRQBFMSG ;
		    if (d23_msgrcv (d23_info.d23_msgid, &rcvmsg, D23_MSGBUFSZ,
					(signed32) (D23_SITYPE(msgtype) |
					D23_SDSTID(cltaptr->d23_epid.d23_vpid)),
					IPC_NOWAIT, &pbhead) == D23_ERROR) {
			if (pbhead.d2_errvalue != D23_RMSGERR + ENOMSG) {
			    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"),
			    GDS_S_GENERAL, svc_c_debug1, GDS_S_IPCMON_SVR2,
								       i + 1)) ;
			}
		    } else
			break ;
		}

		if (D23_GOEVM(cltaptr->d23_estate) != D23_SYNC_EM)
		    /* remove named FIFO */
		    d23_remove_fifo (instptr, D23_DSTPID,
						   cltaptr->d23_epid.d23_rpid) ;

		/* release client entry */	
	        cltaptr->d23_estate = D23_SES(D23_NOCLT) ;
	        d23_info.d23_genarea->d23_cno-- ;	

		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d"),
				GDS_S_GENERAL, svc_c_debug1, GDS_S_IPCMON_CLT,
				i + 1, cltaptr->d23_epid.d23_rpid,
				cltaptr->d23_epid.d23_vpid, errsav)) ;
	    } else {
		if (ostate == D23_RBINVPEND || ostate == D23_REQPEND) {
		    /* verify whether there is a server process which is */
		    /* able to handle the pending service request */
		    for (j = 0, svrhptr = svrptr; j < mxsvr; j++, svrhptr++) {
			if (D23_GES(svrhptr->d23_estate) == D23_ATTACH &&
			    D23_GSET(svrhptr->d23_estate) == D23_PRIMARY &&
			    svrhptr->d23_esid == cltaptr->d23_esid &&
				    kill (svrhptr->d23_epid.d23_rpid, 0) == 0) {
			    /* corresponding server entry found */
			    if ((D23_GEOS(svrhptr->d23_estate) != D23_ASSOC &&
				svrhptr->d23_cpid.d23_rpid == cltaptr->d23_epid.d23_rpid) ||
				  D23_GOEVM(svrhptr->d23_estate) == D23_SYNC_EM)
				break ;
			    if (D23_GSEST(svrhptr->d23_estate) == D23_SPROCSVR)
				if(cltaptr->d23_cpid.d23_rpid != svrhptr->d23_epid.d23_rpid)
				    /* adjust server information within */
				    /* client entry */
				    cltaptr->d23_cpid = svrhptr->d23_epid ;
				else
				    break ;
			}
		    }
		    if (j == mxsvr) {
			/* no server process available -> */
			/* remove server-ID specific unused message */
			if (d23_msgrcv (d23_info.d23_msgid, &rcvmsg,
			    D23_MSGBUFSZ, (signed32) (D23_SITYPE(D23_INVMSG) |
			    D23_SDSTID(svraptr->d23_esid)), IPC_NOWAIT,
							&pbhead) == D23_ERROR) {
			    if (pbhead.d2_errvalue != D23_RMSGERR + ENOMSG) {
				DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"),
				GDS_S_GENERAL, svc_c_debug1, GDS_S_IPCMON_SVR2,
								       i + 1)) ;
			    }
			    continue ;
			}

			/* send abort message to client process */
			msgtype = (ostate == D23_RBINVPEND) ? D23_RRQBFMSG :
								    D23_RESMSG ;
			if (d23_snd_abort (d23_info.d23_msgid, msgtype,
			    cltaptr->d23_epid.d23_vpid, &d23_info.d23_ipid,
							   i, 0) == D23_ERROR) {
			    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"),
			    GDS_S_GENERAL, svc_c_debug1, GDS_S_IPCMON_CLT3,
								       i + 1)) ;
			}
			if (D23_GOEVM(cltaptr->d23_estate) == D23_FASYNC_EM)
			    /* send attention byte through named FIFO */
			    d23_snd_att (instptr, D23_DSTPID,
						   cltaptr->d23_epid.d23_rpid) ;
			DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"),
			GDS_S_GENERAL, svc_c_debug1, GDS_S_IPCMON_CLT4, i + 1)) ;
		    }
		}
	    }
	}
	
	/* *** verify user credential area information *** */
	/* *********************************************** */
	credptr = d23_info.d23_crdarea ;
	for (i = 0; i < d23_info.d23_genarea->d23_mcred; i++, credptr++) {
	    if (credptr->d23_credusrid != D23_UNUSED) {
		/* look for an existing Server process */
		for (j = 0, svrhptr = svrptr; j < mxsvr; j++, svrhptr++)
		    if (D23_GES(svrhptr->d23_estate) == D23_ATTACH &&
			svrhptr->d23_esid == credptr->d23_credsvrid &&
				      kill (svrhptr->d23_epid.d23_rpid, 0) == 0)
			    break ;
		if (j < mxsvr) {
		    /* look for an existing Client process */
		    if (D23_GACM(credptr->d23_credcontid) == D27_REM_AC) {
			/* remote access entry -> */
			/* look for an existing S-Stub process */
			remid = D23_GRMID(credptr->d23_credcontid) ;
			for (j = 0, svrhptr = svrptr; j < mxsvr; j++, svrhptr++)
			    if (D23_GES(svrhptr->d23_estate) == D23_ATTACH &&
				svrhptr->d23_esid == remid &&
				      kill (svrhptr->d23_epid.d23_rpid, 0) == 0)
			    break ;
			if (j < mxsvr)
			    continue ;
		    } else {
			/* local access entry */
			if (D23_GES((cltaptr = cltptr +
			   credptr->d23_credusrid)->d23_estate) == D23_ATTACH &&
				      kill (cltaptr->d23_epid.d23_rpid, 0) == 0)
			continue ;
		    }
		}

		/* local access entry and no existing client process or */
		/* remote access entry and no existing S-Stub process -> */
		/* release user credential entry */
		d23_shmfree (credptr->d23_credoff) ;
		credptr->d23_credusrid = D23_UNUSED ;
		d23_info.d23_genarea->d23_cred-- ;
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
				      svc_c_debug1, GDS_S_IPCMON_DIST, i + 1)) ;
	    }
	}

	/* unlock shared memory area */
	if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	    exit (1) ;
	}
    }
}

/* **************************** SUBROUTINES ******************************** */

/* ---------------- change logging state (signal routine) ------------------ */

void siglogging (int signo) {

    boolean		confirm = TRUE;
    D23_dreadpb		drdpb ;
    D23_dconfpb		dcfpb ;
    error_status_t	result ;

    /* get distributed command */
    drdpb.d23_Khdinfo.d2_version = D23_V02 ;
    if (d23_read_dist_cmd (&drdpb) == D23_ERROR) {
	return ;
    }

    /* evaluate distributed command */
    switch (drdpb.d23_Kcmd) {
    case D23_DISLOG:
	/* disable DIR-X logging */
	d27_003_reset_routing ((char *) drdpb.d23_Kparref, &result) ;
	break;
    case D23_ENALOG:
	/* enable DIR-X logging */
	d27_003_reset_routing ((char *) drdpb.d23_Kparref, &result) ;
	break;
    case D23_SVCLOGLEVEL:
	/* command: change logging level */
	dce_svc_debug_set_levels (drdpb.d23_Kparref, &result) ;
	if (result != svc_s_ok) {
	    fprintf (stderr,
	     "Monitor: can't change logging level (status = %#x)\n", result) ;
	}
	break ;
    default:
	/* unknown distributed command */
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			    svc_c_debug1, GDS_S_ILL_DIST_CMD, drdpb.d23_Kcmd)) ;
	confirm = FALSE ;
	break ;
    }

    if (confirm ) {
	/* confirm received distributed command */
	dcfpb.d23_Lhdinfo.d2_version = D23_V02 ;
	dcfpb.d23_Lkey = drdpb.d23_Kkey;
	if (d23_conf_dist_cmd (&dcfpb) == D23_ERROR) {
	    return ;
	}
    }
}

/* --------------- terminate process (signal routine) ---------------------- */

void sigterm (int signo) {

#if defined THREADSAFE
    unsigned32		status ;
    
    if (svc_rpc_server_active) {
	/* stop serviceability RPC-server */
	/* (Note: process termination is executed by RPC-server code) */
	rpc_mgmt_stop_server_listening ((rpc_binding_handle_t) NULL, &status) ;
	if (status != rpc_s_ok) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			     svc_c_sev_warning, GDS_S_RPC_RUNTIME_ERR, status) ;
	    exit (1) ;
	}
	return ;
    }
#endif
    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
					  svc_c_sev_notice, GDS_S_EXITMONITOR) ;
    exit (0) ;
}

/* --- check existence of a client-/server process --- */

signed32 d23_cproc_check (D23_ipcentry *earea, signed32 mxassid, signed32 svrid,
								signed32 epid) {

    D23_ipcentry	*eptr ;
    signed32		assid ;

    for (assid = 0, eptr = earea; assid < mxassid; assid++, eptr++) {
	if (D23_GES(eptr->d23_estate) == D23_ATTACH &&
	    eptr->d23_esid == svrid && (epid == D23_UNUSED ||
	    eptr->d23_epid.d23_rpid == epid) &&
	    kill (eptr->d23_epid.d23_rpid, 0) == 0)
	    break ;
    }
    return ((assid == mxassid) ? -1 : assid) ;
}


/* --- remove (unlink) named FIFO --- */

signed32 d23_remove_fifo (char *instname, signed32 fifotype, signed32 fifoid) {

    char	fname[D27_LFILE_NAME] ;

    sprintf (fname, D23_FIFO_FILE1, instname, (fifotype == D23_DSTPID) ? 'p' :
								  's', fifoid) ;
    unlink (fname) ;
}


/* --- send abort message --- */

signed32 d23_snd_abort (signed32 msgid, signed32 msgtype, signed32 dstid,
		      D23_rvpid *srcid, signed32 callassid, signed32 cldassid) {

    D23_ipcmsg	abomsg ;

    abomsg.d23_mtype = D23_SITYPE(msgtype) | D23_SDSTID(dstid) ;
    abomsg.d23_callpid = *srcid ;
    abomsg.d23_iassid = D23_SCLDASS(cldassid) | D23_SCALASS(callassid) ;
    abomsg.d23_msgsz =
    abomsg.d23_invinfo.d23_invid =
    abomsg.d23_invinfo.d23_usrid =
    abomsg.d23_invinfo.d23_contid = 0 ;
    abomsg.d23_opid = D23_SIOF(D23_END) | D23_SIOI(D23_ABORT) ;
    return (d23_msgsnd (msgid, &abomsg, D23_MSGBUFSZ, IPC_NOWAIT)) ;
}


/* --- send attention byte through a named FIFO --- */

signed32 d23_snd_att (char *instname, signed32 fifotype, signed32 fifoid) {

    char	fname[D27_LFILE_NAME] ;
    byte	attbyte = 0 ;
    signed32	fd ;

    sprintf (fname, D23_FIFO_FILE1, instname, (fifotype == D23_DSTPID) ? 'p' :
								  's', fifoid) ;
    if ((fd = open (fname, O_WRONLY | O_NONBLOCK)) < 0) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d%d"),
                        GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_OPEN_ERR,
                                            fname, O_WRONLY|O_NONBLOCK, errno) ;
	return (-1) ;
    }
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s%d%d"), GDS_S_GENERAL,
	     svc_c_debug6, GDS_S_OPEN_FIFO, fname, fd, O_WRONLY | O_NONBLOCK)) ;

    if (write (fd, (void *) &attbyte, (size_t) 1) < 0) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"),
		       GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_WRITE_ERR, errno) ;
	return (-1) ;
    }
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
					    svc_c_debug6, GDS_S_IPCWRATT, fd)) ;

    close (fd) ;
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
					  svc_c_debug6, GDS_S_CLOSE_FIFO, fd)) ;
    return (0) ;
}

