/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_fxPipe.h -- 
 *    Declarations for functions to deal with RPC (IDL) pipes.  They
 *    are used by the DFS to transfer data to an from the file exporter.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxPipe.h,v 1.1.11.1 1996/10/17 18:26:35 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_fxPipe.h,v $
 * Revision 1.1.11.1  1996/10/17  18:26:35  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:22  damon]
 *
 * Revision 1.1.6.1  1994/02/04  20:44:39  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:11  devsrc]
 * 
 * Revision 1.1.4.1  1994/01/28  20:44:28  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:48  annie]
 * 
 * Revision 1.1.2.2  1993/01/13  21:12:45  shl
 * 	Transarc delta: fred-ot-6350-add-fx-tests 1.3
 * 	  Selected comments:
 * 	    To place the DFS fole exporter tests into the tree.
 * 	    Migrate the fx exerciser to the new fx interface (OT 6054)
 * 	    Fix compilation problems.
 * 	[1993/01/13  14:32:51  shl]
 * 
 * $EndLog$
 */

#ifndef ITL_FX_PIPE_H
#define ITL_FX_PIPE_H

#include <dcedfs/osi.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>

typedef struct {
    unsigned char *ioBuffer;     /* where data is being read/written to */
    unsigned char *pipeBuffer;   /* buffer used to transfer data to/from */
                                 /* the ioBuffer */
    unsigned long bufferSize;    /* size of the ioBuffer */
    unsigned long pipeSize;      /* size of the pipeBuffer */
    unsigned long ioOffset;      /* offset into ioBuffer */
} itlFX_pipeState_t;

/*
 *  ITL_FX_PIPE_BUF_ALLOC() --
 *    This macro allocates memory for the pipe buffer.
 *
 *  Arguments:
 *    _pStateP - pointer to a itlFX_pipeState_t
 */
#define ITL_FX_PIPE_BUF_ALLOC(_pStateP)                                       \
        MACRO_BEGIN                                                           \
          ITL_ALLOC((_pStateP)->pipeBuffer, unsigned char *, osi_BUFFERSIZE); \
          (_pStateP)->pipeSize = osi_BUFFERSIZE;                              \
        MACRO_END

/*
 *  ITL_FX_PIPE_BUF_FREE() --
 *    This macro free's the memory allocated for the pipe buffer via 
 *    the ITL_FX_PIPE_BUF_ALLOC.
 *
 *  Arguments:
 *    _pStateP - pointer to a itlFX_pipeState_t
 */
#define ITL_FX_PIPE_BUF_FREE(_pStateP)                                        \
        MACRO_BEGIN                                                           \
          ITL_FREE((_pStateP)->pipeBuffer);                                   \
          (_pStateP)->pipeSize = 0;                                           \
        MACRO_END

/*
 *  ITL_FX_PIPE_SET_IO_BUF()
 *    This macro just sets the ioBuffer field of a itlFX_pipeState_t.
 *
 *  Arguments:
 *    _pStateP - pointer to a itlFX_pipeState_t
 *    _ioP     - pointer to a data buffer for read/write operation
 *    _ioSize  - length of the buffer pointed to by _ioP
 */
#define ITL_FX_PIPE_SET_IO_BUF(_pStateP, _ioP, _ioSize)                       \
        MACRO_BEGIN                                                           \
          (_pStateP)->ioBuffer = (unsigned char *) (_ioP);                    \
          (_pStateP)->bufferSize = (_ioSize);                                 \
          (_pStateP)->ioOffset = 0;                                           \
        MACRO_END


extern void itlFX_PipePull _TAKES((
				   IN void *stateP,
				   IN unsigned char *pipeBuffer,
				   IN unsigned long bufferSize,
				   IN unsigned long *bytesPulledP
				   ));

extern void itlFX_PipePush _TAKES((
				   IN void *stateP,
				   IN unsigned char *pipeBuffer,
				   IN unsigned long bytesToPush
				   ));

extern void itlFX_PipeAlloc _TAKES((
				   IN void *stateP,
				   IN unsigned long requestedSize,
				   IN unsigned char **newBufferPP,
				   IN unsigned long *bufferSizeP
				    ));

#endif  /* ITL_FX_PIPE_H */
