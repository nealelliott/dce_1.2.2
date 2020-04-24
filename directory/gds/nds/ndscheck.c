/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ndscheck.c,v $
 * Revision 1.1.177.2  1996/02/18  18:20:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:06  marty]
 *
 * Revision 1.1.177.1  1995/12/08  16:06:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:16:34  root]
 * 
 * Revision 1.1.175.4  1994/09/06  12:25:56  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:17:33  keutel]
 * 
 * Revision 1.1.175.3  1994/06/09  18:47:04  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:20:14  devsrc]
 * 
 * Revision 1.1.175.2  1994/05/10  15:57:22  marrek
 * 	April 1994 submission.
 * 	[1994/04/29  10:52:47  marrek]
 * 
 * Revision 1.1.175.1  1994/02/22  17:41:32  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:58:09  marrek]
 * 
 * Revision 1.1.173.2  1993/10/14  17:36:02  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:34:41  keutel]
 * 
 * Revision 1.1.173.1  1993/10/13  17:33:26  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  16:50:53  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ndscheck.c,v $ $Revision: 1.1.177.2 $ $Date: 1996/02/18 18:20:28 $";
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
/*   NAME:    nds_check_tsap ()                                              */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function verifies a TSAP-address for consistency.               */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       tsap        = Reference to the TSAP-address information             */
/*       local_tsap  = TSAP-address usage flag                               */
/*                     TRUE: the TSAP-address belongs to a local TSAP        */
/*                     FALSE: the TSAP-address belongs to a remote TSAP      */
/*                     (for a remote TSAP-address the T-selector may be      */
/*                     optional).                                            */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     None                                                  */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*       NDS_NOERR   = The function has been performed successfully.         */
/*       < 0         = An inconsistent information was detected in the       */
/*                     TSAP-address (in this case the returned negative      */
/*                     value describes the detected problem).                */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 27.5.93     */
/*exoff **********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <sys/types.h>
#include <dce/cmx_addr.h>
#include <ndsif.h>
#include <ndscfg.h>

/* ------------------------------------------------------------------------- */

signed32 nds_check_tsap (T_adrdesc *tsap, boolean local_tsap) {

    T_adrdesc	*tsapmax ;
    signed32	translng, nsap_no = 0 ;	

    tsapmax = (T_adrdesc *) ((char *) tsap + tsap->t_translng) ;

    /* verify size of whole TSAP-address */
    if ((translng = tsap->t_translng) < sizeof (T_adrdesc) ||
							translng > T_TSAPSIZE) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
			      svc_c_sev_warning, GDS_S_NDS_TSAP_ERR, translng) ;
	return (NDS_TSAPERR) ;
    }

    /* verify T-selector */
    if (tsap->t_infotype == T_TSAPINFO) {
	if (tsap->t_infovers != T_TSELVERS ||
	    tsap->t_infolng < 0 || tsap->t_infolng > T_TSELSIZE ||
	    tsap->t_nextoff != sizeof (T_adrdesc) + T_ALIGN4(tsap->t_infolng)) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
		       svc_c_sev_warning, GDS_S_NDS_TSEL_ERR, tsap->t_infolng) ;
	    return (NDS_TSELERR) ;
	}
	translng -= tsap->t_nextoff ;
	tsap = (T_adrdesc *) ((char *) tsap + tsap->t_nextoff) ;
    } else {
	if (local_tsap) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_TSEL_ERR, 0) ;
	    return (NDS_TSELERR) ;
	}
    }

    /* verify NSAP-address(es) */
    while (tsap < tsapmax) {
	if (tsap->t_infotype != T_NSAPINFO || tsap->t_infovers != T_NSAPVERS ||
	    tsap->t_infolng < 1 || tsap->t_infolng > T_NSAPSIZE ||
	    tsap->t_nextoff != sizeof (T_adrdesc) + T_ALIGN4(tsap->t_infolng) ||
						 tsap->t_translng != translng) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
		       svc_c_sev_warning, GDS_S_NDS_NSAP_ERR, tsap->t_infolng) ;
	    return (NDS_NSAPERR) ;
	}
	nsap_no++ ;
	translng -= tsap->t_nextoff ;
	tsap = (T_adrdesc *) ((char *) tsap + tsap->t_nextoff) ;
    }
    /* verify no. of NSAP-addresses */
    if (nsap_no == 0 || nsap_no > T_MAXNSAP) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_CMX,
		   svc_c_sev_warning, GDS_S_NDS_NSAP_ERR1, nsap_no, T_MAXNSAP) ;
	return (NDS_NSAPERR) ;
    }
    if (translng != 0) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
			      svc_c_sev_warning, GDS_S_NDS_TSAP_ERR, translng) ;
	return (NDS_TSAPERR) ;
    }

    return (NDS_NOERR) ;
}
