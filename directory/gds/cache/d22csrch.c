/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22csrch.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:41  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:27:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:23  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:24:52  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:13:24  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:10  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:17:02  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:16  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:52:56  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:30:52  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:13:26  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:40:39  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:13:23  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:07:57  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  16:28:36  marrek]
 * 
 * Revision 1.1.2.3  1992/07/01  21:15:43  melman
 * 	Really fix casts of arguments in two calls to d22_evalfilter at or
 * 	around lines 320 and 390.  They should be (char *) so that the number
 * 	of bytes are added to the pointer not the number of structures.
 * 	[1992/07/01  21:09:22  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  20:14:37  melman
 * 	Fixed casts of arguments in two calls to d22_evalfilter at or around
 * 	lines 320 and 390.
 * 	[1992/06/01  18:19:53  melman]
 * 
 * 	New GDS merged drop
 * 	[1992/05/28  21:06:26  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22csrch.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:43 $";
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
/* NAME         : d22csrch.c                                          */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 10.05.88                                            */
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
/*  FUNCTION:  d22_csearch ()                                                */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function searches in the DUA-cache for directory objects which   */
/*      are passing the given filter.                                        */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      admptr      = Pointer to the actual DUA-cache administration table.  */
/*      rqptr       = Pointer to the request parameter block.                */
/*      rsptr       = Pointer to the result parameter block which must be    */
/*                    used to return a result or error message.              */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      D22_OK      = The function has been performed successfully.          */
/*      D22_ERROR   = A system error or an internal error is occurred during */
/*                    execution of the function (the variable 'd22_errno'    */
/*                    contains an error no.).                                */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 18.12.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dce/d2dir.h>
#include <d23ipc.h>
#include <d23apdu.h>
#include <d22cache.h>

/* -------------------- declaration of global data ------------------------- */

extern signed32	d22_errno ;	/* error variable */

/* ------------------------------------------------------------------------- */

signed32 d22_csearch (D22_admin_tab *admptr, D23_wtinpb *rqptr,
							    D23_rsrqpb *rsptr) {

    D23_insearch	*pbptr ;
    D23_com_arg		*comptr ;
    D23_srcharg		*srchptr ;
    D23_ds_name		*nmptr ;
    D23_rq_info		*reqptr ;
    D23_at_type		*atsptr, *atvptr, *atcptr ;
    D23_ft1_info	*ftsptr, *ftptr ;
    D23_eom		*eomptr ;
    D22_hash_entry	*hshptr, *objptr ;
    D22_iobj_entry	*ioptr ;
    D22_attr_entry	*actaptr ;
    D2_obj_id		attr_types[D2_AT_MAX] ;
    char		attr_typval[D2_AT_MAX][D2_OBJ_IDL_MAX] ;
    byte		*eptr, *maxpbptr ;
    boolean		filterattr = FALSE ;
    off_t		minoffset, maxoffset, actmoffset, oboffset ;
    signed32		finfosize ;
    signed16		i, index, rdcount, actrqsize = 0 ;
    signed16		no_rqattr, ret_val, maxobj, fndobj1, fndobj2 ;
    signed16		retobj = 0 ;

    /* allocate memory for saving request message(s) */
    if ((pbptr = (D23_insearch *) malloc (D22_RQMAXSIZE)) ==
							(D23_insearch *) NULL) {
	d22_errno = D22_NOMEM_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			     svc_c_sev_warning, GDS_S_NOMEMORY, D22_RQMAXSIZE) ;
	return (D22_ERROR) ;
    }
    do {
	if (actrqsize > 0) {
	    /* get next message block */
	    if (d23_waitin (rqptr) == D23_ERROR) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""),
		       GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_CACHE_WAITIN_ERR) ;
		exit (1) ;
	    }
	    if (rqptr->d23_2oprid == D23_ABORT) {
		free (pbptr) ;
		return (D22_SPEC_ERROR) ;
	    }
	}
	if (rqptr->d23_2lnidt + actrqsize > D22_RQMAXSIZE) {
	    /* error -> max. request message size exceeded */
	    free (pbptr) ;
	    d22_errno = D22_MSGSIZE_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_REQSZ_ERR,
				rqptr->d23_2lnidt + actrqsize, D22_RQMAXSIZE) ;
	    return (D22_ERROR) ;
	}
	/* save message block */
	(void) memcpy ((void *) ((char *) pbptr + actrqsize),
		       (void *) rqptr->d23_2rfidt, (size_t) rqptr->d23_2lnidt) ;
	actrqsize += rqptr->d23_2lnidt ;
    } while (rqptr->d23_2imdat == D23_MRDATA) ;

    /* verify request parameter */
    maxpbptr = (byte *) pbptr + actrqsize ;

    /* verify common argument parameter */
    comptr = (D23_com_arg *) (pbptr + 1) ;
    if (comptr->d23_cmtag != D23_CMTAG) {
	free (pbptr) ;
	d22_errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				comptr->d23_cmtag, D23_CMTAG) ;
	return (D22_ERROR) ;
    }
    if (comptr->d23_cmctr.d23_scslm != D2_S_UNLIMITED &&
					      comptr->d23_cmctr.d23_scslm < 1) {
	d22_errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
			svc_c_sev_warning, GDS_S_CACHE_INVSLIM_ERR,
			comptr->d23_cmctr.d23_scslm, D2_S_UNLIMITED, 1) ;
	return (D22_ERROR) ;
    }

    srchptr = (D23_srcharg *) (comptr + 1) ;
    D22_SKIPTAG (D23_srcharg, srchptr, D23_SECTAG, D23_S_SEP, 0) ;
    D22_SKIPTAG (D23_srcharg, srchptr, D23_RQTAG, D23_S_NM,
					  ((D23_ds_name *)srchptr)->d23_dsoff) ;
    D22_SKIPTAG (D23_srcharg, srchptr, D23_BITTAG, D23_S_BIT,
					((D23_bit_info *)srchptr)->d23_boff) ;

    /* verify search information */
    if (srchptr->d23_Itag != D23_ITAG) {
	free (pbptr) ;
	d22_errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				srchptr->d23_Itag, D23_ITAG) ;
	return (D22_ERROR) ;
    }

    /* verify base object information */
    nmptr = (D23_ds_name *) (srchptr + 1) ;
    if ((srchptr->d23_Iobjsub != D2_BASE_OBJECT &&
	srchptr->d23_Iobjsub != D2_WHOLE_SUBTREE) ||
	nmptr->d23_dstag != D23_NAMTAG ||
	(srchptr->d23_Iobjsub == D2_WHOLE_SUBTREE &&
			    (nmptr->d23_dsoff != 0 || nmptr->d23_dslen != 0))) {
	free (pbptr) ;
	d22_errno = D22_BASEOBJ_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				    svc_c_sev_warning, GDS_S_CACHE_INVDNS_ERR) ;
	return (D22_ERROR) ;
    }

    if (d22_check_par (pbptr->d23_Ihdr.d23_versno, pbptr->d23_Ihdr.d23_dirid,
	    (srchptr->d23_Iobjsub == D2_BASE_OBJECT) ? nmptr :
		  (D23_ds_name *) NULL, (D22_naming_ref *) NULL) == D22_ERROR) {
	free (pbptr) ;
	return (D22_ERROR) ;
    }

    /* verify requested attributes */
    reqptr = (D23_rq_info *) ((char *) nmptr + D23_S_NM + nmptr->d23_dsoff) ;

    if (reqptr->d23_rqtag != D23_REQTAG ||
	(no_rqattr = reqptr->d23_rqnbr) < D2_ALL_ATT_REQ ||
	 no_rqattr > D2_AT_MAX || ((ret_val = reqptr->d23_rqreq) != D2_R_TYP &&
				   ret_val != D2_R_VAL_TYP && no_rqattr >= 0)) {
	free (pbptr) ;
	d22_errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				  svc_c_sev_warning, GDS_S_CACHE_INVATTRS_ERR) ;
	return (D22_ERROR) ;
    }

    atsptr = atvptr = (D23_at_type *) (reqptr + 1) ;
    for (i = 0; i < no_rqattr; i++) {
	if (atvptr->d23_atttag != D23_ATTTAG) {
	    free (pbptr) ;
	    d22_errno = D22_MSYNTAX_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				atvptr->d23_atttag, D23_ATTTAG) ;
	    return (D22_ERROR) ;
	}
	if (atvptr->d23_atlen > D2_OBJ_IDL_MAX) {
	    free (pbptr) ;
	    d22_errno = D22_MSYNTAX_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVOIDLEN_ERR,
				atvptr->d23_atlen, D2_OBJ_IDL_MAX) ;
	    return (D22_ERROR) ;
	}
	for (atcptr = atsptr; atcptr < atvptr;) {
	    if (atcptr->d23_atlen == atvptr->d23_atlen &&
		memcmp ((void *) (atcptr + 1), (void *) (atvptr + 1),
					     (size_t) atvptr->d23_atlen) == 0) {
		/* error -> attribute type multiple available */
		free (pbptr) ;
		d22_errno = D22_MUATTR_ERR ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				  svc_c_sev_warning, GDS_S_CACHE_INVATTRS_ERR) ;
		return (D22_ERROR) ;
	    }
	    atcptr = (D23_at_type *) ((char *) atcptr + D23_S_ATT +
							   atcptr->d23_atoff) ;
	}
	/* save requested attribute type information */
	attr_types[i].d2_typ_len = atvptr->d23_atlen ;
	attr_types[i].d2_type = (Ob_id_string) attr_typval[i] ;
	(void) memcpy ((void *) attr_typval[i], (void *) (atvptr + 1),
						   (size_t) atvptr->d23_atlen) ;

	atvptr = (D23_at_type *) ((char *) atvptr + D23_S_ATT +
							   atvptr->d23_atoff) ;
    }	

    /* verify filter attribute information if available */
    ftsptr = ftptr = (D23_ft1_info *) atvptr ;
    if (ftptr->d23_fp1tag == D23_PATTAG) {
	if (d22_fcheck_filter (&ftptr, maxpbptr) == D2_ERROR) {
	    free (pbptr) ;
	    return (D22_ERROR) ;
	}
	eomptr = (D23_eom *) ftptr ;

	/* save filter attribute information */
	finfosize = (char *) ftptr - (char *) ftsptr ;
	if ((ftptr = (D23_ft1_info *) malloc (finfosize)) ==
							(D23_ft1_info *) NULL) {
	    free (pbptr) ;
	    d22_errno = D22_NOMEM_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			     svc_c_sev_warning, GDS_S_NOMEMORY, finfosize) ;
	    return (D22_ERROR) ;
	}
	(void) memcpy ((void *) ftptr, (void *) ftsptr, (size_t) finfosize) ;
	ftsptr = ftptr ;

	filterattr = TRUE ;
    } else
	eomptr = (D23_eom *) ftptr ;

    if (eomptr > (D23_eom *) (maxpbptr - D23_S_EOM)) {
	free (pbptr) ;
	if (filterattr)
	    free (ftsptr) ;
	d22_errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				 svc_c_sev_warning, GDS_S_CACHE_INCONSMSG_ERR) ;
	return (D22_ERROR) ;
    }
    if (eomptr->d23_eomtag != D23_EOMTAG) {
	free (pbptr) ;
	if (filterattr)
	    free (ftsptr) ;
	d22_errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
			svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
			eomptr->d23_eomtag, D23_EOMTAG) ;
	return (D22_ERROR) ;
    }

    /* activate specific DUA-cache if required */
    if (pbptr->d23_Ihdr.d23_dirid != admptr->d22_act_cache_id &&
	  d22_activate_cache (pbptr->d23_Ihdr.d23_dirid, admptr) == D22_ERROR) {
	free (pbptr) ;
	if (filterattr)
		free (ftsptr) ;
	return (D22_ERROR) ;
    }

    /* generate default result parameter block and result message */
    rsptr->d23_3hdinfo.d2_version = D23_V02 ;
    rsptr->d23_3assid = rqptr->d23_2assid ;
    rsptr->d23_3invinfo = rqptr->d23_2invinfo ;
    rsptr->d23_3rmdat = D23_LASTFG ;	
    rsptr->d23_3rfrdt = eptr = rqptr->d23_2rfidt ;
    rsptr->d23_3lnrdt = 0 ;

    if (srchptr->d23_Iobjsub == D2_BASE_OBJECT) {
	/* search is for a single directory object in the DUA-cache */
	if (d22_search_obj (admptr, (D2_name_string) (nmptr + 1), &hshptr,
						   &index, TRUE) == D22_ERROR) {
	    free (pbptr) ;
	    if (filterattr)
		free (ftsptr) ;
	    return (D22_ERROR) ;
	}
	ioptr = (admptr->d22_iobjtab + (hshptr->d22_state & D22_IINDEXMASK)) ;
	if (!filterattr || d22_evalfilter (&ftptr,
			(D23_ft1_info *) ((char *) ftsptr + finfosize),
			ioptr->d22_iattrptr, (signed32) hshptr->d22_obsize)) {
	    /* search for requested attribute information, generate */
	    /* result parameter block and result message(s) */
	    if (d22_attrimsg (admptr, &eptr, rsptr, ioptr->d22_iattrptr,
	        hshptr->d22_obsize, no_rqattr, attr_types, ret_val) ==
							       D22_SPEC_ERROR) {
		free (pbptr) ;
		if (filterattr)
		    free (ftsptr) ;
		return (D22_SPEC_ERROR) ;
	    }
	}
	/* update reference time of the directory object entry in the */
	/* DUA-cache 'internally stored'-table and in the DUA-cache hash */
	/* table resp. file (only if required) */
	if ((ioptr->d22_ilreftime = time ((time_t) 0))/D22_TIMEBASE !=
							 hshptr->d22_lreftime) {
	    hshptr->d22_lreftime = ioptr->d22_ilreftime/D22_TIMEBASE ;
	    if (d22_cfwrite (admptr->d22_hsh_fd, (void *) hshptr,
		(size_t) sizeof (D22_hash_entry), (off_t) (index *
				       sizeof (D22_hash_entry))) == D22_ERROR) {
		free (pbptr) ;
		if (filterattr)
		    free (ftsptr) ;
		return (D22_ERROR) ;
	    }
	}
    } else {
	/* search is for all directory objects in the DUA-cache */
	/*allocate memory for temporary holding attribute informations */
	if ((actaptr = (D22_attr_entry *) malloc (D22_SRCHMAXSIZE)) ==
						      (D22_attr_entry *) NULL) {
	    free (pbptr) ;
	    if (filterattr)
		free (ftsptr) ;
	    d22_errno = D22_NOMEM_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			   svc_c_sev_warning, GDS_S_NOMEMORY, D22_SRCHMAXSIZE) ;
	    return (D22_ERROR) ;
	}
	minoffset = maxoffset = actmoffset = (off_t) 0 ;
	maxobj = admptr->d22_desctab->d22_ract_entries +
		     admptr->d22_desctab->d22_pact_entries +
		     admptr->d22_desctab->d22_nact_entries ;
	fndobj1 = 0 ;
	do {
	    /* read attribute informations from DUA-cache attribute file */
	    if ((rdcount = d22_cfread (admptr->d22_attr_fd, (void *) actaptr,
		     	  (size_t) D22_SRCHMAXSIZE, actmoffset)) == D22_ERROR) {
		free (pbptr) ;
		free (actaptr) ;
		if (filterattr)
		    free (ftsptr) ;
		return (D22_ERROR) ;
	    }
	    maxoffset = minoffset + (off_t) rdcount ;
	    for (objptr = admptr->d22_hshtab, fndobj2 = 0; fndobj2 < maxobj;
								     objptr++) {
		if (objptr->d22_state == D22_NO_ENTRY) continue ;
		fndobj2++ ;
		oboffset = objptr->d22_oboffset ;
		if (oboffset >= minoffset && oboffset < maxoffset) {
		    if (oboffset > actmoffset)
			actmoffset = oboffset ;
		    if (oboffset + (off_t) objptr->d22_obsize <= maxoffset) {
			fndobj1++ ;
			ftptr = ftsptr ;
			if (!filterattr ||
			    d22_evalfilter (&ftptr,
				(D23_ft1_info *) ((char *) ftsptr + finfosize),
				(D22_attr_entry *) ((char *) actaptr +
				(oboffset - minoffset)), objptr->d22_obsize)) {
			    retobj++ ;
			    if (comptr->d23_cmctr.d23_scslm != D2_S_UNLIMITED &&
				         retobj > comptr->d23_cmctr.d23_scslm) {
				fndobj1 = maxobj ;
				break ;
			    }
			    if (d22_attrimsg (admptr, &eptr, rsptr,
				(D22_attr_entry *) ((char *) actaptr +
				(oboffset - minoffset)), objptr->d22_obsize,
				no_rqattr, attr_types, ret_val) == D22_SPEC_ERROR) {
	    			free (actaptr) ;
				free (pbptr) ;
				if (filterattr)
				    free (ftsptr) ;
				return (D22_SPEC_ERROR) ;
			    }
			}
		    }
		}
	    }
	    minoffset = actmoffset ;
	} while (fndobj1 < maxobj) ;
	free (actaptr) ;
    }

    rsptr->d23_3rmdat = D23_LASTFG ;
    if (comptr->d23_cmctr.d23_scslm != D2_S_UNLIMITED &&
					retobj > comptr->d23_cmctr.d23_scslm) {
	((D23_part_oq *)eptr)->d23_oqtag = D23_OQTAG ;
	((D23_part_oq *)eptr)->d23_oq_uncrit = FALSE ;
	((D23_part_oq *)eptr)->d23_oq_limit = D2_SIZE_LIMIT_EX ;
	eptr += D23_S_POQ ;
    }
    ((D23_com_res *) eptr)->d23_crtag = D23_CRSTAG ;
    ((D23_com_res *) eptr)->d23_ali_deref = FALSE ;
    eptr += D23_S_CRES ;
    ((D23_eom *) eptr)->d23_eomtag = D23_EOMTAG ;
    rsptr->d23_3lnrdt += D23_S_CRES + D23_S_EOM ;
    free (pbptr) ;
    if (filterattr)
	free (ftsptr) ;
    return (D22_OK) ;
}
