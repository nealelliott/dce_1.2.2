/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_fset.h,v $
 * Revision 1.1.8.1  1996/10/17  18:26:07  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:09  damon]
 *
 * Revision 1.1.2.3  1993/01/11  18:30:22  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:08:48  htf]
 * 
 * Revision 1.1.2.2  1992/10/28  17:49:12  jaffe
 * 	Transarc delta: jdp-add-itl-stuff 1.8
 * 	  Selected comments:
 * 	    Add code for three new itl libraries: libitlUtils.a, libitlAggr.a, and
 * 	    libitlFset.a
 * 	    Interface definition for libitlFset.a
 * 	    Update error message in itlu_CheckReturnCode() so that the offending command
 * 	    is printed.
 * 	    Added documentation for libitlUtils.a, fixed some bugs, and reorganized.
 * 	    Changed interface and implementation of result checking routines.
 * 	    Provide routine for converting a value back to a name, assuming that
 * 	    the value--name mapping was constructed using itlu_ProcessValueMap().
 * 	    Checkpointing in order to import another delta.
 * 	    Fix misc. compilation problems and close inorder to open another delta.
 * 	    Make this stuff compile on the pmax.
 * 	[1992/10/28  12:20:10  jaffe]
 * 
 * $EndLog$
 */
/*
 * (C) Copyright 1992 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 *
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/fset/itl_fset.h,v 1.1.8.1 1996/10/17 18:26:07 damon Exp $
 *
 */


#ifndef _TRANSARC_ITL_FSET_H
#define _TRANSARC_ITL_FSET_H

#include <utils/itl/itl.h>


IMPORT long itlFset_GetFsetId _TAKES((
    OUT opaque*		result,
    OUT boolean_t*	shouldFree
));

IMPORT long itlFset_Init _TAKES((void));


#endif	/* _TRANSARC_ITL_FSET_H */
