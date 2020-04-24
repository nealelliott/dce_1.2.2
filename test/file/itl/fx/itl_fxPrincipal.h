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
 * itl_fxPrincipal.h --
 *    Declarations for using the principal name hash table.
 *
 *    This module stores information about a DCE principal that is needed
 *    to issue DFS cache manager RPC's.
 *    The initialization routine must be called prior to using this
 *    module.  Names (principal's) are added via the AddName function, and
 *    are removed with the RemoveName function.  The only data that is
 *    currently stored is the login context, and it is obtained via the
 *    Lookup function and GET_LOGIN macro.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxPrincipal.h,v 1.1.11.1 1996/10/17 18:26:38 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_fxPrincipal.h,v $
 * Revision 1.1.11.1  1996/10/17  18:26:38  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:24  damon]
 *
 * Revision 1.1.6.1  1994/02/04  20:44:42  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:13  devsrc]
 * 
 * Revision 1.1.4.1  1994/01/28  20:44:32  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:49  annie]
 * 
 * Revision 1.1.2.2  1993/01/13  21:34:10  shl
 * 	Transarc delta: fred-ot-6350-add-fx-tests 1.3
 * 	  Selected comments:
 * 	    To place the DFS fole exporter tests into the tree.
 * 	    Migrate the fx exerciser to the new fx interface (OT 6054)
 * 	    Fix compilation problems.
 * 	[1993/01/13  14:34:45  shl]
 * 
 * $EndLog$
 */

#ifndef ITL_FX_PRINCIPAL_H
#define ITL_FX_PRINCIPAL_H

#include <dce/sec_login.h>
#include <utils/itl/itl_stds.h>
#include <utils/cht_hash.h>

/**
 **  Data definitions
 **/

/*
 *  The following macros are used for accessing data within the 
 *  itlFX_principal_t data type.  Currently, the login context is all that
 *  is there.
 *  The macro expects a pointer to a itlFX_principal_t.
 */
#define ITL_FX_PRINCIPAL_GET_LOGIN(_pp)   (_pp)->loginContext

typedef struct {
    sec_login_handle_t loginContext;
} itlFX_principal_t;


/**
 **  Function declarations
 **/

extern void itlFX_PrincipalInit _TAKES((
				     void
				     ));

extern boolean_t itlFX_PrincipalAddName _TAKES((
						IN char *nameP,
						IN char *passwordP
					     ));

extern void itlFX_PrincipalRemoveName _TAKES((
					      IN char *nameP
					      ));

extern itlFX_principal_t *itlFX_PrincipalLookup _TAKES((
							IN char *nameP
							));

#endif /* ITL_FX_PRINCIPAL_H */

