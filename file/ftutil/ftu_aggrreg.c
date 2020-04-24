/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ftu_aggrreg.c,v $
 * Revision 1.1.21.1  1996/10/02  17:48:31  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:30  damon]
 *
 * Revision 1.1.16.3  1994/07/13  22:28:11  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:35:23  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:15  mbs]
 * 
 * Revision 1.1.16.2  1994/06/09  14:09:10  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:23:49  annie]
 * 
 * Revision 1.1.16.1  1994/02/04  20:19:37  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:33  devsrc]
 * 
 * Revision 1.1.14.3  1994/01/20  18:43:09  annie
 * 	added copyright header
 * 	[1994/01/20  18:39:24  annie]
 * 
 * Revision 1.1.14.2  1993/12/16  17:14:11  jaffe
 * 	Transarc Delta: berman-o-ot9620-make-LookupAggrByDevice-compare-rawdevs 1.2
 * 	    Call ftu_GetRawDeviceName() on the aggrP->dvname
 * 	    and stat the raw name for comparison.  This will
 * 	    allow it to work when char and block devices have
 * 	    different major/minor pairs.
 * 	    Carefully handle GetRawDeviceName return code.
 * 	    Only set code if failure.
 * 	[1993/12/16  14:32:45  jaffe]
 * 
 * Revision 1.1.14.1  1993/12/07  17:25:33  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:05:23  jaffe]
 * 
 * $EndLog$
 */

/*
 * (C) Copyright 1995, 1990, 1991, 1992 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

#include <string.h>
#include <unistd.h>

#include <dcedfs/stds.h>
#include <dcedfs/syscall.h>
#include <dcedfs/agfs_mount.h>
#include <dcedfs/xvfs_vnode.h>	/* for definition of osi_statfs */
#include <dcedfs/osi_mount.h>

#include <ftutil.h>
#include <ftu_syscalls.h>

#if !defined(KERNEL)
#include <pthread.h>
#endif /* !KERNEL */

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ftutil/ftu_aggrreg.c,v 1.1.21.1 1996/10/02 17:48:31 damon Exp $")

/*
 * Constants
 */
#define AGGR_MOUNT_DIR	DCELOCAL_PATH "/var/dfs/aggrs"
#define DIR_MODE	0755

/*
 * External declarations
 */
/*
 * _ftu_Unmount()
 *	Performs the platform-specific part of the unmount operation.
 */
IMPORT long _ftu_Unmount _TAKES((
    IN char*	mountPoint
));

/*
 * Forward declarations
 */
/*
 * MakePath()
 *	Ensures that the given path exists.
 */
static long MakePath _TAKES((
    IN char*	path
));

/*
 * Mounts an aggregate
 */
static long MountAggr _TAKES((
    IN struct astab*	astab,
    IN char*		mountPoint
));

/*
 * Unmounts an aggregate
 */
static long UnmountAggr _TAKES((
    IN char*	mountPoint
));

/*
 * The functionality of each of the following exported routines is described
 * in the header file.  The comments below only give implementation
 * specifics, where necessary.
 */


/******************************************************************************
 * Aggregate registration routines
 *****************************************************************************/

/*
 * ftu_AttachAggr()
 *	Cobbles together an astab structure, and calls
 *	ftu_AttachAggrWithAstab().  Many of this routines documented error
 *	codes are propagated from that function.
 */
EXPORT long
ftu_AttachAggr(devName, aggrType, aggrName, aggrId, flags, fstabInfo)
    IN char*		devName;
    IN char*		aggrType;
    IN char*		aggrName;
    IN ftu_aggrId_t	aggrId;
    IN u_long		flags;
    IN caddr_t		fstabInfo;
{
    struct astab	astab;
    long		code;

    if (strlen(devName) > (size_t)FTU_MAX_DEV_NAME)
	return FTU_E_NAME_TOO_LONG;
    strncpy(astab.as_spec, devName, FTU_MAX_DEV_NAME);
    astab.as_spec[FTU_MAX_DEV_NAME] = '\0';/* strncpy may leave unterminated */

    if (strlen(aggrName) > (size_t)FTU_MAX_AGGR_NAME)
	return FTU_E_NAME_TOO_LONG;
    strncpy(astab.as_aggrName, aggrName, ASTABSIZE_DIR);
    astab.as_aggrName[FTU_MAX_AGGR_NAME] = '\0';

    strncpy(astab.as_type, aggrType, ASTABSIZE_TYPE);
    astab.as_aggrName[ASTABSIZE_TYPE - 1] = '\0';

    astab.as_aggrId = aggrId;

    code = ftu_AttachAggrWithAstab(&astab, flags, fstabInfo);

    return code;
}	/* ftu_AttachAggr() */


/*
 * ftu_AttachAggrWithAstab()
 *	Perform various consistency checks and then call the system call,
 *	remapping ENXIO to FTU_E_TRY_RECOVERY if we're attempting to attach
 *	an LFS aggregate.  We also mount the aggregate.
 */
EXPORT long
ftu_AttachAggrWithAstab(astabP, ftuFlags, fstabInfo)
    IN struct astab*	astabP;
    IN u_long		ftuFlags;
    IN caddr_t		fstabInfo;
{
    long		code;
    int			devFd;
    int			doLock = 1;
    int			mountAggr = (ftuFlags & FTU_ATTACH_FLAGS_MOUNTAGGR);
    int			noExport = (ftuFlags & FTU_ATTACH_FLAGS_NOEXPORT);
    char		rawDevName[MAXPATHLEN+1];
    char 		mountPoint[MAXPATHLEN+1];
    struct stat		statBuf;
    u_long		xaggrFlags = 0;


    if (noExport) {
	afsl_PAssertTruth(!mountAggr,
			  ("MOUNTAGGR and NOEXPORT are mutually exclusive"));
	xaggrFlags |= AGGR_ATTACH_NOEXPORT;
    }
#if 0
    else {
	/*
	 * See if this device is already attached; then check for ID or name
	 * clashes.
	 */
	code = ftu_LookupAggrByDevice(astabP->as_spec, (ftu_aggrId_t*)0);
	if (code != FTU_E_NOT_ATTACHED)
	    return code == 0 ? FTU_E_ALREADY_ATTACHED : code;

	code = ftu_AggrGetInfo(astabP->as_aggrId, (struct aggrInfo*)0);
	if (code != FTU_E_NOT_ATTACHED)
	    return code == 0 ? FTU_E_ID_CLASH : code;

	code = ftu_LookupAggrByName(astabP->as_aggrName, (ftu_aggrId_t*)0);
	if (code != FTU_E_NOT_ATTACHED)
	    return code == 0 ? FTU_E_NAME_CLASH : code;
    }
#endif

    if (code = ftu_GetRawDeviceName(astabP->as_spec, rawDevName))
	return code;

    if ((devFd = open(rawDevName, O_RDWR)) == -1)
	if (errno == EROFS)
	    doLock = 0;
        else
	    return errno;

    if (doLock && osi_ExclusiveLockNoBlock(devFd) == -1) {
	code = errno;
	(void)close(devFd);
	if ((code == EAGAIN || code == EACCES))
	    return FTU_E_DEVICE_LOCKED;
        else
	    return code;
    }

    if (mountAggr && ASTABTYPE_SUPPORTS_EFS(astabP->as_type)) {

	(void)sprintf(mountPoint, "%s%s%s",
		      AGGR_MOUNT_DIR,
		      astabP->as_aggrName[0] != '/' ? "/" : "",
		      astabP->as_aggrName);

	code = MakePath(mountPoint);

	if (!code)
	    code = osi_mount(astabP->as_spec, mountPoint, OSI_MOUNT_TYPE_AGFS,
			     0, NULL, astabP, sizeof *astabP);
	/*
	 * Mount will fail with EBUSY if aggr already mounted;
	 * inform the caller with the same error
	 * code AG_ATTACH() would tell us if it got EEXIST.
	 * mount can also fail if the aggr needs to be salvaged.
	 * Do the same thing as AG_ATTACH would do if it got ENXIO.
	 */
	if (code == EBUSY)
	    return FTU_E_ALREADY_ATTACHED;
	else if (code == ENXIO)
	    return FTU_E_TRY_RECOVERY;
	else if (code)
	    return code;
    }

    if (code = AG_ATTACH(astabP, xaggrFlags, fstabInfo)) {
	if (code == ENXIO &&
	    ASTABTYPE_SUPPORTS_EFS(astabP->as_type)) {
	    code = FTU_E_TRY_RECOVERY;
	} else {
	    if (code == EEXIST)
		code = FTU_E_ALREADY_ATTACHED;
	}

	/* have to undo the mount operation above */
	if (mountAggr && ASTABTYPE_SUPPORTS_EFS(astabP->as_type)) {
	    (void) osi_umount(mountPoint, 0);
	}
	(void)osi_UnLock(devFd);
	(void)close(devFd);
	return code;
    }

    if (doLock && osi_UnLock(devFd) == -1) {
	code = errno;
	(void)close(devFd);
	return code;
    }

    if (doLock && close(devFd) == -1)
	return errno;

    return 0;
}	/* ftu_AttachAggrWithAstab() */


/*
 * MakePath()
 */
static long
MakePath(char *path)
{
    char*	cp;
    char	privatePath[MAXPATHLEN + 1];

    /* Make a private copy, so we can play with it */
    (void)strncpy(privatePath, path, MAXPATHLEN);
    privatePath[MAXPATHLEN] = '\0';

    /* Skip leading slash, if any */
    cp = (privatePath[0] == '/' ? &privatePath[1] : &privatePath[0]);

    for (;;) {
	cp = strchr(cp, '/');
	if (cp != NULL)
	    *cp = '\0';			/* Terminate at the next slash */

	/* Make the directory at the next level, ignoring EEXIST */
	if (mkdir(privatePath, DIR_MODE) == -1
#ifdef AFS_AIX31_ENV
	/* mkdir will fail with EROFS if the aggr (or something else RO)
	 * is already mounted there on RIOS.  This is okay for the last part.
	 */
	    && errno != EROFS
#endif /* AFS_AIX31_ENV */
	    && errno != EEXIST)
	    return errno;

	if (cp == NULL)			/* We're done */
	    break;

	*cp++ = '/';			/* Put the slash back, and move on */
    }

    return 0;
}	/* MakePath() */


/*
 * ftu_DetachAggr()
 *	Unmount the aggregate and then detach it.
 */
EXPORT long
ftu_DetachAggr(aggrId, ftuFlags)
    IN ftu_aggrId_t	aggrId;
    IN u_long		ftuFlags;
{
    long	code;
    int		mountAggr = (ftuFlags & FTU_ATTACH_FLAGS_MOUNTAGGR);
    int		noExport = (ftuFlags & FTU_ATTACH_FLAGS_NOEXPORT);
    u_long	xaggrFlags = 0;
    char	mountPoint[MAXPATHLEN + 1];
    struct aggrInfo	aggrInfo;

    if (noExport) {
	afsl_PAssertTruth(!mountAggr,
			  ("MOUNTAGGR and NOEXPORT are mutually exclusive"));
	xaggrFlags |= AGGR_ATTACH_NOEXPORT;
    }
    if (ftuFlags & FTU_ATTACH_FLAGS_FORCEDETACH)
	xaggrFlags |= AGGR_ATTACH_FORCE;

    if (mountAggr) {
	if (code = ftu_AggrGetInfo(aggrId, &aggrInfo))
	    return code;

	if (AG_TYPE_SUPPORTS_EFS(aggrInfo.type)) {
	    dev_t		devNum;
	    struct stat		statBuf;
	    osi_statfs_t	statfsBuf;
	    long		vfsType;

	    if (stat(aggrInfo.devName, &statBuf) == -1)
		return errno;

	    (void)sprintf(mountPoint, "%s%s%s",
			AGGR_MOUNT_DIR,
			aggrInfo.name[0] != '/' ? "/" : "",
			aggrInfo.name);

	    if (statfs(mountPoint, &statfsBuf) == -1)
		return errno;

	    devNum = osi_statfs_devnum(&statfsBuf);
	    vfsType = osi_statfs_vfstype(&statfsBuf);

	    /* If the the aggregate is mounted, must unmount it to detach */
	    if (devNum == statBuf.st_rdev &&
#if !defined(AFS_SUNOS5_ENV) && !defined(AFS_HPUX_ENV)
		/* sunos has no static vfs mount types... */
		vfsType == OSI_MOUNT_TYPE_AGFS &&
#endif /* SUNOS5 && HPUX */
		(code = osi_umount(mountPoint, 0)))
		return code;
	}
    }

    code = AG_DETACH(aggrId, xaggrFlags);
    if (code == EBUSY) { /* Put the mount back if detach failed */
	if (mountAggr) {
	    if (AG_TYPE_SUPPORTS_EFS(aggrInfo.type)) {
		struct astab	astab;

		strcpy(astab.as_spec, aggrInfo.devName);
		strcpy(astab.as_aggrName, aggrInfo.name);
		strcpy(astab.as_type, "lfs");
		astab.as_aggrId= aggrId;
		(void) osi_mount(astab.as_spec, mountPoint, OSI_MOUNT_TYPE_AGFS,
				 0, NULL, &astab, sizeof astab);
	    }
	}
    }

    return code;
}	/* ftu_DetachAggr() */



/*
 * ftu_ListAggrs()
 *	Call AG_ENUMERATE at least twice, once to get the current size of
 *	the list, and once to retrieve the list.
 */
EXPORT long
ftu_ListAggrs(aggrs, numAggrsP)
    OUT struct aggrList**	aggrs;
    OUT unsigned*		numAggrsP;
{
    struct aggrList*	aggrBuf = 0;
    unsigned		bufSize, bufSizeCopy;
    long		code;

    /*
     * Find out how much space is needed, allocate the space, and then
     * retrieve the aggregate list.  The loop is here because there is a
     * race between the first and second calls to AG_ENUMERATE().  Looping
     * will be good enough in all but the pathological case.
     */
    do {
	/*
	 * This is ugly.  AG_ENUMERATE() returns the amount of space
	 * necessary to hold the aggregate list in the second argument if the
	 * first parameter is less than enough, but if there are no
	 * aggregates in the registry, the required space (ie. 0) is returned
	 * in the third argument.  Exactly one of these conditions will exist,
	 * so we just pass the address of `bufSize' in both positions.
	 */
	if ((code = AG_ENUMERATE(0, &bufSize, &bufSize)) && code != E2BIG)
	    return code;

	if (bufSize == 0) {	/* No aggregates registered */
	    *numAggrsP = 0;
	    *aggrs = (struct aggrList *) 0;
	    return 0;
	}

	bufSizeCopy = bufSize;

	if ((aggrBuf = (struct aggrList*)osi_Alloc(bufSize)) == NULL)
	    return ENOMEM;

	if (code = AG_ENUMERATE(bufSize, aggrBuf, &bufSize)) {
	    osi_Free(aggrBuf, bufSizeCopy);
	    if (code == E2BIG)
		aggrBuf = 0;		/* List grew--try again */
	    else
		return code;
	}
    } while (aggrBuf == 0);

    *aggrs = aggrBuf;
    *numAggrsP = bufSize / sizeof aggrBuf[0];

    return 0;
}	/* ftu_ListAggrs() */


/*
 * ftu_LookupAggrByDevice()
 *	Get a list of all attached aggregates and look for the specified
 *	device.  We don't compare by name; rather, we call stat on each
 *	device and compare `st_rdev' fields.  This ensures that we get
 *	an accurate answer, regardless of any aliasing due to links or
 *	absolute vs. relative names.
 */
EXPORT long
ftu_LookupAggrByDevice(devName, aggrIdP)
    IN char*		devName;
    OUT ftu_aggrId_t*	aggrIdP;
{
    struct aggrList*		aggrs = 0;
    register struct aggrList*	aggrP;
    long			code;
    register dev_t		device;
    register unsigned		i, ret;
    unsigned			numAggrs;
    struct stat			statBuf;
    char			rawDevName[MAXPATHLEN+1];

    if (stat(devName, &statBuf) == -1)
	return errno;

    if (!(S_ISCHR(statBuf.st_mode) || S_ISBLK(statBuf.st_mode)))
	return FTU_E_NOT_A_DEVICE;

    device = statBuf.st_rdev;

    if (code = ftu_ListAggrs(&aggrs, &numAggrs))
	return code;

    code = FTU_E_NOT_ATTACHED;

    for (i = 0, aggrP = aggrs; i < numAggrs; i++, aggrP++) {

	if (ret = ftu_GetRawDeviceName(aggrP->dvname, rawDevName)) {
	    code = ret;
	    break;
	}
	
	if (stat(rawDevName, &statBuf) == -1) {
	    code = errno;
	    break;
	}

	if (device == statBuf.st_rdev) {
	    if (aggrIdP)
		*aggrIdP = aggrP->Id;
	    code = 0;
	    break;
	}
    }

    if (aggrs)
	/* Deallocate only if list is non-null */
	osi_Free(aggrs, numAggrs * sizeof(struct aggrList));

    return code;
}	/* ftu_LookupAggrByDevice() */


/*
 * ftu_LookupAggrByName()
 *	Get a list of all attached aggregates and look for the specified
 *	name.
 */
EXPORT long
ftu_LookupAggrByName(aggrName, aggrIdP)
    IN char*		aggrName;
    OUT ftu_aggrId_t*	aggrIdP;
{
    struct aggrList*		aggrs = 0;
    register struct aggrList*	aggrP;
    long			code;
    register unsigned		i;
    unsigned			numAggrs;

    if (code = ftu_ListAggrs(&aggrs, &numAggrs))
	return code;

    code = FTU_E_NOT_ATTACHED;

    for (i = 0, aggrP = aggrs; i < numAggrs; i++, aggrP++)
	if (strncmp(aggrName, aggrP->name, sizeof aggrP->name) == 0) {
	    if (aggrIdP)
		*aggrIdP = aggrP->Id;
	    code = 0;
	    break;
	}

    if (aggrs)
	/* Deallocate only if list is non-null */
	osi_Free(aggrs, numAggrs * sizeof(struct aggrList));

    return code;
}	/* ftu_LookupAggrByName() */

