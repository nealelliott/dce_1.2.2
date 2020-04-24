/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPPsap.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:26  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:34:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:01  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:19:56  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:43  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:23:12  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:09:58  marrek]
 * 
 * 	Beautification.
 * 	[1994/02/09  15:38:11  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:56:24  marrek]
 * 
 * $EndLog$
 */
#ifndef _DSPPSAP_H
#define _DSPPSAP_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char dsppsap_rcsid[] = "@(#)$RCSfile: dsPPsap.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:29 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


#include <osiaddr.h>

OM_return_code
dsP_encode_psap_addr (
	OMX_workspace	,
	Psap_info *,
	Psap_info *,
	Psap_info *,
	OM_sint	,
	Psap_info *,
	xds_psap	*,
	OM_sint	*);

OM_sint
dsP_decode_psap_addr (
	Psap_info	*p_sel,
	Psap_info *s_sel,
	Psap_info	*t_sel,
	OM_sint *no_nsap,
	Psap_info *nsap_info,
	xds_psap psap_addr);

OM_sint
dsP_get_psap_length(
	xds_psap psap_addr);

#endif /* ifndef _DSPPSAP_H */
