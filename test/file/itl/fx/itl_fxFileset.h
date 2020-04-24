/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (C) Copyright 1996, 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_fxPrincipal.h --
 *    Declarations for using the fileset name hash table.
 *
 *    This module stores information about a DFS fileset that is needed
 *    to issue DFS cache manager RPC's.
 *    The initialization routine must be called prior to using this
 *    module.  Filesets are added via the AddName function, and
 *    are removed with the RemoveName function.  The only data that is
 *    currently stored is defined in the itlFX_fileset_t, and is accessed
 *    by issuing the GetId or GetServer function.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxFileset.h,v 1.1.11.1 1996/10/17 18:26:33 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_fxFileset.h,v $
 * Revision 1.1.11.1  1996/10/17  18:26:33  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:21  damon]
 *
 * $EndLog$
 */

#ifndef ITL_FX_FILESET_H
#define ITL_FX_FILESET_H

#include <dcedfs/common_data.h>
#include <utils/itl/itl_stds.h>
#include <utils/cht_hash.h>
#include <utils/itl/itl_fxServer.h>

/**
 **  Data definitions
 **/

typedef struct {
    afs_hyper_t filesetId;        /* fileset identifier */
    itlFX_server_t *serverP;   /* info about server where fileset resides */
} itlFX_fileset_t;

/*
 *  The following macros access fields within the itlFX_fileset_t data
 *  type.  They expect a pointer to a itlFX_fileset_t.  This can be obtained
 *  via the Lookup function.
 */
#define ITL_FX_FILESET_ID(_fp)       (_fp)->filesetId
#define ITL_FX_FILESET_SERVER(_fp)   (_fp)->serverP

/**
 **  Function declarations
 **/

extern void itlFX_FilesetInit _TAKES((
				     void
				     ));

extern boolean_t itlFX_FilesetAddName _TAKES((
					      IN char *nameP
					      ));

extern void itlFX_FilesetRemoveName _TAKES((
					    IN char *nameP
					    ));

extern itlFX_fileset_t *itlFX_FilesetLookup _TAKES((
						    IN char *nameP
						    ));

#endif /* ITL_FX_FILESET_H */

