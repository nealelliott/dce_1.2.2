/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: efs_dirhandle.h,v $
 * Revision 1.1.63.1  1996/10/02  17:43:54  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:48  damon]
 *
 * $EndLog$
 */
/*
*/
/* $Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/efs_dirhandle.h,v 1.1.63.1 1996/10/02 17:43:54 damon Exp $ */

/*
 * Copyright (C) 1994, 1990 Transarc Corporation
 * All rights reserved.
 */

#ifndef EFS_DIRHANDLE_H
#define EFS_DIRHANDLE_H

/*
 * The epi_Dir_Handle structure overlays the Dir_Handle structure.
 */

struct epi_Dir_Handle {
    struct dh_ops *edh_ops;
    epi_anode_t edh_ap;
    epi_volume_t edh_volp;
    epi_volFileId_t edh_fid;
};

extern void edh_SetDirHandle(
  struct epi_Dir_Handle *edh,
  epi_anode_t ap,
  epi_volume_t volh,
  epi_volFileId_t *fid
);

#endif /* EFS_DIRHANDLE_H */
