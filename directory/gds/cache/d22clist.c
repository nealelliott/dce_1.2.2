/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22clist.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:33  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:26:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:19  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:24:47  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:13:00  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:03  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:16:47  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:06  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:52:09  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:26:17  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:12:33  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:40:20  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:13:04  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:07:14  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  16:27:55  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:14:02  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:06:00  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22clist.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:37 $";
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
/* NAME         : d22clist.c                                          */
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
/*  FUNCTION:  d22_clist ()                                                  */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function returns the distinguished names of all directory ob-    */
/*      jects which are available in the DUA-cache.                          */
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
#include <string.h>
#include <dce/d2dir.h>
#include <d23ipc.h>
#include <d23apdu.h>
#include <d22cache.h>

/* -------------------- declaration of global data ------------------------- */

extern signed32	d22_errno ;	/* error variable */
extern signed16 d22_ipcmsgsize ;/* max. size of a IPC-message buffer */

/* ------------------------------------------------------------------------- */

signed32 d22_clist (D22_admin_tab *admptr, D23_wtinpb *rqptr,
							    D23_rsrqpb *rsptr) {

    D23_inlist		*pbptr = (D23_inlist *) rqptr->d23_2rfidt ;
    D23_com_arg		*comptr = (D23_com_arg *) (pbptr + 1) ;
    D23_ds_name		*nmptr ;
    D22_hash_entry	*hshptr ;
    D22_attr_entry	*namptr ;
    char		tmpbuf[D2_NP_MAX * (sizeof (D22_attr_entry) +
							D2_ALIGN (D2_NP_LEN))] ;
    char		tmp1[D2_DNL_MAX] ;
    byte		*eptr, *maxeptr ;
    byte		*maxiptr = rqptr->d23_2rfidt + rqptr->d23_2lnidt ;
    signed16		dnlen, fndobjcnt, maxobcnt ;

    /* verify request parameter */
    if (rqptr->d23_2imdat != D23_LASTFG) {
	d22_errno = D22_OPTYPE_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		    svc_c_sev_warning, GDS_S_IPCMTYPIL_ERR, rqptr->d23_2imdat) ;
	return (D22_ERROR) ;
    }
    if (d22_check_par (pbptr->d23_Ghdr.d23_versno, pbptr->d23_Ghdr.d23_dirid,
		    (D23_ds_name *) NULL, (D22_naming_ref *) NULL) == D22_ERROR)
	return (D22_ERROR) ;

    /* verify common arguments */
    if (comptr->d23_cmtag != D23_CMTAG) {
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

    nmptr = (D23_ds_name *) (comptr + 1) ;
    D22_SKIPTAG (D23_ds_name, nmptr, D23_SECTAG, D23_S_SEP, 0) ;
    D22_SKIPTAG (D23_ds_name, nmptr, D23_RQTAG, D23_S_NM, nmptr->d23_dsoff) ;
    D22_SKIPTAG (D23_ds_name, nmptr, D23_BITTAG, D23_S_BIT,
					((D23_bit_info *)nmptr)->d23_boff) ;

    /* verify base object name (must be always root) */
    if (nmptr->d23_dstag != D23_NAMTAG) {
	d22_errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				nmptr->d23_dstag, D23_NAMTAG) ;
	return (D22_ERROR) ;
    }
    if (nmptr->d23_dsoff != 0 || nmptr->d23_dslen != 0) {
	d22_errno = D22_BASEOBJ_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				    svc_c_sev_warning, GDS_S_CACHE_INVDNS_ERR) ;
	return (D22_ERROR) ;
    }
    nmptr++ ;
    if ((D23_eom *)nmptr > (D23_eom *) (maxiptr - D23_S_EOM)) {
    }
    if (((D23_eom *)nmptr)->d23_eomtag != D23_EOMTAG) {
	d22_errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				((D23_eom *)nmptr)->d23_eomtag, D23_EOMTAG) ;
	return (D22_ERROR) ;
    }

    /* activate specific DUA-cache if required */
    if (pbptr->d23_Ghdr.d23_dirid != admptr->d22_act_cache_id &&
	    d22_activate_cache (pbptr->d23_Ghdr.d23_dirid, admptr) == D22_ERROR)
	return (D22_ERROR) ;

    /* generate default result parameter block */
    rsptr->d23_3hdinfo.d2_version = D23_V02 ;
    rsptr->d23_3assid = rqptr->d23_2assid ;
    rsptr->d23_3invinfo = rqptr->d23_2invinfo ;
    rsptr->d23_3rltid = D23_RESULT ;
    rsptr->d23_3rmdat = D23_MRDATA ;
    rsptr->d23_3rfrdt = eptr = rqptr->d23_2rfidt ;

    /* search all available directory objects and generate result message */
    maxeptr = eptr + d22_ipcmsgsize ;
    maxobcnt =  admptr->d22_desctab->d22_ract_entries +
		admptr->d22_desctab->d22_pact_entries +
		admptr->d22_desctab->d22_nact_entries ;

    for (fndobjcnt = 0, hshptr = admptr->d22_hshtab; fndobjcnt < maxobcnt;
								     hshptr++) {
	if (hshptr->d22_state == D22_NO_ENTRY)
	    continue ;
	if ((hshptr->d22_state & D22_INSTORED) == D22_INSTORED)
	    /* distinguished name is internally available */
	    namptr = (admptr->d22_iobjtab + (hshptr->d22_state &
						D22_IINDEXMASK))->d22_iattrptr ;
	else {
	    /* read distinguished name from DUA-cache attribute file */
	    if (d22_cfread (admptr->d22_attr_fd, (void *) tmpbuf,
			(size_t) (D2_NP_MAX * (sizeof (D22_attr_entry) +
		     D2_ALIGN (D2_NP_MAX))), hshptr->d22_oboffset) == D22_ERROR)
		return (D22_ERROR) ;
	    namptr = (D22_attr_entry *) tmpbuf ;
	}

	/* convert naming attributes into a distinguished name string */
	d22_convert_to_dn (tmp1, namptr, D2_NP_MAX * (sizeof (D22_attr_entry) +
							D2_ALIGN (D2_NP_LEN))) ;
	dnlen = strlen (tmp1) + 1 ;

	if (maxeptr - eptr < D23_S_SUBORD + D23_S_NM +
				    D2_ALIGN (dnlen) + D23_S_CRES + D23_S_EOM) {
	    /* send message to requestor */
	    rsptr->d23_3lnrdt = (signed16) (eptr - rsptr->d23_3rfrdt) ;
	    if (d23_result (rsptr) == D23_ERROR) {
		if (rsptr->d23_3hdinfo.d2_errvalue == D23_ABOERR)
		    return (D22_SPEC_ERROR) ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""),
		       GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_CACHE_RESULT_ERR) ;
		exit (1) ;
	    }
	    eptr = rsptr->d23_3rfrdt ;
	}
	fndobjcnt++ ;
	/* verify size limit */
	if (comptr->d23_cmctr.d23_scslm != D2_S_UNLIMITED &&
				      fndobjcnt > comptr->d23_cmctr.d23_scslm) {
	    ((D23_part_oq *)eptr)->d23_oqtag = D23_OQTAG ;
	    ((D23_part_oq *)eptr)->d23_oq_uncrit = FALSE ;
	    ((D23_part_oq *)eptr)->d23_oq_limit = D2_SIZE_LIMIT_EX ;
	    eptr += D23_S_POQ ;
	    break ;
	}
	/* insert subordinate information into result message */
	((D23_subord_info *) eptr)->d23_sotag = D23_SOITAG ;
	((D23_subord_info *) eptr)->d23_soali = FALSE ;
	((D23_subord_info *) eptr)->d23_somas = FALSE ;
	eptr += D23_S_SUBORD ;

	/* copy distinguished name into result message */
	((D23_ds_name *) eptr)->d23_dstag = D23_NAMTAG ;
	((D23_ds_name *) eptr)->d23_dscomplete = TRUE ;
	((D23_ds_name *) eptr)->d23_dslen = dnlen ;
	((D23_ds_name *) eptr)->d23_dsoff = D2_ALIGN (dnlen) ;
	strcpy ((char *) ((D23_ds_name *) eptr + 1), tmp1) ;
	eptr += D23_S_NM + ((D23_ds_name *) eptr)->d23_dsoff ;
    }
    ((D23_com_res *) eptr)->d23_crtag = D23_CRSTAG ;
    ((D23_com_res *) eptr)->d23_ali_deref = FALSE ;
    eptr += D23_S_CRES ;
    ((D23_eom *) eptr)->d23_eomtag = D23_EOMTAG ;
    eptr += D23_S_EOM ;
    rsptr->d23_3rmdat = D23_LASTFG ;
    rsptr->d23_3lnrdt = (signed16) (eptr - rsptr->d23_3rfrdt) ;

    return (D22_OK) ;
}
