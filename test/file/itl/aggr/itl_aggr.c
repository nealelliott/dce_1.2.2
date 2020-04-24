/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_aggr.c,v $
 * Revision 1.1.412.1  1996/10/17  18:24:20  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:21  damon]
 *
 * Revision 1.1.407.1  1994/02/04  20:43:00  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:23:29  devsrc]
 * 
 * Revision 1.1.405.1  1993/12/07  17:43:14  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:06:56  jaffe]
 * 
 * Revision 1.1.2.5  1993/01/11  18:29:31  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:07:03  htf]
 * 
 * Revision 1.1.2.4  1992/11/24  21:44:56  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:44:36  bolinger]
 * 
 * Revision 1.1.2.3  1992/11/18  21:44:47  jaffe
 * 	Transarc delta: jdp-ot5415-mount-aggregate-on-attach-test 1.1
 * 	  Selected comments:
 * 	    This delta brings the ftutil tets up to date with the modifications made
 * 	    to the ftutil layer in the DFS delta of the same name.  There is also a
 * 	    matching Episode delta.
 * 	    Use the new interfaces to ftu_{Attach,Detach}Aggr().
 * 	[1992/11/18  14:46:55  jaffe]
 * 
 * Revision 1.1.2.2  1992/10/28  17:38:56  jaffe
 * 	Transarc delta: jdp-add-itl-stuff 1.8
 * 	  Selected comments:
 * 	    Add code for three new itl libraries: libitlUtils.a, libitlAggr.a, and
 * 	    libitlFset.a
 * 	    Routines for manipulating aggregates through ITL
 * 	    Update error message in itlu_CheckReturnCode() so that the offending command
 * 	    is printed.
 * 	    Added documentation for libitlUtils.a, fixed some bugs, and reorganized.
 * 	    Changed interface and implementation of result checking routines.
 * 	    Provide routine for converting a value back to a name, assuming that
 * 	    the value--name mapping was constructed using itlu_ProcessValueMap().
 * 	    Checkpointing in order to import another delta.
 * 	    Fix misc. compilation problems and close inorder to open another delta.
 * 	    Make this stuff compile on the pmax.
 * 	    Fix compilation warnings.
 * 	[1992/10/27  19:57:10  jaffe]
 * 
 * $EndLog$
 */

/*
 * (C) Copyright 1992 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

#include <dcedfs/ftutil.h>

#include <utils/itl/itl_utils.h>

#include "itl_aggr.h"

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/aggr/itl_aggr.c,v 1.1.412.1 1996/10/17 18:24:20 damon Exp $")


#define NEWAGGR			"newaggr"
#define NEWAGGR_OVERWRITE_FLAG	"-overwrite"
#define SALVAGER		"salvage"
#define SALVAGER_VERIFY_FLAG	"-verify"
#define SALVAGER_RECOVER_FLAG	"-recoveronly"

PRIVATE char* infoMsg = "The aggr module provides command interfaces to \
aggregate-related functions.\n";


/*
 * Interface routines 
 */
PRIVATE long InitAggr _TAKES((IN opaque rock, IN opaque argv[]));
PRIVATE long VerifyAggr _TAKES((IN opaque rock, IN opaque argv[]));
PRIVATE long RecoverAggr _TAKES((IN opaque rock, IN opaque argv[]));
PRIVATE long SalvageAggr _TAKES((IN opaque rock, IN opaque argv[]));
PRIVATE long AttachAggr _TAKES((IN opaque rock, IN opaque argv[]));
PRIVATE long DetachAggr _TAKES((IN opaque rock, IN opaque argv[]));
PRIVATE long GetAggrInfo _TAKES((IN opaque rock, IN opaque argv[]));
PRIVATE long GetRawDeviceName _TAKES((IN opaque rock, IN opaque argv[]));
PRIVATE long LockAggrDevice _TAKES((IN opaque rock, IN opaque argv[]));
PRIVATE long UnlockAggrDevice _TAKES((IN opaque rock, IN opaque argv[]));

PRIVATE itlu_valueMap_t ftutilErrorCodes[] = {
    {"FTU_E_ALREADY_ATTACHED", FTU_E_ALREADY_ATTACHED},
    {"FTU_E_NOT_ATTACHED", FTU_E_NOT_ATTACHED},
    {"FTU_E_TRY_RECOVERY", FTU_E_TRY_RECOVERY},
    {"FTU_E_LOCAL_MOUNT", FTU_E_LOCAL_MOUNT},
    {"FTU_E_NO_BACKING_FSET", FTU_E_NO_BACKING_FSET},
    {"FTU_E_NO_SUCH_FSET", FTU_E_NO_SUCH_FSET},
    {"FTU_E_BAD_DUMP", FTU_E_BAD_DUMP},
    {"FTU_E_DISJOINT_DATE", FTU_E_DISJOINT_DATE},
    {"FTU_E_DISJOINT_VERSION", FTU_E_DISJOINT_VERSION},
    {"FTU_E_RETROGRADE_DATE", FTU_E_RETROGRADE_DATE},
    {"FTU_E_RETROGRADE_VERSION", FTU_E_RETROGRADE_VERSION},
    {"FTU_E_NAME_TOO_LONG", FTU_E_NAME_TOO_LONG},
    {"FTU_E_ID_CLASH", FTU_E_ID_CLASH},
    {"FTU_E_NAME_CLASH", FTU_E_NAME_CLASH},
    {"FTU_E_MALFORMED_FSET_ID", FTU_E_MALFORMED_FSET_ID},
    {"FTU_E_NO_MEMORY", FTU_E_NO_MEMORY},
    {"FTU_E_ATTACHED", FTU_E_ATTACHED},
    {"FTU_E_CANT_FIND_CHARACTER_DEVICE", FTU_E_CANT_FIND_CHARACTER_DEVICE},
    {"FTU_E_DEVICE_LOCKED", FTU_E_DEVICE_LOCKED},
    {"FTU_E_NOT_A_BLOCK_DEVICE", FTU_E_NOT_A_BLOCK_DEVICE},
    {"FTU_E_NOT_A_CHARACTER_DEVICE", FTU_E_NOT_A_CHARACTER_DEVICE},
    {"FTU_E_NOT_A_DEVICE", FTU_E_NOT_A_DEVICE},
    {NULL}
};

/* InitAggr args */
PRIVATE itlu_argDesc_t initAggrArgs[] = {
    {"devName", "Device Name: ", itlu_GetString},
    {"blockSize", "Block Size: ", itlu_GetString},
    {"fragSize", "Fragment Size: ", itlu_GetString},
    {NULL}
};

/* VerifyAggr args */
PRIVATE itlu_argDesc_t verifyAggrArgs[] = {
    {"devName", "Device Name: ", itlu_GetString},
    {NULL}
};

/* RecoverAggr args */
PRIVATE itlu_argDesc_t recoverAggrArgs[] = {
    {"devName", "Device Name: ", itlu_GetString},
    {NULL}
};

/* SalvageAggr args */
PRIVATE itlu_argDesc_t salvageAggrArgs[] = {
    {"devName", "Device Name: ", itlu_GetString},
    {NULL}
};

/* AttachAggr args */
PRIVATE itlu_argDesc_t attachAggrArgs[] = {
    {"devName", "Device Name: ", itlu_GetString},
    {"aggrType", "Aggregate Type: ", itlu_GetString},
    {"aggrName", "Aggregate Name: ", itlu_GetString},
    {"aggrId", "Aggregate Id: ", itlu_GetInteger},
    {NULL}
};

/* DetachAggr args */
PRIVATE itlu_argDesc_t detachAggrArgs[] = {
    {"aggrId", "Aggregate Id: ", itlu_GetInteger},
    {NULL}
};

/* GetAggrInfo args */
PRIVATE itlu_argDesc_t getAggrInfoArgs[] = {
    {"aggrId", "Aggregate Id: ", itlu_GetInteger},
    {"infoVar", "Info. Variable: ", itlu_GetString},
    {NULL}
};

/* GetRawDeviceName args */
PRIVATE itlu_argDesc_t getRawDeviceNameArgs[] = {
    {"blockDevName", "Block Device Name: ", itlu_GetString},
    {"rawDevVarName", "Raw Device Variable Name: ", itlu_GetString},
    {NULL}
};

/* LockAggrDevice args */
PRIVATE itlu_argDesc_t lockAggrDeviceArgs[] = {
    {"rawDevName", "Raw Device Name: ", itlu_GetString},
    {"devLockVar", "Lock Variable: ", itlu_GetString},
    {NULL}
};

/* UnlockAggrDevice args */
PRIVATE itlu_argDesc_t unlockAggrDeviceArgs[] = {
    {"devLockVar", "Lock Value: ", itlu_GetInteger},
    {NULL}
};

PRIVATE itlu_commandTable_t commandTable[] = {
    {"initAggr", InitAggr, NULL, initAggrArgs, FALSE,
	 "Initialize an aggregate."},
    {"verifyAggr", VerifyAggr, NULL, verifyAggrArgs, FALSE,
	 "Verify an aggregate."},
    {"recoverAggr", RecoverAggr, NULL, recoverAggrArgs, FALSE,
	 "Recover an aggregate."},
    {"salvageAggr", SalvageAggr, NULL, salvageAggrArgs, FALSE,
	 "Salvage an aggregate."},
    {"attachAggr", AttachAggr, NULL, attachAggrArgs, FALSE,
	 "Attach an aggregate."},
    {"detachAggr", DetachAggr, NULL, detachAggrArgs, FALSE,
	 "Detach an aggrgate."},
    {"getAggrInfo", GetAggrInfo, NULL, getAggrInfoArgs, FALSE,
	 "Get information about an attached agggregate"},
    {"getRawDeviceName", GetRawDeviceName, NULL, getRawDeviceNameArgs, FALSE,
	 "Get name of raw device associated with a given block device"},
    {"lockAggrDevice", LockAggrDevice, NULL, lockAggrDeviceArgs, FALSE,
	 "Lock an aggregate's raw device"},
    {"unlockAggrDevice", UnlockAggrDevice, NULL, unlockAggrDeviceArgs, FALSE,
	 "Unlock an aggregate's raw device"},
    {NULL}
};


EXPORT long
itlAggr_DefineFtutilErrorCodes()
{
    return itlu_ExtendKnownErrorCodes(ftutilErrorCodes);
}	/* itlu_DefineSystemErrors() */


EXPORT long
itlAggr_Init()
{
    long		code;
    unsigned		numEntries;
    itl_commandTable_t*	stdCmdTable;
    boolean_t		success;

    if (code = itlAggr_DefineFtutilErrorCodes())
	return code;

    code = itlu_Cnvt2StdCmdTbl(commandTable, COMMAND_MODE_ALL, "aggr",
			       &stdCmdTable, &numEntries);
    if (code)
	return code;
			    
    success = itl_AddModuleCmds("aggr", stdCmdTable, numEntries,
				COMMAND_MODE_ALL, infoMsg);
    if (!success)
	return ITLU_UNKNOWN_ERROR;

    return 0;
}	/* itlAggr_Init() */


PRIVATE long
InitAggr(rock, args)
    opaque	rock;
    opaque*	args;
{
    char*	argv[6];

    argv[0] = NEWAGGR;
    argv[1] = args[0];			/* Device name */
    argv[2] = args[1];			/* Block size */
    argv[3] = args[2];			/* Fragment size */
    argv[4] = NEWAGGR_OVERWRITE_FLAG;
    argv[5] = 0;

    return itlu_System(NEWAGGR, argv);
}	/* InitAggr() */


PRIVATE long
VerifyAggr(rock, args)
    opaque	rock;
    opaque*	args;
{
    char*	argv[4];

    argv[0] = SALVAGER;
    argv[1] = SALVAGER_VERIFY_FLAG;
    argv[2] = args[0];			/* Device name */
    argv[3] = 0;

    return itlu_System(SALVAGER, argv);
}	/* VerifyAggr() */


PRIVATE long
RecoverAggr(rock, args)
    opaque	rock;
    opaque*	args;
{
    char*	argv[4];

    argv[0] = SALVAGER;
    argv[1] = SALVAGER_RECOVER_FLAG;
    argv[2] = args[0];			/* Device name */
    argv[3] = 0;

    return itlu_System(SALVAGER, argv);
}	/* RecoverAggr() */


PRIVATE long
SalvageAggr(rock, args)
    opaque	rock;
    opaque*	args;
{
    char*	argv[4];

    argv[0] = SALVAGER;
    argv[1] = args[0];			/* Device name */
    argv[2] = 0;

    return itlu_System(SALVAGER, argv);
}	/* SalvageAggr() */


PRIVATE long
AttachAggr(rock, args)
    opaque	rock;
    opaque*	args;
{
    long	code;

    code = ftu_AttachAggr(args[0], args[1], args[2], (ftu_aggrId_t)args[3],
			  FTU_ATTACH_FLAGS_MOUNTAGGR, 0);

    return code;
}	/* AttachAggr() */


PRIVATE long
DetachAggr(rock, args)
    opaque	rock;
    opaque*	args;
{
    long	code;

    code = ftu_DetachAggr((ftu_aggrId_t)args[0], FTU_ATTACH_FLAGS_MOUNTAGGR);

    return code;
}	/* DetachAggr() */


PRIVATE long
GetAggrInfo(rock, args)
    opaque	rock;
    opaque*	args;
{
    struct aggrInfo	aggrInfo;
    long		code;
    char		nameBuf[1024];

    if (code = ftu_AggrGetInfo((ftu_aggrId_t)args[0], &aggrInfo))
	return code;

    strcpy(nameBuf, args[1]);
    strcat(nameBuf, "_name");
    (void)itl_SetStrVariable(nameBuf, aggrInfo.name, var_same);

    strcpy(nameBuf, args[1]);
    strcat(nameBuf, "_devName");
    (void)itl_SetStrVariable(nameBuf, aggrInfo.devName, var_same);

    strcpy(nameBuf, args[1]);
    strcat(nameBuf, "_type");
    (void)itl_SetIntVariable(nameBuf, aggrInfo.type, var_same);

    strcpy(nameBuf, args[1]);
    strcat(nameBuf, "_totalUsable");
    (void)itl_SetIntVariable(nameBuf, aggrInfo.totalUsable, var_same);

    strcpy(nameBuf, args[1]);
    strcat(nameBuf, "_free");
    (void)itl_SetIntVariable(nameBuf, aggrInfo.realFree, var_same);
    
    strcpy(nameBuf, args[1]);
    strcat(nameBuf, "_minFree");
    (void)itl_SetIntVariable(nameBuf, aggrInfo.minFree, var_same);

    return 0;
}	/* GetAggrInfo() */


PRIVATE long
GetRawDeviceName(rock, args)
    opaque	rock;
    opaque*	args;
{
    long	code;
    char	rawDevName[MAXPATHLEN+1];

    if (code = ftu_GetRawDeviceName(args[0], rawDevName))
	return code;

    (void)itl_SetStrVariable(args[1], rawDevName, var_same);

    return 0;
}	/* UnlockAggrDevice() */


PRIVATE long
LockAggrDevice(rock, args)
    opaque	rock;
    opaque*	args;
{
    long		code;
    ftu_devLock_t	devLock;

    if (code = ftu_LockAggrDevice(args[0], &devLock))
	return code;

    (void)itl_SetIntVariable(args[1], devLock, var_same);

    return 0;
}	/* LockAggrDevice() */


PRIVATE long
UnlockAggrDevice(rock, args)
    opaque	rock;
    opaque*	args;
{
    long	code;

    code = ftu_UnlockAggrDevice((ftu_devLock_t)args[0]);

    return code;
}	/* UnlockAggrDevice() */
