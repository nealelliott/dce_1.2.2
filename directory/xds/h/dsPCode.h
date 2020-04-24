/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPCode.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:07  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:34:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:54  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:19:12  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:35  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:22:59  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:09:41  marrek]
 * 
 * 	Beautification.
 * 	[1994/02/09  15:37:17  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:55:29  marrek]
 * 
 * $EndLog$
 */
#ifndef _DSPCODE_H
#define _DSPCODE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char dspcode_rcsid[] = "@(#)$RCSfile: dsPCode.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:22 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code
dsP_decode_switch (
	const OM_private_object		encoding,
	OM_private_object	*original
);
OM_return_code
dsP_encode_switch (
	const OM_private_object		original,
	const OM_object_identifier	rules,
	OM_private_object			*encoding
);
#endif /* ifndef _DSPCODE_H */
