/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25revent.c,v $
 * Revision 1.1.12.2  1996/02/18  18:23:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:19  marty]
 *
 * Revision 1.1.12.1  1995/12/08  16:15:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:20:57  root]
 * 
 * Revision 1.1.10.4  1994/09/06  12:26:31  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:24:54  keutel]
 * 
 * Revision 1.1.10.3  1994/05/10  16:03:30  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:12:48  marrek]
 * 
 * Revision 1.1.10.2  1994/03/23  15:47:25  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:33:43  keutel]
 * 
 * Revision 1.1.10.1  1994/02/22  17:55:24  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:52:15  marrek]
 * 
 * Revision 1.1.8.2  1993/11/09  15:00:04  keutel
 * 	modified event handling (shared ROS-association -> OT9358)
 * 	[1993/11/09  14:43:48  keutel]
 * 
 * Revision 1.1.8.1  1993/10/14  19:19:02  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:16:08  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/09  17:58:04  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  08:29:30  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:20:06  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  22:04:17  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:48:08  bbelch]
 * 
 * Revision 1.1.2.3  1992/07/06  18:13:58  melman
 * 	CR 3975: xds test core dump client to client/server
 * 	[1992/07/06  18:11:49  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  23:09:52  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:33:23  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25revent.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 18:23:24 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d25_rosevent ()                                                */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function gets and evaluates all events which are received at    */
/*       the ROS-interface.                                                  */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       evntpb      = Reference to the function parameter block which       */
/*                     must contain the following input parameter:           */
/*                             - reference to the Stub-information table     */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     The function parameter block contains the output      */
/*		       parameter as follows:                                 */
/*                             - reference to the actual ROS-table entry 1)  */
/*                             - reference to the actual IPC-table entry 1)  */
/*                             - actual state                            1)  */
/*                             - received event type                         */
/*                                                                           */
/*                             1) = parameter is invalid if the received     */
/*                                  event type is equal to D25_IRNO_EV       */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*       > 1         = Function successfully executed (several events are    */
/*                     available)                                            */
/*       = 1         = Function successfully executed (zero or one event     */
/*                     is available)                                         */
/*       D25_ERROR   = A system error or an internal error is occurred       */
/*                     during execution of the function.                     */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 3.6.91      */
/*exoff **********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ros.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>
#include <d23apdu.h>
#include <d25stub.h>



/* --------------------- ROS - EVENT --------------------------------------- */

signed32 d25_rosevent (D25_eventpb *evntpb, R_evtpb *rosev) {

    signed32		i, ev, ipcidx, rosidx, no_efound ;
    D25_stubinfo	*sinfptr = evntpb->d25_esinforef ;
    D25_ipcentry	*ipcptr, *tmpipcptr ;
    D25_rosentry	*rosptr, *tmprosptr ;
    R_evdata		*rcvdatref = (R_evdata *) NULL ;
    static signed32	no_revnt = 0 ;

    /* evaluate received ROS-event type */
    /* a temporary ROS-table entry resp. IPC-table entry is used to store */
    /* received event information */
    ipcidx = sinfptr->d25_imaxentries ;
    ipcptr = sinfptr->d25_itabref + ipcidx ;
    ipcptr->d25_istate = D25_SIOS(D25_IRIDLE) | D25_SITY(D25_DACTIVE) ;
    ipcptr->d25_iassid =
    ipcptr->d25_ioprid = ipcptr->d25_iresid = D25_UNUSED ;
    ipcptr->d25_invinfo.d23_invid = D23_UNUSED ;
    ipcptr->d25_invinfo.d23_usrid = D25_UNUSED ;
    ipcptr->d25_iudat.d25_datlen = ipcptr->d25_iudat.d25_datoff = 0 ;
    ipcptr->d25_iudat.d25_datref = (byte *) NULL ;

    rosidx = sinfptr->d25_rmaxentries ;
    rosptr = sinfptr->d25_rtabref + rosidx ;
    rosptr->d25_rassid = rosptr->d25_rabsid = D25_UNUSED ;
    rosptr->d25_rrefcnt = 1 ;
    rosptr->d25_rstate = D25_SRES(D25_RBUSY) ;

    evntpb->d25_eipcref = ipcptr ;
    evntpb->d25_erosref = rosptr ;
    evntpb->d25_estate = D25_SIOS(D25_IRIDLE) ;

    switch ((ev = rosev->r_evnthd.r_evtype)) {
    case R_BNINEV:
	/* bind request indication event */
	/* get context-ID resp. abstract syntax-ID */
	for (i = 0; i < sinfptr->d25_nacentries; i++)
	    if (rosev->r_bninev.r_ptcon.r_obidnb ==
		(sinfptr->d25_actabref + i)->r_objnum &&
		memcmp (rosev->r_bninev.r_ptcon.r_obidrf,
		sinfptr->d25_oitabref + (sinfptr->d25_actabref + i)->r_apcinx,
		     rosev->r_bninev.r_ptcon.r_obidnb * sizeof (R_unsign)) == 0)
		break ;
	if (i == sinfptr->d25_nacentries) {
	    /* unknown application context */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			    svc_c_sev_warning, GDS_S_ACONTEXT_ERR, D25_UNUSED) ;
	    evntpb->d25_eevent = D25_REXCPT_EV ;
	    return (1) ;
	}
	rosptr->d25_rassid = rosev->r_bninev.r_assid ;
	rosptr->d25_rcontid = i + 1 ;
	rosptr->d25_rabsid = rosev->r_bninev.r_evdata.r_absid ;

	rcvdatref = &rosev->r_bninev.r_evdata ;

	ipcptr->d25_ioprid = D23_BIND ;
	ipcptr->d25_invinfo.d23_usrid = rosev->r_bninev.r_assid ;
	evntpb->d25_eevent = D25_RBIND_EV ;
	break ;
    case R_BNCNEV:
	/* bind request confirmation event */
	rosptr->d25_rassid = rosev->r_bncnev.r_assid ;
	rcvdatref = &rosev->r_bncnev.r_evdata ;
	if (rosev->r_bncnev.r_result == R_ACCEPT) {
	    ipcptr->d25_iresid = D23_RESULT ;
	    evntpb->d25_eevent = D25_RCFBIND_EV ;
	} else {
	    ipcptr->d25_iresid = D23_RREJECT ;
	    evntpb->d25_eevent = D25_RRJBIND_EV ;
	}
	break ;
    case R_UBINEV:
	/* unbind request indication event */
	rosptr->d25_rassid = rosev->r_ubinev.r_assid ;
	ipcptr->d25_ioprid = D23_UNBIND ;
	rcvdatref = &rosev->r_ubinev.r_evdata ;
	evntpb->d25_eevent = D25_RUNBIND_EV ;
	break ;
    case R_UBCNEV:
	/* unbind request confirmation event */
	rosptr->d25_rassid = rosev->r_ubcnev.r_assid ;
	ipcptr->d25_iresid = D23_RESULT ;
	rcvdatref = &rosev->r_ubcnev.r_evdata ;
	evntpb->d25_eevent = D25_RCFUBIND_EV ;
	break ;
    case R_ABINEV:
	/* association abort indication event */
	rosptr->d25_rassid = rosev->r_abinev.r_assid ;
	ipcptr->d25_iresid = D23_ABORT ;
	rcvdatref = &rosev->r_abinev.r_evdata ;
	evntpb->d25_eevent = D25_RABORT_EV ;
	break ;
    case R_IVINEV:
	/* invoke indication event */
	ipcptr->d25_istate = D25_SIOS(D25_IRASSOCIATION) | D25_SITY(D25_DACTIVE) ;
	rosptr->d25_rassid = rosev->r_ivinev.r_assid ;
	ipcptr->d25_ioprid = *rosev->r_ivinev.r_opval.r_oploc ;
	ipcptr->d25_invinfo.d23_invid = rosev->r_ivinev.r_invid ;
	rcvdatref = &rosev->r_ivinev.r_evdata ;
	evntpb->d25_eevent = D25_RINVOKE_EV ;
	break ;
    case R_RSINEV:
	/* result indication event */
	rosptr->d25_rassid = rosev->r_rsinev.r_assid ;
	ipcptr->d25_invinfo.d23_invid = rosev->r_rsinev.r_invid ;
	ipcptr->d25_iresid = D23_RESULT ;
	rcvdatref = &rosev->r_rsinev.r_evdata ;
	evntpb->d25_eevent = D25_RRESULT_EV ;
	break ;
    case R_ERINEV:
	/* error indication event */
	rosptr->d25_rassid = rosev->r_erinev.r_assid ;
	ipcptr->d25_invinfo.d23_invid = rosev->r_erinev.r_invid ;
	ipcptr->d25_iresid = (*rosev->r_erinev.r_erval.r_oploc > 0 &&
		*rosev->r_erinev.r_erval.r_oploc <= D23_MXRERR) ?
		D23_RERROR + *rosev->r_erinev.r_erval.r_oploc : D23_RERROR ;
	rcvdatref = &rosev->r_erinev.r_evdata ;
	evntpb->d25_eevent = D25_RERROR_EV ;
	break ;
    case R_RUINEV:
	/* reject by user indication event */
	rosptr->d25_rassid = rosev->r_ruinev.r_assid ;
	ipcptr->d25_invinfo.d23_invid = rosev->r_ruinev.r_invid ;
	ipcptr->d25_iresid = (rosev->r_ruinev.r_reason > 0 &&
			rosev->r_ruinev.r_reason <= D23_MXRREJ) ?
			D23_RREJECT + rosev->r_ruinev.r_reason : D23_RREJECT ;
	evntpb->d25_eevent = D25_RUSRREJ_EV ;
	break ;
    case R_RPINEV:
	/* reject by provider indication event */
	rosptr->d25_rassid = rosev->r_rpinev.r_assid ;
	ipcptr->d25_iresid = D23_RREJECT ;
	if (rosev->r_rpinev.r_valid == R_TRUE)
	    ipcptr->d25_invinfo.d23_invid = rosev->r_rpinev.r_invid ;
	if (rosev->r_rpinev.r_reason == R_NTRAN) {
	    rcvdatref = &rosev->r_rpinev.r_prdata.r_evdata ;
	}
	evntpb->d25_eevent = D25_RPROVREJ_EV ;
	break ;
    case R_RBINEV:
	/* buffer return indication event */
	ipcptr->d25_iudat.d25_datref = 
		(rosev->r_rbinev.r_buffer.r_mtype == R_MEMORY) ?
		(byte *) rosev->r_rbinev.r_buffer.r_bufref :
		((D25_usrdat *) rosev->r_rbinev.r_buffer.r_bufref)->d25_datref ;

	evntpb->d25_eevent = D25_RRETBUF_EV ;
	break ;
    default:
	evntpb->d25_eevent = D25_IRNO_EV ;
	break ;
    }

    if (rcvdatref != (R_evdata *) NULL) {
	if (rcvdatref->r_specfl == R_TRUE)
	    /* unsuccessful receive of data */
	    evntpb->d25_eevent = D25_REXCPT_EV ;
	else {
	    if (rcvdatref->r_datlen > 0) {
		/* evaluate received data */
		if (rcvdatref->r_mtype != R_FILE || rcvdatref->r_buflen !=
							 sizeof (D25_usrdat)) {
		    /* unexpected receive buffer type resp. buffer length */
	    	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"),
			GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_INVRCVBUFFER_ERR,
			rcvdatref->r_mtype, rcvdatref->r_buflen) ;
		    return (D25_ERROR) ;
		}
		rosptr->d25_rabsid = rcvdatref->r_absid ;
		ipcptr->d25_iudat = *((D25_usrdat *) rcvdatref->r_bufref) ;
	    }
	}
    }

    /* search corresponding ROS-table entry */
    for (i = 0, tmprosptr = sinfptr->d25_rtabref;
				 i < sinfptr->d25_rmaxentries; i++, tmprosptr++)
        if (D25_GRES(tmprosptr->d25_rstate) == D25_RBUSY &&
				    tmprosptr->d25_rassid == rosptr->d25_rassid)
	    break ;
    if (i < sinfptr->d25_rmaxentries) {
	if (rosptr->d25_rabsid != D25_UNUSED && rosptr->d25_rabsid !=
						        tmprosptr->d25_rabsid) {
	    /* unexpected abstract syntax identifier */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
		      svc_c_sev_fatal, GDS_S_INVABSID_ERR, rosptr->d25_rabsid) ;
	    return (D25_ERROR) ;
	}
	rosidx = i ;
	rosptr = tmprosptr ;

	/* search corresponding IPC-table entry */
	for (i = no_efound = 0, tmpipcptr = sinfptr->d25_itabref;
				 i < sinfptr->d25_imaxentries; i++, tmpipcptr++)
	    if (D25_GIOS(tmpipcptr->d25_istate) != D25_IRIDLE &&
		tmpipcptr->d25_irosref == rosptr &&
		( ev == R_BNCNEV || ev == R_UBINEV || ev == R_UBCNEV ||
		  ev == R_ABINEV || ev == R_RPINEV ||
		((ev == R_RSINEV || ev == R_ERINEV || ev == R_RUINEV) &&
		tmpipcptr->d25_invinfo.d23_invid == ipcptr->d25_invinfo.d23_invid) ||
		(ev == R_IVINEV && D25_GIOS(tmpipcptr->d25_istate) ==
							  D25_IRASSOCIATION))) {
		if (no_revnt == 0 ||
				  rosptr->d25_rrefcnt - no_revnt == no_efound++)
	            break ;
	    }
	if (i < sinfptr->d25_imaxentries) {
	    tmpipcptr->d25_iudat = ipcptr->d25_iudat ;
	    if (ipcptr->d25_invinfo.d23_invid != D23_UNUSED)
		tmpipcptr->d25_invinfo.d23_invid = ipcptr->d25_invinfo.d23_invid ;
	    if (ipcptr->d25_invinfo.d23_usrid != D25_UNUSED)
		tmpipcptr->d25_invinfo.d23_usrid = ipcptr->d25_invinfo.d23_usrid ;
	    if (ipcptr->d25_ioprid != D25_UNUSED)
		tmpipcptr->d25_ioprid = ipcptr->d25_ioprid ;
	    if (ipcptr->d25_iresid != D25_UNUSED)
		tmpipcptr->d25_iresid = ipcptr->d25_iresid ;
	    ipcidx = i ;
	    ipcptr = tmpipcptr ;
	}
    }

    /* if the ROS-table entry is referenced by several IPC-table entries, */
    /* then one event must be announced for every IPC-table entry */
    if (no_revnt == 0) { 
	no_revnt = (rosptr->d25_rrefcnt > 1 && (ev == R_ABINEV ||
						ev == R_UBINEV ||
						ev == R_RPINEV)) ?
						rosptr->d25_rrefcnt : 1 ;
    }

    /* supply output parameter */
    evntpb->d25_eipcref = ipcptr ;
    evntpb->d25_erosref = rosptr ;
    evntpb->d25_estate = D25_GIOS(ipcptr->d25_istate) ;

    if (evntpb->d25_eevent == D25_RCFBIND_EV &&
					     ipcptr->d25_iudat.d25_datlen > 0) {
	/* save bind result data for later use (-> association sharing) */
	if ((rosptr->d25_rbndcfref = (byte *)
		      malloc (ipcptr->d25_iudat.d25_datlen)) == (byte *) NULL) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
	      svc_c_sev_warning, GDS_S_NOMEMORY, ipcptr->d25_iudat.d25_datlen) ;
	    evntpb->d25_eevent = D25_REXCPT_EV ;
	} else {
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
			svc_c_debug1, GDS_S_ALO_BNDCFMEM,
			rosptr->d25_rbndcfref, ipcptr->d25_iudat.d25_datlen)) ;
	    memcpy (rosptr->d25_rbndcfref, ipcptr->d25_iudat.d25_datref,
						 ipcptr->d25_iudat.d25_datlen) ;
	    rosptr->d25_rbndcflen = ipcptr->d25_iudat.d25_datlen ;
	}
    }

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d%d%d%d"), GDS_S_GENERAL,
	svc_c_debug1, GDS_S_ROS_EVINFO, ev, evntpb->d25_eevent,
	no_revnt, rosidx, rosptr->d25_rassid, ipcidx, ipcptr->d25_iassid)) ;
    return (no_revnt--) ;
}
