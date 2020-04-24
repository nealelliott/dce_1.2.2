/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22ccomp.c,v $
 * Revision 1.1.12.3  1996/02/18  19:43:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:27  marty]
 *
 * Revision 1.1.12.2  1995/12/08  15:26:29  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/DCE1_1_WP/1  1995/04/26  19:41 UTC  jrr
 * 	Include errno.h for AIX.
 * 	[1995/12/08  14:54:14  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:24:42  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:12:42  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:50:56  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:16:31  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  17:52:27  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:51:25  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:21:02  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:11:45  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:39:56  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:12:45  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:07:01  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  16:27:41  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:13:18  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:05:37  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22ccomp.c,v $ $Revision: 1.1.12.3 $ $Date: 1996/02/18 19:43:30 $";
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
/* NAME         : d22ccomp.c                                          */
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
/*  FUNCTION:  d22_ccompare ()                                               */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function compares a given attribute value of a given directory   */
/*      object with the attribute value(s) of this directory object          */
/*      contained in the DUA-cache.                                          */
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
#include <string.h>
#include <time.h>
#include <errno.h>
#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>

/* -------------------- declaration of global data ------------------------- */

extern signed32	d22_errno ;	/* error variable */

/* ------------------------------------------------------------------------- */

signed32 d22_ccompare (D22_admin_tab *admptr, D23_wtinpb *rqptr,
							    D23_rsrqpb *rsptr) {

    D23_incompare	*pbptr = (D23_incompare *) rqptr->d23_2rfidt ;
    D23_com_arg		*comptr = (D23_com_arg *) (pbptr + 1) ;
    D23_ds_name		*nmptr ;
    D23_at_type		*rqaptr ;
    D23_av_info		*aviptr ;
    D23_av_value	*avptr ;
    D23_rscompinfo	*rscptr ;
    D23_eom		*eomptr ;
    D22_hash_entry	*hshptr ;
    D22_iobj_entry	*ioptr ;
    D22_attr_entry	*attrptr, *maxaptr ;
    D22_size_info	*p, *pmax ;
    signed16		index, val_size ;
    byte		*eptr, *maxiptr = rqptr->d23_2rfidt + rqptr->d23_2lnidt ;

    /* verify request parameter */
    if (rqptr->d23_2imdat != D23_LASTFG) {
	errno = D22_OPTYPE_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		    svc_c_sev_warning, GDS_S_IPCMTYPIL_ERR, rqptr->d23_2imdat) ;
	return (D22_ERROR) ;
    }

    /* verify common arguments */
    if (comptr->d23_cmtag != D23_CMTAG) {
	errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				comptr->d23_cmtag, D23_CMTAG) ;
	return (D22_ERROR) ;
    }
    nmptr = (D23_ds_name *) (comptr + 1) ;
    D22_SKIPTAG (D23_ds_name, nmptr, D23_SECTAG, D23_S_SEP, 0) ;
    D22_SKIPTAG (D23_ds_name, nmptr, D23_RQTAG, D23_S_NM, nmptr->d23_dsoff) ;
    D22_SKIPTAG (D23_ds_name, nmptr, D23_BITTAG, D23_S_BIT,
					  ((D23_bit_info *)nmptr)->d23_boff) ;

    /* verify DN */
    if (d22_check_par (pbptr->d23_Khdr.d23_versno, pbptr->d23_Khdr.d23_dirid,
				   nmptr, (D22_naming_ref *) NULL) == D22_ERROR)
	return (D22_ERROR) ;

    /* verify requested attribute */
    rqaptr = (D23_at_type *) ((char *) nmptr + D23_S_NM + nmptr->d23_dsoff) ;
    if (rqaptr->d23_atttag != D23_ATTTAG) {
	errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				rqaptr->d23_atttag, D23_ATTTAG) ;
	return (D22_ERROR) ;
    }

    aviptr = (D23_av_info *) ((char *) rqaptr + D23_S_ATT + rqaptr->d23_atoff) ;
    if (aviptr->d23_avtag != D23_AVTAG) {
	errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				aviptr->d23_avtag, D23_AVTAG) ;
	return (D22_ERROR) ;
    }
    avptr = (D23_av_value *) (aviptr + 1) ;
    if (avptr->d23_avlen > D2_ATL_MAX ||
			      D2_ALIGN (avptr->d23_avlen) != avptr->d23_avoff) {
	errno = D22_ATTSIZE_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"),
		GDS_S_GENERAL, svc_c_sev_warning, GDS_S_CACHE_INVAVLEN_ERR,
		avptr->d23_avlen, D2_ATL_MAX) ;
	return (D22_ERROR) ;
    }	
    eomptr = (D23_eom *) ((char *) avptr + D23_S_ATV + avptr->d23_avoff) ;
    if (eomptr > (D23_eom *) (maxiptr - D23_S_EOM)) {
	errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				 svc_c_sev_warning, GDS_S_CACHE_INCONSMSG_ERR) ;
	return (D22_ERROR) ;
    }
    if (eomptr->d23_eomtag != D23_EOMTAG) {
	errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				eomptr->d23_eomtag, D23_EOMTAG) ;
	return (D22_ERROR) ;
    }

    /* activate specific DUA-cache if required */
    if (pbptr->d23_Khdr.d23_dirid != admptr->d22_act_cache_id &&
	    d22_activate_cache (pbptr->d23_Khdr.d23_dirid, admptr) == D22_ERROR)
	return (D22_ERROR) ;

    /* search directory object */
    if (d22_search_obj (admptr, (D2_name_string) (nmptr + 1), &hshptr, &index,
							     TRUE) == D22_ERROR)
	return (D22_ERROR) ;

    /* search requested attribute */
    ioptr = admptr->d22_iobjtab + (hshptr->d22_state & D22_IINDEXMASK) ;
    attrptr = ioptr->d22_iattrptr ;
    maxaptr = (D22_attr_entry *) ((char *) attrptr + hshptr->d22_obsize) ;	
    do {
	if (rqaptr->d23_atlen == attrptr->d22_aidsize &&
	    memcmp ((void *) (rqaptr + 1), (void *) attrptr->d22_aident,
					    (size_t) attrptr->d22_aidsize) == 0)
	    break ;
    } while ((attrptr = (D22_attr_entry *) ((char *) attrptr +
	   sizeof (D22_attr_entry) + attrptr->d22_asizes.d22_voff)) < maxaptr) ;
    if (attrptr >= maxaptr) {
	/* error -> attribute type not found */
	errno = D22_NOFNDATTR_ERR ;
	return (D22_ERROR) ;
    }

    /* generate default result message */
    rsptr->d23_3rmdat = D23_LASTFG ;
    rsptr->d23_3rfrdt  = rqptr->d23_2rfidt ;
    rsptr->d23_3lnrdt = D23_S_COMRS + D23_S_EN + D23_S_NM +
				     nmptr->d23_dsoff + D23_S_CRES + D23_S_EOM ;
    rscptr = (D23_rscompinfo *) rsptr->d23_3rfrdt ;
    rscptr->d23_Ltag = D23_LTAG ;
    rscptr->d23_Lmatsgn = FALSE ;
    rscptr->d23_Lfrmen = FALSE ;
    eptr = (byte *) (rscptr + 1) ;
    ((D23_en_info *)eptr)->d23_entag = D23_ENTTAG ;
    ((D23_en_info *)eptr)->d23_enmas = FALSE ;
    ((D23_en_info *)eptr)->d23_enmaxat = 0 ;
    eptr += D23_S_EN ;
    ((D23_ds_name *)eptr)->d23_dstag = D23_NAMTAG ;
    ((D23_ds_name *)eptr)->d23_dsoff = nmptr->d23_dsoff ;
    ((D23_ds_name *)eptr)->d23_dslen = nmptr->d23_dslen ;
    strcpy ((char *) (eptr + D23_S_NM), (char *) (nmptr + 1)) ;
    eptr += D23_S_NM + nmptr->d23_dsoff ;
    ((D23_com_res *)eptr)->d23_crtag = D23_CRSTAG ;
    ((D23_com_res *)eptr)->d23_ali_deref = FALSE ;
    eptr += D23_S_CRES ;
    ((D23_eom *)eptr)->d23_eomtag = D23_EOMTAG ;

    /* attribute found -> compare attribute values */
    pmax = (D22_size_info *) ((char *) attrptr + sizeof (D22_attr_entry) +
						 attrptr->d22_asizes.d22_voff) ;
    p = ((attrptr->d22_aetype & D22_REC_FLAG) == D22_REC_FLAG) ?
      (D22_size_info *) (attrptr + 1) : (D22_size_info *) &attrptr->d22_asizes ;
    while ( p < pmax) {
	if ((val_size = p->d22_vlen) == avptr->d23_avlen) {
	    if ((attrptr->d22_aetype & D22_NAME_AFLAG) == D22_NAME_AFLAG) {
		if (d22_cignore_comp ((char *) p + 1, (char *) rqaptr + 1)) {
		    /* attribute values are equal */
		    rscptr->d23_Lmatsgn = TRUE ;
		    break ;
		}
	    } else {
		if (memcmp ((void *) (avptr + 1), (void *) (p + 1),
						      (size_t) val_size) == 0) {
		    /* attribute values are equal */
		    rscptr->d23_Lmatsgn = TRUE ;
		    break ;
		}
	    }
	}
	p = (D22_size_info *) ((char *) p + sizeof (D22_size_info) +
								  p->d22_voff) ;
    }

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
