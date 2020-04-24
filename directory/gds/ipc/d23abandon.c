/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23abandon.c,v $
 * Revision 1.1.4.2  1996/02/18  18:19:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:08  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:02:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:50  root]
 * 
 * Revision 1.1.2.5  1994/09/06  12:25:10  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:14:28  keutel]
 * 
 * Revision 1.1.2.4  1994/06/09  18:46:29  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:34  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/10  15:56:15  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:39:19  marrek]
 * 
 * Revision 1.1.2.2  1994/03/23  15:18:49  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:32:14  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:21:23  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:12:43  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:54:09  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23abandon.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:19:22 $";
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
/*   NAME:    d23_abandon ()                                                 */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function supports the DS_ABANDON operation in a multi-process   */
/*       DSA environment. It allows the DSA process which receives the       */
/*       abandon operation to inform the DSA process performing the corres-  */
/*       ponding (DS_READ, DS_SEARCH, ...) operation about the abandon       */
/*       request.                                                            */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       abnd_pb      = Reference to the standard header parameter block     */
/*                     which must contain the following input parameter:     */
/*                             - IPC-version number                          */
/*                             - invoke information about the operation      */
/*                               to which the abandon should be applied.     */
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
/*   AUTHOR: R.Horn                                        DATE: 24.11.93    */
/*exoff **********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>


/* ------------------- DECLARATION OF GLOBAL DATA -------------------------- */

extern D23_iprocinfo	d23_info ;	/* process specific IPC-information */

/* ------------------------------------------------------------------------- */

signed32 d23_abandon (D23_abandonpb *abnd_pb) {

    signed32		i, ostate ;
    D23_ipcentry	*svrptr, *svrhlpptr ;
    D23_dwritepb	abcmd ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_IPC,
	svc_c_debug1, GDS_S_SABAND_ENTRY, abnd_pb->d23_Ginvinfo.d23_invid,
	abnd_pb->d23_Ginvinfo.d23_usrid, abnd_pb->d23_Ginvinfo.d23_contid)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, abnd_pb->d23_Ghdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, abnd_pb->d23_Ghdinfo.d2_version) ;
	return (d23_ipcerror (&abnd_pb->d23_Ghdinfo, D23_VERSIL)) ;
    }

    /* check global server state */
    if ((D23_GIAS(d23_info.d23_istate) & D23_ISVRATT) != D23_ISVRATT) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	   	     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	return (d23_ipcerror (&abnd_pb->d23_Ghdinfo, D23_ABNDIL)) ;
    }

    svrptr = d23_info.d23_slstref ;

    /* search server entry associated with invoke information supplied */
    /* lock shared memory area */
    if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&abnd_pb->d23_Ghdinfo, D23_LOSEMA)) ;
    }

    for (i = 0, svrhlpptr = d23_info.d23_svrarea;
			 i < d23_info.d23_genarea->d23_msno; i++, svrhlpptr++) {
	if (D23_GES(svrhlpptr->d23_estate) == D23_ATTACH &&
	    ((ostate = D23_GEOS(svrhlpptr->d23_estate)) == D23_INVPEND ||
	    ostate == D23_REQPEND) && !D23_GDABND(svrhlpptr->d23_estate) &&
	    svrhlpptr->d23_invinfo.d23_invid == abnd_pb->d23_Ginvinfo.d23_invid &&
	    svrhlpptr->d23_invinfo.d23_usrid == abnd_pb->d23_Ginvinfo.d23_usrid &&
	    svrhlpptr->d23_invinfo.d23_contid == abnd_pb->d23_Ginvinfo.d23_contid)
	    break ;
    }
    if (svrhlpptr == svrptr || i == d23_info.d23_genarea->d23_msno) {
	/* invalid invoke-ID */
	d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					     svc_c_debug1, GDS_S_SABAND_EXIT)) ;
	return (d23_ipcerror (&abnd_pb->d23_Ghdinfo, D23_ILLINVID)) ;
    }

    /* verify for 'abandon disabled' state */
    if (D23_GDABND(svrhlpptr->d23_estate) == D23_DABNDRQ) {
	/* operation is disabled for abandon */
	d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					     svc_c_debug1, GDS_S_SABAND_EXIT)) ;
	return (d23_ipcerror (&abnd_pb->d23_Ghdinfo, D23_TOOLATE)) ;
    }

    /* verify operation-ID */
    if (svrhlpptr->d23_oprid != D23_READ && svrhlpptr->d23_oprid != D23_LIST &&
	svrhlpptr->d23_oprid != D23_SEARCH && svrhlpptr->d23_oprid != D23_COMPARE) {
	/* abandon not possible for this operation */
	d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					     svc_c_debug1, GDS_S_SABAND_EXIT)) ;
	return (d23_ipcerror (&abnd_pb->d23_Ghdinfo, D23_CNTABND)) ;
    }

    /* set abandon request flag */
    svrhlpptr->d23_estate |= D23_SABND(D23_ABNDRQ) ;

    if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&abnd_pb->d23_Ghdinfo, D23_ULSEMA)) ;
    }

    /* send distributed command 'abandon' */
    abcmd.d23_Jhdinfo.d2_version = D23_V02 ;
    abcmd.d23_Jcmd = D23_DCABANDON ;
    abcmd.d23_Jsv_prid = svrhlpptr->d23_epid.d23_rpid ;
    abcmd.d23_Jtmout = D23_DMAXTOUT ;
    abcmd.d23_Jparref = (byte *) NULL ;
    if (d23_write_dist_cmd (&abcmd) == D23_ERROR) {
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					     svc_c_debug1, GDS_S_SABAND_EXIT)) ;
	return (d23_ipcerror (&abnd_pb->d23_Ghdinfo, D23_TMOUT)) ;
    }

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					     svc_c_debug1, GDS_S_SABAND_EXIT)) ;
    return (D23_SUCES) ;
}

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d23_disable_abandon ()                                         */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function supports the DS_ABANDON operation in a multi-process   */
/*       DSA environment. It allows the DSA process to disable an abandon    */
/*       operation. After this call, an incoming abandon request associated  */
/*       to the disabled operation is rejected as being too late.            */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       abnd_pb      = Reference to the standard header parameter block     */
/*                     which must contain the following input parameter:     */
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
/*   AUTHOR: R.Horn                                        DATE: 24.11.93    */
/*exoff **********************************************************************/

signed32 d23_disable_abandon (D23_disabandonpb *abnd_pb) {

    signed32		assid, myassid ;
    D23_ipcentry	*svrptr ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_IPC,
		    svc_c_debug1, GDS_S_SDISABAND_ENTRY, abnd_pb->d23_Hassid)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, abnd_pb->d23_Hhdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, abnd_pb->d23_Hhdinfo.d2_version) ;
	return (d23_ipcerror (&abnd_pb->d23_Hhdinfo, D23_VERSIL)) ;
    }

    /* check global server state */
    if ((D23_GIAS(d23_info.d23_istate) & D23_ISVRATT) != D23_ISVRATT) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	return (d23_ipcerror (&abnd_pb->d23_Hhdinfo, D23_ABNDIL)) ;
    }

    /* check association-ID parameter */
    assid = abnd_pb->d23_Hassid - D23_MXCLTPROC ;
    if (assid < 0 || assid > d23_info.d23_genarea->d23_mcno) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		    svc_c_sev_fatal, GDS_S_IPCASIDIL_ERR, abnd_pb->d23_Hassid) ;
	return (d23_ipcerror (&abnd_pb->d23_Hhdinfo, D23_ASIDIL)) ;
    }

    /* check association specific server state */
    for (myassid = 0, svrptr = d23_info.d23_svrarea;
                  myassid < d23_info.d23_genarea->d23_msno; myassid++, svrptr++)        if (D23_GES(svrptr->d23_estate) == D23_ATTACH &&
            svrptr->d23_esid == d23_info.d23_isvrid &&
            svrptr->d23_epid.d23_rpid == d23_info.d23_ipid.d23_rpid &&
            svrptr->d23_cassid == assid)
            break ;
    if (myassid == d23_info.d23_genarea->d23_msno ||
        			  D23_GEOS(svrptr->d23_estate) != D23_REQPEND) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
			      svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, D23_UNUSED) ;
	return (d23_ipcerror (&abnd_pb->d23_Hhdinfo, D23_ABNDIL)) ;
    }

    /* lock shared memory area */
    if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&abnd_pb->d23_Hhdinfo, D23_LOSEMA)) ;
    }

    /* verify for 'abandon request' state */
    if (D23_GABND(svrptr->d23_estate) == D23_ABNDRQ) {
	/* abandon request operation pending */
	d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					  svc_c_debug1, GDS_S_SDISABAND_EXIT)) ;
	return (d23_ipcerror (&abnd_pb->d23_Hhdinfo, D23_TOOLATE)) ;
    }

    /* set 'disable abandon request'-flag */
    svrptr->d23_estate |= D23_SDABND(D23_DABNDRQ) ;

    if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&abnd_pb->d23_Hhdinfo, D23_ULSEMA)) ;
    }

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					  svc_c_debug1, GDS_S_SDISABAND_EXIT)) ;
    return (D23_SUCES) ;
}
