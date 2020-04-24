/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ndsselect.c,v $
 * Revision 1.1.182.2  1996/02/18  18:20:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:19  marty]
 *
 * Revision 1.1.182.1  1995/12/08  16:06:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:16:46  root]
 * 
 * Revision 1.1.180.4  1994/09/06  12:26:04  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:22:03  keutel]
 * 
 * Revision 1.1.180.3  1994/06/09  18:47:12  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:20:23  devsrc]
 * 
 * Revision 1.1.180.2  1994/05/10  15:57:25  marrek
 * 	April 1994 submission.
 * 	[1994/04/29  10:52:54  marrek]
 * 
 * Revision 1.1.180.1  1994/02/22  16:10:40  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:58:56  marrek]
 * 
 * Revision 1.1.178.2  1993/10/14  17:36:06  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:34:54  keutel]
 * 
 * Revision 1.1.178.1  1993/10/13  17:33:37  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  16:51:08  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ndsselect.c,v $ $Revision: 1.1.182.2 $ $Date: 1996/02/18 18:20:38 $";
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
/*   NAME:    nds_select_nsap                                                */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function selects from two lists of OSI NSAP-addresses a pair of */
/*       NSAP-addresses belonging to the same transport provider (transport  */
/*       profile). By the parameter map_rule the selection operation mode    */
/*       can be controlled (use first pair of NSAP-addresses found or use    */
/*       pair of NSAP-addresses corresponding to the transport provider of   */
/*       the highest priority).                                              */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       ifrom       = List of OSI NSAP-addresses of local NSAP's            */
/*       ito         = List of OSI NSAP-addresses of remote NSAP's           */
/*       map_rule    = selection operation mode (NDS_HPRIO, NDS_FMATCH)      */
/*       ofrom, oto  = references to selected NSAP-addresses                 */
/*       ifdesc      = reference to selected transport profile               */
/*       rudesc      = reference to selected matching/mapping rule           */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*       *ofrom      = selected local OSI NSAP-address                       */
/*       *oto        = selected remote OSI NSAP-address                      */
/*       *ifdesc     = reference to corresponding transport provider (pro-   */
/*                     file)                                                 */
/*       *rudesc     = reference to matching/mapping rule description        */
/*                     entry of selected remote OSI NSAP-address             */
/*   RETURN-VALUE:                                                           */
/*       TRUE        = selection of pair of NSAP-addresses was successfully. */
/*       FALSE       = NSAP-address selection impossible.                    */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 27.5.93     */
/*exoff **********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <dce/cmx_addr.h>
#include <ndsif.h>
#include <ndscfg.h>

/* ******************* DEFINITION OF CONSTANTS ***************************** */

#define INVALID	-1

/* ******************* DECLARATION OF GLOBAL DATA ************************** */

				/* reference to NDS-configuration data base */
extern Nds_shmgenarea	*nds_shm_base ;
				/* reference to rule description area */
				/* initialized by nds_init_match_nsap() */
extern Nds_shmruledesc	*rdref ;

/* ------------------------------------------------------------------------- */

boolean nds_select_nsap (T_adrdesc	*ifrom,
			 T_adrdesc	*ito,
			 T_adrdesc	**ofrom,
			 T_adrdesc	**oto,
			 signed32	map_rule,
			 Nds_shmifdesc	**ifdesc,
			 Nds_shmrule	**rudesc) {

    T_adrdesc		*itohlp ;
    Nds_shmifdesc	*ifref ;
    Nds_shmrule		*ruref, *mtchruref, *mxruref ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%b%b"), GDS_S_CMX,
		   			svc_c_debug6, GDS_S_NDS_SELECT_ENTRY,
					ifrom->t_translng, (byte *) ifrom,
					ito->t_translng, (byte *) ito)) ;

    mxruref = (Nds_shmrule *) SHMREF(rdref->nds_mrules) + rdref->nds_nomrules ;
    do {
	ruref = (Nds_shmrule *) SHMREF(rdref->nds_mrules) ;
	while (ruref < mxruref) {
	    ifref = *ifdesc ;
	    /* search match for 'from-NSAP' address */
	    if (nds_match_nsap (ifrom, NDS_MATCH_PRIO, &ifref, &ruref,
							(Nds_options *) NULL)) {
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%b"), GDS_S_CMX,
		   		svc_c_debug6, GDS_S_NDS_SELECT_FRMATCH,
				ifrom->t_infolng, (byte *) (ifrom + 1))) ;
		itohlp = ito ;
		do {
	     	    /* search match for 'to'-NSAP address */
		    *rudesc =(Nds_shmrule *) SHMREF(rdref->nds_mrules) ;
		    if (nds_match_nsap (itohlp, NDS_MATCH_IF, &ifref, rudesc,
				    (Nds_options *)SHMREF(ruref->nds_mprule))) {
        		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%b"), GDS_S_CMX,
		   		svc_c_debug3, GDS_S_NDS_SELECT_TOMATCH,
				itohlp->t_infolng, (byte *) (itohlp + 1))) ;
			mtchruref = *rudesc ;
			*ifdesc = ifref ;
			if (map_rule == NDS_HPRIO &&
					 ifrom->t_translng > ifrom->t_nextoff) {
			    if (nds_select_nsap ((T_adrdesc *) ((char *) ifrom +
						ifrom->t_nextoff), ito, ofrom,
						oto, map_rule, ifdesc, rudesc))
				return (TRUE) ;
			}

			/* return matching 'from'-NSAP-address */
			(void) memcpy ((void *) *ofrom, (void *) ifrom,
						    (size_t) ifrom->t_nextoff) ;

			/* return matching 'to'-NSAP-address */
			(void) memcpy ((void *) *oto, (void *) itohlp,
						   (size_t) itohlp->t_nextoff) ;

        		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%b%b"), GDS_S_CMX,
		   		svc_c_debug3, GDS_S_NDS_SELECT_EXIT,
				ifrom->t_infolng, (byte *) (*ofrom + 1),
				itohlp->t_infolng, (byte *) (*oto + 1))) ;

			*ofrom = (T_adrdesc *) ((char *) *ofrom +
							     ifrom->t_nextoff) ;
			*oto = (T_adrdesc *) ((char *) *oto + itohlp->t_nextoff) ;
			/* return matching rule description */
			*rudesc = mtchruref ;

			return (TRUE) ;
		    }
		} while ((itohlp = (T_adrdesc *) ((itohlp->t_translng >
		    itohlp->t_nextoff) ? ((char *) itohlp + itohlp->t_nextoff) :
		    				 NULL)) != (T_adrdesc *) NULL) ;
		ruref++ ;
	    }
	}
    } while ((ifrom = (T_adrdesc *) ((ifrom->t_translng > ifrom->t_nextoff) ?
	   ((char *) ifrom + ifrom->t_nextoff) : NULL)) != (T_adrdesc *) NULL) ;
    return (FALSE) ;
}
