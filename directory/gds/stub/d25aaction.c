/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25aaction.c,v $
 * Revision 1.1.10.2  1996/02/18  18:23:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:05  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:14:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:20:38  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:26:23  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:24:08  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  16:03:21  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:12:04  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  17:55:06  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:50:37  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  08:20:04  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:17:51  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  22:03:27  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:47:32  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  18:26:46  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  15:07:38  marrek]
 * 
 * Revision 1.1.2.3  1992/07/01  16:53:34  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:22:38  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  23:08:23  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:29:54  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25aaction.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:23:08 $";
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
/*   NAME:    d25aaction.c                                                   */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       This module contains all actions (functions) belonging to the       */
/*       ASN.1-encoding resp. ASN.1-decoding  within the C-Stub.             */
/*       The following functions are available:                              */
/*                                                                           */
/*          * d25_initasn1   - initialize ASN.1-encoding/ASN.1-decoding      */
/*          * d25_apdutoasn1 - convert APDU-message to an ASN.1-message      */
/*          * d25_asn1toapdu - convert ASN.1-message to an APDU-message      */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 20.6.91     */
/*datoff *********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <ros.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d25stub.h>
#include <d2asn1.h>
#include <dce/asn1.h>



/* --------------------- INITIALIZE ASN.1-ENCODING/DECODING ---------------- */

signed32 d25_initasn1 (D25_actionpb *actpb) {

    D25_stubinfo	*sinfptr = actpb->d25_asinforef ;
    char		attr_file[D27_LFILE_NAME] ;

    /* read attribute description file */
    sprintf (attr_file, "%s%s", sinfptr->d25_dirpathref, D27_CONF_DIR) ;
    if (d27_307_init_att (attr_file) == D2_ERROR) {
	/* unable to read the attribute description file */
        dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
					  svc_c_sev_fatal, GDS_S_ATTRFILE_ERR) ;
	return (D25_AERROR) ;
    }

    return (D25_ACONT) ;
}

/* --------------------- ASN.1-ENCODING ------------------------------------ */

signed32 d25_apdutoasn1 (D25_actionpb *actpb) {

    D25_stubinfo	*sinfptr = actpb->d25_asinforef ;
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;
    D25_rosentry	*rosptr = actpb->d25_arosref ;
    signed32		result ;
    asn1_field		asn1info ;

    if (ipcptr->d25_ioprid != D23_BIND && rosptr->d25_rtransid == D27_PRIV_TS)
	/* ASN.1-encoding not required */
	return (D25_ACONT) ;

    /* encode APDU-message */
    result = d2a000_apdu_asn1 (ipcptr->d25_ioprid, ipcptr->d25_iudat.d25_datref,
		&ipcptr->d25_iudat.d25_datlen, &asn1info, ipcptr->d25_ibufref) ;

    if ((result == D2_ERROR ||
	ipcptr->d25_iudat.d25_datref != (byte *) asn1info.v) &&
	(ipcptr->d25_iudat.d25_datref < sinfptr->d25_imnref ||
			  ipcptr->d25_iudat.d25_datref > sinfptr->d25_imxref)) {
	/* release dynamically allocated buffer */
	free ((void *) ipcptr->d25_iudat.d25_datref) ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
	      svc_c_debug1, GDS_S_REL_SRBUFFER, ipcptr->d25_iudat.d25_datref)) ;
    }
    if (result == D2_ERROR) {
	/* unable to do ASN.1-encoding */
	ipcptr->d25_iresid = D23_RERROR ;
	ipcptr->d25_iudat.d25_datref = ipcptr->d25_ibufref ;
	ipcptr->d25_iudat.d25_datlen = D23_S_RSERR + D23_S_EOM ;
	return (D25_ANSUC) ;
    } else {
	if (ipcptr->d25_iudat.d25_datref != (byte *) asn1info.v) {
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
		    svc_c_debug1, GDS_S_ALO_SRBUFFER, asn1info.v, asn1info.l)) ;
	}
	ipcptr->d25_iudat.d25_datref = (byte *) asn1info.v ;
	ipcptr->d25_iudat.d25_datlen = asn1info.l ;
    }
    return (D25_ACONT) ;
}

/* --------------------- ASN.1-DECODING ------------------------------------ */

signed32 d25_asn1toapdu (D25_actionpb *actpb) {

    D25_stubinfo	*sinfptr = actpb->d25_asinforef ;
    D25_ipcentry	*ipcptr = actpb->d25_aipcref ;
    D25_rosentry	*rosptr = actpb->d25_arosref ;
    signed32		result ;
    asn1_field		asn1info ;

    if (rosptr->d25_rtransid == D27_PRIV_TS)
	/* ASN.1-decoding not required */
	return ((actpb->d25_astate == D25_IINVOKE_PENDING) ?
							D25_ASKIP : D25_ACONT) ;

    asn1info.v = ipcptr->d25_iudat.d25_datref ;
    asn1info.l = ipcptr->d25_iudat.d25_datlen ;
    if (ipcptr->d25_iresid == D23_RESULT) {
	/* decode ASN.1-result message */
	result = d2a001_asn1_apdu (ipcptr->d25_ioprid + D2_RES_OPID,
		&ipcptr->d25_iudat.d25_datref, &ipcptr->d25_iudat.d25_datlen,
					       &asn1info, ipcptr->d25_ibufref) ;

    } else {
	if (ipcptr->d25_iresid >= D23_RREJECT &&
			ipcptr->d25_iresid <= D23_RREJECT + D23_MXRREJ) {
	    if (actpb->d25_astate != D25_IBIND_PENDING)
		return (D25_ACONT) ;
	    /* decode ASN.1-reject message */
	    result = d2a001_asn1_apdu (ipcptr->d25_ioprid + D2_REJ_OPID,
					&ipcptr->d25_iudat.d25_datref,
					&ipcptr->d25_iudat.d25_datlen,
					&asn1info, ipcptr->d25_ibufref) ;
	} else {
	    /* decode ASN.1-error message */
	    result = d2a047_asn_error_apdu (&ipcptr->d25_iudat.d25_datref,
					&ipcptr->d25_iudat.d25_datlen,
					ipcptr->d25_iresid - D23_RERROR,
					rosptr->d25_rabsid, &asn1info,
							  ipcptr->d25_ibufref) ;
	}
    }
    if (asn1info.v != (asn1) NULL &&
	(asn1info.v < (asn1) sinfptr->d25_imnref ||
				    asn1info.v > (asn1) sinfptr->d25_imxref)) {
	/* release dynamically allocated buffer */
	free ((void *) asn1info.v) ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
				svc_c_debug1, GDS_S_REL_SRBUFFER, asn1info.v)) ;
    }
    if (result == D2_ERROR) {
	/* unable to do ASN.1-decoding */
	if (actpb->d25_astate == D25_IBIND_PENDING) {
	    ipcptr->d25_iresid = D23_ABORT ;
	    ipcptr->d25_iudat.d25_datref = (byte *) NULL ;
	    ipcptr->d25_iudat.d25_datlen = 0 ;
	    return (D25_ANSUC) ;
	} else {
	    ipcptr->d25_iresid = D23_RERROR ;
	    ipcptr->d25_ierrejval = (actpb->d25_aevent == D25_RRESULT_EV) ?
						    D23R_MRESULT : D23R_MERROR ;
	    ipcptr->d25_iudat.d25_datref = ipcptr->d25_ibufref ;
	    ipcptr->d25_iudat.d25_datlen = D23_S_RSERR + D23_S_EOM ;
	    if (actpb->d25_astate == D25_IINVOKE_PENDING &&
		((D23_rserror *) ipcptr->d25_ibufref)->d23_Zerrval !=
								 D2_NO_RESPONSE)
		return (D25_ASKIP) ;
	}
    } else {
	if (ipcptr->d25_iudat.d25_datref != (byte *) NULL) {
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
		svc_c_debug1, GDS_S_ALO_SRBUFFER, ipcptr->d25_iudat.d25_datref,
						ipcptr->d25_iudat.d25_datlen)) ;
	}
	if (actpb->d25_astate == D25_IINVOKE_PENDING)
	    return (D25_ASKIP) ;
    }
    return (D25_ACONT) ;
}
