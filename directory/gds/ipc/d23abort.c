/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23abort.c,v $
 * Revision 1.1.768.2  1996/02/18  18:19:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:09  marty]
 *
 * Revision 1.1.768.1  1995/12/08  16:02:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:51  root]
 * 
 * Revision 1.1.766.3  1994/09/06  12:25:12  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:14:32  keutel]
 * 
 * Revision 1.1.766.2  1994/05/10  15:56:16  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:39:25  marrek]
 * 
 * Revision 1.1.766.1  1994/02/22  19:21:24  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:51:06  marrek]
 * 
 * Revision 1.1.764.2  1993/10/14  17:35:51  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:33:43  keutel]
 * 
 * Revision 1.1.764.1  1993/10/13  17:33:05  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:56:07  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  12:00:54  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:24:21  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:44:09  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:31:38  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:52:40  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:39:57  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23abort.c,v $ $Revision: 1.1.768.2 $ $Date: 1996/02/18 18:19:23 $";
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
/*   NAME:    d23_abort ()                                                   */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function can be used to abort an IPC-association if a fully     */
/*       asynchronous event mode is enabled.                                 */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       abo_pb      = Reference to the function parameter block which       */
/*                     must contain the following input parameter:           */
/*                             - IPC-version number                          */
/*                             - association-ID                              */
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

signed32 d23_abort (D23_abortpb *abo_pb) {

    signed32		assid, myassid, ostate, msgdstid ;
    signed32		attdstid, dsttype, sndmtype ;
    D23_ipcentry	*ipcptr ;
    D23_ipcmsg		abomsg ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d"), GDS_S_IPC,
		svc_c_debug1, GDS_S_CSABORT_ENTRY, abo_pb->d23_Fassid,
		abo_pb->d23_Finvinfo.d23_invid, abo_pb->d23_Finvinfo.d23_usrid,
		abo_pb->d23_Finvinfo.d23_contid)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, abo_pb->d23_Fhdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, abo_pb->d23_Fhdinfo.d2_version) ;
	return (d23_ipcerror (&abo_pb->d23_Fhdinfo, D23_VERSIL)) ;
    }

    /* check global client-/server state */
    if (D23_GIAS(d23_info.d23_istate) == D23_INATTACH ||
			      D23_GIEVM(d23_info.d23_istate) != D23_FASYNC_EM) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	return (d23_ipcerror (&abo_pb->d23_Fhdinfo, D23_ABOIL)) ;
    }

    /* check association-ID parameter */
    if ((assid = abo_pb->d23_Fassid) >= D23_MXCLTPROC)
	assid -= D23_MXCLTPROC ;
    if (assid < 0 || assid > d23_info.d23_genarea->d23_mcno) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCASIDIL_ERR, abo_pb->d23_Fassid) ;
	return (d23_ipcerror (&abo_pb->d23_Fhdinfo, D23_ASIDIL)) ;
    }

    /* check association specific client-/server state */
    if (abo_pb->d23_Fassid >= D23_MXCLTPROC) {
	/* abort request is executed by a server process */
	if ((D23_GIAS(d23_info.d23_istate) & D23_ISVRATT) != D23_ISVRATT) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	    return (d23_ipcerror (&abo_pb->d23_Fhdinfo, D23_ABOIL)) ;
	}
	ipcptr = d23_info.d23_svrarea ;
	for (myassid = 0; myassid < d23_info.d23_genarea->d23_msno;
							    myassid++, ipcptr++)
	    if (D23_GES(ipcptr->d23_estate) == D23_ATTACH &&
		ipcptr->d23_esid == d23_info.d23_isvrid &&
		ipcptr->d23_epid.d23_rpid == d23_info.d23_ipid.d23_rpid &&
		ipcptr->d23_cassid == assid &&
		((ostate = D23_GEOS(ipcptr->d23_estate)) == D23_RBINVPEND ||
				ostate == D23_REQPEND || ostate == D23_RESPEND))
		break ;
	if (myassid == d23_info.d23_genarea->d23_msno) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	    return (d23_ipcerror (&abo_pb->d23_Fhdinfo, D23_ABOIL)) ;
	}

	sndmtype = (ostate == D23_RBINVPEND) ? D23_RRQBFMSG :
			    (ostate == D23_REQPEND) ? D23_RESMSG : D23_RESCMSG ;
	msgdstid = ipcptr->d23_cpid.d23_vpid ;
	attdstid = ipcptr->d23_cpid.d23_rpid ;
	dsttype = D23_DSTPID ;
    } else {
	/* abort request is executed by a client process */
	ipcptr = d23_info.d23_cltarea + assid ;
	if (D23_GES(ipcptr->d23_estate) != D23_ATTACH ||
	   ((ostate = D23_GEOS(ipcptr->d23_estate)) != D23_ASSOC &&
			    ostate != D23_INVPEND && ostate != D23_RBRESPEND)) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		      svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, ipcptr->d23_estate) ;
	    return (d23_ipcerror (&abo_pb->d23_Fhdinfo, D23_ABOIL)) ;
	}
	myassid = assid ;

	sndmtype = (ostate == D23_ASSOC) ? D23_INVMSG :
			  (ostate == D23_INVPEND) ? D23_INVCMSG : D23_RRSBFMSG ;
	if (sndmtype == D23_INVMSG) {
	    msgdstid = ipcptr->d23_esid ;
	    if (D23_GSEST(ipcptr->d23_estate) == D23_MPROCSVR) {
		attdstid = ipcptr->d23_esid ;
		dsttype = D23_DSTSID ;
	    } else {
		attdstid = ipcptr->d23_cpid.d23_rpid ;
		dsttype = D23_DSTPID ;
	    }
	} else {
	    msgdstid = ipcptr->d23_cpid.d23_vpid ;
	    attdstid = ipcptr->d23_cpid.d23_rpid ;
	    dsttype = D23_DSTPID ;
	}
    }
    /* check invoke information parameter */
    if (ostate != D23_ASSOC &&
	(abo_pb->d23_Finvinfo.d23_invid != ipcptr->d23_invinfo.d23_invid ||
	     abo_pb->d23_Finvinfo.d23_usrid != ipcptr->d23_invinfo.d23_usrid)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_IPC,
	   svc_c_sev_fatal, GDS_S_IPCINVIL_ERR, abo_pb->d23_Finvinfo.d23_invid,
	   abo_pb->d23_Finvinfo.d23_usrid, abo_pb->d23_Finvinfo.d23_contid) ;
	return (d23_ipcerror (&abo_pb->d23_Fhdinfo, D23_INVERR)) ;
    }

    /* send abort message */
    abomsg.d23_mtype = D23_SITYPE(sndmtype) | D23_SDSTID(msgdstid) ;
    abomsg.d23_callpid = ipcptr->d23_epid ;
    abomsg.d23_iassid = D23_SCLDASS((ipcptr->d23_cassid == D23_UNUSED) ? 0 :
				    ipcptr->d23_cassid) | D23_SCALASS(myassid) ;
    abomsg.d23_msgsz = 0 ;
    abomsg.d23_invinfo = abo_pb->d23_Finvinfo ;
    abomsg.d23_opid = D23_SIOF(D23_END) | D23_SIOI(D23_ABORT) ;
    if (d23_msgsnd (d23_info.d23_msgid, &abomsg, D23_MSGBUFSZ, IPC_NOWAIT) ==
								    D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSMSG_ERR) ;
        return (d23_ipcerror (&abo_pb->d23_Fhdinfo, D23_SMSGERR)) ;
    }

    if (D23_GPEVM(ipcptr->d23_estate) == D23_FASYNC_EM) {
	/* send attention byte trough named FIFO */
	if (d23_attsnd (attdstid, dsttype, &abo_pb->d23_Fhdinfo) == D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					  svc_c_sev_fatal, GDS_S_IPCSSATT_ERR) ;
	    return (D23_ERROR) ;
	}
    }

    /* change entry state */
    if (abo_pb->d23_Fassid > D23_MXCLTPROC) {
	if (ipcptr != d23_info.d23_primref) {
	    /* release additional server entry */
	    /* lock shared memory area */
	    if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
		return (d23_ipcerror (&abo_pb->d23_Fhdinfo, D23_LOSEMA)) ;
	    }
	    ipcptr->d23_estate = D23_SES(D23_NOSVR) ;
	    d23_info.d23_genarea->d23_sno-- ;

	    /* unlock shared memory area */
	    if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
		return (d23_ipcerror (&abo_pb->d23_Fhdinfo, D23_ULSEMA)) ;
	    }
	} else {
	    ipcptr->d23_cassid = D23_UNUSED ;
	    ipcptr->d23_cpid.d23_rpid = D23_UNUSED ;
	    ipcptr->d23_cpid.d23_vpid = D23_UNUSED ;
	    D23_CEOS(ipcptr->d23_estate, D23_ASSOC) ;
	}
    } else {
	ipcptr->d23_cassid = D23_UNUSED ;
	if (D23_GSEST(ipcptr->d23_estate) == D23_MPROCSVR) {
	    ipcptr->d23_cpid.d23_rpid = D23_UNUSED ;
	    ipcptr->d23_cpid.d23_vpid = D23_UNUSED ;
	}
	d23_info.d23_clstref = (D23_ipcentry*) NULL ;
	D23_CEOS(ipcptr->d23_estate, D23_ASSOC) ;
    }

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					    svc_c_debug1, GDS_S_CSABORT_EXIT)) ;
    return (D23_SUCES) ;
}
