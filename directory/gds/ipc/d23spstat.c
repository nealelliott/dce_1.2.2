/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23spstat.c,v $
 * Revision 1.1.10.2  1996/02/18  18:19:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:37  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:03:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:15:18  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:25:35  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:16:06  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:56:40  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:40:46  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  17:40:53  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:54:12  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:21:16  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:27:46  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:46:07  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:32:47  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:56:20  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:41:38  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23spstat.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:19:52 $";
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
/*   NAME:    d23_set_pstate ()                                              */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function writes the given (sub)state of the calling process     */
/*       to the corresponding entry (within the registration area or the     */
/*       server process area of the shared memory).                          */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       pstate      = Reference to the (sub)state value (string) of the     */
/*                     process. Valid state values are                       */
/*                            Wxx    (xx = 1 - 99) -> waiting states         */
/*                            Rxx    (xx = 1 - 99) -> running states         */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     None                                                  */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*    D23_SUCES      = The function has been performed successfully.         */
/*    D23_ERROR      = An error is occurred during execution of the          */
/*                     function.                                             */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 17.5.91     */
/*exoff **********************************************************************/

#include <gds.h>
#include <stdlib.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>

/* ---------------- DECLARATION OF GLOBAL DATA ----------------------------- */

extern D23_iprocinfo	d23_info ;	/* process specific IPC-information */

/* ------------------------------------------------------------------------- */

signed32 d23_set_pstate (D23_pstatpb *sps_pb) {

    signed32		i ;
    boolean		entry_found = FALSE ;
    D23_reginfo		*regptr ;
    D23_ipcentry	*srvptr, *cltptr ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s%d"), GDS_S_IPC,
			svc_c_debug1, GDS_S_CSSPSTAT_ENTRY, sps_pb->d23_Ppstate,
							  sps_pb->d23_Passid)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, sps_pb->d23_Phdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, sps_pb->d23_Phdinfo.d2_version) ;
	return (d23_ipcerror (&sps_pb->d23_Phdinfo, D23_VERSIL)) ;
    }

    /* verify state parameter */
    if (strlen (sps_pb->d23_Ppstate) > (size_t) 3 ||
	(*sps_pb->d23_Ppstate != D23_WSTATE &&
	*sps_pb->d23_Ppstate != D23_RSTATE) ||
			 ((i = atoi (sps_pb->d23_Ppstate + 1)) < 1 && i > 99)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"), GDS_S_IPC,
		    svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, sps_pb->d23_Ppstate) ;
	return (d23_ipcerror (&sps_pb->d23_Phdinfo, D23_PPARERR)) ;
    }

    /* get IPC-resources */
    if (d23_get_ipc () == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					 svc_c_sev_fatal, GDS_S_IPCGETRES_ERR) ;
	return (d23_ipcerror (&sps_pb->d23_Phdinfo, D23_GIPCERR)) ;
    }

    if (sps_pb->d23_Passid != D23_UNUSED) {
	/* association specific call -> check association-ID parameter */
	if (sps_pb->d23_Passid < 0 ||
			  sps_pb->d23_Passid > d23_info.d23_genarea->d23_mcno) {
	    return (d23_ipcerror (&sps_pb->d23_Phdinfo, D23_ASIDIL)) ;
	}
	/* search association specific client entry */
	cltptr = d23_info.d23_cltarea + sps_pb->d23_Passid ;
	if (D23_GES(cltptr->d23_estate) == D23_ATTACH &&
		      cltptr->d23_epid.d23_rpid == d23_info.d23_ipid.d23_rpid) {
	    strcpy (cltptr->d23_ucstate, sps_pb->d23_Ppstate) ;
	    entry_found = TRUE ;
	}
    } else {
	/* process specific call -> search corresponding process entries */
	for (i = 0, regptr = d23_info.d23_regarea;
			    i < d23_info.d23_genarea->d23_mrno; i++, regptr++) {
	    if (regptr->d23_pregtype != D23_UNUSED &&
			     regptr->d23_procid == d23_info.d23_ipid.d23_rpid) {
		strcpy (regptr->d23_pstate, sps_pb->d23_Ppstate) ;
		entry_found = TRUE ;
	    }
	}

	srvptr = d23_info.d23_svrarea ;
	for (i = 0; i < d23_info.d23_genarea->d23_msno; i++, srvptr++) {
	    if (D23_GES(srvptr->d23_estate) == D23_ATTACH &&
		      srvptr->d23_epid.d23_rpid == d23_info.d23_ipid.d23_rpid) {
		strcpy (srvptr->d23_ucstate, sps_pb->d23_Ppstate) ;
		entry_found = TRUE ;
	    }
	}
    }
    if (!entry_found) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
			      svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, D23_UNUSED) ;
	return (d23_ipcerror (&sps_pb->d23_Phdinfo, D23_SPSTIL)) ;
    }
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_debug1, GDS_S_CSSPSTAT_EXIT)) ;
    return (D23_SUCES) ;
}
