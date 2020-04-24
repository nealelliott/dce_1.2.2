/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22vrfydn.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:04  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:28:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:37  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:25:09  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:14:23  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:27  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:17:40  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:42  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:55:43  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:50:40  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:16:23  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:42:19  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:14:30  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:32:04  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:38:42  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22vrfydn.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:03 $";
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
/* NAME         : d22vrfydn.c                                         */
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
/*  FUNCTION:  d22_verify_dn ()                                              */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function verifies a given distinguished name for validity.       */
/*      This means, all distinguished name parts must be IA5-strings,        */
/*      all attribute identifiers must be in the predefined range and the    */
/*      distinguished name syntax must meet the form:                        */
/*                                                                           */
/*           <RDN1>DL1<RDN2>DL1< ... >DL1<RDNn>EOS                           */
/*                                                                           */
/*                  with  RDNx = <AVA1>DL2<AVA2>DL2< ... >DL2<AVAm>          */
/*                        with AVAx = <type>DL3<namepart value>              */
/*                                                                           */
/*                                               DL[1-3], EOS  = delimiter   */
/*                                                                           */
/*      No verification is done on the validity of a discrete attribute      */
/*      identifier value (type) and on the validity of the sequence at which */
/*      the distinguished name parts occur, because no directory scheme is   */
/*      available.                                                           */
/*      Simultaneous to the verification the given distinguished name string */
/*      is converted into the DUAC-cache internally used attribute format    */
/*      (only if the input parameter 'idnref' is set to an adequate value).  */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      name        = Pointer to the distinguished name-string.              */
/*      idnref      = Pointer to a structure where reference information     */
/*                    about the converted distinguished name is given        */
/*                    (means a pointer to an area where the converted name   */
/*                    should be stored) resp. can be returned (means the     */
/*                    no. of name parts and the size of the converted name   */
/*                    information block).                                    */
/*                    (Note: no conversion is done if idnref is 'NULL').     */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*      idnref      = See description above.                                 */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      D22_OK      = The function has been performed successfully (the      */
/*                    distinguished name fulfills the requirements).         */
/*      D22_ERROR   = An error was detected during verification (the         */
/*                    variable 'd22_errno' contains an error number).        */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE:  3.12.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>

/* -------------------- declaration of global data ------------------------- */

extern signed32	d22_errno ;	/* error variable */

/* ------------------------------------------------------------------------- */

signed32 d22_verify_dn (D2_name_string name, D22_naming_ref *idnref) {

    signed16		cnt, i, size ;
    signed16		idnparts = 0, rdnflag = D22_RDN_FLAG ;
    boolean		type_mode = TRUE ;
    char		*infoptr ;
    char		objidstr[D2_NP_ID_LEN], objidv[D2_OBJ_IDL_MAX] ;
    Octet_string	p = (Octet_string) name, p1 = (Octet_string) name;
    D22_attr_entry	*idnptr ;
    D2_obj_id		objid ;

    /* sort AVA's of RDN's */
    if (d27_018_sort_name (name) == D2_ERROR) {
	/* error -> wrong syntax */
	d22_errno = D22_NMSYNTAX_ERR ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s"), GDS_S_GENERAL,
				  svc_c_debug1, GDS_S_CACHE_DN_SYN_ERR, name)) ;
	return (D22_ERROR) ;
    }

    /* remove unused blanks from the DN */
    while (*p) {
	if (*p == D2_BLANK && (*(p + 1) == D2_BLANK ||
	   ((*(p1 - 1) == D2_T61_REP || *(p1 - 1) == D2_PRTBL_REP ||
	     *(p1 - 1) == D2_NUM_REP || *(p1 - 1) == D2_IA5_REP) &&
	     *(p + 1)  != D2_RDN_SEP && *(p + 1)  != D2_AVA_SEP &&
	     *(p + 1)  != D2_T61_REP && *(p + 1)  != D2_PRTBL_REP &&
	     *(p + 1)  != D2_NUM_REP && *(p + 1)  != D2_IA5_REP &&
					*(p + 1)  != D2_EOS) ||
	   (*(p1 - 1)  != D2_T61_REP && *(p1 - 1) != D2_PRTBL_REP &&
	    *(p1 - 1)  != D2_NUM_REP && *(p1 - 1) != D2_IA5_REP &&
	   (*(p + 1)   == D2_RDN_SEP || *(p + 1)  == D2_AVA_SEP ||
	    *(p + 1)   == D2_T61_REP || *(p + 1)  == D2_PRTBL_REP ||
	    *(p + 1)   == D2_NUM_REP || *(p + 1)  == D2_IA5_REP ||
					*(p + 1)  == D2_EOS))))
	    p++ ;
	else
	    *p1++ = *p++ ;
    }
    *p1 = D2_EOS ;

    if (idnref != (D22_naming_ref *) NULL)
	idnptr = idnref->d22_nmaptr ;
	
    p = (Octet_string) name ;
    if ((size = strlen ((char *) p)) == 0 || size >= D2_DNL_MAX) {
	/* error -> wrong size of DN */
	d22_errno = D22_DNSIZE_ERR ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s"), GDS_S_GENERAL,
				   svc_c_debug1, GDS_S_CACHE_DN_SZ_ERR, name)) ;
	return (D22_ERROR) ;
    }
    /* verify distinguished name for syntax and IA5 legality */
    for (cnt = 0, i = 0; i <= size; i++, cnt++, p++) {
	if (cnt == 0)
	    infoptr = (char *) p ;
	if (*p >= D2_RGMIN && *p <= D2_RGMAX) {
	    if (type_mode && ((*p < D2_RG9 || *p > D2_RG10) && *p != D2_DL2)) {
		/* error -> wrong syntax */
		d22_errno = D22_NMSYNTAX_ERR ;
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s"), GDS_S_GENERAL,
				  svc_c_debug1, GDS_S_CACHE_DN_SYN_ERR, name)) ;
		return (D22_ERROR) ;
	    }
	} else {
	    if (*p == D2_RDN_SEP   || *p == D2_AVA_SEP || *p == D2_T61_REP ||
		*p == D2_PRTBL_REP || *p == D2_NUM_REP || *p == D2_IA5_REP ||
								 *p == D2_EOS) {
		if (idnparts >= D2_NP_MAX ||
		    (type_mode && (cnt < 1 || cnt > D2_NP_ID_LEN)) ||
			    (!type_mode && (cnt < 1 || cnt > D2_NP_LEN))) {
		    /* error -> wrong syntax */
		    d22_errno = D22_NMSYNTAX_ERR ;
		    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s"), GDS_S_GENERAL,
				  svc_c_debug1, GDS_S_CACHE_DN_SYN_ERR, name)) ;
		    return (D22_ERROR) ;
		}
		/* convert name part information to internally used format */
		if (type_mode) {
		    sprintf (objidstr, "%.*s", cnt, infoptr) ;
		    objid.d2_type = (Ob_id_string) objidv ;
		    if (d27_302_str_oid (objidstr, &objid) == D2_ERROR) {
			/* error -> wrong identifier syntax */
			d22_errno = D22_NMSYNTAX_ERR ;
			DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s"),
				GDS_S_GENERAL, svc_c_debug1,
				GDS_S_CACHE_DN_SYN_ERR, name)) ;
			return (D22_ERROR) ;
		    }
		    if (idnref != (D22_naming_ref *) NULL) {
			idnptr->d22_aetype = D22_NAME_AFLAG | rdnflag ;
			idnptr->d22_aidsize = objid.d2_typ_len ;
			idnptr->d22_aidsyn = (*p == D2_T61_REP) ? D2_T61 :
					     (*p == D2_IA5_REP) ? D2_IA5 :
					     (*p == D2_NUM_REP) ? D2_NUMERIC :
								  D2_PRINTABLE ;
			idnptr->d22_avalno = 1 ;
			(void) memcpy ((void *) idnptr->d22_aident,
				   (void *) objidv, (size_t) objid.d2_typ_len) ;
		    }	
		} else {
		    if (idnref != (D22_naming_ref *) NULL) {
			idnptr->d22_asizes.d22_vlen = cnt ;
			idnptr->d22_asizes.d22_voff = D2_ALIGN (cnt) ;
			(void) memcpy ((void *) ++idnptr, (void *) infoptr,
								 (size_t) cnt) ;
			idnptr = (D22_attr_entry *) ((char *) idnptr +
							       D2_ALIGN (cnt)) ;
		    }
		    idnparts++ ;
		}
		type_mode = (type_mode) ? FALSE : TRUE ;
		if (*p == D2_RDN_SEP)
		    rdnflag = (rdnflag) ? 0 : D22_RDN_FLAG ;
		cnt = -1 ;
	    } else {
		/* error -> not an IA5 character */
		d22_errno = D22_CHRANGE_ERR ;
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s"), GDS_S_GENERAL,
				  svc_c_debug1, GDS_S_CACHE_DN_ICH_ERR, name)) ;
		return (D22_ERROR) ;
	    }
	}
    }
    if (!type_mode) {
	/* error -> wrong syntax */
	d22_errno = D22_NMSYNTAX_ERR ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s"), GDS_S_GENERAL,
				  svc_c_debug1, GDS_S_CACHE_DN_SYN_ERR, name)) ;
	return (D22_ERROR) ;
    }
    if (idnref != (D22_naming_ref *) NULL) {
	idnref->d22_nonp = idnparts ;
	idnref->d22_nmsize = (short) ((char *) idnptr -
						  (char *) idnref->d22_nmaptr) ;
    }
    return (D22_OK) ;
}
