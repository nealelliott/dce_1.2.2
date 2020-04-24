/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ndsxtimap.c,v $
 * Revision 1.1.4.2  1996/02/18  18:20:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:57  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:05:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:16:11  root]
 * 
 * Revision 1.1.2.5  1994/09/06  12:25:49  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:17:06  keutel]
 * 
 * Revision 1.1.2.4  1994/06/21  14:47:36  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:42:38  marrek]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:44  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:49  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:07  marrek
 * 	Bug fixes.
 * 	[1994/04/29  09:30:18  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:42:15  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:25:39  keutel
 * 	creation
 * 	[1994/03/21  15:53:38  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ndsxtimap.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:20:18 $";
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
/*   NAME:    ndsxtimap.c                                                    */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       This module contains all the functions used to map global OSI TSAP- */
/*       address information into the local address formats defined by the   */
/*       supported XTI-transport provider. This module must be modified      */
/*       resp. rewritten if there are changes in existing local address      */
/*       formats or if new local address formats are introduced.             */
/*                                                                           */
/*       The following functions are available:                              */
/*                                                                           */
/*          * nds_map_ositoxti   - mapping entry call function               */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 27.5.93     */
/*datoff *********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <xti.h>
#include <dce/cmx_addr.h>
#include <dce/osiaddr.h>
#include <ndsif.h>
#include <ndscfg.h>

/* -------------------------------------------------------------------------- */

signed32 nds_map_ositoxti (signed32	 profid,     /* >  profile-ID */
			   struct netbuf *from,	     /* >< from TSAP-address */
			   struct netbuf *to,	     /* >< to TSAP-address */
			   Nds_options   *optref,    /* >  specific options info */
			   Nds_options   *coptref) { /* >  common options info */
    T_adrdesc   *fromtsel, *fromnsap, *totsel, *tonsap ;
    byte	tsapbuf[T_TSELSIZE + T_NSAPSIZE + 3], *tsapref ;
    signed32	tsaplen ;

    /* verify from-address */
    fromtsel = (T_adrdesc *) from->buf ;
    if (fromtsel->t_infotype != T_TSAPINFO) {
	/* no T-selector available */
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_TSEL_ERR, 0) ;
	return (NDS_TSELERR) ;
    }
    fromnsap = (T_adrdesc *) ((char *) fromtsel + fromtsel->t_nextoff) ;

    /* generate OTS/9000 specific local TSAP-address */
    tsapref = tsapbuf ;
    tsaplen = fromtsel->t_infolng + fromnsap->t_infolng + 3 ;
 
    /* supply T-selector */
    *tsapref++ = (byte) fromtsel->t_infolng ;
    memcpy ((void *) tsapref, (void *) (fromtsel + 1),
						 (size_t) fromtsel->t_infolng) ;
    tsapref += fromtsel->t_infolng ;

    /* supply NSAP-address */
    *tsapref++ = (byte) fromnsap->t_infolng ;
    memcpy ((void *) tsapref, (void *) (fromnsap + 1),
						 (size_t) fromnsap->t_infolng) ;
    tsapref += fromnsap->t_infolng ;
    *tsapref = 0 ;

    /* copy from-TSAP-address into output buffer */
    memcpy ((void *) from->buf, (void *) tsapbuf, (size_t) tsaplen) ;

    from->len = tsaplen ;

    if (to == (struct netbuf *) NULL)
	/* no to-address specified */
        return (NDS_NOERR) ;

    /* generate OTS/9000 specific remote TSAP-address */
    totsel = (T_adrdesc *) to->buf ;
    tsapref = tsapbuf ;
    if (totsel->t_infotype == T_TSAPINFO) {
	tonsap = (T_adrdesc *) ((char *) totsel + totsel->t_nextoff) ;
        tsaplen = totsel->t_infolng + tonsap->t_infolng + 3 ;

	/* supply T-selector */
	*tsapref++ = (byte) totsel->t_infolng ;
	memcpy ((void *) tsapref, (void *) (totsel + 1),
						   (size_t) totsel->t_infolng) ;
	tsapref += totsel->t_infolng ;
    } else {
	tonsap = totsel ;
        tsaplen = tonsap->t_infolng + 3 ;
	*tsapref++ = 0 ;
    }

    /* supply NSAP-address */
    *tsapref++ = (byte) tonsap->t_infolng ;
    memcpy ((void *) tsapref, (void *) (tonsap + 1),
						   (size_t) tonsap->t_infolng) ;
    tsapref += tonsap->t_infolng ;
    *tsapref = 0 ;

    /* copy to-TSAP-address into output buffer */
    memcpy ((void *) to->buf, (void *) tsapbuf, (size_t) tsaplen) ;

    to->len = tsaplen ;

    return (NDS_NOERR) ;
}
