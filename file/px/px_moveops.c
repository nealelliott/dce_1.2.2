/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: px_moveops.c,v $
 * Revision 1.1.11.1  1996/10/02  18:13:03  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:45:38  damon]
 *
 * $EndLog$
 */
/*
 *      Copyright (C) 1996, 1993 Transarc Corporation
 *      All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/afs4int.h>
#include <dcedfs/basicincludes.h>
#include <dcedfs/fshs.h>
#include <dcedfs/aggr.h>
#include <dcedfs/volume.h>
#include <dcedfs/xcred.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/tkset.h>
#include <px.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/px/px_moveops.c,v 1.1.11.1 1996/10/02 18:13:03 damon Exp $")

#define	VOLCLEAN_DESTROY    0x0
#define	VOLCLEAN_TOKEN_DESTROY    0x1
#define	VOLCLEAN_SRC_FLDB    0x2
#define	VOLCLEAN_TGT_FLDB    0x3

struct volCleanup {
    struct volCleanup *next;
    afs_hyper_t volId;
    unsigned long cleanState;
    unsigned long startTime;
};

static int vc_inited = 0;
static int numVCs;
static struct lock_data vc_lock;
static struct volCleanup *VCs;

static tkm_tokenID_t openPreserve, openExecute, openAll;

#define	BASIC_RHYTHM	  5000	/* how often we get called */
#define	BASIC_RHYTHM_HORIZON	(2*BASIC_RHYTHM + 20) /* how far ahead to look */

/* 
 * Here are the main work for some of the replication-related RPC calls. 
 */

static long vc_Action(volp, actionh)
struct volume *volp;
int actionh;
{/* Called from the xvolume layer. */
    return 0;
}

static void vc_ckInit()
{
    lock_Init(&vc_lock);
    vol_SetActionProc(vc_Action);
    vc_inited = 1;
}

#define	CHECK_INIT  {if (vc_inited == 0) vc_ckInit();}

/* Called periodically from the PX daemon loop. */
long pxvc_Cleanups()
{
    long interv;

    icl_Trace0(px_iclSetp, PX_TRACE_PVC);
    CHECK_INIT;
    return BASIC_RHYTHM;
}
