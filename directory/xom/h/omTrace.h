/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: omTrace.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:58  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:39:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:51  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:21:36  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:21  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:00:29  marrek
 * 	Beautification.
 * 	[1994/02/09  15:41:36  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  11:02:08  marrek]
 * 
 * $EndLog$
 */
#ifndef _OMTRACE_H
#define _OMTRACE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char omtrace_rcsid[] = "@(#)$RCSfile: omTrace.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:38:26 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code
omP_set_trace(void);
void
omP_trace_create(
	FILE			*trc_file,
	char 			*file,
	OM_sint				line,
	OM_private_object	object);
void
omP_trace_encode(
	FILE			*trc_file,
	char *file,
	OM_sint	line,
	OM_private_object	object);
void
omP_trace_decode(
	FILE			*trc_file,
	char *file,
	OM_sint	line,
	OM_private_object	object);
void
omP_trace_copy(
	FILE			*trc_file,
	char *file,
	OM_sint	line,
	OM_private_object	object);
void
omP_trace_get(
	FILE			*trc_file,
	char *file,
	OM_sint	line,
	OM_public_object	object,
	OM_sint	att_nb);
void
omP_trace_delete(
	FILE		*trc_file,
	char *file,
	OM_sint	line,
	OM_object	object);
#endif /* ifndef _OMTRACE_H */
