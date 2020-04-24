/*
 *  @OSF_COPYRIGHT@
 *  COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 *  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 *  src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23cdetach.c,v $
 * Revision 1.1.10.2  1996/02/18  18:19:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:15  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:02:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:55  root]
 * 
 * Revision 1.1.8.4  1994/09/06  12:25:15  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:14:45  keutel]
 * 
 * Revision 1.1.8.3  1994/05/10  15:56:20  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:39:37  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:18:51  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:31:34  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  19:21:28  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:51:36  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:03:50  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:24:54  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  20:44:28  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:31:49  bbelch]
 * 
 * Revision 1.1.4.2  1992/10/06  18:39:28  sommerfeld
 * 	Flush #ident
 * 	[1992/09/30  21:32:41  sommerfeld]
 * 
 * Revision 1.1.2.3  1992/07/17  19:59:19  weisman
 * 	Checked in at OSF for SNI
 * 	[1992/07/17  19:28:49  weisman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23cdetach.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:19:28 $";
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
/*   NAME:    d23_detclt ()                                                  */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function detaches the calling client process from the IPC-      */
/*       resources.                                                          */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       decl_pb     = Reference to the function parameter block which       */
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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>


/* ------------------- DECLARATION OF GLOBAL DATA -------------------------- */

extern D23_iprocinfo	d23_info ;	/* process specific IPC-information */

/* ------------------------------------------------------------------------- */

signed32 d23_detclt (D23_declpb *decl_pb) {

    signed32		istate, i ;
    char		fname[D27_LFILE_NAME] ;
    D23_ipcentry	*cltptr ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
		     			   svc_c_debug1, GDS_S_CDETACH_ENTRY)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, decl_pb->d23_Ahdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, decl_pb->d23_Ahdinfo.d2_version) ;
	return (d23_ipcerror (&decl_pb->d23_Ahdinfo, D23_VERSIL)) ;
    }

    /* check client state */
    if (((istate = D23_GIAS(d23_info.d23_istate)) & D23_ICLTATT) != D23_ICLTATT) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	return (d23_ipcerror (&decl_pb->d23_Ahdinfo, D23_DECLIL)) ;
    }

    /* lock shared memory area */
    if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&decl_pb->d23_Ahdinfo, D23_LOSEMA)) ;
    }

    /* verify for IPC-operations in progress */
    for (i = 0, cltptr = d23_info.d23_cltarea;
			    i < d23_info.d23_genarea->d23_mcno; i++, cltptr++) {
	if (D23_GES(cltptr->d23_estate) == D23_ATTACH &&
	    cltptr->d23_epid.d23_rpid == d23_info.d23_ipid.d23_rpid &&
				    D23_GEOS(cltptr->d23_estate) != D23_ASSOC) {
	    d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	    return (d23_ipcerror (&decl_pb->d23_Ahdinfo, D23_DECLIL)) ;
	}
    }

    /* clear process specific client entries */
    for (i = 0, cltptr = d23_info.d23_cltarea;
			    i < d23_info.d23_genarea->d23_mcno; i++, cltptr++) {
	if (D23_GES(cltptr->d23_estate) == D23_ATTACH &&
		      cltptr->d23_epid.d23_rpid == d23_info.d23_ipid.d23_rpid) {
	    cltptr->d23_estate = D23_SES(D23_NOCLT) ;
	    d23_info.d23_genarea->d23_cno-- ;
	}
    }

    /* unlock shared memory area */
    if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&decl_pb->d23_Ahdinfo, D23_ULSEMA)) ;
    }
	
    /* release named FIFO resources */
    if (D23_GIEVM(d23_info.d23_istate) != D23_SYNC_EM && istate == D23_ICLTATT) {	
	close (d23_info.d23_pevfd) ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
			  svc_c_debug3, GDS_S_CLOSE_FIFO, d23_info.d23_pevfd)) ;
	sprintf (fname, D23_FIFO_FILE, dcelocal_path, D27_CLIENT_DIR,
					      'p', d23_info.d23_ipid.d23_rpid) ;
	unlink (fname) ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s"), GDS_S_IPC,
				      svc_c_debug3, GDS_S_UNLINK_FIFO, fname)) ;
    }
    for (i = 0; i < D23_MXEV; i++)
	if (d23_info.d23_evinfo[i].d23_evfd != D23_UNUSED) {
	    close (d23_info.d23_evinfo[i].d23_evfd) ;
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	     svc_c_debug3, GDS_S_CLOSE_FIFO, d23_info.d23_evinfo[i].d23_evfd)) ;
	}

    d23_info.d23_istate &= (istate == D23_ICLTATT) ?
				  (D23_SIAS(D23_INATTACH) | D23_SIS(D23_INIT)) :
						      (~D23_SIAS(D23_ICLTATT)) ;
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
		     			    svc_c_debug1, GDS_S_CDETACH_EXIT)) ;
    return (D23_SUCES) ;
}
