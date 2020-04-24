/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPAcl.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:04  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:34:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:53  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:19:10  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:34  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:22:57  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:09:39  marrek]
 * 
 * 	Beautification.
 * 	[1994/02/09  15:37:10  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:55:21  marrek]
 * 
 * $EndLog$
 */
#ifndef _DSPACL_H
#define _DSPACL_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char dspacl_rcsid[] = "@(#)$RCSfile: dsPAcl.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:21 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code
dsP_decode_aci (
	Aci_info *ac_i,
	byte *aci_p,
	OM_sint	*aci_l,
	OM_sint	*aci_n);

OM_return_code
dsP_decode_acr (
	Acr_info *ac_r,
	byte *acr_p,
	OM_sint	*acr_l,
	OM_sint	*acr_nval);

OM_return_code
dsP_decode_acl (
	Acr_info *ac_r_l[],
	byte *acl_p,
	OM_sint	*acl_l,
	OM_sint	*acl_nval);

OM_return_code
dsP_encode_aci (
	Aci_info *ac_i,
	byte **aci_p);

OM_return_code
dsP_encode_acr (
	Acr_info *ac_r,
	byte **acr_p);

OM_return_code
dsP_encode_acl (
	Acr_info	*ac_r_l[],
	byte **acl_p);

OM_sint
dsP_get_acl_length(
	xds_acl acl);

#endif /* ifndef _DSPACL_H */
