/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: omCode.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:30  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:38:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:42  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:20:45  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:09  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:00:17  marrek
 * 	Beautification.
 * 	[1994/02/09  15:40:09  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:58:45  marrek]
 * 
 * $EndLog$
 */
#ifndef _OMCODE_H
#define _OMCODE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char omcode_rcsid[] = "@(#)$RCSfile: omCode.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:38:14 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code
omP_xencode	(
	OM_private_object	original,
	OM_object_identifier	rules,
	OM_private_object		*encoding
);
OM_return_code
omP_xdecode	(
	OM_private_object		encoding,
	OM_private_object		*original
);
#endif /* ifndef _OMCODE_H */
