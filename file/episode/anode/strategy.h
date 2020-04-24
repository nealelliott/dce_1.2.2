/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1994, 1993 Transarc Corporation - All rights reserved */

/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RCS/strategy.h,v 1.11 1994/11/02 19:54:48 cfe Exp $ */

#ifndef TRANSARC_EPISODE_STRATEGY_H
#define TRANSARC_EPISODE_STRATEGY_H
/*
 * Definitions and interfaces shared between top-level and OS-specific
 * strategy code.
 */

#include <dcedfs/debug.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_buf.h>
#include <dcedfs/osi_user.h>
#include <dcedfs/osi_vmm.h>
#include <anode.h>
#include <fixed_anode.h>

#include <strategy_mach.h>

/*
 * Check whether next block address is contiguous with current one.
 * Assumes current block is non-empty.
 */
#define EPIA_CONTIGUOUS(curr, next) \
    (!epix_IsBlockEmpty(next) && (next) == (curr) + 1)

extern void epia_FinishBufIO(struct buf *bp, long code, int unmap);

#endif /* !TRANSARC_EPISODE_STRATEGY_H */
