/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */

/* $Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/efs_vattr.h,v 1.1.21.1 1996/10/02 17:44:28 damon Exp $ */
/*
 * Copyright (C) 1993, 1995 Transarc Corporation
 * All rights reserved.
 */
/*
 * HISTORY
 * $Log: efs_vattr.h,v $
 * Revision 1.1.21.1  1996/10/02  17:44:28  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:36:04  damon]
 *
 * Revision 1.1.16.3  1994/07/13  22:21:01  devsrc
 * 	merged with DCE1.1
 * 	[1994/07/11  15:18:16  devsrc]
 * 
 * 	merged with bl-10
 * 	[1994/06/28  20:18:59  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:58:04  mbs]
 * 
 * Revision 1.1.16.2  1994/06/09  14:05:43  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:21:16  annie]
 * 
 * Revision 1.1.16.2  1994/06/09  14:05:43  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:21:16  annie]
 * 
 * Revision 1.1.16.1  1994/02/04  20:17:04  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:12:17  devsrc]
 * 
 * Revision 1.1.14.1  1993/12/07  17:22:53  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  20:55:52  jaffe]
 * 
 * $EndLog$
 */

#ifndef TRANSARC_EFS_VATTR_H
#define TRANSARC_EFS_VATTR_H

#include <dcedfs/osi.h>
#include <dcedfs/osi_kvnode.h>
#include <dcedfs/episode/volume.h>
#include <dcedfs/episode/logbuf.h>
#include <efs_evnode.h>

/*
 * Functions exported by efs_vattr.c.
 */

extern int vnva_FileCreate(
    struct evnode *devp,
    struct vattr *vap,
    epi_volFileId_t *fidP,
    epi_anode_t *fileP,
    int dxflags,
    elbb_tranRec_t transId,
    unsigned int umask
);

extern int vnva_FileConstruct(
    epi_volume_t volh,
    struct vattr *vap,
    epi_volFileId_t *fidP,
    epi_anode_t *fileP,
    elbb_tranRec_t transId
);

extern int vnva_GetAttr(
    struct evnode *evp,
    struct vattr *vap,
    osi_cred_t *cred,
    int extended
);

extern int vnva_SetAttr(
    struct evnode *evp,
    struct vattr *vap,
    osi_cred_t *cred,
    int extended
);

#endif /* TRANSARC_EFS_VATTR_H */
