/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * ID: $Id: itl_listtype.h,v 1.1.7.1 1996/10/17 18:27:30 damon Exp $ 
 *
 * COMPONENT_NAME: Encina Development Infrastructure
 *
 * ORIGINS: Transarc Corp. 
 *
 * (C) COPYRIGHT Transarc Corp. 1992
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */
/*
 * HISTORY
 * $Log: itl_listtype.h,v $
 * Revision 1.1.7.1  1996/10/17  18:27:30  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:47  damon]
 *
 * Revision 1.1.2.3  1993/01/11  18:31:09  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:10:19  htf]
 * 
 * Revision 1.1.2.2  1992/11/18  22:00:23  jaffe
 * 	new file
 * 
 * 	$TALog: itl_listtype.h,v $
 * 	Revision 1.5  1994/11/01  22:29:48  cfe
 * 	Bring over the changes that the OSF made in going from their DCE 1.0.3
 * 	release to their DCE 1.1 release.
 * 	[from r1.4 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 * 
 * 	Revision 1.2  1992/11/04  13:04:29  fred
 * 	Add support for binary data (the ITL vector type) into the read and write
 * 	routines for the file interface library.
 * 
 * 	Exported itl_valListType definition.
 * 	[from revision 1.1 by delta fred-itl-support-binary-data, r1.1]
 * 
 * Revision 1.1  1992/11/03  15:27:41  fred
 * 	Incorporates Ofer's changes that permit procedures to have parameters
 * 	passed to them, and also includes the vector and valList data types.  The
 * 	vector data type is needed for DFS testing.  It allows us to manipulate
 * 	binary data.
 * 	[added by delta fred-itl-ofer-fixes-10-30, r1.1]
 * 
 * Revision 1.3  1992/10/20  19:17:04  oz
 * 	This delta implements ITL procedures with parameters and local variables.
 * 	The delta is backwards compatible: the set command for procedures
 * 	takes an optional integer for the number of parameters and another
 * 	optional number for the number of local variables.  The names of
 * 	the variables defines as either parameters to the procedures or as
 * 	local variables can be accessed in the procedure body by name.
 * 	[added by delta oz-4963-ITL-implement-procs-with-params-and-locals, r1.1]
 * 
 * Revision 1.2  1992/10/18  02:34:16  oz
 * 	- Fixed typos and compilation error
 * 	[from revision 1.1 by delta oz-4963-ITL-create-procs-with-params, r1.4]
 * 
 * Revision 1.1  1992/10/17  19:02:19  oz
 * 	- Added files and enhances basic test script
 * 	[added by delta oz-4963-ITL-create-procs-with-params, r1.2]
 * 
 * $EndLog$
 */

#ifndef TRANSACRC_ITL_LISTTYPE_H
#define TRANSACRC_ITL_LISTTYPE_H 1

extern int itl_valListType;

extern void itl_ValListInit _TAKES(( void ));


extern var_val_t *itl_SetValListVar _TAKES((
  IN int     num,
  IN var_val_t  **valArray
));


#endif  /*  TRANSACRC_ITL_LISTTYPE_H  */
