/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xt_dsFuncs.h,v $
 * Revision 1.1.4.2  1996/02/18  23:04:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:50  marty]
 *
 * Revision 1.1.4.1  1995/12/08  22:13:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:45  root]
 * 
 * Revision 1.1.2.3  1994/06/21  14:28:45  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  07:47:28  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  20:19:37  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:58:38  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:15:42  marrek
 * 	Beautification.
 * 	[1994/02/09  15:45:19  marrek]
 * 
 * 	Created for dce1.1 code drop.
 * 	[1994/02/09  11:30:15  marrek]
 * 
 * $EndLog$
 */
#ifndef _XT_DSFUNCS_H
#define _XT_DSFUNCS_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xt_dsfuncs_rcsid[] = "@(#)$RCSfile: xt_dsFuncs.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:04:04 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_sint xt_ds_abandon (void);
OM_sint xt_ds_add_entry (void);
OM_sint xt_ds_bind (void);
OM_sint xt_ds_compare (void);
OM_sint xt_dsX_extract_attr_values (void);
OM_sint xt_ds_initialize (void);
OM_sint xt_ds_list (void);
OM_sint xt_ds_modify_entry (void);
OM_sint xt_ds_modify_rdn (void);
OM_sint xt_ds_read (void);
OM_sint xt_ds_receive_result (void);
OM_sint xt_ds_remove_entry (void);
OM_sint xt_ds_search (void);
OM_sint xt_ds_shutdown (void);
OM_sint xt_ds_unbind (void);
OM_sint xt_ds_version (void);
#endif /* ifndef _XT_DSFUNCS_H */
