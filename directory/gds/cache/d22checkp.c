/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22checkp.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:31  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:26:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:17  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:24:45  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:12:50  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:00  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:16:40  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:02  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:51:51  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:24:11  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:12:15  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:40:13  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:12:56  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:13:46  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:05:50  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22checkp.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:35 $";
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
/* NAME         : d22checkp.c                                         */
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
/*  FUNCTION:  d22_check_par ()                                              */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function verfies the parameter 'version', 'directory identifier' */
/*      and 'distinguished name' (optional) of a service request call.       */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      version     = Version no. which must be verified.                    */
/*      dirid       = Directory Identifier which must be verified.           */
/*      name        = Pointer to the distinguished name which must be        */
/*                    verified (a NIL-Pointer means that no verification     */
/*                    must be done one a distinguished name).                */
/*      idnref      = Pointer to a structure holding information about the   */
/*                    converted distinguished name.                          */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*      idnref      = Information about the converted distinguished name     */
/*                    (only if the input parameters 'name' and 'idnref' are  */
/*                    both not equal to NULL).                               */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      D22_OK      = The function has been performed successfully.          */
/*      D22_ERROR   = One of the verified parameter is incorrect (the        */
/*                    variable 'errno' contains an error no.).               */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 15.12.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>

/* -------------------- declaration of global data ------------------------- */

extern signed32	d22_errno ;	/* error variable */

/* ------------------------------------------------------------------------- */

signed32 d22_check_par (signed16 version, signed16 dirid, D23_ds_name *name,
						       D22_naming_ref *idnref) {

    /* verify version number */
    if (!D2_CHECK_MINMAX (D23_APDUMIN, D23_APDUMAX, version)) {
	d22_errno = D22_VERSION_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			  svc_c_sev_warning, GDS_S_CACHE_INVVERS_ERR, version) ;
	return (D22_ERROR) ;
    }

    /* verify directory identifier */
    if (!D2_CHECK_MINMAX (D2MINDIR_ID, D2CACHE_MAXDIR_ID, dirid)) {
	d22_errno = D22_DIRID_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			   svc_c_sev_warning, GDS_S_CACHE_INVDIRID_ERR, dirid) ;
	return (D22_ERROR) ;
    }

    /* verify distinguished name */
    if (name != (D23_ds_name *) NULL) {
	if (name->d23_dstag != D23_NAMTAG) {
	    d22_errno = D22_MSYNTAX_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				name->d23_dstag, D23_NAMTAG) ;
	    return (D22_ERROR) ;
	} else
	    return (d22_verify_dn ((D2_name_string) (name + 1), idnref)) ;
    }

    return (D22_OK) ;
}
