/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: omInst.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:45  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:38:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:48  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:21:29  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:17  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:00:24  marrek
 * 	Beautification.
 * 	[1994/02/09  15:41:04  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  11:01:00  marrek]
 * 
 * $EndLog$
 */
#ifndef _OMINST_H
#define _OMINST_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char ominst_rcsid[] = "@(#)$RCSfile: omInst.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:38:22 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code
omP_xinstance(
	OM_object			subject,
	OM_object_identifier	class,
	OM_boolean			*instance
);
#endif /* ifndef _OMINST_H */
