/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rep_peer.c,v $
 * Revision 1.1.154.1  1996/10/02  18:14:27  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:46:07  damon]
 *
 * $EndLog$
 */
/*
 * (C) Copyright 1990, 1996 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

/* rep_peer.c: peer promise management. */

#include <dcedfs/param.h>
#include <pthread.h>
#include <stdio.h>
#ifndef AFS_OSF_ENV
#include <unistd.h>
#endif /* AFS_OSF_ENV */
#include <dcedfs/sysincludes.h>		/* Basic kernel include files */
#include <dcedfs/basicincludes.h>	/* Another basic include file set */
#include <dcedfs/osi.h>
#include <dcedfs/compat.h>
#include <dcedfs/afs4int.h>		/* afsFid */
#include <dcedfs/rep_proc.h>
#include <dcedfs/afsvl_proc.h>
#include <dcedfs/icl.h>
#include <dcedfs/osi_dfserrors.h>
#include <dcedfs/dfsauth.h>

#include <repser.h>
#include <rep_trace.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/rep/rep_peer.c,v 1.1.154.1 1996/10/02 18:14:27 damon Exp $")

/* Keeping track of our will-call promises. */

/* The format of the will-call file is as follows.  Everything is either a
    ``long'' or a structure. 
  first word: CALLS_Signature
  zero or more of: {
      CALLS_Vol
      vol-counter-value
      sizeof(struct willCall)
      the willCall structure itself
      zero or more of: {
	  CALLS_Whom
	  copy of vol-counter-value
	  sizeof(struct callWhom)
	  the callWhom structure itself
      }
  }

This file is replaced by creating another file in the same directory
and renaming the newly-written file over the old copy of the file.
 */

/* Structures to save the collection of stable promises we've made, on the local disk. */
#define	NUM_SAVED_ADDRS	4
#define	CALLS_Signature		(0xb6ae71ec)
#define	CALLS_Vol		1	/* a willCall structure */
#define	CALLS_Whom		2	/* a callWhom structure */
struct callWhom {
    struct callWhom *Next;
    struct afsNetAddr addrs[NUM_SAVED_ADDRS];
    u_long lastTried;
    int numAddrs;
};

struct willCall {
    struct willCall *Next;
    afs_hyper_t cellId, volId;
    struct callWhom *whom;
    u_long whenRcvd;
    u_long Flags;
};

static struct willCall *CallRoot;
static char willCallFilename[600];
static int willCallState;	/* -1 init, 0 clean, 1 dirty */
static osi_dlock_t callRootLock, writeCallsLock, princLock;

static struct princs {
    struct princs *next;
    struct afsNetAddr addrs[NUM_SAVED_ADDRS];
    int numAddrs;
    kerb_princ_name princ;
    u_char valid;
} *princRoot = NULL;

static u_char *getPrincName(addrp)
struct afsNetAddr *addrp;
{
    struct princs *pp;
    struct princs *oldpp;
    struct replica *rp;
    int ix;
    unsigned32 code;
    struct siteDesc desc;

    oldpp = NULL;
    /* See if the principal name is cached already */
    lock_ObtainRead(&princLock);
    for (pp = princRoot; pp != NULL; pp = pp->next) {
	for (ix = 0; ix < pp->numAddrs; ++ix) {
	    if (SameAddress(&pp->addrs[ix], addrp)) {
		oldpp = pp;
		if (pp->valid) {
		    lock_ReleaseRead(&princLock);
		    return (u_char*)&pp->princ.text[0];
		}
	    }
	}
    }
    lock_ReleaseRead(&princLock);
    /* No--try to find principal in set of primaries. */
    pp = (struct princs *) osi_Alloc(sizeof(struct princs));
    if (pp == NULL)
	return NULL;	/* sigh */
    bzero((char *)pp, sizeof(*pp));	/* clears 'valid' too */
    lock_ObtainRead(&RepTableLock);
    for (rp = RepRoot; rp != NULL && !pp->valid; rp = rp->Next) {
	lock_ObtainRead(&(rp->lockObject));
	for (ix = 0; ix < rp->v.NumPrimaryAddrs && !pp->valid; ++ix) {
	    if (SameAddress(&rp->v.PrimaryAddrs[ix], addrp)) {
		pp->numAddrs = rp->v.NumPrimaryAddrs;
		bcopy((char*)&rp->v.PrimaryAddrs[0], (char*)&pp->addrs[0],
		      sizeof(struct afsNetAddr)*pp->numAddrs);
		bcopy((char*)&rp->v.hName[0], (char*)&pp->princ.text[0],
		      sizeof(pp->princ));
		pp->valid = 1;
		icl_Trace3(rep_iclSetp, REP_TRACE_PRINC_FROMPRIM,
			   ICL_TYPE_LONG, ((struct sockaddr_in
					    *)addrp)->sin_addr.s_addr,
			   ICL_TYPE_STRING, &pp->princ.text[0],
			   ICL_TYPE_STRING, &rp->v.mainVolName[0]);
		break;
	    }
	}
	lock_ReleaseRead(&(rp->lockObject));
    }
    lock_ReleaseRead(&RepTableLock);

    if (!pp->valid) {
	/* Not there either.  Check with the FLDB. */
	code = ubik_Call(VL_GetSiteInfo, VLDBcstruct, 0, addrp, &desc);
	if (code != 0) {
	    osi_Free(pp, sizeof(struct princs));
	    icl_Trace2(rep_iclSetp, REP_TRACE_PRINC_VLERR,
		       ICL_TYPE_LONG, ((struct sockaddr_in
					*)addrp)->sin_addr.s_addr,
		       ICL_TYPE_LONG, code);
	    return NULL;
	}
	for (ix = 0; ix < NUM_SAVED_ADDRS; ++ix) {
	    if (desc.Addr[ix].type == (unsigned short) -1)
		break;
	    bcopy((char*)&desc.Addr[ix], (char*)&pp->addrs[ix], sizeof(afsNetAddr));
	    pp->numAddrs = ix;
	}
	bcopy((char*)&desc.KerbPrin[0], (char*)&pp->princ.text[0],
	      sizeof(pp->princ));
	pp->valid = 1;
	icl_Trace2(rep_iclSetp, REP_TRACE_PRINC_FROMFLDB,
		   ICL_TYPE_LONG, ((struct sockaddr_in
				    *)addrp)->sin_addr.s_addr,
		   ICL_TYPE_STRING, &pp->princ.text[0]);
    }
    lock_ObtainWrite(&princLock);
    if (oldpp /* Does this just revalidate an existing entry? */
	&& oldpp->numAddrs == pp->numAddrs
	&& SameAddresses(&oldpp->addrs[0], &pp->addrs[0],
		pp->numAddrs)
	&& strcmp((char*)&oldpp->princ.text[0],
		  (char*)&pp->princ.text[0]) == 0) {
	/* This one duplicates the old one. */
	osi_Free(pp, sizeof(struct princs));
	oldpp->valid = 1;
	pp = oldpp;
	icl_Trace2(rep_iclSetp, REP_TRACE_PRINC_DUPLICATE,
		   ICL_TYPE_LONG, ((struct sockaddr_in
				    *)addrp)->sin_addr.s_addr,
		   ICL_TYPE_STRING, &pp->princ.text[0]);
    } else {
	/* Nope; it's a new one. */
	pp->next = princRoot;
	princRoot = pp;
    }
    lock_ReleaseWrite(&princLock);
    return (u_char*)&pp->princ.text[0];
}

static void discardPrincs()
{
    struct princs *pp;

    /* We don't deallocate since we handed out pointers to this storage */
    lock_ObtainWrite(&princLock);
    for (pp = princRoot; pp != NULL; pp = pp->next) {
	pp->valid = 0;
    }
    lock_ReleaseWrite(&princLock);
}

static long writeCallsFile()
{/* Write the will-call file after a change. */
    struct willCall *wc;
    struct callWhom *whom;
    char fn[650];
    int ofd, err;
    u_long dum, ctr;
    FILE *nfile;

    strcpy(fn, willCallFilename);
    strcat(fn, ".N");
    lock_ObtainWrite(&callRootLock);
    if (willCallState < 0) {
	lock_ReleaseWrite(&callRootLock);
	return EINVAL;
    }
    if (willCallState == 0) {
	lock_ReleaseWrite(&callRootLock);
	return 0;
    }
    lock_ObtainWrite(&writeCallsLock);
    /* get exclusive lock on the promise file. */
    errno = 0;
    ofd = open(willCallFilename, O_RDWR, 0);
    if (ofd < 0 && errno != ENOENT) {
	lock_ReleaseWrite(&writeCallsLock); lock_ReleaseWrite(&callRootLock);
	return (errno == 0 ? EINVAL : errno);
    }
    if (ofd >= 0) {
	errno = 0;
	if (osi_ExclusiveLockNoBlock(ofd) < 0) {
	    err = (errno? errno : EINVAL);
	    close(ofd); lock_ReleaseWrite(&writeCallsLock); lock_ReleaseWrite(&callRootLock);
	    return err;
	}
    }
    errno = 0;
    nfile = fopen(fn, "w");
    if (nfile == NULL) {
	err = (errno? errno : EINVAL);
	if (ofd >= 0) close(ofd);
	lock_ReleaseWrite(&writeCallsLock); lock_ReleaseWrite(&callRootLock);
	return err;
    }
    dum = CALLS_Signature;
    errno = 0;
    if (fwrite((char *)&dum, 1, sizeof(dum), nfile) != sizeof(dum)) {
	err = (errno? errno : EINVAL);
	unlink(fn); fclose(nfile); if (ofd >= 0) close(ofd);
	lock_ReleaseWrite(&writeCallsLock); lock_ReleaseWrite(&callRootLock);
	return err;
    }
    for (wc = CallRoot, ctr = 0; wc != NULL; wc = wc->Next, ++ctr) {
	dum = CALLS_Vol;
	errno = 0;
	if (fwrite((char *)&dum, 1, sizeof(dum), nfile) != sizeof(dum)) {
	    err = (errno? errno : EINVAL);
	    unlink(fn); fclose(nfile); if (ofd >= 0) close(ofd);
	    lock_ReleaseWrite(&writeCallsLock); lock_ReleaseWrite(&callRootLock);
	    return err;
	}
	errno = 0;
	if (fwrite((char *)&ctr, 1, sizeof(ctr), nfile) != sizeof(ctr)) {
	    err = (errno? errno : EINVAL);
	    unlink(fn); fclose(nfile); if (ofd >= 0) close(ofd);
	    lock_ReleaseWrite(&writeCallsLock); lock_ReleaseWrite(&callRootLock);
	    return err;
	}
	dum = sizeof(struct willCall);
	errno = 0;
	if (fwrite((char *)&dum, 1, sizeof(dum), nfile) != sizeof(dum)) {
	    err = (errno? errno : EINVAL);
	    unlink(fn); fclose(nfile); if (ofd >= 0) close(ofd);
	    lock_ReleaseWrite(&writeCallsLock); lock_ReleaseWrite(&callRootLock);
	    return err;
	}
	errno = 0;
	if (fwrite((char *)wc, 1, sizeof(struct willCall), nfile) != sizeof(struct willCall)) {
	    err = (errno? errno : EINVAL);
	    unlink(fn); fclose(nfile); if (ofd >= 0) close(ofd);
	    lock_ReleaseWrite(&writeCallsLock); lock_ReleaseWrite(&callRootLock);
	    return err;
	}
	for (whom = wc->whom; whom != NULL; whom = whom->Next) {
	    dum = CALLS_Whom;
	    errno = 0;
	    if (fwrite((char *)&dum, 1, sizeof(dum), nfile) != sizeof(dum)) {
		err = (errno? errno : EINVAL);
		unlink(fn); fclose(nfile); if (ofd >= 0) close(ofd);
		lock_ReleaseWrite(&writeCallsLock); lock_ReleaseWrite(&callRootLock);
		return err;
	    }
	    errno = 0;
	    if (fwrite((char *)&ctr, 1, sizeof(ctr), nfile) != sizeof(ctr)) {
		err = (errno? errno : EINVAL);
		unlink(fn); fclose(nfile); if (ofd >= 0) close(ofd);
		lock_ReleaseWrite(&writeCallsLock); lock_ReleaseWrite(&callRootLock);
		return err;
	    }
	    dum = sizeof(struct callWhom);
	    errno = 0;
	    if (fwrite((char *)&dum, 1, sizeof(dum), nfile) != sizeof(dum)) {
		err = (errno? errno : EINVAL);
		unlink(fn); fclose(nfile); if (ofd >= 0) close(ofd);
		lock_ReleaseWrite(&writeCallsLock); lock_ReleaseWrite(&callRootLock);
		return err;
	    }
	    errno = 0;
	    if (fwrite((char *)whom, 1, sizeof(struct callWhom), nfile) != sizeof(struct callWhom)) {
		err = (errno? errno : EINVAL);
		unlink(fn); fclose(nfile); if (ofd >= 0) close(ofd);
		lock_ReleaseWrite(&writeCallsLock); lock_ReleaseWrite(&callRootLock);
		return err;
	    }
	}
    }
    errno = 0;
    if (fflush(nfile) == EOF) {
	err = (errno? errno : EINVAL);
	unlink(fn); fclose(nfile); if (ofd >= 0) close(ofd);
	lock_ReleaseWrite(&writeCallsLock); lock_ReleaseWrite(&callRootLock);
	return err;
    }
    errno = 0;
    if (fsync(fileno(nfile)) != 0) {
	err = (errno? errno : EINVAL);
	unlink(fn); fclose(nfile); if (ofd >= 0) close(ofd);
	lock_ReleaseWrite(&writeCallsLock); lock_ReleaseWrite(&callRootLock);
	return err;
    }
    errno = 0;
    if (fclose(nfile) == EOF) {
	err = (errno? errno : EINVAL);
	unlink(fn); if (ofd >= 0) close(ofd);
	lock_ReleaseWrite(&writeCallsLock); lock_ReleaseWrite(&callRootLock);
	return err;
    }
    errno = 0;
    if (rename(fn, willCallFilename) != 0) {
	err = (errno? errno : EINVAL);
	unlink(fn); if (ofd >= 0) close(ofd);
	lock_ReleaseWrite(&writeCallsLock); lock_ReleaseWrite(&callRootLock);
	return err;
    }
    if (ofd >= 0) {
	(void) close(ofd);
    }
    lock_ReleaseWrite(&writeCallsLock);
    willCallState = 0;
    lock_ReleaseWrite(&callRootLock);
    return 0;
}

static long readCallsFile()
{/* Read the will-call file, at startup time. */
    struct willCall *wc;
    struct callWhom *whom;
    int err;
    u_long dum, ctr;
    FILE *nfile;

    lock_ObtainWrite(&callRootLock);
    errno = 0;
    nfile = fopen(willCallFilename, "r");
    if (nfile == NULL) {
	err = (errno? errno : EINVAL);
	if (err == ENOENT) {	/* It's OK if we're about to create this file. */
	    willCallState = 0;
	    err = 0;
	}
	lock_ReleaseWrite(&callRootLock);
	return err;
    }
    errno = 0;
    if (fread((char *)&dum, 1, sizeof(dum), nfile) != sizeof(dum)) {
	err = (errno? errno : EINVAL);
	fclose(nfile); lock_ReleaseWrite(&callRootLock);
	return err;
    }
    if (dum != CALLS_Signature) {
	fclose(nfile); lock_ReleaseWrite(&callRootLock);
	return EINVAL;
    }
    errno = 0;
    err = fread((char *)&dum, 1, sizeof(dum), nfile);
    if (err == 0) {				/* at EOF */
	fclose(nfile);
	willCallState = 0;
	lock_ReleaseWrite(&callRootLock);
	return 0;
    }
    if (err != sizeof(dum)) {
	err = (errno? errno : EINVAL);
	fclose(nfile); lock_ReleaseWrite(&callRootLock);
	return err;
    }
    CallRoot = NULL;
    for (;;) {	/* just after beginning of a new willCall structure record */
	if (dum != CALLS_Vol) {
	    fclose(nfile); lock_ReleaseWrite(&callRootLock);
	    return EINVAL;
	}
	errno = 0;
	if (fread((char *)&dum, 1, sizeof(dum), nfile) != sizeof(dum)) {
	    err = (errno? errno : EINVAL);
	    fclose(nfile); lock_ReleaseWrite(&callRootLock);
	    return err;
	}
	ctr = dum;		/* will expect this value in sub-structures */
	errno = 0;
	if (fread((char *)&dum, 1, sizeof(dum), nfile) != sizeof(dum)) {
	    err = (errno? errno : EINVAL);
	    fclose(nfile); lock_ReleaseWrite(&callRootLock);
	    return err;
	}
	if (dum != sizeof(struct willCall)) {
	    fclose(nfile); lock_ReleaseWrite(&callRootLock);
	    return EINVAL;
	}
	wc = (struct willCall *) osi_Alloc(sizeof(struct willCall));
	if (wc == NULL) {
	    fclose(nfile); lock_ReleaseWrite(&callRootLock);
	    return ENOMEM;
	}
	errno = 0;
	if (fread((char *)wc, 1, sizeof(struct willCall), nfile) != sizeof(struct willCall)) {
	    err = (errno? errno : EINVAL);
	    fclose(nfile); lock_ReleaseWrite(&callRootLock);
	    return err;
	}
	wc->whom = NULL;
	wc->Next = CallRoot;
	CallRoot = wc;	/* got a new willCall record */
	errno = 0;
	err = fread((char *)&dum, 1, sizeof(dum), nfile);
	if (err == 0) {			/* at EOF */
	    fclose(nfile); willCallState = 0; lock_ReleaseWrite(&callRootLock);
	    return 0;
	}
	if (err != sizeof(dum)) {
	    err = (errno? errno : EINVAL);
	    fclose(nfile); lock_ReleaseWrite(&callRootLock);
	    return err;
	}
	while (dum == CALLS_Whom) {	/* got a new callWhom record */
	    errno = 0;
	    if (fread((char *)&dum, 1, sizeof(dum), nfile) != sizeof(dum)) {
		err = (errno? errno : EINVAL);
		fclose(nfile); lock_ReleaseWrite(&callRootLock);
		return err;
	    }
	    if (dum != ctr) {fclose(nfile); lock_ReleaseWrite(&callRootLock); return EINVAL;}
	    errno = 0;
	    if (fread((char *)&dum, 1, sizeof(dum), nfile) != sizeof(dum)) {
		err = (errno? errno : EINVAL);
		fclose(nfile); lock_ReleaseWrite(&callRootLock);
		return err;
	    }
	    if (dum != sizeof(struct callWhom)) {
		fclose(nfile); lock_ReleaseWrite(&callRootLock);
		return EINVAL;
	    }
	    whom = (struct callWhom *) osi_Alloc(sizeof(struct callWhom));
	    if (whom == NULL) {
		fclose(nfile); lock_ReleaseWrite(&callRootLock);
		return ENOMEM;
	    }
	    errno = 0;
	    if (fread((char *)whom, 1, sizeof(struct callWhom), nfile) != sizeof(struct callWhom)) {
		err = (errno? errno : EINVAL);
		fclose(nfile); lock_ReleaseWrite(&callRootLock);
		return err;
	    }
	    whom->Next = wc->whom;	/* got a new callWhom record */
	    wc->whom = whom;
	    errno = 0;
	    err = fread((char *)&dum, 1, sizeof(dum), nfile);
	    if (err == 0) {			/* at EOF */
		fclose(nfile); willCallState = 0; lock_ReleaseWrite(&callRootLock);
		return 0;
	    }
	    if (err != sizeof(dum)) {
		err = (errno? errno : EINVAL);
		fclose(nfile); lock_ReleaseWrite(&callRootLock);
		return err;
	    }
	}
    }
}

static struct willCall *getWC(cellidp, volidp, flags)
afs_hyper_t *cellidp, *volidp;
u_long flags;
{/* Return an old or new pointer to a willCall structure. */
    struct willCall *wc;

    lock_ObtainShared(&callRootLock);
    for (wc = CallRoot; wc != NULL; wc = wc->Next) {
	if (AFS_hsame(*volidp, wc->volId) && sameCell(cellidp, &wc->cellId)
	    && wc->Flags == flags) {
	    lock_ReleaseShared(&callRootLock);
	    return wc;
	}
    }
    wc = (struct willCall *) osi_Alloc(sizeof(struct willCall));
    if (wc == NULL) {
	lock_ReleaseShared(&callRootLock);
	return (struct willCall *) 0;
    }
    bzero((char *)wc, sizeof(struct willCall));
    wc->cellId = *cellidp;
    wc->volId = *volidp;
    wc->Flags = flags;
    lock_UpgradeSToW(&callRootLock);
    wc->Next = CallRoot;
    CallRoot = wc;
    willCallState = 1;
    lock_ReleaseWrite(&callRootLock);
    return wc;
}

static long addToWc(wc, adrp, num)
struct willCall *wc;
struct afsNetAddr *adrp;
int num;
{/* Add the ``num'' addresses (a single machine) at ``adrp'' to the callWhom structure. */
    struct callWhom *whom;
    struct afsNetAddr *adt1, *adt2;
    int ix1, ix2;

    lock_ObtainShared(&callRootLock);
    /* All OK if this address is for the local machine. */
    for (adt1 = myNetAddrs, ix1 = 0; ix1 < numMyNetAddrs; ++adt1, ++ix1) {
	for (adt2 = adrp, ix2 = 0; ix2 < num; ++ix2, ++adt2) {
	    if (SameAddress(adt1, adt2)) {
		lock_ReleaseShared(&callRootLock);
		return 0;
	    }
	}
    }
    /* Attempt to find this machine in the list already. */
    for (whom = wc->whom; whom != NULL; whom = whom->Next) {
	for (adt1 = whom->addrs, ix1 = 0; ix1 < whom->numAddrs; ++ix1, ++adt1) {
	    for (adt2 = adrp, ix2 = 0; ix2 < num; ++ix2, ++adt2) {
		if (SameAddress(adt1, adt2)) {
		    lock_ReleaseShared(&callRootLock);
		    return 0;
		}
	    }
	}
    }
    /* Not in the list already.  Allocate a new callWhom structure. */
    whom = (struct callWhom *) osi_Alloc(sizeof(struct callWhom));
    if (whom == NULL) return ENOMEM;
    bzero((char *)whom, sizeof(struct callWhom));
    ix2 = (num > NUM_SAVED_ADDRS ? NUM_SAVED_ADDRS : num);
    bcopy((char *)adrp, (char *)whom->addrs, ix2 * sizeof(struct afsNetAddr));
    whom->numAddrs = ix2;
    lock_UpgradeSToW(&callRootLock);
    whom->Next = wc->whom;
    wc->whom = whom;
    willCallState = 1;
    lock_ReleaseWrite(&callRootLock);
    return 0;
}


u_long DoWillCalls()
{/* Execute the will-call calls.  Return when we should be called again. */
  struct willCall * volatile wc, * volatile lastwc, * volatile nextwc;
  struct callWhom * volatile whom, * volatile lastwhom, * volatile nextwhom;
  volatile u_long Now, nextTime;
  u_long timeTemp;
  struct afsNetAddr * volatile adrp;
  volatile struct afsNetAddr badAddr;
  volatile u_long badTriedTime;
  u_long ret;
  int ix;
  volatile int anyDeleted;
  volatile long code;
  volatile rpc_binding_handle_t connp;
  u_long spare4, spare5;
  u_long st;
  u_char *princp;
  char pNameBuf[1024];
  extern char *ExceptionToString();
  char *excNameP;
  char excBuff[50];

  Now = osi_Time();
  nextTime = Now + 24*60*60;
  anyDeleted = 0;
  bzero((char *)&badAddr, sizeof(badAddr));
  badTriedTime = 0;
  lock_ObtainRead(&callRootLock);
  for (wc = CallRoot; wc != NULL; wc = wc->Next) {
    if (wc->whenRcvd != 0) {
      /* don't keep trying forever. */
      if (wc->whom == NULL || (wc->whenRcvd + (2*24*60*60)) < Now) {
	wc->whenRcvd = 0;
	anyDeleted = 1;
      }
      for (whom = wc->whom; whom != NULL; whom = whom->Next) {
	if (whom->lastTried == 0 || (whom->lastTried + 15*60) <= Now) {
	    if (SameAddress(&whom->addrs[0], &badAddr)) {
		whom->lastTried = badTriedTime;
	    } else {
		whom->lastTried = Now;
		for (ix = 0, adrp = &whom->addrs[0]; ix < whom->numAddrs; ++ix, ++adrp) {
		    if (adrp->type == AF_INET) {
			icl_Trace1(rep_iclSetp, REP_DOCALLS_CONNECT,
				   ICL_TYPE_STRING,
				   inet_ntoa(((struct sockaddr_in *) adrp)->sin_addr));
			princp = getPrincName(adrp);
			if (princp == NULL) {
			    icl_Trace1(rep_iclSetp, REP_DOCALLS_PRINC_FAIL,
				       ICL_TYPE_STRING, 
				       inet_ntoa(((struct sockaddr_in *) adrp)->sin_addr));
			    continue;
			}
			errno = 0;
			((struct sockaddr_in *) adrp)->sin_port = 0;
			rpcx_binding_from_sockaddr((struct sockaddr *)adrp, 
						   (rpc_binding_handle_t *)&connp);
			if (connp == NULL) {
			    icl_Trace2(rep_iclSetp, REP_DOCALLS_CONN_FAIL,
				       ICL_TYPE_STRING, 
				       inet_ntoa(((struct sockaddr_in *) adrp)->sin_addr),
				       ICL_TYPE_LONG,
				       errno);
			    continue;
			}
			/* Initialize the principal name */
			sprintf(pNameBuf, "%s/%s/dfs-server", localCellName, princp);
			code = dfsauth_client_InitBindingAuth(
						(rpc_binding_handle_t*)&connp,
						0, 1, pNameBuf);
			if (code == 0)
			    break;
			icl_Trace3(rep_iclSetp, REP_DOCALLS_AUTH_FAIL,
				   ICL_TYPE_STRING, 
				   inet_ntoa(((struct sockaddr_in *) adrp)->sin_addr),
				   ICL_TYPE_STRING, pNameBuf,
				   ICL_TYPE_LONG, code);
			rpc_binding_free((rpc_binding_handle_t *)&connp, &st);
			discardPrincs();
		    }
		}
	    if (connp != NULL) {
	      TRY {
		if (wc->Flags & 1) {
		  code = REP_UpdateSelf(connp, &wc->cellId, &wc->volId,
					wc->Flags, 0, 0, 0, &spare4, &spare5);
		  code = osi_errDecode(code);
		  if (code == REP_ERR_NOVOLUME)
		    code = 0;	/* authoritative answer from other repserver */
		} else {
		  code = REP_CheckReplicationConfig(connp, &wc->cellId,
						    &wc->volId,
						    wc->Flags,
						    0, 0, 0, &spare4,
						    &spare5);
		  code = osi_errDecode(code);
		}
	      } CATCH_ALL {
		excNameP = ExceptionToString(THIS_CATCH);
		if (excNameP == NULL) {
		  sprintf(excBuff, "[[unk: %#lx]]", THIS_CATCH);
		  excNameP = excBuff;
		}
		Log("DoWillCalls, flags %d: Exception while calling: %s",
		    wc->Flags, excNameP);
		icl_Trace2(rep_iclSetp, REP_DOCALLS_EXCEPTION,
			   ICL_TYPE_LONG, wc->Flags,
			   ICL_TYPE_STRING, excNameP);
		code = EPIPE;
	      } ENDTRY
	      if (code == 0) {
		  whom->lastTried = 0; /* done */
		  anyDeleted = 1;
	      } else 
		  icl_Trace3(rep_iclSetp, REP_DOCALLS_FAIL,
			     ICL_TYPE_STRING, inet_ntoa(((struct sockaddr_in *) adrp)->sin_addr),
			     ICL_TYPE_STRING, ((wc->Flags & 1) ? "update" : "config"),
			     ICL_TYPE_STRING, dfs_dceErrTxt(code));
	      rpc_binding_free((rpc_binding_handle_t *)&connp, &st);
	      /* mark one unreachable address for this pass */
	      if (code >= rpc_s_mod && code <= (rpc_s_mod+4096)) {
		badAddr = whom->addrs[0];
		badTriedTime = Now;
	      }
	    }
	  }
	}
	if (whom->lastTried != 0) {
	  timeTemp = whom->lastTried + 15*60;
	  if (timeTemp < nextTime) nextTime = timeTemp;
	}
      }
    }
  }
  lock_ReleaseRead(&callRootLock);
  if (anyDeleted) {
    lock_ObtainWrite(&callRootLock);
    /* First, get rid of the expired entities. */
    lastwc = NULL;
    for (wc = CallRoot; wc != NULL; wc = nextwc) {
      nextwc = wc->Next;
      if (wc->whenRcvd == 0) {
	for (whom = wc->whom; whom != NULL; whom = nextwhom) {
	  nextwhom = whom->Next;
	  osi_Free((opaque) whom, sizeof(struct callWhom));
	}
	wc->whom = NULL;
      }
    }
    /* Now, delete the ones we've made calls to. */
    lastwc = NULL;
    for (wc = CallRoot; wc != NULL; wc = nextwc) {
      nextwc = wc->Next;
      lastwhom = NULL;
      for (whom = wc->whom; whom != NULL; whom = nextwhom) {
	nextwhom = whom->Next;
	if (whom->lastTried == 0) {
	  if (lastwhom == NULL) {
	    wc->whom = whom->Next;
	  } else {
	    lastwhom->Next = whom->Next;
	  }
	  osi_Free((opaque) whom, sizeof(struct callWhom));
	} else {
	  lastwhom = whom;
	}
      }
      if (wc->whom == NULL) {
	if (lastwc == NULL) {
	  CallRoot = wc->Next;
	} else {
	  lastwc->Next = wc->Next;
	}
      } else {
	lastwc = wc;
      }
    }
    willCallState = 1;
    lock_ReleaseWrite(&callRootLock);
    ret = writeCallsFile();
    if (ret != 0) {
      sprintf(ErrBuff, "DoWillCalls: error %s (%d) writing calls file", dfs_dceErrTxt(ret), ret);
      icl_Trace1(rep_iclSetp, REP_TR_MSG,
		  ICL_TYPE_STRING, ErrBuff);
      Log("%s", ErrBuff);
    }
  }
  return nextTime;
}

long handleWillCall(cellIdP, volIdP, howManyGuys, TheseGuysToo, flags)
afs_hyper_t *cellIdP, *volIdP;
u_long howManyGuys;
afsNetAddrs *TheseGuysToo;
u_long flags;
{/* Promise to get everybody to check the replication configuration for the given volume. */
/* Keep the initial time of call, the volume descriptor, and the set of other guys. */
    long ret, ret2;
    struct RepSite *site;
    int ix;
    struct willCall *wc;
    struct replica *rp;

    lock_ObtainRead(&callRootLock);
    if (willCallState < 0) {
	lock_ReleaseRead(&callRootLock);
	readCallsFile();
	lock_ObtainRead(&callRootLock);
    }
    ret = willCallState;
    lock_ReleaseRead(&callRootLock);
    if (ret < 0) return REP_ERR_NOSTORAGE;
    wc = getWC(cellIdP, volIdP, flags);
    if (wc == NULL) return ENOMEM;
    wc->whenRcvd = osi_Time();
#if 0
    wc->Flags |= flags;
#endif /* 0 */
    willCallState = 1;
    for (ix = 0; ix < TheseGuysToo->afsNetAddrs_len; ++ix) {
	ret = addToWc(wc, &TheseGuysToo->afsNetAddrs_val[ix], 1);
	if (ret != 0) return ret;
    }
    /* Remember the set of servers before and after a VLDB lookup. */
    lock_ObtainRead(&RepTableLock);
    for (rp = RepRoot; rp != NULL; rp = rp->Next) {
	if (AFS_hsame(*volIdP, rp->v.VolIDOfSecondary) && sameCell(cellIdP, &rp->CellId)) {
	    ret = addToWc(wc, &rp->v.PrimaryAddrs[0], rp->v.NumPrimaryAddrs);
	    if (ret != 0) {lock_ReleaseRead(&RepTableLock); return ret;}
	    for (ix = 0, site = rp->v.Reps; ix < rp->v.NumReps; ++ix, ++site) {
		ret = addToWc(wc, &site->AddrInfo[0], site->NumAddrs);
		if (ret != 0) {lock_ReleaseRead(&RepTableLock); return ret;}
	    }
	    if (flags & 1)
		RepHold(rp);
	    break;
	}
    }
    lock_ReleaseRead(&RepTableLock);
    /* Now, do one of two things: either update a local replica, or absorb new VLDB info. */
    if (flags & 1) {
	if (rp != NULL) {
	    rep_doUpdate(rp);
	    RepRele(rp);
	    ret = 0;
	} else {
	    ret = REP_ERR_UNKNOWNREPLICA;
	}
    } else {
	ret = CheckVLDBRelationship(cellIdP, volIdP, flags);	/* checks locally */
	if (ret == 0) {
	    lock_ObtainRead(&RepTableLock);
	    for (rp = RepRoot; rp != NULL; rp = rp->Next) {
		if (AFS_hsame(*volIdP, rp->v.VolIDOfSecondary) && sameCell(cellIdP, &rp->CellId)) {
		    ret = addToWc(wc, &rp->v.PrimaryAddrs[0], rp->v.NumPrimaryAddrs);
		    if (ret != 0) {lock_ReleaseRead(&RepTableLock); return ret;}
		    for (ix = 0, site = rp->v.Reps; ix < rp->v.NumReps; ++ix, ++site) {
			ret = addToWc(wc, &site->AddrInfo[0], site->NumAddrs);
			if (ret != 0) {lock_ReleaseRead(&RepTableLock); return ret;}
		    }
		}
	    }
	    lock_ReleaseRead(&RepTableLock);
	}
    }
    if (ret != 0) return ret;
    ret2 = writeCallsFile();
    if (ret == 0) ret = ret2;
    ret2 = pthread_cond_signal(&actNowCond);
    osi_assert(ret2 == 0);
    return ret;
}

void initPeers()
{/* Called from initialization */
    strcpy(willCallFilename, DCELOCAL_PATH);
    strcat(willCallFilename, "/var/dfs/RepPromises");
    lock_Init(&callRootLock);
    lock_Init(&writeCallsLock);
    lock_Init(&princLock);
    willCallState = -1;
    CallRoot = NULL;
    (void) readCallsFile();
}
