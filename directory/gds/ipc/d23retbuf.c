/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23retbuf.c,v $
 * Revision 1.1.10.2  1996/02/18  18:19:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:33  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:03:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:15:14  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:25:31  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:15:56  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:56:37  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:40:34  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  17:40:48  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:53:44  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:17:29  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:27:17  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:45:47  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:32:36  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:55:36  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:41:21  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23retbuf.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:19:48 $";
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
/*   NAME:    d23_retbuf ()                                                  */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function must be used to return a received buffer to IPC if     */
/*       a fully asynchronous event mode is enabled.                         */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       rbuf_pb     = Reference to the function parameter block which       */
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

signed32 d23_retbuf (D23_rbufpb *rbuf_pb) {

    signed32		assid, myassid, sndmtype = D23_RRSBFMSG ;
    D23_ipcentry	*ipcptr ;
    D23_ipcmsg		rbfmsg ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_debug1, GDS_S_CSRETBUF_ENTRY, rbuf_pb->d23_Cassid)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, rbuf_pb->d23_Chdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, rbuf_pb->d23_Chdinfo.d2_version) ;
	return (d23_ipcerror (&rbuf_pb->d23_Chdinfo, D23_VERSIL)) ;
    }

    /* check global client-/server state */
    if (D23_GIAS(d23_info.d23_istate) == D23_INATTACH ||
			      D23_GIEVM(d23_info.d23_istate) != D23_FASYNC_EM) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	return (d23_ipcerror (&rbuf_pb->d23_Chdinfo, D23_RBUFIL)) ;
    }

    /* check association-ID parameter */
    if ((assid = rbuf_pb->d23_Cassid) >= D23_MXCLTPROC) {
	assid -= D23_MXCLTPROC ;
	sndmtype = D23_RRQBFMSG ;
    }
    if (assid < 0 || assid > d23_info.d23_genarea->d23_mcno) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		    svc_c_sev_fatal, GDS_S_IPCASIDIL_ERR, rbuf_pb->d23_Cassid) ;
	return (d23_ipcerror (&rbuf_pb->d23_Chdinfo, D23_ASIDIL)) ;
    }

    /* check association specific client-/server state */
    if (sndmtype == D23_RRQBFMSG) {
	if ((D23_GIAS(d23_info.d23_istate) & D23_ISVRATT) != D23_ISVRATT) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	    return (d23_ipcerror (&rbuf_pb->d23_Chdinfo, D23_RBUFIL)) ;
	}
	ipcptr = d23_info.d23_svrarea ;
	for (myassid = 0; myassid < d23_info.d23_genarea->d23_mcno;
							    myassid++, ipcptr++)
	    if (D23_GES(ipcptr->d23_estate) == D23_ATTACH &&
		ipcptr->d23_esid == d23_info.d23_isvrid &&
		ipcptr->d23_epid.d23_rpid == d23_info.d23_ipid.d23_rpid &&
		ipcptr->d23_cassid == assid &&
				  D23_GEOS(ipcptr->d23_estate) == D23_RBINVPEND)
		break ;
	if (myassid == d23_info.d23_genarea->d23_mcno) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	    return (d23_ipcerror (&rbuf_pb->d23_Chdinfo, D23_RBUFIL)) ;
	}
    } else {
	ipcptr = d23_info.d23_cltarea + assid ;
	if (D23_GES(ipcptr->d23_estate) != D23_ATTACH ||
				D23_GEOS(ipcptr->d23_estate) != D23_RBRESPEND) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	    return (d23_ipcerror (&rbuf_pb->d23_Chdinfo, D23_RBUFIL)) ;
	}
	myassid = assid ;
    }

    /* send return buffer message */
    rbfmsg.d23_mtype = D23_SITYPE(sndmtype) |
					 D23_SDSTID(ipcptr->d23_cpid.d23_vpid) ;
    rbfmsg.d23_callpid = ipcptr->d23_epid ;
    rbfmsg.d23_iassid = D23_SCLDASS(ipcptr->d23_cassid) | D23_SCALASS(myassid) ;
    rbfmsg.d23_msgsz = 0 ;
    rbfmsg.d23_invinfo = ipcptr->d23_invinfo ;
    rbfmsg.d23_opid = D23_SIOF(D23_END) | D23_SIOI(D23_NOOPR) ;
    if (d23_msgsnd (d23_info.d23_msgid, &rbfmsg, D23_MSGBUFSZ, IPC_NOWAIT) ==
								    D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSMSG_ERR) ;
        return (d23_ipcerror (&rbuf_pb->d23_Chdinfo, D23_SMSGERR)) ;
    }

    if (D23_GPEVM(ipcptr->d23_estate) == D23_FASYNC_EM) {
	/* send attention byte trough named FIFO */
	if (d23_attsnd (ipcptr->d23_cpid.d23_rpid, D23_DSTPID,
					  &rbuf_pb->d23_Chdinfo) == D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					  svc_c_sev_fatal, GDS_S_IPCSSATT_ERR) ;
	    return (D23_ERROR) ;
	}
    }

    /* change entry state */
    D23_CEOS(ipcptr->d23_estate, (sndmtype == D23_RRQBFMSG) ?
						    D23_INVPEND : D23_RESPEND) ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_debug1, GDS_S_CSRETBUF_EXIT)) ;
    return (D23_SUCES) ;
}
