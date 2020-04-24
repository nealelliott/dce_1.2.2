/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * client.c -- 
 *    Client code for cache consistency tests.
 */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: client.c,v $
 * Revision 1.1.409.1  1996/10/17  18:17:33  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:49:44  damon]
 *
 * Revision 1.1.404.1  1994/02/04  20:39:56  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:22:17  devsrc]
 * 
 * Revision 1.1.402.1  1993/12/07  17:41:27  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  18:39:54  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/11  18:25:02  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:56:17  htf]
 * 
 * Revision 1.1.2.2  1992/10/28  16:01:05  jaffe
 * 	Transarc delta: fred-add-cc-tests 1.2
 * 	  Selected comments:
 * 	    Added Mike Kazar's cache consistency tests to the test tree.
 * 	    Forgot to put a EndLog marker in the header comment.
 * 	    Added EndLog marker.
 * 	[1992/10/27  16:59:14  jaffe]
 * 
 * $EndLog$
 */

#include <dcedfs/osi.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#ifdef _AIX
#include <sys/select.h>
#endif

#include "com.h"
#include "queue.h"

long wlr_cidCounter = 1;

#define WLR_REVBINDNAME	"ReverseBind"

/* copy a client set */
wlr_ClientSetCopy(asetp, osetp)
  struct wlr_clientSet *asetp;
  struct wlr_clientSet *osetp;
{
    struct wlr_client *isp, *osp;

    wlr_ClientSetInit(osetp);

    for(isp = wlr_ClientSetFirst(asetp); isp;
	isp=wlr_ClientSetNext(asetp, isp)) {
	osp = (struct wlr_client *) malloc(sizeof(*osp));
	bzero(osp, sizeof(*osp));
	osp->name = (char *) malloc(strlen(isp->name)+1);
	strcpy(osp->name, isp->name);
	osp->userFD = isp->userFD;
	osp->callp = isp->callp;
	wlr_Hold(osp->callp);
	bcopy(&isp->remoteAddr, &osp->remoteAddr, sizeof(osp->remoteAddr));
	wlr_ClientSetAdd(osetp, osp);
    }
    return 0;
}

/* free a client set */
wlr_ClientSetClear(setp)
  struct wlr_clientSet *setp;
{
    struct wlr_client *icp;
    /* remove all client sets, free the name, and rele the call*/
    while(icp = wlr_ClientSetFirst(setp)) {
	free(icp->name);
	wlr_Rele(icp->callp);
	wlr_ClientSetRemove(setp, icp);
    }
    return 0;
}


/* add a client to the client list.  Need client structure before
 * can make RPCs via call or acall.
 *
 * asockp is socket addr to bind to
 * aname is name for error messages
 * aclientpp will get new client structure
 */
wlr_NewClient(asockp, aname, aclientpp)
  struct sockaddr_in *asockp;
  char *aname;
  struct wlr_client **aclientpp;
{
    register struct wlr_client *cp;
    struct wlr_call *callp;
    int s;
    int code;

    /* ensure system initialized properly */
    if (!wlr_Inited) wlr_Init();

    cp = (struct wlr_client *) malloc(sizeof(struct wlr_client));
    bzero(cp, sizeof(*cp));
    callp = wlr_GetCall();	/* return with refcount high */
    bcopy(asockp, &cp->remoteAddr, sizeof(struct sockaddr_in));
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) return -1;
    /* now bind the socket to the appropriate address */
    code = connect(s, (struct sockaddr *)asockp, sizeof(struct sockaddr_in));
    if (code) {
	close(s);
	return errno;
    }
    if (aname) {
	code = strlen(aname) + 1;
	cp->name = (char *) malloc(code);
	strcpy(cp->name, aname);
    }
    callp->socket = s;
    callp->cid = wlr_cidCounter++;
    callp->clientp = cp;
    cp->callp = callp;
    if (aclientpp) *aclientpp = cp;
    return 0;
}

/* generate a reverse-binding client
 *
 * baseCallp is call from site to which to bind
 * clientpp gets new client structure
 */
wlr_ReverseBind(baseCallp, clientpp)
  struct wlr_call *baseCallp;
  struct wlr_client **clientpp;
{
    struct wlr_call *callp;
    long code;
    struct wlr_client *cp;
    int s;

    cp = (struct wlr_client *) malloc(sizeof(struct wlr_client));
    bzero(cp, sizeof(*cp));
    callp = wlr_GetCall();
    callp->cid = wlr_cidCounter++;
    callp->socket = baseCallp->socket;
    cp->callp = callp;
    cp->name = (char *) malloc(strlen(WLR_REVBINDNAME)+1);
    strcpy(cp->name, WLR_REVBINDNAME);
    *clientpp = cp;
    return 0;
}

/* init a client set */
wlr_ClientSetInit(setp)
  struct wlr_clientSet *setp;
{
    QInit(&setp->q);
    return 0;
}

/* return # of elements in client set */
long wlr_Count(setp)
  struct wlr_clientSet *setp;
{
    long count;
    struct wlr_client *cp;

    count = 0;
    cp = wlr_ClientSetFirst(setp);
    while(cp) {
	count++;
	cp = wlr_ClientSetNext(setp, cp);
    }
    return count;
}

/* get first client in client set */
struct wlr_client *wlr_ClientSetFirst(setp)
  struct wlr_clientSet *setp;
{
    struct squeue *tqp;

    tqp = QNext(&setp->q);
    if (tqp == &setp->q) return (struct wlr_client *) 0;
    /* otherwise we have a real client here */
    return (struct wlr_client *) tqp;
}

/* get next client in client set after clientp */
struct wlr_client *wlr_ClientSetNext(setp, clientp)
  struct wlr_clientSet *setp;
  struct wlr_client *clientp;
{
    struct squeue *tqp;

    tqp = QNext(&clientp->q);
    if (tqp == &setp->q) return (struct wlr_client *) 0;
    /* otherwise we have a real client here */
    return (struct wlr_client *) tqp;
}

/* move first client in client set to last element */
wlr_ClientSetRotate(setp)
  struct wlr_clientSet *setp;
{
    struct squeue *tqp;
    tqp = QNext(&setp->q);
    if (tqp != &setp->q) {
	/* non-empty: move guy from head of the list to tail */
	QRemove(tqp);
	QAddT(&setp->q, tqp);
    }
    return 0;
}

/* add a client to the end of a client set */
wlr_ClientSetAdd(setp, clientp)
  struct wlr_clientSet *setp;
  struct wlr_client *clientp;
{
    QAddT(&setp->q, &clientp->q);
    return 0;
}

/* remove a client from a client set */
wlr_ClientSetRemove(setp, clientp)
  struct wlr_clientSet *setp;
  struct wlr_client *clientp;
{
    QRemove(&clientp->q);
    return 0;
}

/* Split off 1/fraction of the clinets from baseSetp into newSetp
 * if min is set, then ensure that at least one ends up in newSetp,
 * otherwise, might be empty.
 * for example, splitting out 1/3 of clients, with a minimum of
 * 1 would be done by:
 * wlr_ClientSetSplit(base, new, 3, 1);
 */
wlr_ClientSetSplit(baseSetp, newSetp, fraction, min)
  struct wlr_clientSet *baseSetp;
  struct wlr_clientSet *newSetp;
  int fraction;
  int min;
{
    struct wlr_client *tcp, *ncp;
    int count;

    if (min) count = 0;
    else count = fraction - 1;
    wlr_ClientSetInit(newSetp);
    for(tcp = wlr_ClientSetFirst(baseSetp);
	tcp;
	tcp = ncp) {
	/* do this first, since once it moves to the new set, "next"
	 * means something different.
	 */
	ncp = wlr_ClientSetNext(baseSetp, tcp);
	if (count-- == 0) {
	    count = fraction-1;	/* reset counter */
	    wlr_ClientSetRemove(baseSetp, tcp);
	    wlr_ClientSetAdd(newSetp, tcp);
	}
    }
    return 0;
}
