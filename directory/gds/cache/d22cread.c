/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22cread.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:38  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:27:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:21  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:24:50  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:13:14  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:07  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:16:56  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:12  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:52:35  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:28:58  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:13:04  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:40:34  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:13:15  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:07:27  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  16:28:09  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:14:22  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:06:14  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22cread.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:40 $";
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
/* NAME         : d22cread.c                                          */
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
/*  FUNCTION:  d22_cread ()                                                  */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function reads attribute informations about a given directory    */
/*      object from the DUA-cache.                                           */
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
/*  AUTHOR: R.Horn                                        DATE: 15.12.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>

/* -------------------- declaration of global data ------------------------- */

extern signed32	d22_errno ;	/* error variable */
extern signed16 d22_ipcmsgsize ;/* max. size of a IPC-message buffer */

/* ------------------------------------------------------------------------- */

signed32 d22_cread (D22_admin_tab *admptr, D23_wtinpb *rqptr,
							    D23_rsrqpb *rsptr) {


    D23_inread		*pbptr = (D23_inread *) rqptr->d23_2rfidt ;
    D23_com_arg		*comptr = (D23_com_arg *) (pbptr + 1) ;
    D23_ds_name		*nmptr ;
    D23_rq_info		*rqaptr ;
    D22_hash_entry	*hshptr ;
    D22_iobj_entry	*ioptr ;
    D2_ret_val		ret_val ;
    D23_at_type		*attr_sptr, *attr_vptr, *attr_cptr ;
    D2_obj_id		attr_types[D2_AT_MAX] ;
    char		attr_typval[D2_AT_MAX][D2_OBJ_IDL_MAX] ;
    signed16		i, nmsize, index, no_rqattr ;
    signed32		res ;
    byte		*eptr ;
    char		nmbuf[D2_DNL_MAX] ;
    byte		*maxiptr = rqptr->d23_2rfidt + rqptr->d23_2lnidt ;
    byte		*maxoptr = rqptr->d23_2rfidt + d22_ipcmsgsize ;

    /* verify structure of request message */
    if (rqptr->d23_2imdat != D23_LASTFG) {
	d22_errno = D22_OPTYPE_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		    svc_c_sev_warning, GDS_S_IPCMTYPIL_ERR, rqptr->d23_2imdat) ;
	return (D22_ERROR) ;
    }
    if (comptr->d23_cmtag != D23_CMTAG) {
	d22_errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				comptr->d23_cmtag, D23_CMTAG) ;
	return (D22_ERROR) ;
    }

    /* examine and verify object name */
    /* (but first skip irrelevant common argument informations) */
    nmptr = (D23_ds_name *) (comptr + 1) ;
    D22_SKIPTAG (D23_ds_name, nmptr, D23_SECTAG, D23_S_SEP, 0) ;
    D22_SKIPTAG (D23_ds_name, nmptr, D23_RQTAG, D23_S_NM, nmptr->d23_dsoff) ;
    D22_SKIPTAG (D23_ds_name, nmptr, D23_BITTAG, D23_S_BIT,
					((D23_bit_info *)nmptr)->d23_boff) ;

    if (d22_check_par (pbptr->d23_Ehdr.d23_versno, pbptr->d23_Ehdr.d23_dirid,
				   nmptr, (D22_naming_ref *) NULL) == D22_ERROR)
	return (D22_ERROR) ;

    /* save object name */
    (void) strcpy ((char *) nmbuf, (char *) (nmptr + 1)) ;

    /* verify requested attribute information */
    rqaptr = (D23_rq_info *) ((char *) nmptr + D23_S_NM + nmptr->d23_dsoff) ;
	
    if (rqaptr->d23_rqtag != D23_REQTAG) {
	d22_errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				rqaptr->d23_rqtag, D23_REQTAG) ;
	return (D22_ERROR) ;
    }
    no_rqattr = rqaptr->d23_rqnbr ;
    ret_val = rqaptr->d23_rqreq ;
    if (no_rqattr < D2_ALL_ATT_REQ || no_rqattr > D2_AT_MAX ||
	   (no_rqattr >= 0 && ret_val != D2_R_TYP && ret_val != D2_R_VAL_TYP)) {
	d22_errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				  svc_c_sev_warning, GDS_S_CACHE_INVATTRS_ERR) ;
	return (D22_ERROR) ;
    }
    /* verify requested attribute types */
    attr_sptr = attr_vptr = (D23_at_type *) (rqaptr + 1) ;
    for (i = 0; i < no_rqattr; i++) {
	if (attr_vptr->d23_atttag != D23_ATTTAG) {
	    d22_errno = D22_MSYNTAX_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				attr_vptr->d23_atttag, D23_ATTTAG) ;
	    return (D22_ERROR) ;
	}
	if (attr_vptr->d23_atlen > D2_OBJ_IDL_MAX) {
	    d22_errno = D22_MSYNTAX_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVOIDLEN_ERR,
				attr_vptr->d23_atlen, D2_OBJ_IDL_MAX) ;
	    return (D22_ERROR) ;
	}
	for (attr_cptr = attr_sptr; attr_cptr < attr_vptr;) {
	    if (attr_cptr->d23_atlen == attr_vptr->d23_atlen &&
	        memcmp ((void *) (attr_cptr + 1), (void *) (attr_vptr + 1),
					  (size_t) attr_cptr->d23_atlen) == 0) {
		/* error -> attribute type multiple available */
		d22_errno = D22_MUATTR_ERR ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				  svc_c_sev_warning, GDS_S_CACHE_INVATTRS_ERR) ;
		return (D22_ERROR) ;
	    }
	    attr_cptr = (D23_at_type *) ((char *) attr_cptr + D23_S_ATT +
							 attr_cptr->d23_atoff) ;
	}
	/* save requested attribute type information */
	attr_types[i].d2_typ_len = attr_vptr->d23_atlen ;
	attr_types[i].d2_type = (Ob_id_string) attr_typval[i] ;
	(void) memcpy ((void *) attr_typval[i], (void *) (attr_vptr + 1),
						(size_t) attr_vptr->d23_atlen) ;

	attr_vptr = (D23_at_type *) ((char *) attr_vptr + D23_S_ATT +
							 attr_vptr->d23_atoff) ;
    }
    if ((D23_eom *) attr_vptr > (D23_eom *) (maxiptr - D23_S_EOM)) {
	d22_errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				 svc_c_sev_warning, GDS_S_CACHE_INCONSMSG_ERR) ;
	return (D22_ERROR) ;
    }
    if (((D23_eom *) attr_vptr)->d23_eomtag != D23_EOMTAG) {
	d22_errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
			svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
			((D23_eom *) attr_vptr)->d23_eomtag, D23_EOMTAG) ;
	return (D22_ERROR) ;
    }
	
    /* activate specific DUA-cache if required */
    if (pbptr->d23_Ehdr.d23_dirid != admptr->d22_act_cache_id &&
	    d22_activate_cache (pbptr->d23_Ehdr.d23_dirid, admptr) == D22_ERROR)
	return (D22_ERROR) ;

    /* search directory object */
    if (d22_search_obj (admptr, (D2_name_string) (nmptr + 1), &hshptr, &index,
							     TRUE) == D22_ERROR)
	return (D22_ERROR) ;

    /* generate constant parts of result parameter block */
    rsptr->d23_3hdinfo.d2_version = D23_V02 ;
    rsptr->d23_3assid = rqptr->d23_2assid ;
    rsptr->d23_3invinfo = rqptr->d23_2invinfo ;
    rsptr->d23_3rfrdt = eptr = rqptr->d23_2rfidt ;

    /* search requested attribute informations and generate result message */
    ioptr = admptr->d22_iobjtab + (hshptr->d22_state & D22_IINDEXMASK) ;
    res = d22_attrimsg (admptr, &eptr, rsptr, ioptr->d22_iattrptr,
			   hshptr->d22_obsize, no_rqattr, attr_types, ret_val) ;
    if (res == D22_SPEC_ERROR)
	return (res) ;
    if (res == 0) {
	/* no attributes found -> generate specific error message */
	nmsize = strlen (nmbuf) + 1 ;
	if ((eptr + D23_S_RSERR + D23_S_NM + D2_ALIGN (nmsize)) > maxoptr) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""),
		        GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_CACHE_BUFSZ_ERR) ;
	}

	rsptr->d23_3rltid = D23_RERROR ;

	/* write error information to error result message */
	((D23_rserror *)eptr)->d23_Zretcod = D2_CALL_ERR ;
	((D23_rserror *)eptr)->d23_Zerrcls = D2_PAR_ERR ;
	((D23_rserror *)eptr)->d23_Zerrval = D2_ATT_ERROR ;
	((D23_rserror *)eptr)->d23_Zversion = D23_V1988 ;
	eptr += D23_S_RSERR ;

	/* write DN to error result message */
	((D23_ds_name *)eptr)->d23_dstag = D23_NAMTAG ;
	((D23_ds_name *)eptr)->d23_dsoff = D2_ALIGN (nmsize) ;
	((D23_ds_name *)eptr)->d23_dslen = nmsize ;
	strcpy ((char *) ((D23_ds_name *) eptr + 1), nmbuf) ;
	eptr += D23_S_NM + ((D23_ds_name *)eptr)->d23_dsoff ;	

	/* write attribute information to result message */
	for (i = 0; i < no_rqattr; i++) {
	    if ((eptr + D23_S_ATPROB + D23_S_ATT + D2_OBJ_IDL_MAX +
							 D23_S_EOM) > maxoptr) {
		rsptr->d23_3rmdat = D23_MRDATA ;
		rsptr->d23_3lnrdt = (signed16) (eptr - rsptr->d23_3rfrdt) ;
		if (d23_result (rsptr) == D23_ERROR) {
		    dce_svc_printf (DCE_SVC (gds_svc_handle, ""),
		       GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_CACHE_RESULT_ERR) ;
		    exit (1) ;
		}
		eptr = rsptr->d23_3rfrdt ;	
	    }
	    ((D23_at_prob *)eptr)->d23_aptag = D23_APTAG ;
	    ((D23_at_prob *)eptr)->d23_apval = D2_ATT_ERROR ;
	    eptr += D23_S_ATPROB ;
	    ((D23_at_type *)eptr)->d23_atttag = D23_ATTTAG ;
	    ((D23_at_type *)eptr)->d23_atoff =
					   D2_ALIGN (attr_types[i].d2_typ_len) ;
	    ((D23_at_type *)eptr)->d23_atlen =
						      attr_types[i].d2_typ_len ;
	    (void) memcpy ((void *) ((D23_at_type *) eptr + 1),
	    (void *) attr_types[i].d2_type, (size_t) attr_types[i].d2_typ_len) ;
	    eptr += D23_S_ATT + ((D23_at_type *)eptr)->d23_atoff ;
	}
	((D23_eom *)eptr)->d23_eomtag = D23_EOMTAG ;
	eptr += D23_S_EOM ;
	rsptr->d23_3rmdat = D23_LASTFG ;
	rsptr->d23_3lnrdt = (signed16) (eptr - rsptr->d23_3rfrdt) ;
	if (d23_result (rsptr) == D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""),
		       GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_CACHE_RESULT_ERR) ;
	    exit (1) ;
	}
	return (D22_SPEC_ERROR) ;
    } else {
	rsptr->d23_3rmdat = D23_LASTFG ;
	((D23_com_res *) eptr)->d23_crtag = D23_CRSTAG ;
	((D23_com_res *) eptr)->d23_ali_deref = FALSE ;
	eptr += D23_S_CRES ;
	((D23_eom *) eptr)->d23_eomtag = D23_EOMTAG ;
	rsptr->d23_3lnrdt += D23_S_CRES + D23_S_EOM ;

	/* update reference time of the directory object entry in the */
	/* DUA-cache 'internally stored'-table and in the DUA-cache hash */
	/* table resp. file (only if required) */
	if ((ioptr->d22_ilreftime = time ((time_t) 0))/D22_TIMEBASE !=
							 hshptr->d22_lreftime) {
	    hshptr->d22_lreftime = ioptr->d22_ilreftime/D22_TIMEBASE ;
	    if (d22_cfwrite (admptr->d22_hsh_fd, (void *) hshptr,
		(size_t) sizeof (D22_hash_entry), (off_t) (index *
					 sizeof (D22_hash_entry))) == D22_ERROR)
		return (D22_ERROR) ;
	}

	return (D22_OK) ;
    }
}
