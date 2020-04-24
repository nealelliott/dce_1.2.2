/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsTrace.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:49  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:35:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:07  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:20:12  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:52  devsrc]
 * 
 * Revision 1.1.2.2  1994/03/23  15:49:00  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:37:23  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:23:27  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:10:19  marrek]
 * 
 * 	Beautification.
 * 	[1994/02/09  15:39:16  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:57:41  marrek]
 * 
 * $EndLog$
 */
#ifndef _DSTRACE_H
#define _DSTRACE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char dstrace_rcsid[] = "@(#)$RCSfile: dsTrace.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:38 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

#define DSP_ERROR_STRING_LEN 51
extern const char *dsP_error_string[DSP_ERROR_STRING_LEN];

void 
dsP_trace_init(
    void
);
void 
dsP_trace_msg (
    const OM_sint  tid,
    const char *fmt,
    ...
);
OM_sint
dsP_display_object(
	const OM_sint       tid,
	const OM_object arg,
	const char     *arg_str
);

OM_sint
dsP_display_feature(
	const  DS_feature *f,
	const  char *arg
);

OM_sint
dsP_display_integer(
	const  OM_sint i,
	const  char *arg
);

OM_sint
dsP_display_boolean(
 const OM_sint b,
 const char *arg
);

OM_sint
dsP_print_descrs(
	const OM_sint       indent,
	const OM_object desc
);

OM_sint
dsP_print_oid(
	const OM_string *str
);

#endif /* ifndef _DSTRACE_H */
