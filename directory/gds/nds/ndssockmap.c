/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ndssockmap.c,v $
 * Revision 1.1.184.2  1996/02/18  18:20:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:22  marty]
 *
 * Revision 1.1.184.1  1995/12/08  16:06:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:16:49  root]
 * 
 * Revision 1.1.182.5  1994/09/06  12:26:05  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:22:10  keutel]
 * 
 * Revision 1.1.182.4  1994/06/21  14:47:46  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:43:13  marrek]
 * 
 * Revision 1.1.182.3  1994/06/09  18:47:13  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:20:26  devsrc]
 * 
 * Revision 1.1.182.2  1994/05/10  15:57:27  marrek
 * 	April 1994 submission.
 * 	[1994/04/29  10:52:57  marrek]
 * 
 * Revision 1.1.182.1  1994/02/22  16:10:45  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:59:16  marrek]
 * 
 * Revision 1.1.180.2  1993/10/14  17:52:20  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:42:22  keutel]
 * 
 * Revision 1.1.180.1  1993/10/13  17:33:40  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  16:51:13  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ndssockmap.c,v $ $Revision: 1.1.184.2 $ $Date: 1996/02/18 18:20:41 $";
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
/*   NAME:    ndssockmap.c                                                   */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       This module contains all functions used to map global OSI TSAP-     */
/*       address information into the local address format defined by the    */
/*       socket interface.                                                   */
/*                                                                           */
/*       The following functions are available:                              */
/*                                                                           */
/*          * nds_map_ositosock - mapping entry call function                */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 27.5.93     */
/*datoff *********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <dce/cmx_addr.h>
#include <dce/osiaddr.h>
#include <ndsif.h>
#include <ndscfg.h>

/* ******************* DEFINITION OF CONSTANTS ***************************** */

#define NDS_DEFAULT_PORT	102		/* default RFC1006 port no. */

/* ------------------------------------------------------------------------- */

signed32 nds_map_ositosock (T_adrdesc	       *tsapref,   /* >  TSAP-address */
			    struct sockaddr_in *sockref,   /* >< mapped socket- */
							   /*    addr info */
			    Nds_options	       *optref) {  /* >  specific */
							   /*    options info */

    T_adrdesc		*tsel, *nsap ;
    Nds_opt_oct		*ipopt ;
    Nds_opt_recshort	*portopt ;
    Nds_opt_recoct	*tselopt ;
    char		ipval[MAX_DSP], *tselval ;
    signed32		i, ipvallen, tsellen, portval, portinfolen ;

    if (tsapref->t_infotype == T_TSAPINFO) {
	tsel = tsapref ;
	nsap = (T_adrdesc *) ((char *) tsapref + tsapref->t_nextoff) ;
    } else {
	/* no T-selector available */
	tsel = (T_adrdesc *) NULL ;
	nsap = tsapref ;
    }
    memset ((void *) sockref, (int) 0, (size_t) sizeof (struct sockaddr_in)) ;
    sockref->sin_family = AF_INET ;

    if (nds_get_option (NDS_IP_OPT, optref, (void **) &ipopt) == 0) {
	/* no IP-address information available */
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_MAPNSAP_ERR) ;
	return (NDS_NSAPERR) ;
    }

    if (ipopt->nds_opt_rep & NDS_IMP_MSK) {
	/* get IP-address from NSAP-address */
	if (nds_get_imp_val (nsap, (Nds_opt_short *) ipopt, ipval,
								 &ipvallen) < 0)
	    return (NDS_NSAPERR) ;
	if (ipvallen != 4) {
	    /* invalid size of IP-address */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_MAPNSAP_ERR) ;
	    return (NDS_NSAPERR) ;
	}

	memcpy ((void *) &sockref->sin_addr, (void *) ipval,
							    (size_t) ipvallen) ;
    } else
	memcpy ((void *) &sockref->sin_addr, (void *) ipopt->nds_opt_octval,
						  (size_t) ipopt->nds_opt_len) ;

    if ((portinfolen = nds_get_option (NDS_PORT_OPT, optref,
						     (void **) &portopt)) > 0) {
	if (portopt->nds_opt_rep & NDS_IMP_MSK) {
	    /* get port no. from NSAP-address */
	    if (nds_get_imp_val (nsap, (Nds_opt_short *) portopt, &portval,
							 (signed32 *) NULL) < 0)
		return (NDS_NSAPERR) ;
	    if (portval > 32768) {
		/* invalid port no. */
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_MAPNSAP_ERR) ;
		return (NDS_NSAPERR) ;
	    }
	    sockref->sin_port = htons ((short) portval) ;
	} else {
	    if (tsel != (T_adrdesc *) NULL) {
		/* get port no. related to T-selector */
		i = 0 ;
		while (i < portinfolen) {
		    tselopt = (Nds_opt_recoct *) ((char *) portopt +
							portopt->nds_opt_roff) ;
		    switch (tselopt->nds_opt_rep & NDS_EXPREC_MSK) {
		    case NDS_REC_ASCSTR:
			tselval = ((Nds_opt_recstr *) tselopt)->nds_opt_restrval ;
			tsellen = strlen (tselval) ;
			break ;
		    case NDS_REC_OCTSTR:
			tselval = (char *) tselopt->nds_opt_reoctval ;
			tsellen = tselopt->nds_opt_len ;
			break ;
		    default:
			/* invalid representation of T-selector */
			dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_MAPNSAP_ERR) ;
			return (NDS_NSAPERR) ;
			break ;
		    }
		    if (tsellen == tsel->t_infolng &&
			memcmp ((void *) tselval, (void *) (tsel + 1),
						       (size_t) tsellen) == 0) {
			sockref->sin_port = htons (portopt->nds_opt_reshortval) ;
			break ;
		    }
		    i += portopt->nds_opt_roff + tselopt->nds_opt_roff ;
		    portopt = (Nds_opt_recshort *) ((char *) tselopt +
							tselopt->nds_opt_roff) ;
		}
		if (i >= portinfolen) {
		    /* no corresponding T-selector found */
		    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_MAPNSAP_ERR) ;
		    return (NDS_NSAPERR) ;
		}
	    } else {
		/* no port information available -> use default port */
		sockref->sin_port = htons (NDS_DEFAULT_PORT) ;
	    }
	}
    } else {
	    /* no port information available -> use default port */
	    sockref->sin_port = htons (NDS_DEFAULT_PORT) ;
    }

    return (NDS_NOERR) ;
}
