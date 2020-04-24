/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23svrinfo.c,v $
 * Revision 1.1.10.2  1996/02/18  18:19:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:39  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:03:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:15:22  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:25:37  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:16:20  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:56:41  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:40:50  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  17:40:55  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:54:22  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:22:36  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:27:55  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:46:13  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:32:50  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:56:34  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:41:43  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23svrinfo.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:19:55 $";
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
/*   NAME:    d23_getsvrinfo ()                                              */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function returns to the calling server process the no. of       */
/*       processes belonging to the same server-ID and being in a busy-      */
/*       resp. idle state. This information can be used to increase or       */
/*       decrease dynamically the no. of server processes.                   */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       gtsv_pb     = Reference to the function parameter block which       */
/*                     must contain the following input parameter:           */
/*                             - IPC-version number                          */
/*                             - requested server-ID                         */
/*                             - process unload factor                       */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     The function parameter block contains the output      */
/*		       parameter as follows:                                 */
/*                             - total no. of server processes belonging     */
/*                               to the requested server-ID.                 */
/*                                                                           */
/*                             - no of server processes belonging to the     */
/*                               requested server-ID and being in a busy     */
/*                               state.                                      */
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
#include <signal.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>


/* ------------------- DECLARATION OF GLOBAL DATA -------------------------- */

extern D23_iprocinfo	d23_info ;	/* process specific IPC-information */

/* ------------------------------------------------------------------------- */

signed32 d23_getsvrinfo (D23_gtsvpb *gtsv_pb) {

    signed32		i, unloadf ;
    D23_ipcentry	*svrptr ;
    
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_IPC,
			svc_c_debug1, GDS_S_SGSVINFO_ENTRY, gtsv_pb->d23_Dsvrid,
			gtsv_pb->d23_Dunloadf)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, gtsv_pb->d23_Dhdinfo.d2_version)) {
	return (d23_ipcerror (&gtsv_pb->d23_Dhdinfo, D23_VERSIL)) ;
    }

    /* check server state */
    if ((D23_GIAS(d23_info.d23_istate) & D23_ISVRATT) != D23_ISVRATT) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	return (d23_ipcerror (&gtsv_pb->d23_Dhdinfo, D23_GTSVIL)) ;
    }

    /* check server-ID parameter */
    if (gtsv_pb->d23_Dsvrid < 1 || gtsv_pb->d23_Dsvrid > D23_MAXSVR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		    svc_c_sev_fatal, GDS_S_IPCSVIDIL_ERR, gtsv_pb->d23_Dsvrid) ;
	return (d23_ipcerror (&gtsv_pb->d23_Dhdinfo, D23_SVIDIL)) ;
    }

    /* check unload factor parameter */
    if ((unloadf = (signed32) gtsv_pb->d23_Dunloadf) != 0 && (unloadf < 0 ||
							       unloadf > 100)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
				svc_c_sev_fatal, GDS_S_IPCUNLDIL_ERR, unloadf) ;
	return (d23_ipcerror (&gtsv_pb->d23_Dhdinfo, D23_UNLDIL)) ;
    }

    /* search for requested server entries */
    gtsv_pb->d23_Dnototpr = 0 ;
    gtsv_pb->d23_Dnobsypr = 0 ;
    /* lock shared memory area */
    if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&gtsv_pb->d23_Dhdinfo, D23_LOSEMA)) ;
    }

    for (i = 0, svrptr = d23_info.d23_svrarea;
			    i < d23_info.d23_genarea->d23_msno; i++, svrptr++) {
	if (D23_GES(svrptr->d23_estate) == D23_ATTACH &&
	    svrptr->d23_esid == d23_info.d23_isvrid &&
	    D23_GSET(svrptr->d23_estate) == D23_PRIMARY &&
	    kill (svrptr->d23_epid.d23_rpid, 0) == 0 &&
				  D23_GSEUR(svrptr->d23_estate) != D23_UNLDRQ) {
	    gtsv_pb->d23_Dnototpr++ ;
	    if (D23_GEOS(svrptr->d23_estate) != D23_ASSOC)
		gtsv_pb->d23_Dnobsypr++ ;
	}
    }

    if (unloadf != 0 && ((gtsv_pb->d23_Dnototpr - gtsv_pb->d23_Dnobsypr) *
				       100) / gtsv_pb->d23_Dnototpr > unloadf) {
	/* unload factor exceeded -> mark server entries */
	for (i = 0, svrptr = d23_info.d23_svrarea;
			    i < d23_info.d23_genarea->d23_msno; i++, svrptr++) {
	    if (D23_GES(svrptr->d23_estate) == D23_ATTACH &&
		svrptr->d23_esid == d23_info.d23_isvrid &&
		D23_GSET(svrptr->d23_estate) == D23_PRIMARY &&
			svrptr->d23_epid.d23_rpid == d23_info.d23_ipid.d23_rpid)
		svrptr->d23_estate |= D23_SSEUR(D23_UNLDRQ) ;
	}
    }

    /* unlock shared memory area */
    if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&gtsv_pb->d23_Dhdinfo, D23_ULSEMA)) ;
    }

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_IPC,
		svc_c_debug1, GDS_S_SGSVINFO_EXIT, gtsv_pb->d23_Dnototpr,
		gtsv_pb->d23_Dnobsypr)) ;
    return (D23_SUCES) ;
}
