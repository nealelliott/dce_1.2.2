/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * ID: $Id: itl_stdtypes.h,v 1.1.7.1 1996/10/17 18:27:46 damon Exp $
 *
 * COMPONENT_NAME: Encina Development Infrastructure
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1991, 1992
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */
/*
 * HISTORY
 * $Log: itl_stdtypes.h,v $
 * Revision 1.1.7.1  1996/10/17  18:27:46  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:56  damon]
 *
 * Revision 1.1.2.3  1993/01/11  18:31:32  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:11:08  htf]
 * 
 * Revision 1.1.2.2  1992/11/18  22:00:47  jaffe
 * 	New File
 * 
 * 	$TALog: itl_stdtypes.h,v $
 * 	Revision 1.4  1994/11/01  22:30:09  cfe
 * 	Bring over the changes that the OSF made in going from their DCE 1.0.3
 * 	release to their DCE 1.1 release.
 * 	[from r1.3 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 * 
 * 	Revision 1.1  1992/11/03  15:27:45  fred
 * 	Incorporates Ofer's changes that permit procedures to have parameters
 * 	passed to them, and also includes the vector and valList data types.  The
 * 	vector data type is needed for DFS testing.  It allows us to manipulate
 * 	binary data.
 * 	[added by delta fred-itl-ofer-fixes-10-30, r1.1]
 * 
 * Revision 1.3  1992/10/20  19:17:07  oz
 * 	This delta implements ITL procedures with parameters and local variables.
 * 	The delta is backwards compatible: the set command for procedures
 * 	takes an optional integer for the number of parameters and another
 * 	optional number for the number of local variables.  The names of
 * 	the variables defines as either parameters to the procedures or as
 * 	local variables can be accessed in the procedure body by name.
 * 	[added by delta oz-4963-ITL-implement-procs-with-params-and-locals, r1.1]
 * 
 * Revision 1.2  1992/10/18  02:11:46  oz
 * 	Fixes in the new procedure structures:
 * 	  Allow the output of one procedure to go directly to that of another
 * 	  Required changes in the itl_SetVariable command to make it understand
 * 	    fields of structres.
 * 
 * 	- itlStds_PreProc and itlStds_PostProc have an additional parameter
 * 	  which passes the context for the procedure (the local variables).
 * 	[from revision 1.1 by delta oz-4963-ITL-create-procs-with-params, r1.3]
 * 
 * Revision 1.1  1992/10/17  19:02:19  oz
 * 	- Added files and enhances basic test script
 * 	[added by delta oz-4963-ITL-create-procs-with-params, r1.2]
 * 
 * $EndLog$
 */

/*
 * itl_stdtypes.h -- 
 *    The routines for handling ITL standard types
 *
 */
#ifndef TRANSARC_ITL_STDTYPES_H
#define TRANSARC_ITL_STDTYPES_H 1

/*
 *  itlStds_PreProc
 *	Perform the initialization before a procedure starts
 *	This reads the parameters for the procedure.
 *	The types of the arguments are in the procedure structure
 *	stored in the variable value.
 *
 *	This function is a no-op if the variable is not a procType
 */
extern void itlStds_PreProc _TAKES((
  IN	itl_var_t	*varP,
  OUT	pointer_t	*contextP
));

/*
 *  itlStds_PostProc
 *	Perform the code after a procedure terminates..
 *	This copies the OUT parameters for the procedure.
 *	The types of the arguments are in the procedure structure
 *	stored in the variable value.
 *
 *	This function is a no-op if the variable is not a procType
 */
extern void itlStds_PostProc _TAKES((
  IN	itl_var_t	*varP,
  IN	pointer_t	contextP
));


#endif  /*  TRANSARC_ITL_STDTYPES_H  */

