/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23waitin.c,v $
 * Revision 1.1.10.2  1996/02/18  18:20:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:46  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:04:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:15:28  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:25:42  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:16:54  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:56:46  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:41:15  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  17:41:09  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:55:12  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:28:51  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:28:51  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:46:43  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:33:07  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:57:43  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:42:08  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23waitin.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:20:02 $";
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
/*   NAME:    d23_waitin ()                                                  */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function waits resp. checks for an incoming invoke request      */
/*       from a client process.                                              */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       wtin_pb     = Reference to the function parameter block which       */
/*                     must contain the following input parameter:           */
/*                             - IPC-version number                          */
/*                             - wait-mode                                   */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     The function parameter block contains the output      */
/*		       parameter as follows:                                 */
/*                              - association-ID                             */
/*                              - context-ID                                 */
/*                              - user-ID                                    */
/*                              - operation-ID                               */
/*                              - more/end data flag                         */
/*                              - invoke data reference                      */
/*                              - invoke data length                         */
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

signed32 d23_waitin (D23_wtinpb *wtin_pb) {

    signed32		evmode, ostate = D23_ASSOC ;
    D23_ipcentry	*svrptr ;
    D23_ipcmsg		rbfmsg ;
    D23_evntpb		evntpb ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		      svc_c_debug1, GDS_S_SWAITIN_ENTRY, wtin_pb->d23_2wtime)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, wtin_pb->d23_2hdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, wtin_pb->d23_2hdinfo.d2_version) ;
	return (d23_ipcerror (&wtin_pb->d23_2hdinfo, D23_VERSIL)) ;
    }

    /* check global server state */
    if ((D23_GIAS(d23_info.d23_istate) & D23_ISVRATT) != D23_ISVRATT) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	return (d23_ipcerror (&wtin_pb->d23_2hdinfo, D23_WTINIL)) ;
    }

    /* check association specific server state */
    evmode = D23_GIEVM(d23_info.d23_istate) ;
    svrptr = d23_info.d23_slstref ;
    if (evmode == D23_FASYNC_EM || (evmode == D23_PASYNC_EM && 
	svrptr == (D23_ipcentry *) NULL) ||
	(svrptr != (D23_ipcentry *) NULL &&
	(ostate = D23_GEOS(svrptr->d23_estate)) != D23_ASSOC &&
	ostate != D23_INVPEND && ostate != D23_RBINVPEND)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, (svrptr == (D23_ipcentry *) NULL) ?
				     d23_info.d23_istate : svrptr->d23_estate) ;
	return (d23_ipcerror (&wtin_pb->d23_2hdinfo, D23_WTINIL)) ;
    }

    /* check wait time parameter */
    if (wtin_pb->d23_2wtime != D23_INFIN && wtin_pb->d23_2wtime != D23_CHECK) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		    svc_c_sev_fatal, GDS_S_IPCWTYPIL_ERR, wtin_pb->d23_2wtime) ;
	return (d23_ipcerror (&wtin_pb->d23_2hdinfo, D23_WTYPIL)) ;
    }

    if (ostate == D23_RBINVPEND) {
	/* invoke pending -> send return buffer message */
	rbfmsg.d23_mtype = D23_SITYPE(D23_RRQBFMSG) |
					 D23_SDSTID(svrptr->d23_cpid.d23_vpid) ;
	rbfmsg.d23_callpid = svrptr->d23_epid ;
	rbfmsg.d23_iassid = D23_SCLDASS(svrptr->d23_cassid) |
	D23_SCALASS(((char *) svrptr - (char *) d23_info.d23_svrarea) /
							sizeof (D23_ipcentry)) ;
	rbfmsg.d23_msgsz = 0 ;
	rbfmsg.d23_invinfo = svrptr->d23_invinfo ;
	rbfmsg.d23_opid = D23_SIOF(D23_END) | D23_SIOI(D23_NOOPR) ;
	if (d23_msgsnd (d23_info.d23_msgid, &rbfmsg, D23_MSGBUFSZ, IPC_NOWAIT) ==
								    D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSMSG_ERR) ;
	    return (d23_ipcerror (&wtin_pb->d23_2hdinfo, D23_SMSGERR)) ;
	}
	if (D23_GPEVM(svrptr->d23_estate) == D23_FASYNC_EM) {
	    /* send attention byte trough named FIFO */
	    if (d23_attsnd (svrptr->d23_cpid.d23_rpid, D23_DSTPID,
					  &wtin_pb->d23_2hdinfo) == D23_ERROR) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					  svc_c_sev_fatal, GDS_S_IPCSSATT_ERR) ;
		return (D23_ERROR) ;
	    }
	}
	D23_CEOS(svrptr->d23_estate, D23_INVPEND) ;
    }

    /* wait resp. check for a relevant event in the message queue */
    evntpb.d23_Bhdinfo.d2_version = D23_V02 ;
    if (d23_ievent (&evntpb, wtin_pb->d23_2wtime, (svrptr == (D23_ipcentry *)
	      NULL) ? (1 << D23_INVMSG) : (1 << D23_INVCMSG)) == D23_ERROR) {
	wtin_pb->d23_2hdinfo = evntpb.d23_Bhdinfo ;
	return (D23_ERROR) ;
    }

    /* supply result parameter */
    if (evntpb.d23_Bevtype == D23_NOEVNT) {
	wtin_pb->d23_2oprid = D23_NOOPR ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		      svc_c_debug1, GDS_S_SWAITIN_EXIT1, wtin_pb->d23_2oprid)) ;
    } else {
	wtin_pb->d23_2oprid = (evntpb.d23_Bevtype == D23_ABOEVNT) ? D23_ABORT :
							      evntpb.d23_Bopid ;
	wtin_pb->d23_2assid = evntpb.d23_Bassid ;
	wtin_pb->d23_2invinfo = evntpb.d23_Binvinfo ;
	wtin_pb->d23_2imdat = evntpb.d23_Bmdat ;
	wtin_pb->d23_2rfidt = evntpb.d23_Bref ;
	wtin_pb->d23_2lnidt = evntpb.d23_Bln ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d%d%d%d%d"), GDS_S_IPC,
	       svc_c_debug1, GDS_S_SWAITIN_EXIT, wtin_pb->d23_2assid,
	       wtin_pb->d23_2invinfo.d23_invid, wtin_pb->d23_2invinfo.d23_usrid,
	       wtin_pb->d23_2invinfo.d23_contid, wtin_pb->d23_2oprid,
	       wtin_pb->d23_2imdat, wtin_pb->d23_2rfidt, wtin_pb->d23_2lnidt)) ;
    }
    return (D23_SUCES) ;
}
