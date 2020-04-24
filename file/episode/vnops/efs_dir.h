/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: efs_dir.h,v $
 * Revision 1.1.22.1  1996/10/02  17:43:52  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:47  damon]
 *
 * $EndLog$
 */

/* $Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/efs_dir.h,v 1.1.22.1 1996/10/02 17:43:52 damon Exp $ */
/*
 * Copyright (C) 1996, 1993 Transarc Corporation
 * All rights reserved.
 */

#ifndef TRANSARC_EFS_DIR_H
#define TRANSARC_EFS_DIR_H

/*
 * Functions exported by efs_dir.c.
 */

#include <dcedfs/osi.h>
#include <dcedfs/episode/volume.h>
#include <dcedfs/episode/anode.h>
#include <dcedfs/episode/logbuf.h>
#include <efs_evnode.h>

/* Flags for vnd_Create */

#define DC_CHANGELINK	1		/* increment link count of subfile */
#define DC_MARKCTIME	2		/* mark ctime of subfile */

/* Input flags for vnd_dirCheck */

#define DXC_ISDELETE	1		/* deleting (vs. inserting) */
#define DXC_CREATING	2		/* creating */

/* Output flags for vnd_dirCheck */

#define DX_OBJEXISTS		1	/* object exists */
#define DX_INHERITFILEACL	2	/* file would inherit acl */
#define DX_INHERITDIRACL	4	/* dir would inherit acl */

extern int vnd_Create(
    elbt_TranRec_t transId,
    struct evnode *devp,
    struct evnode *nevp,
    char *fname,
    long npos,
    int flags
);

extern int vnd_dirLookup(
    struct evnode *devp,
    char *name,
    epi_volFileId_t *fid
);

extern int vnd_Delete(
    elbt_TranRec_t transId,
    struct evnode *devp,
    struct evnode *oevp,
    long opos,
    int rmdir
);

extern int vnd_Replace(
    elbt_TranRec_t transId,
    struct evnode *devp,
    struct evnode *noevp,
    struct evnode *ooevp,
    long opos,
    int rmdir
);

extern int vnd_dirCheck(
    struct evnode *devp,
    char *entryName,
    osi_cred_t *cred,
    epi_volFileId_t *Fidptr,
    int inFlags,
    long *offsetP,
    int *outFlagsP
);

extern int vnd_Read(
    efs_vnode_t devp,
    struct uio *uiop,
    osi_cred_t  *cred,
    int isvolop,
    int *numEntriesp,
    int *eofp
);

extern int vnd_CheckFilenameLength(char *name);

extern int vnd_Rename(
    struct evnode *sd,
    char *sname,
    struct evnode *td,
    char *tname,
    osi_cred_t *cred
);
#endif /* TRANSARC_EFS_DIR_H */
