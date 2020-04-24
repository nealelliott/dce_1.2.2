/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPInst.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:21  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:34:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:59  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:19:53  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:41  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:23:09  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:09:54  marrek]
 * 
 * 	Beautification.
 * 	[1994/02/09  15:37:56  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:56:10  marrek]
 * 
 * $EndLog$
 */
#ifndef _DSPINST_H
#define _DSPINST_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char dspinst_rcsid[] = "@(#)$RCSfile: dsPInst.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:27 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code
dsP_instance_switch(
	const OM_workspace		workspace,
	const OM_object			subject,
	const OM_object_identifier	class,
	const OMP_object_type	class_type,
	const OMP_object_type	subject_type,
	OM_boolean				*flag
);
#endif /* ifndef _DSPINST_H */
