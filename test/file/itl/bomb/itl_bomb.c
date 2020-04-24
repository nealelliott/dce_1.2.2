/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_bomb.c,v $
 * Revision 1.1.9.1  1996/10/17  18:25:05  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:39  damon]
 *
 * Revision 1.1.4.1  1994/02/04  20:43:06  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:23:30  devsrc]
 * 
 * Revision 1.1.2.2  1994/01/20  18:43:54  annie
 * 	added copyright header
 * 	[1994/01/20  18:40:12  annie]
 * 
 * Revision 1.1.2.1  1993/12/09  21:20:45  jaffe
 * 	Include itl_bomb library for fset tests
 * 	[1993/12/09  21:16:36  jaffe]
 * 
 * $EndLog$
 */

/*
 * (C) Copyright 1993 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

#include <pthread.h>
#include <dcedfs/bomb.h>

#include <utils/itl/itl_utils.h>

#include "itl_bomb.h"

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/bomb/itl_bomb.c,v 1.1.9.1 1996/10/17 18:25:05 damon Exp $")


static char* infoMsg = "The bomb module provides command interfaces to \
bomb point related functions.\n";


/*
 * Interface routines 
 */
static long SetBomb(opaque rock, opaque argv[]);
static long SetKernelBomb(opaque rock, opaque argv[]);
static long UnsetBomb(opaque rock, opaque argv[]);
static long UnsetKernelBomb(opaque rock, opaque argv[]);

/* SetBomb args */
static itlu_argDesc_t setBombArgs[] = {
    {"bpDesc", "Bomb Point Description: ", itlu_GetString},
    {NULL}
};

/* UnsetBomb args */
static itlu_argDesc_t unsetBombArgs[] = {
    {"bpName", "Bomb Point Name: ", itlu_GetString},
    {NULL}
};

static itlu_commandTable_t commandTable[] = {
    {"setBomb", SetBomb, NULL, setBombArgs, FALSE,
	 "Set a bomb point."},
    {"setKernelBomb", SetKernelBomb, NULL, setBombArgs, FALSE,
	 "Set a kernel bomb point."},
    {"unsetBomb", UnsetBomb, NULL, unsetBombArgs, FALSE,
	 "Unset a bomb point."},
    {"unsetKernelBomb", UnsetKernelBomb, NULL, unsetBombArgs, FALSE,
	 "Unset a kernel bomb point."},
    {NULL}
};

PRIVATE itlu_valueMap_t bombErrorCodes[] = {
    {"BOMB_E_MALFORMED_BOMB_POINT", BOMB_E_MALFORMED_BOMB_POINT},
    {"BOMB_E_BOMB_POINT_NOT_SET", BOMB_E_BOMB_POINT_NOT_SET},
    {"BOMB_E_NAME_TOO_LONG", BOMB_E_NAME_TOO_LONG},
    {"BOMB_E_BAD_EXPLOSION_TYPE", BOMB_E_BAD_EXPLOSION_TYPE},
    {"BOMB_E_BAD_TRIGGER_TYPE", BOMB_E_BAD_TRIGGER_TYPE},
    {NULL}
};

extern long
itlBomb_Init(void)
{
    long		code;
    unsigned		numEntries;
    itl_commandTable_t*	stdCmdTable;
    boolean_t		success;

    code = itlu_Cnvt2StdCmdTbl(commandTable, COMMAND_MODE_ALL, "bomb",
			       &stdCmdTable, &numEntries);
    if (code)
	return code;
			    
    success = itl_AddModuleCmds("bomb", stdCmdTable, numEntries,
				COMMAND_MODE_ALL, infoMsg);
    if (!success)
	return ITLU_UNKNOWN_ERROR;

    if (code = itlu_ExtendKnownErrorCodes(bombErrorCodes))
	return code;

    return 0;
}	/* itlBomb_Init() */


static long
SetBomb(rock, args)
    opaque	rock;
    opaque*	args;
{
    return bomb_SetDesc(args[0]);
}	/* SetBomb() */

static long
SetKernelBomb(rock, args)
    opaque	rock;
    opaque*	args;
{
    return bomb_KernelSetDesc(args[0]);
}	/* SetKernelBomb() */

static long
UnsetBomb(rock, args)
    opaque	rock;
    opaque*	args;
{
    return bomb_Unset(args[0]);
}	/* UnsetBomb() */

static long
UnsetKernelBomb(rock, args)
    opaque	rock;
    opaque*	args;
{
    return bomb_KernelUnset(args[0]);
}	/* UnsetKernelBomb() */
