/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1992 Transarc Corporation - All rights reserved */
/*
 * This file contains stubs for zlc functions and variable that are called 
 * from the xvolume package so that stand-alone episode, which includes
 * libvolume, can include this file and properly link sdcelfs.ext.
 * Obviously, since stand-alone episode doesn't use tokens, it doesn't
 * require the zlc functionality from this package.
 */
#include <dcedfs/zlc.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/zlc/RCS/zlc_sepistubs.c,v 1.10 1996/03/04 19:19:23 ota Exp $")

/* 
 * zlc.h needs to be included to set zlc_recoveryState and since zlc.h
 * provides prototypes for the following functions, need to specify
 * the arguments with types here. 
 */

void zlc_TryRemove(struct vnode *avp, afsFid *afidp)
{
    /* null */
}

void zlc_CleanVolume(afs_hyper_t *volIDp) 
{
    /* null */
}

void zlc_SetRestartState(unsigned long expire) 
{
    /* null */
}

int zlc_recoveryState = ZLC_REC_UNKNOWN;
