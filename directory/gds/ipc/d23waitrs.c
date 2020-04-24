/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23waitrs.c,v $
 * Revision 1.1.10.2  1996/02/18  18:20:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:47  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:04:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:15:30  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:25:43  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:16:56  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:56:48  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:41:19  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  17:41:10  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:55:22  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:30:04  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:29:01  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:46:50  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:33:10  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:58:00  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:42:13  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23waitrs.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:20:04 $";
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
/*   NAME:    d23_waitrs ()                                                  */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function waits resp. checks for incoming results from a         */
/*       server process.                                                     */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       wtrs_pb     = Reference to the function parameter block which       */
/*                     must contain the following input parameter:           */
/*                             - IPC-version number                          */
/*                             - wait-mode                                   */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     The function parameter block contains the output      */
/*		       parameter as follows:                                 */
/*                             - association-ID                              */
/*                             - context-ID                                  */
/*                             - user-ID                                     */
/*                             - result type                                 */
/*                             - more/end data flag                          */
/*                             - reference to the result data                */
/*                             - size of the result data                     */
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

signed32 d23_waitrs (D23_wtrspb *wtrs_pb) {

    signed32		i, evmode, ostate = D23_ASSOC ;
    D23_ipcentry	*cltptr ;
    D23_ipcmsg		rbfmsg ;
    D23_evntpb		evntpb ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		      svc_c_debug1, GDS_S_CWAITRS_ENTRY, wtrs_pb->d23_7wtime)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, wtrs_pb->d23_7hdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, wtrs_pb->d23_7hdinfo.d2_version) ;
	return (d23_ipcerror (&wtrs_pb->d23_7hdinfo, D23_VERSIL)) ;
    }

    /* check global client state */
    if ((D23_GIAS(d23_info.d23_istate) & D23_ICLTATT) != D23_ICLTATT) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	return (d23_ipcerror (&wtrs_pb->d23_7hdinfo, D23_WTRSIL)) ;
    }

    /* check association specific client state */
    cltptr = d23_info.d23_clstref ;
    if ((evmode = D23_GIEVM(d23_info.d23_istate)) == D23_FASYNC_EM ||
	(evmode == D23_PASYNC_EM && cltptr == (D23_ipcentry *) NULL) ||
	(cltptr != (D23_ipcentry *) NULL &&
	(ostate = D23_GEOS(cltptr->d23_estate)) != D23_RESPEND &&
						     ostate != D23_RBRESPEND)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, (cltptr == (D23_ipcentry *) NULL) ?
				     d23_info.d23_istate : cltptr->d23_estate) ;
	return (d23_ipcerror (&wtrs_pb->d23_7hdinfo, D23_WTRSIL)) ;
    }

    if (cltptr == (D23_ipcentry *) NULL) {
	/* look whether there is any request pending */
	for (i = 0, cltptr = d23_info.d23_cltarea;
			      i < d23_info.d23_genarea->d23_mcno; i++, cltptr++)
	    if (D23_GES(cltptr->d23_estate) == D23_ATTACH &&
		cltptr->d23_epid.d23_rpid == d23_info.d23_ipid.d23_rpid &&
				    D23_GEOS(cltptr->d23_estate) == D23_REQPEND)
		break ;
	if (i == d23_info.d23_genarea->d23_mcno) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	    return (d23_ipcerror (&wtrs_pb->d23_7hdinfo, D23_WTRSIL)) ;
	}
	cltptr = (D23_ipcentry *) NULL ;
    }

    /* check wait time parameter */
    if (wtrs_pb->d23_7wtime != D23_INFIN && wtrs_pb->d23_7wtime != D23_CHECK) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		    svc_c_sev_fatal, GDS_S_IPCWTYPIL_ERR, wtrs_pb->d23_7wtime) ;
	return (d23_ipcerror (&wtrs_pb->d23_7hdinfo, D23_WTYPIL)) ;
    }

    if (ostate == D23_RBRESPEND) {
	/* result pending -> send return buffer message */
	rbfmsg.d23_mtype = D23_SITYPE(D23_RRSBFMSG) |
					D23_SDSTID(cltptr->d23_cpid.d23_vpid) ;
	rbfmsg.d23_callpid = cltptr->d23_epid ;
	rbfmsg.d23_iassid = D23_SCLDASS(cltptr->d23_cassid) |
	D23_SCALASS(((char *) cltptr - (char *) d23_info.d23_cltarea) /
							sizeof (D23_ipcentry)) ;
	rbfmsg.d23_invinfo = cltptr->d23_invinfo ;
	rbfmsg.d23_msgsz = 0 ;
	rbfmsg.d23_opid = D23_SIOF(D23_END) | D23_SIOI(D23_NOOPR) ;
	if (d23_msgsnd (d23_info.d23_msgid, &rbfmsg, D23_MSGBUFSZ, IPC_NOWAIT) ==
								    D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSMSG_ERR) ;
	    return (d23_ipcerror (&wtrs_pb->d23_7hdinfo, D23_SMSGERR)) ;
	}
	if (D23_GPEVM(cltptr->d23_estate) == D23_FASYNC_EM) {
	    /* send attention byte trough named FIFO */
	    if (d23_attsnd ( cltptr->d23_cpid.d23_rpid, D23_DSTPID,
					  &wtrs_pb->d23_7hdinfo) == D23_ERROR) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					  svc_c_sev_fatal, GDS_S_IPCSSATT_ERR) ;
		return (D23_ERROR) ;
	    }
	}
	D23_CEOS(cltptr->d23_estate, D23_RESPEND) ;
    }

    /* wait resp. check for a relevant event in the message queue */
    evntpb.d23_Bhdinfo.d2_version = D23_V02 ;
    if (d23_ievent (&evntpb, wtrs_pb->d23_7wtime, (cltptr == (D23_ipcentry *)
	      NULL) ? (1 << D23_RESMSG) : (1 << D23_RESCMSG)) == D23_ERROR) {
	wtrs_pb->d23_7hdinfo = evntpb.d23_Bhdinfo ;
	return (D23_ERROR) ;
    }

    /* supply result parameter */
    if (evntpb.d23_Bevtype == D23_NOEVNT)
	wtrs_pb->d23_7rltid = D23_NOOPR ;
    else {
	wtrs_pb->d23_7rltid = (evntpb.d23_Bevtype == D23_ABOEVNT) ? D23_ABORT :
							      evntpb.d23_Bopid ;
	wtrs_pb->d23_7assid = evntpb.d23_Bassid ;
	wtrs_pb->d23_7invinfo = evntpb.d23_Binvinfo ;
	wtrs_pb->d23_7rmdat = evntpb.d23_Bmdat ;
	wtrs_pb->d23_7rfrdt = evntpb.d23_Bref ;
	wtrs_pb->d23_7lnrdt = evntpb.d23_Bln ;
    }
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d%d%d%d%d"), GDS_S_IPC,
	svc_c_debug1, GDS_S_CWAITRS_EXIT, wtrs_pb->d23_7assid,
	wtrs_pb->d23_7invinfo.d23_invid, wtrs_pb->d23_7invinfo.d23_usrid,
        wtrs_pb->d23_7invinfo.d23_contid, wtrs_pb->d23_7rltid,
	wtrs_pb->d23_7rmdat, wtrs_pb->d23_7rfrdt, wtrs_pb->d23_7lnrdt)) ;
    return (D23_SUCES) ;
}
