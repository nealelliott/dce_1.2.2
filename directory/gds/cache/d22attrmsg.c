/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22attrmsg.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:20  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:26:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:10  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:24:37  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:12:22  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:50:49  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:16:15  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  17:52:17  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:50:34  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:16:08  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:10:46  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:39:39  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:12:27  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:06:01  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  16:26:33  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:12:39  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:05:15  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22attrmsg.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:24 $";
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
/* NAME         : d22attrmsg.c                                        */
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
/*  FUNCTION:  d22_attrimsg ()                                               */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function searches in the given attribute information block of a  */
/*      directory object for requested attribute types. If an attribute type */
/*      is found in the attribute information block, the requested infor-    */
/*      mation about this attribute (attribute type only or attribute type   */
/*      and attribute value(s)) is transferred to a given message buffer and */
/*      stored there in IPC-format. If the complete requested informations   */
/*      about attributes cannot be stored in the message buffer, then the    */
/*      filled buffer is send to the service requestor and the buffer is     */
/*      filled again with the next attribute informations.                   */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      admptr      = Pointer to the DUA-cache administration table.         */
/*      eptr        = Address of a pointer to the message buffer where the   */
/*                    found attribute information can be inserted.           */
/*      rsptr       = Pointer to the IPC-return result parameter block       */
/*                    which must be used for sending the message buffer to   */
/*                    the requestor.                                         */
/*      attrptr     = Pointer to the attribute information block.            */
/*      attrsize    = Size of the attribute information block.               */
/*      rqattrno    = No. of the requested attribute types.                  */
/*      rqattrptr   = Pointer to an array which contains the requested       */
/*                    attribute types.                                       */
/*      rsmode      = Type of operation:                                     */
/*                    D2_R_TYP = return attribute type only.                 */
/*                    D2_R_VAL_TYP = return attribute type and value.        */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*      eptr        = Modified pointer which points to the end of the        */
/*                    inserted attribute information block.                  */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*                    No. of found and returned attributes.                  */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 11.1.88      */
/*exoff **********************************************************************/

#include <gds.h>
#include <string.h>
#include <dce/d2dir.h>
#include <d23ipc.h>
#include <d23apdu.h>
#include <d22cache.h>


/* ------------------ declaration of global data --------------------------- */

extern signed16	d22_ipcmsgsize ;	/* max. size of an IPC-message buffer */

/* ------------------------------------------------------------------------- */

signed32 d22_attrimsg (D22_admin_tab *admptr, byte **eptr, D23_rsrqpb *rsptr,
		  D22_attr_entry *attrptr, signed16 attrsize, signed16 rqattrno,
				      D2_obj_id *rqattrptr, D2_ret_val rsmode) {

    D22_attr_entry	*maxaptr = (D22_attr_entry *) ((byte *) attrptr +
								    attrsize ) ;
    D22_size_info	*hptr ;
    D2_obj_id		*attr_cptr ;
    signed16		i, fndattr_no = 0, dnlen ;
    byte		*acteptr = *eptr ;
    byte		*maxeptr = rsptr->d23_3rfrdt + d22_ipcmsgsize ;
    char		tmpdn[D2_DNL_MAX] ;

    rsptr->d23_3rltid = D23_RESULT ;
    rsptr->d23_3rmdat = D23_MRDATA ;

    /* convert naming attributes into a distinguished name string */
    d22_convert_to_dn (tmpdn, attrptr, attrsize) ;
    dnlen = strlen (tmpdn) + 1 ;

    /* search requested attribute types and put them into result message */
    while (attrptr < maxaptr && (fndattr_no < rqattrno || rqattrno == 0 ||
						  rqattrno == D2_ALL_ATT_REQ)) {
	if (rqattrno == D2_ALL_ATT_REQ &&
	   (attrptr->d22_aetype & D22_NAME_AFLAG) == D22_NAME_AFLAG) {
	    /* don't return naming attributes, if all attributes are requested */
	    attrptr = (D22_attr_entry *) ((byte *) attrptr +
			sizeof (D22_attr_entry) + attrptr->d22_asizes.d22_voff) ;
	    continue ;
	}
	if (rqattrno > 0) {
	    /* compare attribute type with all requested attribute types */
	    for (attr_cptr = rqattrptr, i = 0; i < rqattrno; i++, attr_cptr++)
		if (attrptr->d22_aidsize == attr_cptr->d2_typ_len &&
		    memcmp ((void *) attrptr->d22_aident,
			    (void *) attr_cptr->d2_type,
					    (size_t) attrptr->d22_aidsize) == 0)
		    break ;
	    if (i == rqattrno) {
		/* attribute type not found */
		attrptr = (D22_attr_entry *) ((byte *) attrptr +
		       sizeof (D22_attr_entry) + attrptr->d22_asizes.d22_voff) ;
		continue ;
	    }
	}

	if (fndattr_no == 0) {
	    /* first attribute found -> write DN into result buffer */
	    /* check for a full result message buffer */
	    if (maxeptr - acteptr < D23_S_EN + D23_S_NM +
				    D2_ALIGN (dnlen) + D23_S_CRES + D23_S_EOM) {
    		/* insufficient buffer space -> send message to requestor */
		rsptr->d23_3lnrdt = (signed16) (acteptr - rsptr->d23_3rfrdt) ;
		if (d23_result (rsptr) == D23_ERROR) {
		    if (rsptr->d23_3hdinfo.d2_errvalue == D23_ABOERR)
			return (D22_SPEC_ERROR) ;
		    dce_svc_printf (DCE_SVC (gds_svc_handle, ""),
		       GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_CACHE_RESULT_ERR) ;
		    exit (1) ;
		}
		acteptr = rsptr->d23_3rfrdt ;
	    }

	    ((D23_en_info *) acteptr)->d23_entag = D23_ENTTAG ;
	    ((D23_en_info *) acteptr)->d23_enmas = FALSE ;
	    ((D23_en_info *) acteptr)->d23_enmaxat = D2_AT_MAX ;
	    acteptr += D23_S_EN ;
		
	    /* insert distinguished name into result message */
	    ((D23_ds_name *) acteptr)->d23_dstag = D23_NAMTAG ;
	    ((D23_ds_name *) acteptr)->d23_dscomplete = TRUE ;
	    ((D23_ds_name *) acteptr)->d23_dslen = dnlen ;
	    ((D23_ds_name *) acteptr)->d23_dsoff = D2_ALIGN (dnlen) ;
	    strcpy ((char *) ((D23_ds_name *) acteptr + 1), tmpdn) ;
	    acteptr += D23_S_NM + ((D23_ds_name *) acteptr)->d23_dsoff ;
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s"), GDS_S_GENERAL,
				 svc_c_debug2, GDS_S_CACHE_OBJ_RESULT, tmpdn)) ;
	}
	if (rqattrno == 0) {
	    /* only the DN is requested */
	    fndattr_no++ ;
	    break ;
	}

	/* write requested attribute information into result buffer */
	/* check for a full result message buffer */
	if (maxeptr - acteptr < D23_S_ATT + D2_ALIGN (attrptr->d22_aidsize) +
						       D23_S_CRES + D23_S_EOM) {
	    /* insufficient buffer space -> send message to requestor */
	    rsptr->d23_3lnrdt = (signed16) (acteptr - rsptr->d23_3rfrdt) ;
	    if (d23_result (rsptr) == D23_ERROR) {
		if (rsptr->d23_3hdinfo.d2_errvalue == D23_ABOERR)
		    return (D22_SPEC_ERROR) ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""),
		       GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_CACHE_RESULT_ERR) ;
		exit (1) ;
	    }
	    acteptr = rsptr->d23_3rfrdt ;
	}
	/* insert attribute information into result message */
	((D23_at_type *) acteptr)->d23_atttag = D23_ATTTAG ;
	((D23_at_type *) acteptr)->d23_atoff = D2_ALIGN (attrptr->d22_aidsize) ;
	((D23_at_type *) acteptr)->d23_atlen = attrptr->d22_aidsize ;
	acteptr += D23_S_ATT ;
	memcpy ((void *) acteptr, (void *) attrptr->d22_aident,
						(size_t) attrptr->d22_aidsize) ;
	acteptr += D2_ALIGN (attrptr->d22_aidsize) ;
	if (rsmode != D2_R_TYP) {
	    ((D23_av_info *) acteptr)->d23_avtag = D23_AVTAG ;
	    ((D23_av_info *) acteptr)->d23_avnum = attrptr->d22_avalno ;
	    ((D23_av_info *) acteptr)->d23_asyntx = attrptr->d22_aidsyn ;
	    acteptr += D23_S_AV ;

	    /* copy attribute value(s) into result message buffer */
	    hptr = ((attrptr->d22_aetype & D22_REC_FLAG) != D22_REC_FLAG) ?
					(D22_size_info *) &attrptr->d22_asizes :
					(D22_size_info *) (attrptr + 1) ;
	    for (i = 0; i < attrptr->d22_avalno; i++) {
	        if (maxeptr - acteptr < D23_S_ATV + hptr->d22_voff +
						       D23_S_CRES + D23_S_EOM) {
		    /* insufficient buffer space -> send message to requestor */
		    rsptr->d23_3lnrdt = (signed16) (acteptr - rsptr->d23_3rfrdt) ;
		    if (d23_result (rsptr) == D23_ERROR) {
			if (rsptr->d23_3hdinfo.d2_errvalue == D23_ABOERR)
			    return (D22_SPEC_ERROR) ;
			dce_svc_printf (DCE_SVC (gds_svc_handle, ""),
						GDS_S_GENERAL, svc_c_sev_fatal,
						GDS_S_CACHE_RESULT_ERR) ;
			exit (1) ;
		    }
		    acteptr = rsptr->d23_3rfrdt ;
		}
		((D23_av_value *) acteptr)->d23_avoff = hptr->d22_voff ;
		((D23_av_value *) acteptr)->d23_avlen = hptr->d22_vlen ;
		((D23_av_value *) acteptr)->d23_avrep = hptr->d22_vrep ;
		acteptr += D23_S_ATV ;
		memcpy ((void *) acteptr, (void *) (hptr + 1),
						      (size_t) hptr->d22_vlen) ;
		acteptr += hptr->d22_voff ;
		hptr = (D22_size_info *) ((char *) hptr + 
				      sizeof (D22_size_info) + hptr->d22_voff) ;
	    }	
	}
	fndattr_no++ ;
	attrptr = (D22_attr_entry *) ((byte *) attrptr +
		       sizeof (D22_attr_entry) + attrptr->d22_asizes.d22_voff) ;
    }
    *eptr = acteptr ;
    rsptr->d23_3lnrdt = (signed16) (acteptr - rsptr->d23_3rfrdt) ;
    return (fndattr_no) ;
}
