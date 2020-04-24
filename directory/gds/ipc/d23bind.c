/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23bind.c,v $
 * Revision 1.1.10.2  1996/02/18  18:19:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:11  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:02:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:53  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:25:13  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:14:35  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:56:18  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:39:29  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  19:21:25  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:51:15  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:01:54  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:24:32  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:44:19  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:31:42  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:52:50  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:40:02  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23bind.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:19:25 $";
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
/*   NAME:    d23_bind ()                                                    */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function binds the calling client process to the server         */
/*       which belongs to the requested server-ID.                           */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       bind_pb     = Reference to the function parameter block which       */
/*                     must contain the following input parameter:           */
/*                             - IPC-version number                          */
/*                             - requested server-ID                         */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     The function parameter block contains the output      */
/*		       parameter as follows:                                 */
/*                             - reference to the IPC-send/receive buffer    */
/*                             - max. size of the IPC-send/receive buffer    */
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
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>


/* ------------------- DECLARATION OF GLOBAL DATA -------------------------- */

extern D23_iprocinfo	d23_info ;	/* process specific IPC-information */

/* ------------------------------------------------------------------------- */

signed32 d23_bind (D23_bindpb *bind_pb) {

    signed32		assid, i ;
    D23_ipcentry	*svrptr ;
    D23_ipcentry	*cltptr ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
			svc_c_debug1, GDS_S_CBIND_ENTRY, bind_pb->d23_4svrid)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, bind_pb->d23_4hdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, bind_pb->d23_4hdinfo.d2_version) ;
	return (d23_ipcerror (&bind_pb->d23_4hdinfo, D23_VERSIL)) ;
    }

    /* check client-state */
    if ((D23_GIAS(d23_info.d23_istate) & D23_ICLTATT) != D23_ICLTATT) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	return (d23_ipcerror (&bind_pb->d23_4hdinfo, D23_BINDIL)) ;
    }

    /* check server-ID parameter */
    if (bind_pb->d23_4svrid  < 1 || bind_pb->d23_4svrid > D23_MAXSVR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		    svc_c_sev_fatal, GDS_S_IPCSVIDIL_ERR, bind_pb->d23_4svrid) ;
	return (d23_ipcerror (&bind_pb->d23_4hdinfo, D23_SVIDIL)) ;
    }

    /* search for a corresponding server entry */
    for (i = 0, svrptr = d23_info.d23_svrarea;
			    i < d23_info.d23_genarea->d23_msno; i++, svrptr++) {
	if (D23_GES(svrptr->d23_estate) == D23_ATTACH &&
	    D23_GSET(svrptr->d23_estate) == D23_PRIMARY &&
					svrptr->d23_esid == bind_pb->d23_4svrid)
	    break ;
    }
    if (i == d23_info.d23_genarea->d23_msno) {
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		      svc_c_debug1, GDS_S_IPCNOSERV_ERR, bind_pb->d23_4svrid)) ;
	return (d23_ipcerror (&bind_pb->d23_4hdinfo, D23_NOSERV)) ;
    }

    /* search for a free client entry */
    /* lock shared memory area */
    if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&bind_pb->d23_4hdinfo, D23_LOSEMA)) ;
    }

    for (assid = 0, cltptr = d23_info.d23_cltarea;
		    assid < d23_info.d23_genarea->d23_mcno; assid++, cltptr++) {
	if (D23_GES(cltptr->d23_estate) == D23_NOCLT)
	    break ;
    }
    if (assid == d23_info.d23_genarea->d23_mcno) {
	if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	    return (d23_ipcerror (&bind_pb->d23_4hdinfo, D23_ULSEMA)) ;
	}
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
			         svc_c_sev_notice, GDS_S_IPCLIMASS_ERR, assid) ;
	return (d23_ipcerror (&bind_pb->d23_4hdinfo, D23_LIMASS)) ;
    }

    /* occupy client entry */
    cltptr->d23_epid.d23_rpid = d23_info.d23_ipid.d23_rpid ;
    if (d23_info.d23_ipid.d23_vpid == D23_UNUSED)
	d23_info.d23_ipid.d23_vpid = d23_getvpid () ;
    cltptr->d23_epid.d23_vpid = d23_info.d23_ipid.d23_vpid ;
    cltptr->d23_esid = bind_pb->d23_4svrid ;
    cltptr->d23_cassid = cltptr->d23_invinfo.d23_invid = D23_UNUSED ;
    if (D23_GOEVM(svrptr->d23_estate) != D23_SYNC_EM &&
				D23_GSEST(svrptr->d23_estate) == D23_SPROCSVR)
	cltptr->d23_cpid = svrptr->d23_epid ;
    else
	cltptr->d23_cpid.d23_rpid = cltptr->d23_cpid.d23_vpid = D23_UNUSED ;
    cltptr->d23_estate = D23_SES(D23_ATTACH) | D23_SEOS(D23_ASSOC) |
			 D23_SOEVM(D23_GIEVM(d23_info.d23_istate)) |
			 D23_SPEVM(D23_GOEVM(svrptr->d23_estate)) |
			 D23_SSEST(D23_GSEST(svrptr->d23_estate)) ;
    d23_info.d23_genarea->d23_cno++ ;
	
    /* unlock shared memory area */
    if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&bind_pb->d23_4hdinfo, D23_ULSEMA)) ;
    }

    /* supply result parameter */
    bind_pb->d23_4assid = assid ;
    bind_pb->d23_4lenms = d23_info.d23_genarea->d23_bsize ;
    bind_pb->d23_4refms = (byte *) (d23_info.d23_bufarea +
				    (d23_info.d23_genarea->d23_bsize * assid)) ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_IPC,
			svc_c_debug1, GDS_S_CBIND_EXIT, bind_pb->d23_4assid,
			bind_pb->d23_4refms, bind_pb->d23_4lenms)) ;
    return (D23_SUCES) ;
}
