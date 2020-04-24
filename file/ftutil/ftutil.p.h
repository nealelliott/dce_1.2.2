/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ftutil.p.h,v $
 * Revision 1.1.66.1  1996/10/02  17:48:57  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:40  damon]
 *
 * $EndLog$
 */
/*
 * (C) Copyright 1996, 1990 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 *
 * $Header: /u0/rcs_trees/dce/rcs/file/ftutil/ftutil.p.h,v 1.1.66.1 1996/10/02 17:48:57 damon Exp $
 *
 */

#ifndef	_TRANSARC_DFS_FTUTIL_H
#define	_TRANSARC_DFS_FTUTIL_H

/*
 * Interface specification for the ftutil layer.  The namespace reserved by
 * this layer includes all symbols with the following prefixes:
 *	ftu_, _ftu_, FTU_, _FTU_
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>		/* Transarc coding standards */
#include <dcedfs/sysincludes.h>		/* System defined types, etc. */
#include <dcedfs/aggr.h>		/* AGGR_ATTACH_NOEXPORT */
#include <dcedfs/astab.h>		/* ASTAB constants */
#include <dcedfs/ag_init.h>		/* struct aggrInfo, struct aggrList */
#include <dcedfs/volume.h>		/* Volume-related symbols */
#include <dcedfs/pipe.h>		/* afsPipe_t */
#include <dcedfs/icl.h>			/* ICL types */


/******************************************************************************
 * Constants
 *****************************************************************************/
#define FTU_MAX_AGGR_NAME	(ASTABSIZE_DIR - 1)
#define FTU_MAX_DEV_NAME	(ASTABSIZE_SPEC - 1)
#define FTU_MAX_FSET_NAME	(VOLNAMESIZE - 1)

/*
 * Pass these as the `flags' argument to ftu_AttachAggr() and ftu_DetachAggr().
 * For the time being, these flags are mutually exclusive
 */
#define FTU_ATTACH_FLAGS_NOEXPORT	0x1
#define FTU_ATTACH_FLAGS_MOUNTAGGR	0x2
#define FTU_ATTACH_FLAGS_FORCEDETACH	0x4

/*
 * Pass these as the states argument to ftu_AggrCreateFset() and
 * ftu_FsetClone().
 */
#define FTU_FSET_STATES_RW	(VOL_TYPE_RW|VOL_RW)
#define FTU_FSET_STATES_RO	(VOL_TYPE_RO|VOL_READONLY)
#define FTU_FSET_STATES_BK	(VOL_TYPE_BK|VOL_READONLY)
#define FTU_FSET_STATES_TEMP	(VOL_TYPE_TEMP|VOL_READONLY)

/*
 * Pass these as the `fsetFlags' argument to ftu_AggrOpenFset().
 */
#define FTU_SYS_GET_STATUS	(VOL_SYS_GETSTATUS)
#define FTU_SYS_SET_STATUS	(VOL_SYS_SETSTATUS)
#define FTU_SYS_PUSH_STATUS	(VOL_SYS_PUSHSTATUS)
#define FTU_SYS_SYNC		(VOL_SYS_SYNC)
#define FTU_SYS_CLONE		(FTU_SYS_GET_STATUS|FTU_SYS_PUSH_STATUS\
				 |VOL_SYS_CLONE|FTU_SYS_SYNC\
				 |FTU_SYS_BULK_SET_STATUS|FTU_SYS_DESTROY)
#define FTU_SYS_RECLONE		(FTU_SYS_GET_STATUS|VOL_SYS_RECLONE\
				 |FTU_SYS_SYNC)
#define FTU_SYS_RECLONE_INTO	(FTU_SYS_GET_STATUS|VOL_SYS_RECLONE\
				 |FTU_SYS_SYNC|FTU_SYS_CLONE)
#define FTU_SYS_DESTROY		(FTU_SYS_GET_STATUS|FTU_SYS_SET_STATUS\
				 |FTU_SYS_PUSH_STATUS|VOL_SYS_UNCLONE\
				 |FTU_SYS_SYNC|VOL_SYS_DEPLETE|VOL_SYS_DESTROY\
				 |FTU_SYS_BULK_SET_STATUS)
#define FTU_SYS_DUMP		(FTU_SYS_GET_STATUS|VOL_SYS_SCAN\
				 |VOL_SYS_GETATTR|VOL_SYS_GETACL|VOL_SYS_READ\
				 |VOL_SYS_READHOLE|VOL_SYS_READDIR)
#define FTU_SYS_RESTORE		(FTU_SYS_GET_STATUS|VOL_SYS_SETSTATUS\
				 |VOL_SYS_CREATE|VOL_SYS_SETACL\
				 |VOL_SYS_COPYACL|VOL_SYS_DELETE\
				 |VOL_SYS_TRUNCATE|VOL_SYS_WRITE\
				 |VOL_SYS_SETATTR|VOL_SYS_GETATTR|VOL_SYS_SCAN\
				 |VOL_SYS_SYNC|VOL_SYS_APPENDDIR)
#define FTU_SYS_BULK_SET_STATUS	(VOL_SYS_SETSTATUS|VOL_SYS_SWAPVOLIDS)
#define FTU_SYS_SWAP_IDS	(FTU_SYS_GET_STATUS|FTU_SYS_BULK_SET_STATUS)


/* Pass these as the sync op argument to ftu_AggrSync() */
#define FTU_AGGR_SYNC_COMMIT_META	AGGR_SYNC_COMMITMETA
#define FTU_AGGR_SYNC_COMMIT_ALL	AGGR_SYNC_COMMITALL

/* Pass these as the sync op argument to ftu_FsetSync() */
#define FTU_FSET_SYNC_COMMIT_STATUS	VOL_SYNC_COMMITSTATUS
#define FTU_FSET_SYNC_COMMIT_META	VOL_SYNC_COMMITMETA
#define FTU_FSET_SYNC_COMMIT_ALL	VOL_SYNC_COMMITALL

/* Use these as the type field in ftu_dumpCriteria structures */
#define FTU_DUMP_TYPE_FULL		0
#define FTU_DUMP_TYPE_DATE_WITH_DIRS	1
#define FTU_DUMP_TYPE_VERS_WITH_DIRS	2
#define FTU_DUMP_TYPE_DATE		3
#define FTU_DUMP_TYPE_VERS		4

/* Pass these as the flags argument to the fileset restore routines */
#define FTU_RESTORE_FORCE_PRESERVE	0x1
#define FTU_RESTORE_DISJOINT_OK		0x2

/* This is declared as a variable because it's a afs_hyper_t. */
IMPORT afs_hyper_t ftu_unlimitedQuota;

/******************************************************************************
 * Types
 *****************************************************************************/
typedef u_long		ftu_aggrId_t;
typedef int		ftu_devLock_t;
typedef afs_hyper_t	ftu_fsetId_t;
typedef int		ftu_fsetDesc_t;
typedef void		(*ftu_kaProc_t)(void*);

typedef struct ftu_dumpCriteria {
    int		dc_type;
    union {
	afsTimeval	dcu_date;
	afs_hyper_t	dcu_version;
    }		dc_from;

    afs_hyper_t	dc_spare1;
    afs_hyper_t	dc_spare2;
    afs_hyper_t	dc_spare3;
}	ftu_dumpCriteria_t;
#define dc_date		dc_from.dcu_date
#define dc_version	dc_from.dcu_version


/*
 * The following routines return 0 on success and one of the documented
 * return codes under exceptional conditions.  The actual preprocessor
 * symbols for the error codes begin with "FTU_E_", but this prefix has been
 * left out in the documentation below.
 *
 * The documented error codes are those that are returned directly by the
 * ftutil layer; others may be propagated from lower level routines.
 */

/******************************************************************************
 * Logging routines
 *****************************************************************************/

/*
 * ftu_CreateIclSet()
 *	Adds an event set named "ftutil" to the given ICL log.  This will
 *	cause this library's ICL trace messages to be written to the log.
 *
 * PARAMETERS:
 *	iclLogP
 *		The ICL log to which the "ftutil" ICL event set will be
 *		added.  If the log is set to 0, ICL logging will be disabled.
 *
 * RETURN CODES:
 * 	None returned directly by the ftutil layer, but codes from lower
 *	levels may be propagated.
 */
IMPORT long ftu_CreateIclSet _TAKES((
    IN struct icl_log*	iclLogP
));

/*
 * ftu_SetLogStream()
 *	Sets the stream to which the ftutil routines will log informational
 *	messages.
 *
 * PARAMETERS:
 *	stream
 *		The stream to which log messages will be written.  If the
 *		log stream is set to 0, logging will be disabled.
 *
 * RETURN CODES:
 * 	None.
 */
IMPORT void ftu_SetLogStream _TAKES((
    IN FILE*	stream
));

/*
 * ftu_GetLogStream()
 *	Retrieves the current log stream.
 *
 * PARAMETERS:
 *	*streamP
 *		Set to the value of the current log stream.  0 is returned
 *		when logging is disabled.
 *
 * RETURN CODES:
 *	None.
 */
IMPORT void ftu_GetLogStream _TAKES((
    OUT FILE** streamP
));

/*
 * ftu_LogPrintf()
 *
 * PARAMETERS:
 *	format
 *		A printf(3)-style format string that describes the
 *		subsequent arguments, if any.
 *	...
 *		A variable number of arguments to be printed according
 *		to `format'.
 *
 * RETURN CODES:
 * 	None.
 */
IMPORT void ftu_LogPrintf _TAKES((
    IN char* format,
    ...
));


/******************************************************************************
 * Utility routines
 *****************************************************************************/

/*
 * ftu_GetDevice()
 *	Checks that the given pathname specifies a device.
 *	If the pathname is not absolute (doesn't start with "/" or "./")
 *	the relative path is checked first, and if it doesn't yield a
 *	device, the check is retried with "/dev/" prepended to the name.
 *	If a device is found, both the path name and the result of
 *	of stat(2) is returned.
 *
 * PARAMETERS:
 *	devName
 *		The name of a device.
 *	fullDevName
 *		If a device is found, the fully-qualified pathname is copied
 *		to this location, unless 0 is passed in, indicating that the
 *		full pathname should not be returned.  If this parameter is
 *		non-zero, it should point to an array of at least
 *		`MAXPATHLEN'+1 characters.
 *	*statBufP
 *		If a device is found, its stat(2) structure is returned here,
 *		unless 0 is passed in, indicating that the stat information
 *		should not be returned.
 *
 * RETURN CODES:
 *	NOT_A_DEVICE
 *		`devName' does not name a device.
 */
IMPORT long ftu_GetDevice _TAKES((
    IN char*		devName,
    OUT char*		fullDevName,
    OUT struct stat*	statBufP
));

/*
 * ftu_GetFile()
 *	Has the same semantics as ftu_GetDevice, except that it doesn't
 *	require that the pathname may specify any type of file system object.
 */
IMPORT long ftu_GetFile _TAKES((
    IN char*		fileName,
    OUT char*		fullName,
    OUT struct stat*	statBufP
));

/*
 * ftu_GetRawDeviceName()
 *	Returns the name of the raw device that is associated with the
 *	specified block device.
 *
 * PARAMETERS:
 *	blockDevName
 *		The name of a block device.
 *	rawDevName
 *		The name of the character device associated with
 *		`blockDevName' is copied to this location.  This parameter
 *		should point to an array of at least `MAXPATHLEN'+1
 *		characters.
 *
 * RETURN CODES:
 *	CANT_FIND_CHARACTER_DEVICE
 *		The associated character device can't be found.
 *	NOT_A_BLOCK_DEVICE
 *		`devName' does not name a block device.
 */
IMPORT long ftu_GetRawDeviceName _TAKES((
    IN char*	blockDevName,
    OUT char*	rawDevName
));

/*
 * ftu_ParseFsetId()
 *	Parses a string representing a fileset ID and returns the actual
 *	fileset ID.
 *
 * PARAMETERS:
 *	fsetIdString
 *		Null-terminated string of the form:
 *			[<high>,,]<low>
 *	*fsetIdP
 *		Set to the result of parsing `fsetIdString'
 *
 * RETURN CODES:
 *	MALFORMED_FSET_ID
 *		`fsetIdString' is not a well-formed fileset ID.
 */
IMPORT long ftu_ParseFsetId _TAKES((
    IN char*		fsetIdString,
    OUT ftu_fsetId_t*	fsetIdP
));

/******************************************************************************
 * Aggregate locking routines
 *****************************************************************************/

/*
 * ftu_LockAggrDevice()
 *	Locks the aggregate's device, preventing future lock attempts from
 *	succeeding.  After the lock is obtained, this routine checks to
 *	ensure that the aggregate has not been attached, allowing
 *	applications that operate on the raw device to continue without
 *	fear of damaging a "live" aggregate.
 *
 * PARAMETERS:
 *	devName
 *		The name of a character device.
 *	*devLockP
 *		An opaque value representing the newly-obtained lock.  Pass
 *		this value ftu_UnlockAggrDevice().
 *
 * RETURN CODES:
 *	ATTACHED
 *		The aggregate is attached.
 *	DEVICE_LOCKED
 *		The device has already been locked.
 *	NOT_A_CHARACTER_DEVICE
 *		`devName' does not name a character device.
 */
IMPORT long ftu_LockAggrDevice _TAKES((
    IN char*		devName,
    OUT ftu_devLock_t*	devLockP
));

/*
 * ftu_UnlockAggrDevice()
 *	Unlocks the aggregate's device, allowing future lock attempts to
 *	succeed.  This routine should be used to release a lock obtained by
 *	calling ftu_LockAggrDevice().
 *
 * PARAMETERS:
 *	devLock
 *		The lock variable returned by a previoius call to 
 *		ftu_LockAggrDevice().
 *
 * RETURN CODES:
 * 	None returned directly by the ftutil layer, but codes from lower
 *	levels may be propagated.
 */
IMPORT long ftu_UnlockAggrDevice _TAKES((
    IN ftu_devLock_t	devLock
));

/******************************************************************************
 * Aggregate registration routines
 *****************************************************************************/

/*
 * ftu_AttachAggr()
 *	Attaches an aggregate and places it in the aggregate registry,
 *	creating a mapping from `devName' or `aggrName' to `aggrId'.  An
 *	aggregate must be attached before any routine taking an aggregate
 *	ID (`aggrId' below) can operate on it.
 *	
 * PARAMETERS:
 *	devName
 *		The name of the aggregate's block device.  Must be no longer
 *		than `FTU_MAX_DEVICE_NAME'.
 * 	aggrType
 *		The type of the aggregate.  Must be one of the `ASTABTYPE_*'
 *		values defined in <dcedfs/astab.h>.
 *	aggrName
 *		An arbitrary, but unique, name for the aggregate.  Must be no
 *		longer than `FTU_MAX_AGGR_NAME'.
 *	aggrId
 *		An arbitrary, but unique, ID for the aggregate.
 *	flags
 *		Bitmask of values from FTU_ATTACH_FLAGS_*, as defined above.
 *	fstabInfo
 *		Opaque data that is passed to the filesystem-specific attach
 *		operatioin (This currently only applies to UFS.)
 *
 * RETURN CODES:
 *	ALREADY_ATTACHED
 *		The aggregate (device) is already attached.
 *	CANT_FIND_CHARACTER_DEVICE
 *		The character device associated with `devName' can't be found.
 *	DEVICE_LOCKED
 *		The device has been locked by another process.
 *	ID_CLASH
 *		The same ID is already in use.
 *	NAME_CLASH
 *		The same name is already in use.
 *	NAME_TOO_LONG
 *		The length of `devName' is longer than `FTU_MAX_DEV_NAME', or
 *		the length of `aggrName' is longer than `FTU_MAX_AGGR_NAME'.
 *	NOT_A_BLOCK_DEVICE
 *		`devName' does not name a block device.
 *	TRY_RECOVERY
 *		The aggregate is an LFS aggregate and recovery should be
 *		attempted run before attaching.
 */
IMPORT long ftu_AttachAggr _TAKES((
    IN char*		devName,
    IN char*		aggrType,
    IN char*		aggrName,
    IN ftu_aggrId_t	aggrId,
    IN u_long		flags,
    IN caddr_t		fstabInfo
));

/*
 * ftu_AttachAggrWithAstab()
 *	Attaches an aggregate and places it in the aggregate registry.  An
 *	aggregate must be attached before any routine taking an aggregate
 *	ID (`aggrId' below) can operate on it.  Use this routine in favor
 *	of the one above when you already have an astab structure available.
 *
 * PARAMETERS:
 *	*astabP
 *		A populated astab structure (as defined in <dcedfs/astab.h>)
 *		that describes the device to be attaced.
 *	flags
 *		Values from the FTU_ATTACH_FLAGS_* set above.
 *	fstabInfo
 *		Data passed ot the FS-specific attach op. (This seems to only
 *		be used by the UFS attach op.)
 *
 * RETURN CODES:
 *	ALREADY_ATTACHED
 *		The aggregate (device) is already attached.
 *	CANT_FIND_CHARACTER_DEVICE
 *		The character device associated with `astabP->as_spec' can't
 *		be found.
 *	DEVICE_LOCKED
 *		The device has been locked by another process.
 *	ID_CLASH
 *		The same ID is already in use.
 *	NAME_CLASH
 *		The same name is already in use.
 *	NOT_A_BLOCK_DEVICE
 *		`astabP->as_spec' does not name a block device.
 *	TRY_RECOVERY
 *		The aggregate is an LFS aggregate and recovery should be
 *		attempted run before attaching.
 */
IMPORT long ftu_AttachAggrWithAstab _TAKES((
    IN struct astab*	astabP,
    IN u_long		flags,
    IN caddr_t		fstabInfo
));

/*
 * ftu_DettachAggr()
 *	Removes an aggregate from the aggregate registry.  No further
 *	operations which take an aggregate ID may be performed on the
 *	aggregate.
 *
 * PARAMETERS:
 *	aggrId
 *		The ID of the aggregate to be detached.
 *	flags
 *		Pass the same value here as was passed in as the `flags'
 *		argument to the corresponding ftu_AttachAggr* call.
 *
 * RETURN CODES:
 * 	None returned directly by the ftutil layer, but codes from lower
 *	levels may be propagated.
 */
IMPORT long ftu_DetachAggr _TAKES((
    IN ftu_aggrId_t	aggrId,
    IN u_long		flags
));

/*
 * ftu_ListAggrs()
 *	Returns a list of all attached aggregates.
 *
 * PARAMETERS:
 *	*aggrs
 *		Set to point to an array of `aggrList' structures.  The
 *		memory for the array is allocated by malloc(3).  The caller
 *		is responsible for deallocating it.
 *	*numAggrsP
 *		Set to the number of entries returned in `aggrs'.
 *
 * RETURN CODES:
 * 	None returned directly by the ftutil layer, but codes from lower
 *	levels may be propagated.
 */
IMPORT long ftu_ListAggrs _TAKES((
    OUT struct aggrList**	aggrs,
    OUT unsigned*		numAggrsP
));

/*
 * ftu_LookupAggrByDevice()
 *	Returns the ID of the (attached) aggregate named by a device name.
 *	`devName' can name either a block or character device.
 *
 * PARAMETERS:
 *	devName
 *		The name of a block device.
 *	*aggrIdP
 *		Set to the ID of the aggregate named by `devName', if any.
 *		If `aggrId' is 0, it is not set by this routine, but the
 *		return code still indicates whether or not the aggregate
 *		was found.
 *
 * RETURN CODES:
 *	NOT_A_DEVICE
 *		`devName' does not name a device.
 *	NOT_ATTACHED
 *		There is no attached aggregate corresponding to `devName'.
 */
IMPORT long ftu_LookupAggrByDevice _TAKES((
    IN char*		devName,
    OUT ftu_aggrId_t*	aggrIdP
));

/*
 * ftu_LookupAggrByName()
 *	Returns the ID of the (attached) aggregate named by `aggrName'.
 *
 * PARAMETERS:
 *	aggrName
 *		The aggregate name used when attaching the aggregate.
 *	*aggrIdP
 *		Set to the ID of the aggregate named by `aggrName', if any.
 *		If `aggrId' is 0, it is not set by this routine, but the
 *		return code still indicates whether or not the aggregate
 *		was found.
 *
 * RETURN CODES:
 *	NOT_ATTACHED
 *		There is no attached aggregate corresponding to `aggrName'.
 */
IMPORT long ftu_LookupAggrByName _TAKES((
    IN char*		aggrName,
    OUT ftu_aggrId_t*	aggrIdP
));

/******************************************************************************
 * Aggregate routines
 *****************************************************************************/

/*
 * ftu_AggrGetInfo()
 *	Returns information about an aggregate.
 *
 * PARAMETERS:
 *	aggrId
 *		The aggregate ID of an attached aggregate.
 *	*aggrInfoP
 *		A preallocated `aggrInfo' structure (as defined in
 *		(<dcedfs/ag_init.h>) whose fields will be set to reflect
 *		the specified aggregate's status.  If `aggrInfoP' is 0,
 *		it is not set by this routine, but the return code still
 *		indicates whether or not the aggregate was found.
 *
 * RETURN CODES:
 *	NOT_ATTACHED
 *		There is no attached aggregate corresponding to `aggrId'.
 */
IMPORT long ftu_AggrGetInfo _TAKES((
    IN ftu_aggrId_t		aggrId,
    OUT struct aggrInfo*	aggrInfoP
));

/*
 * ftu_AggrSync()
 *	Commits an aggregate's modified data to secondary storage.
 *
 * PARAMETERS:
 *	aggrId
 *		The aggregate ID of an attached aggregate.
 *	syncOp
 *		One of the FTU_AGGR_SYNC_* values, as defined above.
 *
 * RETURN CODES:
 * 	None returned directly by the ftutil layer, but codes from lower
 *	levels may be propagated.
 */
IMPORT long ftu_AggrSync _TAKES((
   IN ftu_aggrId_t	aggrId,
   IN int		syncOp
));

/*
 * ftu_AggrCreateFset()
 *	Create a fileset within an aggregate.
 *
 * PARAMETERS:
 * 	aggrId
 *		The aggregate ID of an attached aggregate.
 *	*fsetIdP
 *		An arbitrary, but unique, ID for the new fileset.  Unlike
 *		aggregate IDs, fileset IDs are persistent, and identify a
 *		fileset throughout its lifetime.
 *	fsetName
 *		An arbitrary, but unique, name for the new fileset.  Unlike
 *		aggregate names, fileset names are persistent.
 *	*parentIdP
 *		The ID of the new fileset's parent.
 *	fsetStates
 *		The new fileset's states.  The legal values are the same
 *		as the legal values for the `vol_st.states' field within
 *		the `vol_status' structure.  For convenience, the
 *		FTU_FSET_STATES_* values may be used here to specify the
 *		type of the new fileset.
 *	fsetMaxQuota
 *		The maximum space, in kilobytes, that can be allocated within
 *		the new fileset.  This number is not bounded by the size of
 *		the aggregate, and can be changed at any time throughout the
 *		fileset's lifetime.
 *	fsetFlags
 *		Bitmask made up of values from <dcedfs/aggr.h> (ie.
 *		`AGGR_CREATE_ROOT') that control specifics of the fileset
 *		creation.
 *
 * RETURN CODES:
 *	NOT_ATTACHED
 *		There is no attached aggregate corresponding to `aggrId'.
 *	NAME_TOO_LONG
 *		The length of `fsetName' is longer that `FTU_MAX_FSET_NAME'.
 *	NAME_CLASH
 *		The same name is already in use within the specified aggregate.
 *	ID_CLASH
 *		The same ID is already in use within the specified aggregate.
 */
IMPORT long ftu_AggrCreateFset _TAKES((
    IN ftu_aggrId_t	aggrId,
    IN ftu_fsetId_t*	fsetIdP,
    IN char*		fsetName,
    IN ftu_fsetId_t*	parentIdP,
    IN u_long		fsetStates,
    IN long		fsetMaxQuota,
    IN u_long		fsetFlags
));

/*
 * ftu_AggrCreateFsetWithStatus()
 *	Create a fileset within an aggregate.  Use this routine in favor of 
 *	the one above when you already have a vol_status structure available
 *	or if you wish to initialize status fields other than the ones
 *	available through the more limited interface.
 *
 * PARAMETERS:
 *	aggrId
 *		The aggregate ID of an attached aggregate.
 *	*fsetIdP
 *		An arbitrary, but unique, ID for the new fileset.  Unlike
 *		aggregate IDs, fileset IDs are persistent, and identify a
 *		fileset throughout its lifetime.
 *	*fsetStatusP
 *		A populated vol_status structure that describes the fileset
 *		to be created.
 *	fsetFlags
 *		Bitmask made up of values from "dcedfs/aggr.h" (ie.
 *		`AGGR_CREATE_ROOT') that control specifics of the fileset
 *		creation.
 *
 * RETURN CODES:
 *	NOT_ATTACHED
 *		There is no attached aggregate corresponding to `aggrId'.
 *	NAME_CLASH
 *		The same name is already in use within the specified aggregate.
 *	ID_CLASH
 *		The same ID is already in use within the specified aggregate.
 */
IMPORT long ftu_AggrCreateFsetWithStatus _TAKES((
    IN ftu_aggrId_t		aggrId,
    IN ftu_fsetId_t*		fsetIdP,
    IN struct vol_status*	fsetStatusP,
    IN u_long			fsetFlags
));

/*
 * ftu_AggrOpenFset()
 *	Opens a fileset in an attached aggregate.  The act of opening a
 *	fileset grants the opener a lock on the fileset in the sense that
 *	subsequent, conflicting opens will not be allowed; thus, bracketing
 *	a set of fileset operations with an open-close pair provides for
 *	atomic updates.
 *
 * PAREMETERS:
 *	aggrId
 *		The aggregate ID of an attached aggregate.
 *	*fsetIdP
 *		The fileset ID of an existing fileset within the specified
 *		aggregate.
 *	fsetFlags
 *		Bitmask specifying what operations may be performed.  The
 *		legal bits are the same as the legal bits for the `accStat'
 *		field of the `vol_AgOpen' structure.  For convenience, the
 *		FTU_SYS_* values may be used here.
 *	fsetErrorIn
 *		This value will be returned to subsequent, conflicting openers
 *		if this open is successful.
 *	*fsetDescP
 *		On success, this parameter is set to a descriptor that can be
 *		used to identify the open fileset.
 *
 * RETURN CODES:
 *	NOT_ATTACHED
 *		There is no attached aggregate corresponding to `aggrId'.
 *	NO_SUCH_FSET
 *		There is no fileset corresponding to `fsetId'.
 */
IMPORT long ftu_AggrOpenFset _TAKES((
    IN u_long		aggrId,
    IN ftu_fsetId_t*	fsetIdP,
    IN u_long		fsetFlags,
    IN u_long		fsetErrorIn,
    OUT ftu_fsetDesc_t*	fsetDescP
));

/*
 * ftu_AggrCloseFset()
 *	Closes an already open fileset.  After this operation `fsetDesc' may
 *	no longer be used to perform other fileset operations.  Use this
 *	routine only if the preceding operations were successful.  If the
 *	fileset needs to be closed as part of handling a failure, call
 *	ftu_AbortFsetOpen() instead.
 *
 * PARAMETERS:
 *	fsetDesc
 *		The descriptor for an open fileset.
 *
 * RETURN CODES:
 * 	None returned directly by the ftutil layer, but codes from lower
 *	levels may be propagated.
 */
IMPORT long ftu_AggrCloseFset _TAKES((
    IN ftu_fsetDesc_t	fsetDesc
));

/*
 * ftu_AggrAbortFsetOpen()
 *	Use this routine in place of ftu_AggrCloseFset() when the intended
 *	sequence of operations is being aborted because of a failure.  It
 *	lets the underlying system know that the fileset was not updated
 *	completely and may be in an inconsistent state.
 *
 * PARAMETERS:
 *	fsetDesc
 *		The descriptor for an open fileset.
 *
 * RETURN CODES:
 * 	None returned directly by the ftutil layer, but codes from lower
 *	levels may be propagated.
 */
IMPORT void ftu_AggrAbortFsetOpen _TAKES((
    IN ftu_fsetDesc_t	fsetDesc
));

/*
 * ftu_AggrEnumerateFsets()
 *	Enumerate the filesets within an attached aggregate.  Unlike
 *	ftu_ListAggrs(), this routine is called in a loop until the
 *	number of filesets returned is less than the number asked for.
 *	There are potentially a large number of filesets within an
 *	aggregate, so allocating a chunk of memory to hold them all may
 *	not be possible.
 *
 * PARAMETERS:
 *	aggrId
 *		The aggregate ID of an attached aggregate.
 *	*index
 *		The index at which the enumeration should start.  This value
 *		should be set to 0 initially.  It is then modified during
 *		the call and should be passed back in on subsequent calls to
 *		continue the enumeration.
 *	fsetBuf
 *		A pointer to a preallocated array large enough to hold at
 *		least `numFsets' vol_status structures.
 *	numFsets
 *		The maximum number of filesets to be returned in `fsetBuf'.
 *	*numFsetsReturnedP
 *		Set to the number of filesets that were actually returned.
 *		When the number of filesets returned is less than the number
 *		of filesets asked for (in `numFsets') the enumeration is
 *		complete.
 *
 * RETURN CODES:
 *	NOT_ATTACHED
 *		There is no attached aggregate corresponding to `aggrId'.
 */
IMPORT long ftu_AggrEnumerateFsets _TAKES((
    IN ftu_aggrId_t		aggrId,
    INOUT unsigned*		index,
    IN struct vol_status*	fsetBuf,
    IN unsigned			numFsets,
    OUT unsigned*		numFsetsReturnedP
));

/*
 * ftu_AggrLookupFset()
 *	Returns the ID of a named fileset.
 *
 * PARAMETERS:
 *	aggrId
 *		The aggregate ID of an attached aggregate.
 *	fsetName
 *		The name of the fileset being searched for.
 *	*fsetIdP
 *		Set to the ID of the corresponding fileset, if any.
 *		If `fsetId' is 0, it is not set by this routine, but the
 *		return code still indicates whether or not the fileset
 *		was found.
 *
 * RETURN CODES:
 *	NOT_ATTACHED
 *		There is no attached aggregate corresponding to `aggrId'.
 *	NO_SUCH_FSET
 *		There is no fileset corresponding to `fsetName'.
 */
IMPORT long ftu_AggrLookupFset _TAKES((
    IN ftu_aggrId_t	aggrId,
    IN char*		fsetName,
    OUT ftu_fsetId_t*	fsetIdP
));

/*
 * The remaining aggregate routines are wrappers that surround the related
 * fileset routines with an open and close.  Note that they are
 * called with fileset IDs instead of fileset descriptors.  These routines
 * should be used whenever you want to perform just one operation on a
 * fileset.
 */

/*
 * ftu_AggrGetFsetStatus()
 *	Returns status information for the specified fileset.
 *
 * PARAMETERS:
 *	aggrId
 *		The aggregate ID of the attached aggregate containing the
 *		fileset desribed by `*fsetIdP'.
 *	*fsetIdP
 *		The descriptor for an open fileset.
 *	*fsetStatusP
 *		A preallocated `vol_status' structure whose fields will be
 *		set to reflect the specified fileset's status.
 *
 * RETURN CODES:
 *	NOT_ATTACHED
 *		There is no attached aggregate corresponding to `aggrId'.
 *	NO_SUCH_FSET
 *		There is no fileset corresponding to `*fsetIdP'.
 */
IMPORT long ftu_AggrGetFsetStatus _TAKES((
    IN ftu_aggrId_t		aggrId,
    IN ftu_fsetId_t*		fsetIdP,
    OUT struct vol_status*	fsetStatusP
));

/*
 * ftu_AggrSetFsetStatus()
 *	Sets the specified fileset's status.
 *
 * PARAMETERS:
 *	aggrId
 *		The aggregate ID of the attached aggregate containing the
 *		fileset identified by `*fsetIdP'.
 *	*fsetIdP
 *		The descriptor for an open fileset.
 *	fsetMask
 *		Mask made up of VOL_STAT_* values (as defined in 
 *		<dcedfs/volume.h>) indicating which status fields are
 *		being set.
 *	*fsetStatusP
 *		A `vol_status' structure.
 *
 * RETURN CODES:
 *	NOT_ATTACHED
 *		There is no attached aggregate corresponding to `aggrId'.
 *	NO_SUCH_FSET
 *		There is no fileset corresponding to `*fsetIdP'.
 */
IMPORT long ftu_AggrSetFsetStatus _TAKES((
    IN ftu_aggrId_t		aggrId,
    IN ftu_fsetId_t*		fsetId,
    IN u_long			fsetMask,
    IN struct vol_status*	fsetStatusP
));

/*
 * ftu_AggrCloneFset()
 * 	Makes a clone of a fileset.  When this operation completes the
 *	destination fileset will be the read-only copy and the original ID will
 *	still identify the read-write copy (ie. the IDs are swapped).
 *
 * PARAMETERS:
 *	aggrId
 *		The aggregate ID of the attached aggregate containing the
 *		fileset identified by `*srcIdP'.
 *	*srcIdP
 *		The ID for the source fileset.
 *	*destIdP
 *		An arbitrary, but unique, ID for the new fileset.
 *	destName
 *		The name to use for the clone.  The string can be
 *		no longer than `FTU_MAX_FSET_NAME'.
 *	destStates
 *		The clone's states.  The legal values are the same
 *		as the legal values for the `vol_st.states' field within
 *		the `vol_status' structure.  For convenience, the
 *		FTU_FSET_STATES_* values may be used here to specify the
 *		type of the clone.
 * 
 * RETURN CODES:
 *	NOT_ATTACHED
 *		There is no attached aggregate corresponding to `aggrId'.
 *	NO_SUCH_FSET
 *		There is no fileset corresponding to `*srcIdP'.
 *	NAME_CLASH
 *		The same name is already in use within the specified aggregate.
 *	ID_CLASH
 *		The same ID is already in use within the specified aggregate.
 */
IMPORT long ftu_AggrCloneFset _TAKES((
    IN ftu_aggrId_t	aggrId,
    IN ftu_fsetId_t*	srcIdP,
    IN ftu_fsetId_t*	destIdP,
    IN char*		destName,
    IN u_long		destStates
));

#if 0	/* XXX Not implemented */
/*
 * ftu_AggrCloneFsetWithoutSwap()
 *	Same as the above routine, except that the original fileset becomes
 *	the read-only copy and the new fileset is the read-write copy.
 *
 * PARAMETERS:
 *	aggrId
 *		The aggregate ID of the attached aggregate containing the
 *		fileset identified by `*srcIdP'.
 *	*srcIdP
 *		The ID for the source fileset.
 *	*destIdP
 *		An arbitrary, but unique, ID for the new fileset.
 *	destName
 *		The name to use for the clone.  The string can be
 *		no longer than `FTU_MAX_FSET_NAME'.
 *	destStates
 *		The clone's states.  The legal values are the same
 *		as the legal values for the `vol_st.states' field within
 *		the `vol_status' structure.  For convenience, the
 *		FTU_FSET_STATES_* values may be used here to specify the
 *		type of the clone.
 *
 * RETURN CODES:
 *	NOT_ATTACHED
 *		There is no attached aggregate corresponding to `aggrId'.
 *	NO_SUCH_FSET
 *		There is no fileset corresponding to `*srcIdP'.
 *	NAME_CLASH
 *		The same name is already in use within the specified aggregate.
 *	ID_CLASH
 *		The same ID is already in use within the specified aggregate.
 */
IMPORT long ftu_AggrCloneFsetWithoutSwap _TAKES((
    IN ftu_aggrId_t	aggrId,
    IN ftu_fsetId_t*	srcIdP,
    IN ftu_fsetId_t*	destId,
    IN char*		destNameP,
    IN u_long		destStates
));
#endif

/*
 * ftu_AggrRecloneFset()
 *	Reclones a fileset into the closest backing fileset.
 *
 * PARAMETERS:
 *	aggrId
 *		The aggregate ID of the attached aggregate containing the
 *		fileset identified by `*fsetIdP'.
 *	*fsetIdP
 *		The ID for the target of the reclone.
 *
 * RETURN CODES:
 *	NOT_ATTACHED
 *		There is no attached aggregate corresponding to `aggrId'.
 *	NO_SUCH_FSET
 *		There is no fileset corresponding to `*fsetIdP'.
 *	NO_BACKING_FSET
 *		There is no backing fileset into which we can reclone.
 */
IMPORT long ftu_AggrRecloneFset _TAKES((
    IN ftu_aggrId_t	aggrId,
    IN ftu_fsetId_t*	fsetIdP
));

/*
 * ftu_AggrRecloneFsetInto()
 *	Reclones a fileset into a specific backing fileset.
 *
 * PARAMETERS:
 *	aggrId
 *		The aggregate ID of the attached aggregate containing the
 *		filesets identified by `*fsetIdP' and `*backingIdP'.
 *	*fsetIdP
 *		The ID for the read/write copy.
 *	*backingIdP
 *		The ID for the read-only copy.
 *
 * RETURN CODES:
 *	NO_SUCH_FSET
 *		There is no fileset corresponding to `*fsetIdP' or there is no
 *		fileset corresponding to `*backingIdP'.
 *	NOT_ATTACHED
 *		There is no attached aggregate corresponding to `aggrId'.
 */
IMPORT long ftu_AggrRecloneFsetInto _TAKES((
    IN ftu_aggrId_t	aggrId,
    IN ftu_fsetId_t*	fsetIdP,
    IN ftu_fsetId_t*	backingIdP
));

/*
 * ftu_AggrDestroyFset()
 *	Destroys a fileset, while preserving the integrity of its relatives
 *	in the cloning hierarchy.
 *
 * PARAMETERS:
 *	aggrId
 *		The aggregate ID of the attached aggregate containing the
 *		fileset identified by `*fsetIdP'.
 *	*fsetIdP
 *		The ID for the doomed fileset.
 *
 * RETURN CODES:
 *	LOCAL_MOUNT
 *		The fileset can not be destroyed because it is locally
 *		mounted.
 *	NO_SUCH_FSET
 *		There is no fileset corresponding to `*fsetIdP'.
 *	NOT_ATTACHED
 *		There is no attached aggregate corresponding to `aggrId'.
 */
IMPORT long ftu_AggrDestroyFset _TAKES((
    IN ftu_aggrId_t	aggrId,
    IN ftu_fsetId_t*	fsetIdP
));

/*
 * ftu_AggrDumpFset()
 *	Dumps the contents of a fileset to a file.
 *
 * PARAMETERS:
 *	aggrId
 *		The aggregate ID of the attached aggregate containing the
 *		fileset identified by `*fsetIdP'.
 *	*fsetIdP
 *		The ID of the fileset that is to be dumped.
 *	*dumpCriteriaP
 *		An ftu_dumpCriteria structure that has been initialized to
 *		indicate how the fileset is to be dumped.
 *	fileStream
 *		The contents of the fileset will be dumped to this stream.
 *
 * RETURN CODES:
 *	NO_MEMORY
 *		An attempt to dynamically allocate memory failed.
 *	NO_SUCH_FSET
 *		There is no fileset corresponding to `*fsetIdP'.
 *	NOT_ATTACHED
 *		There is no attached aggregate corresponding to `aggrId'.
 */
IMPORT long ftu_AggrDumpFset _TAKES((
    IN ftu_aggrId_t		aggrId,
    IN ftu_fsetId_t*		fsetId,
    IN ftu_dumpCriteria_t*	dumpCriteriaP,
    IN FILE*			fileStream
));

/*
 * ftu_AggrDumpFsetToPipe()
 *	Dumps the contents of a fileset to a pipe.
 *
 * PARAMETERS:
 *	aggrId
 *		The aggregate ID of the attached aggregate containing the
 *		fileset identified by `*fsetIdP'.
 *	*fsetIdP
 *		The ID of the fileset that is to be dumped.
 *	*dumpCriteriaP
 *		An ftu_dumpCriteria structure that has been initialized to
 *		indicate how the fileset is to be dumped.
 *	*pipeP
 *		The contents of the fileset will be dumped to this pipe.
 *
 * RETURN CODES:
 *	NO_MEMORY
 *		An attempt to dynamically allocate memory failed.
 *	NO_SUCH_FSET
 *		There is no fileset corresponding to `*fsetIdP'.
 *	NOT_ATTACHED
 *		There is no attached aggregate corresponding to `aggrId'.
 */
IMPORT long ftu_AggrDumpFsetToPipe _TAKES((
    IN ftu_aggrId_t		aggrId,
    IN ftu_fsetId_t*		fsetId,
    IN ftu_dumpCriteria_t*	dumpCriteriaP,
    IN afsPipe_t*		pipeP
));

/*
 * ftu_AggrRestoreFset()
 *	Restores the contents of a fileset from a file.
 *
 * PARAMETERS:
 *	aggrId
 *		The aggregate ID of the attached aggregate containing the
 *		fileset identified by `*fsetIdP'.
 *	*fsetIdP
 *		The ID of the fileset that is to be restored.
 *	fileStream
 *		The contents of the fileset will be restored from this stream.
 *	flags
 *		Bitmask made up of FTU_RESTORE_* values.
 *
 * RETURN CODES:
 *	BAD_DUMP
 *		The dump stream is corrupted.
 *	DISJOINT_DATE
 *		The dump stream contains an incremental dump, and the span of
 *		modifications that it contains starts later than the time of
 *		the fileset's last modification.
 *	DISJOINT_VERSION
 *		The dump stream contains an incremental dump, and the span of
 *		modifications that it contains start with a fileset
 *		version number that is higher than the fileset's current
 *		version number.
 *	NO_MEMORY
 *		An attempt to dynamically allocate memory failed.
 *	NO_SUCH_FSET
 *		There is no fileset corresponding to `*fsetIdP'.
 *	NOT_ATTACHED
 *		There is no attached aggregate corresponding to `aggrId'.
 *	RESTORED_INCONSISTENT_FSET
 *		The newly-restored fileset should be marked inconsistent
 *		(because it was inconsistent when it was dumped), but it is
 *		otherwise alright.
 *	RETROGRADE_DATE
 *		The dump stream's span of modifications will not extend the
 *		fileset's modification time.
 *	RETROGRADE_VERSION
 *		The dump stream's span of modifications will not extend the 
 *		filset's version number
 */
IMPORT long ftu_AggrRestoreFset _TAKES((
    IN ftu_aggrId_t	aggrId,
    IN ftu_fsetId_t*	fsetIdP,
    IN FILE*		fileStream,
    IN u_long		flags
));

/*
 * ftu_AggrRestoreFsetFromPipe()
 *	Restores the contents of a fileset from a pipe.
 *
 * PARAMETERS:
 *	aggrId
 *		The aggregate ID of the attached aggregate containing the
 *		fileset identified by `*fsetIdP'.
 *	*fsetIdP
 *		The ID of the fileset that is to be restored.
 *	*pipeP
 *		The contents of the fileset will be restored from this pipe.
 *	flags
 *		Bitmask made up of FTU_RESTORE_* values.
 *
 * RETURN CODES:
 *	BAD_DUMP
 *		The dump stream is corrupted.
 *	DISJOINT_DATE
 *		The dump stream contains an incremental dump, and the span of
 *		modifications that it contains starts later than the time of
 *		the fileset's last modification.
 *	DISJOINT_VERSION
 *		The dump stream contains an incremental dump, and the span of
 *		modifications that it contains start with a fileset
 *		version number that is higher than the fileset's current
 *		version number.
 *	NO_MEMORY
 *		An attempt to dynamically allocate memory failed.
 *	NO_SUCH_FSET
 *		There is no fileset corresponding to `*fsetIdP'.
 *	NOT_ATTACHED
 *		There is no attached aggregate corresponding to `aggrId'.
 *	RESTORED_INCONSISTENT_FSET
 *		The newly-restored fileset should be marked inconsistent
 *		(because it was inconsistent when it was dumped), but it is
 *		otherwise alright.
 *	RETROGRADE_DATE
 *		The dump stream's span of modifications will not extend the
 *		fileset's modification time.
 *	RETROGRADE_VERSION
 *		The dump stream's span of modifications will not extend the 
 *		filset's version number
 */
IMPORT long ftu_AggrRestoreFsetFromPipe _TAKES((
    IN ftu_aggrId_t	aggrId,
    IN ftu_fsetId_t*	fsetIdP,
    IN afsPipe_t*	pipeP,
    IN u_long		flags
));

/******************************************************************************
 * Fileset routines
 *****************************************************************************/

/*
 * ftu_FsetGetStatus()
 *	Returns status information for an open fileset.
 *
 * PARAMETERS:
 *	fsetDesc
 *		The descriptor for an open fileset.
 *	*fsetStatusP
 *		A preallocated `vol_status' structure whose fields will be
 *		set to reflect the specified fileset's status.
 *
 * RETURN CODES:
 * 	None returned directly by the ftutil layer, but codes from lower
 *	levels may be propagated.
 */
IMPORT long ftu_FsetGetStatus _TAKES((
    IN ftu_fsetDesc_t		fsetDesc,
    OUT struct vol_status*	fsetStatusP
));

/*
 * ftu_FsetSetStatus()
 *	Sets the specified fileset's status.
 *
 * PARAMETERS:
 *	fsetDesc
 *		The descriptor for an open fileset.
 *	fsetMask
 *		Mask made up of VOL_STAT_* values (as defined in 
 *		<dcedfs/volume.h>) indicating which status fields are
 *		being set.
 *	*fsetStatusP
 *		A `vol_status' structure.
 *
 * RETURN CODES:
 * 	None returned directly by the ftutil layer, but codes from lower
 *	levels may be propagated.
 */
IMPORT long ftu_FsetSetStatus _TAKES((
    IN ftu_fsetDesc_t		fsetDesc,
    IN u_long			fsetMask,
    IN struct vol_status*	fsetStatusP
));

/*
 * ftu_FsetBulkSetStatus()
 *	Sets a number of fileset's status fields in an atomic operation.
 *
 * PARAMETERS:
 *	numStatusDescs
 *		The number of status descriptors being passed in
 *		`statusDescArray'.
 *	statusDescArray
 *		Array of status descriptors.
 *
 * RETURN CODES:
 * 	None returned directly by the ftutil layer, but codes from lower
 *	levels may be propagated.
 */
IMPORT long ftu_FsetBulkSetStatus _TAKES((
    IN unsigned		numStatusDescs,
    IN vol_statusDesc_t	*statusDescArray
));

/*
 * ftu_FsetSwapIds()
 *	Swaps the identities of two filesets.
 *
 * PARAMETERS:
 *	fsetDesc1
 *		The descriptor for an open fileset.
 *	fsetDesc2
 *		The descriptor for an open fileset.
 *
 * RETURN CODES:
 * 	None returned directly by the ftutil layer, but codes from lower
 *	levels may be propagated.
 */
IMPORT long ftu_FsetSwapIds _TAKES((
    IN ftu_fsetDesc_t	fsetDesc1,
    IN ftu_fsetDesc_t	fsetDesc2
));

/*
 * ftu_FsetPushStatus()
 *	Serializes previous updates to a fileset's status with respect to
 *	later modifications of the fileset.
 *
 * PARAMETERS:
 *	fsetDesc
 *		The descriptor for an open fileset.
 *
 * RETURN CODES:
 * 	None returned directly by the ftutil layer, but codes from lower
 *	levels may be propagated.
 */
IMPORT long ftu_FsetPushStatus _TAKES((
    IN ftu_fsetDesc_t	fsetDesc
));

/*
 * ftu_FsetSync()
 *	Commits a fileset's modified data to secondary storage.
 *
 * PARAMETERS:
 *	fsetDesc
 *		The descriptor for an open fileset.
 *	syncOp
 *		One of the FTU_FSET_SYNC_* values, as defined above.
 *
 * RETURN CODES:
 * 	None returned directly by the ftutil layer, but codes from lower
 *	levels may be propagated.
 */
IMPORT long ftu_FsetSync _TAKES((
    IN ftu_fsetDesc_t	fsetDesc,
    IN int		syncOp
));

/*
 * ftu_FsetClone()
 * 	Makes a clone of a fileset.  When this operation completes the
 *	destination fileset will be the read-only copy and the original ID will
 *	still identify the read-write copy.
 *
 * PARAMETERS:
 *	srcDesc
 *		The descriptor for the source fileset.
 *	*destIdP
 *		An arbitrary, but unique, ID for the new fileset.
 *	destName
 *		The name to use for the clone.  The string can be
 *		no longer than `FTU_MAX_FSET_NAME'.
 *	destStates
 *		The clone's states.  The legal values are the same
 *		as the legal values for the `vol_st.states' field within
 *		the `vol_status' structure.  For convenience, the
 *		FTU_FSET_STATES_* values may be used here to specify the
 *		type of the clone.
 * 
 * RETURN CODES:
 *	NAME_CLASH
 *		The same name is already in use within the specified aggregate.
 *	ID_CLASH
 *		The same ID is already in use within the specified aggregate.
 */
IMPORT long ftu_FsetClone _TAKES((
    IN ftu_fsetDesc_t	srcDesc,
    IN ftu_fsetId_t*	destIdP,
    IN char*		destName,
    IN u_long		destStates
));

/*
 * ftu_FsetCloneWithKA()
 * 	Makes a clone of a fileset.  When this operation completes the
 *	destination fileset will be the read-only copy and the original ID will
 *	still identify the read-write copy.  This routine is identical
 *	to ftu_FsetClone(), except that it takes a keep alive routine and
 *	argument.
 *
 * PARAMETERS:
 *	srcDesc
 *		The descriptor for the source fileset.
 *	*destIdP
 *		An arbitrary, but unique, ID for the new fileset.
 *	destName
 *		The name to use for the clone.  The string can be
 *		no longer than `FTU_MAX_FSET_NAME'.
 *	destStates
 *		The clone's states.  The legal values are the same
 *		as the legal values for the `vol_st.states' field within
 *		the `vol_status' structure.  For convenience, the
 *		FTU_FSET_STATES_* values may be used here to specify the
 *		type of the clone.
 *	kaProc
 *		A routine to call periodically to allow the client of this
 *		interface to keep alive any volatile state that it may have.
 *		A value of 0 may be passed in, indicating that no keep alive
 *		procedure needs to be called.
 *	kaArg
 *		An arbitray data pointer that will be passed to the keep
 *		alive callback.
 * 
 * RETURN CODES:
 *	NAME_CLASH
 *		The same name is already in use within the specified aggregate.
 *	ID_CLASH
 *		The same ID is already in use within the specified aggregate.
 */
IMPORT long ftu_FsetCloneWithKA _TAKES((
    IN ftu_fsetDesc_t	srcDesc,
    IN ftu_fsetId_t*	destIdP,
    IN char*		destName,
    IN u_long		destStates,
    IN ftu_kaProc_t	kaProc,
    IN void*		kaArg
));

#if 0	/* XXX Not implemented */
/*
 * ftu_FsetCloneWithoutSwap()
 *	Same as the above routine, except that the original fileset becomes
 *	the read-only copy and the new fileset is the read-write copy.
 *
 * PARAMETERS:
 *	srcDesc
 *		The descriptor for the source fileset.
 *	*destIdP
 *		An arbitrary, but unique, ID for the new fileset.
 *	destName
 *		The name to use for the clone.  The string can be
 *		no longer than `FTU_MAX_FSET_NAME'.
 *	destStates
 *		The clone's states.  The legal values are the same
 *		as the legal values for the `vol_st.states' field within
 *		the `vol_status' structure.  For convenience, the
 *		FTU_FSET_STATES_* values may be used here to specify the
 *		type of the clone.
 *
 * RETURN CODES:
 *	NAME_CLASH
 *		The same name is already in use within the specified aggregate.
 *	ID_CLASH
 *		The same ID is already in use within the specified aggregate.
 */
IMPORT long ftu_FsetCloneWithoutSwap _TAKES((
    IN ftu_fsetDesc_t	srcDesc,
    IN char*		destName,
    IN ftu_fsetId_t*	destIdP,
    IN u_long		destStates
));
#endif

/*
 * ftu_FsetReclone()
 *	Reclones a fileset into the closest backing fileset.
 *
 * PARAMETERS:
 *	fsetDesc
 *		The descriptor for an open fileset.
 *
 * RETURN CODES:
 *	NO_BACKING_FSET
 *		There is no backing fileset into which we can reclone.
 */
IMPORT long ftu_FsetReclone _TAKES((
    IN ftu_fsetDesc_t	fsetDesc
));

/*
 * ftu_FsetRecloneInto()
 *	Reclones a fileset into a specific backing volume.
 *
 * PARAMETERS:
 *	fsetDesc
 *		The descriptor for the read/write copy.
 *	backingDesc
 *		The descriptor for the read-only copy.
 *
 * RETURN CODES:
 * 	None returned directly by the ftutil layer, but codes from lower
 *	levels may be propagated.
 */
IMPORT long ftu_FsetRecloneInto _TAKES((
    IN ftu_fsetDesc_t	fsetDesc,
    IN ftu_fsetId_t*	backingId
));

/*
 * ftu_FsetRecloneIntoWithKA()
 *	Reclones a fileset into a specific backing volume.  This routine is
 *	identical to ftu_FsetRecloneInto(), except that it takes a keep alive
 *	routine and argument.
 *
 * PARAMETERS:
 *	fsetDesc
 *		The descriptor for the read/write copy.
 *	backingDesc
 *		The descriptor for the read-only copy.
 *	kaProc
 *		A routine to call periodically to allow the client of this
 *		interface to keep alive any volatile state that it may have.
 *		A value of 0 may be passed in, indicating that no keep alive
 *		procedure needs to be called.
 *	kaArg
 *		An arbitray data pointer that will be passed to the keep
 *		alive callback.
 *
 * RETURN CODES:
 * 	None returned directly by the ftutil layer, but codes from lower
 *	levels may be propagated.
 */
IMPORT long ftu_FsetRecloneIntoWithKA _TAKES((
    IN ftu_fsetDesc_t	fsetDesc,
    IN ftu_fsetId_t*	backingId,
    IN ftu_kaProc_t	kaProc,
    IN void*		kaArg
));

/*
 * ftu_FsetDestroy()
 *	Destroys a fileset, while preserving the integrity of its relatives
 *	in the cloning hierarchy.
 *
 * PARAMETERS:
 *	fsetDesc
 *		The descriptor for the doomed fileset.
 *
 * RETURN CODES:
 *	LOCAL_MOUNT
 *		The fileset can not be destroyed because it is locally
 *		mounted.
 */
IMPORT long ftu_FsetDestroy _TAKES((
    IN ftu_fsetDesc_t	fsetDesc
));

/*
 * ftu_FsetDestroyWithKA()
 *	Destroys a fileset, while preserving the integrity of its relatives
 *	in the cloning hierarchy.  This routine is identical to
 *	ftu_FsetDestroy(), except that it takes a keep alive routine and
 *	argument.
 *
 * PARAMETERS:
 *	fsetDesc
 *		The descriptor for the doomed fileset.
 *	kaProc
 *		A routine to call periodically to allow the client of this
 *		interface to keep alive any volatile state that it may have.
 *		A value of 0 may be passed in, indicating that no keep alive
 *		procedure needs to be called.
 *	kaArg
 *		An arbitray data pointer that will be passed to the keep
 *		alive callback.
 *
 * RETURN CODES:
 *	LOCAL_MOUNT
 *		The fileset can not be destroyed because it is locally
 *		mounted.
 */
IMPORT long ftu_FsetDestroyWithKA _TAKES((
    IN ftu_fsetDesc_t	fsetDesc,
    IN ftu_kaProc_t	kaProc,
    IN void*		kaArg
));

/*
 * ftu_FsetDump()
 *	Dumps the contents of a fileset to a file.
 *
 * PARAMETERS:
 *	fsetDesc
 *		The descriptor for an open fileset
 *	*dumpCriteriaP
 *		An ftu_dumpCriteria structure that has been initialized to
 *		indicate how the fileset is to be dumped.
 *	fileStream
 *		The contents of the fileset will be dumped to this stream.
 *
 * RETURN CODES:
 *	NO_MEMORY
 *		An attempt to dynamically allocate memory failed.
 */
IMPORT long ftu_FsetDump _TAKES((
    IN ftu_fsetDesc_t		fsetDesc,
    IN ftu_dumpCriteria_t*	dumpCriteria,
    IN FILE*			fileStream
));

/*
 * ftu_FsetDumpToPipe()
 *	Dumps the contents of a fileset to a pipe.
 *
 * PARAMETERS:
 *	fsetDesc
 *		The descriptor for an open fileset
 *	*dumpCriteriaP
 *		An ftu_dumpCriteria structure that has been initialized to
 *		indicate how the fileset is to be dumped.
 *	*pipeP
 *		The contents of the fileset will be dumped to this pipe.
 *
 * RETURN CODES:
 *	NO_MEMORY
 *		An attempt to dynamically allocate memory failed.
 */
IMPORT long ftu_FsetDumpToPipe _TAKES((
    IN ftu_fsetDesc_t		fsetDesc,
    IN ftu_dumpCriteria_t*	dumpCriteria,
    IN afsPipe_t*		pipeP
));

/*
 * ftu_FsetDumpToPipeWithKA()
 *	Dumps the contents of a fileset to a pipe.  This routine is
 *	identical to ftu_FsetDumpToPipe(), except that it takes a keep alive
 *	routine and argument.
 *
 * PARAMETERS:
 *	fsetDesc
 *		The descriptor for an open fileset
 *	*dumpCriteriaP
 *		An ftu_dumpCriteria structure that has been initialized to
 *		indicate how the fileset is to be dumped.
 *	*pipeP
 *		The contents of the fileset will be dumped to this pipe.
 *
 * RETURN CODES:
 *	NO_MEMORY
 *		An attempt to dynamically allocate memory failed.
 */
IMPORT long ftu_FsetDumpToPipeWithKA _TAKES((
    IN ftu_fsetDesc_t		fsetDesc,
    IN ftu_dumpCriteria_t*	dumpCriteria,
    IN afsPipe_t*		pipeP,
    IN ftu_kaProc_t		kaProc,
    IN void*			kaArg
));

/*
 * ftu_FsetRestore()
 *	Restores the contents of a fileset from a file.
 *
 * PARAMETERS:
 *	fsetDesc
 *		The descriptor for an open fileset
 *	fileStream
 *		The contents of the fileset will be restored from this stream.
 *	flags
 *		Bitmask made up of FTU_RESTORE_* values.
 *
 * RETURN CODES:
 *	BAD_DUMP
 *		The dump stream is corrupted.
 *	DISJOINT_DATE
 *		The dump stream contains an incremental dump, and the span of
 *		modifications that it contains starts later than the time of
 *		the fileset's last modification.
 *	DISJOINT_VERSION
 *		The dump stream contains an incremental dump, and the span of
 *		modifications that it contains start with a fileset
 *		version number that is higher than the fileset's current
 *		version number.
 *	NO_MEMORY
 *		An attempt to dynamically allocate memory failed.
 *	RESTORED_INCONSISTENT_FSET
 *		The newly-restored fileset should be marked inconsistent
 *		(because it was inconsistent when it was dumped), but it is
 *		otherwise alright.
 *	RETROGRADE_DATE
 *		The dump stream's span of modifications will not extend the
 *		fileset's modification time.
 *	RETROGRADE_VERSION
 *		The dump stream's span of modifications will not extend the 
 *		filset's version number
 */
IMPORT long ftu_FsetRestore _TAKES((
    IN ftu_fsetDesc_t	fsetDesc,
    IN FILE*		fileStream,
    IN u_long		flags
));

/*
 * ftu_FsetRestoreFromPipe()
 *	Restores the contents of a fileset from a pipe.
 *
 * PARAMETERS:
 *	fsetDesc
 *		The descriptor for an open fileset
 *	*pipeP
 *		The contents of the fileset will be restored from this pipe.
 *	flags
 *		Bitmask made up of FTU_RESTORE_* values.
 *
 * RETURN CODES:
 *	BAD_DUMP
 *		The dump stream is corrupted.
 *	DISJOINT_DATE
 *		The dump stream contains an incremental dump, and the span of
 *		modifications that it contains starts later than the time of
 *		the fileset's last modification.
 *	DISJOINT_VERSION
 *		The dump stream contains an incremental dump, and the span of
 *		modifications that it contains start with a fileset
 *		version number that is higher than the fileset's current
 *		version number.
 *	NO_MEMORY
 *		An attempt to dynamically allocate memory failed.
 *	RESTORED_INCONSISTENT_FSET
 *		The newly-restored fileset should be marked inconsistent
 *		(because it was inconsistent when it was dumped), but it is
 *		otherwise alright.
 *	RETROGRADE_DATE
 *		The dump stream's span of modifications will not extend the
 *		fileset's modification time.
 *	RETROGRADE_VERSION
 *		The dump stream's span of modifications will not extend the 
 *		filset's version number
 */
IMPORT long ftu_FsetRestoreFromPipe _TAKES((
    IN ftu_fsetDesc_t	fsetDesc,
    IN afsPipe_t*	pipeP,
    IN u_long		flags
));

/*
 * ftu_FsetRestoreFromPipeWithKA()
 *	Restores the contents of a fileset from a pipe.  This routine is
 *	identical to ftu_FsetRestoreFromPipe(), except that it takes a keep
 *	alive routine and argument.
 *
 * PARAMETERS:
 *	fsetDesc
 *		The descriptor for an open fileset
 *	*pipeP
 *		The contents of the fileset will be restored from this pipe.
 *	flags
 *		Bitmask made up of FTU_RESTORE_* values.
 *	kaProc
 *		A routine to call periodically to allow the client of this
 *		interface to keep alive any volatile state that it may have.
 *		A value of 0 may be passed in, indicating that no keep alive
 *		procedure needs to be called.
 *	kaArg
 *		An arbitray data pointer that will be passed to the keep
 *		alive callback.
 *
 * RETURN CODES:
 *	BAD_DUMP
 *		The dump stream is corrupted.
 *	DISJOINT_DATE
 *		The dump stream contains an incremental dump, and the span of
 *		modifications that it contains starts later than the time of
 *		the fileset's last modification.
 *	DISJOINT_VERSION
 *		The dump stream contains an incremental dump, and the span of
 *		modifications that it contains start with a fileset
 *		version number that is higher than the fileset's current
 *		version number.
 *	NO_MEMORY
 *		An attempt to dynamically allocate memory failed.
 *	RESTORED_INCONSISTENT_FSET
 *		The newly-restored fileset should be marked inconsistent
 *		(because it was inconsistent when it was dumped), but it is
 *		otherwise alright.
 *	RETROGRADE_DATE
 *		The dump stream's span of modifications will not extend the
 *		fileset's modification time.
 *	RETROGRADE_VERSION
 *		The dump stream's span of modifications will not extend the 
 *		filset's version number
 */
IMPORT long ftu_FsetRestoreFromPipeWithKA _TAKES((
    IN ftu_fsetDesc_t	fsetDesc,
    IN afsPipe_t*	pipeP,
    IN u_long		flags,
    IN ftu_kaProc_t	kaProc,
    IN void*		kaArg
));

#if 0 /* XXX Not implemented */
IMPORT long ftu_FsetScan _TAKES((
    IN ftu_fsetDesc_t	fsetDesc,
    IN long		index
));

/******************************************************************************
 * Anode utilities
 *****************************************************************************/

IMPORT long ftu_AnodeCreate _TAKES((
    IN ftu_fsetDesc_t		fsetDesc,
    IN int			index,
    IN struct xvfs_attr*	attrP
));

IMPORT long ftu_AnodeDelete _TAKES((
    IN ftu_fsetDesc_t fsetDesc
));

IMPORT long ftu_AnodeRead _TAKES((
    IN ftu_fsetDesc_t	fsetDesc,
    OUT char*   	buf,
    IN long		len,
    OUT long*		outLenP
));

IMPORT long ftu_AnodeWrite _TAKES((
    IN ftu_fsetDesc_t	fsetDesc,
    IN char*		buf,
    IN long		len,
    OUT long*		outLenP
));

IMPORT long ftu_AnodeTruncate _TAKES((
    IN ftu_fsetDesc_t	fsetDesc,
    IN long		newSize
));

IMPORT long ftu_AnodeGetattr _TAKES((
    IN ftu_fsetDesc_t		fsetDesc,
    OUT struct xvfs_attr*	attrP
));

IMPORT long ftu_AnodeSetattr _TAKES((
    IN long			fsetDesc,
    IN struct xvfs_attr*	attrP
));

IMPORT long ftu_AnodeGetacl _TAKES((
    IN ftu_fsetDesc_t	fsetDesc,
    OUT struct dfs_acl*	aclP,
    IN long		whichAcl
));

IMPORT long ftu_AnodeSetacl _TAKES((
    IN ftu_fsetDesc_t	fsetDesc,
    IN struct dfs_acl*	acl,
    IN long		whichAcl,
    IN long		index
));

IMPORT long ftu_AnodeCopyacl _TAKES((
    IN ftu_fsetDesc_t	fsetDesc,
    IN long		whichDestAcl,
    IN long		srcIndex,
    IN long		whichSrcAcl
));
#endif


/* from cloneops.c */
IMPORT void ftutil_ThreadYield _TAKES ((void));

IMPORT long ftutil_VolClone _TAKES ((
				      int srcftd,
				      int destftd,
				      char *errBuf
				      ));

IMPORT long ftutil_VolUnClone _TAKES ((
					int ftd,
					int backftd,
					char *errBuf
					));
IMPORT long ftutil_VolReClone _TAKES ((
					int ftd,
					int backftd,
					char *errBuf
					));
IMPORT long ftutil_VolDeplete _TAKES ((
				        int ftd,
					char *errBuf
					));

IMPORT long ftutil_VolCloneAux _TAKES ((
					 int srcftd,
					 int destftd,
					 char *errBuf,
					 void (*keepAliveProc)(),
					 void *keepAliveRock
					 ));

IMPORT long ftutil_VolUnCloneAux _TAKES ((
					   int ftd,
					   int backftd,
					   char *errBuf,
					   void (*keepAliveProc)(),
					   void *keepAliveRock
					   ));
IMPORT long ftutil_VolReCloneAux _TAKES ((
					   int ftd,
					   int backftd,
					   char *errBuf,
					   void (*keepAliveProc)(),
					   void *keepAliveRock
					   ));
IMPORT long ftutil_VolDepleteAux _TAKES ((
					   int ftd,
					   char *errBuf,
					   void (*keepAliveProc)(),
					   void *keepAliveRock
					   ));

#endif	/* _TRANSARC_DFS_FTUTIL_H */
