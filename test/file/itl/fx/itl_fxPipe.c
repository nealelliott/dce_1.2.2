/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (C) Copyright 1991 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_fxPipe.c -- 
 *    Routines necessary to implement the RPC (IDL) pipe data type.
 *    Basically these routines just move data to/from a user data area
 *    (most likely an ITL variable) to/from an RPC buffer.
 *
 */
/*
 * HISTORY
 * $Log: itl_fxPipe.c,v $
 * Revision 1.1.321.1  1996/10/17  18:26:34  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:21  damon]
 *
 * Revision 1.1.316.1  1994/02/04  20:44:37  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:11  devsrc]
 * 
 * Revision 1.1.314.2  1994/01/20  18:44:13  annie
 * 	added copyright header
 * 	[1994/01/20  18:40:32  annie]
 * 
 * Revision 1.1.314.1  1993/12/07  17:44:12  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:20:53  jaffe]
 * 
 * $EndLog$
 */

#ifdef	SUNOS5
#include <sys/types.h>
#endif
#include <string.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_fxPipe.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxPipe.c,v 1.1.321.1 1996/10/17 18:26:34 damon Exp $")


SHARED void itlFX_PipeAlloc(stateP, requestedSize, newBufferPP, bufferSizeP)
  IN void *stateP;
  IN unsigned long requestedSize;
  IN unsigned char **newBufferPP;
  IN unsigned long *bufferSizeP;
{
    itlFX_pipeState_t *pipeStateP = (itlFX_pipeState_t *) stateP;

    assert(requestedSize <= pipeStateP->pipeSize);
    *newBufferPP = (unsigned char *) pipeStateP->pipeBuffer;
    *bufferSizeP = pipeStateP->pipeSize;

    return;
}

/*
 *  itlFX_PipePull() --
 *    This function is called when issuing the AFS_StoreData RPC.  It
 *    moves data out of the IO buffer into the pipe.
 */
SHARED void itlFX_PipePull(stateP, pipeBuffer, bufferSize, bytesPulledP)
  IN void *stateP;
  IN unsigned char *pipeBuffer;
  IN unsigned long bufferSize;
  IN unsigned long *bytesPulledP;
{
    itlFX_pipeState_t *pipeStateP = (itlFX_pipeState_t *) stateP;

    if (pipeStateP->ioOffset == pipeStateP->bufferSize)
	*bytesPulledP = 0;
    else
    {
	*bytesPulledP = ((pipeStateP->ioOffset + pipeStateP->pipeSize) > 
			 pipeStateP->bufferSize) ? 
			     pipeStateP->bufferSize - pipeStateP->ioOffset :
				 pipeStateP->pipeSize;

	(void) memcpy((void *) pipeBuffer, 
		      (void *) &(pipeStateP->ioBuffer[pipeStateP->ioOffset]),
		      *bytesPulledP);
	pipeStateP->ioOffset += *bytesPulledP;
	assert(pipeStateP->ioOffset <= pipeStateP->bufferSize);
    }

    return;
}

/*
 *  itlFX_PipePush() --
 *    This function is called when issuing the AFS_FetchData RPC.  It
 *    moves data out of the pipe and into the IO buffer.
 */
SHARED void itlFX_PipePush(stateP, pipeBuffer, bytesToPush)
  IN void *stateP;
  IN unsigned char *pipeBuffer;
  IN unsigned long bytesToPush;
{
    itlFX_pipeState_t *pipeStateP = (itlFX_pipeState_t *) stateP;

    if (bytesToPush)
    {
	assert((pipeStateP->ioOffset + bytesToPush) <= pipeStateP->bufferSize);

	(void) memcpy((void *) &(pipeStateP->ioBuffer[pipeStateP->ioOffset]),
		      (void *) pipeBuffer, bytesToPush);
	pipeStateP->ioOffset += bytesToPush;
	assert(pipeStateP->ioOffset <= pipeStateP->bufferSize);
    }

    return;
}
