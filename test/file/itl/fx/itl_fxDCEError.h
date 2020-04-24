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
 * itl_fxDCEError.h -- 
 *    Contains useful macros/functions for dealing with DCE errors.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxDCEError.h,v 1.1.317.1 1996/10/17 18:26:22 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_fxDCEError.h,v $
 * Revision 1.1.317.1  1996/10/17  18:26:22  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:15  damon]
 *
 * Revision 1.1.312.1  1994/02/04  20:44:21  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:02  devsrc]
 * 
 * Revision 1.1.310.2  1994/01/20  18:44:09  annie
 * 	added copyright header
 * 	[1994/01/20  18:40:26  annie]
 * 
 * Revision 1.1.310.1  1993/12/07  17:44:05  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:18:23  jaffe]
 * 
 * $EndLog$
 */

#ifndef ITL_FX_DCE_ERROR_H
#define ITL_FX_DCE_ERROR_H

#include <utils/itl/itl_stds.h>

/*
 *  ITL_FX_DCE_ERROR() --
 *    This macro is around for backwards compatibility.  From now on use
 *    the function itlu_DCEError().
 */
#define ITL_FX_DCE_ERROR(_errStat, _errMsg) itlu_DCEError((_errStat), (_errMsg))

#endif  /* ITL_FX_DCE_ERROR_H */
