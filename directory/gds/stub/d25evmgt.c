/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25evmgt.c,v $
 * Revision 1.1.12.2  1996/02/18  18:23:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:12  marty]
 *
 * Revision 1.1.12.1  1995/12/08  16:15:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:20:48  root]
 * 
 * Revision 1.1.10.4  1994/09/06  12:26:26  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:24:29  keutel]
 * 
 * Revision 1.1.10.3  1994/06/21  14:48:13  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:45:44  marrek]
 * 
 * Revision 1.1.10.2  1994/05/10  16:03:25  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:12:26  marrek]
 * 
 * Revision 1.1.10.1  1994/02/22  17:55:14  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:51:29  marrek]
 * 
 * Revision 1.1.8.1  1993/10/14  19:13:10  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:11:05  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/09  18:01:15  keutel]
 * 
 * Revision 1.1.6.4  1993/08/11  08:25:22  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:19:06  marrek]
 * 
 * Revision 1.1.6.3  1993/07/12  20:57:40  treff
 * 	DCT @ OSF
 * 	replace ifdef __HPUX__ with ifdef __hpux
 * 	[1993/07/12  19:43:52  treff]
 * 
 * Revision 1.1.6.2  1993/07/02  15:16:24  marrek
 * 	Changes for port to HP800.
 * 	[1993/07/02  14:41:42  marrek]
 * 
 * Revision 1.1.4.5  1992/12/31  22:03:47  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:47:49  bbelch]
 * 
 * Revision 1.1.4.4  1992/12/09  16:15:32  marrek
 * 	November 1992 code drop
 * 	add missing usrpar argument in d25_ev_select
 * 	[1992/12/09  14:42:07  marrek]
 * 
 * Revision 1.1.4.3  1992/12/04  18:12:08  marrek
 * 	November 1992 code drop
 * 	add third argument in r_callback
 * 	[1992/12/04  15:56:06  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  18:28:20  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  15:08:44  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:09:09  melman
 * 	Changed __OSF1__ to __OSF__ in #if around #include <sys/select.h>
 * 	[1992/06/01  19:02:30  melman]
 * 
 * 	New GDS merged drop
 * 	[1992/05/29  12:30:24  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25evmgt.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 18:23:16 $";
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
/*   NAME:    d23evmgt.c                                                     */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       This module contains the function d25_evmgt () which represents     */
/*       the central Stub-event management for all events received at the    */
/*       transport interface (or at the high level ROS-interface) and for    */
/*       all events received at the IPC-interface. All events occurring      */
/*       at these interfaces are evaluated by interface specific event       */
/*       functions (d25_rosevent() resp. d25_ipcevent()) which are called    */
/*       by the central event management routine.                            */
/*                                                                           */
/*       The second function d25_exec_actions () contained in the module     */
/*       executes actions which are defined through the state/event table    */
/*       for any allowed state/event combination. The function is called     */
/*       by the central event management function at any time an event       */
/*       is detected and evaluated.                                          */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 3.6.91      */
/*datoff *********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#if defined(_AIX)
#include <sys/select.h>
#endif
#include <ros.h>
#include <cmx.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>
#include <d25stub.h>

/* ----------------- DECLARATION OF LOCAL DATA ---------------------------- */

static D23_ipcempb	empb ;	/* fd's relevant for IPC-events */
				/* 'IPC-event available'-flag */
static boolean		ipcev = FALSE ;
				/* 'fatal event error'-flag */
static boolean		selerr = FALSE ;

/* ----------------- PROTOTYPING OF LOCAL FUNCTIONS ----------------------- */

int d25_ev_select (int, fd_set *, fd_set *, fd_set *,
						     struct timeval *, void *) ;

/* ------------------ CENTRAL EVENT MANAGEMENT ----------------------------- */

signed32 d25_evmgt (D25_stubinfo *sinfptr) {

    D25_eventpb		evntpb ;
    R_chwtpb		cwpb ;
    R_evtpb		rosev ;
    signed32		res ;
    D25_actionpb	actionpb ;

    /* initialize event parameter block */
    evntpb.d25_esinforef = sinfptr ;

    /* initialize action parameter block */
    actionpb.d25_asinforef = sinfptr ;

    /* get IPC-event description information */
    empb.d23_Ehdinfo.d2_version = D23_V02 ;
    if (d23_gipc_eminfo (&empb) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
					     svc_c_sev_fatal, GDS_S_IPCEV_ERR) ;
	return (D25_ERROR) ;
    }

    /* announce call back-routine */
    (void) r_callback (d25_ev_select, (void *) NULL, (void *) NULL) ;

    for (;;) {
	/* provide receive buffer for ROS-events */
	if (d25_rosprbf (&actionpb) == D25_AERROR)
	    return (D25_ERROR) ;

	/* supply ROS-check and wait parameter block */
	cwpb.r_hdinfo.r_versnb = R_VERSION ;
	cwpb.r_hdinfo.r_fnctnb = R_CWAIT ;
	cwpb.r_wtime = R_INFIN ;
	cwpb.r_eflag = R_FALSE ;
	cwpb.r_evlen = sizeof (R_evtpb) ;
	cwpb.r_evntrf = &rosev ;

	/* execute  ROS-check and wait */
	if (r_cwait (&cwpb) != R_SUCES) {
	    if (cwpb.r_hdinfo.r_retcod == R_NOEVNT ||
					   cwpb.r_hdinfo.r_retcod == R_TIMOUT) {
		/* no event occurred */
		if (selerr)
		    return (D25_ERROR) ;

		/* evaluate type of received event */
		if (ipcev) {
		    /* get IPC-event */
		    ipcev = FALSE ;
		    if ((res = d25_ipcevent (&evntpb)) == D25_SUCES &&
					     evntpb.d25_eevent != D25_IRNO_EV) {
			/* execute state/event specific actions */
			if (d25_exec_actions (&evntpb) == D25_ERROR)
			    return (D25_ERROR) ;
		    }
		}
	    } else {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"),
			GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_ROSFUFAILED_ERR,
			cwpb.r_hdinfo.r_retcod, cwpb.r_hdinfo.r_erclas,
			cwpb.r_hdinfo.r_errval) ;
	        return (D25_ERROR) ;
	    }
	} else {
	    /* get ROS-event */
	    do {
		if ((res = d25_rosevent (&evntpb, &rosev)) != D25_ERROR &&
					     evntpb.d25_eevent != D25_IRNO_EV) {
		    /* execute state/event specific actions */
		    if (d25_exec_actions (&evntpb) == D25_ERROR)
			return (D25_ERROR) ;
		}
	    } while (res > 1) ;
	    if (res == D25_ERROR)
	        return (D25_ERROR) ;
	}
    }
}

/* ------------- EVENT MANAGEMENT CALL BACK FUNCTION ----------------------- */

int d25_ev_select (int fdsetsize, fd_set *rfds, fd_set *wfds, fd_set *xfds,
					struct timeval *timeout, void *usrpar) {

    signed32	i ;
    int		res, fsize = (((fdsetsize-1)/NFDBITS)+1)*sizeof (fd_mask) ;

    /* combine IPC- and TRANSPORT-event description information */
    for (i = 0; i < empb.d23_Enfds; i++)
	FD_SET(*(empb.d23_Eipcfds + i), rfds) ;
		
    /* wait for incoming IPC- resp. TRANSPORT-events */
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%b%b%b"), GDS_S_GENERAL,
     svc_c_debug4, GDS_S_SELEVINFO_IN, fsize, rfds, fsize, wfds, fsize, xfds)) ;
    if ((res = select (fdsetsize, rfds, wfds, xfds, timeout)) > 0) {
	/* evaluate type of received event */
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%b%b%b"), GDS_S_GENERAL,
		svc_c_debug4, GDS_S_SELEVINFO_OUT, fsize, rfds, fsize, wfds,
								 fsize, xfds)) ;
	for (i = 0; i < empb.d23_Enfds; i++)
	    if (FD_ISSET(*(empb.d23_Eipcfds + i), rfds))
		break ;
	if (i < empb.d23_Enfds) {
	    /* IPC-event occurred */
	    ipcev = TRUE ;
	    return (T_USEREVENT) ;
	} else
	    /* network-event occurred */
	    return (T_TSEVENT) ;
    } else {
	if (res < 0 && errno != EINTR) {
	    selerr = TRUE ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"),
		    GDS_S_GENERAL, svc_c_sev_warning, GDS_S_SELECT_ERR, errno) ;
	}
	return (T_NOEVENT) ;
    }
}


/* ------------- EXECUTE ACTIONS FROM THE STATE/EVENT-TABLE ---------------- */

signed32 d25_exec_actions (D25_eventpb *evntpb) {

    D25_stubinfo	*sinfptr = evntpb->d25_esinforef ;
    D25_actionpb	actpb ;
    D25_steventry	*stevptr ;
    signed32		state = evntpb->d25_estate ;
    signed32		event = evntpb->d25_eevent ;
    signed32		mx_action, suc_state, suc_flag ;
    signed32		no_stev, result, action_no ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d"), GDS_S_GENERAL,
    svc_c_debug1, GDS_S_EXECACT_ENTRY, evntpb->d25_estate, evntpb->d25_eevent,
    (evntpb->d25_erosref != (D25_rosentry *) NULL) ?
				   evntpb->d25_erosref->d25_rassid : D25_UNUSED,
    (evntpb->d25_eipcref != (D25_ipcentry *) NULL) ?
				evntpb->d25_eipcref->d25_iassid : D25_UNUSED)) ;

    /* search corresponding state/event entry */
    if ((stevptr = sinfptr->d25_stsubref[state - 1]) ==
						       (D25_steventry *) NULL) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
					  svc_c_sev_fatal, GDS_S_ILLSTATE_ERR) ;
	return (D25_ERROR) ;
    }
    for (no_stev = 0; D25_GST(stevptr->d25_stevinfo) == state; no_stev++,
								      stevptr++)
	if ((stevptr->d25_stevinfo & D25_EXTFLG) != D25_EXTFLG &&
					D25_GEV(stevptr->d25_stevinfo) == event)
	    break ;
    if (D25_GST(stevptr->d25_stevinfo) != state) {
	/* corresponding state/event entry not found */
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
		      svc_c_sev_fatal, GDS_S_ILLSTATE_EVENT_ERR, state, event) ;
#if defined UNDER_DEVELOPMENT
	return (D25_ERROR) ;
#else
	/* illegal events are ignored under normal conditions */
	return (D25_SUCES) ;
#endif
    }

    /* execute actions */
    mx_action = D25_G1NA(stevptr->d25_stevinfo) ;
    suc_state = D25_G1SU(stevptr->d25_stevinfo) ;
    suc_flag = 1 ;
    actpb.d25_asinforef = sinfptr ;
    actpb.d25_arosref = evntpb->d25_erosref ;
    actpb.d25_aipcref = evntpb->d25_eipcref ;
    actpb.d25_astate = state ;
    actpb.d25_aevent = event ;
    actpb.d25_aglopar = D25_GAPAR(stevptr->d25_stevinfo) ;
    actpb.d25_lpar.d25_alocpar = 0 ;
    for (action_no = 0; action_no < mx_action; ) {
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
	     svc_c_debug1, GDS_S_EXECACTION_INFO,
	     d25_cvfname(stevptr->d25_action[action_no]), no_stev, action_no)) ;
	result = stevptr->d25_action[action_no++] (&actpb) ;
	switch (result) {
	case D25_ACONT:
	    if (action_no == D25_MXACTION &&
			 (stevptr->d25_stevinfo & D25_CONTFLG) == D25_CONTFLG) {
		/* continuation flag set -> use extension entry */
		stevptr++ ; no_stev++ ;
		if (D25_G1SU(stevptr->d25_stevinfo) == suc_state) {
		    action_no = 0 ;
		    mx_action = D25_G1NA(stevptr->d25_stevinfo) ;
		    actpb.d25_aglopar = D25_GAPAR(stevptr->d25_stevinfo) ;
		}
	    }
	    break ;
	case D25_ASKIP:
	    /* skip next action entry */
	    if (++action_no >= D25_MXACTION &&
			 (stevptr->d25_stevinfo & D25_CONTFLG) == D25_CONTFLG) {
		/* continuation flag set -> use extension entry */
		stevptr++ ; no_stev++ ;
		if (D25_G1SU(stevptr->d25_stevinfo) == suc_state) {
		    action_no = (action_no > D25_MXACTION) ? 1 : 0 ;
		    mx_action = D25_G1NA(stevptr->d25_stevinfo) ;
		    actpb.d25_aglopar = D25_GAPAR(stevptr->d25_stevinfo) ;
		}
	    }
	    break ;
	case D25_ANSUC:
	    if ((action_no = mx_action) < D25_MXACTION && suc_flag == 1) {
		/* continue at the next successor state which is */
		/* described by the same state/event entry */
		mx_action += D25_G2NA(stevptr->d25_stevinfo) ;
		suc_state = D25_G2SU(stevptr->d25_stevinfo) ;
		suc_flag = 2 ;
	    } else {
		for (;;) {
		    /* continue at the next successor state which is */
		    /* described by an extension state/event entry */
		    stevptr++ ; no_stev++ ;
		    if (D25_G1SU(stevptr->d25_stevinfo) != suc_state) {
			/* successor state is described by the successor */
			/* state 1 field of the extension entry */
			action_no = 0 ;
			mx_action = D25_G1NA(stevptr->d25_stevinfo) ;
			suc_state = D25_G1SU(stevptr->d25_stevinfo) ;
			suc_flag = 1 ;
			break ;
		    } else {
			if ((action_no = D25_G1NA(stevptr->d25_stevinfo)) <
								 D25_MXACTION) {
			    /* successor state is described by the successor */
			    /* state 2 field of the extension entry */
			    mx_action = action_no +
					       D25_G2NA(stevptr->d25_stevinfo) ;
			    suc_state = D25_G2SU(stevptr->d25_stevinfo) ;
			    suc_flag = 2 ;
			    break ;
			}
		    }
		}
		actpb.d25_aglopar = D25_GAPAR(stevptr->d25_stevinfo) ;
	    }
	    break ;
	case D25_ATERM:
	    goto terminate ;
	case D25_AREXCPT:
	    evntpb->d25_eevent = D25_REXCPT_EV ;
	    return (d25_exec_actions (evntpb)) ;
	case D25_AIEXCPT:
	    evntpb->d25_eevent = D25_IEXCPT_EV ;
	    return (d25_exec_actions (evntpb)) ;
	case D25_AERROR:
	    return (D25_ERROR) ;
	default:
	    break ;
	}
    }

terminate:
    /* adjust successor state */
    if (actpb.d25_aipcref != (D25_ipcentry *) NULL)
	D25_CIOS(actpb.d25_aipcref->d25_istate, suc_state) ;

    return (D25_SUCES) ;
}
