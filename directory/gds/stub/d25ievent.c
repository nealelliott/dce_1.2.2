/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25ievent.c,v $
 * Revision 1.1.11.2  1996/02/18  18:23:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:15  marty]
 *
 * Revision 1.1.11.1  1995/12/08  16:15:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:20:51  root]
 * 
 * Revision 1.1.9.3  1994/09/06  12:26:28  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:24:40  keutel]
 * 
 * Revision 1.1.9.2  1994/05/10  16:03:28  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:12:35  marrek]
 * 
 * Revision 1.1.9.1  1994/02/22  17:55:19  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:51:51  marrek]
 * 
 * Revision 1.1.7.2  1993/08/11  08:27:23  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:19:33  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  22:04:04  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:47:58  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:09:31  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:33:08  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25ievent.c,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 18:23:20 $";
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
/*   NAME:    d25_ipcevent ()                                                */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function gets and evaluates all events which are received at    */
/*       the IPC-interface.                                                  */
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
/*       D25_SUCES   = The function has been performed successfully.         */
/*       D25_ERROR   = A system error or an internal error is occurred       */
/*                     during execution of the function.                     */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 3.6.91      */
/*exoff **********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <ros.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>
#include <d25stub.h>
#include <d23apdu.h>



/* ------------------------------------------------------------------------- */

signed32 d25_ipcevent (D25_eventpb *evntpb) {

    signed32		i, ipcidx, rosidx ;
    byte		*rcvptr ;
    D23_evntpb		evpb ;
    D25_stubinfo	*sinfptr = evntpb->d25_esinforef ;
    D25_ipcentry	*ipcptr = sinfptr->d25_itabref ;
    D25_rosentry	*rosptr = sinfptr->d25_rtabref ;

    /* supply IPC-event parameter block */
    evpb.d23_Bhdinfo.d2_version = D23_V02 ;

    /* get IPC-event */
    if (d23_event (&evpb) == D23_ERROR)
	return (D25_ERROR) ;

    /* evaluate received IPC-event */
    if (evpb.d23_Bevtype == D23_NOEVNT) {
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
					       svc_c_debug1, GDS_S_NO_IPC_EV)) ;
	evntpb->d25_eevent = D25_IRNO_EV ;
	return (D25_SUCES) ;
    }

    /* search corresponding IPC-table entry */
    for (i = 0; i < sinfptr->d25_imaxentries; i++, ipcptr++)
	if (D25_GIOS(ipcptr->d25_istate) != D25_IRIDLE &&
					  ipcptr->d25_iassid == evpb.d23_Bassid)
	    break ;
    if (i == sinfptr->d25_imaxentries) {
	/* no corresponding IPC-table entry found -> */
	/* use temporary IPC-table resp. ROS-table entry */
	ipcptr->d25_iassid = evpb.d23_Bassid ;
	ipcptr->d25_irosref = (D25_rosentry *) NULL ;
	ipcptr->d25_ibufsz = sinfptr->d25_imbufsz ;
	ipcptr->d25_ibufref = evpb.d23_Bref ;
	ipcptr->d25_ioprid =
	ipcptr->d25_iresid = D25_UNUSED ;
	ipcptr->d25_istate = D25_SIOS(D25_IRIDLE) | D25_SITY(D25_DPASSIVE) ;

	/* occupy temporary ROS-table entry */
	rosidx = sinfptr->d25_rmaxentries ;
	rosptr += rosidx ;
	rosptr->d25_rassid = D25_UNUSED ;
	rosptr->d25_rcontid = D23_GAC(evpb.d23_Binvinfo.d23_contid) ;
	rosptr->d25_rabsid = D23_GAS(evpb.d23_Binvinfo.d23_contid) ;
	rosptr->d25_rtransid = D23_GTS(evpb.d23_Binvinfo.d23_contid) ;
	rosptr->d25_rstate = D25_SRES(D25_RBUSY) | D25_SRIF(D25_RINITIATOR) ;
    } else {
	if ((rosptr = ipcptr->d25_irosref) != (D25_rosentry *) NULL)
	    rosidx = ((byte *) rosptr - (byte *) sinfptr->d25_rtabref)/
							 sizeof (D25_rosentry) ;
	else
	    rosidx = D25_UNUSED ;
    }
    ipcidx = i ;

    /* supply output parameter */
    evntpb->d25_eipcref = ipcptr ;
    evntpb->d25_erosref = rosptr ;
    evntpb->d25_estate = D25_GIOS(ipcptr->d25_istate) ;

    /* evaluate event-type */
    switch (evpb.d23_Bevtype) {
    case D23_INVEVNT:
    case D23_INVCEVNT:
	/* invoke indication event */
	if (evpb.d23_Bevtype == D23_INVEVNT) {
	    ipcptr->d25_iudat.d25_datref = ipcptr->d25_ibufref ;
	    ipcptr->d25_iudat.d25_datoff = 0 ;
	    ipcptr->d25_iudat.d25_datlen = evpb.d23_Bln ;
	    ipcptr->d25_ioprid = evpb.d23_Bopid ;
	    ipcptr->d25_invinfo = evpb.d23_Binvinfo ;
	} else
	    ipcptr->d25_iudat.d25_datlen += evpb.d23_Bln ;

	/* remove invoke data from IPC-buffer */
	if (ipcptr->d25_iudat.d25_datoff > 0 || evpb.d23_Bmdat == D23_MRDATA) {
	    rcvptr = (ipcptr->d25_iudat.d25_datoff == 0) ?
				malloc ((size_t) ipcptr->d25_iudat.d25_datlen) :
				realloc ((void *) ipcptr->d25_iudat.d25_datref,
					(size_t) ipcptr->d25_iudat.d25_datlen) ;
	    if (rcvptr == (byte *) NULL) {
		/* no memory available */
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
					svc_c_sev_warning, GDS_S_NOMEMORY,
					ipcptr->d25_iudat.d25_datlen) ;
		evntpb->d25_eevent = D25_IEXCPT_EV ;
		return (D25_SUCES) ;
	    }
	    if (ipcptr->d25_iudat.d25_datoff > 0) {
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
		svc_c_debug1, GDS_S_REALO_SRBUFFER, ipcptr->d25_iudat.d25_datref,
					rcvptr, ipcptr->d25_iudat.d25_datlen)) ;
	    } else {
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
		svc_c_debug1, GDS_S_ALO_SRBUFFER, rcvptr,
						ipcptr->d25_iudat.d25_datlen)) ;
	    }
	    ipcptr->d25_iudat.d25_datref = rcvptr ;
	    (void) memcpy ((void *) (ipcptr->d25_iudat.d25_datref +
	    ipcptr->d25_iudat.d25_datoff), (void *) ipcptr->d25_ibufref,
	    (size_t) (ipcptr->d25_iudat.d25_datlen -
						ipcptr->d25_iudat.d25_datoff)) ;
	    ipcptr->d25_iudat.d25_datoff = ipcptr->d25_iudat.d25_datlen ;
	}

	evntpb->d25_eevent = (evpb.d23_Bmdat == D23_MRDATA) ? D25_IMINVOKE_EV :
			     (evpb.d23_Bopid == D23_BIND)   ? D25_IBIND_EV :
			     (evpb.d23_Bopid == D23_UNBIND) ? D25_IUNBIND_EV :
							      D25_IINVOKE_EV ;
	break ;
    case D23_RESEVNT:
    case D23_RESCEVNT:
	/* result indication event */
	if (evpb.d23_Bevtype == D23_RESEVNT) {
	    ipcptr->d25_iudat.d25_datref = ipcptr->d25_ibufref ;
	    ipcptr->d25_iudat.d25_datoff = 0 ;
	    ipcptr->d25_iudat.d25_datlen = evpb.d23_Bln ;
	    ipcptr->d25_iresid = evpb.d23_Bopid ;
	} else
	    ipcptr->d25_iudat.d25_datlen += evpb.d23_Bln ;

	/* remove result data from IPC-buffer */
	if (ipcptr->d25_iudat.d25_datoff > 0 || evpb.d23_Bmdat == D23_MRDATA) {
	    rcvptr = (ipcptr->d25_iudat.d25_datoff == 0) ?
				malloc ((size_t) ipcptr->d25_iudat.d25_datlen) :
				realloc ((void *) ipcptr->d25_iudat.d25_datref,
					(size_t) ipcptr->d25_iudat.d25_datlen) ;
	    if (rcvptr == (byte *) NULL) {
		/* no memory available */
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
					svc_c_sev_warning, GDS_S_NOMEMORY,
					ipcptr->d25_iudat.d25_datlen) ;
		evntpb->d25_eevent = D25_IEXCPT_EV ;
		return (D25_SUCES) ;
	    }
	    if (ipcptr->d25_iudat.d25_datoff > 0) {
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
		svc_c_debug1, GDS_S_REALO_SRBUFFER, ipcptr->d25_iudat.d25_datref,
					rcvptr, ipcptr->d25_iudat.d25_datlen)) ;
	    } else {
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
		svc_c_debug1, GDS_S_ALO_SRBUFFER, rcvptr,
						ipcptr->d25_iudat.d25_datlen)) ;
	    }
	    ipcptr->d25_iudat.d25_datref = rcvptr ;
	    (void) memcpy ((void *) (ipcptr->d25_iudat.d25_datref +
	    ipcptr->d25_iudat.d25_datoff), (void *) ipcptr->d25_ibufref,
	    (size_t) (ipcptr->d25_iudat.d25_datlen -
						ipcptr->d25_iudat.d25_datoff)) ;
	    ipcptr->d25_iudat.d25_datoff = ipcptr->d25_iudat.d25_datlen ;
	}

	evntpb->d25_eevent = (evpb.d23_Bmdat == D23_MRDATA) ? D25_IMRESULT_EV :
			     (evpb.d23_Bopid >= D23_RERROR && evpb.d23_Bopid <=
			     D23_RERROR + D23_MXRERR)      ?  D25_IERROR_EV :
			     (evpb.d23_Bopid >= D23_RREJECT && evpb.d23_Bopid <=
			     D23_RREJECT + D23_MXRREJ)      ? D25_IREJECT_EV :
							      D25_IRESULT_EV ;
	if (evntpb->d25_eevent == D25_IERROR_EV)
	    ipcptr->d25_ierrejval = ipcptr->d25_iresid - D23_RERROR ;
	else if (evntpb->d25_eevent == D25_IREJECT_EV)
	    ipcptr->d25_ierrejval = ipcptr->d25_iresid - D23_RREJECT ;
	break ;
    case D23_RRQBFEVNT:
    case D23_RRSBFEVNT:
	/* return buffer event */
	evntpb->d25_eevent = D25_IRETBUF_EV ;
	break ;
    case D23_ABOEVNT:
	/* abort event */
	ipcptr->d25_ioprid = ipcptr->d25_iresid = evpb.d23_Bopid ;
	evntpb->d25_eevent = D25_IABORT_EV ;
	break ;
    default:
	evntpb->d25_eevent = D25_IRNO_EV ;
	break ;
    }
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d%d"), GDS_S_GENERAL,
	svc_c_debug1, GDS_S_IPC_EVINFO, evntpb->d25_eevent, ipcidx,
	ipcptr->d25_iassid, rosidx, (rosptr != (D25_rosentry *) NULL) ? 
					     rosptr->d25_rassid : D25_UNUSED)) ;
    return (D25_SUCES) ;
}
