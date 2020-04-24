/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23unbind.c,v $
 * Revision 1.1.10.2  1996/02/18  18:19:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:40  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:03:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:15:23  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:25:38  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:16:23  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:56:42  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:40:55  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  17:40:57  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:54:32  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:23:55  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:28:04  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:46:19  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:32:53  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:56:50  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:41:48  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23unbind.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:19:57 $";
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
/*   NAME:    d23_unbind ()                                                  */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function unbinds the calling client process from the server     */
/*       of a specific server-ID.                                            */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       ubnd_pb     = Reference to the function parameter block which       */
/*                     must contain the following input parameter:           */
/*                             - IPC-version number                          */
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
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>


/* ------------------- DECLARATION OF GLOBAL DATA -------------------------- */

extern D23_iprocinfo	d23_info ;	/* process specific IPC-information */

/* ------------------------------------------------------------------------- */

signed32 d23_unbind (D23_ubndpb *ubnd_pb) {

    signed32		assid, i = D23_UNUSED ;
    D23_ipcentry	*cltptr, *svrptr ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		      svc_c_debug1, GDS_S_CUNBIND_ENTRY, ubnd_pb->d23_5assid)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, ubnd_pb->d23_5hdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, ubnd_pb->d23_5hdinfo.d2_version) ;
	return (d23_ipcerror (&ubnd_pb->d23_5hdinfo, D23_VERSIL)) ;
    }

    /* check client-state */
    if ((D23_GIAS(d23_info.d23_istate) & D23_ICLTATT) != D23_ICLTATT) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	return (d23_ipcerror (&ubnd_pb->d23_5hdinfo, D23_UBNDIL)) ;
    }

    /* check association-ID parameter */
    if ((assid = ubnd_pb->d23_5assid) < 0 ||
				       assid > d23_info.d23_genarea->d23_mcno) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
				  svc_c_sev_fatal, GDS_S_IPCASIDIL_ERR, assid) ;
	return (d23_ipcerror (&ubnd_pb->d23_5hdinfo, D23_ASIDIL)) ;
    }

    /* check client entry state */
    cltptr = d23_info.d23_cltarea + assid ;
    if (D23_GES(cltptr->d23_estate) != D23_ATTACH ||
		      cltptr->d23_epid.d23_rpid != d23_info.d23_ipid.d23_rpid) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
				  svc_c_sev_fatal, GDS_S_IPCASIDIL_ERR, assid) ;
	return (d23_ipcerror (&ubnd_pb->d23_5hdinfo, D23_ASIDIL)) ;
    }
    if (D23_GEOS(cltptr->d23_estate) != D23_ASSOC) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		      svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, cltptr->d23_estate) ;
	return (d23_ipcerror (&ubnd_pb->d23_5hdinfo, D23_UBNDIL)) ;
    }

    /* release client entry */
    /* lock shared memory area */
    if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&ubnd_pb->d23_5hdinfo, D23_LOSEMA)) ;
    }

    cltptr->d23_estate = D23_SES(D23_NOCLT) ;
    d23_info.d23_genarea->d23_cno-- ;

    /* verify for further IPC-entries belonging to this process */
    if ((D23_GIAS(d23_info.d23_istate) & D23_ISVRATT) == D23_ISVRATT) {
	svrptr = d23_info.d23_svrarea ;
	for (i = 0; i < d23_info.d23_genarea->d23_msno; i++, svrptr++)
	    if (D23_GES(svrptr->d23_estate) == D23_ATTACH &&
			svrptr->d23_epid.d23_rpid == d23_info.d23_ipid.d23_rpid)
		break ;
    }	
    if (i == D23_UNUSED || i == d23_info.d23_genarea->d23_msno) {
	cltptr = d23_info.d23_cltarea ;
	for (i = 0; i < d23_info.d23_genarea->d23_mcno; i++, cltptr++)
	    if (D23_GES(cltptr->d23_estate) == D23_ATTACH &&
			cltptr->d23_epid.d23_rpid == d23_info.d23_ipid.d23_rpid)
		break ;
	if (i == d23_info.d23_genarea->d23_mcno)
	    /* no additional entry found -> clear virtual process-ID */
	    d23_info.d23_ipid.d23_vpid = D23_UNUSED ;
    }

    if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&ubnd_pb->d23_5hdinfo, D23_ULSEMA)) ;
    }

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					    svc_c_debug1, GDS_S_CUNBIND_EXIT)) ;
    return (D23_SUCES) ;
}
