/*
 * (C) Copyright 1991 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * com.c -- 
 *    Code for communicating between the hub and spoke in cache
 *    consistency tests.
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
 */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>
#ifdef __hpux
#include <unistd.h>
#endif
#ifdef _AIX
#include <sys/select.h>
#endif

#include "com.h"
#include "queue.h"

long wlr_xidCounter = 1;
struct wlr_client *wlr_clientListp = 0;
struct wlr_call *wlr_activeCallsp = 0;
struct wlr_call *wlr_freeCallsp = 0;
int wlr_Inited = 0;
struct wlr_packet *wlr_freePacketp = 0;
int wlr_clientSocket = -1;
int wlr_serverSocket = -1;
int (*wlr_InterfaceProcp)();

/* function called by server stubs after server code executes */
wlr_AfterServerCall(callp, code)
  struct wlr_call *callp;
  long code;
{
    callp->r->d.code = code;
    callp->mp = callp->r->d.buffer;
    return 0;
}

/* zap all calls on an FD */
wlr_ZapFDCalls(fd)
  int fd;
{
    register struct wlr_call *tcp;
    for(tcp = wlr_activeCallsp; tcp; tcp=tcp->nextp) {
	if (tcp->socket == fd) {
	    wlr_CallBad(tcp);
	    wlr_Deactivate(tcp);
	}
    }
}

/* get a new packet */
struct wlr_packet *wlr_GetPacket()
{
    struct wlr_packet *tmp;

    if (!wlr_freePacketp) {
	tmp = (struct wlr_packet *) malloc(sizeof(struct wlr_packet));
    }
    else {
	tmp = wlr_freePacketp;
	wlr_freePacketp  = tmp->nextp;
    }
    bzero(tmp, sizeof(*tmp));
    return tmp;
}

/* free a packet */
wlr_FreePacket(packetp)
  struct wlr_packet *packetp;
{
    packetp->nextp = wlr_freePacketp;
    wlr_freePacketp = packetp;
    return 0;
}

/* mark a client call as bad */
wlr_CallBad(acallp)
  register struct wlr_call *acallp;
{
    acallp->states |= WLR_CALL_BAD | WLR_CALL_DONE;
    if (acallp->socket >= 0) {
	close(acallp->socket);
	acallp->socket = -1;
    }
    wlr_ResetCall(acallp);
    return 0;
}

/* INTERNAL: sigpipe handler */
static void wlr_Handler()
{
    return;
}

/* initialize communications package */
wlr_Init()
{
    int code;

#ifdef AFS_SIGACTION_ENV
#define SIGFUNC sigaction
    struct sigaction	nvec;

    bzero((char *)&nvec, sizeof(nvec));
    nvec.sa_handler = wlr_Handler;
#else
#ifdef __hpux
#define SIGFUNC sigvector
#else
#define SIGFUNC sigvec
#endif
    struct sigvec	nvec;

    bzero((char *)&nvec, sizeof(nvec));
    nvec.sv_handler = wlr_Handler;
#endif
    code = SIGFUNC(SIGPIPE, &nvec, 0);
    if (code) wlr_panic("init signals");
    wlr_Inited = 1;
    return 0;
}

/* find call with id CID, using FD afd.  IsClient
 * is set iff this is a client call, otherwise look for a
 * server call.
 */
static struct wlr_call *wlr_FindCall(afd, cid, isClient)
  int afd;
  long cid;
  int isClient;
{
    register struct wlr_call *cp;

    for(cp = wlr_activeCallsp; cp; cp=cp->nextp) {
	if (!(cp->states & WLR_CALL_BAD) && cp->socket == afd
	    && cp->cid == cid
	    && (((cp->states & WLR_CALL_SERVER) && !isClient)
		|| (!(cp->states & WLR_CALL_SERVER) && isClient))) {
	    cp->refCount++;
	    return cp;
	}
    }
    if (isClient) return (struct wlr_call *) 0;
    /* otherwise, create a new server call */
    return wlr_NewServerCall(afd, cid);
}

/* allocate a new call structure, allocating some, if necessary */
struct wlr_call *wlr_GetCall()
{
    register struct wlr_call *tcp;

    if (wlr_freeCallsp) {
	tcp = wlr_freeCallsp;
	wlr_freeCallsp = tcp->nextp;
    }
    else tcp = (struct wlr_call *) malloc(sizeof(struct wlr_call));

    bzero(tcp, sizeof(*tcp));
    tcp->socket = -1;
    tcp->refCount = 1;
    return tcp;
}

/* free a call structure back to the free list */
wlr_FreeCall(acallp)
  register struct wlr_call *acallp;
{
    acallp->nextp = wlr_freeCallsp;
    wlr_freeCallsp = acallp;
}

/* create a server call, using incoming FD afd, and connection ID
 * cid.
 */
struct wlr_call *wlr_NewServerCall(afd, cid)
  int afd;
  long cid;
{
    register struct wlr_call *cp;

    cp = wlr_GetCall();
    cp->states |= WLR_CALL_SERVER;	/* mark as a server call */
    cp->socket = afd;
    cp->cid = cid;
    wlr_Activate(cp);	/* put on runnable list */
    return cp;
}

/* called on the client side to wait for something interesting to
 * happen to acallp, and to handle incoming network traffic until
 * that time.
 */
static Listen(acallp)
  register struct wlr_call *acallp;
{
    struct fd_set ifd, xfd;
    register struct wlr_call *tp;
    register struct wlr_client *cp;
    int nfd;
    struct timeval tv;
    struct sockaddr_in taddr;
    int taddrLen;
    int code, i;
    struct wlr_packet *packetp;
    int didAnything;

    nfd = osi_GetMaxNumberFiles();
    if (nfd > FD_SETSIZE)
	nfd = FD_SETSIZE;

    didAnything = 0;
    while (1) {
	/* collect file descriptors */
	FD_ZERO(&ifd);
	FD_ZERO(&xfd);
	for(tp = wlr_activeCallsp; tp; tp=tp->nextp) {
	    cp = tp->clientp;
	    if (!(tp->states & WLR_CALL_BAD)) {
		if (tp->socket >= 0)
		    FD_SET(tp->socket, &ifd);
	    }
	}
	/* watch for our main socket receiving input */
	if (wlr_clientSocket >= 0) FD_SET(wlr_clientSocket, &ifd);
	if (wlr_serverSocket >= 0) FD_SET(wlr_serverSocket, &ifd);
	/* watch for exceptional conditions, too */
	bcopy(&ifd, &xfd, sizeof(xfd));

	/* now do a blocking select, waiting for input or timeout */
	tv.tv_sec = 10;
	tv.tv_usec = 0;
#ifdef __hpux
        code = select(nfd, (int *)&ifd, (int *)0, (int *)&xfd, &tv);
#else
        code = select(nfd, &ifd, (struct fd_set *) 0, &xfd, &tv);
#endif
	if (code < 0) {
	    if (code == EINTR) continue;
	    fprintf(stderr,"select returned code %d, errno %d\n", code, errno);
	    wlr_panic("client select");
	}

	/* process active input descriptors */
	for(i=0; i<nfd; i++) {
	    if (FD_ISSET(i, &ifd)) {
		didAnything = 1;
		if (wlr_serverSocket >= 0 && i == wlr_serverSocket) {
		    /* do listen dispatch */
		    taddrLen = sizeof(taddr);
		    code = accept(i, (struct sockaddr *)&taddr, &taddrLen);
		    if (code < 0)
			wlr_panic("accept");
		    wlr_NewServerCall(code, 0);
		}
		else {
		    /* found some input here, do server/client dispatch */
		    packetp = wlr_GetPacket();
		    code = wlr_read(i, (char *) &packetp->d,
				    sizeof(struct wlr_packetData));
		    if (code) {
			fprintf(stderr,"Terminating FD %d (code %d)\n", i, code);
			wlr_ZapFDCalls(i);
			wlr_FreePacket(packetp);
			continue;
		    }
		    wlr_ntohPacket(packetp);
		    tp = wlr_FindCall(i, packetp->d.cid,
				      ((packetp->d.flags & WLR_PACKET_TYPEMASK)
				       == WLR_PACKET_TYPERETURN));
		    if (!tp) wlr_panic("missing client call");
		    if (tp->states & WLR_CALL_SERVER) {
			tp->xid = packetp->d.xid;
			if (tp->s) wlr_FreePacket(tp->s);
			if (tp->r) wlr_FreePacket(tp->r);
			tp->s = packetp;
			packetp = tp->r = wlr_GetPacket();
			/* call server stub with mp pointing at
			 * packet to unmarshall (s).  Pointer changes
			 * to r in server stub code itself.
			 */
			tp->mp = tp->s->d.buffer;
			if (wlr_InterfaceProcp)
			    code = (*wlr_InterfaceProcp)(tp);
			else code = -1;
			packetp->d.flags = WLR_PACKET_TYPERETURN;
			packetp->d.xid = tp->xid;
			packetp->d.cid = tp->cid;
			packetp->d.opcode = 0;
			/* code filled in by stub procedure */
			packetp->d.len = sizeof(packetp->d);
			wlr_htonPacket(packetp);
			code = write(i, &packetp->d,
				     sizeof(struct wlr_packetData));
			if (code != sizeof(struct wlr_packetData)) {
			    fprintf(stderr,"short write to client (code %d, errno %d)\n",
				   code, errno);
			}
		    }
		    else {
			/* process response to a call */
			tp->states |= WLR_CALL_DONE;
			if (tp->r)
			    wlr_FreePacket(tp->r);
			tp->r = packetp;
			/* setup for client unmarshall */
			tp->mp = packetp->d.buffer;
		    }
		    wlr_Rele(tp);
		}
	    }
	}

	/* process exceptional file descriptors.  Happens if client dies
	 * for example, and file descriptor goes bad.
	 * Plan is to mark call as bad and continue.
	 */
	for(i = 0; i<nfd; i++) {
	    if (FD_ISSET(i, &xfd)) {
		fprintf(stderr,"FD %d went bad\n", i);
		wlr_ZapFDCalls(i);
		didAnything = 1;
	    }
	}

	/* finally, if something potentially useful happened, return
	 * so our caller can make some progress.
	 */
	if (didAnything) return;
    }
}

/* called to start listening for requests from a particular socket.
 * These are server requests.
 */
wlr_ServerListen(as, aprocp)
  int as;
  int (*aprocp)();
{
    int code;

    wlr_AsyncServerListen(as, aprocp);	/* does init, too */

    while (1) {
	code = Listen((struct wlr_call *) 0);
    }
}

/* called to start listening for requests from a particular socket.
 * These are server requests.
 */
wlr_AsyncServerListen(as, aprocp)
  int as;
  int (*aprocp)();
{
    int code;

    /* ensure proper initialization done */
    if (!wlr_Inited) wlr_Init();

    wlr_serverSocket = as;
    wlr_InterfaceProcp = aprocp;
    return 0;
}

/* start an async call. calls aclientp with opcode and parameters
 * as specifield by aopcode, aip* (int parms), ais* (string parms),
 * returning a call handle in acallp.
 * Returns an error code or 0.  Don't change *acallpp unless we
 * return success.
 * Data should already be marshalled into send packet.
 */
wlr_ACall(callp, opcode)
  struct wlr_call *callp;
  long opcode;
{
    long i, code;
    long xid;
    struct wlr_packet *sendp;

    sendp = callp->s;

    /* marshall parms in network byte order into sbuf */
    callp->xid = wlr_xidCounter++;
    sendp->d.flags = WLR_PACKET_TYPECALL;
    sendp->d.cid = callp->cid;
    sendp->d.xid = callp->xid;
    sendp->d.opcode = opcode;
    sendp->d.len = sizeof(sendp->d);
    wlr_htonPacket(sendp);

    /* send the message */
    code = write(callp->socket, &sendp->d, sizeof(sendp->d));
    if (code != sizeof(sendp->d))
	return (code < 0? errno : EIO);

    /* now, allocate a call ID and queue it so that someone can
     * wait for the response.  Place in queue so that response can
     * be properly sorted.
     */
    callp->states |= WLR_CALL_BUSY;
    wlr_Activate(callp);

    return 0;
}

/* get a call from a client structure, and init the call
 * for marshalling functions to work
 */
wlr_ABegin(clientp, callpp)
  struct wlr_client *clientp;
  struct wlr_call **callpp;
{
    struct wlr_call *tcallp;
    *callpp = tcallp = clientp->callp;
    if (!tcallp->s)
	tcallp->s = wlr_GetPacket();
    tcallp->mp = tcallp->s->d.buffer;
    return 0;
}

/* hold a reference to a call */
wlr_Hold(acallp)
  register struct wlr_call *acallp;
{
    acallp->refCount++;
    return 0;
}

/* activate a call: put it on the active list where we'll notice incoming
 * traffic on the FDs.  Server calls are always active, while client
 * calls are active only while there is a call outstanding.
 */
wlr_Activate(acallp)
  register struct wlr_call *acallp;
{
    if (acallp->states & WLR_CALL_ACTIVE) return 0;

    acallp->nextp = wlr_activeCallsp;
    wlr_activeCallsp = acallp;
    acallp->states |= WLR_CALL_ACTIVE;
    return 0;
}

/* deactivate a call by removing from the list to which incoming traffic
 * is dispatched.
 */
wlr_Deactivate(acallp)
  register struct wlr_call *acallp;
{
    struct wlr_call **lpp, *tp;

    if (!(acallp->states & WLR_CALL_ACTIVE))
	return 0;
    acallp->states &= ~WLR_CALL_ACTIVE;
    lpp = &wlr_activeCallsp;
    for(tp = *lpp; tp; lpp = &tp->nextp, tp = *lpp) {
	if (tp == acallp) {
	    *lpp = tp->nextp;
	    break;
	}
    }
    return 0;
}

/* Reset the state of a call.  If the call is a server call, we leave
 * it in the active chain so that we'll keep selecting on its FD, and
 * will notice new incoming calls.  If it is a client call, we should
 * deactivate it so that we don't keep polling on FDs that are guaranteed
 * to be idle.
 */
wlr_ResetCall(acallp)
  register struct wlr_call *acallp;
{
    acallp->states &= ~(WLR_CALL_BUSY | WLR_CALL_DONE);
    if (!(acallp->states & WLR_CALL_SERVER)) {
	wlr_Deactivate(acallp);
    }
#ifdef notdef
    /* when do we do this? */
    if (acallp->r) {
	wlr_FreePacket(acallp->r);
	acallp->r = 0;
    }
    if (acallp->s) {
	wlr_FreePacket(acallp->s);
	acallp->s = 0;
    }
#endif /* notdef */
    return 0;
}

/* drop a reference to a call structure */
wlr_Rele(acallp)
  register struct wlr_call *acallp;
{
    acallp->refCount--;
    return 0;
}

/* wait for call to complete, or fail.  Return parameters ap1-4.
 * Also, decrement reference count, so that wlr_ACall / wlr_AWait
 * pair will end up deleting reference.
 *
 * This function returns -1 if some error occurs, but otherwise
 * returns what the called function returns.
 */
wlr_AWaitCall(acallp)
  register struct wlr_call *acallp;
{
    long rcode;

    /* basic algorithm: see if we're happy with the call.  If we are,
     * just return the appropriate information.  If not, then we
     * call the Listen function, which does receives and returns when
     * a particular call either times out or returns properly.
     */
    if (!(acallp->states & WLR_CALL_BUSY))
	wlr_panic("wlr_AWait: inactive call");
    while (!(acallp->states & WLR_CALL_DONE)) {
	Listen(acallp);
    }

    /* if we  make it here, our call is done.  We return the approppriate
     * parameters and clean up the call.
     */
    rcode = acallp->r->d.code;

    /* finally, release the call and return the return code */
    wlr_ResetCall(acallp);
    return rcode;
}

/* wait for a client instead of a call */
wlr_AWait(acp)
  struct wlr_client *acp;
{
    return wlr_AWaitCall(wlr_CallOf(acp));
}


/* function that waits all calls in a client set, when
 * they're all done.  Returns first non-zero return code encountered,
 * if any.
 * Leaves output parameters around for unmarshalling.
 */
wlr_AWaitAll(cp)
  struct wlr_clientSet *cp;
{
    int i;
    struct wlr_client *tcp;
    struct wlr_call *tcallp;
    long rcode, code;

    /* can do better than this if there's some reason to wait
     * for the first call
     */
    rcode = 0;
    for(tcp = wlr_ClientSetFirst(cp); tcp; tcp = wlr_ClientSetNext(cp, tcp)) {
	tcallp = wlr_CallOf(tcp);
	wlr_AWaitCall(tcallp);
	if (code = tcallp->r->d.code) {
	    if (rcode == 0) rcode = code;
	}
	wlr_ResetCall(tcallp);
    }
    return rcode;
}

/* function that resets all calls in a client set.
 */
wlr_ResetAll(cp)
  struct wlr_clientSet *cp;
{
    struct wlr_client *tcp;

    /* can do better than this if there's some reason to wait
     * for the first call
     */
    for(tcp = wlr_ClientSetFirst(cp); tcp; tcp = wlr_ClientSetNext(cp, tcp)) {
	wlr_ResetCall(wlr_CallOf(tcp));
    }
    return 0;
}

/* functions to byte swap recv or send buffer, both directions */
wlr_ntohPacket(am)
  register struct wlr_packet *am;
{
    am->d.flags = ntohl(am->d.flags);
    am->d.xid = ntohl(am->d.xid);
    am->d.cid = ntohl(am->d.cid);
    am->d.code = ntohl(am->d.code);
    am->d.opcode = ntohl(am->d.opcode);
    am->d.len = ntohl(am->d.len);
    return 0;
}

wlr_htonPacket(am)
  register struct wlr_packet *am;
{
    am->d.flags = htonl(am->d.flags);
    am->d.xid = htonl(am->d.xid);
    am->d.cid = htonl(am->d.cid);
    am->d.code = htonl(am->d.code);
    am->d.opcode = htonl(am->d.opcode);
    am->d.len = htonl(am->d.len);
    return 0;
}

wlr_panic(astrp)
  char *astrp;
{
    fprintf(stderr,"PANIC: %s\n", astrp);
    exit(1);
}

/* patient version of read that reads this many bytes, waiting until
 * either an error occurs, or all the input arrives.
 * Needed since not all the bytes in the tcp stream will be guaranteed
 * to arrive at once.
 */
wlr_read(fd, buffer, bufSize)
  int fd;
  char *buffer;
  long bufSize;
{
    long code;

    /* read in small pieces */
    while (bufSize > 0) {
	code = read(fd, buffer, bufSize);
	if (code == EINTR) continue;
	if (code < 0) return errno;
	if (code == 0) return -1;	/* premature EOF */
	buffer += code;
	bufSize -= code;
    }
    return 0;	/* success, got all the bytes */
}
