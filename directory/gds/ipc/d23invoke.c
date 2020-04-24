/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23invoke.c,v $
 * Revision 1.1.10.2  1996/02/18  18:19:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:23  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:03:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:15:03  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:25:22  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:15:11  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:56:27  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:40:01  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  19:21:36  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:52:20  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:08:10  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:25:43  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:44:55  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:32:05  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:53:59  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:40:35  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23invoke.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:19:37 $";
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
/*   NAME:    d23_invoke ()                                                  */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function sends an invoke request to a server.                   */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       inrq_pb     = Reference to the function parameter block which       */
/*                     must contain the following input parameter:           */
/*                             - IPC-version number                          */
/*                             - association-ID                              */
/*                             - context-ID                                  */
/*                             - user-ID                                     */
/*                             - operation-ID                                */
/*                             - more/end data flag                          */
/*                             - invoke data reference                       */
/*                             - invoke data length                          */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*       inrq_pb      = invoke-ID                                            */
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

signed32 d23_invoke (D23_inrqpb *inrq_pb) {

    signed32		evmode, assid, ostate, result, oprid ;
    signed32		msgdstid, attdstid, dstidtype ;
    D23_ipcentry	*cltptr ;
    D23_ipcmsg		invmsg ;
    D23_evntpb		evntpb ;
    D23_invinfo		invinfo ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d%d%d%d%d"), GDS_S_IPC,
	svc_c_debug1, GDS_S_CINVOKE_ENTRY, inrq_pb->d23_6assid,
	inrq_pb->d23_6invinfo.d23_invid, inrq_pb->d23_6invinfo.d23_usrid,
	inrq_pb->d23_6invinfo.d23_contid, inrq_pb->d23_6oprid,
	inrq_pb->d23_6imdat, inrq_pb->d23_6rfidt, inrq_pb->d23_6lnidt)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, inrq_pb->d23_6hdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, inrq_pb->d23_6hdinfo.d2_version) ;
	return (d23_ipcerror (&inrq_pb->d23_6hdinfo, D23_VERSIL)) ;
    }

    /* check global client state */
    if ((D23_GIAS(d23_info.d23_istate) & D23_ICLTATT) != D23_ICLTATT) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	   svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	return (d23_ipcerror (&inrq_pb->d23_6hdinfo, D23_INRQIL)) ;
    }

    /* check association-ID parameter */
    if ((assid = inrq_pb->d23_6assid) < 0 ||
				       assid > d23_info.d23_genarea->d23_mcno) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
				  svc_c_sev_fatal, GDS_S_IPCASIDIL_ERR, assid) ;
	return (d23_ipcerror (&inrq_pb->d23_6hdinfo, D23_ASIDIL)) ;
    }

    /* check association specific client state */
    cltptr = d23_info.d23_cltarea + assid ;
    if (D23_GES(cltptr->d23_estate) != D23_ATTACH ||
	cltptr->d23_epid.d23_rpid != d23_info.d23_ipid.d23_rpid ||
	((ostate = D23_GEOS(cltptr->d23_estate)) != D23_ASSOC &&
						       ostate != D23_INVPEND)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		      svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, cltptr->d23_estate) ;
	return (d23_ipcerror (&inrq_pb->d23_6hdinfo, D23_INRQIL)) ;
    }

    /* check invoke information parameter */
    if (ostate == D23_INVPEND &&
	(inrq_pb->d23_6invinfo.d23_invid != cltptr->d23_invinfo.d23_invid ||
	inrq_pb->d23_6invinfo.d23_usrid != cltptr->d23_invinfo.d23_usrid ||
	inrq_pb->d23_6invinfo.d23_contid != cltptr->d23_invinfo.d23_contid)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_IPC,
	   svc_c_sev_fatal, GDS_S_IPCINVIL_ERR, inrq_pb->d23_6invinfo.d23_invid,
	   inrq_pb->d23_6invinfo.d23_usrid, inrq_pb->d23_6invinfo.d23_contid) ;
	return (d23_ipcerror (&inrq_pb->d23_6hdinfo, D23_INVERR)) ;
    }
	
    if ((oprid = inrq_pb->d23_6oprid) != D23_ABORT) {
	/* check operation-ID parameter */
	if (oprid <= 0 || oprid > D23_MXUOPID) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
				  svc_c_sev_fatal, GDS_S_IPCOPIDIL_ERR, oprid) ;
	    return (d23_ipcerror (&inrq_pb->d23_6hdinfo, D23_OPIDIL)) ;
	}
	/* check fragment type parameter */
	if (inrq_pb->d23_6imdat != D23_LASTFG &&
					    inrq_pb->d23_6imdat != D23_MRDATA) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		    svc_c_sev_fatal, GDS_S_IPCMTYPIL_ERR, inrq_pb->d23_6imdat) ;
	    return (d23_ipcerror (&inrq_pb->d23_6hdinfo, D23_MTYPIL)) ;
	}
	/* check memory reference parameter */
	if (inrq_pb->d23_6rfidt != (byte *) d23_info.d23_bufarea +
				    (d23_info.d23_genarea->d23_bsize * assid)) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		    svc_c_sev_fatal, GDS_S_IPCMREFIL_ERR, inrq_pb->d23_6rfidt) ;
	    return (d23_ipcerror (&inrq_pb->d23_6hdinfo, D23_MREFIL)) ;
	}

	/* check message length parameter */
	if (inrq_pb->d23_6lnidt < 0 ||
			inrq_pb->d23_6lnidt > d23_info.d23_genarea->d23_bsize) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCILNIL_ERR, inrq_pb->d23_6lnidt) ;
	    return (d23_ipcerror (&inrq_pb->d23_6hdinfo, D23_ILNERR)) ;
	}

	invmsg.d23_opid = D23_SIOF((inrq_pb->d23_6imdat == D23_LASTFG) ?
					 D23_END : D23_MORE) | D23_SIOI(oprid) ;
	invmsg.d23_msgsz = inrq_pb->d23_6lnidt ;
    } else {
	invmsg.d23_opid = D23_SIOF(D23_END) | D23_SIOI(oprid) ;
	invmsg.d23_msgsz = 0 ;
    }
    invinfo = inrq_pb->d23_6invinfo ;

    if (inrq_pb->d23_6invinfo.d23_invid == D23_UNUSED) {
	/* get unambiguous invoke-ID */
	/* lock shared memory area */
	if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	    return (d23_ipcerror (&inrq_pb->d23_6hdinfo, D23_LOSEMA)) ;
	}
	invinfo.d23_invid = d23_info.d23_genarea->d23_linvid++ ;

	/* unlock shared memory area */
	if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	    return (d23_ipcerror (&inrq_pb->d23_6hdinfo, D23_ULSEMA)) ;
	}
    }

    /* send invoke-/invoke continuation-message */
    invmsg.d23_mtype = (ostate == D23_ASSOC) ?
	      D23_SITYPE(D23_INVMSG) | D23_SDSTID(cltptr->d23_esid) :
	      D23_SITYPE(D23_INVCMSG) | D23_SDSTID(cltptr->d23_cpid.d23_vpid) ;
    invmsg.d23_callpid = cltptr->d23_epid ;
    invmsg.d23_opid |= D23_SIOEVM(D23_GIEVM(d23_info.d23_istate)) ;
    invmsg.d23_iassid = D23_SCLDASS((ostate == D23_ASSOC) ? 0 :
				      cltptr->d23_cassid) | D23_SCALASS(assid) ;
    invmsg.d23_invinfo = invinfo ;
    if (d23_msgsnd (d23_info.d23_msgid, &invmsg, D23_MSGBUFSZ, IPC_NOWAIT) ==
								    D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSMSG_ERR) ;
        return (d23_ipcerror (&inrq_pb->d23_6hdinfo, D23_SMSGERR)) ;
    }

    if ((evmode = D23_GPEVM(cltptr->d23_estate)) == D23_FASYNC_EM ||
	(evmode == D23_PASYNC_EM && ostate == D23_ASSOC)) {
	/* send attention byte trough named FIFO */
	if (ostate == D23_ASSOC && D23_GSEST(cltptr->d23_estate) ==
								 D23_MPROCSVR) {
	    dstidtype = D23_DSTSID ;
	    msgdstid = attdstid = cltptr->d23_esid ;
	} else {
	    dstidtype = D23_DSTPID ;
	    msgdstid = cltptr->d23_cpid.d23_vpid ;
	    attdstid = cltptr->d23_cpid.d23_rpid ;
	}
	if (d23_attsnd (attdstid, dstidtype, &inrq_pb->d23_6hdinfo) == D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					  svc_c_sev_fatal, GDS_S_IPCSSATT_ERR) ;
	    return (D23_ERROR) ;
	}
    }

    if (D23_GIOI(invmsg.d23_opid) == D23_ABORT) {
	/* clear some client entry information unnecessary after an un- */
	/* confirmed abort operation */
	D23_CEOS(cltptr->d23_estate, D23_ASSOC) ;
	cltptr->d23_cassid = D23_UNUSED ;
	cltptr->d23_invinfo.d23_invid = D23_UNUSED ;
	if (D23_GPEVM(cltptr->d23_estate) == D23_SYNC_EM ||
				  D23_GSEST(cltptr->d23_estate) != D23_SPROCSVR)
	    cltptr->d23_cpid.d23_rpid = cltptr->d23_cpid.d23_vpid = D23_UNUSED ;
    } else {
	if (ostate == D23_ASSOC)
	    cltptr->d23_invinfo = invinfo ;
	if (D23_GIOF(invmsg.d23_opid) == D23_END) {
	    D23_CEOS(cltptr->d23_estate, D23_REQPEND) ;
	} else {
	    D23_CEOS(cltptr->d23_estate, D23_RBINVPEND) ;
	    if (D23_GIEVM(d23_info.d23_istate) != D23_FASYNC_EM) {
		/* continuation invoke-message follows -> */
		/* wait for a return buffer event from the message queue */
		evntpb.d23_Bhdinfo.d2_version = D23_V02 ;
		while ((result = d23_ievent (&evntpb, D23_INFIN,
			    		 (1 << D23_RRQBFMSG))) == D23_SUCES &&
					 (evntpb.d23_Bevtype != D23_RRQBFEVNT &&
					 evntpb.d23_Bevtype != D23_ABOEVNT)) ;
		if (result == D23_ERROR) {
		    inrq_pb->d23_6hdinfo = evntpb.d23_Bhdinfo ;
		    cltptr->d23_invinfo.d23_invid = D23_UNUSED ;
		    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
					svc_c_debug1, GDS_S_CINVOKE_EXIT,
					inrq_pb->d23_6invinfo.d23_invid)) ;
		    return (D23_ERROR) ;
		}
		if (evntpb.d23_Bevtype == D23_ABOEVNT) {
		    cltptr->d23_invinfo.d23_invid = D23_UNUSED ;
		    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
					svc_c_debug1, GDS_S_CINVOKE_EXIT,
					inrq_pb->d23_6invinfo.d23_invid)) ;
		    return (d23_ipcerror (&inrq_pb->d23_6hdinfo, D23_ABOERR)) ;
		}
	    }
	}
    }
    /* supply result parameter */
    if (inrq_pb->d23_6invinfo.d23_invid == D23_UNUSED)
	inrq_pb->d23_6invinfo.d23_invid = invinfo.d23_invid ;
	
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	   svc_c_debug1, GDS_S_CINVOKE_EXIT, inrq_pb->d23_6invinfo.d23_invid)) ;
    return (D23_SUCES) ;
}
