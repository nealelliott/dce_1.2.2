/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ndsgetopt.c,v $
 * Revision 1.1.179.2  1996/02/18  18:20:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:13  marty]
 *
 * Revision 1.1.179.1  1995/12/08  16:06:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:16:39  root]
 * 
 * Revision 1.1.177.4  1994/09/06  12:26:01  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:17:55  keutel]
 * 
 * Revision 1.1.177.3  1994/06/09  18:47:08  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:20:19  devsrc]
 * 
 * Revision 1.1.177.2  1994/05/10  15:57:23  marrek
 * 	April 1994 submission.
 * 	[1994/04/29  10:52:50  marrek]
 * 
 * Revision 1.1.177.1  1994/02/22  16:10:30  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:58:32  marrek]
 * 
 * Revision 1.1.175.2  1993/10/14  17:36:03  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:34:49  keutel]
 * 
 * Revision 1.1.175.1  1993/10/13  17:33:30  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  16:51:01  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ndsgetopt.c,v $ $Revision: 1.1.179.2 $ $Date: 1996/02/18 18:20:34 $";
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
/*   NAME:    ndsgetopt.c                                                    */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       This module contains functions to get information of a specific     */
/*       option from an 'options information block' or from a NSAP-address   */
/*       (implicit option value).                                            */
/*                                                                           */
/*       The following functions are available:                              */
/*                                                                           */
/*          * nds_get_option  - get specific option information              */
/*          * nds_get_imp_val - get implicit option value from NSAP-address  */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 27.5.93     */
/*datoff *********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dce/cmx_addr.h>
#include <dce/osiaddr.h>
#include <ndsif.h>
#include <ndscfg.h>

/* ------------------------------------------------------------------------- */

signed32 nds_get_option (OPTFLDTYPE  optid,	 /* >  option-ID */
			 Nds_options *optref,    /* >  options info block */
			 void	     **optval) { /* >< option information */

    OPTFLDTYPE		i ;
    signed32  		optcnt ;

    if (optref == (Nds_options *) NULL || (optref->nds_opt_types & optid) == 0)
	/* option not available */
	return (0) ;

    for (i = 1, optcnt = 0; i < optid; i <<= 1)
	if ((optref->nds_opt_types & i))
	    optcnt++ ;

    *optval = (void *) ((char *) optref + optref->nds_opt_off[optcnt]) ;
    return ((signed32) (optref->nds_opt_off[optcnt + 1] -
						 optref->nds_opt_off[optcnt])) ;
}

/* ----------------------------------------------------------------------- */

signed32 nds_get_imp_val (T_adrdesc     *nsap,    /* >  NSAP-address */
			  Nds_opt_short *impdesc, /* >  impl. value descript. */
			  void		*impres,  /* >< implicit value */
			  signed32	*implen) {/* >< size of impl. value */

    signed32		i, res, octval, impvallen, dspno1, dspno2 ;
    signed32		dsppartrep[MAX_DSP_PARTS], inforep ;
    char	  	impval[MAX_DSP] ;
    static signed32	afi, no_dspparts ;
    static char		idi[MAX_IDI+2], *dspparts[MAX_DSP_PARTS] ;
    static char		dsppartval[MAX_DSP_PARTS][MAX_DSP+1] ;

    if (nsap != (T_adrdesc *) NULL) {
	/* initialize NSAP-decoding result areas */
	for (i = 0; i < MAX_DSP_PARTS; i++) {
	dspparts[i] = dsppartval[i] ;
	dsppartval[i][0] = '\0' ;
	}
	/* decode NSAP-address */
	if ((res = d2p_decode_nsap_addr (DEC_COMPLETE, &afi, idi, &no_dspparts,
					 dspparts, dsppartrep, (byte *) (nsap +
					 1), nsap->t_infolng)) != NO_ERR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
				  svc_c_sev_warning, GDS_S_NDS_DNSAP_ERR, res) ;
            return (res) ;
	}
    }

    if (impdesc->nds_opt_shortval > 0) {
	/* get implicit information from DSP-part(s) */
	dspno1 = impdesc->nds_opt_shortval >> 4 ;
	dspno2 = impdesc->nds_opt_shortval & 0x0F ;
	if (dspno1 > no_dspparts || dspno2 > no_dspparts) {
            /* reference to an invalid dsp part */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_MAPNSAP_ERR) ;
            return (NDS_NSAPERR) ;
	}
	if (dspno1 > 0) {
	    /* get implicit information from two different DSP-parts */
	    if (dsppartrep[dspno1 - 1] != dsppartrep[dspno2 - 1]) {
        	/* DSP-parts with different representation */
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_MAPNSAP_ERR) ;
        	return (NDS_NSAPERR) ;
	    }
	    strcpy (impval, dspparts[dspno1 - 1]) ;
	    strcat (impval, dspparts[dspno2 - 1]) ;
	} else
	    strcpy (impval, dspparts[dspno2 - 1]) ;
	inforep = dsppartrep[dspno2 - 1] ;
    } else {
	if (strlen (idi) == 0) {
	    /* no IDI available */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_MAPNSAP_ERR) ;
            return (NDS_NSAPERR) ;
	}
	/* get implicit information from IDI */
	strcpy (impval, idi) ;
	inforep = DSP_UNIT_DEC1 ;
    }

    /* convert NSAP-address information into requested format */
    switch (impdesc->nds_opt_rep) {
    case NDS_IMP_DIGSTR:
	switch (inforep) {
	case DSP_UNIT_DEC1:
	case DSP_UNIT_DEC2:
	    strcpy (impres, impval) ;
	    impvallen = (signed32) strlen (impres) ;
	    break ;
	default:
	    /* invalid representation */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_MAPNSAP_ERR) ;
	    return (NDS_NSAPERR) ;
	    break ;
	}
	break ;
    case NDS_IMP_OCTSTR:
	switch (inforep) {
	case DSP_UNIT_HEX2:
	case DSP_UNIT_DOT2:
	case DSP_UNIT_ASCII2:
	    if ((strlen (impval) % (size_t) 2) != (size_t) 0) {
		/* invalid size of hex-string */
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_MAPNSAP_ERR) ;
		return (NDS_NSAPERR) ;
	    }

	    impvallen = (signed32) (strlen (impval) / (size_t) 2) ;
	    /* convert hex-string into an octet representation */
	    for (i = 0; i < impvallen; i++) {
		sscanf (&impval[i * 2], "%2x", &octval) ;
		*((byte *)((byte *) impres + i)) = (byte) octval ;
	    }
	    break ;
	case DSP_UNIT_DOT1:
	case DSP_UNIT_ASCII1:
	    if ((strlen (impval) % (size_t) 3) != (size_t) 0) {
		/* invalid size of dotted digit-string */
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_MAPNSAP_ERR) ;
		return (NDS_NSAPERR) ;
	    }

	    impvallen = (signed32) (strlen (impval) / (size_t) 3) ;
	    /* convert dotted digit-string into an octet representation */
	    for (i = 0; i < impvallen; i++) {
		sscanf (&impval[i * 3], "%3d", &octval) ;
		*((byte *)((byte *) impres + i)) = (byte) octval ;
	    }
	    break ;
	default:
	    /* invalid representation */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_MAPNSAP_ERR) ;
	    return (NDS_NSAPERR) ;
	    break ;
	}
	break ;
    case NDS_IMP_INT:
	switch (inforep) {
	case DSP_UNIT_DEC1:
	case DSP_UNIT_DEC2:
	case DSP_UNIT_DOT1:
	case DSP_UNIT_SLASH11:
	case DSP_UNIT_SLASH12:
	    sscanf (impval, "%d", (int *) impres) ;
	    break ;
	case DSP_UNIT_HEX1:
	case DSP_UNIT_HEX2:
	case DSP_UNIT_DOT2:
	case DSP_UNIT_SLASH21:
	case DSP_UNIT_SLASH22:
	    sscanf (impval, "%x", (int *) impres) ;
	    break ;
	default:
	    /* invalid representation */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_MAPNSAP_ERR) ;
	    return (NDS_NSAPERR) ;
	    break ;
	}
	impvallen = sizeof (signed32) ;
	break ;
    default:
	/* unknown representation */
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_MAPNSAP_ERR) ;
	return (NDS_NSAPERR) ;
	break ;
    }

    if (implen != (signed32 *) NULL)
	*implen = impvallen ;
    return (NDS_NOERR) ;
}
