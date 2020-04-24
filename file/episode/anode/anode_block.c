/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* anode_block.c -- This file contains procedures which manage pages of anodes.
 *     These pages are assembled into volume tables (for the AVL and for all
 *     volumes ). */

/* Copyright (C) 1995, 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/osi.h>
#include <dcedfs/episode/logbuf.h>
#include "anode.h"
#include "anode_block.h"

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RCS/anode_block.c,v 4.24 1995/12/05 13:49:01 ota Exp $")

/* EXPORT */
u_int epic_anodesPerBlock[] = {
    epic_AnodesPerBlock(1024<<0),
    epic_AnodesPerBlock(1024<<1),
    epic_AnodesPerBlock(1024<<2),
    epic_AnodesPerBlock(1024<<3),
    epic_AnodesPerBlock(1024<<4),
    epic_AnodesPerBlock(1024<<5),
    epic_AnodesPerBlock(1024<<6),
};

struct anodeBlock {
    int32 initialStamp;			/* set for every page write */
    int32 magic;			/* ANODE_BLOCK_*_MAGIC */
    u_int32 volIndex;			/* index of volume table in avl */
    char anode[/*at least*/4][ANODE_SIZE];
    int32 finalStamp;			/* same as initialStamp */
};

/* AnodeBlockType -- Returns the magic number associated with the anode block
 *     contained in the buffer \arg{abb}, which identifies its type.  This is
 *     the value that was specified in the call to \fcn{epic_FormatAnodeBlock}.
 */

/* SHARED */
long epic_AnodeBlockType(IN struct buffer *abb)
{
    long magic;
    magic = ((struct anodeBlock *)(elbb_BufferData(abb)))->magic;
    return magic;
}

/* GetVolIndex -- Returns the index of an anode block contained in the
 *     \argname{abb} buffer.  This is the value that was specified in the call
 *     to \fcname{epiv_FormatAnodeBlock}. */

/* SHARED */
u_long epic_GetVolIndex(IN struct buffer *abb)
{
    u_long index;
    index = ((struct anodeBlock *)(elbb_BufferData(abb)))->volIndex;
    return index;
}

/* AnodeBlockOkay -- Returns a boolean value which is \cst{TRUE} if the magic
 *     number in the anode block contained in the buffer \arg{abb} is legal.
 *     If \arg{magic} is non-zero then it specifies which of the two anode
 *     block magic number to expect: \cst{ANODE_BLOCK_AVL_MAGIC} or
 *     \cst{ANODE_BLOCK_VOLUME_MAGIC}, otherwise either is acceptable.  If
 *     \arg{volIndex} is non-zero it must match the volIndex field. */

/* SHARED */
int epic_AnodeBlockOkay(
  struct buffer *abb,
  long magic,
  u_long volIndex)
{
    struct async_device *dev = elbb_BufferDevice(abb);
    struct anodeBlock *ab = (struct anodeBlock *)elbb_BufferData(abb);
    int32 final;			/* final timestamp */

    afsl_Assert(offsetof(struct anodeBlock, anode[0][0])
	    == ANODE_BLOCK_HEADERSIZE);
    if (magic) {
	if (ab->magic != magic) return 0;
    } else {
	if ((ab->magic != ANODE_BLOCK_AVL_MAGIC) &&
	    (ab->magic != ANODE_BLOCK_VOLUME_MAGIC)) return 0;
    }
    if (volIndex && (ab->volIndex != volIndex)) return 0;
    final = *((int32 *)((char *)ab + dev->blkSize - sizeof(int32)));
    return (ab->initialStamp == final);
}

/* epic_FormatAnodeBlock -- Makes the block contained in the buffer \arg{bP} as
 *     an anode block.  The parameter \arg{index} specified the epia-style
 *     index of the fileset anode within the AVL or the index of the AVL anode
 *     itself if this is a block of the AVL.  The \arg{magic} is one of
 *     \cst{ANODE_BLOCK_AVL_MAGIC} or \cst{ANODE_BLOCK_VOLUME_MAGIC} which
 *     specifies whether the block is part of the AVL or of a fileset. */

/* SHARED */
long epic_FormatAnodeBlock(
  IN elbb_tranRec_t trans,
  IN struct buffer **bP,
  IN u_int32 index,
  IN int32 magic)
{
    long o;				/* offset in anodeBlock */
    int32 stamp = osi_Time();
    int32 bsize = elbb_BufferDevice (*bP)->blkSize;
    int log;
    int32 aflags = 0;			/* mark as totally free */
    long code;

    log = !elbb_IsZeroTrans(trans);

#define CallModify(offset,addr,size) \
    if ((code = elbb_Modify (*bP, offset, (caddr_t)addr, size,\
			     trans, log, bP)) != 0) \
        return code

    CallModify (offsetof (struct anodeBlock, volIndex),
		&index, sizeof(index));
    CallModify (offsetof (struct anodeBlock, magic),
		&magic, sizeof(magic));

    CallModify (offsetof (struct anodeBlock, initialStamp),
		&stamp, sizeof(int32));
    CallModify (bsize-sizeof(int32)/*finalStamp*/, &stamp, sizeof(int32));

    /* now initialize all the anode flags words to zero */
    for (o = epic_FirstOffset(); epic_LegalOffset(o, bsize);
	 o = epic_NextOffset(o)) {
	CallModify (o, &aflags, sizeof(aflags));
    }

    return 0;
}

/* epic_HowMuchLikeAnodeBlock -- Determines how much the block pointed to by
 *     \argname{dataptr} is like an anode block.  The integer at \argname{valP}
 *     is set to a value in the range [0,10000].  The meaning of this value is
 *     explained in the description of the function
 *     \fcname{epig_SayWhyNotLikeSuperBlock}.  The anode block's magic number
 *     is assigned into \argname{sigP} and a pointer to the first anode is
 *     assigned into \argname{firstP}.  The return value is always zero. */

/* EXPORT */
long epic_HowMuchLikeAnodeBlock(
  IN struct async_device *dev,
  IN opaque dataptr,
  OUT int *valP,
  OUT int32 *sigP,
  OUT opaque *firstP)
{
    struct anodeBlock *ab = (struct anodeBlock *) dataptr;
    int score;

    *sigP = ab->magic;			/* return MAGIC unconditionally */
    if ((ab->magic != ANODE_BLOCK_AVL_MAGIC) &&
	(ab->magic != ANODE_BLOCK_VOLUME_MAGIC)) {
	*valP = 0;
    } else {
	score = 3000;
	if (dev != NULL) {
	    if (ab->initialStamp == * ((int32 *) (((char *) dataptr) + dev->blkSize - sizeof(int32)))) {
		score += 1000;
	    }
	}
	if (ab->magic == ANODE_BLOCK_AVL_MAGIC) {
	    if (ab->volIndex < 15) score += 3000;
	} else {
	    if (ab->volIndex < 20000) score += 3000;
	}
	*valP = score;
	*firstP = &(ab->anode[0][0]);
    }
    return 0;
}

#ifndef KERNEL
#ifdef SCAN_SALVAGER
/* EXPORT */
long epic_HowMuchLikeAnodeBlock(
  struct async_device *dev,
  opaque dataptr,
  int *valP,
  int32 *sigP,
  opaque *firstP)
{/* Return how much the given block is like an AnodeBlock, in [0,10000], with signature via sigP. */
    struct anodeBlock *ab = (struct anodeBlock *) dataptr;
    int score;

    if (ab->magic != ANODE_BLOCK_AVL_MAGIC && ab->magic != ANODE_BLOCK_VOLUME_MAGIC) {
	*valP = 0;
    } else {
	score = 0;
	if (ab->initialStamp == * ((int32 *) (((char *) dataptr) + dev->blkSize - sizeof(int32)))) {
	    score += 500;
	}
	if (ab->magic == ANODE_BLOCK_AVL_MAGIC) {
	    if (ab->volIndex < 15) score += 300;
	} else {
	    if (ab->volIndex < 20000) score += 300;
	}
	*valP = score;
	*sigP = ab->magic;
	*firstP = &(ab->anode[0][0]);
    }
    return 0;
}

/* Procedure for guessing whether a given block looks like a page of anodes. */
/* EXPORT */
long epic_BlockScore(
  struct epiz_ReconState *recon,
  u_long ix,
  opaque data,
  long *typeP,
  long *scoreP,
  int Recurse)
{/* evaluate whether the given block looks like a block of anodes. */
    long score, type, code;
    long subScore, subType;
    struct anodeBlock *AB = (struct anodeBlock *) data;

    score = 0;
/* Magic number should agree. */
    if (AB->magic == ANODE_BLOCK_AVL_MAGIC) {
	score += 100;
	if (AB->volIndex <= 0) {
	    score -= 100;
	} else if (AB->volIndex < 15) {
	    score += 80;
	} else {
	    score -= 50;
	}
    } else if (AB->magic == ANODE_BLOCK_VOLUME_MAGIC) {
	score += 100;
	if (AB->volIndex <= 0) {
	    score -= 100;
	} else if (AB->volIndex < 20000) {
	    score += 80;
	} else {
	    score -= 50;
	}
    } else score -= 100;
/* Timestamps should agree, but don't 100% have to. */
    if (AB->initialStamp == *(int32 *)((char *)(page) - sizeof(int32) + recon->Dev->blkSize))
	score += 50;
    else
	score -= 50;
    score = EvidenceConst(score, (100+80+50));
    type = TYPEANODEBLOCK | LISTTYPEUNK | SECTIONUNK;
    if (score >= EvidenceConst(1,2) && (AB->magic == ANODE_BLOCK_AVL_MAGIC || AB->magic == ANODE_BLOCK_VOLUME_MAGIC)) {
	if (AB->magic == ANODE_BLOCK_AVL_MAGIC) {
	    code = epig_BlockScore(recon, ix, data, &subType, &subScore, Recurse);
	} else {
	    code = epit_BlockScore(recon, ix, data, &subType, &subScore, Recurse);
	}
	if (code != 0) return code;
	if (subScore > EvidenceConst(1,3)) {
	    score = subScore;
	    type = subType;
	}
    }
    *scoreP = score;
    *typeP = type;
    if (score > EvidenceConst(1,4) && Recurse) {
	recon->State[ix].NativeGuess = type;
	recon->State[ix].NativeEvidence = score + EVIDENCE_OFFSET;
    }
    return 0;
}
#endif /* SCAN_SALVAGER */
#endif /* not KERNEL */
