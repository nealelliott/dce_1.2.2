/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22cfilter.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:30  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:26:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:16  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:24:44  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:12:47  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:50:59  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:16:37  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:00  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:51:43  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:23:04  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:12:06  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:40:08  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:12:53  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:13:37  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:05:46  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22cfilter.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:33 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d22cfilter.c                                        */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 22.10.90                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  d22_fcheck_filter ()                                          */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function verifies the syntax of the filter information which is  */
/*      available in the request information block of a search.              */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      ftsptr      = Pointer to the reference of the filter information     */
/*                    block.                                                 */
/*      maxptr      = Pointer to the end of the request information block.   */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                                                                           */
/*     *ftsptr      = Pointer to the (modified) reference of the filter      */
/*                    information block (the reference is set to the end of  */
/*                    the checked filter information block).                 */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      D2_NOERROR  = The syntax of the filter information is correct.       */
/*      D2_ERROR    = The syntax of the filter information is incorrect.     */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 22.10.90     */
/*exoff **********************************************************************/

#include <gds.h>
#include <dce/d2dir.h>
#include <d23ipc.h>
#include <d23apdu.h>
#include <d22cache.h>

/* -------------------- declaration of global data ------------------------- */

extern signed32	d22_errno ;	/* error variable */

/* ------------------------------------------------------------------------- */

signed32 d22_fcheck_filter (D23_ft1_info **ftsptr, byte *maxptr) {

    D23_ft1_info	*ftptr = *ftsptr ;
    D23_fi1_info	*fiptr ;
    D23_fs1_info	*fsptr ;
    D23_at_type		*tpptr ;
    D23_av_value	*avptr ;
    D2_a_match		fimatch ;
    signed32		i, pos ;

    if (ftptr + 1 > (D23_ft1_info *) maxptr) {
	d22_errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				 svc_c_sev_warning, GDS_S_CACHE_INCONSMSG_ERR) ;
	return (D22_ERROR) ;
    }
    if (ftptr->d23_fp1tag != D23_PATTAG) {	
	d22_errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				ftptr->d23_fp1tag, D23_PATTAG) ;
	return (D22_ERROR) ;
    }

    switch (ftptr->d23_fp1mod) {
    case D2_ITEM:
	/* check filter item syntax */
	if ((fiptr = (D23_fi1_info *) (ftptr + 1)) + 1 >
						      (D23_fi1_info *) maxptr) {
	    d22_errno = D22_MSYNTAX_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				 svc_c_sev_warning, GDS_S_CACHE_INCONSMSG_ERR) ;
	    return (D22_ERROR) ;
	}
	if (fiptr->d23_fi1tag != D23_FIITAG) {
	    d22_errno = D22_MSYNTAX_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				fiptr->d23_fi1tag, D23_FIITAG) ;
	    return (D22_ERROR) ;
	}
	if (((fimatch = fiptr->d23_fi1match) != D2_EQUAL &&
	     fimatch != D2_SUBSTRINGS && fimatch != D2_GTEQ &&
	     fimatch != D2_LTEQ && fimatch != D2_PRESENT &&
						  fimatch != D2_APPROX_MATCH)) {
	    d22_errno = D22_MSYNTAX_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			 svc_c_sev_warning, GDS_S_CACHE_INVMATCH_ERR, fimatch) ;
	    return (D22_ERROR) ;
	}
	if ((tpptr = (D23_at_type *) (fiptr + 1)) + 1 >
						       (D23_at_type *) maxptr) {
	    d22_errno = D22_MSYNTAX_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				 svc_c_sev_warning, GDS_S_CACHE_INCONSMSG_ERR) ;
	    return (D22_ERROR) ;
	}
	if (tpptr->d23_atttag != D23_ATTTAG) {
	    d22_errno = D22_MSYNTAX_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				tpptr->d23_atttag, D23_ATTTAG) ;
	    return (D22_ERROR) ;
	}
	if (tpptr->d23_atlen > D2_OBJ_IDL_MAX) {
	    d22_errno = D22_MSYNTAX_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVOIDLEN_ERR,
				tpptr->d23_atlen, D2_OBJ_IDL_MAX) ;
	    return (D22_ERROR) ;
	}

	switch (fimatch) {
	case D2_SUBSTRINGS:
	    fsptr = (D23_fs1_info *) ((char *) tpptr + D23_S_ATT +
							     tpptr->d23_atoff) ;
	    do {
		if ((fsptr + 1) > (D23_fs1_info *) maxptr) {
		    d22_errno = D22_MSYNTAX_ERR ;
	    	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				 svc_c_sev_warning, GDS_S_CACHE_INCONSMSG_ERR) ;
		    return (D22_ERROR) ;
		}
		if (fsptr->d23_fs1tag != D23_FISTAG) {
		    d22_errno = D22_MSYNTAX_ERR ;
		    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"),
				GDS_S_GENERAL, svc_c_sev_warning,
				GDS_S_CACHE_INVMSGTAG_ERR,
				fsptr->d23_fs1tag, D23_FISTAG) ;
		    return (D22_ERROR) ;
		}
		if (((pos = fsptr->d23_fs1position) != D2_INITIAL &&
		    pos != D2_ANY && pos != D2_FINAL)) {
		    d22_errno = D22_MSYNTAX_ERR ;
		    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"),
					GDS_S_GENERAL, svc_c_sev_warning,
					GDS_S_CACHE_INVMATCH_ERR, pos) ;
		    return (D22_ERROR) ;
		}
 		if (fsptr->d23_fs1len > D2_ATL_MAX) {
		    d22_errno = D22_MSYNTAX_ERR ;
		    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"),
				GDS_S_GENERAL, svc_c_sev_warning,
				GDS_S_CACHE_INVAVLEN_ERR,
				fsptr->d23_fs1len, D2_ATL_MAX) ;
		    return (D22_ERROR) ;
		}
		fsptr = (D23_fs1_info *) ((char *) fsptr +
						D23_S_FS1 + fsptr->d23_fs1off) ;
	    } while (fsptr->d23_fs1tag == D23_FISTAG) ;
	    *ftsptr = (D23_ft1_info *) fsptr ;
	    break ;
	case D2_EQUAL:
	case D2_APPROX_MATCH:
	case D2_GTEQ:
	case D2_LTEQ:
	    if ((avptr = (D23_av_value *) ((char *) tpptr + D23_S_ATT +
			     tpptr->d23_atoff)) + 1 > (D23_av_value *) maxptr) {
		d22_errno = D22_MSYNTAX_ERR ;
	    	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				 svc_c_sev_warning, GDS_S_CACHE_INCONSMSG_ERR) ;
		return (D22_ERROR) ;
	    }
	    if (avptr->d23_avlen > D2_ATL_MAX) {
		d22_errno = D22_MSYNTAX_ERR ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVAVLEN_ERR,
				avptr->d23_avlen, D2_ATL_MAX) ;
		return (D22_ERROR) ;
	    }
	    *ftsptr = (D23_ft1_info *) ((char *) avptr + D23_S_ATV +
							     avptr->d23_avoff) ;
	    break ;
	case D2_PRESENT:
	    *ftsptr = (D23_ft1_info *) ((char *) tpptr + D23_S_ATT +
							     tpptr->d23_atoff) ;
	    break ;
	default:
	    break ;
	}
	break ;
    case D2_AND:
    case D2_OR:
	/* check set of filter syntax */
	if (ftptr->d23_fp1count < 1) {
	    d22_errno = D22_MSYNTAX_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
		svc_c_sev_warning, GDS_S_CACHE_INVFILT_ERR, ftptr->d23_fp1mod) ;
	    return (D22_ERROR) ;
	}
	(*ftsptr)++ ;
	for (i = 0; i < (signed32) ftptr->d23_fp1count; i++)
	    if (d22_fcheck_filter (ftsptr, maxptr) == D2_ERROR)
		return (D2_ERROR) ;
	break ;
    case D2_NOT:
	/* check filter syntax */
	(*ftsptr)++ ;
	return (d22_fcheck_filter (ftsptr, maxptr)) ;
    default:
	d22_errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
		svc_c_sev_warning, GDS_S_CACHE_INVFILT_ERR, ftptr->d23_fp1mod) ;
	return (D22_ERROR) ;
    }

    return (((byte *) *ftsptr > maxptr) ? D2_ERROR : D2_NOERROR) ;
}
