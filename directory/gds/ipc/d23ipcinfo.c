/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23ipcinfo.c,v $
 * Revision 1.1.10.2  1996/02/18  18:19:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:26  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:03:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:15:06  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:25:25  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:15:26  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:56:29  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:40:12  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  17:40:36  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:52:44  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:10:46  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:26:13  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:45:07  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:32:13  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:54:27  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:40:48  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23ipcinfo.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:19:40 $";
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
/*   NAME:    d23_gipc_eminfo ()                                             */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function must be used to get information about all (named       */
/*       FIFO) file descriptors on which IPC can receive events. These       */
/*       file descriptor(s) can be used by the caller to implement a         */
/*       central event management routine. The function can only be called   */
/*       if an asynchronous event mode is enabled.                           */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       ipcem_pb    = Reference to the function parameter block which       */
/*                     must contain the following input parameter:           */
/*                             - IPC-version number                          */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     The function parameter block contains the output      */
/*		       parameter as follows:                                 */
/*                             - # of file descriptors returned              */
/*                             - reference to an array which contains the    */
/*                               file descriptors returned                   */
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

signed32 d23_gipc_eminfo (D23_ipcempb *ipcem_pb) {

    static int	evfds[2] ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					svc_c_debug1, GDS_S_CSGIPCINFO_ENTRY)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, ipcem_pb->d23_Ehdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
					svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR,
					ipcem_pb->d23_Ehdinfo.d2_version) ;
	return (d23_ipcerror (&ipcem_pb->d23_Ehdinfo, D23_VERSIL)) ;
    }

    /* check client-/server state */
    if (D23_GIAS(d23_info.d23_istate) == D23_INATTACH ||
				D23_GIEVM(d23_info.d23_istate) == D23_SYNC_EM) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	return (d23_ipcerror (&ipcem_pb->d23_Ehdinfo, D23_GTEMIL)) ;
    }

    /* supply result parameter */
    ipcem_pb->d23_Enfds = 0 ;
    ipcem_pb->d23_Eipcfds = evfds ;
    if ((evfds[ipcem_pb->d23_Enfds] = d23_info.d23_pevfd) != D23_UNUSED)
	ipcem_pb->d23_Enfds++ ;
    if ((evfds[ipcem_pb->d23_Enfds] = d23_info.d23_sevfd) != D23_UNUSED)
	ipcem_pb->d23_Enfds++ ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_IPC,
	  svc_c_debug1, GDS_S_CSGIPCINFO_EXIT, ipcem_pb->d23_Enfds, evfds[0])) ;
    return (D23_SUCES) ;
}
