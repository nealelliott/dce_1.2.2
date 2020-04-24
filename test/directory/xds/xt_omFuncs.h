/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xt_omFuncs.h,v $
 * Revision 1.1.4.2  1996/02/18  23:04:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:02  marty]
 *
 * Revision 1.1.4.1  1995/12/08  22:13:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:46:02  root]
 * 
 * Revision 1.1.2.3  1994/06/21  14:28:51  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  07:47:53  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  20:19:39  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:58:41  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:16:01  marrek
 * 	Beautification.
 * 	[1994/02/09  15:45:23  marrek]
 * 
 * 	Created for dce1.1 code drop.
 * 	[1994/02/09  11:30:31  marrek]
 * 
 * $EndLog$
 */
#ifndef _XT_OMFUNCS_H
#define _XT_OMFUNCS_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xt_omFuncs_rcsid[] = "@(#)$RCSfile: xt_omFuncs.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:04:11 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_sint xt_om_copy (void);
OM_sint xt_om_copy_value (void);
OM_sint xt_om_create (void);
OM_sint xt_om_decode (void);
OM_sint xt_om_delete (void);
OM_sint xt_om_encode (void);
OM_sint xt_om_get (void);
OM_sint xt_om_instance (void);
OM_sint xt_om_put (void);
OM_sint xt_om_read (void);
OM_sint xt_om_remove (void);
OM_sint xt_om_write (void);
OM_sint xt_omX_fill (void);
OM_sint xt_omX_fill_oid (void);
OM_sint xt_omX_extract (void);
OM_sint xt_omX_string_to_object (void);
OM_sint xt_omX_object_to_string (void);
char    *xt_om_error_to_string(OM_sint i);
OM_return_code
xt_to_string_and_back(
			OM_object *pobject,
			const char *object_name);
char *OM_to_normal_string(
OM_string string);
#endif /* ifndef _XT_OMFUNCS_H */
