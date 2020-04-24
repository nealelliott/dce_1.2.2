/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1990, 1996 Transarc Corporation - All rights reserved */
/*
 * HISTORY
 * $Log: vote.c,v $
 * Revision 1.1.63.1  1996/10/02  17:56:34  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:04  damon]
 *
 * $EndLog$
 */
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#include <sys/types.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <dcedfs/icl.h>
#include <dcedfs/compat.h>
#include <dcedfs/tpq.h>
#include <pthread.h>
#include <dce/rpc.h>

#define UBIK_INTERNALS
#include <ubik.h>
#include <utrace.h>
#include <ubikvote_proc.h>
#include <ubikdisk_proc.h>

/*
    General Ubik Goal:
    The goal is to provide reliable operation among N servers, such that any 
    server can crash with the remaining servers continuing operation within a 
    short period of time. While a *short* outage is acceptable, this time 
    should be order of 3 minutes or less.
    
    Theory of operation:
    
    Note: SMALLTIME and BIGTIME are essentially the same time value, separated
    only by the clock skew, MAXSKEW.  In general, if you are making guarantees
    for someone else, promise them no more than SMALLTIME seconds of whatever 
    invariant you provide.  If you are waiting to be sure some invariant is now
    *false*, wait at least BIGTIME seconds to be sure that SMALLTIME seconds 
    has passed at the other site.

    Now, back to the design:
    One site in the collection is a special site, designated the *sync* site.
    The sync site sends periodic messages, which can be thought of as keep-
    alive messages.  When a non-sync site hears from the sync site, it knows 
    that it is getting updates for the next SMALLTIME seconds from that sync 
    site.
    
    If a server does not hear from the sync site in SMALLTIME seconds, it 
    determines that it no longer is getting updates, and thus refuses to give 
    out potentially out-of-date data.  If a sync site can not muster a majority
    of servers to agree that it is the sync site, then there is a possibility 
    that a network partition has occurred, allowing another server to claim to 
    be the sync site.  Thus, any time that the sync site has not heard from a 
    majority of the servers in the last SMALLTIME seconds, it voluntarily 
    relinquishes its role as sync site.
    
    While attempting to nominate a new sync site, certain rules apply.  First,
    a server can not reply "ok" (return 1 from ServBeacon) to two different 
    hosts in less than BIGTIME seconds; this allows a server that has heard 
    affirmative replies from a majority of the servers to know that no other 
    server in the network has heard enough affirmative replies in the last 
    BIGTIME seconds to become sync site, too.  The variables ubik_lastYesTime 
    and lastYesHost are used by all servers to keep track of which host they 
    have last replied affirmatively to, when queried by a potential new sync 
    site.
    
    Once a sync site has become a sync site, it periodically sends beacon
    messages with a parameter of 1, indicating that it already has determined 
    it is supposed to be the sync site.  The servers treat such a message as a
    guarantee that no other site will become sync site for the next SMALLTIME 
    seconds. In the interim, these servers can answer a query concerning which
    site is the sync site without any communication with any server.  The 
    variables lastBeaconArrival and lastBeaconHost are used by all servers to 
    keep track of which sync site has last contacted them.
    
    One complication occurs while nominating a new sync site: each site may be
    trying to nominate a different site (based on the value of lastYesHost),yet
    we must nominate the smallest host (under some order), to prevent this
    process from looping.  The process could loop by having each server
    give one vote to another server, but with no server getting a majority of
    the votes.  To avoid this, we try to withhold our votes for the server 
    with the lowest internet address (an easy-to-generate order).
    To this effect, we keep track (in lowestTime and lowestHost) of the lowest 
    server trying to become a sync site.  We wait for this server unless there
    is already a sync site (indicated by ServBeacon's parameter being 1).
*/

long ubik_debugFlag	= 0;		/* print out debugging messages? */

/* definition of event set structures for tracing */
extern struct icl_set *ubikvote_iclSetp;
IMPORT void *ubik_tpq_handle;

/* these statics are used by all sites in nominating new sync sites */
static long ubik_lastYesTime = 0;	/* time we sent the last *yes* vote */
static unsigned long lastYesHost = 0xffffffff; /* host to which we sent *yes* 
						  vote */

/*
 * This is for in preparation for the move to IPv6. Currently, the servers
 * use IP addresses to refer to each other in the voting process, and the
 * provider version 1 client interface uses IP addresses to locate the sync
 * site for update requests.
 *
 * Starting with provider version 2, clients prefer to identify servers by
 * their DCE realm and principal UUIDs, and use IP addresses only for
 * compatability with provider version 1 servers.
 *
 * Hopefully, by the time we need to switch to IPv6, most clients will be
 * at provider version 2, so we will be able to remove the IPv4 protocol 
 * from the servers without having to upgrade all of the clients. At least
 * we won't need to upgrade because of Ubik.
 *
 * These next fields are used to store the DCE identity of the sync site.
 */
static uuid_t lastYesRealm;
static uuid_t lastYesPrinc;

/* Next is time sync site began this vote: guarantees sync site until this 
   + SMALLTIME */
static long lastYesClaim = 0;
static int lastYesState = 0;    /* did last site we voted for claim to be sync
				   site? */

/* used to guarantee that nomination process doesn't loop */
static long lowestTime = 0;
static unsigned long lowestHost = 0xffffffff;
static long syncTime = 0;
static long syncHost = 0;

/* used to remember which dbase version is the one at the sync site 
   (for non-sync sites */
struct ubik_version ubik_dbVersion;	/* sync site's dbase version */
struct ubik_tid	ubik_dbTid;		/* sync site's tid, or 0 if none */

/* decide if we should try to become sync site.  The basic rule is that we 
   don't run if there is a valid sync site and it ain't us (we have to run if 
   it is us, in order to keep our votes). If there is no sync site, then we 
   want to run if we're the lowest numbered host running, otherwise we defer 
   to the lowest host.  However, if the lowest host hasn't been heard from for 
   a while, then we start running again, in case he crashed.
   This function returns true if we should run, and false otherwise.
 */
int uvote_ShouldIRun() {
    register long now;
    void *configSyncHandle;
    extern void ubik_DetectDeletion();

    now = time(0);
    if ((BIGTIME + ubik_lastYesTime) < now) {
      /*
       * we seem to have lost track of our sync site. Make sure there were
       * no deletions in ubik server group that caused this loss of quorum
       */
      icl_Trace1(ubikvote_iclSetp, UK_TRACE_INTERACT_NOBODY_ELSE,
		  ICL_TYPE_LONG, ubik_lastYesTime);
      return 1;    /* no valid guy even trying */
    }
    if (lastYesState && lastYesHost != ubik_host.addr) {
	icl_Trace3(ubikvote_iclSetp, UK_TRACE_INTERACT_SOMEBODY_ELSE,
		   ICL_TYPE_LONG, lastYesHost,
		   ICL_TYPE_LONG, lastYesState,
		   ICL_TYPE_LONG, ubik_lastYesTime);
	return 0; /* other guy is sync site, leave him alone */
    }
    if (((unsigned long) ntohl(lastYesHost)) < 
	((unsigned long) ntohl(ubik_host.addr))) {
	icl_Trace2(ubikvote_iclSetp, UK_TRACE_INTERACT_OTHER_BETTER,
		   ICL_TYPE_LONG, ntohl(lastYesHost),
		   ICL_TYPE_LONG, ntohl(ubik_host.addr));
	return 0;    /* if someone is valid and better than us, don't run */
    }
    /* otherwise we should run */
    icl_Trace0(ubikvote_iclSetp, UK_TRACE_INTERACT_WE_GO_NOW);
    return 1;
}

/*
 * uvote_ResetState
 *	reset the state variables so that we can restart the voting and quorum
 * process all over again,as if we we rerestarted. This is required in cases
 * such as when the clocks between any two ubik servers get skewed.
 */
long uvote_ResetState()
{
  icl_Trace0(ubikvote_iclSetp, UK_TRACE_RESET_RESETTING_CONFIG);
  ubik_lastYesTime = time(0); /* delay starting beacons by BIGTIME */
  lastYesHost = 0xffffffff;
  lastYesClaim = 0;
  lastYesState = 0;
  lowestTime = 0;
  lowestHost = 0xffffffff;
  syncTime = 0;
  syncHost = 0;
}

/*
 * uvote_CheckSyncSite
 *     returns true if the sync site is known, otherwise returns false.
 */
long uvote_CheckSyncSite()
{
    long now;
    if (lastYesState) {
	now = time(0);
	if ((SMALLTIME + lastYesClaim) >= now) {
	    /* We have a sync site */
	    return(1);
	}
    }
    return(0);
}

error_status_t
SUBIKVOTE_GetServerInterfaces (h, serverInterfacesP)
     register handle_t h;
     dfs_interfaceList *serverInterfacesP;
{
    error_status_t st;
    volatile int cancelState, cancelErr;
    char *excNameP, excString[20];
    extern char *ExceptionToString();

    DFS_DISABLE_CANCEL(&cancelState, cancelErr);
    ubik_hold();
    TRY {
        icl_Trace1(ubikvote_iclSetp, UK_TRACE_SUBIKVOTE_GET_SERVER_INTERFACES,
		   ICL_TYPE_POINTER, h);
        dfs_GetServerInterfaces (UBIKVOTE_v4_0_s_ifspec, serverInterfacesP, 
				 &st);
    } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	  excNameP = excString;
	}
	icl_Trace3(ubikvote_iclSetp, UK_TRACE_EXCEPTION, ICL_TYPE_STRING,
		   excNameP, ICL_TYPE_STRING, __FILE__, ICL_TYPE_LONG,
		   __LINE__);
	st = UEXCEPTION;
    } ENDTRY
    ubik_rele();
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
    return st;
}

/* called by the sync site to handle vote beacons; if aconn is null, this is a
   local call; returns 0 or time when the vote was sent.  It returns 0 if we 
   are not voting for this sync site, or the time we actually voted yes, if 
   non-zero.
 */
SUBIKVOTE_Beacon(rxcall, astate, astart, avers, atid, voteTime)
    register handle_t rxcall;
    volatile long astate;
    struct ubik_version *avers;
    struct ubik_tid *atid;
    long astart; 
    long * volatile voteTime;
{
    register int i;
    volatile unsigned long otherHost;
    register long now;
    volatile uuid_t otherRealm;
    volatile uuid_t otherPrinc;
    long vote;
    struct ubik_server * volatile ts;
    volatile struct in_addr inAddr;
    volatile int cancelState, cancelErr;
    char *excNameP, excString[20], ip1[20];
    static char rtnName[] = "SUBIKVOTE_Beacon";
    extern char *ExceptionToString();
    unsigned32 unusedSt;
    volatile long code=0;

    DFS_DISABLE_CANCEL(&cancelState, cancelErr);
    *voteTime=0;
    inAddr.s_addr = 0;
    now = time(0);				/* close to current time */
    if (rxcall && (code = ubik_CheckAuth(rxcall))) {
	goto end_Beacon;
    }
    /*
     * check for clock skew between servers; do not vote if skewed clock
     */
    if ((now>astart) && ((now-astart) > MAXSKEW)) {
      /* client clock is more than MAXSKEW behind us, or this is a slow RPC */
      icl_Trace1(ubikvote_iclSetp, UK_TRACE_CLOCK_SKEW,
		ICL_TYPE_LONG, (now-astart));
      /* ubik_ResetState(); */
      code = USKEWED;
      goto end_Beacon;
    }
    else if ((astart>now) && (astart-now)>MAXSKEW) {
      /* client clock is more than MAXSKEW ahead of us */
      icl_Trace1(ubikvote_iclSetp, UK_TRACE_CLOCK_SKEW,
		ICL_TYPE_LONG, (now-astart));
      ubik_ResetState();
      code = USKEWED;
      goto end_Beacon;
    }
    else if (ubik_lastYesTime > now) {
      /* our clock went backward */
      icl_Trace1(ubikvote_iclSetp, UK_TRACE_CLOCK_SKEW,
		 ICL_TYPE_LONG, ubik_lastYesTime-now);
      ubik_ResetState();
      code = USKEWED;
      goto end_Beacon;
    }
    else if ((ubik_lastYesTime > astart) && 
	     ((ubik_lastYesTime-astart)>MAXSKEW)) {
      /* client's clock is more than MAXSKEW behind when we last voted yes */
      /* or this RPC was slow */
      icl_Trace1(ubikvote_iclSetp, UK_TRACE_CLOCK_SKEW,
		 ICL_TYPE_LONG, ubik_lastYesTime-astart);
      /* ubik_ResetState(); */
      code = USKEWED;
      goto end_Beacon;
    }

    /*
     * We need to map the client binding handle onto the lowest IP
     * address for the host. Search for an entry in the ubik server
     * list (ubik_servers) that matches the client's identity. The lowest
     * IP address for the server is stored in the server's list entry.
     * This is the mechanism to guarantee that sync site determination process
     * is identical in all ubik servers that interact.
     */
    if (rxcall) {
      code = dfsauth_server_GetClientIdentity(rxcall, &otherRealm,
					      &otherPrinc);
      if (code) {
	goto end_Beacon;
      }
      ubik_hold();
      for(ts = ubik_servers ; ts ; ts=ts->next) {
	if (uuid_equal(&(ts->principal), (uuid_t *)&otherPrinc, &unusedSt) &&
            uuid_equal(&(ts->realm), (uuid_t *)&otherRealm, &unusedSt)) { 
	    otherHost = ts->addr;
	    inAddr.s_addr = otherHost;
	    break;
	}
      }

      if (!ts) { /* we didn't find it */
	/*
	 * We heard from a server we don't know about. This could be due to 
	 * one of two reasons.
	 *	- a malicious ubik server
	 *	_ a newly configured server
	 * We should verify that the server is newly configured, and if so
	 * reset our ubik state to incorporate the new server. If the server
	 * is not present in the configuration, then it has to be a malicious
	 * server and the vote would be rejected.
	 */
	code = UNOTMEMBER;
	ubik_dprint(
	 "Received beacon type %d from host %x that we don't know about time %d\n",
	 astate, otherHost, time(0));
	ubik_rele();
	icl_Trace2(ubikvote_iclSetp, UK_TRACE_SUBIKVOTE_BEACON_NOT_FOUND, 
		   ICL_TYPE_STRING, inet_ntoa(inAddr),
		   ICL_TYPE_LONG, *voteTime);
	icl_Trace1(ubikvote_iclSetp, UK_TRACE_SUBIKVOTE_BEACON_VERIFY_SERVER,
		  ICL_TYPE_STRING, dfs_dceErrTxt(code));
	goto end_Beacon;
      }	
    } else {
      /* 
       * This assignment is ok, because we already sorted the addr field to 
       * hold the lowest IP address among all the bindings for this server
       */
      ubik_hold();
      otherHost = ubik_host.addr;	/* this call is from this host */
      otherPrinc = ubik_host.principal; /* this server's identity */
      otherRealm = ubik_host.realm;     /* this server's realm */
      inAddr.s_addr = otherHost;
    }
    icl_Trace3(ubikvote_iclSetp, UK_TRACE_SUBIKVOTE_BEACON, 
	       ICL_TYPE_STRING, inet_ntoa(inAddr), 
	       ICL_TYPE_LONG, astate, ICL_TYPE_LONG, astart);
    if (avers && atid)
      icl_Trace4(ubikvote_iclSetp, UK_TRACE_SUBIKVOTE_BEACON_PARAMS,
		 ICL_TYPE_LONG, avers->epoch, ICL_TYPE_LONG, avers->counter,
		 ICL_TYPE_LONG, atid->epoch, ICL_TYPE_LONG, atid->counter);
    ubik_dprint("Received beacon type %d from host %x time %d\n", 
		astate, otherHost, now);

    /* compute the lowest server we've heard from.  We'll try to only vote for
       this dude if we don't already have a synchronization site.  Also, don't
       let a very old lowestHost confusing things forever.  We pick a new 
       lowestHost after BIGTIME seconds to limit the damage if this host 
       actually crashes.  Finally, we also count in this computation: don't 
       pick someone else if we're even better!
	
       Note that the test below must be <=, not <, so that we keep refreshing 
       lowestTime.  Otherwise it will look like we haven't heard from lowestHost
       in a while and another host could slip in.  */

    /* First compute the lowest host we've heard from, whether we want them
	for a sync site or not.  If we haven't heard from a site in BIGTIME
	seconds, we ignore its presence in lowestHost: it may have crashed.
	Note that we don't ever let anyone appear in our lowestHost if we're
	lower than them, 'cause we know we're up. */
    if (((unsigned long) ntohl(otherHost)) <= ((unsigned long) ntohl(lowestHost))
	|| (lowestTime + BIGTIME) < now) {
	lowestTime = now;
	lowestHost = otherHost;
    }
    /* why do we need this next check?  Consider the case where each of two 
       servers decides the other is lowestHost.  Each stops sending beacons 
       'cause the other is there.  Not obvious that this process terminates: 
       i.e. each guy could restart procedure and again think other side is 
       lowest.   Need to prove: if one guy in the system is lowest and knows 
       he's lowest, these loops don't occur. because if someone knows he's 
       lowest, he will send out beacons telling others to vote for him. */
    if (((unsigned long) ntohl(ubik_host.addr))
	<= ((unsigned long) ntohl(lowestHost))
	|| (lowestTime + BIGTIME) < now) {
	lowestTime = now;
	lowestHost = ubik_host.addr;
    }
    
    /* tell if we've heard from a sync site recently (even if we're not voting
       for this dude yet).  After a while, time the guy out. */
    if (astate) {   /* this guy is a sync site */
	if (syncTime <= now) {
	    syncHost = otherHost;
	    syncTime = now;
	}
    }
    if ((syncTime + BIGTIME) < now) {
	syncHost = 0;
	syncTime = 0;
    }

    /* decide how to vote */
    vote = 0;		    /* start off voting no */
    /* if we this guy isn't a sync site, we don't really have to vote for him.
       We get to apply some heuristics to try to avoid weird oscillation states
       in the voting procedure. */
    if (astate == 0) {
	/* in here only if this guy doesn't claim to be a sync site */

	/* lowestHost is also trying for our votes, then just say no. */
	if (lowestHost != otherHost && (lowestTime + BIGTIME) >= now) {
	  struct in_addr lowAddr;
	  lowAddr.s_addr = lowestHost;
	  *voteTime = 0;
	  ubik_rele();
	  strcpy(ip1, (char *)inet_ntoa(inAddr));
	  icl_Trace3(ubikvote_iclSetp, UK_TRACE_SUBIKVOTE_BEACON_NOT_LOWEST,
		     ICL_TYPE_STRING, ip1,
		     ICL_TYPE_STRING, inet_ntoa(lowAddr),
		     ICL_TYPE_LONG, *voteTime);
	  code = 0;
	  goto end_Beacon;
	}

	/* someone else *is* a sync site, just say no */
	if (syncHost && syncHost != otherHost) {
	  struct in_addr syncAddr;
	  *voteTime = 0;
	  syncAddr.s_addr = syncHost;

	  ubik_rele();
	  strcpy(ip1, (char *)inet_ntoa(inAddr));
	  icl_Trace3(ubikvote_iclSetp, UK_TRACE_SUBIKVOTE_BEACON_NOT_SYNC,
		     ICL_TYPE_STRING, ip1,
		     ICL_TYPE_STRING, inet_ntoa(syncAddr),
		     ICL_TYPE_LONG, *voteTime);
	  code = 0;
	  goto end_Beacon;
	}
    }
    TRY {
	/* Don't promise sync site support to more than one host every BIGTIME 
	   seconds. This is the heart of our invariants in this system. */
	/* Don't make ubik_lastYesTimeor lastYesClaim go backwards, though. */
	if ((ubik_lastYesTime <= now) && (lastYesClaim <= astart)
	    && ((ubik_lastYesTime + BIGTIME) < now || otherHost == lastYesHost)) {
	  vote = now;		     	/* vote yes */
	  ubik_lastYesTime = now;	/* remember when we voted yes */
	  lastYesClaim = astart;	/* remember for computing when 
					   sync site expires */
	  lastYesHost = otherHost;    	/* and who for */
	  lastYesRealm = otherRealm;
	  lastYesPrinc = otherPrinc;
	  lastYesState = astate;	/* remember if site is a sync site */
	  ubik_dbVersion = *avers;    	/* resync value */
	  ubik_dbTid = *atid;	    /* transaction id, if any,of active trans*/
	  icl_Trace1(ubikvote_iclSetp, UK_TRACE_CALL_CHECKTID,
		     ICL_TYPE_STRING, rtnName);
	  urecovery_CheckTid(atid); /* check if current write trans, if any, 
				       needs to be aborted */
	}
	*voteTime = vote;
    } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	  excNameP = excString;
	}
	icl_Trace3(ubikvote_iclSetp, UK_TRACE_EXCEPTION, ICL_TYPE_STRING,
		   excNameP, ICL_TYPE_STRING, __FILE__, ICL_TYPE_LONG,
		   __LINE__);
	code = UEXCEPTION;
	*voteTime=0;
    } ENDTRY
    
    ubik_rele();
end_Beacon:
    icl_Trace3(ubikvote_iclSetp, UK_TRACE_SUBIKVOTE_BEACON_EXIT, 
	       ICL_TYPE_STRING, inet_ntoa(inAddr),
	       ICL_TYPE_LONG, *voteTime,
	       ICL_TYPE_LONG, 0);
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
    return code;
}

/* handle per-server debug command, where 0 is the first server.  Basic 
   network debugging hooks. */
SUBIKVOTE_SDebug(rxcall, awhich, aparm)
    handle_t rxcall;
    long awhich;
    struct ubik_sdebug *aparm; {
    int done;
    done = SUBIKVOTE_SDebugV2(rxcall, awhich, aparm, NULL);
    return done;
}
SUBIKVOTE_SDebugV2(rxcall, awhich, aparm, aparm_v2)
    handle_t rxcall;
    long awhich;
    struct ubik_sdebug *aparm;
    struct ubik_sdebug_v2 *aparm_v2; {
    struct ubik_server *ts;
    int done=2, i;
    int cancelState, cancelErr;
    char *excNameP, excString[20];
    struct timespec delay;
    extern char *ExceptionToString();

    DFS_DISABLE_CANCEL(&cancelState, cancelErr);
    delay.tv_sec=5;
    delay.tv_nsec=0;
    for(i=0;i<UBIK_TRYLOCK_MAXTRIES;i++) {
      if (ubik_hold_no_block())
	break;
      (void)pthread_delay_np(&delay);
    }
    if (i==UBIK_TRYLOCK_MAXTRIES) {
      DFS_ENABLE_CANCEL(cancelState, cancelErr);
      return (UNOLOCK);
    }
    for(ts=ubik_servers; ts; ts=ts->next) {
	if (awhich-- == 0) {
	    /* we're done */
	    aparm->addr = (unsigned long)ntohl(ts->addr); 
	    aparm->lastVoteTime = ts->lastVoteTime;
	    aparm->lastBeaconSent = ts->lastBeaconSent;
	    *((struct ubik_version *) &aparm->remoteVersion) = ts->version;
	    aparm->lastVote = ts->lastVote;
	    aparm->up = ts->up;
	    aparm->beaconSinceDown = ts->beaconSinceDown;
	    aparm->currentDB = ts->currentDB;
	    if (aparm_v2 != NULL) {
		aparm_v2->providerVers = ts->ProviderVers;
		aparm_v2->realm = ts->realm;
		aparm_v2->principal = ts->principal;
	    }
	    done=0;
	    break;
	}
    }

    ubik_rele();
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
    return done;
}


/* handle basic network debug command.  This is the global state dumper */
SUBIKVOTE_Debug(rxcall, aparm)
    handle_t rxcall;
    register struct ubik_debug *aparm;
{
    long code;
    code = SUBIKVOTE_DebugV2(rxcall, aparm, NULL);
    return code;
}
SUBIKVOTE_DebugV2(rxcall, aparm, aparm_v2)
    handle_t rxcall;
    register struct ubik_debug *aparm; 
    register struct ubik_debug_v2 *aparm_v2;
{
    register int i;
    volatile int cancelState, cancelErr;
    char *excNameP, excString[20];
    extern char *ExceptionToString();
    volatile long code=0;
    struct timespec delay;

    /* fill in the basic debug structure */
    DFS_DISABLE_CANCEL(&cancelState, cancelErr);
    delay.tv_sec=5;
    delay.tv_nsec=0;
    for(i=0;i<UBIK_TRYLOCK_MAXTRIES;i++) {
      if (ubik_hold_no_block())
	break;
      (void)pthread_delay_np(&delay);
    }
    if (i==UBIK_TRYLOCK_MAXTRIES) {
      DFS_ENABLE_CANCEL(cancelState, cancelErr);
      return (UNOLOCK);
    }
    TRY {
    aparm->now = time(0);
    aparm->lastYesTime = ubik_lastYesTime;
    aparm->lastYesHost = (unsigned long)ntohl(lastYesHost);
    aparm->lastYesState = lastYesState;
    aparm->lastYesClaim = lastYesClaim;
    aparm->lowestHost = (unsigned long)ntohl(lowestHost);
    aparm->lowestTime = lowestTime;
    aparm->syncHost = (unsigned long)ntohl(syncHost);
    aparm->syncTime = syncTime;

    aparm->amSyncSite = ubik_amSyncSite;
    ubeacon_Debug(aparm);
    
    udisk_Debug(aparm);
    
    ulock_Debug(aparm);

    aparm->recoveryState = urecovery_state;
    *((struct ubik_version *) &aparm->syncVersion) = ubik_dbVersion;
    *((struct ubik_tid *) &aparm->syncTid) = ubik_dbTid;
    aparm->activeWrite = (ubik_dbase->flags & DBWRITING);
    aparm->tidCounter = ubik_dbase->tidCounter;
    
    if (ubik_currentTrans) {
	aparm->currentTrans = 1;
	if (ubik_currentTrans->type == UBIK_WRITETRANS) aparm->writeTrans = 1;
	else aparm->writeTrans = 0;
    }
    else {
	aparm->currentTrans = 0;
    }

    aparm->epochTime = ubik_epochTime;
    } CATCH_ALL {
      excNameP = ExceptionToString(THIS_CATCH);
      if (excNameP == NULL) {
	sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	excNameP = excString;
      }
      icl_Trace3(ubikvote_iclSetp, UK_TRACE_EXCEPTION, ICL_TYPE_STRING,
		 excNameP, ICL_TYPE_STRING, __FILE__, ICL_TYPE_LONG,
		 __LINE__);
      code = UEXCEPTION;
    } ENDTRY

    if (aparm_v2 != NULL) {
	aparm_v2->myAddr = (unsigned32)ntohl(ubik_host.addr);
	aparm_v2->myRealm = ubik_host.realm;
	aparm_v2->myPrinc = ubik_host.principal;
	aparm_v2->providerVers = UBIK_PROVIDER_VERSION;
	aparm_v2->lastYesRealm = lastYesRealm;
	aparm_v2->lastYesPrinc = lastYesPrinc;
    }

    ubik_rele();
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
    return code;
}


/* OLD FASHIONED WAY:
   Return the current synchronization site, if any.  Simple approach: if the 
   last guy we voted yes for claims to be the sync site, then we we're happy 
   to use that guy for a sync site until the time his mandate expires.  If the
   guy does not claim to be sync site, then, of course, there's none.
   In addition, if we lost the sync, we set urecovery_syncSite to an invalid 
   value, indicating that we no longer know which version of the dbase is the 
   one we should have.  We'll get a new one when we next hear from the sync 
   site.

   If we do not have a current sync site, then set the return host
   address to zero.
 */
SUBIKVOTE_GetSyncSite(rxcall, ahost)
    handle_t rxcall;
    unsigned long *ahost; {
    struct in_addr inAddr;
    int cancelState, cancelErr;
    long now;
    long code=0;

    DFS_DISABLE_CANCEL(&cancelState, cancelErr);
    ubik_hold();
    *ahost = 0;
    inAddr.s_addr = 0;
    if (lastYesState) {
	now = time(0);
	if ((SMALLTIME + lastYesClaim) >= now) {
	    /* We have a sync site */
	    *ahost = (unsigned long)ntohl(lastYesHost);
	    inAddr.s_addr = lastYesHost;
	}
    }
    icl_Trace3(ubikvote_iclSetp, UK_TRACE_SUBIKVOTE_GETSYNCSITE, 
	       ICL_TYPE_POINTER, rxcall, ICL_TYPE_STRING, inet_ntoa(inAddr),
	       ICL_TYPE_LONG, 0);
    ubik_rele();
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
    return code;
}

/* NEW WAY:

   Return the DCE realm and principal UUIDs for the current sync site.
   If we do not know the sync site, return UNOTSYNC.
*/
  
SUBIKVOTE_GetSyncSiteIdentity(rxcall, syncRealm, syncPrinc)
    handle_t rxcall;
    uuid_t *syncRealm;
    uuid_t *syncPrinc; {
    struct in_addr inAddr;
    int cancelState, cancelErr;
    long now;
    long code=UNOTSYNC;

    DFS_DISABLE_CANCEL(&cancelState, cancelErr);
    ubik_hold();
    inAddr.s_addr = 0;
    if (lastYesState) {
	now = time(0);
	if ((SMALLTIME + lastYesClaim) >= now) {
	    /* We have a sync site */
	    *syncRealm = lastYesRealm;
	    *syncPrinc = lastYesPrinc;
	    inAddr.s_addr = lastYesHost;
	    code =0;
	}
    }
    icl_Trace3(ubikvote_iclSetp, UK_TRACE_SUBIKVOTE_GETSYNCSITEIDENTITY,
	       ICL_TYPE_POINTER, rxcall, ICL_TYPE_STRING, inet_ntoa(inAddr),
	       ICL_TYPE_LONG, 0);
    ubik_rele();
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
    return code;
}

ubik_dprint(a,b,c,d,e,f,g,h)
    char *a, *b, *c, *d, *e, *f, *g, *h; {
    if (ubik_debugFlag) {
	printf(a, b, c, d, e, f, g, h);
    }
}

/* called once/run to init the vote module */
uvote_Init() {
    unsigned32 unusedSt;

    /* pretend we just voted for someone else, since we just restarted */
    ubik_lastYesTime = time(0);
    uuid_create_nil(&lastYesPrinc, &unusedSt);
    uuid_create_nil(&lastYesRealm, &unusedSt);
    return 0;
}
