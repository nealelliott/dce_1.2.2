/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_commonFid.h --
 *    Declarations for using the DFS file id data type.
 *
 *    This module stores information about a DFS file id.
 *    This is used to uniquely identify files with the DFS name space.
 *
 *    The initialization routine must be called prior to using this
 *    module.  File id's are added via the Create function.  They  are
 *    never removed.  Once a file id is created it is accessed via a call
 *    to the Get function.
 */
/*
 * HISTORY
 * $Log: itl_commonFid.h,v $
 * Revision 1.1.11.1  1996/10/17  18:25:21  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:46  damon]
 *
 * Revision 1.1.6.1  1994/02/04  20:43:13  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:23:34  devsrc]
 * 
 * Revision 1.1.4.1  1994/01/28  20:44:16  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:41  annie]
 * 
 * Revision 1.1.2.2  1993/01/28  19:44:45  shl
 * 	Transarc delta: jess-ot6603-add-two-itl-datatypes 1.2
 * 	  Selected comments:
 * 	    Add two itl common data types. One is moved form Fred's FX implementation,
 * 	    which is "itlCommon_afsfidDataType". Another one is moved form Jess's PIOCTL
 * 	    implementation,  which is "itlCommon_sockaddrDataType".
 * 	    Add Header file.
 * 	    Rename the files.
 * 	[1993/01/27  22:57:30  shl]
 * 
 * $EndLog$
 */

#ifndef ITL_COMMON_FID_H
#define ITL_COMMON_FID_H

#include <utils/itl/itl_stds.h>

extern int itlCommon_fidDataType;

/**
 **  Function declarations
 **/

extern void itlCommon_FidInit _TAKES((
				  void
				  ));

extern afsFid *itlCommon_FidGet _TAKES((
				    void
				    ));

extern var_val_t *itlCommon_FidCreate _TAKES((
					  IN afsFid *fidP
					  ));
#endif /* ITL_COMMON_FID_H */

