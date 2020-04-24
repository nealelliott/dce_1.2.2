/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: omCopy.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:32  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:38:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:43  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:21:21  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:11  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:00:18  marrek
 * 	Beautification.
 * 	[1994/02/09  15:40:16  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:59:06  marrek]
 * 
 * $EndLog$
 */
#ifndef _OMCOPY_H
#define _OMCOPY_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char omcopy_rcsid[] = "@(#)$RCSfile: omCopy.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:38:15 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code
omP_xcopy(
	OM_private_object		original,
	OM_workspace		workspace,
	OM_private_object		*copy
);
#endif /* ifndef _OMCOPY_H */
