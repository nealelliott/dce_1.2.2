/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23event.c,v $
 * Revision 1.1.770.2  1996/02/18  18:19:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:20  marty]
 *
 * Revision 1.1.770.1  1995/12/08  16:03:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:15:00  root]
 * 
 * Revision 1.1.768.4  1994/09/06  12:25:20  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:15:01  keutel]
 * 
 * Revision 1.1.768.3  1994/05/10  15:56:24  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:39:52  marrek]
 * 
 * Revision 1.1.768.2  1994/03/23  15:18:52  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:31:37  keutel]
 * 
 * Revision 1.1.768.1  1994/02/22  19:21:33  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:52:00  marrek]
 * 
 * Revision 1.1.766.2  1993/10/14  17:35:53  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:33:47  keutel]
 * 
 * Revision 1.1.766.1  1993/10/13  17:33:09  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:55:50  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  12:06:14  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:25:21  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:44:38  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:31:58  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:53:37  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:40:24  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23event.c,v $ $Revision: 1.1.770.2 $ $Date: 1996/02/18 18:19:34 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d23_event ()                                                   */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function must be used to look up for IPC-events if an           */
/*       partially or fully asynchronous event mode is enabled.              */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       evnd_pb     = Reference to the function parameter block which       */
/*                     must contain the following input parameter:           */
/*                             - IPC-version number                          */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     The function parameter block contains the output      */
/*		       parameter as follows:                                 */
/*                             - event type                                  */
/*                             - association-ID                1)            */
/*                             - context-ID                    1) 2)         */
/*                             - user-ID                       1) 2)         */
/*                             - request/result operation-ID   1) 2) 3)      */
/*                             - more/end of data flag         1) 2) 3)      */
/*                             - length of received data       1) 2) 3)      */
/*                             - reference to received data    1) 2) 3)      */
/*                                                                           */
/*                1) output parameter is invalid if the event type is        */
/*                   equal to D23_NOEVNT                                     */
/*                2) output parameter is invalid if the event type is        */
/*                   equal to D23_RBFEVNT                                    */
/*                3) output parameter is invalid if the event type is        */
/*                   equal to D23_ABOEVNT or D23_REJEVNT                     */
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

#include <gds.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>


/* ------------------- DECLARATION OF GLOBAL DATA -------------------------- */

extern D23_iprocinfo	d23_info ;	/* process specific IPC-information */

/* ------------------------------------------------------------------------- */

signed32 d23_event (D23_evntpb *evnt_pb) {

    signed32 result ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
		     			   svc_c_debug1, GDS_S_CSEVENT_ENTRY)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, evnt_pb->d23_Bhdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, evnt_pb->d23_Bhdinfo.d2_version) ;
	return (d23_ipcerror (&evnt_pb->d23_Bhdinfo, D23_VERSIL)) ;
    }

    /* check global client-/server state */
    if (D23_GIAS(d23_info.d23_istate) == D23_INATTACH ||
				D23_GIEVM(d23_info.d23_istate) == D23_SYNC_EM) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	return (d23_ipcerror (&evnt_pb->d23_Bhdinfo, D23_EVNTIL)) ;
    }

    /* check whether there is any relevant event in the message queue */
    if ((result = d23_ievent (evnt_pb, D23_CHECK, d23_info.d23_enaevmsk)) ==
								    D23_SUCES) {
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d%d%d%d%d%d"),
	GDS_S_IPC, svc_c_debug1, GDS_S_CSEVENT_EXIT, evnt_pb->d23_Bevtype,
	evnt_pb->d23_Bassid, evnt_pb->d23_Binvinfo.d23_invid,
	evnt_pb->d23_Binvinfo.d23_usrid, evnt_pb->d23_Binvinfo.d23_contid,
	evnt_pb->d23_Bopid, evnt_pb->d23_Bmdat, evnt_pb->d23_Bref,
	evnt_pb->d23_Bln)) ;
    }
    return (result) ;
}

/* ------------------------------------------------------------------------- */

signed32 d23_ievent (D23_evntpb *evnt_pb, signed32 waitmode, signed32 evmask) {

    static signed32	nxtrcvmtype = 0 ;
    signed32		i, assid, opid, opflg ;
    signed32		evmode, svrtype, rcvmtype ;
    D23_ipcentry	*svrptr, *cltptr ;
    D23_ipcmsg		rejmsg, rcvmsg ;
    byte		attbyte ;
    int			attfd ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     	     svc_c_debug4, GDS_S_CSIEVENT_ENTRY, nxtrcvmtype)) ;

    /* check whether there is any relevant event in the message queue */
    /* (this is done by using a round robin method) */
    rcvmtype = nxtrcvmtype = (nxtrcvmtype % D23_MXEVNT) + 1 ;
    for (i = 0; i < D23_MXEVNT; i++, rcvmtype = (rcvmtype % D23_MXEVNT) + 1) {
	if ((evmask & (1 << rcvmtype)) == 0)
	    /* event type not enabled */
	    continue ;

	if (d23_msgrcv (d23_info.d23_msgid, &rcvmsg, D23_MSGBUFSZ,
	    (signed32) (D23_SITYPE(rcvmtype) |
	    D23_SDSTID((rcvmtype == D23_INVMSG) ?
	    d23_info.d23_isvrid : d23_info.d23_ipid.d23_vpid)),
	    (waitmode == D23_CHECK) ? IPC_NOWAIT : 0,
					  &evnt_pb->d23_Bhdinfo) == D23_ERROR) {
	    if (evnt_pb->d23_Bhdinfo.d2_errvalue == D23_RMSGERR + ENOMSG)
		continue ;
	    if (evnt_pb->d23_Bhdinfo.d2_errvalue == D23_RMSGERR + EINTR) {
		i = D23_MXEVNT ;
		break ;
	    }
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCRMSG_ERR) ;
	    return (D23_ERROR) ;
	} else
	    break ;
    }

    if (i == D23_MXEVNT) {
	/* no event available */
	evnt_pb->d23_Bevtype = D23_NOEVNT ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
		     			   svc_c_debug4, GDS_S_CSIEVENT_EXIT)) ;
	return (D23_SUCES) ;
    }

    /* IPC-message received */
    /* get attention byte from named FIFO */
    if ((evmode = D23_GIEVM(d23_info.d23_istate)) == D23_FASYNC_EM ||
	(evmode == D23_PASYNC_EM && (rcvmtype == D23_INVMSG ||
						     rcvmtype == D23_RESMSG))) {
	svrtype = D23_GISTY(d23_info.d23_istate) ;
	attfd = (rcvmtype == D23_INVMSG && svrtype == D23_MPROCSVR) ? 
				       d23_info.d23_sevfd : d23_info.d23_pevfd ;
	if (read (attfd, (void *) &attbyte, (size_t) 1) < 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_IPC,
			    svc_c_sev_fatal, GDS_S_IPCSRATT_ERR, attfd, errno) ;
	    return (d23_ipcerror (&evnt_pb->d23_Bhdinfo, D23_IFFRED)) ;
	}
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     			 svc_c_debug2, GDS_S_IPCRDATT, attfd)) ;
    }
					
    /* evaluate received event and supply result parameter */
    assid = D23_GCLDASS(rcvmsg.d23_iassid) ;
    evnt_pb->d23_Binvinfo = rcvmsg.d23_invinfo ;
    evnt_pb->d23_Bopid = opid = D23_GIOI(rcvmsg.d23_opid) ;
    opflg = D23_GIOF(rcvmsg.d23_opid) ;
    evnt_pb->d23_Bmdat = (opflg == D23_END) ? D23_LASTFG : D23_MRDATA ;
    evnt_pb->d23_Bln = rcvmsg.d23_msgsz ;

    if (rcvmtype == D23_INVMSG) {
	if (opid == D23_ABORT) {
	    evnt_pb->d23_Bassid = D23_GCALASS(rcvmsg.d23_iassid) +
								 D23_MXCLTPROC ;
	    evnt_pb->d23_Bevtype = D23_ABOEVNT ;

	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
		     			   svc_c_debug4, GDS_S_CSIEVENT_EXIT)) ;
	    return (D23_SUCES) ;
	}

	if (D23_GEOS(d23_info.d23_primref->d23_estate) != D23_ASSOC) {
	    /* parallel service request -> occupy free server entry */
	    /* lock shared memory area */
	    if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
		return (d23_ipcerror (&evnt_pb->d23_Bhdinfo, D23_LOSEMA)) ;
	    }
	    if (d23_info.d23_genarea->d23_sno == d23_info.d23_genarea->d23_msno) {
		/* no free server entry available */
		if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
		    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
		    return (d23_ipcerror (&evnt_pb->d23_Bhdinfo, D23_ULSEMA)) ;

		}
		/* send reject message */
		rejmsg.d23_mtype = D23_SDSTID(rcvmsg.d23_callpid.d23_vpid) |
		    D23_SITYPE((opflg == D23_END) ? D23_RESMSG : D23_RRQBFMSG) ;
		rejmsg.d23_callpid = d23_info.d23_ipid ;
		rejmsg.d23_iassid =
			 D23_SCLDASS(D23_GCALASS(rcvmsg.d23_iassid)) ;
		rejmsg.d23_msgsz = 0 ;
		rejmsg.d23_invinfo = rcvmsg.d23_invinfo ;
		rejmsg.d23_opid = D23_SIOF(D23_END) | D23_SIOI(D23_RREJECT) ;
		if (d23_msgsnd (d23_info.d23_msgid, &rejmsg, D23_MSGBUFSZ,
						     IPC_NOWAIT) == D23_ERROR) {
		    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSMSG_ERR) ;
		    return (d23_ipcerror (&evnt_pb->d23_Bhdinfo, D23_SMSGERR)) ;
		}

		if (D23_GIOEVM(rcvmsg.d23_opid) == D23_FASYNC_EM) {
		    /* send attention byte through named FIFO */
		    if (d23_attsnd (rcvmsg.d23_callpid.d23_rpid, D23_DSTPID,
					  &evnt_pb->d23_Bhdinfo) == D23_ERROR) {
			dce_svc_printf (DCE_SVC (gds_svc_handle, ""),
			   GDS_S_IPC, svc_c_sev_fatal, GDS_S_IPCSSATT_ERR) ;
			return (D23_ERROR) ;
		    }
		}

		evnt_pb->d23_Bevtype = D23_NOEVNT ;

		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
		     			   svc_c_debug4, GDS_S_CSIEVENT_EXIT)) ;
		return (D23_SUCES) ;
	    }
	    svrptr = d23_info.d23_svrarea ;
	    for (assid = 0; assid < d23_info.d23_genarea->d23_msno; assid++,
								       svrptr++)
		if (D23_GES(svrptr->d23_estate) == D23_NOSVR)
		    break ;

	    /* occupy free server entry */
	    strcpy (svrptr->d23_ucstate, D23_NOSTATE) ;
	    svrptr->d23_epid = d23_info.d23_primref->d23_epid ;
	    svrptr->d23_esid = d23_info.d23_isvrid ;
	    svrptr->d23_estate = D23_SES(D23_ATTACH) | D23_SEOS(D23_ASSOC) |
				 D23_SOEVM(evmode) | D23_SSET(D23_SECONDARY) |
				 D23_SSEST(svrtype) ;
	    d23_info.d23_genarea->d23_sno++ ;

	    /* unlock shared memory */
	    if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
		return (d23_ipcerror (&evnt_pb->d23_Bhdinfo, D23_LOSEMA)) ;
	    }
	} else
	    assid = d23_info.d23_prassid ;
    }

    evnt_pb->d23_Bassid = assid ;
    svrptr = d23_info.d23_svrarea + assid ;
    cltptr = d23_info.d23_cltarea + assid ;
    if (opid == D23_ABORT) {
	if (rcvmtype == D23_INVCMSG || rcvmtype == D23_RRSBFMSG) {
	    if (svrptr != d23_info.d23_primref) {
		/* release additional server entry */
		/* lock shared memory area */
		if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
		    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
		    return (d23_ipcerror (&evnt_pb->d23_Bhdinfo, D23_LOSEMA)) ;
		}
		svrptr->d23_estate = D23_SES(D23_NOSVR) ;
		d23_info.d23_genarea->d23_sno-- ;

		/* unlock shared memory area */
		if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
		    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
		    return (d23_ipcerror (&evnt_pb->d23_Bhdinfo, D23_ULSEMA)) ;
		}
	    } else {
		D23_CEOS(svrptr->d23_estate, D23_ASSOC) ;
		svrptr->d23_invinfo.d23_invid =
		svrptr->d23_invinfo.d23_usrid =
		svrptr->d23_invinfo.d23_contid = D23_UNUSED ;
		svrptr->d23_cassid = D23_UNUSED ;
		svrptr->d23_cpid.d23_rpid = svrptr->d23_cpid.d23_vpid =
								    D23_UNUSED ;
	    }
	    d23_info.d23_slstref = (D23_ipcentry *) NULL ;
	    evnt_pb->d23_Bassid = D23_GCALASS(rcvmsg.d23_iassid) +
								 D23_MXCLTPROC ;
	} else {
	    D23_CEOS(cltptr->d23_estate, D23_ASSOC) ;
	    cltptr->d23_cassid = D23_UNUSED ;
	    if (D23_GPEVM(cltptr->d23_estate) == D23_SYNC_EM ||
				D23_GSEST(cltptr->d23_estate) != D23_SPROCSVR) {
		cltptr->d23_cpid.d23_rpid = cltptr->d23_cpid.d23_vpid =
								    D23_UNUSED ;
	    }
	    d23_info.d23_clstref = (D23_ipcentry *) NULL ;
	}

	evnt_pb->d23_Bevtype = D23_ABOEVNT ;

	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
		     			   svc_c_debug4, GDS_S_CSIEVENT_EXIT)) ;
	return (D23_SUCES) ;
    }

    switch (rcvmtype) {
    case D23_RRSBFMSG:
	D23_CEOS(svrptr->d23_estate, D23_RESPEND) ;
	evnt_pb->d23_Bassid = D23_GCALASS(rcvmsg.d23_iassid) + D23_MXCLTPROC ;
	break ;
    case D23_RRQBFMSG:
	D23_CEOS(cltptr->d23_estate, D23_INVPEND) ;
	cltptr->d23_cpid = rcvmsg.d23_callpid ;
	cltptr->d23_cassid = D23_GCALASS(rcvmsg.d23_iassid) ;
	break ;
    case D23_INVMSG:
    case D23_INVCMSG:
	if (rcvmtype == D23_INVMSG) {
	    D23_CPEVM(svrptr->d23_estate, D23_GIOEVM(rcvmsg.d23_opid)) ;
	    D23_CABND(svrptr->d23_estate) ;
	    D23_CDABND(svrptr->d23_estate) ;
	    svrptr->d23_invinfo = rcvmsg.d23_invinfo ;
	    svrptr->d23_oprid = opid ;
	    svrptr->d23_cpid = rcvmsg.d23_callpid ;
	    svrptr->d23_cassid = D23_GCALASS (rcvmsg.d23_iassid) ;
	    if (evmode != D23_FASYNC_EM && opflg == D23_MORE)
		d23_info.d23_slstref = svrptr ;
	}
	if (opflg == D23_END) {
	    D23_CEOS(svrptr->d23_estate, D23_REQPEND) ;
	    d23_info.d23_slstref = (D23_ipcentry *) NULL ;
	} else
	    D23_CEOS(svrptr->d23_estate, D23_RBINVPEND) ;
	evnt_pb->d23_Bassid = D23_GCALASS(rcvmsg.d23_iassid) + D23_MXCLTPROC ;
	evnt_pb->d23_Bref = (byte *) (d23_info.d23_bufarea +
	   (d23_info.d23_genarea->d23_bsize * D23_GCALASS(rcvmsg.d23_iassid))) ;
	break ;
    default:
	/* D23_RESMSG/D23_RESCMSG */
	if (rcvmtype == D23_RESMSG) {
	    cltptr->d23_cpid = rcvmsg.d23_callpid ;
	    cltptr->d23_cassid = D23_GCALASS (rcvmsg.d23_iassid) ;
	    if (evmode != D23_FASYNC_EM && opflg == D23_MORE)
		d23_info.d23_clstref = cltptr ;
	}
	if (opflg == D23_END) {
	    D23_CEOS(cltptr->d23_estate, D23_ASSOC) ;
	    cltptr->d23_invinfo.d23_invid =
	    cltptr->d23_cassid = D23_UNUSED ;
	    if (D23_GPEVM(cltptr->d23_estate) == D23_SYNC_EM ||
				D23_GSEST(cltptr->d23_estate) != D23_SPROCSVR) {
		cltptr->d23_cpid.d23_rpid = cltptr->d23_cpid.d23_vpid =
								    D23_UNUSED ;
	    }
	    d23_info.d23_clstref = (D23_ipcentry *) NULL ;
	} else
	    D23_CEOS(cltptr->d23_estate, D23_RBRESPEND) ;
	evnt_pb->d23_Bref = (byte *) (d23_info.d23_bufarea +
	   (d23_info.d23_genarea->d23_bsize * D23_GCLDASS(rcvmsg.d23_iassid))) ;
	break ;
    }
    evnt_pb->d23_Bevtype = rcvmtype ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
		     			   svc_c_debug4, GDS_S_CSIEVENT_EXIT)) ;
    return (D23_SUCES) ;
}
