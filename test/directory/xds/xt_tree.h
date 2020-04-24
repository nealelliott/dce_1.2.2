/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xt_tree.h,v $
 * Revision 1.1.4.2  1996/02/18  23:04:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:20  marty]
 *
 * Revision 1.1.4.1  1995/12/08  22:14:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:46:37  root]
 * 
 * Revision 1.1.2.3  1994/06/21  14:29:05  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  07:49:24  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  20:19:45  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:58:46  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:16:34  marrek
 * 	Beautification.
 * 	[1994/02/09  15:45:31  marrek]
 * 
 * 	Created for dce1.1 code drop.
 * 	[1994/02/09  11:31:03  marrek]
 * 
 * $EndLog$
 */
#ifndef _XT_TREE_H
#define _XT_TREE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xt_tree_rcsid[] = "@(#)$RCSfile: xt_tree.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:04:23 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_sint xt_build_tree (void);
OM_sint xt_add_entries (OM_sint last, char instr[30]);
OM_sint xt_remove_tree (void);
OM_sint xt_remove_entries (OM_sint last, char instr[30]);
#endif /* ifndef _XT_TREE_H */
