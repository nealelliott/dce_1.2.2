/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: omCopyValu.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:34  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:38:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:44  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:21:23  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:12  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:00:19  marrek
 * 	Beautification.
 * 	[1994/02/09  15:40:23  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:59:26  marrek]
 * 
 * $EndLog$
 */
#ifndef _OMCOPYVALU_H
#define _OMCOPYVALU_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char omcopyvalu_rcsid[] = "@(#)$RCSfile: omCopyValu.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:38:16 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code
omP_xcopy_value(
	OM_private_object		source,
	OM_type			source_type,
	OM_value_position		source_value_position,
	OM_private_object		destination,
	OM_type			destination_type,
	OM_value_position		destination_value_position
);
#endif /* ifndef _OMCOPYVALU_H */
