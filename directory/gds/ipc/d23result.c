/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23result.c,v $
 * Revision 1.1.10.2  1996/02/18  18:19:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:32  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:03:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:15:13  root]
 * 
 * Revision 1.1.8.5  1994/09/06  12:25:30  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:15:51  keutel]
 * 
 * Revision 1.1.8.4  1994/06/21  14:47:28  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:42:17  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:56:35  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:40:30  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:18:58  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:31:54  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:40:46  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:53:35  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:16:23  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:27:07  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  20:45:39  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:32:32  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:32:21  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  14:06:42  marrek]
 * 
 * Revision 1.1.2.3  1992/07/01  16:53:31  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:22:33  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  20:55:22  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:41:16  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23result.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:19:46 $";
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
/*   NAME:    d23_result ()                                                  */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function returns the result which corresponds to an invoke      */
/*       request to a client.                                                */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       rsrq_pb     = Reference to the function parameter block which       */
/*                     must contain the following input parameter:           */
/*                             - IPC-version number                          */
/*                             - association-ID                              */
/*                             - context-ID                                  */
/*                             - user-ID                                     */
/*                             - result type                                 */
/*                             - more/end data flag                          */
/*                             - result data reference                       */
/*                             - result data length                          */
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

#include <gds.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>


/* ------------------- DECLARATION OF GLOBAL DATA -------------------------- */

extern D23_iprocinfo	d23_info ;	/* process specific IPC-information */

/* ------------------------------------------------------------------------- */

signed32 d23_result (D23_rsrqpb *rsrq_pb) {

    signed32		evmode, assid, myassid, ostate, result, rstype ;
    D23_ipcentry	*svrptr ;
    D23_ipcmsg		resmsg ;
    D23_evntpb		evntpb ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d%d%d%d%d"), GDS_S_IPC,
	svc_c_debug1, GDS_S_SRESULT_ENTRY, rsrq_pb->d23_3assid,
	rsrq_pb->d23_3invinfo.d23_invid, rsrq_pb->d23_3invinfo.d23_usrid,
        rsrq_pb->d23_3invinfo.d23_contid, rsrq_pb->d23_3rltid,
	rsrq_pb->d23_3rmdat, rsrq_pb->d23_3rfrdt, rsrq_pb->d23_3lnrdt)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, rsrq_pb->d23_3hdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, rsrq_pb->d23_3hdinfo.d2_version) ;
	return (d23_ipcerror (&rsrq_pb->d23_3hdinfo, D23_VERSIL)) ;
    }

    /* check global server state */
    if ((D23_GIAS(d23_info.d23_istate) & D23_ISVRATT) != D23_ISVRATT) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	   svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	return (d23_ipcerror (&rsrq_pb->d23_3hdinfo, D23_RSRQIL)) ;
    }

    /* check association-ID parameter */
    assid = rsrq_pb->d23_3assid - D23_MXCLTPROC ;
    if (assid < 0 || assid > d23_info.d23_genarea->d23_mcno) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		    svc_c_sev_fatal, GDS_S_IPCASIDIL_ERR, rsrq_pb->d23_3assid) ;
	return (d23_ipcerror (&rsrq_pb->d23_3hdinfo, D23_ASIDIL)) ;
    }

    /* check association specific server state */
    for (myassid = 0, svrptr = d23_info.d23_svrarea;
		  myassid < d23_info.d23_genarea->d23_msno; myassid++, svrptr++)
	if (D23_GES(svrptr->d23_estate) == D23_ATTACH &&
	    svrptr->d23_esid == d23_info.d23_isvrid &&
	    svrptr->d23_epid.d23_rpid == d23_info.d23_ipid.d23_rpid &&
	    svrptr->d23_cassid == assid)
	    break ;
    if (myassid == d23_info.d23_genarea->d23_msno ||
	((ostate = D23_GEOS(svrptr->d23_estate)) != D23_REQPEND &&
						       ostate != D23_RESPEND)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		      svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, svrptr->d23_estate) ;
	return (d23_ipcerror (&rsrq_pb->d23_3hdinfo, D23_RSRQIL)) ;
    }
	
    /* check result type parameter */
    if (((rstype = rsrq_pb->d23_3rltid) != D23_RESULT && rstype != D23_ABORT &&
	(rstype < D23_RERROR || rstype > D23_RERROR + D23_MXRERR) &&
	(rstype < D23_RREJECT || rstype > D23_RREJECT + D23_MXRREJ)) ||
	(rstype != D23_RESULT && rstype != D23_ABORT && ostate != D23_REQPEND)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
				 svc_c_sev_fatal, GDS_S_IPCRSTYIL_ERR, rstype) ;
	return (d23_ipcerror (&rsrq_pb->d23_3hdinfo, D23_RSTYIL)) ;
    }

    /* check invoke information parameter */
    if (rsrq_pb->d23_3invinfo.d23_invid != svrptr->d23_invinfo.d23_invid ||
	rsrq_pb->d23_3invinfo.d23_usrid != svrptr->d23_invinfo.d23_usrid) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_IPC,
	   svc_c_sev_fatal, GDS_S_IPCINVIL_ERR, rsrq_pb->d23_3invinfo.d23_invid,
	   rsrq_pb->d23_3invinfo.d23_usrid, rsrq_pb->d23_3invinfo.d23_contid) ;
	return (d23_ipcerror (&rsrq_pb->d23_3hdinfo, D23_INVERR)) ;
    }

    if (rstype != D23_ABORT) {
	/* check fragment type parameter */
	if ((rsrq_pb->d23_3rmdat != D23_LASTFG &&
	     rsrq_pb->d23_3rmdat != D23_MRDATA) ||
	    (rstype != D23_RESULT && rsrq_pb->d23_3rmdat == D23_MRDATA)) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		    svc_c_sev_fatal, GDS_S_IPCMTYPIL_ERR, rsrq_pb->d23_3rmdat) ;
	    return (d23_ipcerror (&rsrq_pb->d23_3hdinfo, D23_MTYPIL)) ;
	}

	/* check memory reference parameter */
	if (rsrq_pb->d23_3rfrdt != (byte *) d23_info.d23_bufarea +
				    (d23_info.d23_genarea->d23_bsize * assid)) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		    svc_c_sev_fatal, GDS_S_IPCMREFIL_ERR, rsrq_pb->d23_3rfrdt) ;
	    return (d23_ipcerror (&rsrq_pb->d23_3hdinfo, D23_MREFIL)) ;
	}

	/* check message length parameter */
	if (rsrq_pb->d23_3lnrdt < 0 ||
			rsrq_pb->d23_3lnrdt > d23_info.d23_genarea->d23_bsize) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCILNIL_ERR, rsrq_pb->d23_3lnrdt) ;
	    return (d23_ipcerror (&rsrq_pb->d23_3hdinfo, D23_RLNERR)) ;
	}

	resmsg.d23_opid = D23_SIOF((rsrq_pb->d23_3rmdat == D23_LASTFG) ?
					D23_END : D23_MORE) | D23_SIOI(rstype) ;
	resmsg.d23_msgsz = rsrq_pb->d23_3lnrdt ;
    } else {
	resmsg.d23_msgsz = 0 ;
	resmsg.d23_opid = D23_SIOF(D23_END) | D23_SIOI(rstype) ;
    }

    /* change operational state to 'result operation pending', means */
    /* an abandon request for this operation is not longer possible */
    if (ostate == D23_REQPEND) {
	if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	    return (d23_ipcerror (&rsrq_pb->d23_3hdinfo, D23_LOSEMA)) ;
	}
	D23_CEOS(svrptr->d23_estate, D23_RESPEND) ;

	if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	    return (d23_ipcerror (&rsrq_pb->d23_3hdinfo, D23_ULSEMA)) ;
	}
    }

    /* send result message */
    resmsg.d23_mtype = D23_SITYPE((ostate == D23_REQPEND) ?
		      D23_RESMSG :
		      D23_RESCMSG) | D23_SDSTID(svrptr->d23_cpid.d23_vpid) ;
    resmsg.d23_callpid = svrptr->d23_epid ;
    resmsg.d23_invinfo = rsrq_pb->d23_3invinfo ;
    resmsg.d23_iassid = D23_SCLDASS(assid) | D23_SCALASS(myassid) ;
    if (d23_msgsnd (d23_info.d23_msgid, &resmsg, D23_MSGBUFSZ, IPC_NOWAIT) ==
								    D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSMSG_ERR) ;
        return (d23_ipcerror (&rsrq_pb->d23_3hdinfo, D23_SMSGERR)) ;
    }

    if ((evmode = D23_GPEVM(svrptr->d23_estate)) == D23_FASYNC_EM ||
	(evmode == D23_PASYNC_EM && ostate == D23_REQPEND)) {
	/* send attention byte trough named FIFO */
	if (d23_attsnd (svrptr->d23_cpid.d23_rpid, D23_DSTPID,
					  &rsrq_pb->d23_3hdinfo) == D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					  svc_c_sev_fatal, GDS_S_IPCSSATT_ERR) ;
	    return (D23_ERROR) ;
	}
    }

    evntpb.d23_Bevtype = D23_NOEVNT ;
    if (D23_GIOF(resmsg.d23_opid) == D23_END) {
	D23_CEOS(svrptr->d23_estate, D23_ASSOC) ;
	svrptr->d23_cassid = D23_UNUSED ;
	svrptr->d23_invinfo.d23_invid = D23_UNUSED ;
	svrptr->d23_cpid.d23_rpid = svrptr->d23_cpid.d23_vpid = D23_UNUSED ;
    } else {
	D23_CEOS(svrptr->d23_estate, D23_RBRESPEND) ;
	if (D23_GIEVM(d23_info.d23_istate) != D23_FASYNC_EM) {
	    /* wait for a return buffer event from the message queue */
	    evntpb.d23_Bhdinfo.d2_version = D23_V02 ;
	    while ((result = d23_ievent (&evntpb, D23_INFIN,
			    		 (1 << D23_RRSBFMSG))) == D23_SUCES &&
					 (evntpb.d23_Bevtype != D23_RRSBFEVNT &&
					 evntpb.d23_Bevtype != D23_ABOEVNT)) ;
	    if (result == D23_ERROR) {
		rsrq_pb->d23_3hdinfo = evntpb.d23_Bhdinfo ;
		return (D23_ERROR) ;
	    }
	}
    }
    if (D23_GEOS(svrptr->d23_estate) == D23_ASSOC &&
					       svrptr != d23_info.d23_primref) {
	/* release additional server entry */
	/* lock shared memory area */
	if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	    return (d23_ipcerror (&rsrq_pb->d23_3hdinfo, D23_LOSEMA)) ;
	}
	svrptr->d23_estate = D23_SES(D23_NOSVR) ;
	d23_info.d23_genarea->d23_sno-- ;

	/* unlock shared memory area */
	if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	    return (d23_ipcerror (&rsrq_pb->d23_3hdinfo, D23_ULSEMA)) ;
	}
    }
    if (evntpb.d23_Bevtype == D23_ABOEVNT)
	return (d23_ipcerror (&rsrq_pb->d23_3hdinfo, D23_ABOERR)) ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					    svc_c_debug1, GDS_S_SRESULT_EXIT)) ;
    return (D23_SUCES) ;
}
