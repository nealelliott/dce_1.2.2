/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ndsmatch.c,v $
 * Revision 1.1.181.2  1996/02/18  18:20:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:18  marty]
 *
 * Revision 1.1.181.1  1995/12/08  16:06:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:16:43  root]
 * 
 * Revision 1.1.179.4  1994/09/06  12:26:03  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:21:51  keutel]
 * 
 * Revision 1.1.179.3  1994/06/09  18:47:11  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:20:22  devsrc]
 * 
 * Revision 1.1.179.2  1994/05/10  15:57:24  marrek
 * 	April 1994 submission.
 * 	[1994/04/29  10:52:52  marrek]
 * 
 * Revision 1.1.179.1  1994/02/22  16:10:36  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:58:48  marrek]
 * 
 * Revision 1.1.177.2  1993/10/14  17:36:05  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:34:53  keutel]
 * 
 * Revision 1.1.177.1  1993/10/13  17:33:35  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  16:51:06  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ndsmatch.c,v $ $Revision: 1.1.181.2 $ $Date: 1996/02/18 18:20:37 $";
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
/*   NAME:    ndsmatch.c                                                     */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       This module contains functions necessary to match an OSI NSAP-      */
/*       address against a single NSAP-address or a group of NSAP-addresses  */
/*       (specified by a regular expression) available in the NDS-con-       */
/*       figuration data base.                                               */
/*                                                                           */
/*       The following functions are available:                              */
/*                                                                           */
/*          * nds_init_match_nsap - initialized (dependent from mapping      */
/*                                  mode) reference to corresponding NSAP-   */
/*                                  address matching rule area               */
/*          * nds_match_nsap      - match OSI NSAP-address against NSAP-     */
/*                                  addresses from matching rule area        */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 27.5.93     */
/*datoff *********************************************************************/

#include <gds.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dce/cmx_addr.h>
#include <ndsif.h>
#include <ndscfg.h>


/* ******************* DECLARATION OF GLOBAL DATA ************************** */

				/* reference to NDS-configuration data base */
extern Nds_shmgenarea	*nds_shm_base ;
				/* reference to matching rule area */
Nds_shmruledesc	*rdref = (Nds_shmruledesc *) NULL ;    

/* ------------------------------------------------------------------------- */

boolean nds_init_match_nsap (signed32	     map_type,	 /* >  mapping type */
			     Nds_shmruledesc **rudesc) { /* >< matching rules */


    signed32	i ;

    /* search matching/mapping rule description entry */
    rdref = (Nds_shmruledesc *) SHMREF(nds_shm_base->nds_ruledescarea) ;
    for (i = 0; i < nds_shm_base->nds_noruledesc; i++, rdref++)
	if ((rdref->nds_mtype & map_type) == map_type)
	    break ;
    if (i == nds_shm_base->nds_noruledesc) {
	/* no matching rule description entry found */
	rdref = (Nds_shmruledesc *) NULL ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				       svc_c_sev_warning, GDS_S_NDS_RULES_ERR) ;
	return (FALSE) ;
    }
    *rudesc = rdref ;
    return (TRUE) ;
}

/* ------------------------------------------------------------------------- */

boolean nds_match_nsap (T_adrdesc     *nsapref,   /* >  NSAP-address */
			signed32      match_mode, /* >  matching mode */
			Nds_shmifdesc **ifdesc,   /* >< profile description */
			Nds_shmrule   **rudesc,   /* >< matching rules */
			Nds_options   *optref) {  /* >  specific options info */

    Nds_shmrule		*ruref, *rumxref ;
    Nds_opt_short	*lsubprofopt, *rsubprofopt ;
    signed32		i ;
    byte		*nsaprule ;
    char		nsapstr[(T_NSAPSIZE*2)+1], *p ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%b"), GDS_S_CMX,
		   		svc_c_debug6, GDS_S_NDS_MATCH_ENTRY,
				nsapref->t_infolng, (byte *) (nsapref + 1))) ;

    /* convert NSAP-address into a string */
    p = (char *) (nsapref + 1) ;
    for (i = 0; i < nsapref->t_infolng; i++, p++)
	sprintf (&nsapstr[i * 2], "%02X", (int) (*p & 0xFF)) ;

    /* search matching rule entry which matches the supplied NSAP-address */
    rumxref = (Nds_shmrule *) SHMREF(rdref->nds_mrules) + rdref->nds_nomrules ;
    for (ruref = *rudesc; ruref < rumxref; ruref++) {
	if (match_mode == NDS_MATCH_IF &&
			  (Nds_shmifdesc *) SHMREF(ruref->nds_ifref) != *ifdesc)
	    continue ;
	else {
	    if (match_mode == NDS_MATCH_PRIO &&
		*ifdesc != (Nds_shmifdesc *) NULL &&
		((Nds_shmifdesc *) SHMREF(ruref->nds_ifref))->nds_profprioid <=
						      (*ifdesc)->nds_profprioid)
		continue ;
	}

	nsaprule = (unsigned char *) SHMREF(ruref->nds_mtchrule) ;
	if ((ruref->nds_mtchtype & NDS_ADDR_TY_MSK) == NDS_GROUP_ADDR) {
		/* group of NSAP-addresses -> compare compiled regular */
		/* expression against subject string */
		if (nds_regex (nsaprule, nsapstr) != 0)
		    continue ;
	} else {
		if (nsapref->t_infolng != (ruref->nds_mtchtype & NDS_ADDR_LN_MSK) ||
		    memcmp ((void *) nsaprule, (void *) (nsapref + 1),
					      (size_t) nsapref->t_infolng) != 0)
		    continue ;
	}
	if (nds_get_option (NDS_SUBPROF_OPT, optref, (void **) &lsubprofopt) &&
	    nds_get_option (NDS_SUBPROF_OPT, (Nds_options *)
			   SHMREF(ruref->nds_mprule), (void **) &rsubprofopt)) {
	    /* check sub-profile */
	    if (lsubprofopt->nds_opt_shortval != rsubprofopt->nds_opt_shortval)
		continue ;
	}
	if (match_mode != NDS_MATCH_IF)
	    *ifdesc = (Nds_shmifdesc *) SHMREF(ruref->nds_ifref) ;
	*rudesc = ruref ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
				    svc_c_debug6, GDS_S_NDS_MATCH_EXIT, TRUE)) ;
	return (TRUE) ;
    }
    /* no matching rule entry found */
    *rudesc = ruref ;
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
				   svc_c_debug6, GDS_S_NDS_MATCH_EXIT, FALSE)) ;
    return (FALSE) ;
}
