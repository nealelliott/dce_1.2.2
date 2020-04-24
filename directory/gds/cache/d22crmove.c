/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22crmove.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:39  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:27:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:22  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:24:51  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:13:17  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:09  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:16:58  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:14  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:52:44  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:29:53  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:13:15  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:40:37  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:13:19  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:07:39  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  16:28:22  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:14:28  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:06:20  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22crmove.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:42 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d22crmove.c                                         */
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
/*  FUNCTION:  d22_crmove ()                                                 */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function removes a given directory object from the DUA-cache.    */
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
/*  AUTHOR: R.Horn                                        DATE: 16.12.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <dce/d2dir.h>
#include <d23ipc.h>
#include <d23apdu.h>
#include <d22cache.h>

/* -------------------- declaration of global data ------------------------- */

extern signed32	d22_errno ;	/* error variable */

/* ------------------------------------------------------------------------- */

signed32  d22_crmove (D22_admin_tab *admptr, D23_wtinpb *rqptr,
							    D23_rsrqpb *rsptr) {

    D23_inremobjt	*pbptr = (D23_inremobjt *) rqptr->d23_2rfidt ;
    D23_com_arg		*comptr = (D23_com_arg *) (pbptr + 1) ;
    D23_ds_name		*nmptr ;
    D23_eom		*eomptr ;
    D22_hash_entry	*hshptr ;
    signed16		index, stclass ;
    byte		*maxiptr = rqptr->d23_2rfidt + rqptr->d23_2lnidt ;

    /* verify request parameter */
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

    nmptr = (D23_ds_name *) (comptr + 1) ;
    D22_SKIPTAG (D23_ds_name, nmptr, D23_SECTAG, D23_S_SEP, 0) ;
    D22_SKIPTAG (D23_ds_name, nmptr, D23_RQTAG, D23_S_NM, nmptr->d23_dsoff) ;
    D22_SKIPTAG (D23_ds_name, nmptr, D23_BITTAG, D23_S_BIT,
					  ((D23_bit_info *)nmptr)->d23_boff) ;

    if (d22_check_par (pbptr->d23_Phdr.d23_versno, pbptr->d23_Phdr.d23_dirid,
				   nmptr, (D22_naming_ref *) NULL) == D22_ERROR)
	return (D22_ERROR) ;

    eomptr = (D23_eom *) ((char *) nmptr + D23_S_NM + nmptr->d23_dsoff) ;
    if (eomptr > (D23_eom *) (maxiptr - D23_S_EOM)) {
	d22_errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				 svc_c_sev_warning, GDS_S_CACHE_INCONSMSG_ERR) ;
	return (D22_ERROR) ;
    }
    if (eomptr->d23_eomtag != D23_EOMTAG) {
	d22_errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				eomptr->d23_eomtag, D23_EOMTAG) ;
	return (D22_ERROR) ;
    }

    /* activate specific DUA-cache if required */
    if (pbptr->d23_Phdr.d23_dirid != admptr->d22_act_cache_id &&
	    d22_activate_cache (pbptr->d23_Phdr.d23_dirid, admptr) == D22_ERROR)
	return (D22_ERROR) ;

    /* search directory object */
    if (d22_search_obj (admptr, (D2_name_string) (nmptr + 1), &hshptr, &index,
							    FALSE) == D22_ERROR)
	return (D22_ERROR) ;

    /* remove directory object */
    if ((hshptr->d22_state & D22_STCLMASK) == D22_RESENTRY)
	stclass = D22_RESIDENT ;
    else 
	stclass =((hshptr->d22_state & D22_STCLMASK) == D22_PRIENTRY) ?
						   D22_PRIVILEGED : D22_NORMAL ;

    if (d22_delete_obj (stclass, admptr, hshptr, index, TRUE) == D22_ERROR)
	return (D22_ERROR) ;

    /* generate specific result parameter block */
    rsptr->d23_3rmdat = D23_LASTFG ;
    rsptr->d23_3rfrdt = rqptr->d23_2rfidt ;
    rsptr->d23_3lnrdt = sizeof (D23_eom) ;
    ((D23_eom *) (rsptr->d23_3rfrdt))->d23_eomtag = D23_EOMTAG ;

    return (D22_OK) ;
}
