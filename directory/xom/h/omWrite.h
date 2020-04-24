/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: omWrite.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:02  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:39:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:52  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:21:41  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:23  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:00:31  marrek
 * 	Beautification.
 * 	[1994/02/09  15:41:51  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  11:02:35  marrek]
 * 
 * $EndLog$
 */
#ifndef _OMWRITE_H
#define _OMWRITE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char omwrite_rcsid[] = "@(#)$RCSfile: omWrite.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:38:28 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code
append_bit_strings(
	char *result,
	OM_sint	*result_length,
	char *s1,
	OM_sint	l1,
	char *s2,
	OM_sint	l2);
OM_return_code
omP_xwrite(
	OM_private_object		subject,
	OM_type			type,
	OM_value_position		value_position,
	OM_syntax			syntax,
	OM_string_length		*element_position,
	OM_string			elements
);
#endif /* ifndef _OMWRITE_H */
