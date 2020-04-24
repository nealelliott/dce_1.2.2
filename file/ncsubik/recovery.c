/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: recovery.c,v $
 * Revision 1.1.61.1  1996/10/02  17:55:41  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:42:41  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1990, 1995, 1996 Transarc Corporation - All rights reserved */
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/errno.h>

#include <dce/rpc.h>
#include <pthread.h>
#include <ubikdisk_proc.h>
#include <ubikvote_proc.h>
#include <dcedfs/dfsauth.h>
#include <dcedfs/icl.h>
#include <dcedfs/tpq.h>
#include <dcedfs/compat.h>
#include <dcedfs/bomb.h>
#define UBIK_INTERNALS
#include <ubik.h>
#include <utrace.h>

void upipe_FilePush(), upipe_Alloc(), upipe_FilePull();
extern struct icl_set *ubikdisk_iclSetp;
extern void ubik_hold(), ubik_rele(), ubik_versionLock(), ubik_versionUnlock();
PRIVATE int recovery_ResetState;

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/ncsubik/recovery.c,v 1.1.61.1 1996/10/02 17:55:41 damon Exp $")

/* This module is responsible for determining when the system has recovered to
 * the point that it can handle new transactions.  It replays logs, polls to
 * determine the current dbase after a crash, and distributes the new database
 * to the others. */

/* The sync site associates a version number with each database.  It broadcasts
 * the version associated with its current dbase in every one of its beacon
 * messages.  When the sync site send a dbase to a server, it also sends the
 * db's version.  A non-sync site server can tell if it has the right dbase
 * version by simply comparing the version from the beacon message
 * (uvote_dbVersion) with the version associated with the database
 * (ubik_dbase->version).  The sync site itself simply has one counter to keep
 * track of all of this (again ubik_dbase->version). */

/* sync site: routine called when the sync site loses its quorum; this
 * procedure is called "up" from the beacon package.  It resyncs the dbase and
 * nudges the recovery daemon to try to propagate out the changes.  It also
 * resets the recovery daemon's state, since recovery must potentially find a
 * new dbase to propagate out.  This routine should not do anything with
 * variables used by non-sync site servers.
 */ 
SHARED int urecovery_ResetState(wakeup)
int wakeup;	/* prod the recovery thread */
{
  urecovery_state = 0;
  ubik_dprint("sync site lost quorum; resyncing dbase, reset state\n");
  osi_Wakeup((opaque)&urecovery_state);
  if (wakeup) {
    recovery_ResetState = 1;
    osi_Wakeup((opaque)&recovery_ResetState);
  }
  return 0;
}

/* sync site: routine called when a non-sync site server goes down; restarts
 * recovery process to send missing server the new db when it comes back up.
 * This routine should not do anything with variables used by non-sync site
 * servers.
 */
SHARED int urecovery_LostServer() {  
  icl_Trace0(ubikdisk_iclSetp, UK_TRACE_RECOVERY_LOSTSERVER);
  urecovery_state &= ~UBIK_RECSENTDB;
  ubik_dprint("non sync-site server going down; restart recovery\n");
  osi_Wakeup((opaque) &urecovery_state);
  return 0;
}

/* return true iff we have a current database (called by both sync sites and
 * non-sync sites) How do we determine this?  If we're the sync site, we wait
 * until recovery has finished fetching and re-labelling its dbase (it may
 * still be trying to propagate it out to everyone else; that's THEIR problem).
 * If we're not the sync site, then we must have a dbase labelled with the
 * right version, and we must have a currently-good sync site.
 */
SHARED int urecovery_AllBetter(adbase, areadAny)
    IN  int areadAny;
    IN  struct ubik_dbase *adbase; {
    long rcode;

    ubik_dprint("allbetter checking\n");
    /* first, check if we're sync site and we've got the right data */
    rcode = 0;
    if (areadAny) return 1;		/* Happy with anything? All done. */
    else if (ubeacon_AmSyncSite() && (urecovery_state & UBIK_RECSBETTER))
	rcode = 1;
    /* next, check if we're aux site, and we've ever been sent the right data
     * (note that if a dbase update fails, we won't think that the sync site is
     * still the sync site, 'cause it won't talk to us until a timeout period
     * has gone by.  When we recover, we leave this clear until we get a new
     * dbase */
    else if (uvote_CheckSyncSite() &&	/* && order is important */
	     (vcmp(ubik_dbVersion, ubik_dbase->version) == 0))
	rcode = 1;

    ubik_dprint("allbetter: returning %d\n", rcode);
    return rcode;
}

/* abort all transactions on this database */
SHARED int urecovery_AbortAll(adbase)
  IN  struct ubik_dbase *adbase;
{
    struct ubik_trans *tt;
    icl_Trace0(ubikdisk_iclSetp, UK_TRACE_RECOVERY_ABORTALL);	      
    for(tt = adbase->activeTrans; tt; tt=tt->next) {
	udisk_abort(tt);
    }
    return 0;
}

/* this routine aborts the current remote transaction, if any, if the tid is
 * wrong */
SHARED int urecovery_CheckTid(atid)
    struct ubik_tid *atid;
{
  if (ubik_currentTrans) {
    /* there is remote write trans, see if we match, see if this is a new
     * transaction */
    if (atid->epoch != ubik_currentTrans->tid.epoch ||
	atid->counter > ubik_currentTrans->tid.counter) {
      /* don't match, abort it */
      icl_Trace4(ubikdisk_iclSetp, UK_TRACE_RECOVERY_CHECKTID,
		 ICL_TYPE_LONG, ubik_currentTrans->tid.epoch, 
		 ICL_TYPE_LONG, ubik_currentTrans->tid.counter,
		 ICL_TYPE_LONG, atid->epoch, ICL_TYPE_LONG, atid->counter);
      udisk_end(ubik_currentTrans);
      ubik_currentTrans = (struct ubik_trans *) 0;
    }
  }
  return 0;
}

/* log format is defined here, and implicitly in disk.c
 *
 * 4 byte opcode, followed by parameters, each 4 bytes long.  All integers
 * are logged in network standard byte order, in case we want to move logs
 * from machine-to-machine someday.
 *
 * Begin transaction: opcode
 * Commit transaction: opcode, version (8 bytes)
 * Truncate file: opcode, file number, length
 * Abort transaction: opcode
 * Write data: opcode, file, position, length, <length> data bytes
 *
 * A very simple routine, it just replays the log.  Note that this is a
 * new-value only log, which implies that no uncommitted data is written to the
 * dbase: one writes data to the log, including the commit record, then we
 * allow data to be written through to the dbase.  In our particular
 * implementation, once a transaction is done, we write out the pages to the
 * database, so that our buffer package doesn't have to know about stable and
 * uncommitted data in the memory buffers: any changed data while there is an
 * uncommitted write transaction can be zapped during an abort and the
 * remaining dbase on the disk is exactly the right dbase, without having to
 * read the log.
 */

/* replay logs */
static int ReplayLog(adbase)
    IN  struct ubik_dbase *adbase;
{
    long opcode;
    long code, tpos;
    int logIsGood;
    long len, thisSize, tfile, filePos;
    long buffer[4];
    long syncFile = -1;
    long data[1024];

    /* read the log twice, once to see whether we have a transaction to deal
	with that committed, (theoretically, we should support more than one
	trans in the log at once, but not yet), and once replaying the
	transactions.  */
    tpos = 0;
    logIsGood = 0;
    /* for now, assume that all ops in log pertain to one transaction; see if
     * there's a commit */
    while (1) {
	code = (*adbase->read)(adbase, LOGFILE, &opcode, tpos, sizeof(long));
	if (code)
	    break;
	if (opcode == LOGNEW) {
	    /* handle begin trans */
	    tpos += sizeof(long);
	}
	else if (opcode == LOGABORT) break;
	else if (opcode == LOGEND) {
	    logIsGood = 1;
	    break;
	}
	else if (opcode == LOGTRUNCATE) {
	    tpos += 4;
	    code = (*adbase->read)(adbase, LOGFILE, buffer, tpos,
				   2*sizeof(long));
	    if (code)
	        break;	/* premature eof or io error */
	    tpos += 2*sizeof(long);
	}
	else if (opcode == LOGDATA) {
	    tpos += 4;
	    code = (*adbase->read)(adbase, LOGFILE, buffer, tpos,
				   3*sizeof(long));
	    if (code)
	        break;
	    /* otherwise, skip over the data bytes, too */
	    tpos += buffer[2] + 3*sizeof(long);
	}
	else {
	    ubik_dprint("corrupt log opcode (%d) at position %d\n",
			opcode, tpos);
	    break;	/* corrupt log! */
	}
    }
    if (logIsGood) {
	/* actually do the replay; log should go all the way through the commit
         * record, since we just read it above. */
	tpos = 0;
	logIsGood = 0;
	syncFile = -1;
	while (1) {
	    code = (*adbase->read)(adbase, LOGFILE, &opcode, tpos,
				   sizeof(long));
	    if (code)
	        break;
	    if (opcode == LOGNEW) {
		/* handle begin trans */
		tpos += sizeof(long);
	    }
	    else if (opcode == LOGABORT) afsos_panic ("log abort");
	    else if (opcode == LOGEND) {
		tpos += 4;
		code = (*adbase->read) (adbase, LOGFILE, buffer, tpos,
					2*sizeof(long));
		if (code)
		    return UBADLOG;
		code = (*adbase->setlabel) (adbase, 0, buffer);
		if (code) 
		    return code;
		logIsGood = 1;
		break;	    /* all done now */
	    }
	    else if (opcode == LOGTRUNCATE) {
		tpos += 4;
		code = (*adbase->read)(adbase, LOGFILE, buffer, tpos,
				       2*sizeof(long));
		/* premature eof or io error */
		if (code)
		    break;
		tpos += 2*sizeof(long);
		code = (*adbase->truncate) (adbase, ntohl(buffer[0]),
					    ntohl(buffer[1]));
		if (code) 
		    return code;
	    }
	    else if (opcode == LOGDATA) {
		tpos += 4;
		code = (*adbase->read)(adbase, LOGFILE, buffer, tpos,
				       3*sizeof(long));
		if (code)
		    break;
		tpos += 3*sizeof(long);
		/* otherwise, skip over the data bytes, too */
		len = ntohl(buffer[2]);	    /* total number of bytes to copy */
		filePos = ntohl(buffer[1]);
		tfile = ntohl(buffer[0]);
		/* try to minimize file syncs */
		if (syncFile != tfile) {
		    if (syncFile >= 0)
			code = (*adbase->sync)(adbase, syncFile);
		    else 
		        code = 0;
		    syncFile = tfile;
		    if (code) return code;
		}
		while (len > 0) {
		    thisSize = (len > sizeof(data)? sizeof(data) : len);
		    /* copy sizeof(data) buffer bytes at a time */
		    code = (*adbase->read)(adbase, LOGFILE, data, tpos,
					   thisSize);
		    if (code) 
		        return UBADLOG;
		    code = (*adbase->write)(adbase, tfile, data, filePos,
					    thisSize);
		    if (code)
		        return UBADLOG;
		    filePos += thisSize;
		    tpos += thisSize;
		    len -= thisSize;
		}
	    }
	    else {
		ubik_dprint("corrupt log opcode (%d) at position %d\n",
			    opcode, tpos);
		break;	/* corrupt log! */
	    }
	}
	if (logIsGood) {
	    if (syncFile >= 0) code = (*adbase->sync)(adbase, syncFile);
	    if (code) return code;
	}
	else {
	    ubik_dprint("Log read error on pass 2\n");
	    return UBADLOG;
	}
    }

    /* now truncate the log, we're done with it */
    code = (*adbase->truncate)(adbase, LOGFILE, 0);
    return code;
}

/* Called at initialization to figure out version of the dbase we really have.
 * This routine is called after replaying the log; it reads the restored
 * labels. */
static InitializeDB(adbase)
    IN  struct ubik_dbase *adbase;
{
    long code;
    
    code = (*adbase->getlabel)(adbase, 0, &adbase->version);
    if (code) {
	/* try setting the label to a new value */
	adbase->version.epoch =	1;	/* value for newly-initialized db */
	adbase->version.counter = 1;
	code = (*adbase->setlabel) (adbase, 0, &adbase->version);
	if (code) {
	    /* failed, try to set it back */
	    adbase->version.epoch = 0;
	    adbase->version.counter = 0;
	    (*adbase->setlabel) (adbase, 0, &adbase->version);
	}
	osi_Wakeup((opaque) &adbase->version);
    }
    return 0;
}

/* initialize the local dbase
 * We replay the logs and then read the resulting file to figure out what
 * version we've really got.
 */
SHARED int urecovery_Initialize(adbase)
    struct ubik_dbase *adbase;
{
    long code;

    code = ReplayLog(adbase);
    if (code) return code;
    code = InitializeDB(adbase);
    return code;
}

typedef struct ubik_probe_args {
  struct ubik_server *ts;
  long index;
  error_status_t rpc_status;
} ubik_probe_args_t;

PRIVATE ubik_probe_args_t probe_args[UBIK_MAXSERVERS];
PRIVATE void *ubik_probe_parset_handle;	/* tpq probe set */
IMPORT void *ubik_tpq_handle;

PRIVATE void urecovery_send_probe(args)
     void *args;
{
  volatile int i;
  ubik_probe_args_t * volatile probe_args = (ubik_probe_args_t *)args;
  struct ubik_server * volatile ts = probe_args->ts;
  volatile struct in_addr inaddr;
  char *excNameP, excString[20];
  extern char *ExceptionToString();

  i=0;
  inaddr.s_addr = ts->addr;
  while(i<ts->bindingCount) {
    TRY {
      icl_Trace1(ubikdisk_iclSetp, UK_TRACE_SEND_PROBE,
                 ICL_TYPE_STRING, inet_ntoa(inaddr));
      UBIK_RPC_PREWRAP
	probe_args->rpc_status = UBIKDISK_Probe(ts->diskCid[i]);
      UBIK_RPC_POSTWRAP(probe_args->rpc_status)
      } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	  excNameP = excString;
	}
	icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		   ICL_TYPE_STRING,
		   excNameP, ICL_TYPE_STRING, __FILE__, 
		   ICL_TYPE_LONG,  __LINE__);
	probe_args->rpc_status = UEXCEPTION;
      } ENDTRY
	if ((probe_args->rpc_status >= rpc_s_mod) && 
	    (probe_args->rpc_status <= (rpc_s_mod+4096))) {
	  i++;
	  continue;
	}
	else break;
  } /* while */
  probe_args->index = i;
}

PRIVATE void urecovery_wait_for_probes(count)
     int count;
{
  int i;
  struct ubik_server *ts;
  struct in_addr inaddr;
  extern char *serverSecGroupP;

  ubik_rele();
  tpq_WaitParSet(ubik_probe_parset_handle);
  ubik_hold();

  for(i=0;i<count;i++) {
    ts = probe_args[i].ts;
    if (!(probe_args[i].rpc_status) && 
	probe_args[i].index) /*The first binding didn't succeed, shuffle*/
    ubik_shuffleBindings(ts, probe_args[i].index);

    inaddr.s_addr = ts->addr;
    if (probe_args[i].rpc_status == 0) {
      icl_Trace1(ubikdisk_iclSetp, UK_TRACE_RECOVERY_HOST_UP,
		 ICL_TYPE_STRING, inet_ntoa(inaddr));
      ts->up = 1;
      ts->ProviderVers = UBIK_PROVIDER_VERSION;
    }
    else {
      /* 
       * if server not in admin list of other servers, the probe may
       * fail with permission denied, we don't want to mark server
       * as up in this case, because all other RPCs would fail too.
       */
      if (probe_args[i].rpc_status == DAUT_ERROR_ACCESS_DENIED) {
	  /* print out a message for this error so admin can list list */
	  (void) fprintf(stderr, "ubik [security group %s]: server probe to %s was rejected due to authorization failure.  Check that host's admin list\n",
			 serverSecGroupP, inet_ntoa(inaddr));
      }
      icl_Trace2(ubikdisk_iclSetp, UK_TRACE_PROBE_FAIL,
                 ICL_TYPE_STRING, inet_ntoa(inaddr),
                 ICL_TYPE_LONG, probe_args[i].rpc_status);
      ubik_dprint("recovery probe server %s failed with code %lu\n",
		  inet_ntoa(inaddr), probe_args[i].rpc_status);
    }
  }
}

/* Main interaction loop for the recovery manager
 * The recovery light-weight process only runs when you're the synchronization
 * site.  It performs the following tasks, if and only if the prerequisite
 * tasks have been performed successfully (it keeps track of which ones have
 * been performed in its bit map, urecovery_state).
 *
 * First, it is responsible for probing that all servers are up.  This is the
 * only operation that must be performed even if this is not yet the sync site,
 * since otherwise this site may not notice that enough other machines are
 * running to even elect this guy to be the sync site.
 *
 * After that, the recovery process does nothing until the beacon and voting
 * modules manage to get this site elected sync site.
 *
 * After becoming sync site, recovery first attempts to find the best database
 * available in the network (it must do this in order to ensure finding the
 * latest committed data).  After finding the right database, it must fetch
 * this dbase to the sync site.
 *
 * After fetching the dbase, it relabels it with a new version number, to
 * ensure that everyone recognizes this dbase as the most recent dbase.
 *
 * One the dbase has been relabelled, this machine can start handling requests.
 * However, the recovery module still has one more task: propagating the dbase
 * out to everyone who is up in the network.
 */
SHARED void urecovery_Interact() {
    volatile long code;
    error_status_t st;
    struct ubik_server * volatile bestServer;
    struct ubik_server * volatile ts;
    volatile int dbok, i, j, replyCount, doneReset;
    volatile long lastProbeTime, lastDBVCheck;
    /* if we're the sync site, the best db version we've found yet */
    static struct ubik_version bestDBVersion;
    volatile struct ubik_version tversion;
    int timeout;
    volatile int length, file;
    volatile pipe_t tpipe;
    volatile struct ubik_pipeState tstate;
    volatile struct ubik_stat ubikstat;
    struct timespec delayTime, absTime;
    volatile struct ubik_version avers;
    char *excNameP, excString[20];
    long temp, probe_count;
    extern char *ExceptionToString();
    volatile struct in_addr inAddr;

    /* otherwise, begin interaction */
    ubik_hold();
    urecovery_state = 0;
    lastProbeTime = 0;
    lastDBVCheck = 0;
    while (1) {
	/* sleep for a little while */
	delayTime.tv_sec = 5;
	delayTime.tv_nsec = 0;
/*	ubik_rele();
	pthread_delay_np(&delayTime);
*/
	/* sleep until signalled or temp seconds elapse */
	pthread_get_expiration_np(&delayTime, &absTime);
	recovery_ResetState = timeout = 0;

	/*
	 * loop because we may get spurios wakeups where nobody woke us up
	 * and time limit hasn't expired.
	 */
	while (recovery_ResetState == 0 && !timeout) {
	  ubik_timedSleep((long)&recovery_ResetState, &absTime, 
			  &timeout);
	  ubik_hold();
	}
	/* local reset state */
	if (recovery_ResetState) {
	  recovery_ResetState = lastProbeTime = lastDBVCheck = 0;
	}

	ubik_dprint("recovery running in state %x\n", urecovery_state);
	inAddr.s_addr = ubik_host.addr;
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_RECOVERY_STATE,
		   ICL_TYPE_STRING, inet_ntoa(inAddr),
		   ICL_TYPE_LONG, urecovery_state);
	/* check that all servers are running */
	if (time(0) > 30 + lastProbeTime) {
	ubik_probe_parset_handle = tpq_CreateParSet(ubik_tpq_handle);
	if (ubik_probe_parset_handle == (void *)NULL) {
	  icl_Trace0(ubikdisk_iclSetp, 
		     UK_TRACE_CONCURR_PROBE_CREATEPARSET_FAIL);
	}
	else {
	for (probe_count=0,ts=ubik_servers; ts; ts=ts->next) { /*f*/
	  if (ts->up) continue; /* don't bother probing up servers */
	  else { /*e*/
	    for(i=0;i<ts->bindingCount;i++) {
	      rpc_binding_reset (ts->voteCid[i], &st);
	      rpc_binding_reset (ts->diskCid[i], &st); /* ignore errors */
	    }
	    probe_args[probe_count].ts = (struct ubik_server *)ts;
	    probe_args[probe_count].rpc_status = 0;
	    probe_args[probe_count].index = 0;
	    ubik_rele();
	    if (!tpq_AddParSet(ubik_probe_parset_handle,
			       urecovery_send_probe,
			       (void *)&(probe_args[probe_count]),
			       TPQ_MEDPRI,
			       UBIK_PROBE_GRACEPERIOD))
	      icl_Trace0(ubikdisk_iclSetp, 
			 UK_TRACE_CONCURR_PROBE_ADDPARSET_FAIL);
	    else
	      probe_count++;
	    ubik_hold();
	  }
	}
	urecovery_wait_for_probes(probe_count);
        }
	  lastProbeTime = time(0);
        }

	if (!ubeacon_AmSyncSite()) {
	    urecovery_state &= ~UBIK_RECSYNCSITE;
	    continue;		/* nothing to do */
	}
	else urecovery_state |= UBIK_RECSYNCSITE;
	icl_Trace1(ubikdisk_iclSetp,UK_TRACE_RECOVERY_SYNC_SITE, ICL_TYPE_LONG,
		   urecovery_state);
	/* check that the dbase didn't get trashed (not a local guarantee) */
	if (time(0) > 60 + lastDBVCheck) {
	    if (urecovery_state & UBIK_RECLABELDB) {
	        ubik_versionLock(ubik_dbase); 
		ubik_dprint("recovery about to get version\n");	        
	        replyCount=2;	/* count ourselves */
		if ( ubik_amIMagic ) replyCount++; /* magic breaks tie */
		for(ts=ubik_servers; ts; ts=ts->next) {
		    if (!ts->up) continue;  /* don't bother with these guys */
		    ubik_rele();
		    doneReset = i = 0;
		    while(i<ts->bindingCount) {

		      TRY {
			UBIK_RPC_PREWRAP
			  code = UBIKDISK_GetVersion(ts->diskCid[i], 
					 (struct net_version *)&bestDBVersion);
			UBIK_RPC_POSTWRAP(code)
		      } CATCH_ALL {
			excNameP = ExceptionToString(THIS_CATCH);
			if (excNameP == NULL) {
			  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
			  excNameP = excString;
			}
			icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
				   ICL_TYPE_STRING, excNameP, ICL_TYPE_STRING, 
				   __FILE__, ICL_TYPE_LONG, __LINE__);
			code = UEXCEPTION;
		      } ENDTRY
		      /*
		       * If it is an rpc failure, then there could be two 
		       * reasons for it. 
		       * 1. This is the wrong network interface to make the RPC
		       * 2. The callee has restarted or changed state that a
		       *    retry is in order.
		       */
		      if ((code >= rpc_s_mod) && (code <= (rpc_s_mod+4096))) {

			/* the fist time we hit an rpc error, we play it safe
			 * and reset our bindings. We don't bother to do it
			 * on each interface failure though.
			 */
			if (doneReset)
			  i++; /* go to next binding only after retry */
			else {
			  for(j=0;j<ts->bindingCount;j++) {
			    rpc_binding_reset (ts->voteCid[j], &st);
			    rpc_binding_reset (ts->diskCid[j], &st); 
			  }
			  doneReset++;
			}
		        continue;
		      }
		      else break;

		    }
		    ubik_hold();
		    if (!code && i)/*The first binding didn't succeed,shuffle*/
		      ubik_shuffleBindings(ts, i);
		    inAddr.s_addr = ts->addr;
		    if (code == 0) {
			/* keep track of good replies, server with lowest
		         * ip address breaks ties when number servers is even.
			 */
		        replyCount += 2; /* keep track of good replies */
			if ( ts->magic ) replyCount++; /* magic breaks tie*/
			if (vcmp(bestDBVersion, ts->version) != 0) {
			    icl_Trace3(ubikdisk_iclSetp, 
				       UK_TRACE_RECOVERY_BAD_VERSION,
				       ICL_TYPE_STRING, inet_ntoa(inAddr),
				       ICL_TYPE_LONG, ts->version.epoch,
				       ICL_TYPE_LONG, ts->version.counter);
			    /* mysteriously went bad (race at destination) */
			    ts->version = bestDBVersion;
			    ts->currentDB = 0;
			    urecovery_state &= ~UBIK_RECSENTDB;
			}
		    }
		    else {
		      icl_Trace2(ubikdisk_iclSetp, 
				UK_TRACE_RECOVERY_GET_VERSION_FAIL, 
				ICL_TYPE_STRING, inet_ntoa(inAddr),
				 ICL_TYPE_LONG, code);
		    }
		}
	      /* 
	       * We could use replyCount to check if we got to at least
	       * majority of servers, but is it really useful?
	       */
	      ubik_versionUnlock(ubik_dbase);
	    }
	    lastDBVCheck = time(0);
	}

	if (!(urecovery_state & UBIK_RECFOUNDDB)) {
	    /* now we're the sync site; find best dbase in system */
	    bestServer = (struct ubik_server *) 0;
	    bestDBVersion.epoch = 0;
	    bestDBVersion.counter = 0;
	    replyCount=2;	/* count ourselves */
	    if ( ubik_amIMagic ) replyCount++; /* magic breaks tie */
	    for(ts=ubik_servers; ts; ts=ts->next) {
	      if (!ts->up) continue; /* don't bother checking these guys */
	      ubik_rele();
	      doneReset=i=0;
	      while(i<ts->bindingCount) {

		TRY {
		  UBIK_RPC_PREWRAP
		    code = UBIKDISK_GetVersion(ts->diskCid[i], 
					   (struct net_version *)&avers);
		  UBIK_RPC_POSTWRAP(code)
		} CATCH_ALL {
		  excNameP = ExceptionToString(THIS_CATCH);
		  if (excNameP == NULL) {
		    sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
		    excNameP = excString;
		  }
		  icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
			     ICL_TYPE_STRING, excNameP, ICL_TYPE_STRING, 
			     __FILE__, ICL_TYPE_LONG, __LINE__);
		  code = UEXCEPTION;
		} ENDTRY
		/*
		 * If it is an rpc failure, then there could be two 
		 * reasons for it. 
		 * 1. This is the wrong network interface to make the RPC
		 * 2. The callee has restarted or changed state that a
		 *    retry is in order.
		 */
		  if ((code >= rpc_s_mod) && (code <= (rpc_s_mod+4096))) {
		    
		    /* the fist time we hit an rpc error, we play it safe
		     * and reset our bindings. We don't bother to do it
		     * on each interface failure though.
		     */
		    if (doneReset)
		      i++; /* go to next binding only after retry */
		    else {
		      for(j=0;j<ts->bindingCount;j++) {
			rpc_binding_reset (ts->voteCid[j], &st);
			rpc_binding_reset (ts->diskCid[j], &st); 
		      }
		      doneReset++;
		    }
		    continue;
		  }
		  else break;
	      }
		ubik_hold();
		ts->version = avers;
		if (!code && i) /* The first binding didn't succeed, shuffle */
		  ubik_shuffleBindings(ts, i);

		if (code == 0) {
		    replyCount += 2; /* count number good replies */
		    if ( ts->magic ) replyCount++; /* magic breaks tie */
		    /* perhaps this is the best version */
		    if (vcmp(ts->version, bestDBVersion) > 0) {
			/* new best version */
			bestDBVersion = ts->version;
			bestServer = ts;
		    }
		}
	    }

	    /* Unless we hear from at atleast a majority of servers, we cannot
	     * assume we found the latest database version. It could be that
	     * this server was down for a prolonged period, came back up along
	     * with at least a majority of servers (thereby achieving quorum)
	     * but those servers were somehow unable to give out their version
	     * numbers and returned failures from the GetVersion RPCs. By
	     * ignoring the code, we might erroneously determine that our
	     * database is the latest, when in fact it is not.
	     *
	     * All servers get two votes except the magic host, which gets
	     * three votes. The magic host breaks ties when there are an
	     * even number of hosts in the configuration.
	     */

	    if (replyCount > ubik_nServers) {
	      /* take into consideration our version */
	      if (vcmp(bestDBVersion, ubik_dbase->version) < 0) {
		bestDBVersion = ubik_dbase->version;
		bestServer = (struct ubik_server *) 0;
	      }
	      urecovery_state |= UBIK_RECFOUNDDB;
	      if (bestServer)
		  inAddr.s_addr = bestServer->addr;
	      else
		  inAddr.s_addr = ubik_host.addr;
	      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_RECOVERY_FOUND_DB, 
			ICL_TYPE_LONG, bestDBVersion.epoch,
			ICL_TYPE_LONG, bestDBVersion.counter,
			ICL_TYPE_STRING, 
			inet_ntoa(inAddr));
	    }
	    else {
	      urecovery_state &= ~UBIK_RECFOUNDDB;
	      icl_Trace2(ubikdisk_iclSetp, UK_TRACE_RECOVERY_NOT_FOUND_DB,
			 ICL_TYPE_LONG, replyCount,
			 ICL_TYPE_LONG, code);
	    }
	}

	/* now check if someone else has best version */
	if (!(urecovery_state & UBIK_RECFOUNDDB)) continue; /* not ready */
	ubik_dprint("recovery about to check version\n");
	if (!(urecovery_state & UBIK_RECHAVEDB)) {
	    if (bestServer && vcmp(ubik_dbase->version, bestDBVersion) < 0) {
		/* we don't have the best version; we should fetch it. */
	        ubik_versionLock(ubik_dbase);
		urecovery_AbortAll(ubik_dbase);
		ubik_dprint("recovery fetching new db from %x\n",
			    bestServer->addr);
		inAddr.s_addr = bestServer->addr;
		icl_Trace3(ubikdisk_iclSetp, UK_TRACE_RECOVERY_FETCH_DB, 
			   ICL_TYPE_LONG, bestDBVersion.epoch,
			   ICL_TYPE_LONG, bestDBVersion.counter,
			   ICL_TYPE_STRING, inet_ntoa(inAddr));
		file = 0;
		bzero((char *)&tpipe, sizeof(tpipe));
		tpipe.push = upipe_FilePush;
		tpipe.alloc = upipe_Alloc;
		tpipe.state = (char *) &tstate;
		tstate.offset = 0;
		tstate.allocBytes = 0;
		tstate.dbase = ubik_dbase;
		tstate.error = 0;
		/* give invalid label during file transit */
		tversion.epoch = 0;
		tversion.counter = 0;
		TRY {
		  (*ubik_dbase->setlabel)(ubik_dbase, file, &tversion);
		} CATCH_ALL {
		  excNameP = ExceptionToString(THIS_CATCH);
		  if (excNameP == NULL) {
		    sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
		    excNameP = excString;
		  }
		  icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
			     ICL_TYPE_STRING,excNameP, ICL_TYPE_STRING, 
			     __FILE__, ICL_TYPE_LONG, __LINE__);
		  code = UEXCEPTION;
		} ENDTRY
 		ubik_rele();

		doneReset=i=0;
		while(i<bestServer->bindingCount) {
		  TRY {
		    UBIK_RPC_PREWRAP
		      code = UBIKDISK_GetFile(bestServer->diskCid[i], file, 
					      (pipe_t *)&tpipe,
					      (struct net_version *)&tversion);
		    UBIK_RPC_POSTWRAP(code)
		  } CATCH_ALL {
		    excNameP = ExceptionToString(THIS_CATCH);
		    if (excNameP == NULL) {
		      sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
		      excNameP = excString;
		    }
		    icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
			       ICL_TYPE_STRING, excNameP, ICL_TYPE_STRING, 
			       __FILE__, ICL_TYPE_LONG, __LINE__);
		    code = UEXCEPTION;
		  } ENDTRY

		  if (!code)
		    code = tstate.error;
		  /*
		   * If it is an rpc failure, then there could be two 
		   * reasons for it. 
		   * 1. This is the wrong network interface to make the RPC
		   * 2. The callee has restarted or changed state that a
		   *    retry is in order.
		   */
		  if ((code >= rpc_s_mod) && (code <= (rpc_s_mod+4096))) {
		    
		    /* the fist time we hit an rpc error, we play it safe
		     * and reset our bindings. We don't bother to do it
		     * on each interface failure though.
		     */
		    if (doneReset)
		      i++; /* go to next binding only after retry */
		    else {
		      for(j=0;j<ts->bindingCount;j++) {
			rpc_binding_reset (ts->voteCid[j], &st);
			rpc_binding_reset (ts->diskCid[j], &st); 
		      }
		      doneReset++;
		    }
		    continue;
		  }
		  else break;
		}
		ubik_hold();
		if (!code && i) /* The first binding didn't succeed, shuffle */
		  ubik_shuffleBindings(bestServer, i);

		icl_Trace3(ubikdisk_iclSetp,UK_TRACE_RECOVERY_FETCH_DB_END,
			   ICL_TYPE_LONG, tversion.epoch,
			   ICL_TYPE_LONG, tversion.counter,
			   ICL_TYPE_LONG, code);
		if (code == 0) {
		    /* we got a new file, set up its header */
		    urecovery_state |= UBIK_RECHAVEDB;
		    ubik_dbase->version = tversion;
		    /* get data out first */
		    TRY {
		      (*ubik_dbase->sync)(ubik_dbase, 0);
		      /* after data is good, sync disk with correct label */
		      (*ubik_dbase->setlabel)(ubik_dbase, 0,
					      &ubik_dbase->version);
		    } CATCH_ALL {
		      excNameP = ExceptionToString(THIS_CATCH);
		      if (excNameP == NULL) {
			sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
			excNameP = excString;
		      }
		      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
				 ICL_TYPE_STRING, excNameP, ICL_TYPE_STRING,
				 __FILE__, ICL_TYPE_LONG, __LINE__);
		      code = UEXCEPTION;
		    } ENDTRY
 		}
		else {
		    ubik_dbase->version.epoch = 0;
		    ubik_dbase->version.counter = 0;
		}
		udisk_Invalidate(ubik_dbase, 0);	/* data has changed */
		osi_Wakeup((opaque) &ubik_dbase->version);
		ubik_versionUnlock(ubik_dbase);
	    }
	    else {
	      urecovery_state |= UBIK_RECHAVEDB;
	      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_RECOVERY_ALREADY_HAVE_DB,
			 ICL_TYPE_LONG, ubik_dbase->version.epoch,
			 ICL_TYPE_LONG, ubik_dbase->version.counter,
			 ICL_TYPE_LONG, urecovery_state);
	    }
	}
	
	/* mark our version with a brand new db version number, if need be */
	if (!(urecovery_state & UBIK_RECHAVEDB)) continue; /* not ready */
	if (!(urecovery_state & UBIK_RECLABELDB)) {
	    ubik_versionLock(ubik_dbase);
	    urecovery_AbortAll(ubik_dbase);
	    temp = time(0);
	    /*
	     * check for possible clock skews
	     */
	    if (temp < ubik_epochTime) { /* back skew */
	      ubik_versionUnlock(ubik_dbase);
	      continue;
	    }
	    ubik_epochTime = temp;	/* new dbase epoch */
	    ubik_dbase->version.epoch = ubik_epochTime;
	    ubik_dbase->version.counter = 1;
	    ubik_dprint("recovery labelling new version %d.%d\n",
			ubik_dbase->version.epoch,
			ubik_dbase->version.counter);
	    TRY {
	      code = (*ubik_dbase->setlabel) (ubik_dbase, 0,
					      &ubik_dbase->version);
	    } CATCH_ALL {
	      excNameP = ExceptionToString(THIS_CATCH);
	      if (excNameP == NULL) {
		sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
		excNameP = excString;
	      }
	      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, ICL_TYPE_STRING,
			 excNameP, ICL_TYPE_STRING, __FILE__, ICL_TYPE_LONG,
			 __LINE__);
	      code = UEXCEPTION;
	    } ENDTRY

 	    if (code == 0) {
	      urecovery_state |= UBIK_RECLABELDB;
	      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_RECOVERY_LABEL_DB, 
			 ICL_TYPE_LONG, ubik_dbase->version.epoch, 
			 ICL_TYPE_LONG, ubik_dbase->version.counter,
			 ICL_TYPE_LONG, 0);
	    }
	    else {
	      ubik_dprint("label operation failed %d\n", code);
	      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_RECOVERY_LABEL_DB,
			 ICL_TYPE_LONG, ubik_dbase->version.epoch, 
			 ICL_TYPE_LONG, ubik_dbase->version.counter,
			 ICL_TYPE_LONG, code);
	    }
	    udisk_Invalidate(ubik_dbase, 0);	/* data may have changed */
	    osi_Wakeup((opaque) &ubik_dbase->version);
	    ubik_versionUnlock(ubik_dbase);
	    ubik_dprint("recovery completed labelling\n");
	}

	if (!(urecovery_state & UBIK_RECLABELDB)) continue; /* not ready */
	if (!(urecovery_state & UBIK_RECSENTDB)) {
	    /* now propagate out new version to everyone else */
	    ubik_dprint("recovery propagating new version\n");
	    dbok = 1;	    /* start off assuming they all worked */
	    /*
	     * Check if a write transaction is in progress. We can't send the
	     * db when a write is in progress here because the db would be
	     * obsolete as soon as it goes there. Also, ops after the begin
	     * trans would reach the recepient and wouldn't find a transaction
	     * pending there.
	     */
	    ubik_versionLock(ubik_dbase);
	    while(ubik_dbase->flags & DBWRITING) {
	      icl_Trace0(ubikdisk_iclSetp, UK_TRACE_SLEEP_WAIT_FOR_WRITER);
	      ubik_sleep(ubik_dbase, (opaque) &(ubik_dbase->flags));
	      icl_Trace0(ubikdisk_iclSetp, UK_TRACE_READY_TO_SENDFILE);
	      ubik_versionLock(ubik_dbase);
	    }
	    for(ts=ubik_servers; ts; ts=ts->next) {
		inAddr.s_addr = ts->addr;
		if (!ts->up) {
		    ubik_dprint("recovery cannot send version to %s\n",	
				inet_ntoa(inAddr));
		    dbok = 0;
		    continue;
		}
		ubik_dprint("recovery sending version to %s\n",
			    inet_ntoa(inAddr));
		if (vcmp(ts->version, ubik_dbase->version) != 0) {
		    ubik_dprint("recovery stating local database\n");
		    TRY {
		      code = (*ubik_dbase->stat)(ubik_dbase, 0, &ubikstat);
		    } CATCH_ALL {
		      excNameP = ExceptionToString(THIS_CATCH);
		      if (excNameP == NULL) {
			sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
			excNameP = excString;
		      }
		      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
				 ICL_TYPE_STRING,excNameP, ICL_TYPE_STRING, 
				 __FILE__, ICL_TYPE_LONG, __LINE__);
		      code = UEXCEPTION;
		    } ENDTRY
 
		    if (!code) {
		      length = ubikstat.size;
		      file = 0;
		      tpipe.alloc = upipe_Alloc;
		      tpipe.pull = upipe_FilePull;
		      tpipe.state = (char *) &tstate;
		      tstate.xfrBytes = length;
		      tstate.offset = 0;
		      tstate.dbase = ubik_dbase;
		      tstate.error = 0;
		      ubik_rele();
		      ubik_dprint("recovery sending database to %s\n",
				  inet_ntoa(inAddr));
		      doneReset=i=0;	
		      while(i<ts->bindingCount) {

			TRY {
			  UBIK_RPC_PREWRAP
			    code=UBIKDISK_SendFile(ts->diskCid[i],file,length,
				   (struct net_version *)&ubik_dbase->version,
						   (pipe_t *)&tpipe);
			  UBIK_RPC_POSTWRAP(code)
			} CATCH_ALL {
			  excNameP = ExceptionToString(THIS_CATCH);
			  if (excNameP == NULL) {
			    sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
			    excNameP = excString;
			  }
			  icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
				   ICL_TYPE_STRING, excNameP, ICL_TYPE_STRING, 
				     __FILE__, ICL_TYPE_LONG, __LINE__);
			  code = UEXCEPTION;
			} ENDTRY

			if (!code)
			  code = tstate.error;/* record any pipe errors */
			/*
			 * If it is an rpc failure, then there could be two 
			 * reasons for it. 
			 * 1. This is the wrong network interface to make RPC
			 * 2. The callee has restarted or changed state that a
			 *    retry is in order.
			 */
			if ((code >= rpc_s_mod) && (code <= (rpc_s_mod+4096))){

			  /* the fist time we hit an rpc error, we play it safe
			   * and reset our bindings. We don't bother to do it
			   * on each interface failure though.
			   */
			  if (doneReset)
			    i++; /* go to next binding only after retry */
			  else {
			    for(j=0;j<ts->bindingCount;j++) {
			      rpc_binding_reset (ts->voteCid[j], &st);
			      rpc_binding_reset (ts->diskCid[j], &st); 
			    }
			    doneReset++;
			  }
			  continue;
			}
			else break;
		      }
		      ubik_hold();
		      if (!code && i) /* The first binding didn't succeed, 
					 shuffle */
			ubik_shuffleBindings(ts, i);
		    }
		    if (code == 0) {
		      icl_Trace2(ubikdisk_iclSetp, UK_TRACE_RECOVERY_SEND_DB, 
				 ICL_TYPE_STRING, inet_ntoa(inAddr),
				 ICL_TYPE_LONG, 0);
			/* we sent a new file, process its header */
		       ubik_dprint("recovery successfully sent new version\n");
			ts->version = ubik_dbase->version;
			ts->currentDB = 1;
		    }
		    else {
		      icl_Trace2(ubikdisk_iclSetp, UK_TRACE_RECOVERY_SEND_DB, 
				 ICL_TYPE_STRING, inet_ntoa(inAddr),
				 ICL_TYPE_LONG, code);
		      dbok = 0;
		      ubik_dprint("recovery failed to send new version, code %lu, pipe error %lu\n", code, tstate.error);
		    }
		}
		else {
		  /* mark file up to date */
		    ts->currentDB = 1;
		}
	    }
	    ubik_versionUnlock(ubik_dbase);
	    if (dbok) urecovery_state |= UBIK_RECSENTDB;
	}
    }
}
