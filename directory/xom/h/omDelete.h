/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: omDelete.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:39  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:38:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:45  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:21:25  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:14  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:00:21  marrek
 * 	Beautification.
 * 	[1994/02/09  15:40:42  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:59:57  marrek]
 * 
 * $EndLog$
 */
#ifndef _OMDELETE_H
#define _OMDELETE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char omdelete_rcsid[] = "@(#)$RCSfile: omDelete.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:38:18 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code
omP_xdelete(
	OM_object  subject
);
void
delete_desc_value(
	OMX_workspace	workspace,
	OM_descriptor *desc_ptr);
#endif /* ifndef _OMDELETE_H */
