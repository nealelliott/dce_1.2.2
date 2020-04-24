/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (C) 1992,1993 Transarc Corporation.
 * All rights reserved.
 */
/*
 * HISTORY
 * $Log: agfs_vfsops.c,v $
 * Revision 1.1.19.1  1996/10/02  16:57:37  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:36  damon]
 *
 * Revision 1.1.13.3  1994/07/13  22:27:02  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:16:42  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:52:54  mbs]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  17:26:15  mbs]
 * 
 * Revision 1.1.13.2  1994/06/09  13:49:56  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:23:25  annie]
 * 
 * Revision 1.1.13.1  1994/02/04  20:04:13  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:05:42  devsrc]
 * 
 * Revision 1.1.11.1  1993/12/07  17:11:16  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/06  13:32:53  jaffe]
 * 
 * $EndLog$
 */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/stds.h>
#include <dcedfs/aggr.h>
#include <dcedfs/xvfs_vnode.h>	/* for vfsops structure definition */
#ifdef AFS_OSF_ENV
#include <dce_afs.h>
#endif	/* AFS_OSF_ENV */

#include <agfs_mount.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/agfs/agfs_vfsops.c,v 1.1.19.1 1996/10/02 16:57:37 damon Exp $")

extern struct osi_vnodeops	agfs_vnops;	/* From ./agfs_vnops.c */

#define PREALLOCATED_MOUNT_TABLE_ENTRIES	24

typedef struct mount_table {
    unsigned		mt_fullCount;
    struct lock_data	mt_lock;
    unsigned		mt_numslots;
    agfs_mount_t*	mt_slot;
} mount_table_t;

int	agfs_inited = 0;
static mount_table_t	mountTable;

int	agfs_Mount(), agfs_Unmount(), agfs_Statfs(), agfs_NoSys();
static int agfs_Root(struct osi_vfs *vfsP, struct vnode **vPP);
#ifdef AFS_SUNOS5_ENV
static int agfs_Sync(struct osi_vfs *vfsP, short flag, osi_cred_t *credp);
#else
static int agfs_Sync(struct osi_vfs *vfsP);
#endif /* AFS_SUNOS5_ENV */

#ifdef AFS_OSF_ENV
static int agfs_Start(), agfs_Init();
#endif	/* AFS_OSF_ENV */
#if defined(AFS_HPUX_ENV) && defined(GETMOUNT)
extern int agfs_GetMount();
#endif /* AFS_HPUX_ENV && GETMOUNT */

static void InitMountTableEntry(agfs_mount_t *mtEntryP, int isPreallocated);
static void PutMountTableEntry(agfs_mount_t *mtEntryP);

#ifdef AFS_OSF_ENV
struct osi_vfsops agfs_vfsops = {
	agfs_Mount,
	agfs_Start,
	agfs_Unmount,
	agfs_Root,
	agfs_NoSys,			/* quotactl */
	agfs_Statfs,
	agfs_Sync,
	agfs_NoSys,			/* fhtovp */
	agfs_NoSys,			/* vptofh */
	agfs_Init
};
#else
struct osi_vfsops agfs_vfsops = {
	agfs_Mount,
	agfs_Unmount,
	agfs_Root,
	agfs_Statfs,
	agfs_Sync,
	agfs_NoSys,			/* vget */
#ifdef	AFS_AIX31_ENV
	agfs_NoSys,			/* fscntl */
#elif	defined(AFS_HPUX_ENV) && defined(GETMOUNT)
	agfs_GetMount
#elif	defined(AFS_SUNOS5_ENV)
	agfs_NoSys,			/* mountroot */
	agfs_NoSys			/* swapvp */
#endif
};
#endif	/* AFS_OSF_ENV */
int
agfs_NoSys()
{
    return ENOSYS;
}


/* This function should be called with Preemption turned off */
/* SHARED */
#ifdef	AFS_AIX31_ENV
int agfs_Init(void *gfsP)
#else
int agfs_Init(void)
#endif	/* AFS_AIX31_ENV */
{
    unsigned	i;
    unsigned	slotsSize;

    lock_Init(&mountTable.mt_lock);
    lock_ObtainWrite(&mountTable.mt_lock);

    mountTable.mt_fullCount = 0;
    mountTable.mt_numslots = PREALLOCATED_MOUNT_TABLE_ENTRIES;
    slotsSize = sizeof (*mountTable.mt_slot) * mountTable.mt_numslots;
    mountTable.mt_slot = (agfs_mount_t*)osi_Alloc(slotsSize);

    for (i = 0; i < mountTable.mt_numslots; i++)
	InitMountTableEntry(&mountTable.mt_slot[i], 1);

    agfs_inited = 1;
    lock_ReleaseWrite(&mountTable.mt_lock);

    return 0;
}	/* agfs_Init() */

static void
InitMountTableEntry(agfs_mount_t *mtEntryP, int isPreallocated)
{
    struct vnode*	rootVP = &mtEntryP->am_rootvnode;

    bzero((char *) mtEntryP, sizeof *mtEntryP);
    if (isPreallocated)
	mtEntryP->am_flags = AGFS_MOUNT_FLAGS_PREALLOCATED;

#ifdef AFS_AIX31_ENV
    rootVP->v_vfsnext = rootVP->v_vfsprev = NULL;

    {
	struct gnode*	rootGP = &mtEntryP->am_rootgnode;

	rootVP->v_gnode = rootGP;
	rootVP->v_next = NULL;
	rootGP->gn_vnode = rootVP;
    }
#elif defined(AFS_OSF_ENV)
    rootVP->v_flag = V_static;
    rootVP->v_mount = DEADMOUNT;
#endif	/* AFS_AIX31_ENV */

    osi_SetVnodeOps(rootVP, &agfs_vnops);
}	/* InitMountEntry() */

static void
PutMountTableEntry(agfs_mount_t *mtEntryP)
{
#ifdef AFS_OSF_ENV
    osi_vSetVfsp(&mtEntryP->am_rootvnode, DEADMOUNT);
#endif	/* AFS_OSF_ENV */

    if (mtEntryP->am_flags & AGFS_MOUNT_FLAGS_PREALLOCATED)
	mtEntryP->am_state &= ~AGFS_MOUNT_STATE_INUSE;
    else
	osi_Free((opaque)mtEntryP, sizeof *mtEntryP);
}	/* PutMountTableEntry() */


int
agfs_GetMountTableEntry(
    agfs_mount_data_t *	mountDataP,
    dev_t		dev,
    int			flags,
    struct vnode *	devVP,
    struct osi_vfs *	vfsP,
    agfs_mount_t **	mtEntryPP)
{
    struct aggr *	aggrP;
    long		code;
    int			fsDataLen;
    unsigned		i;
    agfs_mount_t *	mtEntryP = NULL;
    struct vnode *	rootVP;
    char		type;

    *mtEntryPP = NULL;
    lock_ObtainWrite(&mountTable.mt_lock);
    for (i = 0; i < mountTable.mt_numslots; i++) {
	if (mountTable.mt_slot[i].am_state & AGFS_MOUNT_STATE_INUSE) {
	    /* Check if aggregate is already mounted */
#ifdef AFS_OSF_ENV
	    if (mountTable.mt_slot[i].am_rdev == dev)
#else
	    if (mountTable.mt_slot[i].am_aggr.devvp->v_rdev == dev)
#endif	/* AFS_OSF_ENV */
	    {
		code = EBUSY;
		goto error_exit;
	    }
	} else
	    mtEntryP = &mountTable.mt_slot[i];
    }
    if (mtEntryP == NULL) {		/* Mount table full */
	mountTable.mt_fullCount++;
	mtEntryP = (agfs_mount_t*)osi_Alloc(sizeof *mtEntryP);
	InitMountTableEntry(mtEntryP, 0);
    } else
	mtEntryP->am_state |= AGFS_MOUNT_STATE_INUSE;

    ASTABTYPE_TO_AGTYPE(mountDataP->as_type, type);
    if (type == AG_TYPE_UNKNOWN) {
	code = EINVAL;
	goto error_exit;
    }

    if (!agOpvec[type]) {
	code = EIO;
	goto error_exit;
    }

    aggrP = &mtEntryP->am_aggr;

    code = (*agOpvec[type]->ag_attach)(dev, devVP, flags, NULL,
				       &aggrP->a_fsDatap, &fsDataLen);
    if (code)
	goto error_exit;

    aggrP->a_aggrOpsp = agOpvec[type];
    aggrP->devvp = devVP;
    aggrP->a_type = type;
    aggrP->a_aggrId = mountDataP->as_aggrId;

    rootVP = &mtEntryP->am_rootvnode;
    osi_lockvp(rootVP);
    osi_assert(rootVP->v_count == 0);
    osi_unlockvp(rootVP);
#ifdef AFS_OSF_ENV
    rootVP->v_usecount = 1;
    osi_vInitLocks(rootVP);
    osi_vSetVfsp(rootVP, vfsP);
    osi_vSetType(rootVP, VDIR);

    rootVP->v_flag |= VROOT;

    mtEntryP->am_rdev = dev;
#else
    OSI_VN_INIT(rootVP, vfsP, VDIR, (dev_t)0, 0);
    rootVP->v_flag |= VROOT;
#endif

    lock_ReleaseWrite(&mountTable.mt_lock);

    *mtEntryPP = mtEntryP;

    return 0;

error_exit:
    if (mtEntryP)
	PutMountTableEntry(mtEntryP);
    lock_ReleaseWrite (&mountTable.mt_lock);
    return code;
}	/* agfs_GetMountTableEntry() */


int
agfs_PutMountTableEntry(agfs_mount_t *mtEntryP)
{
    struct aggr*	aggrP;
    int			code;

    lock_ObtainWrite(&mountTable.mt_lock);
    aggrP = &mtEntryP->am_aggr;
    if (code = AG_DETACH(aggrP)) {
	lock_ReleaseWrite(&mountTable.mt_lock);
	return code;
    }
    OSI_VN_RELE(&mtEntryP->am_rootvnode);

    PutMountTableEntry(mtEntryP);
    lock_ReleaseWrite(&mountTable.mt_lock);

    return 0;
}	/* agfs_PutMountTableEntry() */


static int
agfs_Root (struct osi_vfs *vfsP, struct vnode **vPP)
{
    *vPP = &(VFSTOAGFS(vfsP)->am_rootvnode);
    VN_HOLD(*vPP);
    return 0;
}	/* agfs_Root() */


int
agfs_Statfs(
    struct osi_vfs *vfsP
#if defined(AFS_AIX31_ENV) || defined(AFS_SUNOS5_ENV) || defined(AFS_HPUX_ENV)
    , osi_statfs_t *sbP
#endif
)
{
    struct aggr*	aggrP;
    int			code;
    agfs_mount_t*	mtEntryP;
    struct ag_status	status;
#ifdef AFS_OSF_ENV
    struct statfs*	sbP = &vfsP->m_stat;
#endif /* AFS_OSF_ENV */
    int			kPerBlock;

    mtEntryP = VFSTOAGFS(vfsP);
    aggrP = &mtEntryP->am_aggr;

#ifdef	AFS_SUNOS5_ENV
    (void) osi_PreemptionOff();
#endif
    if (code = AG_STAT(aggrP, &status)) {
#ifdef	AFS_SUNOS5_ENV
	osi_RestorePreemption(0);
#endif
	return code;
    }
#ifdef	AFS_SUNOS5_ENV
    osi_RestorePreemption(0);
#endif
#ifdef AFS_OSF_ENV
    sbP->f_type = MOUNT_AGFS;
    sbP->f_fsize = status.ag_dy.fragsize;
    sbP->f_bsize = status.ag_dy.blocksize;
#ifdef AFS_OSF11_ENV
    sbP->f_linkmax = LINK_MAX;
    sbP->f_namemax = NAME_MAX;
    sbP->f_pathmax = PATH_MAX;
    sbP->f_fsflags = f_notrunc;
    strcpy(sbP->f_vfstype, "agfs");
#endif /* AFS_OSF11_ENV */
#elif defined(AFS_SUNOS5_ENV)
    sbP->f_bsize = status.ag_dy.blocksize;
#else
    sbP->f_bsize = status.ag_dy.fragsize;
#endif /* AFS_OSF_ENV */

    kPerBlock = status.ag_dy.fragsize / 1024;

    /* totalUsable and free are in K, convert to blocks */
    sbP->f_blocks = status.ag_dy.totalUsable / kPerBlock;
    sbP->f_bfree = sbP->f_bavail = status.ag_dy.realFree / kPerBlock;
    sbP->f_files = -1;
    sbP->f_ffree = -1;

#ifdef AFS_OSF_ENV
    sbP->f_fsid = vfsP->m_stat.f_fsid;
#elif defined(AFS_AIX31_ENV) ||  defined(AFS_HPUX_ENV)
    sbP->f_fsid = vfsP->vfs_fsid;
#elif defined(AFS_SUNOS5_ENV)
    sbP->f_fsid = vfsP->vfs_fsid.val[0];
#endif

#ifdef	AFS_AIX31_ENV
    sbP->f_vfstype = MOUNT_AGFS;
    sbP->f_vfsnumber = vfsP->vfs_number;
    (void)strncpy(sbP->f_fname, "", sizeof(sbP->f_fname));
    bzero (sbP->f_fpack, sizeof(sbP->f_fpack)); /* what to put here? */
    sbP->f_name_max = -1;
#elif	defined(AFS_SUNOS5_ENV)
    sbP->f_frsize = status.ag_dy.fragsize;
    sbP->f_favail = -1;
    strcpy(sbP->f_basetype, vfssw[vfsP->vfs_fstype].vsw_name);
    sbP->f_flag = vf_to_stf(vfsP->vfs_flag);
    sbP->f_namemax = OSI_MAXNAMLEN;
    sbP->f_fstr[0] = '\0';
#endif	/* AFS_SUNOS5_ENV */

    return 0;
}	/* agfs_Statfs() */

/*
 * Iterate through mount table returning a new mount entry on
 * each call.  "cookie" keeps our place in the table.
 */
static agfs_mount_t *
agfs_MountIterate(int *cookie)
{
    int	i;
    agfs_mount_t *mtEntryP = NULL;

    i = *cookie;
    if (i < 0)
	return (NULL);

    lock_ObtainWrite(&mountTable.mt_lock);
    while (i++ < mountTable.mt_numslots) {
	mtEntryP = &mountTable.mt_slot[i - 1];
	if (mtEntryP->am_state & AGFS_MOUNT_STATE_INUSE)
		break;
    }
    lock_ReleaseWrite(&mountTable.mt_lock);
    if (i > mountTable.mt_numslots)
	mtEntryP = NULL;

    *cookie = i;
    return mtEntryP;
}

#ifdef AFS_SUNOS5_ENV
/* ARGSUSED */
/*
 * agfs_Sync is superfluous on SunOS since all the underlying
 * file systems will be sync'ed directly.
 */
static int
agfs_Sync(
    struct osi_vfs *vfsP,
    short flag,
    osi_cred_t *credp)
{
	return 0;
}
#else
static int
agfs_Sync(struct osi_vfs *vfsP)
{
    agfs_mount_t *mtEntryP;
    int cookie;
    int error = 0, err1;

    if (vfsP != NULL) {
	mtEntryP = VFSTOAGFS(vfsP);
	error = AG_SYNC(&mtEntryP->am_aggr, AGGR_SYNC_FILESYS);
    } else {
	/*
	 * Sync all in-use aggregates when vfsP is null.
	 */
	cookie = 0;
	while ((mtEntryP = agfs_MountIterate(&cookie)) != NULL) {
	    err1 = AG_SYNC(&mtEntryP->am_aggr, AGGR_SYNC_FILESYS);
	    if (error == 0)
		error = err1;
	}
    }
    return (error);
}
#endif /* AFS_SUNOS5_ENV */
