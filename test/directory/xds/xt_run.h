/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xt_run.h,v $
 * Revision 1.1.4.2  1996/02/18  23:04:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:15  marty]
 *
 * Revision 1.1.4.1  1995/12/08  22:14:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:46:23  root]
 * 
 * Revision 1.1.2.4  1994/06/21  14:29:02  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  07:49:15  marrek]
 * 
 * Revision 1.1.2.3  1994/06/10  20:19:42  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:58:44  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:33:28  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/06  10:24:02  marrek]
 * 
 * Revision 1.1.2.1  1994/02/22  19:16:19  marrek
 * 	Beautification.
 * 	[1994/02/09  15:45:27  marrek]
 * 
 * 	Created for dce1.1 code drop.
 * 	[1994/02/09  11:30:48  marrek]
 * 
 * $EndLog$
 */
#ifndef _XT_RUN_H
#define _XT_RUN_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xt_run_rcsid[] = "@(#)$RCSfile: xt_run.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:04:19 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


int xt_main(int argc, char *argv[]);
OM_sint xt_run(void);
struct jump_struct *xt_sbyname(char *name);  /* Name of entry to find.*/
OM_sint xt_build_ava(OM_object ava, char value[], OM_sint num);
OM_sint xt_build_rdn(OM_object rdn, OM_object ava);
OM_sint xt_build_name(OM_descriptor name[], OM_object rdns[], OM_sint count, OM_sint first);
OM_sint xt_build_attribute_list(OM_object att_list, char *type);
#endif /* ifndef _XT_RUN_H */
