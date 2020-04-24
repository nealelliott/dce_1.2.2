/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: omRead.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:50  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:39:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:49  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:21:32  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:19  devsrc]
 * 
 * Revision 1.1.2.2  1994/03/23  15:49:20  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:38:35  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:00:26  marrek
 * 	Beautification.
 * 	[1994/02/09  15:41:23  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  11:01:37  marrek]
 * 
 * $EndLog$
 */
#ifndef _OMREAD_H
#define _OMREAD_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char omread_rcsid[] = "@(#)$RCSfile: omRead.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:38:24 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code
omP_check_value_syntax(
	OMX_workspace	workspace,
	OM_syntax syntax);

OM_return_code
omP_xread(
	OM_private_object		subject,
	OM_type			type,
	OM_value_position		value_position,
	OM_boolean			local_string,
	OM_string_length		*element_position,
	OM_string			*elements
);
#endif /* ifndef _OMREAD_H */
