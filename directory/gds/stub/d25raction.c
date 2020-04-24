/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25raction.c,v $
 * Revision 1.1.12.2  1996/02/18  18:23:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:17  marty]
 *
 * Revision 1.1.12.1  1995/12/08  16:15:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:20:54  root]
 * 
 * Revision 1.1.10.4  1994/09/06  12:26:30  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:24:50  keutel]
 * 
 * Revision 1.1.10.3  1994/05/10  16:03:29  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:12:43  marrek]
 * 
 * Revision 1.1.10.2  1994/03/23  15:47:24  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:33:39  keutel]
 * 
 * Revision 1.1.10.1  1994/02/22  17:55:22  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:52:06  marrek]
 * 
 * Revision 1.1.8.1  1993/10/14  19:19:00  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:16:05  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/09  17:58:02  keutel]
 * 
 * Revision 1.1.6.4  1993/08/11  08:28:19  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:19:49  marrek]
 * 
 * Revision 1.1.6.3  1993/07/12  20:57:50  treff
 * 	DCT @ OSF
 * 	replace ifdef __HPUX__ with ifdef __hpux
 * 	[1993/07/12  19:45:06  treff]
 * 
 * Revision 1.1.6.2  1993/07/02  15:16:50  marrek
 * 	Changes for port to HP800.
 * 	[1993/07/02  14:42:00  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  22:04:08  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:48:03  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/10  13:12:07  marrek
 * 	November 1992 code drop
 * 	Workaround with #ifdef NO_ROS_BUG until new ros library is
 * 	available. Until then do not define NO_ROS_BUG.
 * 	[1992/12/10  13:11:17  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  18:28:44  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  15:09:00  marrek]
 * 
 * Revision 1.1.2.3  1992/07/06  18:13:49  melman
 * 	CR 3975: xds test core dump client to client/server
 * 	[1992/07/06  18:11:43  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  23:09:42  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:33:16  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25raction.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 18:23:22 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton **********************************************************************/
/*                                                                           */
/*   TYPE:    module                                                         */
/*                                                                           */
/*   NAME:    d25raction.c                                                   */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       This module contains all actions (functions) belonging to the       */
/*       ROS-interface.                                                      */
/*       The following functions are available:                              */
/*                                                                           */
/*          * d25_rosattach - Attach application                             */
/*          * d25_rosdetach - Detach application                             */
/*          * d25_rosbinrq  - Bind request                                   */
/*          * d25_robinrs   - Bind response                                  */
/*          * d25_rosubnrq  - Unbind request                                 */
/*          * d25_roubnrs   - Unbind response                                */
/*          * d25_rosinvrq  - Invoke request                                 */
/*          * d25_rosresrq  - Result request                                 */
/*          * d25_roerrrq   - Return error request                           */
/*          * d25_rorjurq   - Reject request by user                         */
/*          * d25_rosaborq  - Abort association request                      */
/*          * d25_rosprbf   - Provide buffer for receive                     */
/*          * d25_rostrrq   - Get transfer syntax                            */
/*          * tp2_001_open  - TP-open virtual file                           */
/*          * tp2_002_read  - TP-read from virtual file                      */
/*          * tp2_003_write - TP-write into virtual file                     */
/*          * tp2_005_close - TP-close virtual file                          */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 3.6.91      */
/*datoff *********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <ros.h>
#include <tpdu.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>
#include <d25stub.h>

#if defined(__hpux)
#   ifndef NBBY
#     define NBBY   8
#   endif
#endif /* __hpux */


/* ---------------- DECLARATION OF GLOBAL DATA ----------------------------- */

signed16	d25_mxtpbuf ;	/* max. # of TP-routines send/receive buffer */
D25_tpinfo	*tpinfref ;	/* reference to the TP-routines send/receive */
				/* buffer description table */

/* ---------------------- ROS - ATTACH ------------------------------------- */

signed32 d25_rosattach (D25_actionpb *actpb) {

    R_attapb		atta ;
    D25_stubinfo	*sinfptr = actpb->d25_asinforef ;

    /* supply ROS-attach application parameter block */
    atta.r_hdinfo.r_versnb = R_VERSION ;
    atta.r_hdinfo.r_fnctnb = R_ATACH ;
    atta.r_cont = R_FALSE ;
    atta.r_accept = R_SELECT ;
    atta.r_pid = sinfptr->d25_pid ;

    atta.r_aplref.r_paddrf = (char *) sinfptr->d25_spsapref ;
    atta.r_aplref.r_paddln = sinfptr->d25_spsaplen ;
    atta.r_aplref.r_aetitl.r_obidrf = (R_unsign) NULL ;
    atta.r_aplref.r_aetitl.r_obidnb = 0 ;

    atta.r_apclst.r_apcnum = (R_unsign) sinfptr->d25_nacentries ;
    atta.r_apclst.r_absnum = (R_unsign) sinfptr->d25_nasentries ;
    atta.r_apclst.r_trsnum = (R_unsign) sinfptr->d25_ntsentries ;
    atta.r_apclst.r_apcary = sinfptr->d25_actabref ;
    atta.r_apclst.r_absary = sinfptr->d25_astabref ;
    atta.r_apclst.r_idsary = sinfptr->d25_asitabref ;
    atta.r_apclst.r_trsary = sinfptr->d25_tstabref ;
    atta.r_apclst.r_idtary = sinfptr->d25_tsitabref ;
    atta.r_apclst.r_objary = sinfptr->d25_oitabref ;

    /* execute ROS-attach application */
    if (r_atach (&atta) != R_SUCES) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
                svc_c_sev_fatal, GDS_S_ROSFUFAILED_ERR, atta.r_hdinfo.r_retcod, 
			       atta.r_hdinfo.r_erclas, atta.r_hdinfo.r_errval) ;
	return (D25_AERROR) ;
    }
    sinfptr->d25_aplid = atta.r_aplid ;

    return (D25_ACONT) ;
}

/* ---------------------- ROS - DETACH ------------------------------------- */

signed32 d25_rosdetach (D25_actionpb *actpb) {

    R_detapb		deta ;
    D25_stubinfo	*sinfptr = actpb->d25_asinforef ;

    /* supply ROS-detach application parameter block */
    deta.r_hdinfo.r_versnb = R_VERSION ;
    deta.r_hdinfo.r_fnctnb = R_DTACH ;
    deta.r_cont = R_FALSE ;
    deta.r_aplid = sinfptr->d25_aplid ;

    /* execute ROS-detach application */
    if (r_dtach (&deta) != R_SUCES) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
                svc_c_sev_fatal, GDS_S_ROSFUFAILED_ERR, deta.r_hdinfo.r_retcod, 
			       deta.r_hdinfo.r_erclas, deta.r_hdinfo.r_errval) ;
	return (D25_AERROR) ;
    }
    return (D25_ACONT) ;
}

/* ---------------------- ROS - BIND REQUEST ------------------------------- */

signed32 d25_rosbinrq (D25_actionpb *actpb) {

    R_bnrqpb		bnrq ; 
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;
    D25_rosentry	*rosptr = actpb->d25_arosref ;
    D25_stubinfo	*sinfptr = actpb->d25_asinforef ;
    D25_usrdat		rcvdat ;

    /* supply ROS-bind request parameter block */
    bnrq.r_hdinfo.r_versnb = R_VERSION ;
    bnrq.r_hdinfo.r_fnctnb = R_BINRQ ;
    bnrq.r_cont = R_FALSE ;
    bnrq.r_otype = R_ACKASN ;
    bnrq.r_aclass = (rosptr->d25_rcontid == D27_DAP_AC) ? R_ACL1 : R_ACL3 ;
    bnrq.r_aplid = sinfptr->d25_aplid ;

    bnrq.r_ptnref.r_paddln = rosptr->d25_rpsaplen ;
    bnrq.r_ptnref.r_paddrf = (char *) rosptr->d25_rpsapref ;
    bnrq.r_ptnref.r_aetitl.r_obidrf = (R_unsign) NULL ;
    bnrq.r_ptnref.r_aetitl.r_obidnb = 0 ;

    bnrq.r_apcon.r_obidrf = sinfptr->d25_oitabref +
		   (sinfptr->d25_actabref + rosptr->d25_rcontid - 1)->r_apcinx ;
    bnrq.r_apcon.r_obidnb =
		   (sinfptr->d25_actabref + rosptr->d25_rcontid - 1)->r_objnum ;

    bnrq.r_sndata.r_mtype  = R_FILE ;
    bnrq.r_sndata.r_more  = R_FALSE ;
    bnrq.r_sndata.r_absid  = rosptr->d25_rabsid ;
    bnrq.r_sndata.r_fraref = (char *) &ipcptr->d25_iudat ;
    bnrq.r_sndata.r_fralen = sizeof (D25_usrdat) ;

    bnrq.r_evdata.r_mtype  = R_FILE ;
    rcvdat.d25_datref = ipcptr->d25_ibufref ;
    rcvdat.d25_datlen = 0 ;
    rcvdat.d25_datoff = ipcptr->d25_ibufsz ;
    bnrq.r_evdata.r_bufref = (char *) &rcvdat ;
    bnrq.r_evdata.r_buflen = sizeof (D25_usrdat) ;

    /* execute ROS-bind request */
    if (r_binrq (&bnrq) != R_SUCES) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
                svc_c_sev_fatal, GDS_S_ROSFUFAILED_ERR, bnrq.r_hdinfo.r_retcod, 
			       bnrq.r_hdinfo.r_erclas, bnrq.r_hdinfo.r_errval) ;
	return (D25_AERROR) ;
    }
    rosptr->d25_rassid = bnrq.r_assid ;

    return (D25_ACONT) ;
}

/* ---------------------- ROS - BIND RESPONSE ------------------------------ */

signed32 d25_robinrs (D25_actionpb *actpb) {

    R_bnrspb		bnrs ; 
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;
    D25_rosentry	*rosptr = actpb->d25_arosref ;
    D25_stubinfo	*sinfptr = actpb->d25_asinforef ;

    /* supply ROS-bind response parameter block */
    bnrs.r_hdinfo.r_versnb = R_VERSION ;
    bnrs.r_hdinfo.r_fnctnb = R_BINRS ;
    bnrs.r_cont = R_FALSE ;
    bnrs.r_otype = R_ACKASN ;
    bnrs.r_assid = rosptr->d25_rassid ;
    bnrs.r_result = (ipcptr->d25_iresid == D23_RESULT) ? R_ACCEPT : R_UREJCT ;

    bnrs.r_apcon.r_obidrf = sinfptr->d25_oitabref +
		   (sinfptr->d25_actabref + rosptr->d25_rcontid - 1)->r_apcinx ;
    bnrs.r_apcon.r_obidnb =
		   (sinfptr->d25_actabref + rosptr->d25_rcontid - 1)->r_objnum ;

    bnrs.r_pradd.r_paddrf = (char *) NULL ;
    bnrs.r_pradd.r_paddln = 0 ;

    bnrs.r_aetitl.r_obidrf = (R_unsign) NULL ;
    bnrs.r_aetitl.r_obidnb = 0 ;

    bnrs.r_sndata.r_mtype = R_MEMORY;
    bnrs.r_sndata.r_more = R_FALSE;
    bnrs.r_sndata.r_absid = rosptr->d25_rabsid ;
    bnrs.r_sndata.r_fraref = (char *) ipcptr->d25_iudat.d25_datref ;
    bnrs.r_sndata.r_fralen = ipcptr->d25_iudat.d25_datlen ;

    /* execute ROS-bind response */
    if (r_binrs (&bnrs) != R_SUCES) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
                svc_c_sev_fatal, GDS_S_ROSFUFAILED_ERR, bnrs.r_hdinfo.r_retcod, 
			       bnrs.r_hdinfo.r_erclas, bnrs.r_hdinfo.r_errval) ;
	return (D25_AERROR) ;
    }
    return (D25_ACONT) ;
}

/* ---------------------- ROS - UNBIND REQUEST ----------------------------- */

signed32 d25_rosubnrq (D25_actionpb *actpb) {

    R_ubrqpb		ubrq ; 
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;
    D25_rosentry	*rosptr = actpb->d25_arosref ;
    D25_usrdat		rcvdat ;

    if (rosptr->d25_rrefcnt > 1)
	/* the ROS-unbind request is not executed if the ROS-association is */
	/* used by additional IPC-associations */
	return (D25_ANSUC) ;

    /* supply ROS-unbind request parameter block */
    ubrq.r_hdinfo.r_versnb = R_VERSION ;
    ubrq.r_hdinfo.r_fnctnb = R_UBNRQ ;
    ubrq.r_cont = R_FALSE ;
    ubrq.r_otype = R_ACKASN ;
    ubrq.r_assid = rosptr->d25_rassid ;

    ubrq.r_sndata.r_mtype  = R_MEMORY ;
    ubrq.r_sndata.r_more  = R_FALSE ;
    ubrq.r_sndata.r_absid  = rosptr->d25_rabsid ;
    ubrq.r_sndata.r_fraref = (char *) ipcptr->d25_iudat.d25_datref ;
    ubrq.r_sndata.r_fralen = ipcptr->d25_iudat.d25_datlen ;

    ubrq.r_evdata.r_mtype  = R_FILE ;
    rcvdat.d25_datref = ipcptr->d25_ibufref ;
    rcvdat.d25_datlen = 0 ;
    rcvdat.d25_datoff = ipcptr->d25_ibufsz ;
    ubrq.r_evdata.r_bufref = (char *) &rcvdat ;
    ubrq.r_evdata.r_buflen = sizeof (D25_usrdat) ;

    /* execute ROS-unbind request */
    if(r_ubnrq(&ubrq) != R_SUCES) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
                svc_c_sev_fatal, GDS_S_ROSFUFAILED_ERR, ubrq.r_hdinfo.r_retcod, 
			       ubrq.r_hdinfo.r_erclas, ubrq.r_hdinfo.r_errval) ;
	return (D25_AERROR) ;
    }
    return (D25_ACONT) ;
}

/* ---------------------- ROS - UNBIND RESPONSE ---------------------------- */

signed32 d25_roubnrs (D25_actionpb *actpb) {

    R_ubrspb		ubrs ; 
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;
    D25_rosentry	*rosptr = actpb->d25_arosref ;

    if (rosptr->d25_rrefcnt > 1)
	/* the ROS-unbind response is not executed if the ROS-association is */
	/* used by additional IPC-associations */
	return (D25_ACONT) ;

    /* supply ROS-unbind response parameter block */
    ubrs.r_hdinfo.r_versnb = R_VERSION ;
    ubrs.r_hdinfo.r_fnctnb = R_UBNRS ;
    ubrs.r_cont = R_FALSE ;
    ubrs.r_otype = R_ACKASN ;
    ubrs.r_fail = R_FALSE ;
    ubrs.r_assid = rosptr->d25_rassid ;

    ubrs.r_sndata.r_mtype = R_MEMORY ;
    ubrs.r_sndata.r_more = R_FALSE ;
    ubrs.r_sndata.r_absid  = rosptr->d25_rabsid ;
    ubrs.r_sndata.r_fraref = (char *) ipcptr->d25_iudat.d25_datref ;
    ubrs.r_sndata.r_fralen = ipcptr->d25_iudat.d25_datlen ;

    /* execute ROS-unbind response */
    if (r_ubnrs (&ubrs) != R_SUCES) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
                svc_c_sev_fatal, GDS_S_ROSFUFAILED_ERR, ubrs.r_hdinfo.r_retcod, 
			       ubrs.r_hdinfo.r_erclas, ubrs.r_hdinfo.r_errval) ;
	return (D25_AERROR) ;
    }

    return (D25_ACONT) ;
}

/* ---------------------- ROS - INVOKE REQUEST ----------------------------- */

signed32 d25_rosinvrq (D25_actionpb *actpb) {

    R_ivrqpb		ivrq ; 
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;
    D25_rosentry	*rosptr = actpb->d25_arosref ;
    D25_usrdat		rcvdat ;

    /* supply ROS-invoke request parameter block */
    ivrq.r_hdinfo.r_versnb = R_VERSION ;
    ivrq.r_hdinfo.r_fnctnb = R_INVRQ ;
    ivrq.r_cont = R_FALSE ;
    ivrq.r_otype = R_ACKASN ;
    ivrq.r_oclass = R_OPC2 ;
    ivrq.r_opval.r_oploc = (R_int *) &ipcptr->d25_ioprid ;
    ivrq.r_assid = rosptr->d25_rassid ;
    ivrq.r_invid = ipcptr->d25_invinfo.d23_invid ;
    ivrq.r_lnkid = (R_int *) NULL ;

    if (ipcptr->d25_iudat.d25_datlen >
			     (signed32) (1 << (sizeof (signed16) * NBBY)) - 1) {
	ivrq.r_sndata.r_mtype  = R_FILE ;
	ivrq.r_sndata.r_fraref = (char *) &ipcptr->d25_iudat ;
	ivrq.r_sndata.r_fralen = sizeof (D25_usrdat) ;
    } else {
	ivrq.r_sndata.r_mtype  = R_MEMORY ;
	ivrq.r_sndata.r_fraref = (char *) ipcptr->d25_iudat.d25_datref ;
	ivrq.r_sndata.r_fralen = ipcptr->d25_iudat.d25_datlen ;
    }
    ivrq.r_sndata.r_more  = R_FALSE ;
    ivrq.r_sndata.r_absid  = rosptr->d25_rabsid ;

    ivrq.r_evdata.r_mtype  = R_FILE ;
    rcvdat.d25_datref = ipcptr->d25_ibufref ;
    rcvdat.d25_datlen = 0 ;
    rcvdat.d25_datoff = ipcptr->d25_ibufsz ;
    ivrq.r_evdata.r_bufref = (char *) &rcvdat ;
    ivrq.r_evdata.r_buflen = sizeof (D25_usrdat) ;

    /* execute ROS-invoke request */
    if (r_invrq (&ivrq) != R_SUCES) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
                svc_c_sev_fatal, GDS_S_ROSFUFAILED_ERR, ivrq.r_hdinfo.r_retcod, 
			       ivrq.r_hdinfo.r_erclas, ivrq.r_hdinfo.r_errval) ;
	return (D25_AERROR) ;
    }
    return (D25_ACONT) ;
}

/* ---------------------- ROS - RETURN RESULT REQUEST ---------------------- */

signed32 d25_rosresrq (D25_actionpb *actpb) {

    R_rsrqpb		rsrq ; 
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;
    D25_rosentry	*rosptr = actpb->d25_arosref ;

    /* supply ROS-return result request parameter block */
    rsrq.r_hdinfo.r_versnb = R_VERSION ;
    rsrq.r_hdinfo.r_fnctnb = R_RESRQ ;
    rsrq.r_cont = R_FALSE ;
    rsrq.r_otype = R_ACKASN ;
    rsrq.r_assid = rosptr->d25_rassid ;
    rsrq.r_invid = ipcptr->d25_invinfo.d23_invid ;
    rsrq.r_opval.r_oploc = (R_int *) &ipcptr->d25_ioprid ;

    if (ipcptr->d25_iudat.d25_datlen >
			     (signed32) (1 << (sizeof (signed16) * NBBY)) - 1) {
	rsrq.r_sndata.r_mtype  = R_FILE ;
	rsrq.r_sndata.r_fraref = (char *) &ipcptr->d25_iudat ;
	rsrq.r_sndata.r_fralen = sizeof (D25_usrdat) ;
    } else {
	rsrq.r_sndata.r_mtype  = R_MEMORY ;
	rsrq.r_sndata.r_fraref = (char *) ipcptr->d25_iudat.d25_datref ;
	rsrq.r_sndata.r_fralen = ipcptr->d25_iudat.d25_datlen ;
    }
    rsrq.r_sndata.r_more = R_FALSE ;
    rsrq.r_sndata.r_absid = rosptr->d25_rabsid ;

    /* execute ROS-return result request */
    if (r_resrq (&rsrq) != R_SUCES) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
                svc_c_sev_fatal, GDS_S_ROSFUFAILED_ERR, rsrq.r_hdinfo.r_retcod, 
			       rsrq.r_hdinfo.r_erclas, rsrq.r_hdinfo.r_errval) ;
	return (D25_AERROR) ;
    }
    return (D25_ACONT) ;
}

/* ---------------------- ROS - RETURN ERROR REQUEST ----------------------- */

signed32 d25_roerrrq (D25_actionpb *actpb) {

    R_errqpb		errq ; 
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;
    D25_rosentry	*rosptr = actpb->d25_arosref ;

    /* supply ROS-return error request parameter block */
    errq.r_hdinfo.r_versnb = R_VERSION ;
    errq.r_hdinfo.r_fnctnb = R_ERRRQ ;
    errq.r_cont = R_FALSE ;
    errq.r_otype = R_ACKASN ;
    errq.r_assid = rosptr->d25_rassid ;
    errq.r_erval.r_oploc = (R_int *) &ipcptr->d25_ierrejval ;
    errq.r_invid = ipcptr->d25_invinfo.d23_invid ;

    errq.r_sndata.r_mtype = R_MEMORY ;
    errq.r_sndata.r_more = R_FALSE ;
    errq.r_sndata.r_absid = rosptr->d25_rabsid ;
    errq.r_sndata.r_fraref = (char *) ipcptr->d25_iudat.d25_datref ;
    errq.r_sndata.r_fralen = ipcptr->d25_iudat.d25_datlen ;

    /* execute ROS-return error request */
    if (r_errrq (&errq) != R_SUCES) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
                svc_c_sev_fatal, GDS_S_ROSFUFAILED_ERR, errq.r_hdinfo.r_retcod, 
			       errq.r_hdinfo.r_erclas, errq.r_hdinfo.r_errval) ;
	return (D25_AERROR) ;
    }
    return (D25_ACONT) ;
}

/* ---------------------- ROS - RETURN REJECT REQUEST ---------------------- */

signed32 d25_rorjurq (D25_actionpb *actpb) {

			/* reject reason conversion table */
    static R_reason	rjreason[] = {  R_RINDU, R_RINUO, R_RINMA, R_RINRL,
					R_RINIR, R_RINUL, R_RINLR, R_RINUC,
					R_RREUI, R_RRERU, R_RREMR, R_RERUI,
					R_RERRU, R_RERCE, R_RERXE, R_RERMP } ;

    R_rurqpb		rurq ; 
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;
    D25_rosentry	*rosptr = actpb->d25_arosref ;

    /* supply ROS-reject request by user parameter block */
    rurq.r_hdinfo.r_versnb = R_VERSION ;
    rurq.r_hdinfo.r_fnctnb = R_RJURQ ;
    rurq.r_cont = R_FALSE ;
    rurq.r_otype = R_ACKASN ;
    rurq.r_reason = rjreason[ipcptr->d25_ierrejval] ;
    rurq.r_assid = rosptr->d25_rassid ;
    rurq.r_invid = ipcptr->d25_invinfo.d23_invid ;

    /* execute ROS-reject request by user */
    if (r_rjurq (&rurq) != R_SUCES) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
                svc_c_sev_fatal, GDS_S_ROSFUFAILED_ERR, rurq.r_hdinfo.r_retcod, 
			       rurq.r_hdinfo.r_erclas, rurq.r_hdinfo.r_errval) ;
	return (D25_AERROR) ;
    }
    return (D25_ACONT) ;
}

/* ---------------------- ROS - ABORT REQUEST ------------------------------ */

signed32 d25_rosaborq (D25_actionpb *actpb) {

    R_abrqpb		abrq ; 
    D25_rosentry	*rosptr = actpb->d25_arosref ;

    /* supply ROS-abort association request parameter block */
    abrq.r_hdinfo.r_versnb = R_VERSION ;
    abrq.r_hdinfo.r_fnctnb = R_ABORQ ;
    abrq.r_cont = R_FALSE ;
    abrq.r_otype = R_ACKASN ;
    abrq.r_assid = rosptr->d25_rassid ;

    abrq.r_sndata.r_fralen = 0 ;

    /* execute ROS-abort association request */
    if (r_aborq (&abrq) != R_SUCES) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
                svc_c_sev_fatal, GDS_S_ROSFUFAILED_ERR, abrq.r_hdinfo.r_retcod, 
			       abrq.r_hdinfo.r_erclas, abrq.r_hdinfo.r_errval) ;
	return (D25_AERROR) ;
    }
    return (D25_ACONT) ;
}

/* ---------------------- ROS - PROVIDE BUFFER REQUEST --------------------- */

signed32 d25_rosprbf (D25_actionpb *actpb) {

    R_pvbfpb		pvbf ; 
    R_getpbf		gtbf ;
    D25_stubinfo	*sinfptr = actpb->d25_asinforef ;
    D25_usrdat		rcvdat ;
    signed32		no_prbf ;

    /* supply ROS-get # of unused event buffer parameter block */
    gtbf.r_hdinfo.r_versnb = R_VERSION ;
    gtbf.r_hdinfo.r_fnctnb = R_GTPBF ;
    gtbf.r_cont = R_FALSE ;
    gtbf.r_aplid = sinfptr->d25_aplid ;

    /* execute ROS-get # of unused event buffer */
    if (r_gtpbf (&gtbf) != R_SUCES) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
                svc_c_sev_fatal, GDS_S_ROSFUFAILED_ERR, gtbf.r_hdinfo.r_retcod, 
			       gtbf.r_hdinfo.r_erclas, gtbf.r_hdinfo.r_errval) ;
	return (D25_AERROR) ;
    }
    no_prbf = (sinfptr->d25_ractentries + 2) - gtbf.r_number ;
    while (no_prbf-- > 0) {
	/* supply ROS-provide buffer for receive parameter block */
	pvbf.r_hdinfo.r_versnb = R_VERSION ;
	pvbf.r_hdinfo.r_fnctnb = R_PRBUF ;
	pvbf.r_cont = R_FALSE ;
	pvbf.r_aplid = sinfptr->d25_aplid ;

	pvbf.r_buffer.r_mtype = R_FILE ;
	rcvdat.d25_datref = (byte *) NULL ;
	rcvdat.d25_datlen = rcvdat.d25_datoff = 0 ;
	pvbf.r_buffer.r_bufref = (char *) &rcvdat ;
	pvbf.r_buffer.r_buflen = sizeof (D25_usrdat) ;

	/* execute ROS-provide buffer for receive */
	if (r_prbuf (&pvbf) != R_SUCES) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
                svc_c_sev_fatal, GDS_S_ROSFUFAILED_ERR, pvbf.r_hdinfo.r_retcod, 
			       pvbf.r_hdinfo.r_erclas, pvbf.r_hdinfo.r_errval) ;
	    return (D25_AERROR) ;
	}
    }
    return (D25_ACONT) ;
}

/* ---------------------- ROS - GET TRANSFER SYNTAX REQUEST ---------------- */

signed32 d25_rostrrq (D25_actionpb *actpb) {

    signed32		i ;
    R_getdpb		getd ; 
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;
    D25_rosentry	*rosptr = actpb->d25_arosref ;

    if (ipcptr->d25_iudat.d25_datlen == 0) {
	/* no receipt of user data */
	/* ASN1 transfer syntax assumed, because a default error message */
	/* is generated by the ASN1-decoding functionality in this case */
	rosptr->d25_rtransid = D27_ASN1_TS ;
	return (D25_ACONT) ;
    }

    /* supply ROS-get transfer syntax parameter block */
    getd.r_hdinfo.r_versnb = R_VERSION ;
    getd.r_hdinfo.r_fnctnb = R_GTDCS ;
    getd.r_cont = R_FALSE ;
    getd.r_assid = rosptr->d25_rassid ;

    /* execute ROS-get transfer syntax request */
    if (r_gtdcs (&getd) != R_SUCES) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
                svc_c_sev_fatal, GDS_S_ROSFUFAILED_ERR, getd.r_hdinfo.r_retcod, 
			       getd.r_hdinfo.r_erclas, getd.r_hdinfo.r_errval) ;
	return (D25_AERROR) ;
    }

    /* evaluate transfer syntax */
    for (i = 0; i < (signed32) getd.r_pcnum; i++)
	if (getd.r_prcnt[i].r_absid == rosptr->d25_rabsid)
	    break ;
    if (i == getd.r_pcnum) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
        	  svc_c_sev_warning, GDS_S_NOTRSYNTAX_ERR, rosptr->d25_rabsid) ;
	return (D25_AREXCPT) ;
    }
    rosptr->d25_rtransid = getd.r_prcnt[i].r_trsid ;

    return (D25_ACONT) ;
}

/* ---------------------- ROS - TP-OPEN REQUEST ---------------------------- */

TP_short tp2_001_open (Ptp1001 *opb) {

    signed32	i ;
    D25_tpinfo	*tpiptr ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			       svc_c_debug3, GDS_S_TPOPEN_ENTRY, *opb->tpdir)) ;

    /* check version parameter */
    if (opb->tp_pbhead->tpversion != TP2V02) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
            svc_c_sev_warning, GDS_S_INVVERSNO_ERR, opb->tp_pbhead->tpversion) ;
	opb->tp_pbhead->tpret_code = TP_CALL_ERR ;
	opb->tp_pbhead->tperr_class = TP_ILL_VERS ;
	opb->tp_pbhead->tperr_value = TPNOSUPVS ;
	return (TP_CALL_ERR) ;
    }

    /* check transfer type parameter */
    if (*opb->tpdir != TPIN && *opb->tpdir != TPOUT) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			   svc_c_sev_warning, GDS_S_INVTRANS_ERR, *opb->tpdir) ;
	opb->tp_pbhead->tpret_code = TP_CALL_ERR ;
	opb->tp_pbhead->tperr_class = TP_PAR_ERR ;
	opb->tp_pbhead->tperr_value = TPDIRUSE ;
	return (TP_CALL_ERR) ;
    }

    /* check data description length */
    if (*opb->tplen_data_description != sizeof (D25_usrdat)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
	     svc_c_sev_fatal, GDS_S_INVDATA_ERR, *opb->tplen_data_description) ;
	opb->tp_pbhead->tpret_code = TP_PERM_ERR ;
	opb->tp_pbhead->tperr_class = TP_NOT_SPEC ;
	opb->tp_pbhead->tperr_value = TPINTERR ;
	return (TP_PERM_ERR) ;
    }

    /* search empty TP-send/receive buffer description entry */
    for (i = 0, tpiptr = tpinfref; i < d25_mxtpbuf; i++, tpiptr++)
	if (tpiptr->d25_tpstate == D25_UNUSED)
	    break ;
    if (i == d25_mxtpbuf) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
					 svc_c_sev_warning, GDS_S_NOTPBUF_ERR) ;
	opb->tp_pbhead->tpret_code = TP_TEMP_ERR ;
	opb->tp_pbhead->tperr_class = TP_NOT_SPEC ;
	opb->tp_pbhead->tperr_value = TPINTERR ;
	return (TP_TEMP_ERR) ;
    }

    /* occupy new TP-send/receive buffer description entry */
    tpiptr->d25_tpref = (D25_usrdat *) opb->tpdata_description ;
    if (*opb->tpdir == TPIN) {
	tpiptr->d25_tpstate = D25_TPREAD ;
	tpiptr->d25_tpref->d25_datoff = 0 ;
    } else {
	tpiptr->d25_tpstate = D25_TPWRITE ;
	tpiptr->d25_tpref->d25_datlen = 0 ;
    }

    /* supply output parameter */
    *opb->tpopenid = i ;
    *opb->tpfilelength = -1 ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
					  svc_c_debug3, GDS_S_TPOPEN_EXIT, i)) ;
    return (TP_NO_ERR) ;
}

/* ---------------------- ROS - TP-READ REQUEST ---------------------------- */

TP_short tp2_002_read (Ptp1002 *rpb) {

    signed32	rdlen, datlen, openid = *rpb->tpopenid ;
    D25_tpinfo	*tpiptr = tpinfref + openid ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
    svc_c_debug3, GDS_S_TPREAD_ENTRY, openid, *rpb->tpinrequired_data_length)) ;

    /* check version parameter */
    if (rpb->tp_pbhead->tpversion != TP2V02) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
            svc_c_sev_warning, GDS_S_INVVERSNO_ERR, rpb->tp_pbhead->tpversion) ;
	rpb->tp_pbhead->tpret_code = TP_CALL_ERR ;
	rpb->tp_pbhead->tperr_class = TP_ILL_VERS ;
	rpb->tp_pbhead->tperr_value = TPNOSUPVS ;
	return (TP_CALL_ERR) ;
    }

    /* check open-ID parameter */
    if (openid < 0 || openid >= d25_mxtpbuf || tpiptr->d25_tpstate !=
								   D25_TPREAD) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			       svc_c_sev_warning, GDS_S_INVOPENID_ERR, openid) ;
	rpb->tp_pbhead->tpret_code = TP_CALL_ERR ;
	rpb->tp_pbhead->tperr_class = TP_PAR_ERR ;
	rpb->tp_pbhead->tperr_value = TPOPENID ;
	return (TP_CALL_ERR) ;
    }

    /* transfer TP-read user data */
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
		svc_c_debug3, GDS_S_TPREAD_OPR, tpiptr->d25_tpref->d25_datlen,
					       tpiptr->d25_tpref->d25_datoff)) ;
    datlen = tpiptr->d25_tpref->d25_datlen - tpiptr->d25_tpref->d25_datoff ;
    rdlen = (*rpb->tpinrequired_data_length >= datlen) ? datlen :
						*rpb->tpinrequired_data_length ;

    (void) memcpy ((void *) rpb->tpinbufferaddress,
    (void *) (tpiptr->d25_tpref->d25_datref + tpiptr->d25_tpref->d25_datoff),
							       (size_t) rdlen) ;

    /* update TP-read data description buffer */
    tpiptr->d25_tpref->d25_datoff += rdlen ;

    /* supply output parameter */
    *rpb->tpread_data_length = rdlen ;
    *rpb->tpinfurther_data = (datlen > rdlen) ? TP_YES : TP_NO ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
				      svc_c_debug3, GDS_S_TPREAD_EXIT, rdlen)) ;
    return (TP_NO_ERR) ;
}

/* ---------------------- ROS - TP-WRITE REQUEST --------------------------- */

TP_short tp2_003_write (Ptp1003 *wpb) {

    signed32	openid = *wpb->tpopenid ;
    signed32	wrlen = *wpb->tpoutrequired_data_length ;
    signed32	datoff, datlen ;
    byte	*datref, *tmpref ;
    D25_tpinfo	*tpiptr = tpinfref + openid ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
		            svc_c_debug3, GDS_S_TPWRITE_ENTRY, openid, wrlen)) ;

    /* check version parameter */
    if (wpb->tp_pbhead->tpversion != TP2V02) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
            svc_c_sev_warning, GDS_S_INVVERSNO_ERR, wpb->tp_pbhead->tpversion) ;
	wpb->tp_pbhead->tpret_code = TP_CALL_ERR ;
	wpb->tp_pbhead->tperr_class = TP_ILL_VERS ;
	wpb->tp_pbhead->tperr_value = TPNOSUPVS ;
	return (TP_CALL_ERR) ;
    }

    /* check open-ID parameter */
    if (openid < 0 || openid >= d25_mxtpbuf || tpiptr->d25_tpstate !=
								  D25_TPWRITE) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			       svc_c_sev_warning, GDS_S_INVOPENID_ERR, openid) ;
	wpb->tp_pbhead->tpret_code = TP_CALL_ERR ;
	wpb->tp_pbhead->tperr_class = TP_PAR_ERR ;
	wpb->tp_pbhead->tperr_value = TPOPENID ;
	return (TP_CALL_ERR) ;
    }

    /* transfer TP-write user data */
    datref = tpiptr->d25_tpref->d25_datref ;
    datlen = tpiptr->d25_tpref->d25_datlen ;
    datoff = tpiptr->d25_tpref->d25_datoff ;
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
			     svc_c_debug3, GDS_S_TPWRITE_OPR, datlen, datoff)) ;
    if (datlen + wrlen > datoff) {
	if (datlen <= datoff) {
	    /* allocate receive buffer space */
	    if ((datref = (byte *) malloc ((size_t) (datlen + wrlen))) ==
							     (byte *) NULL) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			    svc_c_sev_warning, GDS_S_NOMEMORY, datlen + wrlen) ;
		wpb->tp_pbhead->tpret_code = TP_TEMP_ERR ;
		wpb->tp_pbhead->tperr_class = TP_NOT_SPEC ;
		wpb->tp_pbhead->tperr_value = TPINTERR ;
		return (TP_TEMP_ERR) ;
	    }
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
		    svc_c_debug1, GDS_S_ALO_SRBUFFER, datref, datlen + wrlen)) ;
	    if (datlen > 0) {
		/* transfer TP-write data which are already available */
		/* into new allocated buffer */
		(void) memcpy ((void *) datref,
		      (void *) tpiptr->d25_tpref->d25_datref, (size_t) datlen) ;
	    }
	} else {
	    /* allocate additional receive buffer space */
	    if ((tmpref = (byte *) realloc ((void *) datref,
			      (size_t) (datlen + wrlen))) == (byte *) NULL) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			    svc_c_sev_warning, GDS_S_NOMEMORY, datlen + wrlen) ;
		wpb->tp_pbhead->tpret_code = TP_PERM_ERR ;
		wpb->tp_pbhead->tperr_class = TP_NOT_SPEC ;
		wpb->tp_pbhead->tperr_value = TPINTERR ;
		return (TP_PERM_ERR) ;
	    }
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
			svc_c_debug1, GDS_S_REALO_SRBUFFER, datref, tmpref,
							      datlen + wrlen)) ;
	    datref = tmpref ;
	}
	tpiptr->d25_tpref->d25_datref = datref ;
    }
    memcpy ((void *) (datref + datlen), (void *) wpb->tpoutbufferaddress,
							       (size_t) wrlen) ;

    /* update TP-write data description buffer */
    tpiptr->d25_tpref->d25_datlen += wrlen ;

    /* supply output parameter */
    *wpb->tpwritten_data_length = wrlen ;
    *wpb->tpoutfurtherdataspace= TP_YES ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
				     svc_c_debug3, GDS_S_TPWRITE_EXIT, wrlen)) ;
    return (TP_NO_ERR) ;
}

/* ---------------------- ROS - TP-CLOSE REQUEST --------------------------- */

TP_short tp2_005_close (Ptp1005 *cpb) {

    signed32	openid = *cpb->tpopenid ;
    D25_tpinfo	*tpiptr = tpinfref + openid ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
				   svc_c_debug3, GDS_S_TPCLOSE_ENTRY, openid)) ;

    /* check version parameter */
    if (cpb->tp_pbhead->tpversion != TP2V02) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
            svc_c_sev_warning, GDS_S_INVVERSNO_ERR, cpb->tp_pbhead->tpversion) ;
	cpb->tp_pbhead->tpret_code = TP_CALL_ERR ;
	cpb->tp_pbhead->tperr_class = TP_ILL_VERS ;
	cpb->tp_pbhead->tperr_value = TPNOSUPVS ;
	return (TP_CALL_ERR) ;
    }

    /* check open-ID parameter */
    if (openid < 0 || openid >= d25_mxtpbuf || tpiptr->d25_tpstate ==
								   D25_UNUSED) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			       svc_c_sev_warning, GDS_S_INVOPENID_ERR, openid) ;
	cpb->tp_pbhead->tpret_code = TP_CALL_ERR ;
	cpb->tp_pbhead->tperr_class = TP_PAR_ERR ;
	cpb->tp_pbhead->tperr_value = TPOPENID ;
	return (TP_CALL_ERR) ;
    }

    /* update TP-write data description buffer */
    tpiptr->d25_tpref->d25_datoff = 0 ;

    /* release TP-send/receive buffer description entry */
    tpiptr->d25_tpstate = D25_UNUSED ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
					    svc_c_debug3, GDS_S_TPCLOSE_EXIT)) ;
    return (TP_NO_ERR) ;
}
