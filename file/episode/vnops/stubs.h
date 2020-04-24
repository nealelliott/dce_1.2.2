/* Copyright (C) 1994 Transarc Corporation - All rights reserved. */

/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/stubs.h,v 1.4 1995/02/06 16:55:24 rajesh Exp $ */

/* Declarations for user-space simulation */

#ifdef KERNEL
This is a user-space only file.
#endif

extern struct user u;
extern osi_cred_t Ucred;

/* for simulating AGOPS, imported from efs_agops */
#define AG_EFSSETHANDLE(aggrp,avl) (aggrp->a_fsDatap = (void *)avl)
#define AG_HOLD(aggrp) ((aggrp)->a_refCount++)
#define AG_RELE(aggrp) ((aggrp)->a_refCount--)

extern void StartVnodeOpVol(struct volume *volp);
extern void EndVnodeOpVol(struct volume *volp);
extern void StartVolumeOp (struct volume *volp);
extern void EndVolumeOp (struct volume *volp);
extern int StartVnodeOp (struct evnode *evp);
extern void EndVnodeOp (struct evnode *evp);
extern void StartVFSOp (struct osi_vfs *vfsp);
extern void EndVFSOp (struct osi_vfs *vfsp);

extern void crfree(osi_cred_t *credp);
extern osi_cred_t *crdup(osi_cred_t *credp);
