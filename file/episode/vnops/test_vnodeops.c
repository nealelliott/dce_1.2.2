/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/osi_device.h>
/* XXX FIX ME XXX */
#undef	PRIVATE
#undef	SHARED
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_cred.h>
#include <dcedfs/osi_user.h>
#include <dcedfs/debug.h>
#include <dcedfs/lock.h>
#include <ctype.h>

#include <dcedfs/episode/boot.h>
#include <dcedfs/episode/async.h>
#include <dcedfs/episode/logbuf.h>

/* user space I/O simulation */
#include <dcedfs/episode/us.h>

#include <dcedfs/episode/anode.h>
#include <dcedfs/episode/fid.h>
#include <dcedfs/episode/volume.h>
#include <dcedfs/episode/file.h>
#include <dcedfs/episode/aggregate.h>

#include <dcedfs/aggr.h>		/* struct aggr */
#include <dcedfs/volume.h>		/* Volume definitions */
#include <dcedfs/volreg.h>
#include <dcedfs/dir.h>
#include <dcedfs/xcred.h>
#include <dce/id_base.h>
#include <dcedfs/dacl.h>

#include <dcedfs/episode/salvage.h>

#include <com_err.h>

#include <dcedfs/aclint.h>
#include <dcedfs/osi_kvnode.h>
#include <dcedfs/episode/epi_thread.h>
#include <efs_evnode.h>
#include <efs_volvnops.h>
#include <efs_vfsops.h>
#include <efs_vnodeops.h>
#include <efs_opcode.h>
#include <efs_misc.h>
#include <efsmount.h>
#include <stubs.h>

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/test_vnodeops.c,v 10.20 1996/03/25 21:09:50 cfe Exp $")

int afsdb_episode_anode_test = 0;
/* this over rides DBprint from fixed_anode.h */
#undef DBprint
#define DBprint(p) dprintf (afsdb_episode_anode_test, p)
#define ALWAYSprint(p) dprintf (1, p)

extern void dacl_SetLocalCellID(afsUUID *);

#define ONE_MEG (1024*1024)

#define BIG_PRIME 1257056887
#define NewRandom(v) (randomSeed = randomSeed*BIG_PRIME + (v))

/* partitions we're working with */
static struct partition {
    struct async_device *device;
    struct aggr *aggrp;
    epi_anode_t avl;
    u_long nBlocks;
    u_long fsize;
} partition[US_MAX_MINOR_DEV];

#define TEST_VNODE_POOL_SIZE 32		/* start very small */

/* for now these are just random constants */
#define FIRST_BLOCK 4			/* first legal block number */

extern struct dh_ops dho;

long fidCounter = 0;

int Uniq;

struct evnode *vnstack[20];
int vnstackptr;

char whoami[] = "test_vnodeops";
int lineNumber;				/* current line number in script */

static char *userName;
char *minorName[US_MAX_MINOR_DEV + 1] =
    {"one", "two", "three", "four"};
extern char *getenv();

#ifdef	AFS_AIX31_ENV
struct evmount {
	struct vmount vmt;
	char name[MAXPATHLEN];
	char stub[10];
	struct efs_mount_data efsa;
};
#endif	/* AFS_AIX31_ENV */
struct vnode stubvp;

char *PrintedFid (fid)
  IN epi_volFileId_t *fid;
{
    static char fidPrintName[32];
    sprintf (fidPrintName, "(%d,%d)", epid_ToIndex(fid), epid_ToUnique(fid));
    return fidPrintName;
}

char *PrintedHyper (h)
  IN afs_hyper_t h;
{
    static char hPrintName[32];
    return dfsh_HyperToStr(&h, hPrintName);
}

#if OLD_VNSYNC
struct EnumInfo {                       /* This is used for Enumeration */
    afs_hyper_t     volId;                    /* Given volume Id  */
    char      volName[EPIV_IDENT_NAME_SIZE]; /* Or, a given volume name */
    u_long    index;                    /* Returned INDEX  */
};

static long GetAVolumeIdx(InfoP, aggHandle, index)
  INOUT struct EnumInfo *InfoP;
  IN epi_anode_t aggHandle;
  IN u_long index;
{
    epi_anode_t volP;			/* volume handle */
    struct epiv_ident volIdent;		/* id and name of volP */
    long code;				/* error return code */

    code = epia_Open (aggHandle, index, 0, 0, &volP);
    if (code) return 0;
    code = epiv_GetIdent (volP, &volIdent);
    if (code) {
	code = 0;
	goto out;
    }
    if (AFS_hsame(volIdent.id, InfoP->volId)) {
       InfoP->index = index;    /* Found the desired index to volume table */
       code = 1;                /* tell the caller to stop the enumeration */
    }
out:
    epia_Close (volP);
    return (code);
}
#endif

long GetCurrentAVL (devIndex, deviceP, avlP)
  IN u_int devIndex;
  OUT struct async_device **deviceP;
  OUT epi_anode_t *avlP;
{
    long code;

    if (!(*deviceP = partition[devIndex].device)) {
	com_err (whoami, 0, "Device %d never opened", devIndex);
	return -1;
    }

    code = epig_InitAggregate (US_MAJOR_DEV, devIndex, 1, deviceP, avlP);
    if (code) {
	com_err (whoami, code, "initializing aggregate");
	return code;
    }
    afsl_Assert (!partition[devIndex].avl ||
		 (partition[devIndex].avl == *avlP));

    partition[devIndex].avl = *avlP;
    return 0;
}

int GetCurrentVnode (
  afs_hyper_t volid,
  epi_volFileId_t *fid,
  efs_vnode_t *evpP)
{
    long code;
    efs_vnode_t evp = *evpP;
    struct afsFid afsfid;		/* AFS standard fid */
    struct volume *volp;

    if (epid_IsInvalid(fid)) {
	afsl_Assert (*evpP == NULL);
	return EINVAL;
    }
    if (evp && VOLID_EQ(volid, evp->vd_volid) && epid_Equal(&evp->vd_fid, fid))
	return 0;

    if (evp) {
	VN_RELE (EVTOV (evp));
	*evpP = NULL;
    }

    VOLID_SET (afsfid.Volume, volid);
    code = volreg_Lookup (&afsfid, &volp);
    if (code) return code;

    afsfid.Vnode = epid_ToIndex (fid);
    afsfid.Unique = epid_ToUnique (fid);

    StartVnodeOpVol (volp);
    code = VOL_VGET (volp, &afsfid, ((struct vnode **)evpP));
    EndVnodeOpVol (volp);
    if (code)
	com_err (whoami, code, "looking up file for fid %s", PrintedFid (fid));
    VOL_RELE (volp);
    return code;
}

static long ClosePreviousAggr (devIndex)
  IN int devIndex;
{
    long code;
    struct async_device *device;
    epi_anode_t avl;

    device = partition[devIndex].device;
    if (!device) return 0;
    avl = partition[devIndex].avl;
    if (!avl) return 0;
    code = epig_CloseAggregate (avl);
    if (code) {
	com_err (whoami, code, "closing avl");
	return code;
    }
    partition[devIndex].avl = 0;
    return 0;
}

#define ClosePreviousVnode(evpP) \
    MACRO_BEGIN \
        efs_vnode_t *MACRO_evpP = evpP; \
	if (*MACRO_evpP) { \
	    VN_RELE(EVTOV(*MACRO_evpP)); \
	    *MACRO_evpP = NULL; \
	} \
    MACRO_END

static u_long randomValueIndex, randomSeed;

static void InitRandomValues (volId, fid, offset)
  afs_hyper_t volId;
  epi_volFileId_t *fid;
  u_long offset;
{
    randomSeed = AFS_hgethi(volId);
    NewRandom (AFS_hgetlo(volId));
    NewRandom (epid_ToIndex(fid));
    NewRandom (epid_ToUnique(fid));
    NewRandom (offset);
    randomValueIndex = 0;
}

static void GetRandomValues (len, buf)
  int len;
  char *buf;
{
    static u_long lastWord;
    static int bytesLeft = 0;

    char *lastByte = (char *)&lastWord + sizeof(lastWord) - bytesLeft;

    while (len > 0) {
	if (bytesLeft == 0) {
	    lastWord = NewRandom (randomValueIndex++);
	    bytesLeft = sizeof(lastWord);
	    lastByte = (char *)&lastWord;
	}
	*buf++ = *lastByte++;
	len--;
	bytesLeft--;
    }
}

#define VN_GLUE(vp,call) \
    (((glue_code = StartVnodeOp(vp)) || \
      (glue_code = (call), EndVnodeOp(vp), 0)), \
     glue_code)

#define	VN_ACCESS(VP,M,C) VN_GLUE(VP, efs_access(VP,M,C))
#define	VN_CLOSE(VP,F,C) VN_GLUE(VP, efs_close(VP,F,C))
#define	VN_CREATE(VP,NM,VA,E,M,VPP,C) \
    VN_GLUE(VP, efs_create(VP,NM,VA,E,M,VPP,C))
#define VN_FID(VP,FID) VN_GLUE(VP, efs_fid(VP,FID))
#define	VN_FSYNC(VP) VN_GLUE(VP, efs_fsync(VP))
#define	VN_GETATTR(VP,VA,C) VN_GLUE(VP, efs_getattr(VP,VA,C))
#define	VN_LOCKCTL(VP,LD,CMD,C,ID,OFF) \
    VN_GLUE(VP, efs_lockctl(VP,LD,CMD,C,ID,OFF))
#define	VN_MAP(VP,ADDR,LEN,OFF,FL) VN_GLUE(VP, efs_map(VP,ADDR,LEN,OFF,FL))
#define	VN_OPEN(VP,F,C) VN_GLUE(VP, efs_open(VP,F,C))
#define	VN_SELECT(VP,W,C) VN_GLUE(VP, efs_select(VP,W,C))
#define	VN_RDWR(VP,UIOP,RW,F,C) VN_GLUE(VP, efs_rdwr(VP,UIOP,RW,F,C))
#define	VN_LINK(VP,TDVP,TNM,C) VN_GLUE(VP, efs_link(VP,TDVP,TNM,C))
#define	VN_LOOKUP(VP,NM,VPP,C) VN_GLUE(VP, efs_lookup(VP,NM,VPP,C))
#define	VN_MKDIR(VP,NM,VA,VPP,C) VN_GLUE(VP, efs_mkdir(VP,NM,VA,VPP,C))
#define	VN_READDIR(VP,UIOP,C,EP,PX) VN_GLUE(VP, efs_readdir(VP,UIOP,C,EP,PX))
#define	VN_READLINK(VP,UIOP,C) VN_GLUE(VP, efs_readlink(VP,UIOP,C))
#define	VN_REMOVE(VP,NM,C) VN_GLUE(VP, efs_remove(VP,NM,C))
#define	VN_RENAME(VP,NM,TDVP,TNM,C) VN_GLUE(VP, efs_rename(VP,NM,TDVP,TNM,C))
#define	VN_RMDIR(VP,NM,CD,C) VN_GLUE(VP, efs_rmdir(VP,NM,CD,C))
#define	VN_SETATTR(VP,VA,C) VN_GLUE(VP, efs_setattr(VP,VA,C))
#define	VN_SYMLINK(VP,LNM,VA,TNM,C) VN_GLUE(VP, efs_symlink(VP,LNM,VA,TNM,C))
#define	VN_UNMAP(VP,FL) VN_GLUE(VP, efs_unmap(VP,FL))

#undef VFS_ROOT
#define	VFS_ROOT(VFS, VP) \
    (StartVFSOp(VFS), \
     glue_code = efs_root(VFS,VP), \
     EndVFSOp(VFS), glue_code)
#undef VFS_VGET
#define	VFS_VGET(VFS, VP, FID) \
    (StartVFSOp(VFS), \
     glue_code = efs_vget(VFS,VP,FID), \
     EndVFSOp(VFS), glue_code)
#define	VFS_STATFS(VFS, SFS) \
    (StartVFSOp(VFS), \
     glue_code = efs_statfs(VFS,SFS), \
     EndVFSOp(VFS), glue_code)

struct fidList {
    struct fidList *next;
    long lastCode;
    afs_hyper_t volId;
    epi_volFileId_t fid;
};
static struct fidList *fidAll;
static struct lock_data fidListLock;

static void AddFid (
  afs_hyper_t volId,
  epi_volFileId_t *fid)
{
    struct fidList *f;

    f = osi_Alloc (sizeof(*f));
    bzero(f,sizeof(*f));
    f->volId = volId;
    f->fid = *fid;
    BEGIN_LOCK_WRITE (&fidListLock) {
	/* insert into circular list */
	if (fidAll)
	    f->next = fidAll->next;
	else
	    fidAll = f;
	fidAll->next = f;
    } END_LOCK;
}

static void GetFid (
  afs_hyper_t *volIdP,
  epi_volFileId_t *fidP)
{
    int i = 17;
    BEGIN_LOCK_WRITE (&fidListLock) {
	if (fidAll) {
	    while (i-- > 0)
		fidAll = fidAll->next;
	    *volIdP = fidAll->volId;
	    *fidP = fidAll->fid;
	} else {
	    epid_MakeInvalid (fidP);
	}
    } END_LOCK;
}

int AttachVolume (
  int devIndex,
  struct epiv_status *status)
{
    struct afsFid afsfid;		/* AFS standard fid */
    struct volume *volp;
    struct aggr *aggrp;
    struct vol_stat_dy dystat;
    int code;

    afsfid.Volume = status->ident.id;
    code = volreg_Lookup (&afsfid, &volp);
    if (code == 0) {
	VOL_RELE (volp);
	return 0;
    }
    aggrp = partition[devIndex].aggrp;
    if (!aggrp) {
	epi_anode_t avl;
	char spec[10];
	struct vnode *bdevp;

	aggrp = osi_Alloc (sizeof(*aggrp));
	bzero (aggrp, sizeof(*aggrp));
	partition[devIndex].aggrp = aggrp;
	lock_Init(&aggrp->a_lock);

	avl = partition[devIndex].avl;
	if (!avl) {
	    code = epig_InitAggregate (US_MAJOR_DEV, devIndex, 1, NULL, &avl);
	    afsl_MBZ(code);
	    partition[devIndex].avl = avl;
	}
	AG_EFSSETHANDLE(aggrp,avl);
	/*
	 * Given the device name for the aggregate, get the device
	 * number into 'dev', and a vnode for it into 'bdevp'.
	 */
	sprintf (spec, "%d", devIndex);
	code = osi_getvdev(spec, NULL, &bdevp);
	afsl_MBZ(code);
	aggrp->devvp = bdevp;
    }

    volp = osi_Alloc (sizeof(*volp));
    bzero (volp, sizeof(*volp));
    volp->v_volId = afsfid.Volume;
    strcpy(volp->v_volName, status->ident.name);
    volp->v_count = 1;
    AG_HOLD(aggrp);
    volp->v_paggrp = aggrp;
    dystat.index = status->index;
    code = VOL_SETDYSTAT(volp, &dystat);
    if (code) return code;
    code = volreg_Enter (&afsfid.Volume, volp, status->ident.name);
    if (code) return code;
    code = VOL_ATTACH(volp);
    if (code) return code;
    return 0;
}

int AttachAggr (
  int devIndex)
{
    int code;				/* error return code */
    int index;
    struct aggr *aggrp;
    epi_anode_t avl;			/* aggregate handle */
    u_long lastindex;			/* highest valid index in aggregate */
    epi_anode_t vh;			/* volume handle */
    struct epiv_ident volIdent;		/* id and name of vh */
    struct epiv_status status;

    avl = partition[devIndex].avl;
    afsl_Assert(avl);

    code = epig_LastIndex (avl, &lastindex);
    afsl_MBZ(code);
    for (index=0; index <= lastindex; index++) {
	BEGIN_LCRIT (epi_anode_t vh,
		     code = epia_Open (avl, index, 0, 0, &vh),
		     code = epia_Close (vh)) {
	    code = epiv_GetIdent (vh, &volIdent);
	} END_CRIT;
	if (code) continue;

	status.ident = volIdent;
	status.index = index;
	code = AttachVolume (devIndex, &status);
	afsl_MBZ (code);
    }
    return 0;
}

int DetachVolume (
  afs_hyper_t volid)
{
    int code;
    struct afsFid afsfid;
    struct volume *volp;
    epi_anode_t avl;

    VOLID_SET (afsfid.Volume, volid);
    code = volreg_Lookup (&afsfid, &volp);
    if (code)
	return 0;

    /* simulate vol_open */
    vnm_StopUse (volp, -1);		/* completely shut it down */

    code = VOL_DETACH (volp);
    if (code) return code;

    code = volreg_Delete(&volp->v_volId, (char *)0);
    if (code) return code;
    VOL_RELE (volp);			/* volreg no longer has a ref */

    Assert (volp->v_count == 1);
    lock_ObtainWrite(&volp->v_lock);
    VOL_FREEDYSTAT(volp);	/* UFS also clears some aggr state here */
    AG_RELE (volp->v_paggrp);
    lock_ReleaseWrite(&volp->v_lock);

    /* simulate vol_close */
    vnm_StopUse (volp, 0);

    VOL_RELE (volp);
    return 0;
}

int DetachAggr (
  int devIndex)
{
    int code;				/* error return code */
    int index;
    struct aggr *aggrp;
    epi_anode_t avl;			/* aggregate handle */
    u_long lastindex;			/* highest valid index in aggregate */
    epi_anode_t vh;			/* volume handle */
    struct epiv_ident volIdent;		/* id and name of vh */

    aggrp = partition[devIndex].aggrp;
    if (!aggrp)
	return 0;

    avl = partition[devIndex].avl;
    code = epig_LastIndex (avl, &lastindex);
    afsl_MBZ(code);
    for (index=0; index <= lastindex; index++) {
	BEGIN_LCRIT (epi_anode_t vh,
		     code = epia_Open (avl, index, 0, 0, &vh),
		     code = epia_Close (vh)) {
	    code = epiv_GetIdent (vh, &volIdent);
	} END_CRIT;
	if (code) continue;

	code = DetachVolume (volIdent.id);
	afsl_MBZ (code);
    }

    Assert (aggrp->a_refCount == 0);

    lock_Destroy(&aggrp->a_lock);
    osi_Free (aggrp, sizeof(*aggrp));
    partition[devIndex].aggrp = NULL;

    code = epig_CloseAggregate (avl);
    afsl_MBZ (code);
    partition[devIndex].avl = NULL;

    return 0;
}


static int stopAll = 0;
static int threadsRunning = 0;
static long threadExitCode;		/* first non-zero thread exit code */
static struct lock_data threadLock;
struct threadInfo {
    int allocSize;			/* bytes actually allocated */
    char *name;				/* points past this block */
    char *line;				/* points past this block+*name */
    int procId;
    void *(*proc)(void *);
    int devIndex;
    afs_hyper_t volid;
    epi_volFileId_t fid;
};

void *RunThread(void *threadArg)
{
    struct threadInfo *i = threadArg;
    long code;

    code = (long)(*i->proc)(threadArg);
    if (code)
	stopAll++;
    BEGIN_LOCK_WRITE (&threadLock) {
	threadsRunning--;
	if (threadsRunning == 0)
	    osi_Wakeup ((caddr_t)&threadsRunning);
	if (!threadExitCode && code) {
	    printf ("Thread %s failed with code = %d\n", i->name, code);
	    threadExitCode = code;
	}
    } END_LOCK;
    pthread_exit(0);
    /* NOTREACHED */
    return NULL;
}

long WaitAll ()
{
    stopAll = 1;
    BEGIN_LOCK_WRITE (&threadLock) {
	while (threadsRunning > 0) {
	    osi_SleepW ((caddr_t)&threadsRunning, &threadLock);
	    lock_ObtainWrite (&threadLock);
	}
    } END_LOCK;
    return threadExitCode;
}

long RunStopUse (struct threadInfo *arg)
{
    struct afsFid afsfid;		/* AFS standard fid */
    struct volume *volp;
    long code;
    int i;
    u_int sleep;			/* time to sleep between stopuses */
    int count;
    static int openbits[] = {
	STOPUSE_NO_CHANGE,
	STOPUSE_NO_CHANGE|STOPUSE_NO_DIRTY,
	STOPUSE_NO_CHANGE|STOPUSE_NO_VM,
	STOPUSE_NO_CHANGE|STOPUSE_NO_ANODE|STOPUSE_NO_STATUS|STOPUSE_NO_VM};

    count = sscanf (arg->line, "%u", &sleep);
    if (count < 1)
	sleep = 1;

    VOLID_SET (afsfid.Volume, arg->volid);
    code = volreg_Lookup (&afsfid, &volp);
    if (code) return code;

    for (i=0; !stopAll; i++) {
	int which = i % (sizeof(openbits)/sizeof(openbits[0]));

	StartVolumeOp (volp);
	vnm_StopUse (volp, openbits[which]); /* open */
	osi_Pause(sleep);
	vnm_StopUse (volp, 0);		/* close */
	EndVolumeOp (volp);
	osi_Pause(sleep);
    }
    printf("Ran StopUse %d times in volume %s\n",
	   i, PrintedHyper (arg->volid));

    VOL_RELE (volp);
    return 0;
}

long RunCreate (struct threadInfo *arg)
{
    efs_vnode_t dir = NULL;
    long code;
    long glue_code;			/* for simulated volume glue */
    int i;
    char name[100];
    struct vattr va;
    enum vcexcl xc = EXCL;
    int mode = VREAD | VWRITE | VEXEC;
    va.va_rdev = 0;
    va.va_type = VREG;
    va.va_mode = 0777;

    if (code = GetCurrentVnode (arg->volid, &arg->fid, &dir)) return code;

    for (i=0; !stopAll; i++) {
	struct evnode *new;
	sprintf (name, "f%d.%d", arg->procId, i);

	code = VN_CREATE (dir, name, &va, xc, mode, &new, &Ucred);
	if (code) {
	    printf("unsuccessful RunCreate, name %s, code %d\n",
		   name, code);
	    break;
	}
	AddFid (arg->volid, &new->vd_fid);
	osi_Reschedule();
	VN_RELE (EVTOV (new));
    }
    printf("Created %d files in dir %s on volume %s\n",
	   i, PrintedFid (&arg->fid), PrintedHyper (arg->volid));

    VN_RELE (EVTOV(dir));
    return code;
}

long RunDelete (struct threadInfo *arg)
{
    long code;
    long glue_code;			/* for simulated volume glue */
    efs_vnode_t dir = NULL;
    int deleted, notfound, badname;
    char page[512];
    struct dirent *dirp;

    if (code = GetCurrentVnode (arg->volid, &arg->fid, &dir)) return code;

    deleted = notfound = badname = 0;
    while (!stopAll) {

	/* Read the directory */

	struct uio auio;
	struct iovec aiov;

	auio.osi_uio_iov = &aiov;
	auio.osi_uio_iovcnt = 1;
	auio.osi_uio_offset = 0;
	auio.osi_uio_seg = OSI_UIOSYS;
	while (!stopAll) {

	    /* one chunk at a time */

	    int bytesRead;
	    struct vattr va;

	    auio.osi_uio_resid = sizeof(page);
	    aiov.iov_base = (caddr_t) page;
	    aiov.iov_len = sizeof(page);
	    code = VN_READDIR (dir, &auio, &Ucred, 0, 0);
	    if (code) {
		printf ("unsuccessful readdir, code %d\n", code);
		break;
	    }
	    bytesRead = sizeof(page) - auio.osi_uio_resid;
	    if (bytesRead == 0) break;

	    for (dirp = (struct dirent *) page;
		 !stopAll && (((char *)dirp - page) < bytesRead);
		 dirp = (struct dirent *) ((char *)dirp + dirp->d_reclen)) {

		/* Check each entry to see if it is a regular file or not. */

		BEGIN_LCRIT (struct evnode *e,
			     code = VN_LOOKUP (dir, dirp->d_name, &e, &Ucred),
			     VN_RELE (EVTOV(e))) {
		    code = VN_GETATTR (e, &va, &Ucred);
		    if (code == ESTALE)
			continue;
		    afsl_MBZ (code);
		} END_CRIT;
		if (code == ENOENT) {
		    notfound++;
		    continue;
		} else if (code == ESTALE) {
		    badname++;
		    continue;
		}
		MBZ (code);
		if (va.va_type == VDIR)
		    continue;		/* skip "." & ".." and any subdirs */
		code = VN_REMOVE (dir, dirp->d_name, &Ucred);
		if (code == 0)
		    deleted++;
		else if (code == ENOENT)
		    notfound++;
		else if (code == ESTALE)
		    badname++;
		else {
		    MBZ(code);		/* for now stop here! */
		    return code;
		}
		osi_Reschedule();
	    }
	}
    }
    printf("Deleted %d files (skipping %d) in dir %s on volume %s\n",
	   deleted, notfound+badname,
	   PrintedFid (&arg->fid), PrintedHyper (arg->volid));

    VN_RELE (EVTOV(dir));
    return 0;
}

long RunVGet (struct threadInfo *arg)
{
    long code;
    struct volume *volp = NULL;
    int lookup, fail;
    efs_vnode_t held[TEST_VNODE_POOL_SIZE*10];
    int maxHeld = TEST_VNODE_POOL_SIZE + 10;
    int nHeld = 0;

    lookup = fail = 0;
    while (!stopAll) {
	afs_hyper_t volid;
	epi_volFileId_t fid;
	struct afsFid afsfid;		/* AFS standard fid */
	struct evnode *evp;
	/* "dup" is number of  consecutive already held vnodes we've found */
	int dup = 0;

	GetFid (&volid, &fid);
	if (epid_IsInvalid(&fid)) {
	    osi_Reschedule();
	    continue;
	}
	if (!volp || !VOLID_EQ(volp->v_volId, volid)) {
	    if (volp)
		VOL_RELE (volp);
	    VOLID_SET (afsfid.Volume, volid);
	    code = volreg_Lookup (&afsfid, &volp);
	    if (code) return code;
	}
	afsfid.Vnode = epid_ToIndex (&fid);
	afsfid.Unique = epid_ToUnique (&fid);

	StartVnodeOpVol (volp);
	lookup++;
	code = VOL_VGET (volp, &afsfid, ((struct vnode **)&evp));
	EndVnodeOpVol (volp);
	if (code) {
	    if (code != ESTALE) {
		printf ("Got %d from VOL_VGET\n", code);
		return code;
	    }
	    fail++;
	} else {
	    int i;
	    dup++;
	    for (i=0; i<nHeld; i++)
		if (held[i] == evp)
		    break;
	    if (i == nHeld) {		/* not already on the list, so add */
		held[nHeld++] = evp;
		dup = 0;
	    } else {
		VN_RELE (EVTOV(evp));
	    }
	}
	if (nHeld >= maxHeld) {
	    while (nHeld > 0)
		VN_RELE (EVTOV(held[--nHeld]));
	    maxHeld += maxHeld>>3;	/* increase by 12.5% */
	    if (maxHeld > sizeof(held)/sizeof(held[0]))
		maxHeld = sizeof(held)/sizeof(held[0]);
	}
	if (dup > 10)
	    osi_Reschedule ();
    }
    while (nHeld > 0)
	VN_RELE (EVTOV(held[--nHeld]));

    if (volp)
	VOL_RELE (volp);
    printf ("Looked up %i fids, with %d failures.\n", lookup, fail);
    return 0;
}

/* RunDestroy -- This thread will be fairly destructive, as operations will get
 *     ESTALE randomly.
 *
 * CAUTIONS -- We go out of our way to avoid directories, but still link count
 *     errors can develop due to index reuse.  I get asserts in efs_dir.c in
 *     vnd_Delete where calling ChangeLink on the child returns 565575695:
 *     "file's link count would be negative (dfs / epi)". */

long RunDestroy (struct threadInfo *arg)
{
    long code;
    struct volume *volp;
    int vols, fids, stale;

    vols = fids = stale = 0;
    while (!stopAll) {
	afs_hyper_t volid;
	epi_volFileId_t fid;
	struct afsFid afsfid;		/* AFS standard fid */
	struct evnode *evp;

	GetFid (&volid, &fid);
	if (epid_IsInvalid(&fid)) {
	    osi_Reschedule();
	    continue;
	}
	VOLID_SET (afsfid.Volume, volid);
	code = volreg_Lookup (&afsfid, &volp);
	if (code) return code;
	StartVolumeOp (volp);
	vnm_StopUse (volp, STOPUSE_NO_CHANGE|STOPUSE_NO_STATUS);
	vols++;

	do {
	    int dir;

	    BEGIN_LCRIT (epi_anode_t ap,
			 code = epif_Open (VTOVH(volp), &fid, &ap),
			 code = epif_Close (ap)) {
		dir = (EV_MODE_TO_VTYPE (epif_GetMode (ap)) == VDIR);
	    } END_CRIT;
	    if (dir)
		continue;

	    code = vnm_Delete (volp, &fid);
	    if (!code)
		fids++;
	    else if (code == ESTALE)
		stale++;
	    else {
		printf ("vnm_Delete failed with code %d\n", code);
		return code;
	    }
	    GetFid (&volid, &fid);
	} while (!stopAll && VOLID_EQ (volid, volp->v_volId));

	vnm_StopUse (volp, 0);		/* close */
	EndVolumeOp (volp);
	VOL_RELE (volp);
	osi_Pause (1);
    }

    printf ("Partially destroyed %d volumes, deleting %d fids (%d stale).\n",
	    vols, fids, stale);
    return 0;
}

long RunTestV1 (tests)
  IN FILE *tests;
{
    long code;
    long glue_code;			/* for simulated volume glue */
    int code_valid;
    long lastcode;
    int lastcode_valid;
    struct async_device *device;
    epi_anode_t avl;
    int devIndex = 0;			/* current device number */
    afs_hyper_t volId;			/* current volume id */
    epi_volFileId_t fid;		/* current file id */
    struct evnode *evp = NULL;		/* current vnode */
#if !(defined(AFS_SUNOS5_ENV) || defined(AFS_AIX_ENV))
    struct buf *bp;			/* for bread,brelse */
#endif
    struct osi_vfs vfs;			/* current vfs */

    memset(&vfs, 0, sizeof(struct osi_vfs));
    vnstackptr = 0;
    evp = 0;
    AFS_hzero (volId);
    epid_MakeInvalid (&fid);
    lock_Init (&threadLock);
    lock_Init (&fidListLock);
#ifdef AFS_AIX31_ENV
    vfs.vfs_mdata =
	(struct vmount *) osi_Alloc (sizeof (struct evmount));
#endif /* AFS_AIX31_ENV */

    while (1) {
	char op[32];			/* operation/command name */
	int oplen;
	char line[1024];		/* line following cmd name */
	int linelen;
	int count;			/* count of args scanned in line */
	int existOkay;			/* exist vs create */

	lastcode = code;
	lastcode_valid = code_valid;
	code = 0;
	code_valid = 1;
	if (!fgets (line, sizeof(line), tests)) {
	    fprintf (stderr, "%s encountered while reading command %s\n",
		     (feof(tests)?"EOF":"ERROR"), op);
	    return -1;
	}
	lineNumber++;
	{   int skip = strspn(line," \t\n");
	    int oplen = strcspn(line+skip," \t\n");
	    int i;
	    if (!oplen)
		continue;		/* skip blank line */
	    strncpy (op, line, oplen);
	    op[oplen] = 0;
	    /* blt down the rest of the line */
	    for (i=0; line[i] = line[i+skip+oplen]; i++);
	}
	linelen = strlen (line);
	if (line[linelen-1] != '\n') {
	    fprintf (stderr,
		     "command %s has too long an argument list ('%s')\n",
		     op, line);
	    return -1;
	}
	line[--linelen] = 0;

	{   char *comment;		/* beginning of comment string */
	    /* check for "blank" line */
	    if (op[0] == '#') continue;
	    /* shorten op name and line as appropriate */
	    if (comment = strchr(op, '#')) {
		*comment = 0;
		line[0] = 0;
	    }
	    else if (comment = strchr(line, '#')) *comment = 0;
	    linelen = strlen (line);
	}
	while ((line[linelen-1] == ' ') || (line[linelen-1] == '\t'))
	    line[--linelen] = 0;
#define NO_ARGS \
    if (linelen > 0) { \
        fprintf(stderr, "command %s doesn't take any arguments\n", op); \
	return -1; }

	lcstring (op, op, sizeof(op));
	oplen = strlen (op);
	DBprint (("%s %s", op, line));
#define IsOp(a) (strncmp(op, (a), oplen) == 0)

	if (IsOp("open_device")) {
	    u_long nBlocks;
	    u_long bsize;
	    u_long fsize;

	    ClosePreviousVnode (&evp);
	    if (code = ClosePreviousAggr (devIndex)) return code;
	    avl = NULL;

	    count = sscanf (line, "%u %lu %lu %lu",
			    &devIndex, &bsize, &fsize, &nBlocks);
	    if (count < 1) {
od_usage:
		fprintf (stderr,
			 "Usage error: %s <minor dev number> [<block size> [<fragment size> [<n blocks>]]]\n",
			 op);
		return -1;
	    }
	    if ((devIndex < 0) || (devIndex > US_MAX_MINOR_DEV)) {
		fprintf (stderr,
			 "minor device number %d out of range\n", devIndex);
		return -1;
	    }
	    if (count == 1) { /* just switch default device */
		if (partition[devIndex].device == 0) {
		    fprintf (stderr,
			     "device never opened; must specify size\n");
		    goto od_usage;
		}
		if (!partition[devIndex].fsize) {
		    fprintf (stderr, "No fragment size specified\n");
		    goto od_usage;
		}
	    }
	    else { /* create new device */
		dev_t dev;
		char devName[100];
		osi_off_t size;

		if (!userName) {
		    userName = getenv ("USER");
		}

		if (partition[devIndex].device) {
		    fprintf (stderr, "device number %d exists\n", devIndex);
		    return -1;
		}

		code = asevent_InitDevice (US_MAJOR_DEV, devIndex, bsize,
					   &partition[devIndex].device);
		if (code) {
		    com_err (whoami, code, "calling async_InitDevice");
		    return code;
		}

		dev = osi_makedev(US_MAJOR_DEV, devIndex);
		code = us_ioctl (dev, EPIU_IOCTL_SET_DIR, "/tmp");
		if (code) {
		    com_err (whoami, code, "calling us_ioctl to set path");
		    return code;
		}
		sprintf (devName, "dev-%s-%s", userName, minorName[devIndex]);
		code = us_ioctl (dev, EPIU_IOCTL_SET_FILE, devName);
		if (code) {
		    com_err (whoami, code,
			     "calling us_ioctl to set name to %s", devName);
		    return code;
		}

		code = us_open (dev, US_OFLAGS_ASYNC);
		if (code) {
		    com_err (whoami, code, "calling us_open");
		    return code;
		}
		if (count > 3) {
		    size = ((osi_off_t) nBlocks) * ((osi_off_t) bsize);
		    code = us_ioctl (dev, EPIU_IOCTL_SET_MAX_OFFSET, (caddr_t)&size);
		    if (code) {
			com_err (whoami, code, "calling us_ioctl to size");
			return code;
		    }
		    partition[devIndex].nBlocks = nBlocks;
		}
		if ((count > 2) && fsize)
		    partition[devIndex].fsize = fsize;
		device = partition[devIndex].device;
	    }
	}
	else if (IsOp("check_device")) {
	    struct epiz_salvageState ss;
	    NO_ARGS;
	    bzero((char *) &ss, sizeof(ss) );
	    if (partition[devIndex].device == 0) {
		fprintf (stderr, "device never opened\n");
		return -1;
	    }
	    code = epig_VerifyExistingAggregate(device, &ss );
	    if (code && (code == -1)) {
		/* code of -1 means that the err msg was already issued. */
		com_err (whoami, code, "verifying an aggregate");
	    }
	    if (!code)
		code = epiz_GetExitCode (&ss);
	    if (code)
		return code;
	}
	else if (IsOp("salvage_device")) {
	    struct epiz_salvageState ss;
	    NO_ARGS;
	    bzero((char *) &ss, sizeof(ss) );
	    ss.flags |= EPIZ_SS_SALVAGE;
	    if (partition[devIndex].device == 0) {
		fprintf (stderr, "device never opened\n");
		return -1;
	    }
	    code = epig_VerifyExistingAggregate(device, &ss );
	    if (code && (code == -1)) {
		/* code of -1 means that the err msg was already issued. */
		com_err (whoami, code, "verifying an aggregate");
	    }
	    if (!code)
		code = epiz_GetExitCode (&ss);
	    if (code)
		return code;
	}
	else if (IsOp("create_aggregate")) {
	    NO_ARGS;
	    if (partition[devIndex].avl) {
		fprintf (stderr, "AVL for device %d exists\n", devIndex);
		return -1;
	    }
	    if (!partition[devIndex].nBlocks) {
		fprintf (stderr, "No block size specified for device\n");
		return -1;
	    }
	    code = epig_CreateAggregate
		(device, partition[devIndex].fsize,
		 FIRST_BLOCK,
		 EPISODE_SUPERBLOCK_ADDR / device->blkSize,
		 partition[devIndex].nBlocks - FIRST_BLOCK,
		 elbb_ComputeMinLogSize (device, 5),
		 /*nExceptions*/0,0);
	    if (code) {
		com_err (whoami, code, "creating new aggregate");
		return code;
	    }
	    if (code = GetCurrentAVL(devIndex, &device, &avl)) return code;
	}
	else if (IsOp("open_aggregate")) {
	    NO_ARGS;
	    if (partition[devIndex].avl) {
		fprintf (stderr, "AVL for device %d exists\n", devIndex);
		return -1;
	    }
	    if (code = GetCurrentAVL(devIndex, &device, &avl)) return code;
	    if (code = AttachAggr (devIndex)) return code;
	}
	else if (IsOp("create_volume") ||
		 (existOkay = IsOp("exist_volume"))) {
	    struct afsFid afsfid;	/* AFS standard fid */
	    struct volume *volp;
	    struct epiv_status status;
	    u_long hi,lo;
	    afs_hyper_t id;
	    u_long quota;

	    ClosePreviousVnode (&evp);

	    bzero ((char *)&status, sizeof(status));
	    count = sscanf (line, "%s %lu,,%lu %lu",
			    status.ident.name, &hi,&lo, &quota);
	    if ((count < 1) || (count == 2)) {
		fprintf (stderr,
			 "Usage error: %s <name> [hi,,lo [<quota>]]\n", op);
		return -1;
	    }
	    if (count < 4) quota = 100;
	    if (count < 2) AFS_hzero(id);
	    else AFS_hset64(id,hi,lo);

	    if (AFS_hiszero(id)) {		/* find unused id */
		do {
		    AFS_hset64 (id, 0, rand());
		    VOLID_SET (afsfid.Volume, id);
		    code = volreg_Lookup (&afsfid, &volp);
		    if (code == 0)
			VOL_RELE (volp);
		} while (code == 0);
	    } else {
		VOLID_SET (afsfid.Volume, id);
		code = volreg_Lookup (&afsfid, &volp);
		if (code == 0) {
		    VOL_RELE (volp);
		    code = EPI_E_VOLIDEXISTS;
		    VOLID_SET (status.ident.id, id);
		    goto vol_created;
		}
	    }

	    status.allocated/*XXX Temporary hack XXX*/.limit = quota;
	    status.visible/*XXX Temporary hack XXX*/.limit = quota;
	    strncpy (status.statusMsg, line, sizeof(status.statusMsg));
	    status.ident.id = id;

	    code = vnm_CreateVolume (avl, &Ucred, 1, &status, &status.index);

vol_created:
	    if ((code == EPI_E_VOLIDEXISTS) && existOkay) {
		DBprint (("Volume %s now exists",
			  PrintedHyper(status.ident.id)));
	    } else if (code) {
		com_err (whoami, code, "creating a volume");
		return code;
	    } else {
		DBprint (("Created volume %s with id %s",
			   status.ident.name, PrintedHyper(status.ident.id)));
		code = AttachVolume (devIndex, &status);
		afsl_MBZ (code);
		epid_MakeVolumeRoot (&fid);
		AddFid (status.ident.id, &fid);
		printf ("new root fid is %s\n", PrintedFid (&fid));
	    }

	    volId = status.ident.id;
	}
	else if (IsOp("open_volume")) {
	    u_long hi,lo;
	    struct afsFid afsfid;	/* AFS standard fid */
	    struct volume *volp;

	    ClosePreviousVnode (&evp);

	    count = sscanf (line, "%lu,,%lu", &hi,&lo);
	    if (count != 2) {
ov_usage:
		fprintf (stderr, "Usage error: %s [hi,,lo]\n", op);
		return -1;
	    }
	    AFS_hset64 (volId, hi,lo);
	    if (AFS_hiszero(volId)) goto ov_usage;

	    VOLID_SET (afsfid.Volume, volId);
	    code = volreg_Lookup (&afsfid, &volp);
	    if (code) {
		fprintf (stderr, "No such volume: %s\n", PrintedHyper (volId));
		return -1;
	    }
	    VOL_RELE (volp);
	}
	else if (IsOp("root_vnode")) {
	    NO_ARGS;
	    ClosePreviousVnode (&evp);
	    epid_MakeVolumeRoot (&fid);
	    if (code = GetCurrentVnode(volId, &fid, &evp)) return code;

	    printf ("root fid is %s\n", PrintedFid (&fid));
	}
	else if (IsOp("vn_create")) {
	    char name[256];
	    char excl[32];
	    enum vcexcl xc;
	    int mode;
	    struct evnode *newevp;
	    struct vattr va;

	    xc = EXCL;
	    count = sscanf (line, "%s %31s", name, excl);
	    if (count == 2) {
		if (!strcmp(excl,"exclusive")) {
		    xc = EXCL;
		    mode = 0;
		} else if (!strcmp(excl,"nonexclusive")) {
		    xc = NONEXCL;
		    mode = VREAD | VWRITE | VEXEC;
		} else {
		    fprintf (stderr, "Must be exclusive or nonexclusive.\n");
		    return (-1);
		}
		count = 1;
	    }
	    if (count != 1) {
dirname_usage:
		fprintf (stderr, "Usage error: %s <name>\n", op);
		return (-1);
	    }

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    va.va_rdev = 0;
	    va.va_type = VREG;
	    va.va_mode = 0777;
	    code = VN_CREATE (evp, name, &va, xc, mode, &newevp, &Ucred);

	    VN_RELE (EVTOV (evp)); evp = 0;
	    if (code) {
		printf("unsuccessful vn_create, name %s, code %d\n",
			name, code);
		epid_MakeInvalid (&fid);
	    } else {
		evp = newevp;
		fid = evp->vd_fid;	/* structure assignment */
		AddFid (volId, &fid);
		printf ("name %s, new file fid is %s\n",
			 name, PrintedFid (&fid));
	    }
	}
	else if (IsOp("vn_mkdir")) {
	    char name[256];
	    struct evnode *newevp;
	    struct vattr va;

	    count = sscanf (line, "%s", name);
	    if (count != 1) goto dirname_usage;

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    va.va_rdev = 0;
	    va.va_type = VDIR;
	    va.va_mode = 0777;
	    code = VN_MKDIR (evp, name, &va, &newevp, &Ucred);

	    VN_RELE (EVTOV (evp)); evp = 0;
	    if (code) {
		printf("unsuccessful vn_mkdir, name %s, code %d\n",
			name, code);
		epid_MakeInvalid (&fid);
	    } else {
		evp = newevp;
		fid = evp->vd_fid;	/* structure assignment */
		AddFid (volId, &fid);
		printf ("name %s, new dir fid is %s\n",
			 name, PrintedFid (&fid));
	    }
	}
	else if (IsOp("vn_mknod")) {
	    char name[256];
	    char type[32];
	    char excl[32];
	    enum vcexcl xc;
	    int mode;
	    struct evnode *newevp;
	    struct vattr va;

	    xc = EXCL;
	    mode = 0;
	    count = sscanf (line, "%s %31s %31s", name, type, excl);
	    if (count == 3) {
		if (strcmp(excl,"exclusive") == 0) {
		    xc = EXCL;
		    mode = 0;
		} else if (strcmp(excl,"nonexclusive") == 0) {
		    xc = NONEXCL;
		    mode = VREAD | VWRITE | VEXEC;
		} else {
		    fprintf (stderr, "Must be exclusive or nonexclusive.\n");
		    return (-1);
		}
		count--;
	    }
	    if (count == 2) {
		if ((strcmp(type,"pipe") == 0) || (strcmp(type,"fifo") == 0)) {
		    va.va_type = VFIFO;
		    va.va_rdev = 0;
		} else if ((type[0] == 'b') || (type[0] == 'c')) {
		    int maj, min;
		    char borc;
		    int c;

		    c = sscanf (type, "%c%d,%d", &borc, &maj, &min);
		    if (c != 3) goto bad_mknod;
		    va.va_type = (borc == 'b' ? VBLK : VCHR);
		    va.va_rdev = osi_makedev(maj, min);
		} else
		    goto bad_mknod;
		count--;
	    }
	    if (count != 1) {
bad_mknod:
		fprintf (stderr,
 "Usage error: %s <name> {pipe|fifo|b#,#|c#,#} [exclusive|nonexclusive]\n",
			 op);
		return (-1);
	    }

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    va.va_mode = 0777;
	    code = VN_CREATE (evp, name, &va, xc, mode, &newevp, &Ucred);
	    if (code) {
		printf("unsuccessful vn_mknod, name %s [%s], code %d\n",
		       name, type, code);
	    } else if (!EV_ISVDEV(newevp)) {
		printf("Didn't get a spec device vnode\n");
		return -1;
	    } else {
		VN_RELE (EVTOV (newevp));
		printf ("name %s special %s\n", name, type);
	    }
	}
	else if (IsOp("vn_lookup")) {
	    char name[256];
	    struct evnode *newevp;

	    count = sscanf (line, "%s", name);
	    if (count != 1) goto dirname_usage;

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    code = VN_LOOKUP (evp, name, &newevp, &Ucred);
	    VN_RELE (EVTOV (evp)); evp = 0;
	    if (code) {
		printf("unsuccessful vn_lookup, name %s, code %d\n",
			name, code);
		epid_MakeInvalid (&fid);
	    } else {
		if (EV_ISVDEV(newevp)) {
		    printf("Got a spec device vnode.\n");
		    VN_RELE (EVTOV (newevp));
		    epid_MakeInvalid (&fid);
		} else {
		    evp = newevp;
		    fid = evp->vd_fid;	/* structure assignment */
		    printf ("name %s, looked up fid %s\n",
			    name, PrintedFid (&fid));
		}
	    }
	}
	else if (IsOp("lookuppn")) {
	    char name[256];
	    char *this, *next;
	    struct evnode *newevp;

	    count = sscanf (line, "%s", name);
	    if (count != 1) goto dirname_usage;

	    if (name[0] == '/') {
		epid_MakeVolumeRoot (&fid);
		this = name+1;
	    } else
		this = name;
	    if (code = GetCurrentVnode(volId, &fid, &evp)) return code;

	    for (; strlen (this); this = next) {
		next = strchr (this, '/');
		if (next == this) {
		    this++;		/* ignore multiple slashes */
		    continue;
		} else if (next != NULL) {
		    *next = 0;		/* null terminate this component */
		    next++;		/* continue at beg. of following */
		} else {
		    /* last component */
		    next = this+strlen(this);
		}

		code = VN_LOOKUP (evp, this, &newevp, &Ucred);
		VN_RELE (EVTOV (evp)); evp = 0;
		if (code) {
		    printf("unsuccessful pathname lookup of %s, code %d\n",
			   name, code);
		    return -1;
		}
		evp = newevp;
		fid = evp->vd_fid;
	    }
	}
	else if (IsOp("vn_remove")) {
	    char name[256];

	    count = sscanf (line, "%s", name);
	    if (count != 1) goto dirname_usage;

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    printf ("removing name %s\n", name);
	    code = VN_REMOVE (evp, name, &Ucred);
	    if (code) {
		printf("unsuccessful vn_remove, name %s, code %d\n",
			name, code);
	    }
	}
	else if (IsOp("vn_rmdir")) {
	    char name[256];

	    count = sscanf (line, "%s", name);
	    if (count != 1) goto dirname_usage;

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    printf ("removing directory %s\n", name);
	    code = VN_RMDIR (evp, name, 0, &Ucred);
	    if (code) {
		printf("unsuccessful vn_rmdir, name %s, code %d\n",
			name, code);
	    }
	}
	else if (IsOp("vn_link")) {
	    char from[256],to[256];
	    struct evnode *sevp;
	    int validsevp = 0;

	    count = sscanf (line, "%s %s", from, to);
	    if (count != 2) goto dirname_usage;

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    printf ("creating link %s to file %s\n", to, from);
	    code = VN_LOOKUP (evp, from, &sevp, &Ucred);
	    if (!code) {
		validsevp = 1;
		code = VN_LINK (sevp, evp, to, &Ucred);
	    }
	    VN_RELE (EVTOV (evp)); evp = 0;
	    if (code) {
		printf("unsuccessful vn_link from %s to %s, code %d\n",
			from, to, code);
		if (validsevp) VN_RELE (EVTOV (sevp));
	    } else {
		evp = sevp;
		fid = evp->vd_fid;	/* structure assignment */
	    }
	}
	else if (IsOp("vn_write")) {
	    u_long offset;
	    u_long length;
	    char ranBuf[1234];
	    int useRandomValues;
	    int value;			/* value to check for */
	    int i;
	    struct uio auio;
	    struct iovec aiov;

	    count = sscanf (line, "%lu %lu %d", &offset, &length, &value);
	    if (count < 2) {
wf_usage:
		fprintf (stderr,
			 "Usage error: %s <offset> <length> [<value>]\n", op);
		return (-1);
	    }
	    if (count ==2) useRandomValues = 1;
	    else useRandomValues = 0;
	    printf ("writing %d bytes at offset %d, ", length, offset);
	    if (useRandomValues)
		printf ("random values\n");
	    else
		printf ("value %d\n", value);

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    if (useRandomValues) InitRandomValues (volId, &fid, offset);
	    else for (i=0; i<sizeof(ranBuf); i++) ranBuf[i] = (char)value;

	    while (length > 0) {
		u_long len = sizeof(ranBuf);
		if (len > length) len = length;
		if (useRandomValues) GetRandomValues (len, ranBuf);
		auio.osi_uio_iov = &aiov;
		auio.osi_uio_iovcnt = 1;
		auio.osi_uio_offset = offset;
		auio.osi_uio_seg = OSI_UIOSYS;
		auio.osi_uio_resid = len;
#ifdef	AFS_SUNOS5_ENV
		auio.uio_limit = u.u_rlimit[RLIMIT_FSIZE].rlim_cur;
#endif
		aiov.iov_base = (caddr_t)ranBuf;
		aiov.iov_len = len;

		code = VN_RDWR (evp, &auio, UIO_WRITE, /*ioflag*/ 0, &Ucred);
		if (code) {
		    com_err (whoami, code,
			     "writing to file %s", PrintedFid (&fid));
		    return code;
		}
		length -= len;
		offset += len;
	    }
	}
	else if (IsOp("vn_append")) {
	    u_long length;
	    char ranBuf[1234];
	    int value;			/* value to check for */
	    int i;
	    struct uio auio;
	    struct iovec aiov;

	    count = sscanf (line, "%lu %d", &length, &value);
	    if (count < 2) goto wf_usage;
	    printf ("appending %d bytes, value %d\n", length, value);

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    for (i=0; i<sizeof(ranBuf); i++) ranBuf[i] = (char)value;

	    while (length > 0) {
		u_long len = sizeof(ranBuf);
		if (len > length) len = length;
		auio.osi_uio_iov = &aiov;
		auio.osi_uio_iovcnt = 1;
		auio.osi_uio_offset = 0;
		auio.osi_uio_seg = OSI_UIOSYS;
		auio.osi_uio_resid = len;
		aiov.iov_base = (caddr_t)ranBuf;
		aiov.iov_len = len;

		code = VN_RDWR (evp, &auio, UIO_WRITE, IO_APPEND, &Ucred);
		if (code) {
		    com_err (whoami, code,
			     "appending to file %s", PrintedFid (&fid));
		    return code;
		}
		length -= len;
	    }
	}
	else if (IsOp("vn_read")) {
	    u_long offset;
	    u_long length;
	    char ranBuf[2143];
	    char page[sizeof(ranBuf)];
	    int useRandomValues;
	    int value;			/* value to check for */
	    int i;
	    struct uio auio;
	    struct iovec aiov;

	    count = sscanf (line, "%lu %lu %d", &offset, &length, &value);
	    if (count < 2) goto wf_usage;
	    if (count ==2) useRandomValues = 1;
	    else useRandomValues = 0;
	    printf("reading %d bytes at offset %d, ", length, offset);
	    if (useRandomValues)
		printf ("random values\n");
	    else
		printf ("value %d\n", value);

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    if (useRandomValues) InitRandomValues (volId, &fid, offset);
	    else for (i=0; i<sizeof(ranBuf); i++) ranBuf[i] = (char)value;

	    while (length > 0) {
		u_long len = sizeof(ranBuf);
		if (len > length) len = length;
		auio.osi_uio_iov = &aiov;
		auio.osi_uio_iovcnt = 1;
		auio.osi_uio_offset = offset;
		auio.osi_uio_seg = OSI_UIOSYS;
		auio.osi_uio_resid = len;
		aiov.iov_base = (caddr_t)page;
		aiov.iov_len = len;

		code = VN_RDWR (evp, &auio, UIO_READ, /*ioflag*/ 0, &Ucred);
		if (code) {
		    com_err (whoami, code,
			     "reading from file %s", PrintedFid (&fid));
		    return code;
		}
		if (auio.osi_uio_resid)
		    printf ("Bytes not read: %d\n",
				length - len + auio.osi_uio_resid);
		if (useRandomValues) GetRandomValues (len, ranBuf);
		if (bcmp(page, ranBuf, len - auio.osi_uio_resid) != 0) {
		    fprintf (stderr, "error at offset=%d in %d bytes.\n",
			     offset, len - auio.osi_uio_resid);
		    return -1;
		}
		if (auio.osi_uio_resid) break;
		length -= len;
		offset += len;
	    }
	}
	else if (IsOp("vn_write_acl")
		 || IsOp("vn_write_initial_file_acl")
		 || IsOp("vn_write_initial_dir_acl")) {
	    struct dfs_acl acl;
	    int newlen;
	    char *ln;
	    char c;
	    int flag;
	    char *acltype;

	    if (IsOp("vn_write_acl")) {
		flag = VNX_ACL_REGULAR_ACL;
		acltype = "";
	    } else if (IsOp("vn_write_initial_file_acl")) {
		flag = VNX_ACL_INITIAL_ACL;
		acltype = "initial file ";
	    } else {
		flag = VNX_ACL_DEFAULT_ACL;
		acltype = "initial directory ";
	    }

	    while (line [linelen-1] == ',') {
		if (!fgets (&line [linelen], sizeof(line) - linelen, tests)) {
		    fprintf (stderr, "%s encountered while reading command %s\n",
			     (feof(tests)?"EOF":"ERROR"), op);
		    return -1;
		}
		lineNumber++;
		newlen = strlen (&line [linelen]);
		if (line [newlen+linelen-1] != '\n') {
		    fprintf (stderr,
			     "command %s has too long an argument list ('%s')\n",
			     op, line);
		    return -1;
		}
		linelen += newlen;
		line[--linelen] = 0;
	    }
	    ln = line; acl.dfs_acl_len = 0;
	    do {
		int x;
		count = sscanf (ln, "%x%c", &x, &c);
		if (!count || (count == 2 && c != ',')) {
		    fprintf (stderr, "Usage error: %s <acl>\n", op);
		    return (-1);
		}
		acl.dfs_acl_val[acl.dfs_acl_len++] = x;
	    } while (ln = strchr (ln, ','), ln++);

	    printf ("writing %sACL\n", acltype);

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    code = efsx_setacl (evp, &acl, 0, flag, flag, &Ucred);
	    if (code) {
		printf ("unsuccessful writing of acl of file %s\n",
			 PrintedFid (&fid));
	    }
	}
	else if (IsOp("vn_read_acl")
		 || IsOp("vn_read_initial_file_acl")
		 || IsOp("vn_read_initial_dir_acl")) {
	    struct dfs_acl acl;
	    int flag;
	    char *acltype;

	    NO_ARGS;

	    if (IsOp("vn_read_acl")) {
		flag = VNX_ACL_REGULAR_ACL;
		acltype = "";
	    } else if (IsOp("vn_read_initial_file_acl")) {
		flag = VNX_ACL_INITIAL_ACL;
		acltype = "initial file ";
	    } else {
		flag = VNX_ACL_DEFAULT_ACL;
		acltype = "initial directory ";
	    }

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    code = efsx_getacl (evp, &acl, flag, &Ucred);
	    if (code) {
		com_err (whoami, code,
			 "reading acl of file %s", PrintedFid (&fid));
		return code;
	    } else {
		int i;
		printf ("Reading %sacl of file %s:",
			acltype,
			PrintedFid (&fid));
		for (i=0; i<acl.dfs_acl_len; i++)
			printf ((i % 10) ? ", %x" : "\n%x",
				(unsigned char) acl.dfs_acl_val[i]);
		printf ("\n");
	    }
	}
	else if (IsOp("vn_stat")) {
	    struct vattr va;
	    int blocks;

	    NO_ARGS;
	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    VN_GETATTR (evp, &va, &Ucred);	/* always returns 0 */
#ifdef	AFS_OSF_ENV
	    blocks = va.va_bytes / 512;
#else
	    blocks = va.va_blocks;
#endif
	    printf ("type: %d, mode: %o, rdev: %d, link count: %d\n\
uid: %d, gid: %d, fsid: %x, nodeid: %d\n\
size: %d, blocksize: %d, blocks: %d\n\
atime: %d/%d\nmtime: %d/%d\nctime: %d/%d\n",
			va.va_type, va.va_mode, va.va_rdev, va.va_nlink,
			va.va_uid, va.va_gid, va.va_fsid, va.va_nodeid,
			va.va_size, va.va_blocksize, blocks,
			va.va_atime.tv_sec, va.va_atime.osi_subsec,
			va.va_mtime.tv_sec, va.va_mtime.osi_subsec,
			va.va_ctime.tv_sec, va.va_ctime.osi_subsec);
	}
	else if (IsOp("vn_times")) {
	    struct vattr va;

	    NO_ARGS;
	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    VN_GETATTR (evp, &va, &Ucred);	/* always returns 0 */
	    printf ("atime: %d/%d\nmtime: %d/%d\nctime: %d/%d\n",
			va.va_atime.tv_sec, va.va_atime.osi_subsec,
			va.va_mtime.tv_sec, va.va_mtime.osi_subsec,
			va.va_ctime.tv_sec, va.va_ctime.osi_subsec);
	}
	else if (IsOp("vn_chmod")) {
	    struct vattr va;
	    int mod;

	    count = sscanf (line, "%o", &mod);
	    if (count != 1) {
		fprintf(stderr,"incorrect vn_chmod\n");
		return -1;
	    }

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    printf("Setting mode to %o\n", mod);
	    osi_vattr_init (&va, OSI_VA_MODE);
	    va.va_mode = mod;
	    code = VN_SETATTR (evp, &va, &Ucred);
	    if (code) {
		printf("Error code %d setting mode\n", code);
	    }
	}
	else if (IsOp("vn_chown")) {
	    struct vattr va;
	    int own;

	    count = sscanf (line, "%d", &own);
	    if (count != 1) {
		fprintf(stderr,"incorrect vn_chown\n");
		return -1;
	    }

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    printf("Setting owner to %d\n", own);
	    osi_vattr_init (&va, OSI_VA_UID);
	    va.va_uid = own;
	    code = VN_SETATTR (evp, &va, &Ucred);
	    if (code) {
		printf("Error code %d setting owner\n", code);
	    }
	}
	else if (IsOp("vn_trunc")) {
	    struct vattr va;
	    int len;

	    count = sscanf (line, "%d", &len);
	    if (count != 1) {
		fprintf(stderr,"incorrect vn_trunc\n");
		return -1;
	    }

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    printf("Truncating to %d\n", len);
	    osi_vattr_init (&va, OSI_VA_SIZE);
	    va.va_size = len;
	    code = VN_SETATTR (evp, &va, &Ucred);
	    if (code) {
		printf("Error code %d truncating\n", code);
	    }
	}
	else if (IsOp("vn_utime")) {
	    struct vattr va;

	    NO_ARGS;
	    osi_vattr_init (&va, 0);
	    osi_vattr_add (&va, OSI_VA_MTIME | OSI_VA_ATIME);

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    printf("setting times to now\n");
	    code = efs_setxattr (evp, &va, XVN_SET_TIMES_NOW, &Ucred);
	    if (code) {
		printf("Error code %d setting times\n", code);
	    }
	}
	else if (IsOp("vn_utimes")) {
	    struct vattr va;
	    int hi,lo;
	    char s[100];
	    char *linex;

	    linex = line;
	    osi_vattr_init (&va, 0);
	    while (1) {
		count = sscanf (linex, "%s %d,%d", s, &hi, &lo);
		if (count != 3) {
		    fprintf(stderr,"incorrect vn_utimes\n");
		    return -1;
		}
		if (!strcmp(s,"atime")) {
		    osi_vattr_add (&va, OSI_VA_ATIME);
		    va.va_atime.tv_sec = hi; va.va_atime.osi_subsec = lo;
		} else if (!strcmp(s,"mtime")) {
		    osi_vattr_add (&va, OSI_VA_MTIME);
		    va.va_mtime.tv_sec = hi; va.va_mtime.osi_subsec = lo;
		} else if (!strcmp(s,"ctime")) {
		    osi_vattr_add (&va, OSI_VA_CTIME);
		    va.va_ctime.tv_sec = hi; va.va_ctime.osi_subsec = lo;
		} else {
		    fprintf(stderr,"incorrect vn_utimes\n");
		    return -1;
		}
		linex = index (linex, ';');
		if (!linex) break;
		linex++;
	    }

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    printf("setting times\n");
	    code = VN_SETATTR (evp, &va, &Ucred);
	    if (code) {
		printf("Error code %d setting times\n", code);
	    }
	}
	else if (IsOp("vn_access")) {
	    int mod;

	    count = sscanf (line, "%o", &mod);
	    if (count != 1) {
		fprintf(stderr,"incorrect vn_access\n");
		return -1;
	    }

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    printf("Checking access with mode %o\n", mod);
	    code = VN_ACCESS (evp, mod, &Ucred);
	    printf ("code: %d\n", code);
	}
	else if (IsOp("vn_rename")) {
	    char from[256],to[256],todir[256],temp[256];
	    struct evnode *todirevp;

	    count = sscanf (line, "%s %s %s", from, to, todir);
	    if (count > 3 || count < 2) {
		fprintf (stderr,"Incorrect vn_rename\n");
		return -1;
	    }

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    if (count == 3) {
		code = VN_LOOKUP (evp, todir, &todirevp, &Ucred);
		if (code) {
		    fprintf(stderr,
			"Lookup failure, name %s, code %d\n", todir, code);
		    return code;
		}
	    } else {
		todirevp = evp;
		FAST_VN_HOLD (todirevp);
	    }
	    strcpy (temp, PrintedFid (&evp->vd_fid));
	    printf ("Renaming %s in %s to %s in %s\n",
		     from, temp,
		     to, PrintedFid (&todirevp->vd_fid));
	    code = VN_RENAME (evp, from, todirevp, to, &Ucred);
	    VN_RELE (EVTOV (todirevp));
	    if (code)
		printf ("Unsuccessful rename, code %d\n", code);
	}
	else if (IsOp("ls")) {
	    char page[512];
	    struct uio auio;
	    struct iovec aiov;
	    struct dirent *dirp;

	    NO_ARGS;
	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    printf ("directory listing of %s:\n", PrintedFid (&evp->vd_fid));
	    auio.osi_uio_iov = &aiov;
	    auio.osi_uio_iovcnt = 1;
	    auio.osi_uio_offset = 0;
	    auio.osi_uio_seg = OSI_UIOSYS;
	    while (1) {
		auio.osi_uio_resid = sizeof(page);
		aiov.iov_base = (caddr_t) page;
		aiov.iov_len = sizeof(page);
		code = VN_READDIR (evp, &auio, &Ucred, 0, 0);
		if (code) {
		    printf ("unsuccessful readdir, code %d\n", code);
		    break;
		}
		if (auio.osi_uio_resid == sizeof(page)) break;
		dirp = (struct dirent *) page;
		while (((char *) dirp - page) < (sizeof(page) - auio.osi_uio_resid)) {
		    printf ("%s\t%d\n", dirp->d_name, dirp->d_ino);
		    dirp = (struct dirent *) ((char *) dirp + dirp->d_reclen);
		}
	    }
	}
	else if (IsOp("vn_symlink")) {
	    char name[256];
	    char linkto[256];
	    struct vattr va;

	    count = sscanf (line, "%s %s", name, linkto);
	    if (count != 2) {
		fprintf (stderr,
			 "Usage error: %s <name> <linkto>\n", op);
		return (-1);
	    }

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    va.va_mode = 0755;
	    code = VN_SYMLINK (evp, name, &va, linkto, &Ucred);

	    if (code) {
		printf("unsuccessful vn_symlink, name %s, code %d\n",
			name, code);
	    } else {
		printf ("name %s, symlink to %s\n",
			 name, linkto);
	    }
	}
	else if (IsOp("vn_readlink")) {
	    int len;
	    char *space;
	    struct uio auio;
	    struct iovec aiov;

	    NO_ARGS;

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    len = AFS_hgetlo(epia_GetLength (EVTOA (evp)));
	    space = osi_Alloc (len+1);
	    space[len]='\0';
	    auio.osi_uio_iov = &aiov;
	    auio.osi_uio_iovcnt = 1;
	    auio.osi_uio_offset = 0;
	    auio.osi_uio_seg = OSI_UIOSYS;
	    auio.osi_uio_resid = len;
	    aiov.iov_base = (caddr_t)space;
	    aiov.iov_len = len;

	    code = VN_READLINK (evp, &auio, &Ucred);
	    if (code)
		printf("Unsuccessful readlink, code %d\n", code);
	    else
		printf("Contents of symbolic link: %s\n", space);
	    osi_Free (space, len+1);
	}
	else if (IsOp("vn_bmap")) {
	    int blkno;

	    count = sscanf (line, "%d", &blkno);
	    if (count != 1) {
		fprintf(stderr,"incorrect vn_bmap\n");
		return -1;
	    }

#if defined(AFS_SUNOS5_ENV) || defined(AFS_AIX_ENV)
	    printf ("vn_bmap not implemented for SunOS or AIX\n");
	    code_valid = 0;
#else
	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    {   struct vnode *vp;
		daddr_t pbn;

		code = VN_BMAP (evp, blkno, &vp, &pbn);
		if (code) {
		    printf ("Error code from vn_bmap: %d\n", code);
		} else {
		    printf ("vn_bmap for %s, block %d:\n\
 device vnode is %#x, physical block number is %d.\n",
			    PrintedFid (&fid), blkno, vp, pbn);
		}
	    }
#endif /* AFS_SUNOS5_ENV, AFS_AIX_ENV */
	}
	else if (IsOp("vn_bread")) {
	    int blkno;

	    count = sscanf (line, "%d", &blkno);
	    if (count != 1) {
		fprintf(stderr,"Incorrect vn_bread\n");
		return -1;
	    }

#if defined(AFS_SUNOS5_ENV) || defined(AFS_AIX_ENV)
	    printf("vn_bread not implemented for SunOS or AIX\n");
	    code_valid = 0;
#else
	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    {   int dummy;
		int i, j;
		char *ptr;

		code = VN_BREAD (evp, blkno, &bp, &dummy);
		if (code) {
		    bp = (struct buf *) 0;
		    printf("Unsuccessful vn_bread, code %d\n", code);
		} else {
		    printf("vn_bread, block %d, buffer ptr %#x; contents:\n",
			   blkno, bp);
		    for (ptr = bp->b_un.b_addr, i = 0; i < 32; i++) {
			for (j = 0; j < 16; j++)
			    printf ("%d, ", ptr[i * 16 + j]);
			printf ("\n");
		    }
		}
	    }
#endif /* AFS_SUNOS5_ENV, AFS_AIX_ENV */
	}
	else if (IsOp("vn_brelse")) {
	    NO_ARGS;

#if defined(AFS_SUNOS5_ENV) || defined(AFS_AIX_ENV)
	    printf ("vn_brelse not implemented for SunOS or AIX\n");
	    code_valid = 0;
#else
	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    if (!bp)
		printf ("vn_brelse ignored\n");
	    else {
		code = VN_BRELSE (evp, bp);
		if (code)
		    printf ("Unsuccessful vn_brelse on buf %#x, code %d\n",
			     bp, code);
		else
		    printf ("Released buffer %#x\n", bp);
	    }
#endif /* AFS_SUNOS5_ENV, AFS_AIX_ENV */
	}
	else if (IsOp("vn_fsync")) {
	    NO_ARGS;

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    VN_FSYNC (evp);		/* always returns 0 */
	    printf("fsync performed on current vnode, fid = %s\n",
		   PrintedFid (&fid));
	}
	else if (IsOp("vn_fid")) {
	    NO_ARGS;

#ifdef AFS_OSF_ENV
	    printf ("vn_fid not implemented for OSF/1\n");
	    code_valid = 0;
#else
	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    {
#if defined(AFS_AIX_ENV)
		struct fileid *fidp = (struct fileid *)osi_Alloc(sizeof(struct fileid));

		VN_FID (evp, fidp);	/* always returns 0 */
		printf("AIX fid:  length = %d, ino = %d, gen = %d\n",
		       fidp->fid_len, fidp->fid_ino, fidp->fid_gen);
		osi_Free (fidp, sizeof(*fidp));
#elif defined(AFS_SUNOS54_ENV)
		struct fid fid;
		epi_volFileId_t lfid;

		fid.fid_len = MAXFIDSZ;
		code = VN_FID (evp, &fid);
		if (code) {
		    fprintf (stderr, "Error from vn_fid: %d\n", code);
		    return code;
		}
		if (fid.fid_len != sizeof (epi_volFileId_t)) {
		    fprintf (stderr,
			     "Sol24 fid: bad length: %d\n", fid.fid_len);
		    return -1;
		}
		bcopy ((char *) fid.fid_data, (char *) &lfid,
		       sizeof (epi_volFileId_t));
		printf("Sol24 fid:  length = %d, data = %s\n",
		       fid.fid_len, PrintedFid (&lfid));
#else
		struct fid *fidp;
		epi_volFileId_t lfid;

		VN_FID (evp, &fidp);	/* always returns 0 */
		bcopy ((char *) fidp->fid_data, (char *) &lfid,
		       sizeof (epi_volFileId_t));
		printf("Standard VFS fid:  length = %d, data = %s\n",
		       fidp->fid_len, PrintedFid (&lfid));
		osi_Free (fidp, sizeof(*fidp) - MAXFIDSZ + fidp->fid_len);
#endif
	    }
#endif /* AFS_OSF_ENV */
	}
	else if (IsOp("vn_setlock")) {
	    char s[100], t[100];
	    int first, length;
	    struct flock flock;
	    int cmd;

	    count = sscanf (line, "%s %s %d %d", s, t, &first, &length);
	    if (count != 4) {
badsetlk:	fprintf(stderr,"Incorrect vn_setlock\n");
		return -1;
	    }

#ifdef AFS_SUNOS5_ENV
	    printf ("vn_setlock not implemented for SunOS\n");
	    code_valid = 0;
#else
	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    flock.l_start = first;
	    flock.l_len = length;
	    if (!strcmp(t,"read")) flock.l_type = F_RDLCK;
	    else if (!strcmp(t,"write")) flock.l_type = F_WRLCK;
	    else goto badsetlk;
	    if (!strcmp(s,"wait")) cmd = F_SETLKW;
	    else if (!strcmp(s,"nowait")) cmd = F_SETLK;
	    else goto badsetlk;

	    printf("Setting lock on %s: %s %s %d %d\n",
			PrintedFid (&fid), s, t, first, length);
	    code = VN_LOCKCTL (evp, &flock, cmd, &Ucred, 0,0);
	    if (code) {
		printf ("Error code from vn_setlock: %d\n", code);
	    }
#endif /* AFS_SUNOS5_ENV */
	}
	else if (IsOp("vn_unlock")) {
	    int first, length;
	    struct flock flock;

	    count = sscanf (line, "%d %d", &first, &length);
	    if (count != 2) {
badunlk:	fprintf(stderr,"Incorrect vn_unlock\n");
		return -1;
	    }

#ifdef AFS_SUNOS5_ENV
	    printf ("vn_unlock not implemented for SunOS\n");
	    code_valid = 0;
#else
	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    flock.l_start = first;
	    flock.l_len = length;
	    flock.l_type = F_UNLCK;

	    printf("Unlocking lock on %s: %d %d\n",
		   PrintedFid (&fid), first, length);
	    code = VN_LOCKCTL (evp, &flock, F_SETLK, &Ucred, 0,0);
	    if (code) {
		printf ("Error code from vn_unlock: %d\n", code);
	    }
#endif /* AFS_SUNOS5_ENV */
	}
	else if (IsOp("vn_getlock")) {
	    char s[100];
	    int first, length;
	    struct flock flock;

	    count = sscanf (line, "%s %d %d", s, &first, &length);
	    if (count != 3) {
badgetlk:	fprintf(stderr,"Incorrect vn_getlock\n");
		return -1;
	    }

#ifdef AFS_SUNOS5_ENV
	    printf ("vn_getlock not implemented for SunOS\n");
	    code_valid = 0;
#else
	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    flock.l_start = first;
	    flock.l_len = length;
	    if (!strcmp((const char *)s,"read")) flock.l_type = F_RDLCK;
	    else if (!strcmp((const char *)s,"write")) flock.l_type = F_WRLCK;
	    else goto badgetlk;

	    printf("Getting lock on %s: %s %d %d\n",
			PrintedFid (&fid), s, first, length);
	    code = VN_LOCKCTL (evp, &flock, F_GETLK, &Ucred, 0,0);
	    if (code) {
		printf ("Error code from vn_getlock: %d\n", code);
	    } else {
		printf ("Lock returned: type = %d, start = %d, length = %d, \
process = %d\n",
			flock.l_type, flock.l_start, flock.l_len, flock.l_pid);
	    }
#endif /* AFS_SUNOS5_ENV */
	}
	else if (IsOp("set_proc")) {
	    int pid;
	    count = sscanf (line, "%d", &pid);
	    printf ("New pid: %d\n", pid);
#ifndef AFS_SUNOS5_ENV
#ifdef AFS_AIX31_ENV
	    u.u_epid = pid;
#else
	    u.u_procp->p_pid = pid;
#endif /* AFS_AIX31_ENV */
#endif /* AFS_SUNOS5_ENV */
	}
	else if (IsOp("set_uid")) {
	    int uid;
	    count = sscanf (line, "%d", &uid);
	    printf ("New uid: %d\n", uid);
	    osi_SetUID(&Ucred, uid);
	}
	else if (IsOp("set_gid")) {
	    int gid;
	    count = sscanf (line, "%d", &gid);
	    printf ("New gid: %d\n", gid);
	    osi_SetGID(&Ucred, gid);
	}
	else if (IsOp("set_realm")) {
	    int realm;
	    long len;
	    sec_id_pac_t *pacP;
	    xcred_t *xcP;
	    u_long pag;
	    pac_list_t *pacListP;
	    pac_list_t **pacListPP;

	    count = sscanf (line, "%d", &realm);
	    printf ("New realm: %d\n", realm);
	    /*
	     * If there is not already a PAG, make one
	     */
	    pag = osi_GetPagFromCred(&Ucred);
	    if (pag == OSI_NOPAG) {
		pag = osi_genpag();
		osi_SetPagInCred(&Ucred, pag);
	    }
	    /*
	     * If there is not already an xcred, make one
	     */
	    xcP = xcred_FindByPag (pag);
	    if (!xcP) {
		xcred_Create (&xcP);
		xcP->assocPag = pag;
	    }
	    xcred_Hold (xcP);
	    /*
	     * If the xcred already has a PAC, set its realm.
	     * Otherwise, give it a PAC with the proper realm.
	     */
	    if (!xcred_GetProp (xcP, "DCE_EPAC", 8, (char **) &pacListPP, &len)) {
		*((u_int32 *) &((*pacListPP)->pacp->realm.uuid)) = realm;
	    } else {
		/*
		 * Allocates a pac_list_t
		 */
		dacl_PacListFromUcred (&pacListP, &Ucred);
		*((u_int32 *) &pacListP->pacp->realm.uuid) = realm;
		xcred_PutProp (xcP, "DCE_EPAC", 8,
				(char *) &pacListP, sizeof(pac_list_t *), 0, 0);
	    }
	    xcred_Release (xcP);
	}
	else if (IsOp("stack_vnode")) {
	    NO_ARGS;

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    FAST_VN_HOLD (evp);
	    vnstack[vnstackptr++] = evp;
	}
	else if (IsOp("unstack_vnode")) {
	    NO_ARGS;

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    VN_RELE (EVTOV (evp));
	    evp = vnstack[--vnstackptr];
	    fid = evp->vd_fid;
	}
	else if (IsOp("release_vnode")) {
	    NO_ARGS;

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    VN_RELE (EVTOV (evp));
	    evp = (struct evnode *) 0;
	    epid_MakeInvalid (&fid);
	}
	else if (IsOp("mount_vol")) {
	    int oldUid = osi_GetUID(&Ucred);
	    struct efs_mount_data efsa;
	    int useId;
	    afs_hyper_t id;
	    char name[100];
	    int hi,lo;

	    printf ("Mounting volume %s\n", line);
	    count = sscanf (line, "%lu,,%lu", &hi, &lo);
	    if (count == 2)
		useId = 1;
	    else {
		useId = 0;
	    }

	    count = sscanf (line, "%s", name);
	    if (count < 1) {
		fprintf (stderr, "Bad mount_vol\n");
		return -1;
	    }

	    if (useId) {
		AFS_hset64 (id, hi, lo);
	    }


	    (void)sprintf(efsa.devName, "%d", devIndex);
	    AFS_hzero (efsa.volId);
	    efsa.flags = 0;
	    if (useId) {
		efsa.flags |= EFS_MOUNT_DATA_FLAG_USE_ID;
		efsa.volId = id;
	    }
	    osi_SetUID(&Ucred, 0);	/* pretend to be root */

#ifdef AFS_AIX_ENV
	    {
		struct evmount *evm;
		evm = (struct evmount *) vfs.vfs_mdata;
		strcpy(evm->name, name);
		vfs.vfs_mdata->vmt_data[VMT_OBJECT].vmt_off =
		    (short)&((struct evmount *)0)->name;
		strcpy(evm->stub, "dummy");
		vfs.vfs_mdata->vmt_data[VMT_STUB].vmt_off =
		    (short)&((struct evmount *)0)->stub;
		evm->efsa = efsa;	/* structure assignment */
		vfs.vfs_mdata->vmt_data[VMT_INFO].vmt_off =
		    (short)&((struct evmount *)0)->efsa;
		vfs.vfs_mntdover = &stubvp;
		code = efs_mount (&vfs);
	    }
#elif defined(AFS_OSF_ENV)
	    {
		stubvp.v_count = 1;
		stubvp.v_type = VDIR;
		vfs.m_vnodecovered = &stubvp;
		code = efs_mount (&vfs, "dummy", &efsa, NULL);
	    }
#elif defined(AFS_SUNOS5_ENV)
	    {
		struct mounta mta;

		mta.spec = name;
		mta.flags = 0;
		mta.dataptr = (char *) &efsa;
		mta.datalen = sizeof (efsa);
		stubvp.v_count = 1;
		stubvp.v_type = VDIR;
		code = efs_mount (&vfs, &stubvp, &mta, &Ucred);
		if (!code) {
		    vfs.vfs_fstype = 0;
		    vfssw [0].vsw_name = "foo";
		}
	    }
#else
	    code = efs_mount (&vfs, (char *) 0, &efsa);
#endif

	    osi_SetUID(&Ucred, oldUid);
	    if (code)
		printf ("Unsuccessful mount, code %d\n", code);
	}
	else if (IsOp("vfs_root")) {
	    struct vnode *vp;
	    NO_ARGS;
	    ClosePreviousVnode (&evp);
	    if (code = VFS_ROOT (&vfs, &vp)) return code;
	    evp = VTOEV(vp);
	    fid = evp->vd_fid;		/* structure assignment */
	    printf ("vfs root fid is %s\n", PrintedFid (&fid));
	}
	else if (IsOp("vfs_vget")) {
	    struct fid unixfid;
	    epi_volFileId_t lfid;
	    struct vnode *vp;
	    int ind, uniq;

	    count = sscanf (line, " (%d,%d)", &ind, &uniq);
	    if (count != 2) {
		fprintf (stderr, "Incorrect vfs_vget\n");
		return (-1);
	    }
	    ClosePreviousVnode (&evp);

	    epid_Assemble (&lfid, ind, uniq);
	    unixfid.fid_len = sizeof (epi_volFileId_t);
#ifdef	AFS_IBM_ENV
	    bcopy ((char *) &lfid, (char *) &unixfid.fid_data, sizeof (epi_volFileId_t));
#else
	    bcopy ((char *) &lfid, (char *) unixfid.fid_data, sizeof (epi_volFileId_t));
#endif	/* AFS_IBM_ENV */
	    code = VFS_VGET (&vfs, &vp, &unixfid);
	    if (code)
		printf ("Unsuccessful vfs_vget, code %d\n", code);
	    else {
		printf ("vfs_vget of fid %s\n", PrintedFid (&lfid));
		fid = lfid;
		evp = VTOEV(vp);
	    }
	} else if (IsOp("vfs_stat")) {
	    osi_statfs_t sfs;
	    long fsid[2];

	    NO_ARGS;
	    VFS_STATFS (&vfs, &sfs);	/* always returns 0 */
	    bcopy ((char *) &sfs.f_fsid, (char *) fsid, sizeof (fsid_t));
	    printf (
"block size %d, total blocks %d, free blocks %d, free blocks for user %d,\n\
total files %d, free files %d, file system id %d,%#x\n",
	    sfs.f_bsize, sfs.f_blocks, sfs.f_bfree, sfs.f_bavail,
	    sfs.f_files, sfs.f_ffree, fsid[0], fsid[1]);
	} else if (IsOp("unmount_vfs")) {
	    int oldUid = osi_GetUID(&Ucred);
	    NO_ARGS;
	    printf ("unmounting file system\n");
	    osi_SetUID(&Ucred, 0);	/* pretend to be "root" for this */

#ifdef AFS_AIX_ENV
	    code = efs_unmount (&vfs, 0);
#elif defined(AFS_SUNOS5_ENV)
	    code = efs_unmount (&vfs, &Ucred);
#else
	    code = efs_unmount (&vfs);
#endif /* AFS_AIX31_ENV */

	    osi_SetUID(&Ucred, oldUid);	/* restore uid */
	    if (code) {
		printf ("Unsuccessful vfs unmount, code %d\n", code);
	    }
	}
	else if (IsOp("unmount_vol")) {
	    struct afsFid afsfid;	/* AFS standard fid */
	    struct volume *volp;

	    NO_ARGS;
	    ClosePreviousVnode (&evp);
	    VOLID_SET (afsfid.Volume, volId);
	    code = volreg_Lookup (&afsfid, &volp);
	    afsl_MBZ (code);
	    if (vnm_Inuse (volp)) {
		fprintf (stderr, "Vnode(s) still open in volume %s\n",
				PrintedHyper (volId));
		return -1;
	    }
	    VOL_RELE (volp);
	}
	else if (IsOp("sleep")) {
	    long seconds;

	    count = sscanf (line, "%d", &seconds);
	    if (count != 1) {
		fprintf (stderr, "bad sleep command\n");
		return (-1);
	    }
	    osi_Pause (seconds);
	}
	else if (IsOp("close_aggregate") ||
		 IsOp("detach")) {
	    NO_ARGS;
	    ClosePreviousVnode (&evp);
	    if (code = DetachAggr (devIndex)) return code;
	    avl = NULL;
	}
	else if (IsOp("end")) {
	    struct afsFid afsfid;	/* AFS standard fid */
	    struct volume *volp;

	    NO_ARGS;
	    ClosePreviousVnode (&evp);

	    code = WaitAll();		/* shutdown all other threads */
	    if (code) {
		fprintf (stderr, "Some thread failed with code %d\n", code);
		return -1;
	    }

	    VOLID_SET (afsfid.Volume, volId);
	    code = volreg_Lookup (&afsfid, &volp);
	    if (volp) {
		if (vnm_Inuse (volp)) {
		    fprintf (stderr, "Anode(s) still open in volume %s\n",
			     PrintedHyper (volId));
		    return -1;
		}
		VOL_RELE (volp);
	    }
	    if (avl) {
		if (code = DetachAggr (devIndex)) return code;
		avl = NULL;
	    }

	    /* close other devices also */
	    for (devIndex=0; devIndex<US_MAX_MINOR_DEV; devIndex++) {
		if (!partition[devIndex].device)
		    continue;		/* never opened */
		if (code = DetachAggr (devIndex)) return code;
	    }
	    return 0;
	}
	else if (IsOp("check_code")) {
	    long check;

	    count = sscanf (line, "%d", &check);
	    if (count != 1) {
		fprintf (stderr, "Usage error: %s code\n", op);
		return -1;
	    }
	    if (lastcode_valid && check != lastcode) {
		fprintf (stderr, "Code error:  code = %d, should be %d\n",
			 lastcode, check);
		return -1;
	    }
	}
	else if (IsOp("check_mode")) {
	    long check;
	    struct vattr va;

	    count = sscanf (line, "%o", &check);
	    if (count != 1) {
		fprintf (stderr, "Usage error: %s mode\n", op);
		return -1;
	    }

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    code = VN_GETATTR (evp, &va, &Ucred);
	    afsl_MBZ (code);
	    if (check != (va.va_mode & 0777)) {
		fprintf (stderr,
			 "Mode error:  mode = %o, should be %o\n",
			 va.va_mode & 0777, check);
		return -1;
	    } else printf ("mode is %o\n", va.va_mode & 0777);
	}
	else if (IsOp("check_links")) {
	    long check;
	    struct vattr va;

	    count = sscanf (line, "%d", &check);
	    if (count != 1) {
		fprintf (stderr, "Usage error: %s link count\n", op);
		return -1;
	    }

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    VN_GETATTR (evp, &va, &Ucred);	/* always returns 0 */
	    if (check != va.va_nlink) {
		fprintf (stderr,
			 "Link count error:  count = %d, should be %d\n",
			 va.va_nlink, check);
		return -1;
	    } else printf ("link count is %d\n", va.va_nlink);
	}
	else if (IsOp("check_sizes")) {
	    long check_size, check_blocksize, check_alloc;
	    struct vattr va;
	    int blocks;

	    count = sscanf (line, "%d %d %d",
			    &check_size, &check_blocksize, &check_alloc);
	    if (count != 3) {
		fprintf (stderr, "Usage error: %s sizes\n", op);
		return -1;
	    }

	    if (code = GetCurrentVnode (volId, &fid, &evp)) return code;

	    VN_GETATTR (evp, &va, &Ucred);	/* always returns 0 */
	    if (check_size != va.va_size) {
		fprintf (stderr,
			 "Size error:  size = %d, should be %d\n",
			 va.va_size, check_size);
		return -1;
	    }
	    if (check_blocksize != va.va_blocksize) {
		fprintf (stderr,
			 "Block size error:  block size = %d, should be %d\n",
			 va.va_blocksize, check_blocksize);
		return -1;
	    }
#ifdef	AFS_OSF_ENV
	    blocks = va.va_bytes / 512;
#elif	AFS_HPUX_ENV
	    blocks = va.va_blocks * 2;	/* hack: HP/UX block size is 1024 */
#else
	    blocks = va.va_blocks;
#endif
	    if (check_alloc != blocks) {
		fprintf (stderr,
			 "Size error:  alloc = %d, should be %d\n",
			 blocks, check_alloc);
		return -1;
	    }
	    printf ("size = %d, blocksize = %d, alloc = %d\n",
		    va.va_size, va.va_blocksize, blocks);
	}
	else if ((oplen > 5) && (strncmp(op, "fork_", 5) == 0)) {
	    pthread_t newPid;
	    struct threadInfo *i;
	    int allocSize;

	    allocSize = sizeof(*i) + oplen+1 + linelen+1;
	    i = osi_Alloc(allocSize);
	    i->allocSize = allocSize;
	    i->name = (char *)i + sizeof(*i);
	    bcopy (op, i->name, oplen+1);
	    i->line = i->name + oplen+1;
	    bcopy (line, i->line, linelen+1);
	    i->fid = fid;
	    i->volid = volId;
	    i->devIndex = devIndex;

	    i->proc = (void *(*)(void *))
		(  IsOp("fork_stopuse") ? RunStopUse
		 : IsOp("fork_create") ? RunCreate
		 : IsOp("fork_delete") ? RunDelete
		 : IsOp("fork_vget") ? RunVGet
		 : IsOp("fork_destroy") ? RunDestroy
		 : 0);
	    if (!i->proc)
		goto bad_command;
	    BEGIN_LOCK_WRITE (&threadLock) {
		threadsRunning++;
		i->procId = threadsRunning;
		code = epi_ThreadCreate(RunThread, 16000, EPI_NORMAL_PRIORITY,
					(void *)i, &newPid);
		if (code) {
		    com_err (whoami, code, "can't create new process");
		    EXIT_CRIT;
		}
	    } END_LOCK;
	    if (code)
		return -1;
	}
	else if (IsOp("echo")) {
	    printf("\n### %s\n\n", line);
	}
	else {
bad_command:
	    fprintf (stderr, "Unknown command at line %d: '%s %s'\n",
		     lineNumber, op, line);
	    return EPI_E_NOENT;
	}
	fflush (stdout);
    }
}

void main(argc, argv)
  int   argc;
  char *argv[];
{
    long code;
    int i;

    int a;
    char *testFile = 0;
    char *sp;
    epi_uuid_t localCellID;
    struct efs_params efsParams;

    initialize_epi_error_table();
    initialize_asy_error_table();
    initialize_lgb_error_table();

    procinit();
#if defined(AFS_HPUX_ENV)
    u.u_rlimit[RLIMIT_FSIZE].rlim_cur = 0x7fffffff >> 9;
#else
#if !defined(AFS_OSF11_ENV)
    u.u_rlimit[RLIMIT_FSIZE].rlim_cur = 0x7fffffff;
#endif /* !AFS_OSF11_ENV */
#endif /* AFS_HPUX_ENV */
#ifndef AFS_SUNOS5_ENV
#ifdef AFS_AIX31_ENV
    u.u_epid = 10;
#else
    u.u_procp->p_pid = 10;
#endif /* AFS_AIX31_ENV */
#endif /* AFS_SUNOS5_ENV */
    osi_SetUID(&Ucred, 100);
    osi_SetGID(&Ucred, 200);
    osi_SetNGroups(&Ucred, 0);
    bzero ((char *)&localCellID, sizeof (epi_uuid_t));
    dacl_SetLocalCellID((afsUUID*)&localCellID);

    for (a=1; a<argc; a++) {
	int arglen = strlen(argv[a]);
	char arg[256];
	lcstring (arg, argv[a], sizeof(arg));

#define IsArg(a) (strncmp (arg,a, arglen) == 0)

	if (a==1 && arg[0] != '-' && arg[0] != '\0') testFile = argv[a];
	else if (IsArg("-runtest")) testFile = argv[++a];
	else if (IsArg("-settestdebug")) {
	    afsdb_episode_anode_test = strtol(argv[++a], &sp, 0);
	    if (*sp) goto usage;
	}
	else if (IsArg("-setasyncdebug")) {
	    afsdb_episode_async = strtol(argv[++a], &sp, 0);
	    if (*sp) goto usage;
	}
	else {
	    fprintf (stderr, "unexpected arg '%s'\n", arg);
	  usage:
	    fprintf (stderr, "Usage is: '%s [-runtest <testfile>]", whoami);
	    fprintf (stderr, " [-set{,test,verify,async}debug <n>]\n");
	    exit (1);
	}
    }
    if (afsdb_episode_anode_test || afsdb_episode_async) {
	printf ("Running with debuging on %#x (test), %#x (async).\n",
		afsdb_episode_anode_test, afsdb_episode_async);
    }

    for (i = 0; i < US_MAX_MINOR_DEV; i++)
	partition[i].device = 0, partition[i].avl = 0;

    bzero (&efsParams, sizeof(efsParams));
    EFS_ELBB_PARAMS(efsParams).byteCount = EFS_DEFAULT_BUFFER_POOL_SIZE;
    EFS_EPIG_PARAMS(efsParams).maxHandles = TEST_VNODE_POOL_SIZE; /* anodes */
    EFS_DIRB_PARAMS(efsParams).abuffers = EFS_DEFAULT_DIR_BUFFER_POOL_SIZE;
    efsParams.vnodePoolSize = TEST_VNODE_POOL_SIZE;
    efsParams.flockPoolSize = EFS_DEFAULT_FLOCK_POOL_SIZE;
    code = epii_Init (&efsParams);
    if (code) {
	com_err (whoami, code, "during module initialization");
	exit (2);
    }

    /* Start up the user-space I/O simulator.  Also start asynchronous I/O
     * completer thread running at low priority */

    code = us_Init();
    if (code) {
	com_err (whoami, code, "Can't initialize user-space I/O");
	exit (3);
    }
    {	pthread_t junk;
	code = epi_ThreadCreate(us_Complete, 4000, EPI_NORMAL_PRIORITY - 1,
				(void *)0, &junk);
	if (code) {
	    com_err (whoami, code, "Can't start user-space I/O completer");
	    exit (3);
	}
    }

    srand (1);

    if (testFile) {
	FILE *tests;
	char name[32];			/* test name */
	int version;			/* request tester version */
	char buffer[BUFSIZ];

	tests = fopen (testFile, "r");
	if (!tests) {
	    com_err (whoami, errno, "Can't open file of tests %s", testFile);
	    exit (9);
	}
	lineNumber = 0;
	do {
	    if (!fgets (buffer, sizeof(buffer), tests)) {
		fprintf (stderr, "%s encountered while reading file %s\n",
			 (feof(tests)?"EOF":"ERROR"), testFile);
		exit(9);
	    }
	    lineNumber++;
	} while (!isalnum(buffer[0]));
	sscanf(buffer,"%s %d",name, &version);
	if (version != 1) {
	    com_err (whoami, 0, "Unknown tester version %d in test %s from file %s", version, name, testFile);
	    exit (9);
	}
	fprintf (stdout, "Running test %s (version %d) from file %s\n",
		 name, version, testFile);

	code = RunTestV1 (tests);
	fclose (tests);
	if (code) {
	    printf ("Test %s was FAILED on line %d\n", name, lineNumber);
	    exit (9);
	}
	printf ("Test %s completed successfully\n", name);
    }

    exit (0);
}
