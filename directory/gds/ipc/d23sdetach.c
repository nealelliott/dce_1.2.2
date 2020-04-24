/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23sdetach.c,v $
 * Revision 1.1.10.2  1996/02/18  18:19:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:36  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:03:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:15:17  root]
 * 
 * Revision 1.1.8.5  1994/09/06  12:25:33  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:16:03  keutel]
 * 
 * Revision 1.1.8.4  1994/06/21  14:47:29  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  12:59:40  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:56:39  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:40:42  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:19:00  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:31:59  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:40:51  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:54:03  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:19:58  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:27:37  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  20:46:01  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:32:43  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:32:32  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  14:06:52  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:56:02  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:41:32  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23sdetach.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:19:51 $";
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
/*   NAME:    d23_detsvr ()                                                  */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function detaches the calling server process from the IPC-      */
/*       resources. Thus, the process is no longer known to the client       */
/*       processes.                                                          */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       desv_pb     = Reference to the function parameter block which       */
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
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>


/* ------------------- DECLARATION OF GLOBAL DATA -------------------------- */

extern D23_iprocinfo	d23_info ;	/* process specific IPC-information */

/* ------------------------------------------------------------------------- */

signed32 d23_detsvr (D23_desvpb *desv_pb) {

    signed32		istate, i, nosvr = 0 ;
    char		fname[D27_LFILE_NAME] ;
    D23_ipcentry	*svrptr ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
		     			   svc_c_debug1, GDS_S_SDETACH_ENTRY)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, desv_pb->d23_1hdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, desv_pb->d23_1hdinfo.d2_version) ;
	return (d23_ipcerror (&desv_pb->d23_1hdinfo, D23_VERSIL)) ;
    }

    /* check server state */
    if (((istate = D23_GIAS(d23_info.d23_istate)) & D23_ISVRATT) != D23_ISVRATT) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	return (d23_ipcerror (&desv_pb->d23_1hdinfo, D23_DESVIL)) ;
    }
    for (i = 0, svrptr = d23_info.d23_svrarea; i < d23_info.d23_genarea->d23_msno;
								i++, svrptr++) {
	if (D23_GES(svrptr->d23_estate) == D23_ATTACH &&
	    svrptr->d23_esid == d23_info.d23_isvrid &&
	    svrptr->d23_epid.d23_rpid == d23_info.d23_ipid.d23_rpid &&
	    D23_GEOS(svrptr->d23_estate) != D23_ASSOC) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	    return (d23_ipcerror (&desv_pb->d23_1hdinfo, D23_DESVIL)) ;
	}
    }

    /* lock shared memory area */
    if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&desv_pb->d23_1hdinfo, D23_LOSEMA)) ;
    }

    /* clear process specific server entries */
    for (i = 0, svrptr = d23_info.d23_svrarea; i < d23_info.d23_genarea->d23_msno;
								i++, svrptr++) {
	if (D23_GES(svrptr->d23_estate) == D23_ATTACH &&
	    svrptr->d23_esid == d23_info.d23_isvrid) {
	    nosvr++ ;
	    if (svrptr->d23_epid.d23_rpid == d23_info.d23_ipid.d23_rpid) {
		svrptr->d23_estate = D23_SES(D23_NOSVR) ;
		d23_info.d23_genarea->d23_sno-- ;
		nosvr-- ;
	    }
	}
    }

    /* unlock shared memory area */
    if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&desv_pb->d23_1hdinfo, D23_ULSEMA)) ;
    }
	
    /* release named FIFO resources */
    if (D23_GIEVM(d23_info.d23_istate) != D23_SYNC_EM) {	
	if (d23_info.d23_sevfd != D23_UNUSED) {
	    close (d23_info.d23_sevfd) ;
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
			  svc_c_debug3, GDS_S_CLOSE_FIFO, d23_info.d23_sevfd)) ;
	    if (nosvr == 0) {
		sprintf (fname, D23_FIFO_FILE, dcelocal_path, D27_CLIENT_DIR,
						      's', d23_info.d23_isvrid) ;
		unlink (fname) ;
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s"), GDS_S_IPC,
				      svc_c_debug3, GDS_S_UNLINK_FIFO, fname)) ;
	    }
	}

	if (istate == D23_ISVRATT) {
	    close (d23_info.d23_pevfd) ;
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
			  svc_c_debug3, GDS_S_CLOSE_FIFO, d23_info.d23_pevfd)) ;
	    sprintf (fname, D23_FIFO_FILE, dcelocal_path, D27_CLIENT_DIR,
					      'p', d23_info.d23_ipid.d23_rpid) ;
	    unlink (fname) ;
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s"), GDS_S_IPC,
				      svc_c_debug3, GDS_S_UNLINK_FIFO, fname)) ;
	}
    }
    for (i = 0; i < D23_MXEV; i++)
	if (d23_info.d23_evinfo[i].d23_evfd != D23_UNUSED) {
	    close (d23_info.d23_evinfo[i].d23_evfd) ;
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	     svc_c_debug3, GDS_S_CLOSE_FIFO, d23_info.d23_evinfo[i].d23_evfd)) ;
	}

    d23_info.d23_istate &= (istate == D23_ISVRATT) ?
				(D23_SIAS(D23_INATTACH) | D23_SIS(D23_INIT)) :
				((~D23_SIAS(D23_ISVRATT)) & D23_CISTY) ;
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
		     			    svc_c_debug1, GDS_S_SDETACH_EXIT)) ;
    return (D23_SUCES) ;
}
