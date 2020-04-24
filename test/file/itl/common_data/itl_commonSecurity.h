/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (C) Copyright 1993 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_commonSecurity.h --
 *    This module stores information about a DCE security login context.
 *
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/common_data/itl_commonSecurity.h,v 1.1.118.1 1996/10/17 18:25:28 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_commonSecurity.h,v $
 * Revision 1.1.118.1  1996/10/17  18:25:28  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:49  damon]
 *
 * Revision 1.1.113.1  1994/02/04  20:43:24  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:23:38  devsrc]
 * 
 * Revision 1.1.111.2  1994/01/20  18:44:00  annie
 * 	added copyright header
 * 	[1994/01/20  18:40:18  annie]
 * 
 * Revision 1.1.111.1  1993/12/07  17:43:26  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  22:00:15  jaffe]
 * 
 * $EndLog$
 */

#ifndef ITL_COMMON_SECURITY_H
#define ITL_COMMON_SECURITY_H

#include <dce/sec_login.h>
#include <utils/itl/itl_stds.h>

/**
 **  Function declarations
 **/

extern void itlCommon_SecurityInit _TAKES((
					   void
					   ));

/*
 * itlCommon_SecuritySetContext() --
 *    Sets the current login context.
 *
 * RETURN CODES: 
 *    TRUE  - Successful addition.
 *    FALSE - Unsuccessful addition, principal name exists or is invalid.
 */
extern boolean_t itlCommon_SecuritySetContext _TAKES((
						      IN char *nameP,
						      IN char *passwordP
						      ));

/*
 * itlCommon_SecurityGetContext() -- 
 *    Gets the current login context.
 *
 * RETURN CODES:
 *  TRUE  - success
 *  FALSE - failure
 */
extern boolean_t itlCommon_SecurityGetContext _TAKES((
						      OUT sec_login_handle_t *
						          contextP
						      ));


#endif /* ITL_COMMON_SECURITY_H */

