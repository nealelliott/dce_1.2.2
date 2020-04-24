/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: trace.c,v $
 * Revision 1.1.58.1  1996/10/02  17:42:16  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:34:56  damon]
 *
 * Revision 1.1.53.2  1994/06/09  14:04:24  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:39:08  annie]
 * 
 * Revision 1.1.53.1  1994/02/04  20:15:49  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:11:35  devsrc]
 * 
 * Revision 1.1.51.1  1993/12/07  17:21:44  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:01:56  jaffe]
 * 
 * Revision 1.1.4.4  1993/01/21  18:25:00  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  16:45:41  zeliff]
 * 
 * Revision 1.1.4.3  1993/01/13  16:57:32  shl
 * 	Transarc delta: jaffe-cleanup-2.3-compiler-warnings 1.8
 * 	  Selected comments:
 * 	    Fix compiler warnings found when moving to OSF/1 1.1.1.
 * 	    remove extra ; after RCSID.
 * 	    Remove the function dacl_SetOwnerControl.  It is not used.
 * 	[1993/01/12  20:25:00  shl]
 * 
 * Revision 1.1.4.2  1992/11/24  16:57:58  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  17:52:02  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/05  16:37:43  jdp
 * 	delta jdp-dynamic-tracing 1.1
 * 	[1992/05/04  19:01:53  jdp]
 * 
 * $EndLog$
 */
/* Copyright (C) 1992 Transarc Corporation - All rights reserved */

/*
 * This file provides an interface that allows tracing variables to be
 * manipulated at runtime via epistat.
 */

#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/episode/instrument.h>

RCSID("$Id: trace.c,v 1.1.58.1 1996/10/02 17:42:16 damon Exp $")

IMPORT int afsdb_episode_anode;
IMPORT int afsdb_episode_async;
IMPORT int afsdb_episode_logSleepCnt;
IMPORT int afsdb_episode_logbuf;
IMPORT int afsdb_episode_recover;
IMPORT int afsdb_episode_vnops;

/*
 * Convert epistat variable index to address of tracing variable.
 */
PRIVATE int* numToAddr[] = {
    &afsdb_episode_anode,			/* 0 */
    &afsdb_episode_async,			/* 1 */
    &afsdb_episode_logSleepCnt,			/* 2 */
    &afsdb_episode_logbuf,			/* 3 */
    &afsdb_episode_recover,			/* 4 */
    &afsdb_episode_vnops,			/* 5 */
    &afsdb_osi					/* 6 */
};
PRIVATE int numToAddrSize = sizeof numToAddr / sizeof numToAddr[0];

/*
 * hook for implementing monitoring and tuning specific to this submodule.
 */
SHARED int ins_trace(opcode, vars, argObjIds, resultObjIds, resultValues)
  IN struct ins_opDesc *opcode;
  IN struct ins_varDesc *vars;
  IN struct ins_basicObjectId *argObjIds;
  OUT struct ins_basicObjectId *resultObjIds;
  OUT struct ins_basicObject *resultValues;
{
    int *dataAddr = 0;

    if (vars->varIdx >= numToAddrSize)
	return EINVAL;

    dataAddr = numToAddr[vars->varIdx];

    switch (opcode->opcode) {
      case INS_SET:
	*dataAddr = *(int*)resultValues->value;
	break;
      case INS_GET:
	*(int*)resultValues->value = *dataAddr;
	break;
      case INS_GETNEXT:
	/* not implemented */
	break;
      default:
	return EINVAL;
	break;
    }

    return 0;
}
