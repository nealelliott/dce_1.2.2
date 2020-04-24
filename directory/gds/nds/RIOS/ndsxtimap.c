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
 * Revision 1.1.4.2  1996/02/18  18:20:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:00  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:06:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:16:22  root]
 * 
 * Revision 1.1.2.4  1994/09/06  12:25:51  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:17:11  keutel]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:52  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:58  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:13  marrek
 * 	Bug fixes.
 * 	[1994/04/29  09:30:30  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:42:34  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:25:55  keutel
 * 	creation
 * 	[1994/03/21  15:53:54  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ndsxtimap.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:20:22 $";
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

/* ******************* DEFINITION OF STRUCTURES **************************** */

	/* structure of a TSAP-address at the XTI-interface */
	/* OSIMF/6000 XTI implementation */
typedef struct {
	signed16	tsellng ;	/* length of T-selector */
	byte		tsel[32] ;	/* T-selector value */
	char		nsaplng ;	/* length of NSAP-address */
	byte		nsap[20] ;	/* NSAP-address value */
} XTI_tsap ;

/* -------------------------------------------------------------------------- */

signed32 nds_map_ositoxti (signed32	 profid,     /* >  profile-ID */
			   struct netbuf *from,	     /* >< from TSAP-address */
			   struct netbuf *to,	     /* >< to TSAP-address */
			   Nds_options   *optref,    /* >  specific options info */
			   Nds_options   *coptref) { /* >  common options info */

    T_adrdesc	*fromtsel, *fromnsap, *totsel, *tonsap ;
    XTI_tsap	xtifromtsap, xtitotsap ;

    fromtsel = (T_adrdesc *) from->buf ;
    if (fromtsel->t_infotype != T_TSAPINFO) {
	/* no T-selector available */
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_TSEL_ERR, 0) ;
	return (NDS_TSELERR) ;
    }
    fromnsap = (T_adrdesc *) ((char *) fromtsel + fromtsel->t_nextoff) ;

    if (to != (struct netbuf *) NULL) {
	totsel = (T_adrdesc *) to->buf ;
	if (totsel->t_infotype != T_TSAPINFO) {
	    tonsap = totsel ;
	    totsel = (T_adrdesc *) NULL ;
	} else
	    tonsap = (T_adrdesc *) ((char *) totsel + totsel->t_nextoff) ;
    }

    /* generate XTI implementation specific local TSAP-address */
    memset ((void *) &xtifromtsap, (int) 0, (size_t) sizeof (XTI_tsap)) ;
    xtifromtsap.tsellng = fromtsel->t_infolng ;
    memcpy ((void *) xtifromtsap.tsel, (void *) (fromtsel + 1),
						 (size_t) fromtsel->t_infolng) ;
    xtifromtsap.nsaplng = fromnsap->t_infolng ;
    memcpy ((void *) xtifromtsap.nsap, (void *) (fromnsap + 1),
						 (size_t) fromnsap->t_infolng) ;

    memcpy ((void *) from->buf, (void *) (&xtifromtsap),
						 (size_t) sizeof (XTI_tsap)) ;
    from->len = sizeof (XTI_tsap) - 1 ;	/* Note: because  the compiler */
					/* generates an aligned structure */
					/* the real length of the address */
					/* must be corrected by 1 */

    if (to == (struct netbuf *) NULL)
	return (NDS_NOERR) ;


    /* generate XTI implementation specific remote TSAP-address */
    memset ((void *) &xtitotsap, (int) 0, (size_t) sizeof (XTI_tsap)) ;
    xtitotsap.tsellng = totsel->t_infolng ;
    memcpy ((void *) xtitotsap.tsel, (void *) (totsel + 1),
						 (size_t) totsel->t_infolng) ;
    xtitotsap.nsaplng = tonsap->t_infolng ;
    memcpy ((void *) xtitotsap.nsap, (void *) (tonsap + 1),
						 (size_t) tonsap->t_infolng) ;

    memcpy ((void *) to->buf, (void *) (&xtitotsap),
						 (size_t) sizeof (XTI_tsap)) ;
    to->len = sizeof (XTI_tsap) - 1 ;

    return (NDS_NOERR) ;
}
