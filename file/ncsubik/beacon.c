/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: beacon.c,v $
 * Revision 1.1.77.1  1996/10/02  17:54:56  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:42:26  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1990, 1996 Transarc Corporation - All rights reserved */
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#include <sys/types.h>
#include <sys/file.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <dcedfs/icl.h>
#include <dcedfs/compat.h>
#include <dcedfs/dfsauth.h>

#include <dce/rgybase.h>
#include <dce/pgo.h>
#include <dce/rpc.h>
#include <pthread.h>
#include <dce/rpcexc.h>
#include <stdio.h>
#include <dce/dce_cf.h>
#include <dce/binding.h>
#define UBIK_INTERNALS 1
#include <dcedfs/tpq.h>
#include <ubik_data.h>
#include <ubik.h>
#include <utrace.h>

extern struct icl_set *ubikvote_iclSetp;
extern void ubik_hold(), ubik_rele();
/* statics used to determine if we're the sync site */
PRIVATE long syncSiteUntil = 0;	/* valid only if amSyncSite */
SHARED int ubik_amSyncSite = 0;	/* flag telling if I'm sync site */
SHARED long ubik_nServers;	/* total number of servers */
SHARED long ubik_amIMagic=0;	/* is this host the magic host */
PRIVATE int beacon_ResetState;		/* beacon state reinitialized */

/* Module responsible for both deciding if we're currently the sync site,
 * and keeping collecting votes so as to stay sync site.
 *
 * The basic module contacts all of the servers it can, trying to get them to
 * vote for this server for sync site.  The vote request message (called a 
 * beacon message) also specifies until which time this site claims to be the 
 * sync site, if at all, thus enabling receiving sites to know how long the 
 * sync site guarantee is made for.
 *
 * Each  of these beacon messages is thus both a declaration of how long this 
 * site will remain sync site, and an attempt to extend that time by collecting
 * votes for a later sync site extension.
 *
 * The voting module is responsible for choosing a reasonable time until which
 * it promises not to vote for someone else.  This parameter (BIG seconds) is 
 * not actually passed in the interface (perhaps it should be?) but is instead
 * a compile time constant that both sides know about.
 
 * The beacon and vote modules work intimately together; the vote module 
 * decides how long it should promise the beacon module its vote, and the 
 * beacon module takes all of these votes and decides for how long it is the 
 * synchronization site.
 */

/* procedure called from debug rpc call to get this module's state for 
   debugging */
ubeacon_Debug(aparm)
register ubik_debug *aparm; {
    /* fill in beacon's state fields in the ubik_debug structure */
    aparm->syncSiteUntil = syncSiteUntil;
    aparm->nServers = ubik_nServers;
}

/*
 * ubeacon_ResetState
 *	reset all vote parameters so that a clean restart of the vote gathering
 * process can be done. Any inconsistencies noted by ubik will cause it to lose
 * quorum, and a clean restart of sending beacons.
 */
ubeacon_ResetState() {
  struct ubik_server *ts;
  extern struct ubik_server ubik_host;

  ubik_amSyncSite = 0;	/* flag telling if I'm sync site */
  for(ts = ubik_servers; ts; ts=ts->next) {
    ts->lastVoteTime = ts->lastBeaconSent = 0L;
    ts->lastVote = ts->up = ts->beaconSinceDown = (char)0;
  }
  ubik_host.lastVoteTime = ubik_host.lastBeaconSent = 0L;
  ubik_host.lastVote = ubik_host.up = ubik_host.beaconSinceDown = (char)0;
  beacon_ResetState = 1;
  (void)osi_Wakeup((opaque)&beacon_ResetState);
}

/* procedure that determines whether this site has enough current votes to 
 * remain sync site. called from higher-level modules (everything but the vote
 * module).
 *
 * If we're the sync site, check that our guarantees, obtained by the ubeacon_
 * Interact light-weight process, haven't expired.  We're sync site as long as
 * a majority of the servers in existence have promised us unexpired guarantees
 * The variable syncSiteUntil contains the time at which the latest of the
 * majority of the sync site guarantees expires (if variable ubik_amSyncSite 
 * is true). This module also calls up to the recovery module if it thinks that
 * the recovery module may have to pick up a new database (which occurs if we
 * lose the sync site votes).
 */
ubeacon_AmSyncSite() {
    register long now;
    register long rcode;
    
    /* special case for fast startup */
    if (ubik_nServers == 1) {
	ubik_dprint("beacon: amSyncSite is %d\n", 1);
	return 1;	/* one guy is always the sync site */
    }
    /* if I don't think I'm the sync site, say so */
    if (ubik_amSyncSite == 0) rcode = 0;  
    else {
	now = time(0);
	if (syncSiteUntil <= now) {	/* if my votes have expired, say so */
	    ubik_amSyncSite = 0;
	    rcode = 0;
	}
	else {
	    rcode = 1;		    	/* otherwise still have reqd votes */
	}
    }
    if (rcode == 0) urecovery_ResetState(0); /* force recovery to re-execute */
    ubik_dprint("beacon: amSyncSite is %d\n", rcode);
    return rcode;
}

/*
 * ubeacon_InitServerList
 *	 setup server list. bindingVectorP is list of other servers called
 *	 only at initialization to set up the list of servers to contact for
 *	 votes.  Just creates the server structure.  Note that there are two
 *	 connections in every server structure, one for vote calls 
 *	(which must always go through quickly) and one for database operations,
 *	 which are subject to waiting for
 *	locks.  If we used only one, the votes would sometimes get held up
 *	behind database operations, and the sync site guarantees would timeout
 *	even though the host would be up for communication.  The "magic" host
 *	is the one with the lowest internet address.  It is magic because its
 *	vote counts epsilon more than the others.  This acts as a tie-breaker
 *	when we have an even number of hosts in the system.  For example, if
 *	the "magic" host is up in a 2 site system, then it is sync site.
 *	Without the magic host hack, if anyone crashed in a 2 site system,
 *	we'd be out of business.
 * entry:
 *	ame - my internet address
 *	bindingVectorP - bindings for ubik server group
 *	secGroupP - name of security group to which all server principals must
 *		belong
 * exit:
 *	stP - return status;
 */


SHARED void ubeacon_InitServerList(ame, bindingVectorP, secGroupP, stP)
     IN  long ame;
     IN  ubik_binding_vector_t *bindingVectorP;
     IN  unsigned_char_t *secGroupP;
     OUT unsigned32 *stP;
{
    register int i,j;
    register struct ubik_server *ts, *prev, *tmpptr;
    unsigned long servAddr;
    unsigned long magicHost;
    struct ubik_server *magicServer;
    struct sockaddr_in taddr;
    rpc_binding_handle_t *selectedBinding;
    sec_rgy_bind_auth_info_t ubik_sec_auth_info;
    sec_rgy_handle_t rgyHandle = NULL;
    uuid_t cellUuid;
    uuid_t pUuid;

    char *fullpNameP = 0;
    char *pNameP = 0;
    char *firstCellNameP = 0;
    char *cellNameP = 0;
    char *nsCellName = 0;
    int authSetup = 0;
    long code = 0;
    void *adminCookie = (void *)0;
    error_status_t unusedSt;

    magicHost =	(unsigned long)ntohl(ame);/* do comparisons in host order */
    magicServer = (struct ubik_server *) 0;

    /* First get a handle to the cell registry, ubik_get_pname needs it */
    ubik_sec_auth_info.info_type = sec_rgy_bind_auth_dce;
    ubik_sec_auth_info.tagged_union.dce_info.authn_level = \
      rpc_c_authn_level_default;
    ubik_sec_auth_info.tagged_union.dce_info.authn_svc = \
      rpc_c_authn_dce_private;
    ubik_sec_auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    ubik_sec_auth_info.tagged_union.dce_info.identity = NULL;
    dce_cf_get_cell_name(&nsCellName, stP);
    if (*stP != dce_cf_st_ok) {
      return;
    }

    sec_rgy_site_bind ((unsigned_char_t *)nsCellName,
		       &ubik_sec_auth_info,
		       &rgyHandle,
		       stP);
    if (*stP != error_status_ok) {
      goto exit;
    }

    /* set up call to check membership in admin list */
    adminCookie = dfsauth_server_CheckAdminListAccessBegin(NULL);

    /* save copies of bindings for vote and disk interfaces */

    /* The outer loop creates a ubik_server structure for each ubik server that
     * we have to interact with. The inner loop copies the bindings from the
     * ubik_binding_vector structure into the diskCid and voteCid arrays. The
     * order of bindings in the voteCid and diskCid arrays are the same as that
     * in the ubik_binding_vector. The addr field in ubik_server stores the
     * lowest IP address (in network order) of all the bindings for a 
     * particular server. Each of these bindings is authenticated by making 
     * calls to the authentication sub-system.
     */

    for(i=0;i<bindingVectorP->count;i++) { /* outer loop, for each host */

	ts = (struct ubik_server *) malloc(sizeof(struct ubik_server));
	if (ts == (struct ubik_server *)NULL) {
	  *stP = UNOMEM;
	  goto exit;
	}
	bzero((char *)ts, sizeof(struct ubik_server));
	ts->next = ubik_servers;
	ubik_servers = ts;

	/* inner loop, for each binding, max UBIK_MAXADDR times */
	for(j=0;j<bindingVectorP->s[i].count;j++) { 
	  selectedBinding = (rpc_binding_handle_t *)&(bindingVectorP->s[i].\
						      binding_h[0]);
	  /* extract ip addr out of binding and save it */
	  code = rpcx_binding_to_sockaddr(selectedBinding[j], &taddr);
	  if (code) {
	    if (code == -1)
	      *stP = UINTERNAL;
	    else
	      *stP = code;
	    goto exit;
	  }

	  code = rpcx_ipaddr_from_sockaddr(&taddr, (char *)&servAddr);
	  if (code) {
	    if (code == -1)
	      *stP = UINTERNAL;
	    else
	      *stP = code;
	    goto exit;
	  }

	  /* 
	   * Store in the 'addr' field, the lowest IP address for this host.
	   * This has particular significance for multi-homed hosts which
	   * have multiple host bindings, and hence multiple IP addresses
	   * associated with it. The 'addr' field is primarily used in
	   * determining which host should act as a sync site, and would
	   * be the same on all ubik servers that interact.
	   */
	  if (ntohl(servAddr) < (unsigned long)ntohl(ts->addr) || 
	      ts->addr == 0)
	    ts->addr = servAddr;	/* network order */

	  rpc_binding_copy(selectedBinding[j], &(ts->diskCid[j]), stP);
	  if (*stP != rpc_s_ok)
	    goto exit;

	  rpc_binding_copy(selectedBinding[j], &(ts->voteCid[j]), stP);
	  if (*stP != rpc_s_ok)
	    goto exit;
	  ts->bindingCount++;

	  /* running with authentication, determine principal name */
	  ubik_get_pname_and_uuid(selectedBinding[j], rgyHandle, &fullpNameP,
				  &pNameP, &cellNameP, &pUuid, &cellUuid, stP);
	  if (*stP != rpc_s_ok)
	    goto exit;

	  /* perform consistency check on cellname */
	  if (strcmp(nsCellName, cellNameP) != 0) {
	    /* bindings for other ubik servers should be in our cell */
	    *stP = UBADCELL;
	    (void)free (firstCellNameP);
	    (void)free (cellNameP);
	    (void)free (fullpNameP);
	    (void)free(pNameP);
	    goto exit;
	  }
	  
	  if (ts->bindingCount == 1) {  /* first binding */
	    firstCellNameP = cellNameP;
	    ts->realm = cellUuid;
	    ts->principal = pUuid;
	  } else {
	    if (strcmp (firstCellNameP, cellNameP) != 0) {
	      *stP = UBADCELL;
	      (void)free (firstCellNameP);
	      (void)free (cellNameP);
	      (void)free (fullpNameP);
	      (void)free(pNameP);
	      goto exit;
	    }
	  }

	  /* make sure this principal is in this host's admin list */
	  if (adminCookie && 
	      !dfsauth_server_CheckAdminListAccessNext(adminCookie, &cellUuid,
						       &pUuid) &&
	      !dfsauth_server_CheckAdminListGroupAccess(adminCookie, rgyHandle,
							pNameP, fullpNameP)) {
	      /* bad, but not a fatal error */
	      (void) fprintf(stderr,
			     "ubik [security group %s]: principal %s not in this hosts's admin list\n",
			     secGroupP, pNameP);
	  }

	  ubik_auth_binding(ts->diskCid[j], rgyHandle, cellNameP, fullpNameP, 
			    pNameP, secGroupP, stP);
	  if (*stP != 0) {
	    (void)free (fullpNameP);
	    (void)free(pNameP);
            fprintf(stderr,
		    "Warning: ubik_auth_binding failed for host %s\n %s\n",
                    pNameP, dfs_dceErrTxt(*stP));
            ts->badhost=1;
            ts->up=0;
            *stP=0;
	    break;
	  }

	  ubik_auth_binding(ts->voteCid[j], rgyHandle, cellNameP, fullpNameP, 
			    pNameP, secGroupP, stP);
	  if (*stP != 0) {
	    (void)free (fullpNameP);
	    (void)free(pNameP);
            fprintf(stderr,
		    "Warning: ubik_auth_binding failed for host %s\n %s\n",
                    pNameP, dfs_dceErrTxt(*stP));
            ts->badhost=1;
            ts->up=0;
            *stP=0;
            break;
	  }

	  /* free principal name and cell name */
	  (void)free(fullpNameP); fullpNameP = 0;
	  (void)free(pNameP); pNameP = 0;

	  if (i)  /* don't free the first one as yet */
	    (void)free(cellNameP); cellNameP = 0;

	  ts->up = 1;
	  ts->ProviderVers = UBIK_PROVIDER_VERSION;
          ts->badhost=0;

	} /* for (j=0...) */
	if ((ntohl((unsigned long) ts->addr) < (unsigned long) magicHost) 
             && (!ts->badhost)){
	    magicHost = (unsigned long)ntohl(ts->addr); /* host order */
	    magicServer = ts;
	  }
      } /* for (i=0...) */

    if (firstCellNameP) {
      (void)free (firstCellNameP); 
      firstCellNameP = 0; /* free cell name we missed */
    }
    if (magicServer) 
      /* remember for when counting votes */
      magicServer->magic	= 1;
    else 
      ubik_amIMagic = 1;
    ubik_nServers = bindingVectorP->count+1;/* count this server and remotes */
    if (ubik_nServers == 1) {	/* special case 1 server */
	ubik_amSyncSite = 1;
	syncSiteUntil = 0x7fffffff; /* quite a while */
    }
    (void)free(nsCellName);
    if (adminCookie) 
	dfsauth_server_CheckAdminListAccessEnd(adminCookie);
    /* Remove mis-configured hosts from the list */
    for(ts=ubik_servers, prev= NULL, i=0; i< bindingVectorP->count; i++)    
      {
         if (ts->badhost){
              tmpptr=ts;
              if (!prev)
                  ubik_servers=ts->next;
              else
                  prev->next=ts->next;
              ts=ts->next;
              for(j=0;j<tmpptr->bindingCount; j++){
                  if (tmpptr->diskCid[j])
                      rpc_binding_free(&(tmpptr->diskCid[j]), stP);
                  if (tmpptr->voteCid[j])
                      rpc_binding_free(&(tmpptr->voteCid[j]), stP);
              }
              free(tmpptr);
	    }
          else{
             prev=ts;
             ts=ts->next;
	   }
       }
  
    return;

exit:
    if (rgyHandle) {
	sec_rgy_site_close(rgyHandle, &unusedSt);
    }
    if (adminCookie) 
	dfsauth_server_CheckAdminListAccessEnd(adminCookie);

    /* free the list of ubik servers - TBD */
    (void)free(nsCellName);
    return;
}

/*
 * arguments passed to beacon function called by tpq
 */
typedef struct ubik_beacon_args {
  rpc_binding_handle_t h[UBIK_MAXADDRS];
  long syncsite;
  long startTime;
  struct ubik_version version;
  struct ubik_tid ttid;
  long voteTime;
  error_status_t rpc_status;
} ubik_beacon_args_t;

/*
 * array of args to such routines. use only as much as required
 */
PRIVATE ubik_beacon_args_t beacon_args[UBIK_MAXSERVERS];
PRIVATE void *ubik_vote_parset_handle;	/* tpq beacon set */
IMPORT void *ubik_tpq_handle;


/*
 * ubeacon_send_beacon
 *	routine called by tpq for each element in the parset. Each routine
 * takes an argument that is used to make the beacon RPCs.
 */
PRIVATE void ubeacon_send_beacon(args)
  void *args;
{
  ubik_beacon_args_t * volatile bcon_args = (ubik_beacon_args_t *)args;
  int j;
  volatile int i = 0, doneReset = 0;
  char *excNameP, excString[20];
  error_status_t st;
  extern char *ExceptionToString();

  while (bcon_args->h[i]) {
    TRY {
      UBIK_RPC_PREWRAP
      bcon_args->rpc_status = UBIKVOTE_Beacon(bcon_args->h[i], 
					      bcon_args->syncsite,
					      bcon_args->startTime, 
					      &(bcon_args->version),
					      &(bcon_args->ttid), 
					      &(bcon_args->voteTime));
      UBIK_RPC_POSTWRAP(bcon_args->rpc_status)
    } CATCH_ALL {
      excNameP = ExceptionToString(THIS_CATCH);
      if (excNameP == NULL) {
	sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	excNameP = excString;
      }
      icl_Trace3(ubikvote_iclSetp, UK_TRACE_EXCEPTION, 
		 ICL_TYPE_STRING,
		 excNameP, ICL_TYPE_STRING, __FILE__, 
		 ICL_TYPE_LONG,  __LINE__);
      bcon_args->rpc_status = UEXCEPTION;
    } ENDTRY
    /*
     * If it is an rpc failure, then there could be two 
     * reasons for it. 
     * 1. This is the wrong network interface to make the RPC
     * 2. The callee has restarted or changed state that a
     *    retry is in order.
     */
    if ((bcon_args->rpc_status >= rpc_s_mod) && 
	(bcon_args->rpc_status <= (rpc_s_mod+4096))) {
      /* the fist time we hit an rpc error, we play it safe
       * and reset our bindings. We don't bother to do it
       * on each interface failure though.
       */
      if (doneReset)
	i++; /* go to next binding only after retry */
      else {
	for(j=0;bcon_args->h[j];j++)
	  rpc_binding_reset (bcon_args->h[j], &st);
	doneReset++;
      }
      continue;
    }
    else break;
  }	
}

/*
 * ubeacon_localBeacon
 *	Routine to collect our vote. This is done along with requesting votes
 * from other ubik servers.
 */
PRIVATE void ubeacon_local_beacon(args)
     void *args;
{
  ubik_beacon_args_t * volatile bcon_args = (ubik_beacon_args_t *)args;
  struct ubik_version avers;
  long syncsite;
  char *excNameP, excString[20];
  extern char *ExceptionToString();

  /* now call our own voter module to see if we'll vote for ourself.  
   * Note that the same restrictions apply for our voting for ourself as
   * for our voting for anyone else. 
   */
  TRY {
    bcon_args->rpc_status = SUBIKVOTE_Beacon((rpc_binding_handle_t)NULL,
					     bcon_args->syncsite,
					     bcon_args->startTime,
					     &(bcon_args->version),
					     &(bcon_args->ttid),
					     &(bcon_args->voteTime));
  } CATCH_ALL {
    excNameP = ExceptionToString(THIS_CATCH);
    if (excNameP == NULL) {
      sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
      excNameP = excString;
    }
    icl_Trace3(ubikvote_iclSetp, UK_TRACE_EXCEPTION, ICL_TYPE_STRING,
	       excNameP, ICL_TYPE_STRING, __FILE__, ICL_TYPE_LONG,
	       __LINE__);
    bcon_args->rpc_status = UEXCEPTION;
  } ENDTRY
  ubik_dprint("code from SUBIKVOTE_Beacon is %ld\n", bcon_args->rpc_status);
}

/*
 * ubeacon_wait_for_votes
 *	routine that waits for all elements in parset to complete execution. 
 * The results of each of these elements is analyzed to compute the votes
 * to be returned.
 */
PRIVATE ubeacon_wait_for_votes(servers, yesVotes, voteTime)
     struct ubik_server **servers;
     long *yesVotes, *voteTime;
{
  register int i;
  register struct ubik_server *ts;
  long temp;
  void *configSyncHandle;
  struct in_addr inaddr;

  *yesVotes = 0;	  	/* count how many to ensure we have quorum */
  *voteTime = 0x7fffffff; 	/* time quorum expires */

  ubik_rele();
  tpq_WaitParSet(ubik_vote_parset_handle);
  ubik_hold();
  for(i=0;;i++) {
    if (!servers[i])
      break;
    temp = time(0);	    /* now, more or less */
    ts = servers[i];
    ts->lastBeaconSent = temp;
    /* note that the vote time (the return code) represents the time
       the vote was computed, *not* the time the vote expires.  We 
       compute the latter down below if we got enough votes to go with */
    inaddr.s_addr = ts->addr;
    icl_Trace3(ubikvote_iclSetp, UK_TRACE_BEACON_GET_VOTE,
	       ICL_TYPE_LONG, beacon_args[i].voteTime,
	       ICL_TYPE_STRING, inet_ntoa(inaddr),
	       ICL_TYPE_LONG, beacon_args[i].rpc_status);	    
    if (beacon_args[i].rpc_status) {
      ts->up = 0;
      ts->beaconSinceDown = 0;
      urecovery_LostServer();
      ubik_dprint("code from UBIKVOTE_Beacon is %ld\n", 
		  beacon_args[i].rpc_status);
      ubik_dprint("time out from %08x\n", ts->addr);
      if (beacon_args[i].rpc_status == USKEWED 
	  ) {
	ubik_ResetState();
	*yesVotes = 0;
	*voteTime = 0x7fffffff;
	return;
      }
    }
    else if (beacon_args[i].voteTime == 0) {
      ts->lastVoteTime = temp;
      ts->lastVote = 0;
      ts->up = 1;
      ts->ProviderVers = UBIK_PROVIDER_VERSION;
      ts->beaconSinceDown = 1;
      ubik_dprint("code from UBIKVOTE_Beacon is %ld\n",
		  beacon_args[i].voteTime);
      ubik_dprint("no vote from %08x\n", ts->addr);
    }
    else {
      ts->lastVoteTime = beacon_args[i].voteTime;
      if (beacon_args[i].voteTime < *voteTime) 
	*voteTime = beacon_args[i].voteTime;
      ts->lastVote = 1;
      *yesVotes += 2;
      if (ts->magic) 
	*yesVotes += 1;  /* the extra epsilon */
      /* 
       * set server up (not really necessary: recovery does
       * this for real).
       */
      ts->up = 1;
      ts->ProviderVers = UBIK_PROVIDER_VERSION;
      ts->beaconSinceDown = 1;
      ubik_dprint("code from UBIKVOTE_Beacon is %ld\n", 
		  beacon_args[i].voteTime);
      ubik_dprint("yes vote from host %08x\n", ts->addr);
    }
  }

  /* now for the local beacon */
  if (beacon_args[i].rpc_status == 0 && beacon_args[i].voteTime) {
    *yesVotes += 2;
    if (ubik_amIMagic)
      *yesVotes += 1;	/* extra epsilon */
    if (beacon_args[i].voteTime < *voteTime) 
      *voteTime = beacon_args[i].voteTime;
  }
  return;
}

/*
 * ubeacon_Interact
 * main thread loop for code that sends out beacons.  This code only runs while
 * we're sync site or we want to be the sync site.  It runs in its very own 
 * light-weight process.
 */
SHARED void ubeacon_Interact() { 
    volatile long code;
    volatile rpc_binding_handle_t connections[UBIK_MAXSERVERS][UBIK_MAXADDRS];
    struct ubik_server * volatile servers[UBIK_MAXSERVERS];
    register int k;
    volatile int i, j;
    volatile int doneReset;
    register struct ubik_server *ts;
    int timeout;
    long temp;
    volatile long lastWakeupTime, yesVotes, oldestYesVote, syncsite;
    volatile struct ubik_tid ttid;
    volatile long startTime, voteTime;
    struct timespec delayTime, absTime;
    volatile struct ubik_version avers;
    char *excNameP, excString[20];
    error_status_t st;
    extern char *ExceptionToString();
    struct in_addr inAddr;

    bzero((char *)servers, UBIK_MAXSERVERS * sizeof(struct ubik_server *));
    bzero((char *)connections, 
	  UBIK_MAXSERVERS * UBIK_MAXADDRS * sizeof(rpc_binding_handle_t));
    ubik_hold();
    /* loop forever getting votes */
    lastWakeupTime = 0;	    /* keep track of time we last started a 
			       vote collection */
    while (1) {

	/* don't wakeup more than every POLLTIME seconds */
	temp = (lastWakeupTime + POLLTIME) - time(0);
	/* don't sleep if last collection phase took too long 
	   (probably timed someone out ) */
	if (temp > 0) {
	    if (temp > POLLTIME) temp = POLLTIME;
	    delayTime.tv_sec = temp;
	    delayTime.tv_nsec = 0;
	    /* sleep until signalled or temp seconds elapse */
	    pthread_get_expiration_np(&delayTime, &absTime);
	    beacon_ResetState = timeout = 0;

	    /*
	     * loop because we may get spurios wakeups where nobody woke us up
	     * and time limit hasn't expired.
	     */
	    while (beacon_ResetState == 0 && !timeout) {
	      ubik_timedSleep((long)&beacon_ResetState, &absTime, 
			      &timeout);
	      ubik_hold();
	    }
	    /* local reset state */
	    if (beacon_ResetState) {
	      beacon_ResetState = 0;
	    }
	}
	else code = 0;

	lastWakeupTime = time(0);   /* started a new collection phase */

	if (ubik_nServers == 1) continue; /* special-case 1 server for speedy 
					     startup*/

	if (!uvote_ShouldIRun()) {
	  continue;/* if voter has heard from a better candidate than us, 
		      don't bother running */
	}

	/* otherwise we should run for election, or we're the sync site 
	   (and have already won); send out the beacon packets */
	/* build list of all up hosts (noticing dead hosts are running again
	   is a task for the recovery module, not the beacon module), and
	   prepare to send them an r multi-call containing beacon message */
	i = 0;	    /* collect connections */
	for(ts = ubik_servers; ts; ts=ts->next) {
	    if (ts->up && ts->addr != ubik_host.addr) {
		servers[i] = ts;
		bcopy((char *)&(ts->voteCid[0]), (char *)&(connections[i][0]),
		      ts->bindingCount * sizeof(rpc_binding_handle_t));
		i++;
	    }
	}
	servers[i] = (struct ubik_server *) 0;	/* end of list */
	/* note that we assume in the vote module that we'll always get at 
	   least BIGTIME seconds of vote from anyone who votes for us, which 
	   means we can conservatively assume we'll be fine until SMALLTIME 
	   seconds after we start collecting votes */
	/* this next is essentially an expansion of rgen's ServBeacon routine*/

	ttid.epoch = ubik_epochTime;
	if (ubik_dbase->flags & DBWRITING)
	    /* 
	     * if a write is in progress, we have to send the writeTidCounter 
	     * which holds the tid counter of the write transaction , and not
	     * send the tidCounter value which holds the tid counter of the 
	     * last transaction.
	     */
	    ttid.counter = ubik_dbase->writeTidCounter;
	else
	    ttid.counter = ubik_dbase->tidCounter+1;
	icl_Trace1(ubikvote_iclSetp, UK_TRACE_TIDCOUNTER_BUMP,
		   ICL_TYPE_LONG, ttid.counter);
	/* now analyze return codes, counting up our votes */
	yesVotes = 0;		  /* count how many to ensure we have quorum */
	oldestYesVote = 0x7fffffff; /* time quorum expires */
	syncsite = ubeacon_AmSyncSite();
	/* multiple concurent RPC connections are no longer provided, now that
	 * we're using NCS.  We simply use an iterative implementation for now,
	 * but the real code will have to use N different threads, since we
	 * have serious timeout constraints that must be met in order to avoid
	 * losing quorum.
	 */
	startTime = time(0);
	ubik_vote_parset_handle = tpq_CreateParSet(ubik_tpq_handle);
	if (ubik_vote_parset_handle == (void *)NULL) {
	  icl_Trace0(ubikvote_iclSetp, 
		     UK_TRACE_CONCURR_BEACON_CREATEPARSET_FAIL);
	  continue;
	}

	for(i=0;; i++) {
	  if (!servers[i]) break;	/* all done */

	  /* Some network interfaces do not support all facilities (UDP etc).
	   * If we try a network interface and can't reach the server, we
	   * try the other interfaces too. Would this cause too much delay
	   * for ubik? We really need concurrent RPCs here.
	   */
	  bcopy((char *)connections[i], (char *)beacon_args[i].h,
		UBIK_MAXADDRS*sizeof(rpc_binding_handle_t));
	  beacon_args[i].syncsite = syncsite;
	  beacon_args[i].startTime = startTime;
	  beacon_args[i].version = ubik_dbase->version;
	  beacon_args[i].ttid = ttid;
	  beacon_args[i].voteTime = 0;
	  beacon_args[i].rpc_status = 0;
	  ubik_rele();
	  if (!tpq_AddParSet(ubik_vote_parset_handle,
			     ubeacon_send_beacon,
			     (void *)&(beacon_args[i]),
			     TPQ_HIGHPRI,
			     UBIK_VOTE_GRACEPERIOD))
	    icl_Trace0(ubikvote_iclSetp, 
		       UK_TRACE_CONCURR_BEACON_ADDPARSET_FAIL);
	  ubik_hold();
	}

	bzero((char *)beacon_args[i].h,
	      UBIK_MAXADDRS*sizeof(rpc_binding_handle_t));
	beacon_args[i].syncsite = ubeacon_AmSyncSite();
	beacon_args[i].startTime = startTime;
	beacon_args[i].version = ubik_dbase->version;
	beacon_args[i].ttid = ttid;
	beacon_args[i].voteTime = 0;
	beacon_args[i].rpc_status = 0;
	ubik_rele();
	if (!tpq_AddParSet(ubik_vote_parset_handle,
			   ubeacon_local_beacon,
			   (void *)&(beacon_args[i]),
			   TPQ_HIGHPRI,
			   UBIK_VOTE_GRACEPERIOD))
	  icl_Trace0(ubikvote_iclSetp, 
		     UK_TRACE_CONCURR_BEACON_ADDPARSET_FAIL);
	ubik_hold();
	ubeacon_wait_for_votes(servers, &yesVotes, &oldestYesVote);
	/* now decide if we have enough votes to become sync site.
	   Note that we can still get enough votes even if we didn't vote for 
	   ourself. */
	icl_Trace2(ubikvote_iclSetp, UK_TRACE_BEACON_QUORUM, 
		   ICL_TYPE_LONG, yesVotes,
		   ICL_TYPE_LONG, ubik_nServers);		       
	/* yesVotes is bumped by 2 or 3 for each site */
	if (yesVotes > ubik_nServers) {  
	    ubik_amSyncSite = 1;
	    temp = oldestYesVote + SMALLTIME;
	    if (syncSiteUntil < temp)
		syncSiteUntil = temp;
	    osi_Wakeup((opaque) &ubik_amSyncSite);
	}
	else {
	    /* tell recovery we're no longer the sync site */
	    ubik_amSyncSite = 0;
	    urecovery_ResetState(0); 
	}
    }	/* while loop */
}
