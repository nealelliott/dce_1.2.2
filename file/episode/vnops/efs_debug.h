/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $EndLog$
 */
/*
*/
/*
 * Copyright (C) 1991, 1994 Transarc Corporation
 * All rights reserved.
 */

/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_debug.h,v 4.10 1994/11/02 21:54:18 cfe Exp $ */

EXPORT long efs_debug _TAKES((long, long, long, long));

/*
 * these are identifiers for the package type.
 */
#define EFS_DEBUG_ASYNC (1)
#define EFS_DEBUG_LOGBUF (2)
#define EFS_DEBUG_ANODE (3)
#define EFS_DEBUG_VNOPS (4)
#define EFS_DEBUG_DIR (5)
#define EFS_DEBUG_ACL (6)

#include <dcedfs/episode/asevent_debug.h>
#include <dcedfs/episode/efs_vnops_debug.h>
