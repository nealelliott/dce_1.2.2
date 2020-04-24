/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (C) 1996, 1990 Transarc Corporation
 * All rights reserved.
 */

/*			Episode File System
		Directory support for Vnode operations

This file contains various functions that are useful in managing directories.

Contents:
    vnd_Create, vnd_Delete, vnd_Replace, vnd_Rename
    vnd_dirLookup, vnd_dirCheck
*/

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/osi_kvnode.h>
#include <dcedfs/osi.h>
#include <dcedfs/dacl.h>		/* dacl_perm_xxx */
#include <dcedfs/dir.h>			/* struct Dir_Entry && CONSTS */
#include <dcedfs/dir_buffer.h>		/* struct dh_ops */
#include <dcedfs/icl.h>

#include <dcedfs/episode/tlock.h>	/* struct tlock_data */
#include <dcedfs/episode/file.h>	/* struct epif_status */
#include <efs_dirhandle.h>		/* struct epi_Dir_Handle */
#include <efs_evnode.h>			/* struct evnode */
#include <efs_volvnops.h>		/* volume private data primitives */
#include <efs_access.h>
#include <efs_misc.h>
#include <efs_dirhandle.h>		/* struct epi_Dir_Handle */
#include <efs_dir.h>

#if !defined(KERNEL)
#include <pthread.h>
#endif /* !KERNEL */

static struct efsdircache efsdircache;		/* dir cache statistics */


RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_dir.c,v 4.133 1996/05/03 15:14:40 ota Exp $")
/*
 * vnd_Create - add an entry to a directory
 *
 * This function updates link counts, access/mod/change times, and
 *    directory contents, generally by calling lower-level functions.
 *
 * LOCKS USED -- the directory is assumed to be locked for writing.  If
 *     DC_MARKCTIME is specified the source must also be write locked.
 *
 * PARAMETERS:
 *   flags:
 *      DC_CHANGELINK:  true iff we have to call epif_ChangeLink to increment
 *      the link count.
 *      - We don't when creating a new file, because epif_Create saves us
 *	the trouble by initializing the link count to 1.
 *      - We don't when linking, because efs_link does the increment separately
 *	(for arcane reasons; see comments in that function's code).
 *      - We do call epif_ChangeLink when creating a new directory, because
 *	it must ultimately have a link count of 2 (parent plus ``.'').
 *      - We do call epif_Changelink when renaming.
 *      DC_MARKCTIME:  true iff we have to mark the ctime of the subfile.  This
 *      is unnecessary if the file was created as part of the same operation.
 */

/* SHARED */
int vnd_Create(
    elbt_TranRec_t transId,		/* transaction ID */
    struct evnode *devp,		/* evnode for directory */
    struct evnode *nevp,		/* evnode for object to be added */
    char *fname,			/* name of new directory entry */
    long npos,				/* piece no. at which to insert */
    int flags)				/* various bits */
{
    int code;				/* error return code */
    struct epi_Dir_Handle ddh;		/* directory handle for devp */

    /* Insert source object in target directory */
    vndh_SetDirHandle (devp, &ddh);
    if (code = dir_Create ((struct Dir_Handle *)&ddh, fname, &nevp->vd_fid,
			   npos, 0, transId))
	return (code);
    devp->vd_flags &= ~VD_LENVALID;	/* dir length may have changed */

    /*
     * If source object is a directory, its ``..'' will be or has been changed
     * to point to target directory, so increment target's link count.
     */
    if (EV_ISDIR (nevp)) {
	code = epif_ChangeLink (transId, EVTOA (devp), 1, 0, (u_int *) 0);
	MBZ(code);
    }

    /* Update target's mtime and ctime */
    vnm_SetMCtime (devp, MARK_MC);

    /* Increment source's link count if requested */
    if (flags & DC_CHANGELINK) {
	code = epif_ChangeLink (transId, EVTOA (nevp), 1, 0, (u_int *) 0);
	MBZ(code);
    }

    /* Update source's ctime if requested */
    if (flags & DC_MARKCTIME)
	vnm_SetMCtime (nevp, MARK_C);

    return (0);
}
/*
 * vnd_Delete -- delete an entry from a directory
 *
 * This function updates link counts, access/mod/change times, and
 *    directory contents, generally by calling lower-level functions.
 *
 * LOCKS:
 * Both the directory and the object to be deleted are assumed to be
 *    locked for writing.
 */

/* SHARED */
int vnd_Delete(
    elbt_TranRec_t transId,		/* transaction ID */
    struct evnode *devp,		/* evnode for directory */
    struct evnode *oevp,		/* evnode for object to be deleted */
    long opos,				/* piece no. of object in directory */
    int rmdir)				/* object is expected to get flushed */
{
    int code;				/* error return code */
    struct epi_Dir_Handle ddh;		/* directory handle for devp */
    struct epi_Dir_Handle odh;		/* directory handle for oevp */
    epi_anode_t oap;			/* anode handle for oevp */
    epi_anode_t dap;			/* anode handle for devp */
    int lcdelta;			/* change to object link count */

    oap = EVTOA (oevp);
    dap = EVTOA (devp);

    if (rmdir && EV_ISDIR (oevp)) {	/* if deleting a directory */
	/*
	 * if directory has more than the normal number of hard links,
	 * super-user has been fiddling with it.  Don't touch it.
	 */
	if (epif_GetLinkCount (oap) > 2)
	    return (ENOTEMPTY);

	/* if directory is not empty, can't delete it */
	vndh_SetDirHandle (oevp, &odh);
	if (!dir_IsEmpty ((struct Dir_Handle *)&odh))
	    return (ENOTEMPTY);

	/* when directory is deleted, don't allow lookup of . or .. */
	oevp->vd_flags |= VD_DIRDELETED;

	/*
	 * Decrement link count twice if it's a directory, because
	 * it is about to die, and its own ``.'' pointer will go away.
	 */
	lcdelta = -2;
	osi_dnlc_remove_r(EVTOV(oevp), ".");
	osi_dnlc_remove_r(EVTOV(oevp), "..");
    } else
	lcdelta = -1;

    /* Delete entry from parent directory */
    vndh_SetDirHandle (devp, &ddh);
    if (code = dir_Delete ((struct Dir_Handle *)&ddh, opos, transId))
	return (code);
    devp->vd_flags &= ~VD_LENVALID;	/* we might truncate dirs someday */

    /*
     * Decrement the parent directory's link count if the child is a directory,
     * because child's ``..'' pointer will either be reset or go away.
     */
    if (EV_ISDIR (oevp)) {
	code = epif_ChangeLink (transId, dap, -1, 0, (u_int *) 0);
	afsl_MBZ(code);
    }

    /* Update parent directory's mtime and ctime */
    vnm_SetMCtime (devp, MARK_MC);

    /* Decrement child's link count */
    code = epif_ChangeLink (transId, oap, lcdelta, 0, (u_int *) 0);
    afsl_MBZ(code);

    /* Update child's ctime */
    vnm_SetMCtime (oevp, MARK_C);

    return (0);
}
/*
 * vnd_Replace -- change a directory entry to point to a different object
 *
 * This function updates link counts, access/mod/change times, and
 *    directory contents, generally by calling lower-level functions.
 *
 * LOCKS:
 * Both the directory and the object to be replaced are assumed to be locked
 *    for writing.  The new object is not assumed to be locked.
 *
 * This function is equivalent to vnd_Delete followed by vnd_Create, but is
 *    more elegant and efficient.
 */

/* SHARED */
int vnd_Replace(
    elbt_TranRec_t transId,		/* transaction ID */
    struct evnode *devp,		/* evnode for directory */
    struct evnode *noevp,		/* evnode for new object */
    struct evnode *ooevp,		/* evnode for old object */
    long opos,				/* piece no. of object in directory */
    int rmdir)				/* old object is expected to get flushed */
{
    int code;				/* error return code */
    struct epi_Dir_Handle ddh;		/* directory handle for devp */
    struct epi_Dir_Handle odh;		/* directory handle for ooevp */
    epi_anode_t ooap;			/* anode handle for ooevp */
    epi_anode_t dap;			/* anode handle for devp */
    epi_anode_t noap;			/* anode handle for noevp */
    int lcdelta;			/* change to object link count */

    ooap = EVTOA (ooevp);
    dap = EVTOA (devp);
    noap = EVTOA (noevp);

    if (rmdir && EV_ISDIR (ooevp)) {	/* deleting a directory */
	/*
	 * if directory has more than the normal number of hard links,
	 * super-user has been fiddling with it.  Don't touch it.
	 */
	if (epif_GetLinkCount (ooap) > 2)
	    return (ENOTEMPTY);

	/* if directory is not empty, can't delete it */
	vndh_SetDirHandle (ooevp, &odh);
	if (!dir_IsEmpty ((struct Dir_Handle *)&odh))
	    return (ENOTEMPTY);

	/* when directory is deleted, don't allow lookup of . or .. */
	ooevp->vd_flags |= VD_DIRDELETED;

	/*
	 * Decrement link count twice if it's a directory, because
	 * it is about to die, and its own ``.'' pointer will go away.
	 */
	lcdelta = -2;
    } else
	lcdelta = -1;

    /* Replace old fid with new fid in parent directory */
    vndh_SetDirHandle (devp, &ddh);
    code = dir_Replace ((struct Dir_Handle *)&ddh, opos, &noevp->vd_fid,
			transId);
    if (code)
	return (code);

    /* Update parent directory's mtime and ctime */
    vnm_SetMCtime (devp, MARK_MC);

    /* Decrement old child's link count */
    code = epif_ChangeLink (transId, ooap, lcdelta, 0, (u_int *) 0);
    MBZ(code);

    /* Update old child's ctime */
    vnm_SetMCtime (ooevp, MARK_C);

    /* Increment new child's link count */
    code = epif_ChangeLink (transId, noap, 1, 0, (u_int *) 0);
    MBZ(code);

    /* Update new child's ctime */
    vnm_SetMCtime (noevp, MARK_C);

    return 0;
}

/* vnd_dirLookup -- called from efs_lookup to find an entry in the a directory.
 *     If the vnode entry cache is valid, then use it to determine whether
 *     entryName exists. If NOT valid, do the lookup and cache the result. 
 *
 * NOTE: Caller's rely on tracing being done by this function. */

/* SHARED */
int vnd_dirLookup(
  struct evnode *devp,		/* directory vnode */
  char *name,			/* name looked up */
  epi_volFileId_t *fid)		/* fid of entry */
{
    long code;
    int hit;				/* hit in lookup cache */
    struct epi_Dir_Handle dh;		/* directory handle for devp */
    long offset;

    hit = 0;
    BEGIN_LOCK_WRITE (&devp->vd_cache.lock) {

	/* Maybe use result of recent lookup */

	if (devp->vd_cache.lookupValid) {
	    if (strcmp(devp->vd_cache.lookupName, name) == 0) {
		hit = 1;
		efsdircache.lHits++;
		*fid = devp->vd_cache.lookupFid;
		if (!epid_IsLegal (fid)) {
		    efsdircache.lHitNoEnt++;
		    code = ENOENT;
		    icl_Trace2(efs_iclSet2, EFS_TRACE_LOOKUP_NEG,
			      ICL_TYPE_POINTER, devp,
			      ICL_TYPE_STRING, name);
		} else {
		    code = 0;
		    icl_Trace4(efs_iclSet2, EFS_TRACE_LOOKUP_POS,
			       ICL_TYPE_POINTER, devp,
			       ICL_TYPE_STRING, name,
			       ICL_TYPE_LONG, epid_ToIndex(fid),
			       ICL_TYPE_LONG, epid_ToUnique(fid));
		}
	    } else if ((name[0] == '.') && (name[1] == '\0')) {

		/* We don't really need to depend on the valid bit to do this
		 * optimizaton, however, this means we actually lookup "." once
		 * in a while. */

		hit = 1;
		efsdircache.lDot++;
		*fid = devp->vd_fid;
		code = 0;
	    }
	} else
	    efsdircache.lMisses++;
    } END_LOCK;
    if (hit)
	return code;

    vndh_SetDirHandle (devp, &dh);
    code = dir_Lookup ((struct Dir_Handle *)&dh, name, fid, &offset);
    if (code == 0)			/* uniquifiers in dirs now obsolete */
	epid_SetDCUnique (fid);
    else if (code == ENOENT) {
	epid_MakeInvalid(fid);
	efsdircache.lMissNoEnt++;
    } else {
	icl_Trace3(efs_iclSet, EFS_TRACE_LOOKUP_DIR_ERROR,
		  ICL_TYPE_POINTER, devp,
		  ICL_TYPE_STRING, name,
		  ICL_TYPE_LONG, code);
	return code;
    }

    if  (strlen(name) < EFS_CACHE_LEN) {
	BEGIN_LOCK_WRITE (&devp->vd_cache.lock) {
	    efsdircache.enters++;
	    devp->vd_cache.lookupValid = 1;
	    strcpy(devp->vd_cache.lookupName, name);
	    devp->vd_cache.lookupOffset = offset;
	    devp->vd_cache.lookupFid = *fid;
	} END_LOCK;
	if (code) 
	    icl_Trace2(efs_iclSet2, EFS_TRACE_LOOKUP_NEG_ENTER,
		       ICL_TYPE_POINTER, devp,
		       ICL_TYPE_STRING, name);
	else
	    icl_Trace4(efs_iclSet2, EFS_TRACE_LOOKUP_POS_ENTER,
		       ICL_TYPE_POINTER, devp,
		       ICL_TYPE_STRING, name,
		       ICL_TYPE_LONG, epid_ToIndex(fid),
		       ICL_TYPE_LONG, epid_ToUnique(fid));
    }
    return code;
}

/*
 * vnd_dirCheck -- validity check on directory and object name to be
 *		   created/removed.
 *
 * We duplicate some of the checks that are done in some kernels' generic
 * code, such as checks of name validity.
 *
 * This will be called at the start of a vnode op, which is called from
 * generic code.  This function is optimized for the case where the generic
 * code has recently called VOP_LOOKUP; if there is valid information from
 * that call in the vd_lookupXXX fields in the vnode, this function uses
 * that information rather than calling dir_Lookup itself.
 *
 * LOCKS:
 *    The caller is responsible for locking the dir object.
 *
 * PARAMETERS:
 *    offsetP:
 *	if object exists, offsetP is set to piece # where it is.
 *	if object not found (ENOENT), offsetP is set to piece # where it
 *	  could be put.
 *    outFlagsP:
 *	DX_OBJEXISTS -- object exists
 *	DX_INHERITFILEACL -- if file were created, it would inherit ACL
 *				from directory
 *	DX_INHERITDIRACL -- if subdir were created, it would inherit ACL
 *				from directory
 */

/* SHARED */
int vnd_dirCheck(
    struct evnode *devp,		/* directory evnode */
    char *entryName,			/* object name */
    osi_cred_t *cred,			/* credential structure */
    epi_volFileId_t *Fidptr,		/* ID of file looked up */
    int inFlags,			/* deleting? (or inserting?) */
    long *offsetP,
    int *outFlagsP)
{
    struct epi_Dir_Handle DirHandle;    /* directory handle for devp */
    struct epif_status dirStat;		/* status info for devp */
    struct epif_status *dirStatP = NULL;
    int hit;				/* found name in lookup cache */
    int code = 0;

    *outFlagsP = 0;
    *offsetP = -1;

    /* Sanity check:  parent must be a directory */
    if (!EV_ISDIR(devp))
	return (ENOTDIR);

    /* Check:  directory has been unlinked */
    if (devp->vd_flags & VD_DIRDELETED)
	return (ENOENT);

    /* Check:  may not add/remove . or .. */
    if (entryName[0] == '.' &&
	(entryName[1] == '\0' || strcmp(entryName, "..") == 0)) {
	efsdircache.cDots++;
	return ((inFlags & DXC_ISDELETE) ? EINVAL : EEXIST);
    }

    /*
     * Explanation of strange code
     *
     * Ordinarily, we do not need to call epif_GetStatus.  This function
     * will be called if necessary by vnax_CanAccess, but if possible,
     * vnax_CanAccess will try to get a result from its cache, and will not
     * need to call epif_GetStatus.
     *
     * If DXC_CREATING is set we have to call epif_GetStatus anyway, to look
     * at some other status fields (see below).  So we call it here and
     * pass a pointer to the status block to vnax_CanAccess.
     */

    /* Verify the access right for the dir */
    if (inFlags & DXC_CREATING) {
	epif_GetStatus(EVTOA(devp), &dirStat);
	dirStatP = &dirStat;
    }
    code = vnax_CanAccess(devp, dirStatP,
		(inFlags & DXC_ISDELETE) ?
		   dacl_perm_execute|dacl_perm_write|dacl_perm_delete :
		   dacl_perm_execute|dacl_perm_write|dacl_perm_insert,
		cred);
    if (code != 0)
	return (code);

    /* Determine if ACL might be inherited */
    if (inFlags & DXC_CREATING) {
	if (dirStat.initialDirAcl != 0)
	    *outFlagsP |= DX_INHERITDIRACL;
	if (dirStat.initialFileAcl != 0)
	    *outFlagsP |= DX_INHERITFILEACL;
    }

    /*
     * Verify the info stored in dir evnode. If valid, then use it to
     * determine whether entryName exists. If NOT valid, do the lookup.
     */
    hit = 0;
    efsdircache.checks++;
    BEGIN_LOCK_WRITE (&devp->vd_cache.lock) {
	if (devp->vd_cache.lookupValid &&
	    (strcmp(devp->vd_cache.lookupName, entryName) == 0)) {

	    /* use result of recent lookup */

	    hit = 1;
	    efsdircache.cHits++;
	    *offsetP = devp->vd_cache.lookupOffset;
	    *Fidptr = devp->vd_cache.lookupFid;
	}
	devp->vd_cache.lookupValid = 0;
    } END_LOCK;
    if (hit) {
	if (epid_IsLegal(Fidptr)) {
	    *outFlagsP |= DX_OBJEXISTS;
	    icl_Trace4(efs_iclSet2, EFS_TRACE_DIRCHECK_POS,
		       ICL_TYPE_POINTER, devp,
		       ICL_TYPE_STRING, entryName,
		       ICL_TYPE_LONG, epid_ToIndex(Fidptr),
		       ICL_TYPE_LONG, epid_ToUnique(Fidptr));
	} else {
	    icl_Trace2(efs_iclSet2, EFS_TRACE_DIRCHECK_NEG,
		       ICL_TYPE_POINTER, devp,
		       ICL_TYPE_STRING, entryName);
	    efsdircache.cHitNoEnt++;
	}
	return 0;
    }

    /* Do the lookup */

    efsdircache.cMisses++;
    vndh_SetDirHandle(devp, &DirHandle);
    code = dir_Lookup((struct Dir_Handle *)&DirHandle, entryName, Fidptr,
		      offsetP);
    if (code == 0) {
	*outFlagsP |= DX_OBJEXISTS;
	epid_SetDCUnique(Fidptr);	/* uniquifiers in dirs now obsolete */
    } else if (code == ENOENT) {
	code = 0;
	efsdircache.cMissNoEnt++;
    }
    return (code);
}


/*
 * If called for local access (islocal), we are expected to return a
 *    format such as struct dirent or struct dirent.
 * If called by the PX (ispx), we are expected to return the struct
 *    dir_minxentry format.  This is similar to struct dirent or struct
 *    dirent, but on some platforms, has an extra offset field.
 * In either case, we use the tempDirect structure.  It is identical to
 *    dir_minxentry.  When this is inappropriate (i.e. on platforms where
 *    they are not identical and we are called for local access), we set
 *    partEntrySize so as to exclude the offset field.
 * If called on behalf of a volume dump or restore, we are expected to
 *    return the struct vol_dirent format.  We use the tempVolDirect
 *    structure.
 */

struct tempDirect {
#ifdef	AFS_SUNOS5_ENV
    ino_t   d_ino;
    off_t   d_off;			/* offset of next directory entry */
#elif	defined(AFS_AIX31_ENV)
    long    d_off;			/* offset of next directory entry */
    u_long  d_ino;
#else
    u_long  d_ino;
#endif
    u_short d_reclen;
#if !defined(AFS_SUNOS5_ENV)
    u_short d_namelen;
#endif
#if !defined(AFS_SUNOS5_ENV) && !defined(AFS_AIX31_ENV)
    long    d_off;
#endif
    char d_dummy;
};

struct tempVolDirect {
    int32	offset;			/* Logical directory offset */
    int32	vnodeNum;		/* Canonical vnode number */
    u_int32	codesetTag;		/* Tag for the name's codeset */
    u_short	reclen;			/* Len of this rec, 4-byte boundary */
    u_short	namelen;		/* Length of `name' */
};

/*
 * readEntry -- read an individual entry
 */
static int
readEntry(
    caddr_t partEntry,			/* partial directory entry ptr */
    unsigned partEntrySize,		/* size of partial entry */
    char *name,				/* entry name */
    unsigned nameLen,			/* length of entry name */
    unsigned recLen,			/* length of entire record */
    struct uio *uiop)			/* uio structure */
{
    int code;				/* error return code */
    int padCount;			/* # of nulls to pad name */
    static char padbuf[4] = {'\0', '\0', '\0', '\0'};

    afsl_Assert(strlen(name) == nameLen);
    afsl_Assert(nameLen <= OSI_MAXNAMLEN);

    /* Read fid and bookkeeping fields */
    code = osi_uiomove ((caddr_t) partEntry, partEntrySize, UIO_READ, uiop);
    if (code)
	goto done;

    /* Read name */
    code = osi_uiomove (name, nameLen, UIO_READ, uiop);
    if (code)
	goto done;

    /* Pad to fullword length; pad includes null terminator */
    padCount = ((partEntrySize + nameLen + 4) & ~3)
		    - (partEntrySize + nameLen);
    code = osi_uiomove (padbuf, padCount, UIO_READ, uiop);

    afsl_Assert(recLen == partEntrySize + nameLen + padCount);

done:
    return code;
}
#define ROUNDUP4(x) (((x) + 3) & (-4))

#define VOLDIRENTRY_SIZE(len) \
    (ROUNDUP4(offsetof(struct vol_dirent, name) + (len) + 1))

#define LDIRENTRY_SIZE(len) \
    (ROUNDUP4(offsetof(struct dirent, d_name) + (len) + 1))

#define XDIRENTRY_SIZE(len) \
    (ROUNDUP4(offsetof(struct tempDirect, d_dummy) + (len) + 1))

#define isvolop		(caller == 1)
#define ispx		(caller == 2)
#define islocal		(caller == 0)

/* Must handle ispx -- for now, same as iskernel */

/* ARGSUSED */
int vnd_Read(
    efs_vnode_t devp,			/* target directory vnode */
    struct uio *uiop,			/* pointer to uio data structure */
    osi_cred_t  *cred,			/* who I am  */
    int caller,
    int *numEntriesp,
    int *eofp)				/* reached EOF? */
{
    struct iovec *iovp;			/* io data structure */
    int count;				/* user provided buffer size */
    struct Dir_Entry *entryPtr;		/* pointer to the directory entry */
    struct epi_Dir_Handle dirHandle;	/* directory handle for devp */
    struct tempDirect tempDirect;
    struct tempVolDirect tempVolDirect;
    caddr_t partEntry;			/* partial generic directory record */
    unsigned partEntrySize;
    struct Dir_Page *dbP, *ndbP;	/* directory page buffer pointers */
    long pieceNo, nextPiece;		/* directory entry numbers */
    char *oldName;
    int oldNameLen;
    int nameLen, DirEntrySize, dirEmpty = 1;
    int dirLen, newOffset;
    int code = 0;
    int numEntries = 0;
    int eof;
    unsigned recLen;

    if (!eofp)				/* if we didn't get one, use dummy */
	eofp = &eof;

    *eofp = 0;

    /* Check: directory has been unlinked */
    if (devp->vd_flags & VD_DIRDELETED) {
	*eofp = 1;
	if (numEntriesp)
	    *numEntriesp = 0;
	return 0;
    }

    /*
     * This code assumes only one iov.
     */
    if (uiop->uio_iovcnt != 1)
	return (EINVAL);
    iovp = uiop->uio_iov;
    count = iovp->iov_len;

    dbP = (struct Dir_Page *)0;

    /* Obtain lock on directory */
    tlock_ObtainRead (&devp->vd_tlock);

    /* Check: request starts beyond EOF */
    dirLen = AFS_hgetlo(epia_GetLength (EVTOA (devp)));
    if (uiop->uio_offset >= dirLen) {
	*eofp = 1;
	goto out;
    }

    nextPiece = DIR_OFF_TO_PIECE(uiop->uio_offset);
    vndh_SetDirHandle(devp, &dirHandle);

    if (isvolop) {
	partEntry = (caddr_t)&tempVolDirect;
	partEntrySize = offsetof(struct vol_dirent, name);
    } else {
	partEntry = (caddr_t)&tempDirect;
	if (islocal)
	    partEntrySize = offsetof(struct dirent, d_name);
	else
	    partEntrySize = offsetof(struct tempDirect, d_dummy);
    }

    /* Loop over directory entries */
    while (1) {
	/* Find next entry */
	pieceNo = dir_PieceScan((struct Dir_Handle *)&dirHandle, nextPiece);
	if (pieceNo == 0) {
	    *eofp = 1;
	    if (dirEmpty)
		goto out;
	    else {
		/*
		 * End of the directory.  Modify the d_reclen of
		 * previous dir entry to include the remaining bytes.
		 */
		if (isvolop) {
		    tempVolDirect.reclen += count;
		} else {
		    tempDirect.d_reclen += count;
		}
		newOffset = dirLen;
		break;
	    }
	}
	entryPtr = dir_GetPiece(
	    (struct Dir_Handle *)&dirHandle, pieceNo, &ndbP);
	nameLen = strlen((char *)entryPtr->name);

	DirEntrySize =
	    isvolop ? VOLDIRENTRY_SIZE(nameLen) :
		(ispx ? XDIRENTRY_SIZE(nameLen) : LDIRENTRY_SIZE(nameLen));
	/* Does the new entry fit into the remaining user buffer? */
	if (DirEntrySize > count) {
	    /*
	     * It won't. So, the previous directory entry will be the last
	     * entry in user buffer.
	     */
	    newOffset = DIR_PIECE_TO_OFF(pieceNo);
	    dirb_Release((caddr_t)ndbP);
	    if (!dirEmpty)
		if (isvolop) {
		    tempVolDirect.reclen += count;
		} else {
		    tempDirect.d_reclen += count;
		}
	    break;
	}

	/* next dir entry will fit; flush last one and go round again */
	nextPiece = pieceNo + entryPtr->length;
	if (!dirEmpty) {
	    afsl_Assert(!isvolop ||
			 (tempVolDirect.namelen <= OSI_MAXNAMLEN
			  && partEntry == (caddr_t)&tempVolDirect
			  && partEntrySize == sizeof tempVolDirect
			  && oldNameLen == tempVolDirect.namelen));
	    code = readEntry(partEntry, partEntrySize, oldName, oldNameLen,
			     recLen, uiop);
	    if (code)
		goto out;
	    numEntries++;
	    dirb_Release((caddr_t)dbP);
	}
	dbP = ndbP;
	/*
	 * Copy the efs dir entry to the user buffer in UNIX format
	 * We set next disk dir entry, d_off, but may revise it later
	 */
	if (isvolop) {
	    tempVolDirect.offset =
		DIR_PIECE_TO_OFF(nextPiece - entryPtr->length);
	    tempVolDirect.vnodeNum =
		EFS_EVNUM_TO_NET(epid_ToIndex(&entryPtr->fid));
	    tempVolDirect.codesetTag = 0;
	    tempVolDirect.reclen = DirEntrySize;
	    tempVolDirect.namelen = nameLen;
	} else {
	    tempDirect.d_off = DIR_PIECE_TO_OFF(nextPiece);
	    tempDirect.d_ino = epif_FidToShortHandle(&entryPtr->fid);
	    tempDirect.d_reclen = DirEntrySize;
#if !defined(AFS_SUNOS5_ENV)
	    tempDirect.d_namelen = nameLen;
#endif
	}
	oldName = (char *) entryPtr->name;
	oldNameLen = nameLen;
	dirEmpty = 0;
	count -= DirEntrySize;
	recLen = DirEntrySize;
    }  /* while */

    /* Flush last entry */
    afsl_Assert(!isvolop ||
		(tempVolDirect.namelen <= OSI_MAXNAMLEN
		 && partEntry == (caddr_t)&tempVolDirect
		 && partEntrySize == sizeof tempVolDirect
		 && oldNameLen == tempVolDirect.namelen));
    code = readEntry (partEntry, partEntrySize, oldName, oldNameLen, recLen,
		      uiop);
    if (code)
	goto out;
    numEntries++;

    /* Reset uio_offset in dir to reflect real offset from user's perspective */
    uiop->uio_offset = newOffset;

out:
    if (dbP)
	dirb_Release((caddr_t)dbP);

    /* Update cached atime if not a volop */
    if (!isvolop)
	vnm_SetAtime (devp);

    /* Release lock on directory */
    tlock_ReleaseRead (&devp->vd_tlock);

    if (!code && numEntriesp != 0)
	*numEntriesp = numEntries;

    return(code);
}
/*
 * vnd_CheckFilenameLength -- Check the validity of object name length.
 */
int
vnd_CheckFilenameLength(char *name)
{
    int len = strlen(name);

    if (len == 0)
	return ENOENT;
    else if (len > EFS_MAX_FILENAME_LENGTH)
	return ENAMETOOLONG;
    else
	return 0;
}

/*
 * The following structure records the current state of
 * vnd_Rename to allow clean-up on exit.
 */
struct vnd_RenameState {
    int flags;				/* flags -- see below */
    struct evnode *sDir, *tDir;		/* src and target dirs */
    struct evnode *sObj, *tObj;		/* src and target objects */
    epi_volFileId_t sFid, tFid;		/* FID's */
    long sOff, tOff;			/* Directory offsets */
};

typedef struct vnd_RenameState vnd_RenameState_t;

/*
 * Flag definitions.
 */
#define VND_SD_LOCKED	0x1
#define VND_TD_LOCKED	0x2
#define VND_SO_LOCKED	0x4
#define VND_TO_LOCKED	0x8
#define VND_DIR_MOVE	0x10

static void
RenameCleanup(vnd_RenameState_t *state)
{
     if (state->flags & VND_SD_LOCKED)
	tlock_ReleaseWriteNT(&state->sDir->vd_tlock);

     if (state->flags & VND_TD_LOCKED)
	tlock_ReleaseWriteNT(&state->tDir->vd_tlock);

     if (state->flags & VND_SO_LOCKED) {
	afsl_Assert(state->sObj != NULL);
	tlock_ReleaseWriteNT(&state->sObj->vd_tlock);
    }

     if (state->flags & VND_TO_LOCKED) {
	afsl_Assert(state->tObj != NULL);
	tlock_ReleaseWriteNT(&state->tObj->vd_tlock);
    }

    if (state->sObj != NULL) {
	VN_RELE(EVTOV(state->sObj));
	state->sObj = NULL;
    }

    if (state->tObj != NULL) {
	VN_RELE(EVTOV(state->tObj));
	state->tObj = NULL;
    }

    state->flags = 0;
}

/*
 * Locking routine used by ValidateRename and its helpers.  It first
 * checks the state structure to see if we already have the lock.  If
 * not and we hold no locks yet, then it safely uses the normal blocking
 * call.  Otherwise, it tries to obtain it without blocking.  If the
 * lock is held by someone else, we call RenameCleanup (thus dropping
 * whatever locks we currently hold), and block waiting for it.  In this
 * case, we return 0, otherwise 1.
 */
static int
LockSafely(vnd_RenameState_t *state, struct evnode *evp, int lockBit)
{
    if (state->flags & lockBit)
	return 1;			/* already locked */
    else if (state->flags == 0) {
	tlock_ObtainWriteNT(&evp->vd_tlock); /* can safely block if held */
	state->flags |= lockBit;
	return 1;
    } else if (tlock_ObtainWriteNoBlockNT(&evp->vd_tlock)) {
	state->flags |= lockBit;
	return 1;
    } else {

	 /* Make sure evp stays with us after the cleanup. */

	 FAST_VN_HOLD(evp);
	 RenameCleanup(state);
	 tlock_ObtainWriteNT(&evp->vd_tlock);
	 tlock_ReleaseWriteNT(&evp->vd_tlock);
	 VN_RELE(EVTOV(evp));
	 return 0;
     }
}

/*
 * Linked list of vnodes.  CheckPath uses this to keep track
 * of locked directories.
 */
struct vnd_vplist {
    struct vnd_vplist *next;
    struct evnode *evp;
};

typedef struct vnd_vplist vnd_vplist_t;

static void
FreeLockedDirs(vnd_vplist_t *locked_dirs)
{
    while (locked_dirs != NULL) {
	vnd_vplist_t *next = locked_dirs->next;
	tlock_ReleaseWriteNT(&locked_dirs->evp->vd_tlock);
	VN_RELE(EVTOV(locked_dirs->evp));
	osi_Free_r(locked_dirs, sizeof (*locked_dirs));
	locked_dirs = next;
    }
}

/*
 * CheckPath -- check if source directory is in path of target
 *
 * This is called from ValidateRename when a directory is going
 * to get a new parent and checks whether the source is an ancestor of
 * the target.  It returns 0 if there's no problem, or an error code if
 * there is.  It checks by stepping up the hierarchy from target,
 * until either the (fileset) root or source's parent is reached.
 *
 * On entry, the target is locked for writing.  As we move up the directory
 * tree, we try locking each parent directory without blocking.  If at
 * any point, we find that we must block, we drop all the locks acquired
 * here and elsewhere in ValidateRename and start all over.  Thus,
 * we could spend an unbounded amount of time trying to perform this
 * check.
 */
static int
CheckPath(vnd_RenameState_t *state)
{
    struct evnode *tevp = state->tDir;	/* current vnode */
    struct evnode *nextvp;
    struct epi_Dir_Handle tdh;		/* directory handle for tevp */
    epi_volFileId_t nfid;		/* fid for next anode */
    int code = 0;			/* error return code */
    long offset;			/* dummy arg for dir_Lookup */
    vnd_vplist_t *locked_dirs = NULL;

    if (state->sObj == tevp)
	return EINVAL;

    /*
     * Search upward until we reach the root or the source's parent.
     */
    while (!(tevp->vd_flags & VD_ROOT)) {

	/* Look up parent directory */
	vndh_SetDirHandle(tevp, &tdh);
	code = dir_Lookup((struct Dir_Handle *)&tdh, "..", &nfid, &offset);
	if (code != 0) {
	    code = ENOTDIR;	/* XXX Why? */
	    break;
	}
	epid_SetDCUnique(&nfid);	/* uniquifiers in dirs now obsolete */
	code = vnm_FindVnode(tevp->vd_volp, &nfid, &nextvp);
	if (code != 0)
	    break;

	/* Compare target with source */
	if (state->sObj == nextvp) {
	    VN_RELE(EVTOV(nextvp));
	    code = EINVAL;
	    break;
	} else if (nextvp == state->sDir) {
	    VN_RELE(EVTOV(nextvp));
	    break;
	}

	/*
	 * Obtain lock on parent directory and add to list; if we can't
	 * get the lock without blocking, wait for lock and then bail out.
	 */
	if (tlock_ObtainWriteNoBlockNT(&nextvp->vd_tlock)) {
	    vnd_vplist_t *v = osi_Alloc_r(sizeof (*v));
	    v->evp = nextvp;
	    v->next = locked_dirs;
	    locked_dirs = v;
	} else {
	    FreeLockedDirs(locked_dirs);
	    locked_dirs = NULL;
	    RenameCleanup(state);
	    tlock_ObtainWriteNT(&nextvp->vd_tlock);
	    tlock_ReleaseWriteNT(&nextvp->vd_tlock);
	    VN_RELE(EVTOV(nextvp));
	    break;
	}
	tevp = nextvp;		/* continue up to next level */
    }

    /*
     * Drop locks on parent directories that we acquired
     * during search and release them.
     */
    FreeLockedDirs(locked_dirs);

    return code;
}

/*
 * ValidateRename -- This function does all the checks needed to
 * make sure that a requested rename operation is legal.  Returns with
 * both the source and target directories and objects locked if everything
 * is OK.
 *
 * LOCKING --
 *
 * No vnode locks should be held on entry.  If the requested rename
 * is legal, both the source and target directories will be locked
 * on return.
 *
 * Because we must obtain at least two, and possibly many, vnode
 * locks in the course of our checks, we must be careful to avoid
 * deadlock.  We do this by obtaining each lock without blocking,
 * and dropping all the locks and starting over again when blocking
 * is necessary.  This leaves us vulnerable to starvation, although
 * we do not think that this will be a serious concern in practice.
 */

/* To check for identical source and target objects */
#define EIDENTICAL 0x1000

int
ValidateRename(
  vnd_RenameState_t *state,
  char *sname,			/* Source name */
  char *tname,			/* Target name */
  osi_cred_t *cred)
{
    int code;
    int done = 0;
    int nLoops = 0;
#define VND_LOG_RETRY_INTERVAL	16
#define VND_MAXRENAME_LOOPS	16000

    struct evnode *sd = state->sDir, *td = state->tDir;
    struct evnode *so, *to;
    int dxflags;

    code = vnd_CheckFilenameLength(tname);
    if (code != 0)
	return code;

    while (!done) {
	/*
	 * Sanity check: give up after an inordinate number of retries.
	 */
	if (++nLoops >= VND_LOG_RETRY_INTERVAL) {
	    if ((nLoops % VND_LOG_RETRY_INTERVAL) == 0) {
		icl_Trace3(efs_iclSet, EFS_TRACE_RENAME_LOOP,
			   ICL_TYPE_STRING, sname,
			   ICL_TYPE_STRING, tname,
			   ICL_TYPE_LONG, nLoops);
	    }
	    if (nLoops > VND_MAXRENAME_LOOPS)
		return EIO;
	}

	/*
	 * Lock the source directory.
	 */
	if (!LockSafely(state, sd, VND_SD_LOCKED))
	     continue;

	/*
	 * Check that source object exists and get its vnode
	 */
	code = vnd_dirCheck(sd, sname, cred, &state->sFid, DXC_ISDELETE,
			    &state->sOff, &dxflags);
	if (code == 0 && !(dxflags & DX_OBJEXISTS))
	    code = ENOENT;

	if (code != 0)
	    break;

	code = vnm_FindVnode(sd->vd_volp, &state->sFid, &state->sObj);
	if (code != 0)
	    break;

	so = state->sObj;
	if (!LockSafely(state, so, VND_SO_LOCKED))
	    continue;

	/* Check for sticky bit */
	code = vnax_StickyCheck(sd, so, cred);
	if (code != 0)
	    break;

	if (so == td) {
	    code = EINVAL;
	    break;
	}

	/*
	 * Lock the target directory.
	 */
	if (td != sd && !LockSafely(state, td, VND_TD_LOCKED))
	    continue;

	/* Check: renaming to different volume */
	if (so->vd_volp != td->vd_volp) {
	    code = EXDEV;
	    break;
	}

	/*
	 * Determine if renaming directory into a different directory.
	 * If so, we will have to change its `..' pointer, and hence
	 * must have write access
	 */
	if (sd != td && EV_ISDIR(so)) {
	    state->flags |= VND_DIR_MOVE;
	    code = vnax_CanAccess(so, NULL, dacl_perm_write, cred);
	    /* Check: renaming a directory to be its own descendant */
	    if (code == 0)
		code = CheckPath(state);
	    if (code != 0)
		break;

	    if (state->flags == 0)	/* CheckPath dropped our locks */
		continue;
	}

	/* Determine if target object exists */
	code = vnd_dirCheck(td, tname, cred, &state->tFid, 0,
			    &state->tOff, &dxflags);
	if (code != 0)
	    break;

	if (dxflags & DX_OBJEXISTS) {		/* target object exists */
	    /* Get vnode representing target object */
	    code = vnm_FindVnode(td->vd_volp, &state->tFid, &state->tObj);
	    if (code != 0)
		break;

	    to = state->tObj;
	    /* Check: source and target object identical */
	    if (so == to) {
		code = EIDENTICAL;
		break;	/* Do nothing, return EIDENTICAL code */
	    }
	    else if (to == sd) { /* Check: target is the source directory */
		code = ENOTEMPTY;
		break;
	    }

	    if (!LockSafely(state, to, VND_TO_LOCKED))
		continue;

	    /* Check: one is a directory, the other is not */
	    if (EV_ISDIR(so) && !EV_ISDIR(to))
		code = ENOTDIR;
	    else if (!EV_ISDIR(so) && EV_ISDIR(to))
		code = EISDIR;

	    if (code != 0)
		break;

	    /* Check for sticky bit */
	    code = vnax_StickyCheck(td, to, cred);
	    if (code != 0)
		break;
	}

	done = 1;	/* All done! */
    }

    if (!done)
	RenameCleanup(state);

    return code;
}

int
vnd_Rename(
    struct evnode *sd,			/* source directory */
    char *sname,			/* name of source object */
    struct evnode *td,			/* target directory */
    char *tname,			/* name of target object */
    osi_cred_t *cred)			/* credential structure */
{
    vnd_RenameState_t rState;
    logTranRec_t transId;		/* transaction ID */
    int code, mbz;			/* error return codes */
    struct epi_Dir_Handle dh;		/* miscellaneous dir handle */
    int enterTarg = 0, enterDotDot = 0;
    struct evnode *so;

    rState.sDir = sd;
    rState.tDir = td;
    rState.sObj = rState.tObj = NULL;
    rState.flags = 0;


    code = ValidateRename(&rState, sname, tname, cred);
    if (code != 0) {
	/*
	 * Check for identical source and target objects. The glue
	 * already checks for this, but user space code, test_vnodeops,
	 * does not go through that path.
	 */
	if (code == EIDENTICAL)
	    code = 0;
	return code;
    }

    so = rState.sObj;
    osi_dnlc_remove_r(EVTOV(sd), sname);
    if (rState.tObj != NULL)
	osi_dnlc_remove_r(EVTOV(td), tname);

    mbz = epia_StartTran("vnd_Rename", EVTOVH(sd), &transId);
    afsl_MBZ(mbz);

    if (rState.tObj != NULL) {
	/*
	 * Replace target object with source object in target directory
	 * This will check the case that to is a non-empty directory
	 */
	code = vnd_Replace(transId, td, rState.sObj,
			   rState.tObj, rState.tOff, 1);
    } else {
	/* Insert source object in target directory */
	code = vnd_Create(transId, td, rState.sObj, tname,
			  rState.tOff, DC_CHANGELINK | DC_MARKCTIME);
    }

    if (code == 0) {		 /* Delete so from sd */
	enterTarg = 1;
	/* Merge transaction */
	tlock_EquivAssign(transId, &so->vd_tlock);
	/* Unlink source object from source directory */
	code = vnd_Delete(transId, sd, so, rState.sOff, 0);
    }

    if (code == 0 && (rState.flags & VND_DIR_MOVE)) {
	/*
	 * Need to change `..' entry in source object; invalidate
	 * cached directory entry in case it is ".."
	 */
	BEGIN_LOCK_WRITE (&so->vd_cache.lock) {
	    so->vd_cache.lookupValid = 0;
	} END_LOCK;

	icl_Trace1(efs_iclSet2, EFS_TRACE_RENAME_INVALIDATE_DIRCACHE_1,
		  ICL_TYPE_POINTER, so);
	icl_Trace4(efs_iclSet2, EFS_TRACE_RENAME_INVALIDATE_DIRCACHE_2,
		   ICL_TYPE_POINTER, sd,
		   ICL_TYPE_STRING, sname,
		   ICL_TYPE_POINTER, td,
		   ICL_TYPE_STRING, tname);

	vndh_SetDirHandle(so, &dh);
	mbz = dir_FixDotDot((struct Dir_Handle *)&dh, &td->vd_fid, transId);
	afsl_MBZ(mbz);
	enterDotDot = 1;
    }

    /*
     * Each VN_RELE can potentially start a transaction, so they are
     * sequenced after the rename transaction has ended.  Note that
     * dnlc insertions can also cause VN_RELE's.
     */
    mbz = epia_EndTran(EVTOVH(sd), transId);
    afsl_MBZ(mbz);

    if (efs_SyncMetaData(sd)) {
	mbz = elbb_LogForce(transId, 1/*commit*/);
	afsl_MBZ(mbz);
    }

    RenameCleanup(&rState);

    if (enterTarg)
	osi_dnlc_enter_r(EVTOV(td), tname, EVTOV(so));

    if (enterDotDot) {
	osi_dnlc_remove_r(EVTOV(so), "..");
	osi_dnlc_enter_r(EVTOV(so), "..", EVTOV(td));
    }

    return (code);
}
