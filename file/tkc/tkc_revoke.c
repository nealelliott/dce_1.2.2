/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tkc_revoke.c,v $
 * Revision 1.1.940.1  1996/10/02  21:01:30  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:48:11  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */

#include <tkc.h>
#include <dcedfs/osi_cred.h>
#include <dcedfs/osi_user.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/tkc/tkc_revoke.c,v 1.1.940.1 1996/10/02 21:01:30 damon Exp $")

/* This file contains the operating system independent code for handing
 * token revocation.
 */

/* wait for all active vnode operations to complete; done by waiting
 * for dataHolds to drop to zero.  Called with held, write-locked
 * tkc_vcache structure.
 */
int tkc_WaitForIdle(avc)
register struct tkc_vcache *avc; {
    while (1) {
	if (avc->dataHolds) {
	    avc->vstates |= TKC_DHWAITING;
	    osi_SleepW(avc, &avc->lock);
	    lock_ObtainWrite(&avc->lock);
	}
	else return 0;
    }
}

/* data tokens can be revoked at any time that we're not actually
 * executing a call that reads or writes the file's data.  In various
 * systems, we implicitly cache data in the VM system, and for those systems,
 * we have to get the data out of the VM system before returning the
 * appropriate token, or else processes on this machine will still be able
 * to access mapped in files w/o having the appropriate token.
 *
 * The model for the VM integration of the data tokens is this:
 * Before allowing a getpage (or strategy reading a page) to complete
 * we must lock the vcache entry and make sure that there's a read
 * data token for this file.  A revoke done after this time must sweep
 * the VM system of all pages, including any being loaded at that
 * time, and return them, the idea being that after a revoke completes,
 * there are no pages left in the VM system, since if there were, the
 * VM system would have had to call getpage, which would have gotten
 * a data token.
 */

#ifndef AFS_SUNOS5_ENV
/* we have a special version for Solaris 2.2 */

int tkc_RevokeDataToken(avc, at, atype)
register struct tkc_vcache *avc;
long atype;
register struct afsRevokeDesc *at; {
    register long code;

    /* just wait for people to finish with the data */
    code = tkc_WaitForIdle(avc);

    return code;
}
#endif /* AFS_SUNOS5_ENV */

/* status tokens can be revoked at any time that we're not actually
 * executing a call that is reading or modifying the file's status.
 * Since we don't cache any status information in the glue layer, we
 * don't have to worry about invalidating or writing through any cached
 * information.  Thus, we just wait for everyone to complete their vnode
 * operations, and return.
 *
 * Called with vcache write-locked.
 */
int tkc_RevokeStatusToken(avc, at, atype)
register struct tkc_vcache *avc;
long atype;
register struct afsRevokeDesc *at; {
    register long code;
    code = tkc_WaitForIdle(avc);
    return code;
}


/* Open tokens are cached like other tokens.  They can be revoked as long
 * as no one is using them, that is, as long as there are no files open
 * in the mode guaranteed by the particular open token.
 */
int tkc_RevokeOpenToken(avc, at, atype)
register struct tkc_vcache *avc;
long atype;
register struct afsRevokeDesc *at; {
    register long code;

    /* if being asked to revoke a read token, check if we have any files open
     * for reading.
     */
    if (atype & TKN_OPEN_READ)
	if (avc->readOpens) return -1;	/* can't revoke this token */

    if (atype & TKN_OPEN_SHARED)
	if (avc->execOpens) return -1;	/* can't revoke this token */

    /* if being asked to revoke a write token, check if we have any files open
     * for writing.
     */
    if (atype & TKN_OPEN_WRITE)
	if (avc->writeOpens) return -1;	/* can't revoke this token */

    /* otherwise, things are fine */
    return 0;
}


/*
 * Ordinarily a lock token corresponds to an active lock, and is not
 * revocable.  However, sometimes a process gets a lock token, and then fails
 * to get the lock, because some other process on the same machine has a lock
 * on at least part of the byte range.  When this happens the process that
 * failed to get the lock doesn't try to give back the token; this would be
 * very complicated bcause existing locks, including locks held by the same
 * process, might be using subsets of the byte range.  Instead, we do nothing.
 * This leaves us holding a token in which some subsets of the byte range may
 * not correspond to active locks.  We have to b able to respond to revoke
 * requests for these subsets.
 */
int tkc_RevokeLockToken(avc, at, atype, tlp)
     register struct tkc_vcache *avc;
     register struct afsRevokeDesc *at;
     long atype;
     struct tkc_tokenList *tlp;
{
     int code;
     struct flock flock;
     short oldCounter;

    /*
     * if this is an optimistically granted lock token, we can return
     * it, since we don't care about these; we will also have an explicit
     * lock token if we have a real lock that we need to cover.
     */
    if (!(tlp->states & TKC_LLOCK)) return 0;

    /*
     * The handling of F_SETLK and F_SETLKW are quite different.
     *
     * If someone is trying to obtain a lock via F_SETLK, we wait right here
     * for them to finish.  We detect that they are finished by inspecting
     * the TKC_PARTLOCK bit in the vcache states word (see below).
     *
     * If someone is trying to obtain a lock via F_SETLKW, we don't wait here,
     * because that would cause deadlock.  Instead we return -1 indicating
     * that the lock is held.  This is not quite true (they themselves may
     * be waiting for the lock), but it is close enough:  they will eventually
     * get the lock and later, they will explicitly release it.
     *
     * Possible problem if we return -1, and then the guy gets ^C'ed (and so
     * never explicitly returns token)?
     */
    if (tlp->states & TKC_LBLOCK)
	goto noRevoke;

    /* Ask the file system if there are active locks. */
    flock.l_type = (atype == TKN_LOCK_READ ? F_RDLCK : F_WRLCK);
    flock.l_whence = 0;
    flock.l_start = AFS_hgetlo(tlp->token.beginRange);
    flock.l_len = AFS_hgetlo(tlp->token.endRange) -
	AFS_hgetlo(tlp->token.beginRange) + 1;
#ifdef AFS_AIX31_ENV
    flock.l_pid = u.u_epid;
    flock.l_sysid = u.u_sysid;
#endif

    /* Wait for other lock token holders to finish trying to obtain locks */
retry:
    oldCounter = avc->tokenAddCounter;
    while (avc->vstates & TKC_PARTLOCK) {
	avc->vstates |= TKC_PLWAITING;
	osi_SleepW(&avc->vstates, &avc->lock);
	lock_ObtainWrite(&avc->lock);
    }

    /*
     * Vexing hack:
     * must release vcache entry lock here, because some file systems'
     * lock ops call (glued) VOP_GETATTR even when they don't need the results.
     */
    lock_ReleaseWrite(&avc->lock);
#ifdef AFS_SUNOS5_ENV
    code = VOPX_FRLOCK(avc->vp, F_GETLK, &flock, 0, (offset_t)0, osi_credp);
#else
    code = VOPX_LOCKCTL(avc->vp, &flock, F_GETLK, osi_credp, 0, 0);
#endif
    lock_ObtainWrite(&avc->lock);

    /* Since we dropped the lock, may have to recheck */
    if (avc->tokenAddCounter != oldCounter) goto retry;

    /* If no locks, can return lock token */
    if (code == 0)
	return 0;

noRevoke:
    /* 
     * Don't let lock tokens being used be revoked, but do return this 
     * locker's info.
     */
    at->recordLock.l_type =  AFS_hgetlo(tlp->token.type);
    at->recordLock.l_start_pos = tlp->token.beginRange;
    at->recordLock.l_end_pos = tlp->token.endRange;
    at->recordLock.l_pid =   tlp->procid;
    at->recordLock.l_sysid = 0;
    at->outFlags = AFS_REVOKE_LOCKDATA_VALID;

    return -1;
}
