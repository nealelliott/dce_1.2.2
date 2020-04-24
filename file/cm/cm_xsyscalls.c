/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1989, 1995 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>		/* Should be always first */
#include <dcedfs/stds.h>
#include <cm.h>				/* Cm-based standard headers */
#ifdef AFS_OSF_ENV
#include <kern/parallel.h>
#endif
#include <cm_cell.h>
#include <cm_conn.h>
#include <dcedfs/osi_cred.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/cm/RCS/cm_xsyscalls.c,v 4.50 1995/11/14 21:16:38 cfe Exp $")

/*
 * A system call to inform the CM that a new ticket-granted ticket (tgt) is
 * granted. This is called by security service routines when a user runs
 * either dce_login or kinit.
 */
long afscall_cm_newtgt(
  long uPag,		/* input: specify a particular login session */
  long uLifeTime,	/* input: new tgt's expiration time  */
  long unused3,
  long unused4,
  long unused5,
  int *rval)
{
    unsigned32 pag = (unsigned32)uPag;
    time_t lifeTime = (time_t)uLifeTime;
    struct cm_tgt *tgtp;
    struct icl_log *logp;
    long code;
    unsigned32 mypag;

    /* first initialize the ICL system */
    if (cm_iclSetp == NULL) {
	cm_BasicInit();
    }

    icl_Trace2(cm_iclSetp, CM_TRACE_GETNEWTGT_CALL,
	       ICL_TYPE_LONG, pag,
	       ICL_TYPE_LONG, lifeTime);
    /*
     * If lifeTime is 0, we will stop any attempt to authenticate from this pag
     */
    if (lifeTime != 0 && lifeTime < osi_Time()) {
	icl_Trace0(cm_iclSetp, CM_TRACE_GETNEWTGT_CALL_OLD);
	return EINVAL;
    }

    mypag = osi_GetPagFromCred(osi_getucred());
    if (pag == 0)
	pag = mypag;
    else
	/* Check if this process has right to do this */
	if (mypag != pag && !osi_suser(osi_getucred())) {
	    icl_Trace2(cm_iclSetp, CM_TRACE_GETNEWTGT_ACCESS,
		       ICL_TYPE_LONG, pag,
		       ICL_TYPE_LONG, mypag);
	    return EACCES;
	}

    lock_ObtainWrite(&cm_tgtlock);
    for (tgtp = cm_tgtList; tgtp != NULL; tgtp = tgtp->next) {
         if (tgtp->pag == pag)
	     break;
    }
    if (tgtp == NULL) {
        if (!FreeTGTList)
	    tgtp = (struct cm_tgt *) osi_Alloc(sizeof (struct cm_tgt));
	else {
	    tgtp = FreeTGTList;
	    FreeTGTList = tgtp->next;
	}
	bzero((char *) tgtp, sizeof (struct cm_tgt));
	tgtp->next = cm_tgtList;
	cm_tgtList = tgtp;
	tgtp->pag = pag;
	icl_Trace2(cm_iclSetp, CM_TRACE_GETNEWTGT, ICL_TYPE_LONG, pag,
		   ICL_TYPE_LONG, lifeTime);
    }
    tgtp->tgtTime = lifeTime;
    if (lifeTime != 0)
	tgtp->tgtFlags &= ~CM_TGT_EXPIRED;
    lock_ReleaseWrite(&cm_tgtlock);
    /*
     * NOTE: This tgt should not be GCed if 'lifeTime' is still in the future.
     *
     * Reset all rpc bindings associated with this pag, if any
     */
    cm_ResetUserConns(pag);
    return 0;
}
