/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * This file defines the structure that is passed around as a verification rock. */

/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RCS/verify.h,v 4.14 1994/11/02 20:01:50 cfe Exp $ */

/* Copyright (C) 1993, 1990 Transarc Corporation - All rights reserved */

#ifndef TRANSARC_EPI_VERIFY_H
#define TRANSARC_EPI_VERIFY_H

extern int afsdb_episode_anode_verify;
#define DBVprint(p) dmprintf (afsdb_episode_anode_verify, DEBUG_THIS_FILE, p)

struct epiz_SimBits {
    unsigned char *bits;		/* simulated bitmap */
    u_long bitsLen;			/* length of same */
    unsigned char *logged;		/* simulated logged block map */
    unsigned char *fraged;		/* blocks occupied by fraged anodes */
    u_long loggedLen;			/* length of above two */
    u_long blkSize;			/* this aggregate's block size */
    u_int logBlkSize;			/* log-base-2 of blkSize */
    u_long fragSize;			/* this aggregate's fragment size */
    u_int logFragSize;			/* log-base-2 of fragSize */
    u_int fragsPerBlock;		/* fragments per block */
    daddr_t bitmapOffset;		/* block addr of first bitmap block */
};

#ifndef KERNEL
/* Shared procedures */

#include <dcedfs/episode/block_alloc.h>
#include <salvage.h>

extern long epib_VerifyBitmap( 
  epi_anode_t aggrAnode,
  struct epiz_salvageState *ss,
  epi_anode_t bitmapAnode,
  epi_bitmap_t epiBits);
#endif /* !KERNEL */

#endif /* TRANSARC_EPI_VERIFY_H */
