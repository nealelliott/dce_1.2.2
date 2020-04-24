/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25iaction.c,v $
 * Revision 1.1.11.2  1996/02/18  18:23:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:13  marty]
 *
 * Revision 1.1.11.1  1995/12/08  16:15:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:20:50  root]
 * 
 * Revision 1.1.9.5  1994/09/06  12:26:27  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:24:36  keutel]
 * 
 * Revision 1.1.9.4  1994/07/06  15:08:17  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:20:54  marrek]
 * 
 * Revision 1.1.9.3  1994/05/10  16:03:27  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:12:31  marrek]
 * 
 * Revision 1.1.9.2  1994/03/23  15:47:23  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:33:34  keutel]
 * 
 * Revision 1.1.9.1  1994/02/22  17:55:17  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:51:42  marrek]
 * 
 * Revision 1.1.7.2  1993/08/11  08:26:25  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:19:20  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  22:03:56  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:47:54  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:09:20  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:33:01  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25iaction.c,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 18:23:17 $";
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
/*   NAME:    d25iaction.c                                                   */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       This module contains all actions (functions) belonging to the       */
/*       IPC-interface.                                                      */
/*       The following functions are available:                              */
/*                                                                           */
/*          * d25_ipcsvratt - Attach server application                      */
/*          * d25_ipsvrdet  - Detach server application                      */
/*          * d25_ipccltatt - Attach client application                      */
/*          * d25_ipcltdet  - Detach client application                      */
/*          * d25_ipcbind   - Bind to server                                 */
/*          * d25_ipcunbind - Unbind from server                             */
/*          * d25_ipcinvoke - Invoke request                                 */
/*          * d25_ipcwaitin - Wait for invoke                                */
/*          * d25_ipcresult - Result request                                 */
/*          * d25_ipwaitrs  - Wait for result                                */
/*          * d25_ipretbuf  - Return buffer                                  */
/*          * d25_ipcabort  - Abort request                                  */
/*          * d25_regproc   - Register process at IPC                        */
/*          * d25_unregproc - Remove registered process from IPC             */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 3.6.91      */
/*datoff *********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ros.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>
#include <d25stub.h>
#include <d23apdu.h>



/* --------------------- IPC - ATTACH SERVER ------------------------------- */

signed32 d25_ipcsvratt (D25_actionpb *actpb) {

    D23_atsvpb		atsvpb ;
    D25_stubinfo	*sinfptr = actpb->d25_asinforef ;

    /* supply IPC-attach server parameter block */
    atsvpb.d23_0hdinfo.d2_version = D23_V02 ;
#ifdef CSTUB
    atsvpb.d23_0svrid = D23_CSTSID ;
#else
    atsvpb.d23_0svrid = D23_SSTSID + sinfptr->d25_dirid ;
#endif
    atsvpb.d23_0evmode = D23_SEVM(actpb->d25_aglopar) | D23_SSTY(D23_SPROCSVR) ;

    /* execute IPC-attach server request */
    if (d23_attsvr (&atsvpb) == D23_ERROR)
	return (D25_AERROR) ;

    /* update Stub information table */
    sinfptr->d25_imbufsz = atsvpb.d23_0mslen ;

    return (D25_ACONT) ;
}

/* --------------------- IPC - DETACH SERVER ------------------------------- */

signed32 d25_ipsvrdet (D25_actionpb *actpb) {

    D23_desvpb		desvpb ;

    /* supply IPC-detach server parameter block */
    desvpb.d23_1hdinfo.d2_version = D23_V02 ;

    /* execute IPC-detach server request */
    if (d23_detsvr (&desvpb) == D23_ERROR)
	return (D25_AERROR) ;

    return (D25_ACONT) ;
}

/* --------------------- IPC - ATTACH CLIENT ------------------------------- */

signed32 d25_ipccltatt (D25_actionpb *actpb) {

    D23_atclpb		atclpb ;

    /* supply IPC-attach client parameter block */
    atclpb.d23_9hdinfo.d2_version = D23_V02 ;
    atclpb.d23_9evmode = D23_SEVM(actpb->d25_aglopar) ;

    /* execute IPC-attach client request */
    if (d23_attclt (&atclpb) == D23_ERROR)
	return (D25_AERROR) ;

    return (D25_ACONT) ;
}

/* --------------------- IPC - DETACH CLIENT ------------------------------- */

signed32 d25_ipcltdet (D25_actionpb *actpb) {

    D23_declpb		declpb ;

    /* supply IPC-detach client parameter block */
    declpb.d23_Ahdinfo.d2_version = D23_V02 ;

    /* execute IPC-detach client request */
    if (d23_detclt (&declpb) == D23_ERROR)
	return (D25_AERROR) ;

    return (D25_ACONT) ;
}

/* --------------------- IPC - BIND SERVER --------------------------------- */

signed32 d25_ipcbind (D25_actionpb *actpb) {

    D23_bindpb		bndpb ;
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;
    D25_stubinfo	*sinfptr = actpb->d25_asinforef ;

    /* supply IPC-bind parameter block */
    bndpb.d23_4hdinfo.d2_version = D23_V02 ;
    bndpb.d23_4svrid = (actpb->d25_astate != D25_IRASSOCIATION &&
			actpb->d25_astate != D25_IRIDLE) ? D23_DCASID :
					       D23_DS0SID + sinfptr->d25_dirid ;

    /* execute IPC-bind request */
    for (;;) {
	if (d23_bind (&bndpb) == D23_ERROR) {
	    if (bndpb.d23_4hdinfo.d2_errvalue == D23_NOSERV) {
		if (actpb->d25_astate == D25_IRINIT) {
		    /* during initialization -> retry IPC-bind request */
		    sleep (D25_RETRYTIME1) ;
		    continue ;
		} else {
		    return ((actpb->d25_astate == D25_IRIDLE) ? D25_ANSUC :
								D25_AERROR) ;
		}
	    } else
		return (D25_AERROR) ;
	}	
	break ;
    }

    /* update IPC-table entry */
    ipcptr->d25_iassid = bndpb.d23_4assid ;
    ipcptr->d25_ibufsz = bndpb.d23_4lenms ;
    ipcptr->d25_ibufref = bndpb.d23_4refms ; 

    /* update lowest/highest IPC-buffer references */
    if (sinfptr->d25_imnref == (byte *) NULL || bndpb.d23_4refms <
							    sinfptr->d25_imnref)
	sinfptr->d25_imnref = bndpb.d23_4refms ;

    if (bndpb.d23_4refms > sinfptr->d25_imxref)
	sinfptr->d25_imxref = bndpb.d23_4refms ;

    return (D25_ACONT) ;
}

/* --------------------- IPC - UNBIND SERVER ------------------------------- */

signed32 d25_ipcunbind (D25_actionpb *actpb) {

    D23_ubndpb		ubndpb ;
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;

    /* supply IPC-unbind parameter block */
    ubndpb.d23_5hdinfo.d2_version = D23_V02 ;
    ubndpb.d23_5assid = ipcptr->d25_iassid ;

    /* execute IPC-unbind request */
    if (d23_unbind (&ubndpb) == D23_ERROR)
	return (D25_AERROR) ;

    return (D25_ACONT) ;
}

/* --------------------- IPC - INVOKE REQUEST ------------------------------ */

signed32 d25_ipcinvoke (D25_actionpb *actpb) {

    signed32		sndlen ;
    D23_inrqpb		inrqpb ;
    D25_stubinfo	*sinfptr = actpb->d25_asinforef ;
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;
    D25_rosentry	*rosptr = actpb->d25_arosref ;

    /* supply IPC-invoke parameter block */ 	
    inrqpb.d23_6hdinfo.d2_version = D23_V02 ;
    inrqpb.d23_6assid = ipcptr->d25_iassid ;
#ifdef CSTUB
    if (actpb->d25_astate == D25_IRINIT ||
				    actpb->d25_astate == D25_ICINVOKE_PENDING) {
#else
    if (actpb->d25_astate == D25_IRINIT) {
#endif
	inrqpb.d23_6invinfo.d23_contid =
			D23_SAC(D27_DAP_AC) | D23_SAS(D27_DAP_AS) |
			D23_STS(D27_PRIV_TS) | D23_SACM(D27_LOC_AC) ;
	inrqpb.d23_6invinfo.d23_usrid = ipcptr->d25_iassid ;
    } else { 
	inrqpb.d23_6invinfo.d23_contid =
			D23_SAC(rosptr->d25_rcontid) |
			D23_SAS(rosptr->d25_rabsid) |
			D23_STS(rosptr->d25_rtransid) |
			D23_SACM(D27_REM_AC) |
			D23_SRMID(D23_SSTSID + sinfptr->d25_dirid) ;
	inrqpb.d23_6invinfo.d23_usrid = rosptr->d25_rassid ;
    }
    inrqpb.d23_6invinfo.d23_invid = ipcptr->d25_invinfo.d23_invid ;
    inrqpb.d23_6oprid = ipcptr->d25_ioprid ;
    inrqpb.d23_6rfidt = ipcptr->d25_ibufref ;
    inrqpb.d23_6imdat = D23_LASTFG ;
    sndlen = ipcptr->d25_iudat.d25_datlen - ipcptr->d25_iudat.d25_datoff ;

    if (ipcptr->d25_iudat.d25_datref != ipcptr->d25_ibufref) {
	/* write invoke data into IPC-buffer */
	if (sndlen > ipcptr->d25_ibufsz) {
	    sndlen = ipcptr->d25_ibufsz ;
	    inrqpb.d23_6imdat = D23_MRDATA ;
	}
	(void) memcpy ((void *) ipcptr->d25_ibufref,
	(void *) (ipcptr->d25_iudat.d25_datref + ipcptr->d25_iudat.d25_datoff),
							      (size_t) sndlen) ;
    }
    inrqpb.d23_6lnidt = (signed16) sndlen ;
    ipcptr->d25_iudat.d25_datoff += sndlen ;

    /* execute IPC-invoke request */
    if (d23_invoke (&inrqpb) == D23_ERROR)
	return (D25_AERROR) ;

    if (ipcptr->d25_iudat.d25_datref != (byte *) NULL &&
	ipcptr->d25_iudat.d25_datref != ipcptr->d25_ibufref &&
		 ipcptr->d25_iudat.d25_datoff == ipcptr->d25_iudat.d25_datlen) {
	/* release user data buffer */
	free ((void *) ipcptr->d25_iudat.d25_datref) ;
        DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
              svc_c_debug1, GDS_S_REL_SRBUFFER, ipcptr->d25_iudat.d25_datref)) ;
    }
    return (D25_ACONT) ;
}

/* --------------------- IPC - WAIT FOR INVOKE ----------------------------- */

signed32 d25_ipcwaitin (D25_actionpb *actpb) {

    byte		*rcvbuf ;
    D23_wtinpb		wtinpb ;
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;

    /* supply IPC-wait for invoke parameter block */ 	
    wtinpb.d23_2hdinfo.d2_version = D23_V02 ;
    wtinpb.d23_2wtime = D23_INFIN ;

    for (;;) {
	/* execute IPC-wait for invoke request */
	if (d23_waitin (&wtinpb) == D23_ERROR)
	    return (D25_AERROR) ;

	switch (wtinpb.d23_2oprid) {
	case D23_NOOPR:
#if defined THREADSAFE
	    pthread_yield () ;
#endif
	    break ;
	case D23_ABORT:
            DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			   svc_c_debug1, GDS_S_INVRQ_ABRT, wtinpb.d23_2assid)) ;
	    return (D25_AIEXCPT) ;
	default:
	    if (ipcptr->d25_iudat.d25_datlen > 0 || wtinpb.d23_2imdat ==
								   D23_MRDATA) {
		/* remove invoke data from IPC-buffer */
		if ((rcvbuf = (ipcptr->d25_iudat.d25_datlen == 0) ?
		    (byte *) malloc ((size_t) wtinpb.d23_2lnidt) :
		    (byte *) realloc ((void *) ipcptr->d25_iudat.d25_datref,
			     (size_t) (ipcptr->d25_iudat.d25_datlen +
					wtinpb.d23_2lnidt))) == (byte *) NULL) {
		    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			svc_c_sev_warning, GDS_S_NOMEMORY,
			(ipcptr->d25_iudat.d25_datlen == 0) ? wtinpb.d23_2lnidt :
			ipcptr->d25_iudat.d25_datlen + wtinpb.d23_2lnidt) ;
		    if (ipcptr->d25_iudat.d25_datlen > 0) {
			free ((void *) ipcptr->d25_iudat.d25_datref) ;
			DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"),
			GDS_S_GENERAL, svc_c_debug1, GDS_S_REL_SRBUFFER,
						ipcptr->d25_iudat.d25_datref)) ;
		    }
		    return (D25_AIEXCPT) ;
		}
		if (ipcptr->d25_iudat.d25_datlen > 0) {
		    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d"),
		    GDS_S_GENERAL, svc_c_debug1, GDS_S_ALO_SRBUFFER,
                    ipcptr->d25_iudat.d25_datref, rcvbuf,
			    ipcptr->d25_iudat.d25_datlen + wtinpb.d23_2lnidt)) ;
		} else {
		    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"),
		    GDS_S_GENERAL, svc_c_debug1, GDS_S_REALO_SRBUFFER,
                				   rcvbuf, wtinpb.d23_2lnidt)) ;
		}
		ipcptr->d25_iudat.d25_datref = rcvbuf ;
		(void) memcpy ((void *) (ipcptr->d25_iudat.d25_datref +
		ipcptr->d25_iudat.d25_datlen), (void *) ipcptr->d25_ibufref,
						   (size_t) wtinpb.d23_2lnidt) ;
	    } else
		ipcptr->d25_iudat.d25_datref = ipcptr->d25_ibufref ;
	    ipcptr->d25_iudat.d25_datlen += wtinpb.d23_2lnidt ;

	    if (wtinpb.d23_2imdat == D23_LASTFG)
		return (D25_ACONT) ;
	    break ;
	}	
    }
}

/* --------------------- IPC - RESULT REQUEST ------------------------------ */

signed32 d25_ipcresult (D25_actionpb *actpb) {

    signed32		sndlen ;
    D23_rsrqpb		rsrqpb ;
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;
    D25_rosentry	*rosptr = actpb->d25_arosref ;

    /* supply IPC-result parameter block */ 	
    rsrqpb.d23_3hdinfo.d2_version = D23_V02 ;
    rsrqpb.d23_3assid = ipcptr->d25_iassid ;
    rsrqpb.d23_3invinfo = ipcptr->d25_invinfo ;
    rsrqpb.d23_3invinfo.d23_contid =
			D23_SAC(rosptr->d25_rcontid) |
			D23_SAS(rosptr->d25_rabsid) |
			D23_STS(rosptr->d25_rtransid) |
			D23_SACM(D27_LOC_AC) ;
    rsrqpb.d23_3rltid = ipcptr->d25_iresid ;
    rsrqpb.d23_3rfrdt = ipcptr->d25_ibufref ;
    rsrqpb.d23_3rmdat = D23_LASTFG ;
    sndlen = ipcptr->d25_iudat.d25_datlen - ipcptr->d25_iudat.d25_datoff ;

    if (ipcptr->d25_iudat.d25_datref != ipcptr->d25_ibufref) {
	/* write result data into IPC-buffer */
	if (sndlen > ipcptr->d25_ibufsz) {
	    sndlen = ipcptr->d25_ibufsz ;
	    rsrqpb.d23_3rmdat = D23_MRDATA ;
	}
	(void) memcpy ((void *) ipcptr->d25_ibufref,
	(void *) (ipcptr->d25_iudat.d25_datref + ipcptr->d25_iudat.d25_datoff),
							      (size_t) sndlen) ;
    }
    rsrqpb.d23_3lnrdt = (signed16) sndlen ;
    ipcptr->d25_iudat.d25_datoff += sndlen ;

    /* execute IPC-result request */
    if (d23_result (&rsrqpb) == D23_ERROR)
	return (D25_AERROR) ;

    if (ipcptr->d25_iudat.d25_datref != (byte *) NULL &&
	ipcptr->d25_iudat.d25_datref != ipcptr->d25_ibufref &&
		 ipcptr->d25_iudat.d25_datoff == ipcptr->d25_iudat.d25_datlen) {
	/* release result buffer */
	free ((void *) ipcptr->d25_iudat.d25_datref) ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
	      svc_c_debug1, GDS_S_REL_SRBUFFER, ipcptr->d25_iudat.d25_datref)) ;
	ipcptr->d25_iudat.d25_datref = (byte *) NULL ;
    }
    return (D25_ACONT) ;
}

/* --------------------- IPC - WAIT FOR RESULT ----------------------------- */

signed32 d25_ipwaitrs (D25_actionpb *actpb) {

    byte		*rcvbuf ;
    D23_wtrspb		wtrspb ;
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;

    /* supply IPC-wait for result parameter block */ 	
    wtrspb.d23_7hdinfo.d2_version = D23_V02 ;
    wtrspb.d23_7wtime = D23_INFIN ;

    for (;;) {
	/* execute IPC-wait for result request */
	if (d23_waitrs (&wtrspb) == D23_ERROR)
	    return (D25_AERROR) ;

	/* evaluate result type */
	switch (wtrspb.d23_7rltid) {
	case D23_NOOPR:
	    break ;
	case D23_ABORT:
	   DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
                           svc_c_debug1, GDS_S_RESRQ_ABRT, wtrspb.d23_7assid)) ;
	    return (D25_AIEXCPT) ;
	case D23_RESULT:
	    if (ipcptr->d25_iudat.d25_datlen > 0 || wtrspb.d23_7rmdat ==
								   D23_MRDATA) {
		/* remove result data from IPC-buffer */
		if ((rcvbuf = (ipcptr->d25_iudat.d25_datlen == 0) ?
		    (byte *) malloc ((size_t) wtrspb.d23_7lnrdt) :
		    (byte *) realloc ((void *) ipcptr->d25_iudat.d25_datref,
			     (size_t) (ipcptr->d25_iudat.d25_datlen +
					wtrspb.d23_7lnrdt))) == (byte *) NULL) {
		    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			svc_c_sev_warning, GDS_S_NOMEMORY,
			(ipcptr->d25_iudat.d25_datlen == 0) ? wtrspb.d23_7lnrdt :
			ipcptr->d25_iudat.d25_datlen + wtrspb.d23_7lnrdt) ;
		    if (ipcptr->d25_iudat.d25_datlen > 0) {
			free ((void *) ipcptr->d25_iudat.d25_datref) ;
			DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"),
			GDS_S_GENERAL, svc_c_debug1, GDS_S_REL_SRBUFFER,
						ipcptr->d25_iudat.d25_datref)) ;
		    }
		    return (D25_AIEXCPT) ;
		}
		if (ipcptr->d25_iudat.d25_datlen > 0) {
		    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d"),
		    GDS_S_GENERAL, svc_c_debug1, GDS_S_REALO_SRBUFFER,
                    ipcptr->d25_iudat.d25_datref, rcvbuf,
			    ipcptr->d25_iudat.d25_datlen + wtrspb.d23_7lnrdt)) ;
		} else {
		    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"),
		    GDS_S_GENERAL, svc_c_debug1, GDS_S_ALO_SRBUFFER,
                				   rcvbuf, wtrspb.d23_7lnrdt)) ;
		}
		ipcptr->d25_iudat.d25_datref = rcvbuf ;
		(void) memcpy ((void *) (ipcptr->d25_iudat.d25_datref +
		ipcptr->d25_iudat.d25_datlen), (void *) ipcptr->d25_ibufref,
						   (size_t) wtrspb.d23_7lnrdt) ;
	    } else {
		ipcptr->d25_iresid = wtrspb.d23_7rltid ;
		ipcptr->d25_iudat.d25_datref = ipcptr->d25_ibufref ;
	    }
	    ipcptr->d25_iudat.d25_datlen += wtrspb.d23_7lnrdt ;

	    if (wtrspb.d23_7rmdat == D23_LASTFG)
		return (D25_ACONT) ;
	    break ;
	default:
	    /* result is D23_RERROR resp. D23_RREJECT */
	    ipcptr->d25_iresid = wtrspb.d23_7rltid ;
	    if (wtrspb.d23_7rltid >= D23_RERROR && wtrspb.d23_7rltid <=
						      D23_RERROR + D23_MXRERR) {
		ipcptr->d25_iudat.d25_datref = ipcptr->d25_ibufref ;
		ipcptr->d25_iudat.d25_datlen = wtrspb.d23_7lnrdt ;
	    }
	    return (D25_ACONT) ;
	}	
    }
}

/* --------------------- IPC - RETURN BUFFER ------------------------------- */

signed32 d25_ipretbuf (D25_actionpb *actpb) {

    D23_rbufpb		rbufpb ;
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;

    /* supply IPC-return buffer parameter block */ 	
    rbufpb.d23_Chdinfo.d2_version = D23_V02 ;
    rbufpb.d23_Cassid = ipcptr->d25_iassid ;

    /* execute IPC-return buffer */
    if (d23_retbuf (&rbufpb) == D23_ERROR)
	return (D25_AERROR) ;

    return (D25_ACONT) ;
}

/* --------------------- IPC - ABORT REQUEST ------------------------------- */

signed32 d25_ipcabort (D25_actionpb *actpb) {

    D23_abortpb		abopb ;
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;
#ifndef CSTUB
    D25_rosentry	*rosptr = actpb->d25_arosref ;
    D25_stubinfo	*sinfptr = actpb->d25_asinforef ;
#endif

    /* supply IPC-abort request parameter block */ 	
    abopb.d23_Fhdinfo.d2_version = D23_V02 ;
    abopb.d23_Fassid = ipcptr->d25_iassid ;
    abopb.d23_Finvinfo = ipcptr->d25_invinfo ;
#ifdef CSTUB
    abopb.d23_Finvinfo.d23_contid =
		D23_SAC(D27_DAP_AC) | D23_SAS(D27_DAP_AS) |
		D23_STS(D27_PRIV_TS) | D23_SACM(D27_LOC_AC) ;
/*
    abopb.d23_Finvinfo.d23_usrid = ipcptr->d25_iassid ;
*/
#else
    abopb.d23_Finvinfo.d23_contid =
		D23_SAC(rosptr->d25_rcontid) |
		D23_SAS(rosptr->d25_rabsid) |
		D23_STS(rosptr->d25_rtransid) |
		D23_SACM(D27_REM_AC) |
		D23_SRMID(D23_SSTSID + sinfptr->d25_dirid) ;
/*
    abopb.d23_Finvinfo.d23_usrid = rosptr->d25_rassid ;
*/
#endif 

    /* execute IPC-abort request */
    if (d23_abort (&abopb) == D23_ERROR)
	return (D25_AERROR) ;

    return (D25_ACONT) ;
}

/* --------------------- REGISTER PROCESS AT IPC --------------------------- */

signed32 d25_regproc (D25_actionpb *actpb) {

    D25_stubinfo	*sinfptr = actpb->d25_asinforef ;
    D23_regprocpb	regpb ;
    D23_pstatpb		pstpb ;
    char		procstate[4] ;

    /* register process at IPC */
    regpb.d23_Ihdinfo.d2_version = D23_V02 ;
    regpb.d23_Idirid = sinfptr->d25_dirid ;
    regpb.d23_Iotype = TRUE ;
#ifdef CSTUB
    regpb.d23_Iptype = D23_CSTSID ;
    if (d23_reg_process (&regpb) == D23_ERROR) {
#else
    regpb.d23_Iptype = D23_SSTSID + sinfptr->d25_dirid ;
    if (d23_reg_process (&regpb) == D23_ERROR) {
#endif
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
					       svc_c_sev_fatal, GDS_S_REG_ERR) ;
	return (D25_AERROR) ;
    }

    pstpb.d23_Phdinfo.d2_version = D23_V02 ;
    pstpb.d23_Passid = D23_UNUSED ;
    pstpb.d23_Ppstate = procstate ;
    sprintf (procstate, "%c1", D23_WSTATE) ;
    if (d23_set_pstate (&pstpb) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
					       svc_c_sev_fatal, GDS_S_REG_ERR) ;
	return (D25_AERROR) ;
    }
    return (D25_ACONT) ;
}

/* ------------------REMOVE REGISTERED PROCESS FROM IPC -------------------- */

signed32 d25_unregproc (D25_actionpb *actpb) {

    D25_stubinfo	*sinfptr = actpb->d25_asinforef ;
    D23_regprocpb	regpb ;

    /* remove registered process from IPC */
    regpb.d23_Ihdinfo.d2_version = D23_V02 ;
    regpb.d23_Idirid = sinfptr->d25_dirid ;
    regpb.d23_Iotype = FALSE ;
#ifdef CSTUB
    regpb.d23_Iptype = D23_CSTSID ;
    if (d23_reg_process (&regpb) == D23_ERROR) {
#else
    regpb.d23_Iptype = D23_SSTSID + sinfptr->d25_dirid ;
    if (d23_reg_process (&regpb) == D23_ERROR) {
#endif
	return (D25_AERROR) ;
    }
    return (D25_ACONT) ;
}
