/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: efs_access.h,v $
 * Revision 1.1.21.1  1996/10/02  17:43:37  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:43  damon]
 *
 * Revision 1.1.16.3  1994/07/13  22:20:37  devsrc
 * 	merged with DCE1.1
 * 	[1994/07/11  15:18:00  devsrc]
 * 
 * 	merged with bl-10
 * 	[1994/06/28  20:13:16  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:57:42  mbs]
 * 
 * Revision 1.1.16.2  1994/06/09  14:05:13  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:20:46  annie]
 * 
 * Revision 1.1.16.2  1994/06/09  14:05:13  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:20:46  annie]
 * 
 * Revision 1.1.16.1  1994/02/04  20:16:24  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:11:57  devsrc]
 * 
 * Revision 1.1.14.1  1993/12/07  17:22:18  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  20:55:28  jaffe]
 * 
 * $EndLog$
 */

/* $Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/efs_access.h,v 1.1.21.1 1996/10/02 17:43:37 damon Exp $ */
/*
 * Copyright (C) 1993 Transarc Corporation
 * All rights reserved.
 */

#ifndef TRANSARC_EFS_ACCESS_H
#define TRANSARC_EFS_ACCESS_H

#include <dcedfs/osi.h>
#include <dcedfs/dacl.h>
#include <dcedfs/episode/logbuf.h>
#include <dcedfs/episode/file.h>
#include <efs_evnode.h>

/*
 * Functions exported by efs_access.c.
 */
extern void vnax_GetOwner(
    osi_cred_t *credp,
    struct evnode *pvp,
    struct vattr *vap,
    epi_uuid_t *realmp,
    int *authp
);

extern int vnax_InheritAclMode(struct evnode *evp, elbt_TranRec_t transId);

extern int vnax_GetAnonAccess(
    struct evnode *evp,
    struct epif_status *fstatP,
    dacl_permset_t *permP
);

extern int vnax_GetAccess(
    struct evnode *evp,
    struct epif_status *fstatP,
    dacl_permset_t *permP,
    osi_cred_t *credP
);

extern int vnax_CanAccess(
    struct evnode *aevP,
    struct epif_status *afstatP,
    dacl_permset_t aperm,
    osi_cred_t *aucredP
);

extern int vnax_CanAdminister(
    struct evnode *evP,
    struct epif_status *fstatP,
    u_long *gidP,
    osi_cred_t *cred,
    int *issuperP,
    int *ingroupsP
);

extern int vnax_StickyCheck(
    struct evnode *devP,
    struct evnode *evP,
    osi_cred_t *cred
);

extern int vnax_DoChown(
    struct evnode *evP,
    long *mask,
    struct epif_status *afstatP,
    osi_cred_t *cred,
    uid_t uid,
    int volop
);

extern int vnax_DoChgrp(
    struct evnode *evP,
    long *mask,
    struct epif_status *afstatP,
    osi_cred_t *cred,
    gid_t gid,
    int volop
);

extern int vnax_CorrectAclRealm(
    struct evnode *evp,
    epi_uuid_t *realmp,
    int auth,
    elbt_TranRec_t transId
);

extern int vnax_SetForeignRegularAcl(
    struct evnode *evp,
    epi_uuid_t *realmp,
    int auth,
    elbt_TranRec_t transId
);

extern int vnax_SetForeignInitialAcl(
    struct evnode *evp,
    epi_uuid_t *realmp,
    int auth,
    logTranRec_t transId,
    int alength,
    int flag
);

#endif /* TRANSARC_EFS_ACCESS_H */
