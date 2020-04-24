/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25uaction.c,v $
 * Revision 1.1.12.2  1996/02/18  18:23:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:29  marty]
 *
 * Revision 1.1.12.1  1995/12/08  16:15:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:06  root]
 * 
 * Revision 1.1.10.4  1994/09/06  12:26:34  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:25:11  keutel]
 * 
 * Revision 1.1.10.3  1994/07/06  15:08:19  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:21:07  marrek]
 * 
 * Revision 1.1.10.2  1994/05/10  16:03:33  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:13:00  marrek]
 * 
 * Revision 1.1.10.1  1994/02/22  17:55:33  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:52:59  marrek]
 * 
 * Revision 1.1.8.2  1993/11/09  15:00:06  keutel
 * 	modified logging message
 * 	[1993/11/09  14:11:10  keutel]
 * 
 * Revision 1.1.8.1  1993/10/14  19:19:08  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:16:21  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/09  18:01:12  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  08:33:31  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:20:57  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  22:04:39  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:48:25  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:10:32  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:33:57  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25uaction.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 18:23:30 $";
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
/*   NAME:    d25uaction.c                                                   */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       This module contains all actions (functions) belonging to the       */
/*       additional Stub initialization and to internally used resources     */
/*       (e.g. ROS-table or IPC-table).                                      */
/*       The following functions are available:                              */
/*                                                                           */
/*          * d25_getpsap   - Get own PSAP-address from DUA-cache            */
/*          * d25_getres    - Get internally used resources                  */
/*          * d25_relres    - Release internally used resources              */
/*          * d25_relbuf    - Release send/receive buffer                    */
/*          * d25_checkres  - Check internally used resources                */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 3.6.91      */
/*datoff *********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ros.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d25stub.h>


/* ------------------- DECLARATION OF LOCAL DATA --------------------------- */

				/* requested attribute types from DUA-cache */
static char	*rqattr[] = {
#ifdef CSTUB
		D2_PSAP_ADDRESS } ;
#else
		D2_SEE_ALSO, D2_PSAP_ADDRESS } ;
#endif

				/* size of requested attribute types */
static signed32	rqattsz[] = {
#ifdef CSTUB
		sizeof (D2_PSAP_ADDRESS) - 1 } ;
#else
		sizeof (D2_SEE_ALSO) - 1, sizeof (D2_PSAP_ADDRESS) - 1 } ;
#endif

/* --------------------- GET PSAP-ADDRESS ---------------------------------- */

signed32 d25_getpsap (D25_actionpb *actpb) {

    D25_stubinfo	*sinfptr = actpb->d25_asinforef ;
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;
    signed32		res, attsize, norq = 0 ;
    byte		*p, attval[D2_DNL_MAX] ;

    /* get own PSAP-address from DUA-cache */

    ipcptr->d25_ioprid = D23_READ ;
    ipcptr->d25_iudat.d25_datref = ipcptr->d25_ibufref ;
#ifdef CSTUB
    strcpy ((char *) attval, D2_N_CLIENT) ;
#else
    strcpy ((char *) attval, D2_N_LOC_DSA) ;
#endif

    for (;;) {
	/* --- read local DSA name resp. PSAP-address from DUA-cache --- */
	/* build request message */
	p = ipcptr->d25_ibufref ;
	((D23_inread *) p)->d23_Ehdr.d23_versno = D23_APDUV22 ;
	((D23_inread *) p)->d23_Ehdr.d23_dirid = sinfptr->d25_dirid ;
	((D23_inread *) p)->d23_Ehdr.d23_bindid = 0 ;
	p += D23_S_INR ;

	((D23_com_arg *) p)->d23_cmtag = D23_CMTAG ;
	((D23_com_arg *) p)->d23_cmali = 0 ;
	((D23_com_arg *) p)->d23_cmctr.d23_scopt =
		     D2_NOCHAINING | D2_NOREFERRAL | D2_USEDSA | D2_DONT_STORE ;
	((D23_com_arg *) p)->d23_cmctr.d23_scref = D2_WORLD_SCOPE ;
	((D23_com_arg *) p)->d23_cmctr.d23_scpri = D2_PRIO_MEDIUM ;
	((D23_com_arg *) p)->d23_cmctr.d23_sctlm = D2_T_UNLIMITED ;
	((D23_com_arg *) p)->d23_cmctr.d23_scslm = D2_S_UNLIMITED ;
	((D23_com_arg *) p)->d23_cmopr.d23_opnm_rs = D2_NOT_STARTED ;
	((D23_com_arg *) p)->d23_cmopr.d23_op_nxtrdn = 0 ;
	p += D23_S_CARG ;

	((D23_ds_name *) p)->d23_dstag = D23_NAMTAG ;
	((D23_ds_name *) p)->d23_dslen = strlen ((char *) attval) + 1 ;
	((D23_ds_name *) p)->d23_dsoff =
				      D2_ALIGN(((D23_ds_name *) p)->d23_dslen) ;
	strcpy ((char *) (((D23_ds_name *) p) + 1), (char *) attval) ;
	p += D23_S_NM + ((D23_ds_name *) p)->d23_dsoff ;

	((D23_rq_info *) p)->d23_rqtag = D23_REQTAG ;
	((D23_rq_info *) p)->d23_rqreq = D2_R_VAL_TYP ;
	((D23_rq_info *) p)->d23_rqnbr = 1 ;
	p += D23_S_REQ ;

	((D23_at_type *) p)->d23_atttag = D23_ATTTAG ;
	((D23_at_type *) p)->d23_atlen = rqattsz[norq] ;
	((D23_at_type *) p)->d23_atoff = D2_ALIGN (rqattsz[norq]) ;
	(void) memcpy ((void *) (((D23_at_type *) p) + 1),
				(void *) rqattr[norq], (size_t) rqattsz[norq]) ;
	p += D23_S_ATT + ((D23_at_type *) p)->d23_atoff ;

	((D23_eom *) p)->d23_eomtag = D23_EOMTAG ;
	p += D23_S_EOM ;

	ipcptr->d25_iudat.d25_datlen = p - ipcptr->d25_ibufref ;
	ipcptr->d25_iudat.d25_datoff = 0 ;
	ipcptr->d25_invinfo.d23_invid = D23_UNUSED ;

	/* send invoke request to DUA-Cache */
	if ((res = d25_ipcinvoke (actpb)) != D25_ACONT)
	    return (res) ;

	/* get result from DUA-Cache */
	ipcptr->d25_iudat.d25_datlen = 0 ;
	if ((res = d25_ipwaitrs (actpb)) != D25_ACONT)
	    return (res) ;

	if (ipcptr->d25_iresid >= D23_RERROR && ipcptr->d25_iresid <=
						      D23_RERROR + D23_MXRERR) {
	    if (((D23_rserror *) ipcptr->d25_iudat.d25_datref)->d23_Zerrval ==
								    D2_NFOUND) {
		sleep (D25_RETRYTIME) ;
#if defined THREADSAFE
		pthread_yield () ;
#endif
		continue ;
	    }
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
                svc_c_sev_fatal, GDS_S_CREAD_ERR,
		((D23_rserror *) ipcptr->d25_iudat.d25_datref)->d23_Zretcod,
		((D23_rserror *) ipcptr->d25_iudat.d25_datref)->d23_Zerrcls,
		((D23_rserror *) ipcptr->d25_iudat.d25_datref)->d23_Zerrval) ;
	    return (D25_AERROR) ;
	}
	/* evaluate result message */
	p = ipcptr->d25_ibufref ;
	if (((D23_en_info *) p)->d23_entag != D23_ENTTAG) {
	    /* invalid tag detected */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
            		      svc_c_sev_fatal, GDS_S_CREADMSG_ERR, D23_ENTTAG) ;
	    return (D25_AERROR) ;
	}
	p += D23_S_EN ;

	if (((D23_ds_name *) p)->d23_dstag != D23_NAMTAG) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
            		      svc_c_sev_fatal, GDS_S_CREADMSG_ERR, D23_NAMTAG) ;
	    return (D25_AERROR) ;
	}
	p += D23_S_NM + ((D23_ds_name *) p)->d23_dsoff ;

	if (((D23_at_type *) p)->d23_atttag != D23_ATTTAG) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
            		      svc_c_sev_fatal, GDS_S_CREADMSG_ERR, D23_ATTTAG) ;
	    return (D25_AERROR) ;
	}
	p += D23_S_ATT + ((D23_at_type *) p)->d23_atoff ; 

	if (((D23_av_info *) p)->d23_avtag != D23_AVTAG) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
            		       svc_c_sev_fatal, GDS_S_CREADMSG_ERR, D23_AVTAG) ;
	    return (D25_AERROR) ;
	}
	p += D23_S_AV ;
	attsize = ((D23_av_value *) p)->d23_avlen ;

	(void) memcpy ((void *) attval, (void *) (p + D23_S_ATV),
							     (size_t) attsize) ;

	if (++norq == sizeof (rqattsz)/sizeof (signed32))
	    break ;
    }

    /* allocate memory and transfer PSAP-address */
    if ((sinfptr->d25_spsapref =
			 (byte *) malloc ((size_t) attsize)) == (byte *) NULL) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
				     svc_c_sev_fatal, GDS_S_NOMEMORY, attsize) ;
	return (D25_AERROR) ;
    }
    memcpy ((void *) sinfptr->d25_spsapref, (void *) attval, (size_t) attsize) ;
    sinfptr->d25_spsaplen = attsize ;

    return (D25_ACONT) ;
}

/* --------------------- GET ROS-/IPC-TABLE RESOURCES ---------------------- */

signed32 d25_getres (D25_actionpb *actpb) {

    signed32		i, i1, j, k, rqpsaplen, rqudatlen, retval ;
    unsigned32		ipcstate ;
    byte		*rqpsapptr, *rqudatptr ;
    D25_stubinfo	*sinfptr = actpb->d25_asinforef ;
    D25_ipcentry	*ipcptr, *ipchlpptr, *ipchlp1ptr ;
    D25_rosentry	*rosptr ;
    
    
    if (D25_GITY(actpb->d25_aipcref->d25_istate) == D25_DPASSIVE) {
	/* get internally used resources for outgoing DAP/DSP */
	/* verify requested application context */
	if (actpb->d25_arosref->d25_rcontid != D27_DAP_AC &&
				actpb->d25_arosref->d25_rcontid != D27_DSP_AC) {
	    /* unknown application context */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
					svc_c_sev_warning, GDS_S_ACONTEXT_ERR,
					actpb->d25_arosref->d25_rcontid) ;
	    return (D25_AIEXCPT) ;
	}

	/* search empty IPC-table entry */
	ipcptr = ipchlp1ptr = (D25_ipcentry *) NULL ;
	for (i1 = 0, ipchlpptr = sinfptr->d25_itabref;
			i1 < sinfptr->d25_imaxentries; i1++, ipchlpptr++) {
	    if ((ipcstate = D25_GIOS(ipchlpptr->d25_istate)) == D25_IRIDLE) {
		/* save reference to empty entry */
		if (ipcptr == (D25_ipcentry *) NULL) {
		    ipcptr = ipchlpptr ;
		    i = i1 ;
		}
	    } else {
		if (ipchlpptr->d25_invinfo.d23_usrid ==
				    actpb->d25_aipcref->d25_invinfo.d23_usrid &&
		    (ipcstate == D25_IRASSOCIATION ||
					     ipcstate == D25_IINVOKE_PENDING)) {
		    /* ROS-association already exists for this user */
		    if (ipchlp1ptr == (D25_ipcentry *) NULL)
	    		ipchlp1ptr = ipchlpptr ;
		}
	    }
	    if (ipcptr != (D25_ipcentry *) NULL &&
					    ipchlp1ptr != (D25_ipcentry *) NULL)
		break ;
	}
	if (ipcptr == (D25_ipcentry *) NULL) {
	    /* IPC-table overflow */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
					svc_c_sev_warning, GDS_S_IOVERFLOW_ERR,
					sinfptr->d25_imaxentries) ;
	    return (D25_AIEXCPT) ;
	}

	if (ipchlp1ptr != (D25_ipcentry *) NULL) {
	    /* occupy IPC-table entry for parallel IPC-operation */
	    *ipcptr = *actpb->d25_aipcref ;
	    ipcptr->d25_istate |= D25_SPTY(D25_IPCPOPR) ;
	    ipcptr->d25_irosref = ipchlp1ptr->d25_irosref ;
	    sinfptr->d25_iactentries++ ;
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
		     svc_c_debug1, GDS_S_OCC_IPCENTRY, i, ipcptr->d25_iassid)) ;

	    /* update lowest/highest IPC-buffer references */
	    if (sinfptr->d25_imnref == (byte *) NULL ||
				      ipcptr->d25_ibufref < sinfptr->d25_imnref)
		sinfptr->d25_imnref = ipcptr->d25_ibufref ;

	    if (ipcptr->d25_ibufref > sinfptr->d25_imxref)
		sinfptr->d25_imxref = ipcptr->d25_ibufref ;

	    /* update ROS-table entry and action parameter block */
	    ipcptr->d25_irosref->d25_rrefcnt++ ;
	    actpb->d25_aipcref = ipcptr ;
	    actpb->d25_arosref = ipcptr->d25_irosref ;
	    return (D25_ACONT) ;
	}

	/* evaluate called PSAP-address and bind request user data */
	rqpsapptr = actpb->d25_aipcref->d25_iudat.d25_datref ;
	rqpsaplen = ((D23_av_value *) rqpsapptr)->d23_avlen ;
	rqudatptr = rqpsapptr + ((D23_av_value *) rqpsapptr)->d23_avoff +
								     D23_S_ATV ;
	rqpsapptr += D23_S_ATV ;
	rqudatlen = (signed32) ((actpb->d25_aipcref->d25_iudat.d25_datref +
			actpb->d25_aipcref->d25_iudat.d25_datlen) - rqudatptr) ;

	/* search corresponding ROS-table entry */
	for (j = 0, rosptr = sinfptr->d25_rtabref; j < sinfptr->d25_rmaxentries;
								  j++, rosptr++)
	    if (D25_GRES(rosptr->d25_rstate) == D25_RBUSY &&
		D25_GRIF(rosptr->d25_rstate) == D25_RINITIATOR &&
		rosptr->d25_rcontid == actpb->d25_arosref->d25_rcontid &&
		rosptr->d25_rpsaplen == rqpsaplen &&
		memcmp (rosptr->d25_rpsapref, rqpsapptr, rqpsaplen) == 0 &&
		rosptr->d25_rbndrqlen == rqudatlen &&
		memcmp (rosptr->d25_rbndrqref, rqudatptr, rqudatlen) == 0 &&
				    rosptr->d25_rbndcfref != (byte *) NULL) {
		/* ROS-association already available (check whether it can be shared) */
		if (rosptr->d25_rrefcnt > 1)
		    /* ROS-association already shared (so it must be established) */
		    break ;
		/* ROS-association not shared (check for state) */
		/* search corresponding IPC-table entry */
		for (k = 0, ipchlpptr = sinfptr->d25_itabref;
				 k < sinfptr->d25_imaxentries; k++, ipchlpptr++)
		    if (D25_GIOS(ipchlpptr->d25_istate) == D25_IRASSOCIATION &&
			ipchlpptr->d25_irosref == rosptr)
			break ;
		if (k == sinfptr->d25_imaxentries)
		    j = sinfptr->d25_rmaxentries ;
		    
		break ;
	    }
	if (j == sinfptr->d25_rmaxentries) {
	    /* no corresponding ROS-table entry found -> search empty entry */
	    for (j = 0, rosptr = sinfptr->d25_rtabref;
				    j < sinfptr->d25_rmaxentries; j++, rosptr++)
		if (D25_GRES(rosptr->d25_rstate) == D25_RFREE)
	   	    break ; 
	    if (j == sinfptr->d25_rmaxentries) {
		/* ROS-table overflow */
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
					svc_c_sev_warning, GDS_S_ROVERFLOW_ERR,
					sinfptr->d25_rmaxentries) ;
		return (D25_AIEXCPT) ;
	    }
	}

	/* occupy IPC-table entry */
	*ipcptr = *actpb->d25_aipcref ;
	ipcptr->d25_irosref = rosptr ;
	sinfptr->d25_iactentries++ ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
		     svc_c_debug1, GDS_S_OCC_IPCENTRY, i, ipcptr->d25_iassid)) ;

	/* update lowest/highest IPC-buffer references */
	if (sinfptr->d25_imnref == (byte *) NULL ||
				      ipcptr->d25_ibufref < sinfptr->d25_imnref)
	    sinfptr->d25_imnref = ipcptr->d25_ibufref ;

	if (ipcptr->d25_ibufref > sinfptr->d25_imxref)
	    sinfptr->d25_imxref = ipcptr->d25_ibufref ;

	actpb->d25_aipcref = ipcptr ;

	/* occupy resp. update ROS-table entry */
	if (D25_GRES(rosptr->d25_rstate) == D25_RFREE) {
	    *rosptr = *actpb->d25_arosref ;
	    /* transfer called PSAP-address */
	    if ((rosptr->d25_rpsapref =
		       (byte *) malloc ((size_t) rqpsaplen)) == (byte *) NULL) {
		rosptr->d25_rstate = D25_SRES(D25_RFREE) ;
		ipcptr->d25_istate = D25_SIOS(D25_IRIDLE) ;
		sinfptr->d25_iactentries-- ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
				 svc_c_sev_warning, GDS_S_NOMEMORY, rqpsaplen) ;
		return (D25_AIEXCPT) ;
	    }
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
	    				svc_c_debug1, GDS_S_ALO_PSAPMEM,
					rosptr->d25_rpsapref, rqpsaplen)) ;

	    (void) memcpy ((void *) rosptr->d25_rpsapref, (void *) rqpsapptr,
							   (size_t) rqpsaplen) ;
	    rosptr->d25_rpsaplen = rqpsaplen ;

	    /* transfer bind request user data */
	    if ((rosptr->d25_rbndrqref =
		       (byte *) malloc ((size_t) rqudatlen)) == (byte *) NULL) {
		free ((void *) rosptr->d25_rpsapref) ;
		rosptr->d25_rstate = D25_SRES(D25_RFREE) ;
		ipcptr->d25_istate = D25_SIOS(D25_IRIDLE) ;
		sinfptr->d25_iactentries-- ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
				 svc_c_sev_warning, GDS_S_NOMEMORY, rqudatlen) ;
		return (D25_AIEXCPT) ;
	    }
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
					svc_c_debug1, GDS_S_ALO_BNDRQMEM,
					rosptr->d25_rbndrqref, rqudatlen)) ;

	    (void) memcpy ((void *) rosptr->d25_rbndrqref, (void *) rqudatptr,
							   (size_t) rqudatlen) ;
	    (void) memcpy ((void *) ipcptr->d25_iudat.d25_datref,
				       (void *) rqudatptr, (size_t) rqudatlen) ;
	    rosptr->d25_rbndrqlen = ipcptr->d25_iudat.d25_datlen = rqudatlen ;

	    rosptr->d25_rrefcnt = 1 ;
	    rosptr->d25_rbndcfref = (byte *) NULL ;
	    sinfptr->d25_ractentries++ ;
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
		     			svc_c_debug1, GDS_S_OCC_ROSENTRY, j,
					D25_UNUSED, rosptr->d25_rcontid)) ;
	    actpb->d25_arosref = rosptr ;

	    retval = D25_ACONT ;
	} else {
	    rosptr->d25_rrefcnt++ ;
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
		  svc_c_debug1, GDS_S_SHARE_ROSENTRY, j, rosptr->d25_rrefcnt)) ;
	    actpb->d25_arosref = rosptr ;

	    /* transfer bind confirmation user data */
	    (void) memcpy ((void *) ipcptr->d25_ibufref,
	       (void *) rosptr->d25_rbndcfref, (size_t) rosptr->d25_rbndcflen) ;
	    ipcptr->d25_iudat.d25_datref = ipcptr->d25_ibufref ;
	    ipcptr->d25_iudat.d25_datlen = rosptr->d25_rbndcflen ;
	    ipcptr->d25_iudat.d25_datoff = 0 ;
	    ipcptr->d25_iresid = D23_RESULT ;

	    retval = D25_ANSUC ;
	}
    } else {
	/* get internally used resources for incoming DAP/DSP */
	if (actpb->d25_arosref == sinfptr->d25_rtabref +
						     sinfptr->d25_rmaxentries) {
	    /* search empty ROS-table entry */
	    for (i = 0, rosptr = sinfptr->d25_rtabref;
				    i < sinfptr->d25_rmaxentries; i++, rosptr++)
		if (D25_GRES(rosptr->d25_rstate) == D25_RFREE)
	   	    break ; 
	    if (i == sinfptr->d25_rmaxentries) {
		/* ROS-table overflow */
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
					svc_c_sev_warning, GDS_S_ROVERFLOW_ERR,
					sinfptr->d25_rmaxentries) ;
		return (D25_AREXCPT) ;
	    }
	    /* occupy ROS-table entry */
	    *rosptr = *actpb->d25_arosref ;
	    rosptr->d25_rrefcnt = 0 ;
	    sinfptr->d25_ractentries++ ;
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
		     		svc_c_debug1, GDS_S_OCC_ROSENTRY, i,
				rosptr->d25_rassid, rosptr->d25_rcontid)) ;
	    actpb->d25_arosref = rosptr ;
	}

	if (actpb->d25_aipcref == sinfptr->d25_itabref +
						     sinfptr->d25_imaxentries ||
	      D25_GIOS(actpb->d25_aipcref->d25_istate) == D25_RINVOKE_PENDING) {
	    /* search empty IPC-table entry */
	    for (j = 0, ipcptr = sinfptr->d25_itabref;
				    j < sinfptr->d25_imaxentries; j++, ipcptr++)
		if (D25_GIOS(ipcptr->d25_istate) == D25_IRIDLE)
		    break ;
	    if (j == sinfptr->d25_imaxentries) {
		/* IPC-table overflow */
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
					svc_c_sev_warning, GDS_S_IOVERFLOW_ERR,
					sinfptr->d25_imaxentries) ;
		return (D25_AREXCPT) ;
	    }
	    /* occupy IPC-table entry */
	    *ipcptr = *actpb->d25_aipcref ;
	    ipcptr->d25_iassid = D25_UNUSED ;
	    ipcptr->d25_irosref = actpb->d25_arosref ;
	    ipcptr->d25_irosref->d25_rrefcnt++ ;
	    sinfptr->d25_iactentries++ ;
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
		     svc_c_debug1, GDS_S_OCC_IPCENTRY, j, ipcptr->d25_iassid)) ;
	    actpb->d25_aipcref = ipcptr ;
	    retval = D25_ACONT ;
	} else
	    retval = D25_ASKIP ;
    }
    return (retval) ;
}

/* --------------------- RELEASE ROS-/IPC-TABLE RESOURCES ------------------ */

signed32 d25_relres (D25_actionpb *actpb) {

    D25_stubinfo	*sinfptr = actpb->d25_asinforef ;
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;
    D25_rosentry	*rosptr = ipcptr->d25_irosref ;

    /* release additional allocated memory */
    if (ipcptr->d25_iudat.d25_datref != (byte *) NULL &&
			  ipcptr->d25_iudat.d25_datref != ipcptr->d25_ibufref) {
	free ((void *) ipcptr->d25_iudat.d25_datref) ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
	      svc_c_debug1, GDS_S_REL_SRBUFFER, ipcptr->d25_iudat.d25_datref)) ;
    }

    /* release IPC-table entry */
    if ((rosptr != (D25_rosentry *) NULL && rosptr->d25_rrefcnt > 1) ||
	actpb->d25_astate != D25_RINVOKE_PENDING ||
					   actpb->d25_aevent == D25_IABORT_EV) {
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
			svc_c_debug1, GDS_S_REL_IPCENTRY, ((byte *) ipcptr -
			(byte *) sinfptr->d25_itabref)/sizeof (D25_ipcentry),
							  ipcptr->d25_iassid)) ;
	ipcptr->d25_istate = D25_SIOS(D25_IRIDLE) ;
	sinfptr->d25_iactentries-- ;
    } else
	return (D25_ATERM) ;

    /* release ROS-table entry */
    if (rosptr != (D25_rosentry *) NULL && rosptr != sinfptr->d25_rtabref +
						     sinfptr->d25_rmaxentries) {
	if (--rosptr->d25_rrefcnt == 0) {
	    if (D25_GRIF(rosptr->d25_rstate) == D25_RINITIATOR) {
		/* release additional allocated memory */
		free ((void *) rosptr->d25_rpsapref) ;
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
	    	       svc_c_debug1, GDS_S_REL_PSAPMEM, rosptr->d25_rpsapref)) ;
		free ((void *) rosptr->d25_rbndrqref) ;
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
		     svc_c_debug1, GDS_S_REL_BNDRQMEM, rosptr->d25_rbndrqref)) ;

		if (rosptr->d25_rbndcfref != (byte *) NULL) {
		    free ((void *) rosptr->d25_rbndcfref) ;
		    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
		    svc_c_debug1, GDS_S_REL_BNDCFMEM, rosptr->d25_rbndcfref)) ;
		}
	    }
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
			svc_c_debug1, GDS_S_REL_ROSENTRY, ((byte *) rosptr -
			(byte *) sinfptr->d25_rtabref)/sizeof (D25_rosentry),
							  rosptr->d25_rassid)) ;
	    rosptr->d25_rstate = D25_SRES(D25_RFREE) ;
	    sinfptr->d25_ractentries-- ;
	}
    }

    return (D25_ACONT) ;
}

/* --------------------- CHECK ROS-/IPC-TABLE RESOURCES -------------------- */

signed32 d25_checkres (D25_actionpb *actpb) {

    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;
    D25_rosentry	*rosptr = ipcptr->d25_irosref ;
    signed32		retval = D25_ACONT ;

    switch (actpb->d25_aglopar) {
    case D25_EQTERM:
	if (rosptr->d25_rrefcnt == 1)
	    retval = D25_ATERM ;
	break ;
    case D25_EQNSUC:
	if (rosptr->d25_rrefcnt == 1)
	    retval = D25_ANSUC ;
	break ;
    case D25_GTSKIP:
	if (rosptr->d25_rrefcnt > 1)
	    retval = D25_ASKIP ;
	break ;
    case D25_GTNSUC:
	if (rosptr->d25_rrefcnt > 1) {
	    ipcptr->d25_iresid = D23_RESULT ;
	    ipcptr->d25_iudat.d25_datlen = 0 ;
	    retval = D25_ANSUC ;
	}
	break ;
    case D25_ACTNSUC:
	if (D25_GITY(ipcptr->d25_istate) == D25_DACTIVE)
	    retval = D25_ANSUC ;
	break ;
    case D25_ENDNSUC:
	if (ipcptr->d25_iudat.d25_datoff == ipcptr->d25_iudat.d25_datlen)
	    retval = D25_ANSUC ;
	break ;
    case D25_CONTSKIP:
	if (actpb->d25_lpar.d25_alocpar != D25_ASKIP) {
	    if (rosptr->d25_rrefcnt == 1)
		actpb->d25_lpar.d25_alocpar = retval = D25_ASKIP ;
	}
	break ;
    case D25_POPRNSUC:
	if (D25_GPTY(ipcptr->d25_istate) == D25_IPCPOPR)
	    retval = D25_ANSUC ;
	break ;
    default:
	break ;
    }
    return (retval) ;
}

/* --------------------- RELEASE ROS-BUFFER -------------------------------- */

signed32 d25_relbuf (D25_actionpb *actpb) {

    D25_stubinfo	*sinfptr = actpb->d25_asinforef ;
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;

    if (ipcptr->d25_iudat.d25_datref != (byte *) NULL &&
	(ipcptr->d25_iudat.d25_datref < sinfptr->d25_imnref ||
			  ipcptr->d25_iudat.d25_datref > sinfptr->d25_imxref)) {
	/* release dynamically allocated buffer */
	free ((void *) ipcptr->d25_iudat.d25_datref) ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
	      svc_c_debug1, GDS_S_REL_SRBUFFER, ipcptr->d25_iudat.d25_datref)) ;
    }

    return (D25_ACONT) ;
}
