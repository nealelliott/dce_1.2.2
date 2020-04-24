/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xt_len.h,v $
 * Revision 1.1.6.2  1996/02/18  23:04:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:56  marty]
 *
 * Revision 1.1.6.1  1995/12/08  22:13:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:53  root]
 * 
 * Revision 1.1.4.2  1994/05/10  15:33:13  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/06  10:22:35  marrek]
 * 
 * Revision 1.1.4.1  1994/02/22  19:15:54  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:29:18  marrek]
 * 
 * Revision 1.1.2.2  1993/08/12  12:24:04  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:26:39  marrek]
 * 
 * $EndLog$
 */
#ifndef _XT_LEN_H
#define _XT_LEN_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xt_len_rcsid[] = "@(#)$RCSfile: xt_len.h,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 23:04:09 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/* contains lengths that must be consistent between
	xt_parms.h and xt_thread */

#define XT_LEN_SESSION_TBL			27
#define XT_LEN_PRIVATE_OBJECT_TBL	32
#define XT_LEN_PUBLIC_OBJECT_TBL 	11
#define XT_LEN_WORKSPACE_TBL		 3 
#define XT_LEN_STRING_TBL			12

/* must be consistent with xt_parms.h */
#define PRIVATE_RESULT_INDEX		31
#endif /* _XT_LEN_H */
