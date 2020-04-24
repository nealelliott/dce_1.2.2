/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23regproc.c,v $
 * Revision 1.1.10.2  1996/02/18  18:19:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:30  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:03:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:15:11  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:25:29  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:15:45  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:56:33  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:40:24  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  17:40:44  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:53:26  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:15:21  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:26:57  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:45:36  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:32:28  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:55:08  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:41:10  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23regproc.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:19:45 $";
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
/*   NAME:    d23_reg_process ()                                             */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function writes/removes the informations of the caller (process */
/*       type, directory-id, process-id) to/from the registration area of    */
/*       the shared memory of a directory system installation.               */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       ptype       = process type.                                         */
/*       dirid       = directory identifier (0, if irrelevant).              */
/*       optype      = D2_TRUE = write information to registration area.     */
/*                     D2_FALSE = remove information from registration area. */
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
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>


/* ------------------- DECLARATION OF GLOBAL DATA -------------------------- */

extern D23_iprocinfo	d23_info ;	/* process specific IPC-information */

/* ------------------------------------------------------------------------- */

signed32 d23_reg_process (D23_regprocpb *rpr_pb) {

    signed32		i ;
    D23_reginfo		*regptr ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_IPC,
			svc_c_debug1, GDS_S_CSREGPROC_ENTRY, rpr_pb->d23_Iptype,
			rpr_pb->d23_Idirid, rpr_pb->d23_Iotype)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, rpr_pb->d23_Ihdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, rpr_pb->d23_Ihdinfo.d2_version) ;
        return (d23_ipcerror (&rpr_pb->d23_Ihdinfo, D23_VERSIL)) ;
    }

    /* verify calling parameter */
    if (rpr_pb->d23_Iptype < 0) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		    svc_c_sev_fatal, GDS_S_IPCPTYPEIL_ERR, rpr_pb->d23_Iptype) ;
	return (d23_ipcerror (&rpr_pb->d23_Ihdinfo, D23_PPARERR)) ;
    }
    if (rpr_pb->d23_Idirid < 0 || rpr_pb->d23_Idirid > D2MAXDIR_ID) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		    svc_c_sev_fatal, GDS_S_IPCDIRIDIL_ERR, rpr_pb->d23_Idirid) ;
	return (d23_ipcerror (&rpr_pb->d23_Ihdinfo, D23_PPARERR)) ;
    }

    /* get IPC-resources (shared memory) */
    if (d23_get_ipc () == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					 svc_c_sev_fatal, GDS_S_IPCGETRES_ERR) ;
	return (d23_ipcerror (&rpr_pb->d23_Ihdinfo, D23_GIPCERR)) ;
    }

    /* lock shared memory */
    if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&rpr_pb->d23_Ihdinfo, D23_LOSEMA)) ;
    }

    if (rpr_pb->d23_Iotype) {
	/* search for a free IPC-process registration entry */
	for (i = 0, regptr = d23_info.d23_regarea;
			    i < d23_info.d23_genarea->d23_mrno; i++, regptr++) {
	    if (regptr->d23_pregtype == D23_UNUSED)
		break ;
	}

	if (i == d23_info.d23_genarea->d23_mrno) {
	    /* no free registration entry available */
	    d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
				      svc_c_sev_fatal, GDS_S_IPCLIMREG_ERR, i) ;
	    return (d23_ipcerror (&rpr_pb->d23_Ihdinfo, D23_PENTERR)) ;
	}

	/* write informations to the entry */
	regptr->d23_pregtype = rpr_pb->d23_Iptype ;
	regptr->d23_pdirid = rpr_pb->d23_Idirid ;
	regptr->d23_procid = d23_info.d23_ipid.d23_rpid ;
	strcpy (regptr->d23_pstate, D23_NOSTATE) ;
	d23_info.d23_genarea->d23_rno++ ;
    } else {
	/* search IPC-process registration entry */
	for (i = 0, regptr = d23_info.d23_regarea;
			    i < d23_info.d23_genarea->d23_mrno; i++, regptr++) {
	    if (regptr->d23_pregtype == rpr_pb->d23_Iptype &&
		regptr->d23_procid == d23_info.d23_ipid.d23_rpid &&
		regptr->d23_pdirid == rpr_pb->d23_Idirid) {
		/* delete registration entry */
		regptr->d23_pregtype = D23_UNUSED ;
		d23_info.d23_genarea->d23_rno-- ;
		break ;
	    }	
	}
	if (i == d23_info.d23_genarea->d23_mrno) {
	    /* registration entry not found (may be because of a wrong */
	    /* parameter) */
	    d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_IPC,
		       svc_c_sev_notice, GDS_S_IPCNOREG_ERR, rpr_pb->d23_Iptype,
							   rpr_pb->d23_Idirid) ;
	    return (d23_ipcerror (&rpr_pb->d23_Ihdinfo, D23_PENTERR)) ;
	}
    }

    if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&rpr_pb->d23_Ihdinfo, D23_ULSEMA)) ;
    }

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
				       svc_c_debug1, GDS_S_CSREGPROC_EXIT, i)) ;
    return (D23_SUCES) ;
}
