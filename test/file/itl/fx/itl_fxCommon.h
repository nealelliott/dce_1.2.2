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
 * itl_fxCommon.h -- 
 *    Common definitions needed by a number of the modules.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxCommon.h,v 1.1.314.1 1996/10/17 18:26:15 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_fxCommon.h,v $
 * Revision 1.1.314.1  1996/10/17  18:26:15  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:13  damon]
 *
 * $EndLog$
 */

#ifndef ITL_FX_COMMON_H
#define ITL_FX_COMMON_H

#include <dce/rpc.h>
#include <dcedfs/common_data.h>
#include <dcedfs/lock.h>
#include <dcedfs/nubik.h>

/*
 *  ITL_FX_STR_LEN_ERROR
 *    Just a standard error message for strings that are too lon.
 *
 *  Arguments:
 *    _strTypeP - type of string, e.g. "Path"
 *    _badStrP  - pointer to string that is invalid.
 *    _maxLen   - the maximum length that was exceeded. 
 */
#define ITL_FX_STR_LEN_ERROR(_strTypeP, _badStrP, _maxLen)                    \
        itl_Error("%s string length error - max. length is %d\nSTRING: %s\n", \
		  (_strTypeP), (_maxLen), (_badStrP))

#define ITL_FX_NAME_ERROR(_nameStrP)                                         \
            ITL_FX_STR_LEN_ERROR("Name", (_nameStrP), AFS_NAMEMAX)

#define ITL_FX_PATH_ERROR(_pathStrP)                                         \
            ITL_FX_STR_LEN_ERROR("Path", (_pathStrP), AFS_PATHMAX)

#define ITL_FX_DFS_PROT_SEQ   "ncadg_ip_udp"

/*
 *  The following macros deal with the cell info structure,
 *  itlFX_cell_t.  Except for the ITL_FX_MAX_CELL_NAME, all work on the
 *  global cellInfo variable.  (See comments about itlFX_cell_t.).
 *  They return the type associated with the field being accessed.
 */
#define ITL_FX_MAX_CELL_NAME    256
#define ITL_FX_CELL_NAME        cellInfo.name
#define ITL_FX_CELL_ID          cellInfo.cellId
#define ITL_FX_CELL_UBIK_INFO   cellInfo.ubikInfoP

/*
 *  Currently, the file server API test program assumes you only work
 *  in a single cell (hey, it's an API tester, not a full cache manager
 *  test program).  Also, it only talks to one FLDB server.  If it does
 *  during testing, we're done. (See previous comment if you feel it should
 *  handle multiple FLDB's.) This structure here contains all the relevant 
 *  information for a cell.
 */
typedef struct {
   char name[ITL_FX_MAX_CELL_NAME];    /* name of cell */
   afs_hyper_t cellId;                    /* unique id assigned by fx (high) */
   struct ubik_client *ubikInfoP;      /* needed for calls to fldb via ubik */
} itlFX_cell_t;


extern itlFX_cell_t cellInfo;

#endif  /* ITL_FX_COMMON_H */
