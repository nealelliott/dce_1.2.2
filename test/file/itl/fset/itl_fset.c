/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_fset.c,v $
 * Revision 1.1.411.1  1996/10/17  18:26:05  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:08  damon]
 *
 * $EndLog$
 */

/*
 * (C) Copyright 1996, 1992 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

#include <dcedfs/ftutil.h>
#include <dcedfs/vol_errs.h>

#include <utils/itl/itl_utils.h>

#include "itl_fset.h"

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/fset/itl_fset.c,v 1.1.411.1 1996/10/17 18:26:05 damon Exp $")

#define MOUNT			"epimount"
#define UNMOUNT			"epiunmount"

PRIVATE char* infoMsg = "The fset module provides command interfaces to the \
fileset utility functions.\n";

/*
 * Interface routines 
 */
PRIVATE long CreateFset _TAKES((IN opaque rock, IN opaque argv[]));
PRIVATE long DestroyFset _TAKES((IN opaque rock, IN opaque argv[]));
PRIVATE long MountFset _TAKES((IN opaque rock, IN opaque argv[]));
PRIVATE long UnmountFset _TAKES((IN opaque rock, IN opaque argv[]));
PRIVATE long CloneFset _TAKES((IN opaque rock, IN opaque argv[]));
PRIVATE long RecloneFset _TAKES((IN opaque rock, IN opaque argv[]));
PRIVATE long GetFsetSize _TAKES((IN opaque rock, IN opaque argv[]));
PRIVATE long SetFsetQuota _TAKES((IN opaque rock, IN opaque argv[]));
PRIVATE long SumFset _TAKES((IN opaque rock, IN opaque argv[]));

/* CreateFset args */
PRIVATE itlu_argDesc_t createFsetArgs[] = {
    {"aggrId", "Aggregate Id: ", itlu_GetInteger},
    {"fsetId", "Fileset Id: ", itlFset_GetFsetId},
    {"fsetName", "Fileset Name: ", itlu_GetString},
    {NULL}
};

/* DestroyFset args */
PRIVATE itlu_argDesc_t destroyFsetArgs[] = {
    {"aggrId", "Aggregate Id: ", itlu_GetInteger},
    {"fsetId", "Fileset Id: ", itlFset_GetFsetId},
    {NULL}
};

/* MountFset args */
PRIVATE itlu_argDesc_t mountFsetArgs[] = {
    {"fsetId", "Fileset Id: ", itlu_GetString},
    {"mountPoint", "Mount Point: ", itlu_GetString},
    {NULL}
};

/* UnmountFset args */
PRIVATE itlu_argDesc_t unmountFsetArgs[] = {
    {"mountPoint", "Mount Point: ", itlu_GetString},
    {NULL}
};

/* CloneFset args */
PRIVATE itlu_argDesc_t cloneFsetArgs[] = {
    {"aggrId", "Aggregate Id: ", itlu_GetInteger},
    {"srcId", "Source Id: ", itlFset_GetFsetId},
    {"destId", "Destination Id: ", itlFset_GetFsetId},
    {"destName", "Destination Name: ", itlu_GetString},
    {NULL}
};

/* RecloneFset args */
PRIVATE itlu_argDesc_t recloneFsetArgs[] = {
    {"aggrId", "Aggregate Id: ", itlu_GetInteger},
    {"fsetId", "Fileset Id: ", itlFset_GetFsetId},
    {NULL}
};

/* GetFsetSize args */
PRIVATE itlu_argDesc_t getFsetSizeArgs[] = {
    {"aggrId", "Aggregate Id: ", itlu_GetInteger},
    {"fsetId", "Fileset Id: ", itlFset_GetFsetId},
    {"sizeVar", "Size Variable: ", itlu_GetString},
    {NULL}
};

/* SetFsetQuota args */
PRIVATE itlu_argDesc_t setFsetQuotaArgs[] = {
    {"aggrId", "Aggregate Id: ", itlu_GetInteger},
    {"fsetId", "Fileset Id: ", itlFset_GetFsetId},
    {"quota", "New Quota: ", itlu_GetInteger},
    {NULL}
};

/* SumFset args */
PRIVATE itlu_argDesc_t sumFsetArgs[] = {
    {"aggrId", "Aggregate Id: ", itlu_GetInteger},
    {"fromId", "Fileset Id: ", itlFset_GetFsetId},
    {"sumVar", "Sum Variable: ", itlu_GetString},
    {NULL}
};

/*
 *  Define command table entries supported by the functions in this file.
 */
PRIVATE itlu_commandTable_t commandTable[] = {
    {"createFset", CreateFset, NULL, createFsetArgs, FALSE,
	 "Create a fileset."},
    {"destroyFset", DestroyFset, NULL, destroyFsetArgs, FALSE,
	 "Destroy a fileset."},
    {"mountFset", MountFset, NULL, mountFsetArgs, FALSE,
	 "Mount a fileset."},
    {"unmountFset", UnmountFset, NULL, unmountFsetArgs, FALSE,
	 "Unmount a fileset."},
    {"cloneFset", CloneFset, NULL, cloneFsetArgs, FALSE,
	 "Clone a fileset."},
    {"recloneFset", RecloneFset, NULL, recloneFsetArgs, FALSE,
	 "Reclone a fileset."},
    {"getFsetSize", GetFsetSize, NULL, getFsetSizeArgs, FALSE,
	 "Get a fileset's size"},
    {"setFsetQuota", SetFsetQuota, NULL, setFsetQuotaArgs, FALSE,
	 "Set a fileset's quota"},
    {"sumFset", SumFset, NULL, sumFsetArgs, FALSE,
	 "Generate a checksum for a fileset"},
    {NULL}
};


EXPORT long
itlFset_Init()
{
    long		code;
    unsigned		numEntries;
    boolean_t		success;
    itl_commandTable_t*	stdCmdTable;

    code = itlu_Cnvt2StdCmdTbl(commandTable, COMMAND_MODE_ALL, "fset",
			       &stdCmdTable, &numEntries);
    if (code)
	code;
			    
    success = itl_AddModuleCmds("fset", stdCmdTable, numEntries,
				COMMAND_MODE_ALL, infoMsg);
    if (!success)
	return ITLU_UNKNOWN_ERROR;

    return 0;
}	/* itlFset_Init() */


EXPORT long
itlFset_GetFsetId(result, shouldFree)
    OUT opaque*		result;
    OUT boolean_t*	shouldFree;
{
    ftu_fsetId_t*	fsetId;
    char*		fsetIdStr;
    boolean_t		parseError = FALSE;

    fsetIdStr = itl_GetString(&parseError, 0);
    if (!parseError) {
	long		code;

	ITL_UTIL_ALLOC(fsetId, ftu_fsetId_t*, sizeof *fsetId);

	if (code = ftu_ParseFsetId(fsetIdStr, fsetId)) {
	    ITL_UTIL_FREE(fsetId);
	    ITL_UTIL_FREE(fsetIdStr);
	    return code;
	} else
	    *result = fsetId;

	ITL_UTIL_FREE(fsetIdStr);
    }

    return parseError ? ITLU_UNKNOWN_ERROR : 0;
}	/* itlFset_GetFsetId() */


PRIVATE long
CreateFset(rock, args)
    opaque	rock;
    opaque*	args;
{
    long		code;
    ftu_fsetDesc_t	fsetDesc;
    struct vol_status	status;

    code = ftu_AggrCreateFset((ftu_aggrId_t)args[0], args[1], args[2],
			      (ftu_fsetId_t*)0, FTU_FSET_STATES_RW, 5000,
			      AGGR_CREATE_ROOT);
    if (code)
	return code;

    /*
     * Newly created filesets are left off-line; bring this one on-line
     */
    code = ftu_AggrOpenFset((ftu_aggrId_t)args[0], args[1],
			    FTU_SYS_GET_STATUS|FTU_SYS_SET_STATUS,
			    VOLERR_TRANS_SETSTATUS,
			    &fsetDesc);
    if (code)
	return code;

    if (code = ftu_FsetGetStatus(fsetDesc, &status)) {
	(void)ftu_AggrCloseFset(fsetDesc);
	return code;
    }

    status.vol_st.states &= ~VOL_OFFLINE;
    /* Also, keep this limit out of the way until we have operators to get/set it */
    AFS_hset64(status.vol_dy.visQuotaLimit, 0, 0x40000000);

    if (code = ftu_FsetSetStatus(fsetDesc, VOL_STAT_STATES | VOL_STAT_VISLIMIT, &status)) {
	(void)ftu_AggrCloseFset(fsetDesc);
	return code;
    }

    if (code = ftu_AggrCloseFset(fsetDesc))
	return code;

    return 0;
}	/* CreateFset() */


PRIVATE long
DestroyFset(rock, args)
    opaque	rock;
    opaque*	args;
{
    long	code;

    code = ftu_AggrDestroyFset((ftu_aggrId_t)args[0], args[1]);

    return code;
}	/* DestroyFset() */


PRIVATE long
MountFset(rock, args)
    opaque	rock;
    opaque*	args;
{
    char*	argv[4];

    argv[0] = MOUNT;
    argv[1] = args[0];			/* Mount point */
    argv[2] = args[1];			/* Fileset Id */
    argv[3] = 0;

    return itlu_System(MOUNT, argv);
}	/* MountFset() */


PRIVATE long
UnmountFset(rock, args)
    opaque	rock;
    opaque*	args;
{
    char*	argv[3];

    argv[0] = UNMOUNT;
    argv[1] = args[0];			/* Mount point */
    argv[2] = 0;

    return itlu_System(UNMOUNT, argv);
}	/* MountFset() */


PRIVATE long
CloneFset(rock, args)
    opaque	rock;
    opaque*	args;
{
    long	code;

    code = ftu_AggrCloneFset((ftu_aggrId_t)args[0], args[1], args[2], args[3],
			     FTU_FSET_STATES_BK);

    return code;
}	/* CloneFset() */


PRIVATE long
RecloneFset(rock, args)
    opaque	rock;
    opaque*	args;
{
    long	code;

    code = ftu_AggrRecloneFset((ftu_aggrId_t)args[0], args[1]);

    return code;
}	/* RecloneFset() */


/* Both GetFsetSize and SetFsetQuota use only the 1k-block interface
   to the allocation usage and allocation quota, simply mimicking the old
   quota interfaces.  New procedures (new ITL operations) should deal
   with the dual quotas in byte counts. */
PRIVATE long
GetFsetSize(rock, args)
    opaque	rock;
    opaque*	args;
{
    long		code;
    struct vol_status	fsetStatus;

    code = ftu_AggrGetFsetStatus((ftu_aggrId_t)args[0], args[1], &fsetStatus);
    if (code)
	return code;

    AFS_hrightshift(fsetStatus.vol_dy.allocUsage, 10);	/* cvt to 1k units */
    (void)itl_SetIntVariable(args[2], AFS_hgetlo(fsetStatus.vol_dy.allocUsage),
			     var_same);

    return 0;
}	/* GetFsetSize() */


PRIVATE long
SetFsetQuota(rock, args)
    opaque	rock;
    opaque*	args;
{
    long		code;
    struct vol_status	fsetStatus;

    AFS_hset64(fsetStatus.vol_dy.allocLimit, 0, (long)args[2]);
    AFS_hleftshift(fsetStatus.vol_dy.allocLimit, 10);	/* cvt from 1k units to bytes */
    code = ftu_AggrSetFsetStatus((ftu_aggrId_t)args[0], args[1],
				 VOL_STAT_ALLOCLIMIT, &fsetStatus);

    return code;
}	/* SetFsetQuota() */


PRIVATE long
SumFset(rock, args)
    opaque	rock;
    opaque*	args;
{
    long		code;
    struct vol_status	fsetStatus;

    code = ftu_AggrGetFsetStatus((ftu_aggrId_t)args[0], args[1], &fsetStatus);
    if (code)
	return code;

    (void)itl_SetIntVariable(args[2],
			     AFS_hgetlo(fsetStatus.vol_dy.visQuotaUsage),
			     var_same);

    return 0;
}	/* SumFset() */
